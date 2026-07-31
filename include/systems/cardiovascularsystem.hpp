#include "core/topology.hpp"
#include "organs/heart.hpp"

class CardioVascularSystem {
public:
    Topology* topology = nullptr;
    Heart* heart = nullptr;

    CardioVascularSystem() = default;
};