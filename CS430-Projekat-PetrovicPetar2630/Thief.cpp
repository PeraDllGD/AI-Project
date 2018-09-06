#include "StdAfx.h"

#include "Thief.h"
#include "Utilities.h"
#include "Vector2D.h"


void Thief::reset()
{
	isCaught = false;

	money = rand() % 1500 + 1000;
		
	movementSpeed = 10;

	static int count = 0;
		
	name.str("");
	name.clear();

	name << "Thief " << count;
	count++;

	cout << name << endl;

	isOnSamePlaceAsCitizen = false;

	setPos(m_graph->getRandomNode()->getPos());

	m_pStateMachine->SetCurrentState(ThiefWonder::Instance());
}

Thief::Thief(int id, Graph* graph, float radius): GameObject(entityTypes::agent, Vector2D<double>(-1, -1), 0)
	{

		m_radius = radius;

		m_graph = graph;

		m_pStateMachine = new StateMachine<Thief>(this);

		reset();
	}

StateMachine<Thief>* Thief::GetFSM()const
{
	return m_pStateMachine;
}

void Thief::draw(float radius)
{

	double angle1 = 0.0;        

	glPushMatrix();

	glTranslated(m_pos.x, m_pos.y, 0);

	if(GetFSM()->CurrentState() == ThiefWonder::Instance())
	{
		glColor3f(.4, .4, .4);
	}
	else if(money < 1000)
	{
		glColor3f(1., .4, .4);
		
	}
	else 
	{
		glColor3f(.8, .4, .4);
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

	glColor3f(.1, .1, .1);
	glBegin(GL_QUADS);
		glVertex2f(radius - 1,radius - 3);
		glVertex2f(-radius + 1, radius - 3);
		glVertex2f(-radius + 1, radius - 6);
		glVertex2f(radius - 1, radius - 6);
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
   }*/
   glPushMatrix();
   drawText(name.str().data(), m_pos.x + m_radius, m_pos.y, Color(0.f, 0.f, 0.f));

   std::ostringstream ss;
	ss << money;
	std::string s(ss.str());
	s += "$";

	drawText(s.data(), m_pos.x + m_radius, m_pos.y - m_radius - 5, Color(0.f, 0.f, 0.f));
	   
   glPopMatrix();
}

void Thief::Update(float deltaTime)
{
	move(deltaTime);
   m_pStateMachine->Update();
}

//-----------------------------------------------------------------------------

ThiefWonder* ThiefWonder::Instance()
{
  static ThiefWonder instance;

  return &instance;
}

void ThiefWonder::Enter(Thief* citizen)
{
  //if the miner is not already located at the goldmine, he must
  //change location to the gold mine
	std::cout << "ThiefWonder: " << citizen->name.str() << endl;

	GraphNode * start =	citizen->m_graph->getNodeByPosition(citizen->getPos());
	GraphNode * end =	citizen->m_graph->getRandomNode();

	citizen->setPos(start->getPos()) ;

	AStar* astar = AStar::shortestPath(citizen->m_graph, start, end);
	citizen->setPath(astar);

	citizen->movementSpeed = 5;
}


void ThiefWonder::Execute(Thief* citizen)
{  
  // printf("CitizenWonder::Execute");

	// citizen->move();

	if(citizen->isFinishedMovement)
	{
		citizen->GetFSM()->SetCurrentState(ThiefWonder::Instance());
	}

	citizen->money -= .1;

	if(citizen->money < 1000)
	{
		citizen->GetFSM()->SetCurrentState(ThiefSeekCitizen::Instance());
	}
}


void ThiefWonder::Exit(Thief* citizen)
{
  // printf("CitizenWonder::Exit");
}



//-----------------------------------------------------------------------------

ThiefSeekCitizen* ThiefSeekCitizen::Instance()
{
  static ThiefSeekCitizen instance;

  return &instance;
}

void ThiefSeekCitizen::Enter(Thief* citizen)
{
  //if the miner is not already located at the goldmine, he must
  //change location to the gold mine
  // printf("CitizenWonder::Enter");


	citizen->isOnSamePlaceAsCitizen = false;


	if(citizen->isFinishedMovement)
	{
		std::cout << "ThiefSeekCitizen: " << citizen->name.str() << endl;
	
		GraphNode * start =	citizen->m_graph->getNodeByPosition(citizen->getPos());
		GraphNode * end =	citizen->m_graph->getRandomNode();

		citizen->setPos(start->getPos()) ;

		AStar* astar = AStar::shortestPath(citizen->m_graph, start, end);
		citizen->setPath(astar);
	}

	citizen->movementSpeed = 8;
}


