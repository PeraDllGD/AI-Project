// CS430-Projekat-PetrovicPetar2630.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <iostream>

#include <stdlib.h>
#include <GL\glut.h>
#include <Windows.h>
#include <ctime>
#include "Grid.h"
#include "Citizen.h"
#include "Thief.h"
#include "Police.h"

int Width  = 800;
int Height = 600;

int cellSize = 20;
int collumnNumber = 30;
int rowNumber = 35;

int numberOfCitizens = 10;	// 10
int numberOfThieves = 5;    // 5
int numberOfPolice = 3 ;	// 3

list<Citizen*> listOfCitizens;
list<Thief*> listOfThieves;
list<Police*> listOfPoliceman;


double t  = 0.0;
double dt = 0.0166666666666667;

Grid* grid;

#define TIME_CORRECTION 0.2 // make slower
clock_t currentTime = clock();

using namespace std;


void render() {
	glClear(GL_COLOR_BUFFER_BIT);

	clock_t newTime   = clock();
	double  frameTime = ((double) newTime - currentTime) / (CLOCKS_PER_SEC * TIME_CORRECTION);
	currentTime = newTime;

	while (frameTime > 0.0) {
		double deltaTime = min(frameTime, dt);
		frameTime -= deltaTime;
		t += deltaTime;

		// cout << "Updating" << endl;
		for (std::list<Citizen*>::iterator it= listOfCitizens.begin(); it != listOfCitizens.end(); ++it)
		{
			(*it)->Update(deltaTime);
		}

		for (std::list<Thief*>::iterator it= listOfThieves.begin(); it != listOfThieves.end(); ++it)
		{
			(*it)->Update(deltaTime);
		}

		for (std::list<Police*>::iterator it= listOfPoliceman.begin(); it != listOfPoliceman.end(); ++it)
		{
			(*it)->Update(deltaTime);
		}

		
	}

	// cout << "Rendering" << endl;
	grid->render();

	for (std::list<Citizen*>::iterator it= listOfCitizens.begin(); it != listOfCitizens.end(); ++it)
	{
		(*it)->render();
	}

	for (std::list<Thief*>::iterator it= listOfThieves.begin(); it != listOfThieves.end(); ++it)
	{
		(*it)->render();
	}

	for (std::list<Police*>::iterator it= listOfPoliceman.begin(); it != listOfPoliceman.end(); ++it)
	{
		(*it)->render();
	}


	glutSwapBuffers();
	glFlush();
}

void timer(int val) {
	glutPostRedisplay();
	glutTimerFunc(16, timer, 0);
}

void reshape(GLint width, GLint height) {
	glViewport(0, 0, (GLsizei) width, (GLsizei) height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, width, 0, height, -1, 1);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glClear(GL_COLOR_BUFFER_BIT);
}

int main(int argc, char** argv) 
{
	srand(time(NULL));

	grid = new Grid(collumnNumber, rowNumber, cellSize);
	
	for(int i = 0; i < numberOfCitizens; i++)
	{
		listOfCitizens.push_back(new Citizen(0, grid->getGraph(), 8));
	}

	for(int i = 0; i < numberOfThieves; i++)
	{
		Thief* thief = new Thief(0, grid->getGraph(), 7);
		thief->setCitizens(listOfCitizens);
		listOfThieves.push_back(thief);
	}

	for(int i = 0; i < numberOfPolice; i++)
	{
		Police* police = new Police(0, grid->getGraph(), 7);
		police->setThieves(listOfThieves);
		listOfPoliceman.push_back(police);
	}
	
	srand(static_cast<unsigned int>(time(nullptr)));

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowPosition(150, 150);
	glutInitWindowSize(Width, Height);
	glutCreateWindow("CS430 Projekat - Petrovic Petar 2630");

	glClearColor(1.0, 1.0, 1.0, 0.0);
	glutDisplayFunc(render);
	glutReshapeFunc(reshape);
	glutTimerFunc(0, timer, 0);
	glutMainLoop();

	return 0;
}

