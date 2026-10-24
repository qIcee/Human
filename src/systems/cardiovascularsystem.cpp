#include "systems/cardiovascularsystem.hpp"

#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>

CardioVascularSystem::CardioVascularSystem(Heart* heart) {
    this->heart = heart;

    load_topology("../data/processed/vascular_nodes.csv", "../data/processed/vascular_vessels.csv");
    load_vessels("../data/processed/vascular_vessels.csv");
    build_capillary_beds();

}

void CardioVascularSystem::boot(double dt) {

}


void CardioVascularSystem::load_topology(const std::string& nodes_path, const std::string& vessels_path) {
    topology.nodes.clear();
    topology.edges.clear();

    std::string line;

    // load nodes
    std::ifstream nodes_file(nodes_path);

    std::getline(nodes_file, line); // skip header

    while (std::getline(nodes_file, line)) {
        if (line.empty())
            continue;

        Node node;
        node.id = std::stoi(line);

        topology.nodes.push_back(node);
    }


    // load edges from vessel data
    std::ifstream vessels_file(vessels_path);

    std::getline(vessels_file, line); // skip header

    while (std::getline(vessels_file, line)) {
        if (line.empty())
            continue;

        std::stringstream stream(line);
        std::string value;

        // id
        std::getline(stream, value, ',');

        // name
        std::getline(stream, value, ',');

        // node_a
        std::getline(stream, value, ',');
        int source_id = std::stoi(value);

        // node_b
        std::getline(stream, value, ',');
        int target_id = std::stoi(value);

        topology.edges.push_back({
            source_id,
            target_id
        });
    }


// special nodes
    // separate heart node initialization
    Node heart_node;
    heart_node.id = 4046;
    heart_node.name = "heart";
    heart_node.type = "heart";
    topology.edges.push_back({
        heart_node.id,
        778      // Heart -> aorta
    });

    topology.edges.push_back({
        3809,    // Inferior vena cava -> Heart
        heart_node.id
    });

    topology.edges.push_back({
        3934,    // Superior vena cava -> Heart
        heart_node.id
    });
    topology.nodes.push_back(heart_node);
}

void CardioVascularSystem::load_vessels(const std::string& vessels_path) {
    vessels.clear();

    std::ifstream vessels_file(vessels_path);
    std::string line;

    std::getline(vessels_file, line); // skip header

    while (std::getline(vessels_file, line)) {
        if (line.empty()) {
            continue;
        }

        std::stringstream stream(line);
        std::string value;

        Vessel vessel;

        // id
        std::getline(stream, value, ',');
        vessel.id = std::stoi(value);

        // name
        std::getline(stream, vessel.name, ',');

        // topology
        std::getline(stream, value, ',');
        vessel.node_a = std::stoi(value);

        std::getline(stream, value, ',');
        vessel.node_b = std::stoi(value);

        // geometry
        std::getline(stream, value, ',');
        vessel.length = std::stod(value);

        std::getline(stream, value, ',');
        vessel.radius_a = std::stod(value);

        std::getline(stream, value, ',');
        vessel.radius_b = std::stod(value);

        // initial vessel volume: conical-frustum approximation
        const double PI = 3.14159265358979323846;
        vessel.blood_volume =
            (PI * vessel.length / 3.0) *
            (vessel.radius_a * vessel.radius_a +
             vessel.radius_a * vessel.radius_b +
             vessel.radius_b * vessel.radius_b);

        // type
        std::getline(stream, value, ',');
        vessel.type = (value == "artery") ? 1 : 0;

        // region
        std::getline(stream, value, ',');
        vessel.region = std::stoi(value);

        // perforator
        std::getline(stream, value, ',');
        vessel.perforator = (value == "True");

        // metadata for capillary-bed grouping
        std::getline(stream, value, ',');
        vessel.vascular_territory = std::stoi(value);

        std::getline(stream, value, ',');
        vessel.connection_a = std::stoi(value);

        std::getline(stream, value, ',');
        vessel.connection_b = std::stoi(value);

        vessels.push_back(vessel);
    }
}

