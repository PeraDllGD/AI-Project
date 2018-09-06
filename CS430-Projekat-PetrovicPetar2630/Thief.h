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

#include "Citizen.h"

class Thief : public GameObject
{
private:

	StateMachine<Thief>*  m_pStateMachine;

	void draw(float);

	float m_radius;

	AStar* m_path;
	
	list<Citizen*> m_listOfCitizens;

	

public:
	
	bool isOnSamePlaceAsCitizen;

	bool isCaught;

	Citizen* current_enemy;

	stringstream  name;

	float movementSpeed;
	float money;

	list<Citizen*>& getListOfCitizens()
	{
		return m_listOfCitizens;
	}

	void setCitizens(list<Citizen*>& listOfCitizens){
		m_listOfCitizens = listOfCitizens;
	}

	Thief(int id, Graph* graph, float radius);

	~Thief(){delete m_pStateMachine;}

	//this must be implemented
	void Update(float);

	StateMachine<Thief>*  GetFSM()const;

	Graph* m_graph;

	void setPath(AStar* path)
	{
		m_path = path;
		isFinishedMovement = false;
	}

	void reset();

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

class ThiefWonder : public State<Thief>
{
private:
  
  ThiefWonder(){}
  
  //copy ctor and assignment should be private
  ThiefWonder(const ThiefWonder&);
  ThiefWonder& operator=(const ThiefWonder&);
 
public:

  static ThiefWonder* Instance();
  
public:
  
	virtual void Enter(Thief* citizen);

  virtual void Execute(Thief* citizen);

  virtual void Exit(Thief* citizen);

};

//////////////////////////////////////////////////////////////

class ThiefSeekCitizen : public State<Thief>
{
private:
  
  ThiefSeekCitizen(){}
  
  //copy ctor and assignment should be private
  ThiefSeekCitizen(const ThiefSeekCitizen&);
  ThiefSeekCitizen& operator=(const ThiefSeekCitizen&);
 
public:

  static ThiefSeekCitizen* Instance();
  
public:
  
	virtual void Enter(Thief* citizen);

  virtual void Execute(Thief* citizen);

  virtual void Exit(Thief* citizen);

};

//////////////////////////////////////////////////////////////

class ThiefRunTowardsCitizen : public State<Thief>
{
private:
  
  ThiefRunTowardsCitizen(){}
  
  //copy ctor and assignment should be private
  ThiefRunTowardsCitizen(const ThiefRunTowardsCitizen&);
  ThiefRunTowardsCitizen& operator=(const ThiefRunTowardsCitizen&);
 
public:

  static ThiefRunTowardsCitizen* Instance();
  
public:
  
	virtual void Enter(Thief* citizen);

  virtual void Execute(Thief* citizen);

  virtual void Exit(Thief* citizen);

};

//////////////////////////////////////////////////////////////

class ThiefFollowPolicemanToJail : public State<Thief>
{
private:
  
  ThiefFollowPolicemanToJail(){}
  
  //copy ctor and assignment should be private
  ThiefFollowPolicemanToJail(const ThiefFollowPolicemanToJail&);
  ThiefFollowPolicemanToJail& operator=(const ThiefFollowPolicemanToJail&);
 
public:

  static ThiefFollowPolicemanToJail* Instance();
  
public:
  
	virtual void Enter(Thief* citizen);

  virtual void Execute(Thief* citizen);

  virtual void Exit(Thief* citizen);

};
