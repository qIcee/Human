#pragma once

#include "data/nervousio.hpp"

class Organ {
public:
    virtual ~Organ() = default;

    virtual void update(double dt, const NervousOutput& nervous_output) = 0;
    virtual void status() const = 0;

};