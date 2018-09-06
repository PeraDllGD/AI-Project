#pragma once

#include "stdafx.h"

#include "GameObject.h"
#include "Entity.h"

#include "StateMachine.h"

#include "Graph.h"

#include "Vector2D.h"
#include "Pathfinding.h"

#include <iostream>   
#include <sstream> 
#include <string.h>


class Citizen : public GameObject
{
private:

	StateMachine<Citizen>*  m_pStateMachine;

	void draw(float);

	float m_radius;

	AStar* m_path;

public:

	float money;

	void reset();

	stringstream  name;

	float movementSpeed;

	Citizen(int id, Graph* graph, float radius);

	~Citizen(){delete m_pStateMachine;}

	//this must be implemented
	void Update(float);

	StateMachine<Citizen>*  GetFSM()const{return m_pStateMachine;}

	Graph* m_graph;

	void setPath(AStar* path)
	{
		m_path = path;
		isFinishedMovement = false;
	}

	bool isFinishedMovement;
	void move(float deltaTime)
	{
		// cout << "moving" << endl;
		if ((getPos() - m_path->getCurrentPoint()).squareMagnitude() < 4 && !m_path->isLastPoint()) {

			m_path->setNextPoint();
			// cout << "setNextPoint" << endl;
		}

		float dx = m_path->getCurrentPoint().x - m_pos.x;
		float dy = m_path->getCurrentPoint().y - m_pos.y;
		float dist = std::sqrt(dx*dx+dy*dy);
		
		if (dist > movementSpeed * deltaTime) {
			dx /= dist;
			dy /= dist;
			m_pos.x += dx * movementSpeed * deltaTime;
			m_pos.y += dy * movementSpeed * deltaTime;
		} else {
			m_pos.x = m_path->getCurrentPoint().x;
			m_pos.y = m_path->getCurrentPoint().y;
			isFinishedMovement = true;
		}

	}

	void render()
	{
		draw(m_radius);
	}
};

////////////////////////////////////////////////////////////// 

class CitizenWonder : public State<Citizen>
{
private:
  
  CitizenWonder(){}
  
  //copy ctor and assignment should be private
  CitizenWonder(const CitizenWonder&);
  CitizenWonder& operator=(const CitizenWonder&);
 
public:

  static CitizenWonder* Instance();
  
public:
  
  virtual void Enter(Citizen* citizen);

  virtual void Execute(Citizen* citizen);

  virtual void Exit(Citizen* citizen);

};
