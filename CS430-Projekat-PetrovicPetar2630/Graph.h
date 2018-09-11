#pragma once

#include "stdafx.h"
#include <vector>
#include <list>
#include <iostream>

#include "GameObject.h"
#include "GraphNode.h"
#include "GraphEdge.h"

#include "Entity.h"

using namespace std;

class Graph : public GameObject  {

private:
	vector<GraphNode*>               m_nodes;
	vector<list<GraphEdge> >		m_edges;

	int m_collumn, m_row, m_cellSize;

	GraphNode* jailEntrence;

	GraphNode* policeEntrence;

public:

	Graph(int collumn, int row, int cellSize) : GameObject(entityTypes::graph, Vector2D<double>(-1, -1), 0),
		m_nodes(std::vector<GraphNode*>()),
		m_edges(std::vector<std::list<GraphEdge> >()) {
	
			m_collumn = collumn;
			m_row = row;
			m_cellSize = cellSize;
	
	}

	void addNode(GraphNode* node);

	void addEdge(const GraphEdge &edge);

	GraphNode* getJailEntrence() const;
	void setJailEntrence(GraphNode* node)
	{
		jailEntrence = node;
	}

	GraphNode* getPoliceEntrence() const;
	void setPoliceStation(GraphNode* node)
	{
		policeEntrence = node;
	}
	

	vector<GraphNode*> getNodes() const { return m_nodes; }

	vector<list<GraphEdge> > getEdges() const { return m_edges; }

	GraphNode* getRandomNode() {

		GraphNode* node = m_nodes[rand() % m_nodes.size()];

		while(!node->isRoad())
		{
			node = m_nodes[rand() % m_nodes.size()];
		}

		return node;
	}

	GraphNode* getNodeByPosition(Vector2D<double>) const;

	void setNodes(const vector<GraphNode*> &nodes);

	void render() const override;
};





