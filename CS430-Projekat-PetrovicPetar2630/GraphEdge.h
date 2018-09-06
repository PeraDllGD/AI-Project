#pragma once

#include "GraphNode.h"
#include "GameObject.h"
#include "Entity.h"

class GraphEdge : public GameObject{
protected:
   
    GraphNode* from;
	GraphNode* to;

    double cost;

public:

    GraphEdge(GraphNode* from, GraphNode* to, double cost) : to(to),
                                                         from(from),
                                                         cost(cost),
														 GameObject(entityTypes::graph,
                                                                        Vector2D<double>(),
                                                                        0) {};

    int getTo() const { return to->getIndex(); }

    int getFrom() const { return from->getIndex(); }

	void GraphEdge::render() const;

    bool operator==(const GraphEdge &rhs) const {
        return to == rhs.to &&
               from == rhs.from;
    }

    bool operator!=(const GraphEdge &rhs) const {
        return !(rhs == *this);
    }

    double getCost() const { return cost; }
};

