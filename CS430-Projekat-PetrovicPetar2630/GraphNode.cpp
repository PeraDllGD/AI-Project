#include "StdAfx.h"
#include "GraphNode.h"



GraphNode::GraphNode(int index,
                 const Vector2D<double> &position,
                 terrainType terrainFlag, int cellSize) : GameObject(entityTypes::tile, position, 0),
                                                     m_terrainFlag(terrainFlag),
                                                     m_color(), m_index(index){

	
	m_cellSize = cellSize;

	isTop = false;
	isBottom = false;
	isLeft = false;
	isRight = false;
	
	setColorFromTerrainFlag();
}

void GraphNode::render() const {
    
    int halfSize = m_cellSize / 2;
    glPushMatrix();
    glTranslated(m_pos.x, m_pos.y, 0);
    
	glBegin(GL_POLYGON);
		glColor3d(m_color.r, m_color.g, m_color.b);
		glVertex2d(-halfSize, -halfSize);
		glVertex2d(halfSize, -halfSize);
		glVertex2d(halfSize, halfSize);
		glVertex2d(-halfSize, halfSize);
    glEnd();

	if(m_terrainFlag == terrainType::road)
	{
		glColor3d(0, 0, 0);

		if(isTop)
		{
			glBegin(GL_LINES);
			glVertex2d(0, 0);
			glVertex2d(0, halfSize / 2);
			glEnd();

		}
		
		if(isBottom)
		{
			
			glBegin(GL_LINES);
			glVertex2d(0, 0);
			glVertex2d(0, -halfSize / 2);
			glEnd();

		}

		if(isLeft)
		{
			
			glBegin(GL_LINES);
			glVertex2d(0, 0);
			glVertex2d(-halfSize / 2, 0);
			glEnd();

		}

		if(isRight)
		{
			
			glBegin(GL_LINES);
			glVertex2d(0, 0);
			glVertex2d(halfSize / 2, 0);
			glEnd();

		}


	}else if(m_terrainFlag == terrainType::jail)
	{

		glColor3d(0, 0, 0);

		for(int i = -halfSize; i <= halfSize; i = i + halfSize / 4)
		{
			glBegin(GL_LINES);
				glVertex2d(i, -halfSize );
				glVertex2d(i, halfSize);
			glEnd();
		}

	}

    glPopMatrix();
}