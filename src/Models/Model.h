#pragma once

#include "Data.h"

class Model {
private:
    Data _data;
    std::vector<Bin> _solution;
    std::vector<int> _X;
    std::vector<std::vector<int>> _Z;
    double _objFunctionValue;
    std::vector<Bin> _route;
public:
    Model() = default;

    Model(Data &data, std::vector<Bin> &solution) :
            _data(data),
            _solution(solution),
            _objFunctionValue(0) {
        FillX();
        FillZ();
        std::vector<bool> visited(_data.Bins.size());
        FillObjFunctionValue(_solution.front(), visited);
    }

    double GetObjFunctionValue() const {
        return _objFunctionValue;
    }

    std::vector<Bin> &GetRoute() {
        return _route;
    }

    void FillX();

    void FillZ();

    void FillObjFunctionValue(const Bin &bin, std::vector<bool> &visited);

    int GetMinDistancePosition(const std::vector<std::vector<double>> &distances, const int &searchIndex);

    int GetMinDistancePosition(const std::vector<std::vector<double>> &distances, const int &searchIndex,
                               const std::vector<bool> &forbiddenNodes);

    void PrintModel();

    bool CheckConditions();
};