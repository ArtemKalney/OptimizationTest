#pragma once

class IPosition
{
public:
    virtual ~IPosition() = default;
    virtual int GetPosition() const = 0;
};