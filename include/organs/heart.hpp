#pragma once

#include <iostream>

#include "organs/organ.hpp"

struct HeartState {
    double heart_rate;
    double stroke_volume;
};

class Heart : public Organ {
public:
    HeartState state;

    Heart();
    Heart(double hf, double sv);

    void update(double dt, const NervousOutput& nervous_output) override;
    void status() const override;

    double cardiac_output() const;

};