void CardioVascularSystem::build_capillary_beds() {
    capillary_beds.clear();

    // node-id -> number of real connected vessels
    std::unordered_map<int, int> node_connection_count;

    for (const Vessel& vessel : vessels) {
        node_connection_count[vessel.node_a]++;
        node_connection_count[vessel.node_b]++;
    }

    // returns the loose endpoint of a vessel.
    // returns -1 if it has no unique loose endpoint.
    auto get_terminal_node =
        [&node_connection_count](const Vessel& vessel) {

        bool node_a_is_terminal =
            node_connection_count.at(vessel.node_a) == 1;

        bool node_b_is_terminal =
            node_connection_count.at(vessel.node_b) == 1;

        // neither terminal, or an isolated vessel with two loose ends.
        if (node_a_is_terminal == node_b_is_terminal) {
            return -1;
        }

        return node_a_is_terminal
            ? vessel.node_a
            : vessel.node_b;
    };

    // connection-id -> terminal venous ports
    std::unordered_map<int, std::vector<VascularPort>>
        venous_connections;

    // register all terminal veins under their Connection-ID.
    for (int vessel_index = 0;
         vessel_index < static_cast<int>(vessels.size());
         vessel_index++) {

        const Vessel& vessel = vessels[vessel_index];

        int terminal_node = get_terminal_node(vessel);

        if (terminal_node == -1 || vessel.type != 0) {
            continue;
        }

        VascularPort port;
        port.vessel_index = vessel_index;
        port.node_id = terminal_node;

        if (vessel.connection_a != -1) {
            venous_connections[vessel.connection_a].push_back(port);
        }

        if (vessel.connection_b != -1) {
            venous_connections[vessel.connection_b].push_back(port);
        }
    }

    // build one CapillaryBed per terminal artery.
    for (int vessel_index = 0;
         vessel_index < static_cast<int>(vessels.size());
         vessel_index++) {

        const Vessel& vessel = vessels[vessel_index];

        int terminal_node = get_terminal_node(vessel);

        if (terminal_node == -1 || vessel.type != 1) {
            continue;
        }

        CapillaryBed bed;

        bed.arterial_input.vessel_index = vessel_index;
        bed.arterial_input.node_id = terminal_node;

        // add venous targets associated with one Connection-ID.
        auto add_venous_outputs =
            [&venous_connections, &bed](int connection_id) {

            if (connection_id == -1) {
                return;
            }

            auto found = venous_connections.find(connection_id);

            // connection-ID exists on the artery, but no terminal vein
            // with that ID was found.
            if (found == venous_connections.end()) {
                return;
            }

            for (const VascularPort& port : found->second) {
                bed.venous_outputs.push_back(port);
            }
        };

        add_venous_outputs(vessel.connection_a);
        add_venous_outputs(vessel.connection_b);

        // aorta / unconnected terminal arteries stay outside this system.
        if (bed.venous_outputs.empty()) {
            continue;
        }

        capillary_beds.push_back(bed);
    }
}

void CardioVascularSystem::update(double dt) {
    update_blood(dt);
}


void CardioVascularSystem::update_blood(double dt) {
    update_blood_velocity(dt);
    std::vector<double> delta_volume(vessels.size(), 0.0);

    update_heart_transfer(dt, delta_volume);
    update_vessel_transfer(dt, delta_volume);

    for (int i = 0; i < vessels.size(); i++) {
        vessels[i].blood_volume += delta_volume[i];
    }
}

void CardioVascularSystem::update_heart_transfer(double dt, std::vector<double>& delta_volume) {
    // Heart-facing vessels:
    // 815  -> aortic output, boundary node 778
    // 4149 -> inferior vena cava, boundary node 3809
    // 4287 -> superior vena cava, boundary node 3934

    const int aorta = 815;
    const int ivc = 4149;
    const int svc = 4287;

    double pumped_volume = heart->cardiac_output() / 60.0 * dt;

    double ivc_volume = vessels[ivc].blood_volume;
    double svc_volume = vessels[svc].blood_volume;

    double venous_volume = ivc_volume + svc_volume;

    if (venous_volume <= 0.0) {
        return;
    }

    // the heart cannot pump more blood than is currently available at the venous inputs.
    if (pumped_volume > venous_volume) {
        pumped_volume = venous_volume;
    }


    // bad approximation of pumped blood in the heart
    double ivc_fraction = ivc_volume / venous_volume;
    double svc_fraction = svc_volume / venous_volume;

    double ivc_transfer = pumped_volume * ivc_fraction;
    double svc_transfer = pumped_volume * svc_fraction;

    delta_volume[ivc] -= ivc_transfer;
    delta_volume[svc] -= svc_transfer;

    delta_volume[aorta] += pumped_volume;
}

void CardioVascularSystem::update_vessel_transfer(double dt, std::vector<double>& delta_volume) {
    const double PI = 3.14159265358979323846;

    for (int i = 0; i < vessels.size(); i++) {
        Vessel& vessel = vessels[i];
        Edge& edge = topology.edges[i];

        if (vessel.blood_velocity == 0.0) {
            continue;
        }

        int outgoing_node;
        double outgoing_radius;

        // Positive velocity:
        // source -> target
        if (vessel.blood_velocity > 0.0) {
            outgoing_node = edge.target_id;
            outgoing_radius = vessel.radius_b;
        }

        // Negative velocity:
        // target -> source
        else {
            outgoing_node = edge.source_id;
            outgoing_radius = vessel.radius_a;
        }

        double area = PI * outgoing_radius * outgoing_radius;

        double transfer_volume =
            area * std::abs(vessel.blood_velocity) * dt;

        // Cannot move more blood than the vessel contains.
        if (transfer_volume > vessel.blood_volume) {
            transfer_volume = vessel.blood_volume;
        }

        if (transfer_volume <= 0.0) {
            continue;
        }

        std::vector<int> connected_vessels;

        // Find other vessels connected to the outgoing node.
        for (int j = 0; j < vessels.size(); j++) {
            if (j == i) {
                continue;
            }

            Edge& connected_edge = topology.edges[j];

            if (
                connected_edge.source_id == outgoing_node ||
                connected_edge.target_id == outgoing_node
            ) {
                connected_vessels.push_back(j);
            }
        }

        // The explicit topology ends here.
        if (connected_vessels.empty()) {
            continue;
        }

        // Temporary rule:
        // distribute equally between connected vessels.
        double volume_per_vessel =
            transfer_volume / connected_vessels.size();

        delta_volume[i] -= transfer_volume;

        for (int connected_vessel : connected_vessels) {
            delta_volume[connected_vessel] += volume_per_vessel;
        }
    }
}

