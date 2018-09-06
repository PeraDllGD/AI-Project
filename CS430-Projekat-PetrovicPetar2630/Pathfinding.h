#pragma once

#include "Graph.h"
#include "GraphNode.h"

class AStar {

	private:
    std::list<Vector2D<double> > m_points;

    std::list<Vector2D<double> >::iterator m_currentPoint;

    bool m_isLooped;

public:
    AStar() : m_isLooped(false) {}

    AStar(int numPoints,
         double minX,
         double minY,
         double maxX,
         double maxY,
         bool looped) : m_isLooped(looped) {
        createRandomPath(numPoints, minX, minY, maxX, maxY);

        m_currentPoint = m_points.begin();
    }

    Vector2D<double> getCurrentPoint() const {
        return *m_currentPoint;
    }

	Vector2D<double> getLastPoint() const {
        return m_points.back();
    }

    bool finished() {
        return m_currentPoint == m_points.end();
    }

    bool isLastPoint() {
        return *m_currentPoint == m_points.back();
    }

    void setNextPoint()
	{
		if (++m_currentPoint == m_points.end()) {
			if (m_isLooped) {
				m_currentPoint = m_points.begin();
			}
		}
	}

    std::list<Vector2D<double> > createRandomPath(int numPoints, double minX, double minY, double maxX, double maxY);

    void loop(bool toLoop) { m_isLooped = toLoop; }

    void set(const std::list<Vector2D<double> > &newPath) {
        m_points       = newPath;
        m_currentPoint = m_points.begin();
    }

	

    void clear() {
        m_points.clear();
    }
    void render(float r, float g, float b) const;

public:
	static AStar* shortestPath(Graph* graph, GraphNode* start, GraphNode* finish);
};

