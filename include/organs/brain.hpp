#pragma once

#include "organs/organ.hpp"

#include "data/brainio.hpp"
#include "data/nervousio.hpp"


struct BrainState {
    BrainInput state;
    // more to come
};



class Brain : public Organ {
public:
    BrainInput brain_input;
    BrainState brain_state;
    BrainOutput brain_output;


    Brain() = default;

    void update(double dt, const NervousOutput& nervous_output = NervousOutput{}) override;
    void status() const override;

    void input(const BrainInput& input);
    BrainOutput output() const;

    //double cardiac_output() const;
    double efferent_stimuli() const;
    double metabolic_demand() const;

};