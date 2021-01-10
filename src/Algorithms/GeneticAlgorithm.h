#pragma once

#include "Globals.h"
#include <Models/Data.h>
#include <Models/Model.h>

class GeneticAlgorithm {
private:
    Data _data;
    int _seed = time(0);
    Model _currentMinModel;
    std::vector<Model> _population;
public:
    GeneticAlgorithm() = default;

    GeneticAlgorithm(Data &data) : _data(data) {}

    std::vector<Model> GetInitialPopulation();

    Model ChooseParent();

    Model CrossOperator(Model &parent1, Model &parent2);

    Model MutationOperator(Model &model);

    std::vector<Model> GetNeighborhood(Model &model);

    Model LocalDescentAlgorithm(Model &model);

    std::shared_ptr<Model> GetMinModel();
};