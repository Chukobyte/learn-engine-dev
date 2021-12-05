#ifndef NODE_COMPONENT_H
#define NODE_COMPONENT_H

#include <string>
#include <vector>

struct NodeComponent {
    std::string name;
    std::vector<std::string> tags;
    bool visible = true;
};

#endif //NODE_COMPONENT_H
