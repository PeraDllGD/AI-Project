#include "StdAfx.h"

#include <set>
#include <map>
#include "Pathfinding.h"


double heuristicDistance(const GraphNode* start, const GraphNode* finish) {
    return start->getPos().distanceTo(finish->getPos());
}

GraphNode* findLowestCost(std::set<GraphNode*> set, std::map<int, double> fScore) {
    GraphNode* lowest = nullptr;
    for (auto it = set.begin(); it != set.end(); ++it) {
        if (!lowest) {
            lowest = *it;
            continue;
        }

        if (fScore[(*it)->getIndex()] < fScore[lowest->getIndex()]) {
            lowest = *it;
        }
    }

    return lowest;
}

void fillScores(std::map<int, double>* scoreMap, std::vector<GraphNode*> nodes) {
    for (auto it = nodes.begin(); it != nodes.end(); ++it) {
        scoreMap->insert(std::pair<int, double>((*it)->getIndex(), std::numeric_limits<double>::infinity()));
    }
}

AStar* reconstructPath(const std::map<int, GraphNode*> &cameFrom, GraphNode* current) {
    auto* path = new AStar();
    path->loop(false);
    std::list<Vector2D<double> > list;
    list.push_back(current->getPos());

    while (cameFrom.find(current->getIndex()) != cameFrom.end()) {
        current = cameFrom.at(current->getIndex());
        list.push_back(current->getPos());
    }

    list.reverse();
    path->set(list);

    return path;
}



AStar* AStar::shortestPath(Graph* graph, GraphNode* start, GraphNode* finish) {
	if (!finish->isRoad()) {

		std::cout << "AStar::shortestPath: goal cannot be reached - start " << start->getPos() << ": end "<< finish->getPos() <<  std::endl;
        return new AStar();
    }
    
    std::vector<GraphNode*>              nodes = graph->getNodes();
    std::vector<std::list<GraphEdge> > edges = graph->getEdges();

    std::set<GraphNode*> closedSet;
    std::set<GraphNode*> openSet;
    openSet.insert(start);

    std::map<int, GraphNode*> cameFrom;
    std::map<int, double>   gScore;
    fillScores(&gScore, nodes);
    gScore[start->getIndex()] = 0;

    std::map<int, double> fScore;
    fillScores(&fScore, nodes);
    fScore[start->getIndex()] = heuristicDistance(start, finish);

    while (!openSet.empty()) {
        GraphNode* current = findLowestCost(openSet, fScore);
        if (current == finish) {
            // construct the path
            return reconstructPath(cameFrom, current);
        }

        openSet.erase(current);
        closedSet.insert(current);

        auto neighbors = edges[current->getIndex()];

        for (auto it = neighbors.begin(); it != neighbors.end(); ++it) {
            GraphEdge edge = *it;
            GraphNode* neighbor = nodes[edge.getTo()];
            
            if (closedSet.find(neighbor) != closedSet.end()) {
                // if the neighbor has already been added to the closedSet
                continue;
            }

            if (openSet.find(neighbor) == openSet.end()) {
                openSet.insert(neighbor);
            }

            double tentativeScore = gScore[current->getIndex()] + edge.getCost();
            
            if (tentativeScore >= gScore[neighbor->getIndex()]) {
                // neighbor isn't a better path
                continue;
            }
            
            cameFrom[neighbor->getIndex()] = current;
            gScore[neighbor->getIndex()] = tentativeScore;
            fScore[neighbor->getIndex()] = gScore[neighbor->getIndex()] + heuristicDistance(neighbor, finish);
        }
    }


    return new AStar();
}


std::list<Vector2D<double> > AStar::createRandomPath(int numPoints, double minX, double minY, double maxX, double maxY) {

    m_points.clear();

    double midX = (maxX + minX) / 2.0;
    double midY = (maxY + minY) / 2.0;

    double smaller = min(midX, midY);

    // number of radians at which each point should be placed
    double spacing = (2.0 * 3.14159265359) / (double) numPoints;

    for (int i = 0; i < numPoints; i++) {

        double radialDist =	rand() % ( static_cast<int>(floor(smaller) + 1) + static_cast<int>(floor(smaller * 0.2f)));	

        Vector2D<double> tmp(radialDist, 0.0);
        // rotateAroundOrigin(tmp, i * spacing);

        tmp.x += midX;
        tmp.y += midY;

        m_points.push_back(tmp);

    }

    m_currentPoint = m_points.begin();

    return m_points;
}

void AStar::render(float r, float g, float b) const {

    glColor3f(r, g, b);
    glBegin(GL_LINES);
	glLineWidth(10.0);

    auto iterator = m_points.begin();

    Vector2D<double> point = *iterator++;

    while (iterator != m_points.end()) {
        glVertex2d(point.x, point.y);
        glVertex2d((*iterator).x, (*iterator).y);

        point = *iterator++;
    }

    if (m_isLooped) {
        Vector2D<double> last = *--iterator;

        glVertex2d(last.x, last.y);
        glVertex2d(m_points.begin()->x, m_points.begin()->y);
    }

    glEnd();
}


