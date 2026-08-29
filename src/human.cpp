#include "human.hpp"
#include "organs/heart.hpp"


Human::Human() {
    // organs
    organs_vector.push_back(&organs.heart);   


    // muscles
    muscles_vector.push_back(&muscles.arms);
    muscles_vector.push_back(&muscles.back);
    muscles_vector.push_back(&muscles.chest);
    muscles_vector.push_back(&muscles.core);
    muscles_vector.push_back(&muscles.legs);
    muscles_vector.push_back(&muscles.neck);
    muscles_vector.push_back(&muscles.shoulders);

    // set muscles for test
    muscles.legs.muscle_mass = 20.0;
    muscles.legs.st_fibres_pct = 0.6;
    muscles.legs.ft_fibres_pct = 0.4;

    muscles.arms.muscle_mass = 4.0;
    muscles.arms.st_fibres_pct = 0.4;
    muscles.arms.ft_fibres_pct = 0.6;

    for (MuscleGroup* muscle : muscles_vector) {
        nervous_system.sensor_vector.push_back(&muscle->metabolic_sensor);
    }

    // cardiovascular system
    cardiovascular_system.heart = &organs.heart;

}


void Human::update(double dt) {
    organs.brain.update(dt);

    // give nervous system output back
    BrainOutput brain_output = organs.brain.output();
    nervous_system.process_brain_output(brain_output.nervous_output);


    for (Organ* organ : organs_vector) {
        organ->update(dt, brain_output.nervous_output);
    }

    for (MuscleGroup* muscle : muscles_vector) {
        muscle->update(dt);
    }

    nervous_system.update(dt);

    // give brain all inputs
    organs.brain.input(nervous_system.collect_brain_input());

}

void Human::status() {
    for (Organ* organ : organs_vector) {
        organ->status();
    }
}
