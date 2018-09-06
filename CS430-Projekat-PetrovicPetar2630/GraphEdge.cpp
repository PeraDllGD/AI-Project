#include "StdAfx.h"
#include "GraphEdge.h"

void GraphEdge::render() const {
    
    glPushMatrix();

	
	/*glTranslated((from->getPos().x + to->getPos().x) / 2, (from->getPos().y + to->getPos().y) / 2, 0);
    glBegin(GL_POLYGON);
    glColor3d(0.f, 0.f, 0.f);
    glVertex2d(-5, -5);
    glVertex2d(5, -5);
    glVertex2d(5, 5);
    glVertex2d(-5, 5);
    glEnd();*/

	glLineWidth(1); 
	glColor3d(0.f, 0.f, 0.f);
	glBegin(GL_LINES);
	glVertex3f(from->getPos().x, from->getPos().y, 0.0);
	glVertex3f(to->getPos().x, to->getPos().y, 0.0);
	glEnd();
    glPopMatrix();
}