#include "StdAfx.h"
#include "Graph.h"


void Graph::setNodes(const vector<GraphNode*> &nodes) {
    m_nodes.empty();
    m_edges.empty();

    m_nodes = nodes;
    for (int i = 0; i < nodes.size(); i++) {
        m_edges.push_back(list<GraphEdge>());
    }

}

void Graph::addNode(GraphNode* node) {
    m_nodes.push_back(node);
    m_edges.push_back(list<GraphEdge>());
}

GraphNode* Graph::getJailEntrence() const{
	return jailEntrence;
}

void Graph::addEdge(const GraphEdge &edge) {

    list<GraphEdge> fromList = m_edges[edge.getFrom()];

    list<GraphEdge> toList   = m_edges[edge.getTo()];


    if (edge.getFrom() == edge.getTo()) {
        return;
    }

    for (std::list<GraphEdge>::const_iterator i = fromList.begin(); i != fromList.end(); ++i) {
        if ((*i) == edge) {
            return;
        }
    }

    for (std::list<GraphEdge>::const_iterator i = toList.begin(); i != toList.end(); ++i) {
        if ((*i) == edge) {
            return;
        }
    }

    m_edges[edge.getFrom()].push_back(edge);
}

void Graph::render() const {

    for (unsigned int i = 0; i < m_nodes.size(); i++) {
        m_nodes.at(i)->render();
    }

    /*for (unsigned int i = 0; i < m_edges.size(); i++) {
        list<GraphEdge> list = m_edges.at(i);
        std::list<GraphEdge>::const_iterator iterator;

        for (iterator = list.begin(); iterator != list.end(); ++iterator) {
            (*iterator).render();
        }
    }*/
}

GraphNode* Graph::getNodeByPosition(Vector2D<double> pos) const {
	auto row    = static_cast<int>(floor(pos.x / m_cellSize));
    auto column = static_cast<int>(floor(pos.y / m_cellSize));

	int id = row * m_collumn + column;

    if (id >= 0 && id < m_collumn * m_row) {
        return getNodes().at(static_cast<unsigned long>(id));
    } else {
		return new GraphNode();
    }
}