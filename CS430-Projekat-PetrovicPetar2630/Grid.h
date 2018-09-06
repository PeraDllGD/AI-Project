#pragma once

#include <algorithm>

#include "Graph.h"
#include "GraphNode.h"
#include "Entity.h"

class Grid: public GameObject  {
private:
    Graph* graph;

public:
    Grid(int collumn, int row, int cellSize);
    void render() const override;
	Graph* getGraph(){return graph;}
};


