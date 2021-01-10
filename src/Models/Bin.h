#pragma once

#include "Stdafx.h"
#include "IIdentity.h"
#include "IPosition.h"

class Bin : public IIdentity, public IPosition {
private:
    int _id;
    // Из-за того что полная матрица смежности велика, запоминаем позиции элементов в матрицах меньшей размерности
    int _position;
public:
    Bin() = default;

    Bin(const int &id, const int &position) :
            _id(id),
            _position{position} {}

    int GetId() const override {
        return _id;
    }

    int GetPosition() const override {
        return _position;
    }
};