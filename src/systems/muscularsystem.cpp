#include "systems/muscularsystem.hpp"

MuscularSystem::MuscularSystem() {
    muscle_group_vector.push_back(&groups.legs);
    muscle_group_vector.push_back(&groups.back);
    muscle_group_vector.push_back(&groups.chest);
    muscle_group_vector.push_back(&groups.shoulders);
    muscle_group_vector.push_back(&groups.arms);
    muscle_group_vector.push_back(&groups.core);
    muscle_group_vector.push_back(&groups.neck);
}