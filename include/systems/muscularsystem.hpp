#include <vector>

#include "muscles/musclegroup.hpp"

struct MuscleGroups {
    MuscleGroup legs;
    MuscleGroup back;
    MuscleGroup chest;
    MuscleGroup shoulders;
    MuscleGroup arms;
    MuscleGroup core;
    MuscleGroup neck;
};

class MuscularSystem {
public:
    MuscleGroups groups;
    std::vector<MuscleGroup*> muscle_group_vector;


    MuscularSystem();
};