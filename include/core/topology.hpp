#include <string>
#include <vector>

struct Node {
    short id;
    std::string name;
    std::string type;

};

struct Edge {
    int source_id;
    int target_id;
};

struct Topology {
    std::vector<Node> nodes;
    std::vector<Edge> edges;
};