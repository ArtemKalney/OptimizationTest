#include "GeneticAlgorithm.h"
#include "AlgorithmHelpers.h"

std::shared_ptr<Model> GeneticAlgorithm::GetMinModel() {
    _population = GetInitialPopulation();
    if (_population.empty()) {
        throw std::runtime_error("There ara no models satisfying the task conditions. Please check task conditions.");
    }
    auto minModelIt = std::min_element(_population.begin(), _population.end(), Model::ModelCompare);
    _currentMinModel = *minModelIt;
    if (IS_DEBUG == 1) {
        std::cout << "_currentMinModel:";
        PrintVector(_currentMinModel.GetSolution());
        std::cout << "ObjFunctionValue=" << _currentMinModel.GetObjFunctionValue() << std::endl << std::endl;
    }
    while (_currentMinModel.GetObjFunctionValue() > REQUIRED_ROUTE_DISTANCE) {
        GeneticAlgorithmIterations++;
        auto parent1Model = ChooseParent();
        if (IS_DEBUG == 1) {
            std::cout << "parent1Model:" << std::string(4, ' ');
            PrintVector(parent1Model.GetSolution());
        }
        auto parent2Model = ChooseParent();
        if (IS_DEBUG == 1 && parent1Model != parent2Model) {
            std::cout << "parent2Model:" << std::string(4, ' ');
            PrintVector(parent2Model.GetSolution());
        }
        while (parent1Model == parent2Model) {
            parent2Model = ChooseParent();
            if (IS_DEBUG == 1 && parent1Model != parent2Model) {
                std::cout << "parent2Model:" << std::string(4, ' ');
                PrintVector(parent2Model.GetSolution());
            }
        }
        auto crossedModel = CrossOperator(parent1Model, parent2Model);
        if (IS_DEBUG == 1) {
            // todo возможно надо модифицировать чтобы получать удовлетворяющие услвиям модели, либо не проверять модели вовсе
            if (!crossedModel.GetIsConditionsChecked()) {
                std::cout << "IsCheckConditions=false" << std::endl;
            }
            std::cout << "crossedModel:" << std::string(4, ' ');
            PrintVector(crossedModel.GetSolution());
        }
        auto mutatedModel = MutationOperator(crossedModel);
        if (IS_DEBUG == 1 && mutatedModel != crossedModel) {
            if (!crossedModel.GetIsConditionsChecked()) {
                std::cout << "IsCheckConditions=false" << std::endl;
            }
            std::cout << "mutatedModel:" << std::string(4, ' ');
            PrintVector(mutatedModel.GetSolution());
        }
        auto newMinModel = LocalDescentAlgorithm(mutatedModel);
        if (newMinModel.GetObjFunctionValue() < _currentMinModel.GetObjFunctionValue()) {
            _currentMinModel = newMinModel;
            if (IS_DEBUG == 1) {
                std::cout << "_currentMinModel:";
                PrintVector(newMinModel.GetSolution());
                std::cout << "ObjFunctionValue=" << _currentMinModel.GetObjFunctionValue() << std::endl << std::endl;
            }
        }
        auto maxModelIt = std::max_element(_population.begin(), _population.end(), Model::ModelCompare);
        _population.erase(maxModelIt);
        _population.push_back(newMinModel);
    }

    return std::make_shared<Model>(_currentMinModel);
}

//Выбираем случайно из всех возможных комбинаций
std::vector<Model> GeneticAlgorithm::GetInitialPopulation() {
    std::vector<std::vector<Bin>> combinations;
    std::vector<Bin> vector;
    if (USE_MAX_POPULATION_CAPACITY == 1) {
        AlgorithmHelpers::GetBinsCombinations(_data.Bins, combinations, vector, 0, _data.MaxBins);
    } else {
        for (int i = 1; i < _data.MaxBins; ++i) {
            vector.clear();
            AlgorithmHelpers::GetBinsCombinations(_data.Bins, combinations, vector, 0, i);
        }
    }
    std::vector<Model> models;
    for(auto &solution : combinations) {
        auto model = new Model(_data, solution);
        auto isConditionsChecked = model->GetIsConditionsChecked();
        if (isConditionsChecked && models.size() < MAX_INITIAL_POPULATION_COUNT) {
            models.push_back(*model);
        }
    }

    return models;
}

