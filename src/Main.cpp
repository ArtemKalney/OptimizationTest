#include <Models/Data.h>
#include <Algorithms/FullEnumerationAlgorithm.h>
#include <Algorithms/GeneticAlgorithm.h>

std::ofstream output;
int CheckedConditions;
int UncheckedConditions;
int GeneticAlgorithmIterations;

Data GetData() {
    auto data = Data();
    boost::property_tree::ptree pt;
    boost::property_tree::read_json("data.json", pt);
    data.MaxDistance = std::stoi(pt.get_child("MaxDistance").data());
    data.MaxBins = std::stoi(pt.get_child("MaxBins").data());
    std::vector<Entrance> entrances;
    for (auto &item : pt.get_child("Entrances")) {
        auto entrance = new Entrance(item.second.get_value<int>(), entrances.size());
        entrances.push_back(*entrance);
    }
    data.Entrances = std::move(entrances);
    std::vector<Bin> bins;
    for (auto &item : pt.get_child("Bins")) {
        auto bin = new Bin(item.second.get_value<int>(), bins.size());
        bins.push_back(*bin);
    }
    data.Bins = std::move(bins);
    std::vector<std::vector<double>> binEntranceDistances(data.Bins.size(), std::vector<double>(data.Entrances.size()));
    for (auto &route : pt.get_child("BinEntranceDistances")) {
        Bin bin;
        Entrance entrance;
        double distance = 0;
        for (auto &item : route.second) {
            if (item.first == "start_id") {
                bin = data.GetBinFromId(item.second.get_value<int>());
            }
            if (item.first == "target_id") {
                entrance = data.GetEntranceFromId(item.second.get_value<int>());
            }
            if (item.first == "distance") {
                distance = item.second.get_value<double>();
            }
        }
        binEntranceDistances[bin.GetPosition()][entrance.GetPosition()] = distance;
    }
    data.BinEntranceDistances = std::move(binEntranceDistances);
    std::vector<std::vector<double>> binDistances(data.Bins.size(), std::vector<double>(data.Bins.size()));
    for (auto &route : pt.get_child("BinDistances")) {
        Bin bin1, bin2;
        double distance = 0;
        for (auto &item : route.second) {
            if (item.first == "start_id") {
                bin1 = data.GetBinFromId(item.second.get_value<int>());
            }
            if (item.first == "target_id") {
                bin2 = data.GetBinFromId(item.second.get_value<int>());
            }
            if (item.first == "distance") {
                distance = item.second.get_value<double>();
            }
        }
        binDistances[bin1.GetPosition()][bin2.GetPosition()] = distance;
    }
    data.BinDistances = std::move(binDistances);

    return data;
}

void OutputResult(const std::shared_ptr<Model> model, int startTime) {
    output << "Solution:";
    for(auto &item : model->GetSolution()) {
        output << item.GetId();
        if (item == model->GetSolution().back()) {
            continue;
        } else {
            output << ",";
        }
    }
    output << std::endl << "Route:";
    for(auto &item : model->GetRoute()) {
        output << item.GetId();
        if (item == model->GetRoute().back()) {
            continue;
        } else {
            output << ",";
        }
    }
    output << std::endl;
    output << "ObjFunctionValue:" << model->GetObjFunctionValue() << std::endl;
    output << "Statistics:" << std::endl;
    output << std::string(3, ' ') << "Time:" << clock() - startTime << std::endl;
    if (IS_FULL_ENUMERATION_ALGORITHM == 1) {
        output << std::string(3, ' ') << "CheckedConditions:" << CheckedConditions << std::endl;
        output << std::string(3, ' ') << "UncheckedConditions:" << UncheckedConditions << std::endl;
    }
    if (IS_GENETIC_ALGORITHM == 1) {
        output << std::string(3, ' ') << "GeneticAlgorithmIterations:" << GeneticAlgorithmIterations << std::endl;
    }
}

void HandleException(const std::exception &e) {
    std::cerr << e.what() << std::endl;
    output << e.what() << std::endl;
}

int main() {
    output.open("output.txt");
    Data data;
    try {
        data = GetData();
        for (int i = 0; i < data.Entrances.size(); i++) {
            auto min = DBL_MAX;
            for(auto &item : data.Bins) {
                auto distance = data.BinEntranceDistances[item.GetPosition()][i];
                if (min > distance) {
                    min = distance;
                }
            }
            if (min > data.MaxDistance) {
                throw std::runtime_error("MaxDistance is too small for " + std::to_string(i) + " entrance");
            }
        }
    }
    catch (std::exception const &e) {
        HandleException(e);

        return EXIT_FAILURE;
    }
    std::shared_ptr<Model> minModel;
    int startTime = clock();
    try {
        if (IS_FULL_ENUMERATION_ALGORITHM == 1) {
            auto fullEnumerationAlgorithm = new FullEnumerationAlgorithm(data);
            minModel = fullEnumerationAlgorithm->GetMinModel();
        } else if (IS_GENETIC_ALGORITHM == 1) {
            auto geneticAlgorithm = new GeneticAlgorithm(data);
            minModel = geneticAlgorithm->GetMinModel();
        }
        if (!minModel->GetIsConditionsChecked()) {
            throw std::runtime_error("Solution does not satisfy conditions");
        }
    }
    catch (std::exception const &e) {
        HandleException(e);

        return EXIT_FAILURE;
    }
    OutputResult(minModel, startTime);
    output.close();

    return EXIT_SUCCESS;
}