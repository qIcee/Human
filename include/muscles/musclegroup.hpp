#pragma once

#include "sensors/sensor.hpp"


class MuscleGroup {
public:
    double st_fibres_pct;
    double ft_fibres_pct;
    double muscle_mass;
    double activation;

    double metabolic_demand;

    Sensor metabolic_sensor;

    MuscleGroup();

    void update(double dt);
    double calculate_metabolic_demand();
    
};