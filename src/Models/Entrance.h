#pragma once

#include "IIdentity.h"

class Entrance : public IIdentity, public IPosition {
private:
    int _id;
    int _position;
public:
    Entrance() = default;

    Entrance(const int &id, const int &position) :
            _id(id),
            _position{position} {}

    int GetId() const override {
        return _id;
    }

    int GetPosition() const override {
        return _position;
    }
};