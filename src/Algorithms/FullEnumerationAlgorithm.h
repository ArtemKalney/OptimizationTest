#pragma once

#include <Models/Model.h>

class FullEnumerationAlgorithm {
private:
    Data _data;
public:
    FullEnumerationAlgorithm() = default;

    FullEnumerationAlgorithm(Data &data) : _data(data) {}

    void GetTanksCombinations(std::vector<std::vector<Bin>> &combinations,
                              std::vector<Bin> &combination, const int &offset, const int &k);

    std::shared_ptr<Model> GetMinModel();
};