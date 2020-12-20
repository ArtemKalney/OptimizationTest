#include <Models/Model.h>
#include "FullEnumerationAlgorithm.h"
#include <Globals.h>

void FullEnumerationAlgorithm::GetTanksCombinations(std::vector<std::vector<Bin>> &combinations,
                                                    std::vector<Bin> &combination, const int &offset, const int &k)
{
    if (k == 0)
    {
        combinations.push_back(combination);
        return;
    }

    for (int i = offset; i <= _data.Bins.size() - k; ++i)
    {
        combination.push_back(_data.Bins[i]);
        GetTanksCombinations(combinations, combination, i + 1, k - 1);
        auto res = *combination.end();
        combination.pop_back();
    }
}

std::shared_ptr<Model> FullEnumerationAlgorithm::GetMinModel() {
    std::vector<std::vector<Bin>> combinations;
    std::vector<Bin> vector;
    GetTanksCombinations(combinations, vector, 0, _data.MaxBins);
    std::vector<Model> models;
    for(auto &solution : combinations) {
        auto model = new Model(_data, solution);
        auto checkConditions = model->CheckConditions();
        if (checkConditions) {
            CheckedConditions++;
            models.push_back(*model);
        } else {
            UncheckedConditions++;
        }
        if (IS_DEBUG == 1) {
            std::cout << "CheckConditions=" << checkConditions << std::endl;
            model->PrintModel();
            std::cout << std::endl;
        }
    }
    auto minModelIt = std::min_element(models.begin(), models.end(), [this] (const Model &a, const Model &b) {
        return a.GetObjFunctionValue() < b.GetObjFunctionValue();
    });

    return std::make_shared<Model>(*minModelIt);
}