void ThiefSeekCitizen::Execute(Thief* citizen)
{  
   // printf("ThiefSeekCitizen::Execute");

	// citizen->move();

   std::list<Citizen*>::iterator it;
   
	for (std::list<Citizen*>::iterator it= citizen->getListOfCitizens().begin(); it != citizen->getListOfCitizens().end(); ++it)
	{
		if((*it)->getPos().distanceTo(citizen->getPos()) < 50)
		{
			// cout << (*it)->name << endl;
			citizen->current_enemy = *it;
			citizen->GetFSM()->SetCurrentState(ThiefRunTowardsCitizen::Instance());
		}
	}

	if(citizen->isFinishedMovement)
	{
		citizen->GetFSM()->SetCurrentState(ThiefSeekCitizen::Instance());
	}

	citizen->money -= .1;

	if(citizen->money <= 0)
	{
		citizen->money = 0;
	}
}


void ThiefSeekCitizen::Exit(Thief* citizen)
{
  // printf("CitizenWonder::Exit");
}

//-----------------------------------------------------------------------------

ThiefRunTowardsCitizen* ThiefRunTowardsCitizen::Instance()
{
  static ThiefRunTowardsCitizen instance;

  return &instance;
}

void ThiefRunTowardsCitizen::Enter(Thief* citizen)
{

	citizen->isOnSamePlaceAsCitizen = false;

  //if the miner is not already located at the goldmine, he must
  //change location to the gold mine
	std::cout << "ThiefRunTowardsCitizen: " << citizen->name.str() << endl;

	GraphNode * start =	citizen->m_graph->getNodeByPosition(citizen->getPos());
	GraphNode * end =	citizen->m_graph->getNodeByPosition(citizen->current_enemy->getPos());
	
	AStar* astar = AStar::shortestPath(citizen->m_graph, start, end);
	citizen->setPath(astar);

	citizen->movementSpeed = 8;
}


void ThiefRunTowardsCitizen::Execute(Thief* citizen)
{  
	
	// citizen->move();
	if(citizen->isFinishedMovement && citizen->isOnSamePlaceAsCitizen == false)
	{
		citizen->GetFSM()->SetCurrentState(ThiefRunTowardsCitizen::Instance());
	}
		
	if(citizen->current_enemy->getPos().distanceTo(citizen->getPos()) < 10)
	{
		citizen->isFinishedMovement = true;

		citizen->isOnSamePlaceAsCitizen = true;

		citizen->current_enemy->money -= 1;
		citizen->money += 1;

		citizen->setPos(citizen->current_enemy->getPos());

		if(citizen->money >= 3000)
		{
			citizen->GetFSM()->SetCurrentState(ThiefWonder::Instance());

		}
		else if(citizen->current_enemy->money <= 0)
		{
			citizen->GetFSM()->SetCurrentState(ThiefSeekCitizen::Instance());
		}
	}
	else
	{
		citizen->money -= .1;

		if(citizen->money <= 0)
		{
			citizen->money = 0;
		}

		if(citizen->isOnSamePlaceAsCitizen)
		{
			citizen->isOnSamePlaceAsCitizen = false;
			citizen->GetFSM()->SetCurrentState(ThiefSeekCitizen::Instance());
		}
	}
}


void ThiefRunTowardsCitizen::Exit(Thief* citizen)
{
  // printf("CitizenWonder::Exit");
}

//-----------------------------------------------------------------------------

ThiefFollowPolicemanToJail* ThiefFollowPolicemanToJail::Instance()
{
  static ThiefFollowPolicemanToJail instance;

  return &instance;
}

void ThiefFollowPolicemanToJail::Enter(Thief* citizen)
{
}


void ThiefFollowPolicemanToJail::Execute(Thief* citizen)
{  
	
}


void ThiefFollowPolicemanToJail::Exit(Thief* citizen)
{
  // printf("CitizenWonder::Exit");
}