#pragma once

#include "Stdafx.h"
#include "Bin.h"
#include "Entrance.h"

class Data {
public:
    std::vector<std::vector<double>> BinEntranceDistances;

    std::vector<std::vector<double>> BinDistances;

    std::vector<Bin> Bins;

    std::vector<Entrance> Entrances;

    double MaxDistance;

    int MaxBins;

    Bin GetBinFromId(const int &id);

    Entrance GetEntranceFromId(const int &id);
};