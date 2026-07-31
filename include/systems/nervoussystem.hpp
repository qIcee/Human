#include <vector>

#include "sensors/sensor.hpp"


#include "data/brainio.hpp"
#include "data/nervousio.hpp"



class NervousSystem {
public:
    std::vector<Sensor*> sensor_vector;

    NervousInput nervous_input;
    NervousOutput nervous_output;
    

    NervousSystem();
    void update(double dt);
    void process_brain_output(const NervousOutput& output);
    BrainInput collect_brain_input();
};