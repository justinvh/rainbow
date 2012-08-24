#ifndef RAINBOW_MAP_BSP_HPP
#define RAINBOW_MAP_BSP_HPP

namespace rb {

struct Node {
    Node behind;
    Node front;
    Node* parent;
    Node sibling;
    float* vert;
    float* st;
};

}

#endif // RAINBOW_MAP_BSP_HPP
