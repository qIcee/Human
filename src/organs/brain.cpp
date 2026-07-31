#include <cmath>

#include "organs/brain.hpp"

double Brain::metabolic_demand() const {
    return brain_state.state.nervous_input.sum_metabolic_demand;
}

double Brain::efferent_stimuli() const {
    double alpha = 1.0;
    double beta = 1.0;
    double damping = 0.0; // be changed later


    return std::tanh(alpha * metabolic_demand() - beta * damping);
}

void Brain::input(const BrainInput& input) {
    brain_input = input;
}

BrainOutput Brain::output() const {
    return brain_output;
}


void Brain::update(double dt, const NervousOutput& nervous_output) {
    brain_state.state = brain_input;
    
    brain_output = BrainOutput();
    brain_output.nervous_output.cardiac_drive = efferent_stimuli();
}

void Brain::status() const {
    
}
