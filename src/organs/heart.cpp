#include "organs/heart.hpp"

Heart::Heart() {
    state.heart_rate = 1.0;
    state.stroke_volume = 70.0;
}

Heart::Heart(double hf, double sv) {
    this->state.heart_rate = hf;
    this->state.stroke_volume = sv;
}

void Heart::update(double dt, const NervousOutput& nervous_output) {
    double drive = nervous_output.cardiac_drive; // [-1, 1]

    constexpr double speed = 0.2; // speed in Hz

    state.heart_rate += drive * speed * dt;

    // temporary hard boundary
    if (state.heart_rate > 3.5) state.heart_rate = 3.5;
    if (state.heart_rate < 0.5) state.heart_rate = 0.5;
}

void Heart::status() const {
    std::cout << "Heart Frequency(in BPM): "  << state.heart_rate * 60
              << "\nStroke Volume(in mL): "  << state.stroke_volume
              << "\nCardiac Output(in mL/min): " << cardiac_output()
              << std::endl;
}

double Heart::cardiac_output() const {
    return state.heart_rate*state.stroke_volume*60; 
}