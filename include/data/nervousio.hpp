#pragma once

struct NervousInput {
    double sum_force = 0.0;
    double sum_metabolic_demand = 0.0;
    double sum_pain = 0.0;
    double sum_pressure = 0.0;
    double sum_stretch = 0.0;
    double sum_temperature = 0.0;
};

struct NervousOutput {
    double cardiac_drive = 0.0; // [-1.0, 1.0]  
};