#pragma once

#include <iostream>
#include <vector>

#include "organs/organs.hpp"
#include "organs/organ.hpp"
#include "organs/heart.hpp"


#include "muscles/muscles.hpp"
#include "muscles/musclegroup.hpp"


#include "systems/cardiovascularsystem.hpp"
#include "systems/muscularsystem.hpp"
#include "systems/nervoussystem.hpp"




class Human {
public:
    std::vector<Organ*> organs_vector;
    Organs organs;

    std::vector<MuscleGroup*> muscles_vector;
    Muscles muscles;

    CardioVascularSystem cardiovascular_system;
    MuscularSystem muscular_system;
    NervousSystem nervous_system;


    Human();

    void update(double dt);
    void status();
};