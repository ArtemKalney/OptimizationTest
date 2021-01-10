#include <Models/Model.h>
#include "FullEnumerationAlgorithm.h"
#include "AlgorithmHelpers.h"
#include <Globals.h>

std::shared_ptr<Model> FullEnumerationAlgorithm::GetMinModel() {
    std::vector<std::vector<Bin>> combinations;
    std::vector<Bin> vector;
    for (int i = 1; i < _data.MaxBins; ++i) {
        vector.clear();
        AlgorithmHelpers::GetBinsCombinations(_data.Bins, combinations, vector, 0, i);
    }
    std::vector<Model> models;
    for(auto &solution : combinations) {
        auto model = new Model(_data, solution);
        if (model->GetIsConditionsChecked()) {
            CheckedConditions++;
            models.push_back(*model);
            if (IS_DEBUG == 1) {
                model->PrintModel();
                std::cout << std::endl;
            }
        } else {
            UncheckedConditions++;
        }
    }
    if (models.empty()) {
        throw std::runtime_error("There ara no models satisfying the task conditions. Please check task conditions.");
    }
    auto minModelIt = std::min_element(models.begin(), models.end(), Model::ModelCompare);

    return std::make_shared<Model>(*minModelIt);
}