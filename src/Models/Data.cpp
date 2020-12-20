#include "Data.h"

Bin Data::GetBinFromId(const int &id) {
    return *std::find(Bins.begin(), Bins.end(), id);
}

Entrance Data::GetEntranceFromId(const int &id) {
    return *std::find(Entrances.begin(), Entrances.end(), id);
}