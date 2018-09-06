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

#include "Thief.h"

class Police : public GameObject
{
private:

	StateMachine<Police>*  m_pStateMachine;

	void draw(float);

	float m_radius;

	AStar* m_path;
	
	list<Thief*> m_listOfThieves;

	

public:
	
	bool shouldSireneBeOn;

	Thief* current_thief;

	stringstream  name;

	float movementSpeed;

	list<Thief*>& getListOfThieves()
	{
		return m_listOfThieves;
	}

	void setThieves(list<Thief*>& listOfThieves){
		m_listOfThieves = listOfThieves;
	}

	Police(int id, Graph* graph, float radius);

	~Police(){delete m_pStateMachine;}

	//this must be implemented
	void Update(float);

	StateMachine<Police>*  GetFSM()const{return m_pStateMachine;}

	Graph* m_graph;

	void setPath(AStar* path)
	{
		m_path = path;
		isFinishedMovement = false;
	}
	
	bool isFinishedMovement;

	bool thiefCaught;
	
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

class PoliceWonder : public State<Police>
{
private:
  
  PoliceWonder(){}
  
  //copy ctor and assignment should be private
  PoliceWonder(const PoliceWonder&);
  PoliceWonder& operator=(const PoliceWonder&);
 
public:

  static PoliceWonder* Instance();
  
public:
  
	virtual void Enter(Police* citizen);

  virtual void Execute(Police* citizen);

  virtual void Exit(Police* citizen);

};

////////////////////////////////////////////////////////////// 

class PoliceChase : public State<Police>
{
private:
  
  PoliceChase(){}
  
  //copy ctor and assignment should be private
  PoliceChase(const PoliceChase&);
  PoliceChase& operator=(const PoliceChase&);
 
public:

  static PoliceChase* Instance();
  
public:
  
	virtual void Enter(Police* citizen);

  virtual void Execute(Police* citizen);

  virtual void Exit(Police* citizen);

};

////////////////////////////////////////////////////////////// 

class PoliceExecuteToJail : public State<Police>
{
private:
  
  PoliceExecuteToJail(){}
  
  //copy ctor and assignment should be private
  PoliceExecuteToJail(const PoliceExecuteToJail&);
  PoliceExecuteToJail& operator=(const PoliceExecuteToJail&);
 
public:

  static PoliceExecuteToJail* Instance();
  
public:
  
	virtual void Enter(Police* citizen);

  virtual void Execute(Police* citizen);

  virtual void Exit(Police* citizen);

};