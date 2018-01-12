#ifndef __EDGE_H
#define __EDGE_H

#include "point.h"

class Node; // forward declaration to solve co-dependence with Node.h

class Edge{
    public:
        Node* node; // Point to a node
        double cost;
        Edge();
        Edge(Node*, double);
};

Edge::Edge(){
    cost = -1;
}


Edge::Edge(Node* n, double c){
    node = n;
    cost = c;
}


#endif