#include <string>
#include <vector>

struct Node {
    short id;
    std::string name;
    std::string type;

};

struct Edge {
    short source_id;
    short target_id;
};

struct Topology {
    std::vector<Node> nodes;
    std::vector<Edge> edges;
};