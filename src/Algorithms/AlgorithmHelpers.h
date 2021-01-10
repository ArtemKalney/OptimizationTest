#pragma once

#include <Models/Bin.h>

class AlgorithmHelpers {
private:
    AlgorithmHelpers() = default;
public:
    static void GetBinsCombinations(const std::vector<Bin> &bins, std::vector<std::vector<Bin>> &combinations,
                                    std::vector<Bin> &combination, const int &offset, const int &k);
};