//Турнирная селекция
Model GeneticAlgorithm::ChooseParent() {
    srand(_seed++);
    std::vector<Model> vector;
    for(auto &item : _population) {
        // p=1/2
        if (rand() % 2) {
            vector.push_back(item);
        }
    }
    // если вдруг не выбрали ничего то берём первого из полпуляции
    if (vector.empty()) {
        return _population.front();
    }

    auto minModelIt = std::min_element(vector.begin(), vector.end(), Model::ModelCompare);

    return *minModelIt;
}

// Одноточечный оператор скрещивания
Model GeneticAlgorithm::CrossOperator(Model &parent1, Model &parent2) {
    srand(_seed++);
    auto position = rand() % _data.Bins.size();
    std::vector<Bin> vector;
    for(auto &item : parent1.GetSolution()) {
       if (item.GetPosition() <= position) {
           vector.push_back(item);
       }
    }
    for(auto &item : parent2.GetSolution()) {
        if (item.GetPosition() > position) {
            vector.push_back(item);
        }
    }
    auto model = new Model(_data, vector);

    return *model;
}

//Вероятностный оператор мутации
Model GeneticAlgorithm::MutationOperator(Model &model) {
    srand(_seed++);
    auto mutatedSolution = model.GetSolution();
    for(auto &item : _data.Bins) {
        // p=1/_data.Bins.size()
        if (rand() % _data.Bins.size() == 0) {
            auto it = std::find(mutatedSolution.begin(), mutatedSolution.end(), item);
            if (it != mutatedSolution.end()) {
                mutatedSolution.erase(it);
            } else {
                mutatedSolution.push_back(item);
            }
        }
    }
    auto mutatedModel = new Model(_data, mutatedSolution);

    return *mutatedModel;
}

std::vector<Model> GeneticAlgorithm::GetNeighborhood(Model &model) {
    std::vector<Model> neighborhood;
    for (int i = 1; i <= LOCAL_DISTANCE; ++i) {
        std::vector<std::vector<Bin>> combinations;
        std::vector<Bin> vector;
        AlgorithmHelpers::GetBinsCombinations(model.GetSolution(), combinations, vector, 0, model.GetSolution().size() - i);
        for(auto &item : combinations) {
            auto neighborModel = new Model(_data, item);
            if (neighborModel->GetIsConditionsChecked()) {
                neighborhood.push_back(*neighborModel);
            }
        }
        vector.clear();
        combinations.clear();
        AlgorithmHelpers::GetBinsCombinations(_data.Bins, combinations, vector, 0, i);
        for(auto &combination : combinations) {
            auto solution = model.GetSolution();
            for(auto &item : combination) {
                if (std::find(solution.begin(), solution.end(), item) == solution.end()) {
                    solution.push_back(item);
                }
            }
            if (solution.size() > model.GetSolution().size()) {
                auto neighborModel = new Model(_data, solution);
                if (neighborModel->GetIsConditionsChecked()) {
                    neighborhood.push_back(*neighborModel);
                }
            }
        }
    }

    return neighborhood;
}

Model GeneticAlgorithm::LocalDescentAlgorithm(Model &model) {
    auto neighborhood = GetNeighborhood(model);
    if (neighborhood.empty()) {
        throw std::runtime_error("Empty Neighborhood for model " + VectorToString(model.GetSolution()));
    }
    auto minModelIt = std::min_element(neighborhood.begin(), neighborhood.end(), Model::ModelCompare);
    if (minModelIt->GetObjFunctionValue() < model.GetObjFunctionValue()) {
        return LocalDescentAlgorithm(*minModelIt);
    }
    // Если ранее получили модель не удовлетворяющую условиям, то продолжаем поиск
    else if (!model.GetIsConditionsChecked()) {
        return LocalDescentAlgorithm(*minModelIt);
    }

    return model;
}