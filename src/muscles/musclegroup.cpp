#include "muscles/musclegroup.hpp"

MuscleGroup::MuscleGroup() {
    metabolic_sensor.state.type = SignalType::MetabolicDemand;
    metabolic_sensor.state.value = 0.0;

    st_fibres_pct = 0.5;
    ft_fibres_pct = 0.5;
    muscle_mass = 1.0;
    activation = 0.0;
    metabolic_demand = 0.0;

}

double MuscleGroup::calculate_metabolic_demand() {
    return 0.001 * muscle_mass                                                       // resting demand, not accurate
           + activation * muscle_mass * (st_fibres_pct * 1 + ft_fibres_pct * 1.5);  // active demand
}

void MuscleGroup::update(double dt) {
    metabolic_sensor.state.value = calculate_metabolic_demand();
}