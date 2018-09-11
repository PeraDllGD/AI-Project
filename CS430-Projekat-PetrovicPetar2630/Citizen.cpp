#include "StdAfx.h"
#include "Citizen.h"
#include "Utilities.h"
#include "Vector2D.h"

void Citizen::reset()
{
	noMoneyLeft = false;

	movementSpeed = 10;

	static int count = 0;
		
	name.str("");
	name.clear();
	name << "Citizen " << count;
	count++;

	// cout << name << endl;

	money = rand() % 2000 + 1000.f;

	setPos(m_graph->getRandomNode()->getPos());

	m_pStateMachine->SetCurrentState(CitizenWonder::Instance());
}

Citizen::Citizen(int id, Graph* graph, float radius): GameObject(entityTypes::agent, Vector2D<double>(-1, -1), 0)
	{
		m_radius = radius;

		m_graph = graph;

		m_pStateMachine = new StateMachine<Citizen>(this);

		

		reset();
	}

void Citizen::draw(float radius)
{

	double angle1 = 0.0;        

	glPushMatrix();

	glTranslated(m_pos.x, m_pos.y, 0);

	if(noMoneyLeft)
	{
		glColor3f(.8, 1., .4);
	}
	else
	{
		glColor3f(.4, 1., .4);
	}

	glBegin(GL_POLYGON);

		angle1 = 0.0;        
		glVertex2d( m_radius * cos(0.0) , m_radius * sin(0.0));

		int i;
		for (i = 0; i < 360; i++)
		{
			glVertex2d(m_radius * cos(angle1), m_radius *sin(angle1));
			angle1 += i ;
		}

    glEnd();
   glPopMatrix();

   /*
   if(m_path)
   {
	   glPushMatrix();

	   glTranslated(m_path->getCurrentPoint().x, m_path->getCurrentPoint().y, 0);
		glColor3f(1, 0, 0);

		glBegin(GL_POLYGON);

			angle1 = 0.0;        
			glVertex2d( m_radius * cos(0.0) , m_radius * sin(0.0));

			for (int i = 0; i < 360; i++)
			{
				glVertex2d(m_radius * cos(angle1), m_radius *sin(angle1));
				angle1 += i ;
			}

		glEnd();
	   glPopMatrix();

	   glPushMatrix();

		   glTranslated(m_path->getLastPoint().x, m_path->getLastPoint().y, 0);
			glColor3f(0, 0, 1);

			glBegin(GL_POLYGON);

				angle1 = 0.0;        
				glVertex2d( m_radius * cos(0.0) , m_radius * sin(0.0));

				for (int i = 0; i < 360; i++)
				{
					glVertex2d(m_radius * cos(angle1), m_radius *sin(angle1));
					angle1 += i ;
				}

			glEnd();
	   glPopMatrix();

	   m_path->render(.5, .7, .2);
   }
   */
   glPushMatrix();
   drawText(name.str().data(), m_pos.x + m_radius, m_pos.y, Color(0.f, 0.f, 0.f));

   std::ostringstream ss;
	ss << money;
	std::string s(ss.str());
	s += "$";

	drawText(s.data(), m_pos.x + m_radius, m_pos.y - m_radius - 5, Color(0.f, 0.f, 0.f));
	   
   glPopMatrix();
}

void Citizen::Update(float deltaTime)
{
	move(deltaTime);
   m_pStateMachine->Update();
}

//-----------------------------------------------------------------------------

CitizenWonder* CitizenWonder::Instance()
{
  static CitizenWonder instance;

  return &instance;
}

void CitizenWonder::Enter(Citizen* citizen)
{
	citizen->noMoneyLeft = false;
	GraphNode * start =	citizen->m_graph->getNodeByPosition(citizen->getPos());
	GraphNode * end =	citizen->m_graph->getRandomNode();

	citizen->setPos(start->getPos()) ;

	AStar* astar = AStar::shortestPath(citizen->m_graph, start, end);
	citizen->setPath(astar);

	citizen->movementSpeed = 5;
}


void CitizenWonder::Execute(Citizen* citizen)
{  
	if(citizen->isFinishedMovement)
	{
		citizen->GetFSM()->SetCurrentState(CitizenWonder::Instance());
	}

	if(citizen->money <= 0)
	{
		citizen->GetFSM()->SetCurrentState(CitizenNoMoney::Instance());
	}
}


void CitizenWonder::Exit(Citizen* citizen)
{
}

//-----------------------------------------------------------------------------

CitizenNoMoney* CitizenNoMoney::Instance()
{
  static CitizenNoMoney instance;

  return &instance;
}

void CitizenNoMoney::Enter(Citizen* citizen)
{
	citizen->noMoneyLeft = true;
	GraphNode * start =	citizen->m_graph->getNodeByPosition(citizen->getPos());
	GraphNode * end =	citizen->m_graph->getRandomNode();

	citizen->setPos(start->getPos()) ;

	AStar* astar = AStar::shortestPath(citizen->m_graph, start, end);
	citizen->setPath(astar);

	citizen->movementSpeed = 5;
}


void CitizenNoMoney::Execute(Citizen* citizen)
{  
	if(citizen->isFinishedMovement)
	{
		citizen->GetFSM()->SetCurrentState(CitizenWonder::Instance());
	}
}


void CitizenNoMoney::Exit(Citizen* citizen)
{
}

//-----------------------------------------------------------------------------

CitizenGoToPoliceStation* CitizenGoToPoliceStation::Instance()
{
  static CitizenGoToPoliceStation instance;

  return &instance;
}

void CitizenGoToPoliceStation::Enter(Citizen* citizen)
{

	GraphNode * start =	citizen->m_graph->getNodeByPosition(citizen->getPos());
	GraphNode * end =	citizen->m_graph->getPoliceEntrence();

	citizen->setPos(start->getPos()) ;

	AStar* astar = AStar::shortestPath(citizen->m_graph, start, end);
	citizen->setPath(astar);

	citizen->movementSpeed = 8;
}


void CitizenGoToPoliceStation::Execute(Citizen* citizen)
{  
	if(citizen->isFinishedMovement)
	{
		citizen->GetFSM()->SetCurrentState(CitizenWonder::Instance());
	}
}


void CitizenGoToPoliceStation::Exit(Citizen* citizen)
{
	citizen->money += citizen->m_moneyToReturn;

}