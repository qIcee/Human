#pragma once

#include "core/topology.hpp"
#include "organs/heart.hpp"

#include <vector>
#include <string>

struct Vessel {
    int id;
    std::string name;

    double length;    // cm
    double radius_a;  // cm
    double radius_b;  // cm

    int type;         // 1 artery, 0 vein
    int region;
    bool perforator;
    
    double blood_volume = 0.0;
    double blood_velocity = 0.0;
};

class CardioVascularSystem {
public:
    Topology topology;
    Heart* heart = nullptr;

    std::vector<Vessel> vessels;


    CardioVascularSystem(Heart* heart);


    // Load anatomy
    void load_topology(const std::string& nodes_path, const std::string& vessels_path);

    void load_vessels(const std::string& vessels_path);


    void boot(double dt);
    void supply_blood(double dt);


    // Normal simulation
    void update(double dt);

    void update_blood(double dt);
    void update_blood_velocity(double dt);

    void update_heart_transfer(double dt, std::vector<double>& delta_volume);

    void update_vessel_transfer(double dt, std::vector<double>& delta_volume);
};