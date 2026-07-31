#include "simulation.hpp"
#include "human.hpp"


void Simulation::start_simulation() {
    
    double dt = 0.01;
    double sim_time = 0.0;
    int step = 0;

    double end_time = 10.0;

    // game loop
    while (sim_time < end_time) {
        // TODO: determine what needs to be updated
        

        // external input test
        if (sim_time >= 2.0 && sim_time < 7.0) {
            for (MuscleGroup* muscle : human.muscles_vector) {
                muscle->activation = 1.0;
            }
        } else {
            for (MuscleGroup* muscle : human.muscles_vector) {
                muscle->activation = 0.0;
            }
        }



        human.update(dt);
        
        // log
        sim_time += dt;
        step++;

        if (step % 100 == 0) {
            std::cout << "Sim time: " << sim_time << " s\n";
            human.status();
        }
    }
}    