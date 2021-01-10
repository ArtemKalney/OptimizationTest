#include "Model.h"

void Model::FillX() {
    _X.resize(_data.Bins.size());
    for(auto &item : _data.Bins) {
        if (std::find(_solution.begin(), _solution.end(), item) != _solution.end()) {
            _X[item.GetPosition()] = 1;
        }
    }
}

// Пользуемся эвристикой "иди в ближайщий" для каждого подъезда
void Model::FillZ() {
    _Z.resize(_data.Bins.size(), std::vector<int>(_data.Entrances.size()));
    for (int i = 0; i < _data.Entrances.size(); i++) {
        auto position = GetMinDistancePosition(_data.BinEntranceDistances, i);
        if (position >= 0) {
            _Z[position][i] = 1;
        }
    }
}

bool Model::CheckConditions()
{
    for (int i = 0; i < _data.Entrances.size(); i++)
    {
        int sum = 0;
        for (int j = 0; j < _data.Bins.size(); j++)
        {
            sum += _Z[j][i];
        }
        if (sum != 1)
        {
            return false;
        }
    }

    for (int i = 0; i < _data.Bins.size(); i++)
    {
        for (int j = 0; j < _data.Entrances.size(); j++)
        {
            if (_X[i] < _Z[i][j])
            {
                return false;
            }
        }
    }

    if (std::accumulate(_X.begin(), _X.end(), 0) > _data.MaxBins)
    {
        return false;
    }

    for (int i = 0; i < _data.Bins.size(); i++)
    {
        for (int j = 0; j < _data.Entrances.size(); j++)
        {
            if (_data.BinEntranceDistances[i][j]*_Z[i][j] > _data.MaxDistance)
            {
                return false;
            }
        }
    }

    return true;
}

// Пользуемся эвристикой "иди в ближайщий" по очереди для каждого бака
void Model::FillObjFunctionValue(const Bin &bin, std::vector<bool> &visited) {
    visited[bin.GetPosition()] = true;
    auto position = GetMinDistancePosition(_data.BinDistances, bin.GetPosition(), visited);
    if (position >= 0) {
        _objFunctionValue += _data.BinDistances[position][bin.GetPosition()];
    }
    if (position >= 0 && !visited[position]) {
        FillObjFunctionValue(_data.Bins[position], visited);
        _route.push_back(bin);
    } else {
        _objFunctionValue += _data.BinDistances[_solution.front().GetPosition()][bin.GetPosition()];
        _route.push_back(_solution.front());
    }
}

int Model::GetMinDistancePosition(const std::vector<std::vector<double>> &distances, const int &searchIndex) {
    std::vector<bool> visited(_data.Bins.size());

    return GetMinDistancePosition(distances, searchIndex, visited);
}

// поиск позиции с минмальным расстоянием в матрице смежности по второму индексу, с запретом выбора вершин по первому
int Model::GetMinDistancePosition(const std::vector<std::vector<double>> &distances, const int &searchIndex,
                                  const std::vector<bool> &forbiddenNodes) {
    auto min = DBL_MAX;
    int position = -1;
    for(auto &item : _data.Bins) {
        auto distance = distances[item.GetPosition()][searchIndex];
        if (_X[item.GetPosition()] == 1 && min > distance && distance > 0 &&
                !forbiddenNodes[item.GetPosition()]) {
            min = distance;
            position = item.GetPosition();
        }
    }

    return position;
}

void Model::PrintModel() {
    std::cout << "Route:";
    PrintVector(GetRoute());
    std::cout << "ObjFunctionValue=" << GetObjFunctionValue() << std::endl;
}

bool operator ==(Model &firstElement, Model &secondElement) {
    auto firstSolution = firstElement.GetSolution();
    auto secondSolution = secondElement.GetSolution();
    for (int i = 0; i < firstSolution.size(); ++i) {
        if (firstSolution[i] == secondSolution[i]) {
            continue;
        } else {
            return false;
        }
    }

    return true;
}

bool operator !=(Model &firstElement, Model &secondElement) {
    if (firstElement == secondElement) {
        return false;
    }

    return true;
}