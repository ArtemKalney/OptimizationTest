#pragma once

#include <Models/Model.h>

class FullEnumerationAlgorithm {
private:
    Data _data;
public:
    FullEnumerationAlgorithm() = default;

    FullEnumerationAlgorithm(Data &data) : _data(data) {}

    std::shared_ptr<Model> GetMinModel();
};