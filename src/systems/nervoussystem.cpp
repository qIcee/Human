#include "systems/nervoussystem.hpp"

NervousSystem::NervousSystem() {

}

void NervousSystem::update(double dt) {
    nervous_input = NervousInput{};

    for (Sensor* sensor : sensor_vector) {
        SignalType state_type = sensor->state.type;
        double value = sensor->state.value;

        switch (state_type) {
            case SignalType::Force:
                nervous_input.sum_force += value;
                break;
            case SignalType::MetabolicDemand:
                nervous_input.sum_metabolic_demand += value;
                break;
            case SignalType::Pain:
                nervous_input.sum_pain += value;
                break;
            case SignalType::Pressure:
                nervous_input.sum_pressure += value;
                break;
            case SignalType::Stretch:
                nervous_input.sum_stretch += value;
                break;
            case SignalType::Temperature:
                nervous_input.sum_temperature += value;
                break; 
        }
    }
}

void NervousSystem::process_brain_output(const NervousOutput& output) {
    nervous_output = output;
}

BrainInput NervousSystem::collect_brain_input() {
    BrainInput result = {};
    result.nervous_input = nervous_input;
    return result;
}