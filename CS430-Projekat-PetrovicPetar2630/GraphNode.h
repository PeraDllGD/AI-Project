#pragma once

#include <stdlib.h>
#include <GL\glut.h>
#include "GameObject.h"
#include "Utilities.h"
#include "Entity.h"

class GraphNode : public GameObject {

public:
    
	bool isTop;
	bool isBottom;
	bool isLeft;
	bool isRight;

	enum terrainType {
		road,
		building,
		jail
    };

    GraphNode() : GameObject(entityTypes::tile, Vector2D<double>(), 0),
                m_terrainFlag(building),
                m_color(), 
				m_index(){}

	GraphNode(int index,
            const Vector2D<double> &position,
            terrainType terrainFlag,
			int cellSize);

	~GraphNode();

    int getIndex() const { return m_index; }

    void setIndex(int i) { m_index = i; }

	void GraphNode::render() const;

	terrainType getTerrain() const { return m_terrainFlag; }

	void setTerain(terrainType type) {  m_terrainFlag = type; setColorFromTerrainFlag(); }

	bool isRoad(){ return m_terrainFlag == terrainType::road; }

	

private:
    int m_index;

	terrainType m_terrainFlag;

	Color m_color;

	int m_cellSize;
	
	void setColorFromTerrainFlag() { 
	
		float randColor = (rand() % 40) / 100.;

		switch (m_terrainFlag) {
			case building:
				m_color = Color(1, randColor, randColor);
				break;
			case road:
				m_color = Color(0.95, 0.95, 0.95);
				break;
			case jail:
				m_color = Color(.8, .8, .8);
				break;
		}
	}

};

