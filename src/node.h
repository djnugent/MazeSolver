#ifndef __NODE_H
#define __NODE_H

#include <vector>
#include "point.h"

class Edge; // forward declaration to solve co-dependence with Edge.h

enum class node_type_t{
    NORMAL = 0,
    START = 1,
    END = 2
};

class Node{
    public:
        node_type_t type;
        Point loc;
        bool visited;
        std::vector<Edge> adj;
        Node();
        Node(Point&,node_type_t);
};


Node::Node(){
    visited = false;
    type = node_type_t::NORMAL;
    loc = Point(-1,-1);
}

Node::Node(Point& p, node_type_t t){
    visited = false;
    type = t;
    loc = p;
}

#endif