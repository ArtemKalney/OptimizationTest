#include "AlgorithmHelpers.h"

void AlgorithmHelpers::GetBinsCombinations(const std::vector<Bin> &bins, std::vector<std::vector<Bin>> &combinations,
                                           std::vector<Bin> &combination, const int &offset, const int &k)
{
    if (k == 0)
    {
        combinations.push_back(combination);
        return;
    }

    for (int i = offset; i <= bins.size() - k; ++i)
    {
        combination.push_back(bins[i]);
        GetBinsCombinations(bins, combinations, combination, i + 1, k - 1);
        combination.pop_back();
    }
}