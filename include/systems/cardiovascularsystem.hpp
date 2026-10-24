#pragma once

#include "core/topology.hpp"
#include "organs/heart.hpp"

#include <vector>
#include <string>

struct VascularPort {
    int vessel_index;
    int node_id;
};

struct CapillaryBed {
    VascularPort arterial_input;
    std::vector<VascularPort> venous_outputs;

    double blood_volume = 0.0;
};

struct Vessel {
    int id;
    std::string name;

    // topology
    int node_a;
    int node_b;

    // geometry
    double length;    // cm
    double radius_a;  // cm
    double radius_b;  // cm

    // dataset metadata
    int type;         // 1 artery, 0 vein
    int region;
    bool perforator;

    int vascular_territory;
    int connection_a;
    int connection_b;

    // simulation state
    double blood_volume = 0.0;
    double blood_velocity = 0.0;
};

class CardioVascularSystem {
public:
    Topology topology;
    Heart* heart = nullptr;

    std::vector<Vessel> vessels;
    std::vector<CapillaryBed> capillary_beds;


    CardioVascularSystem() = default;

    CardioVascularSystem(Heart* heart) {
        this->heart = heart;
    }


    // Load anatomy
    void load_topology(const std::string& nodes_path, const std::string& vessels_path);

    void load_vessels(const std::string& vessels_path);

    void build_capillary_beds();

    void boot(double dt);
    void supply_blood(double dt);


    // Normal simulation
    void update(double dt);

    void update_blood(double dt);
    void update_blood_velocity(double dt);

    void update_heart_transfer(double dt, std::vector<double>& delta_volume);

    void update_vessel_transfer(double dt, std::vector<double>& delta_volume);
};