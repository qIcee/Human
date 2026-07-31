#pragma once

enum class SignalType {
    Stretch,
    Force,
    MetabolicDemand,
    Pain,
    Temperature,
    Pressure
};

struct SensorState {
    SignalType type;
    double value;
};

class Sensor {
public:
    SensorState state;

};