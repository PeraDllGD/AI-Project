#include "StdAfx.h"

#include "Police.h"
#include "Utilities.h"
#include "Vector2D.h"


Police::Police(int id, Graph* graph, float radius): GameObject(entityTypes::agent, Vector2D<double>(-1, -1), 0)
	{
		thiefCaught= false;

		m_radius = radius;

		m_graph = graph;

		m_pStateMachine = new StateMachine<Police>(this);

		movementSpeed = 6;

		static int count = 0;
		
		name << "Policeman " << count;
		count++;

		cout << name << endl;
		
		setPos(m_graph->getRandomNode()->getPos());

		m_pStateMachine->SetCurrentState(PoliceWonder::Instance());
	}

void Police::draw(float radius)
{

	double angle1 = 0.0;        

	glPushMatrix();

		glTranslated(m_pos.x, m_pos.y, 0);

		glColor3f(.1, .1, 1);
	
		glBegin(GL_QUADS);
			glVertex2f(radius,radius);
			glVertex2f(-radius,radius);
			glVertex2f(-radius, -radius);
			glVertex2f(radius, -radius);
		glEnd();

		// it would be best to use angle for vectors but for some reason it doesn't work.. so this is a little hack :)
		if(getPos().x < m_path->getCurrentPoint().x && abs(	getPos().x - m_path->getCurrentPoint().x) > abs(	getPos().y - m_path->getCurrentPoint().y))
		{
			glRotatef(-90, 0, 0, 1);
		}
		else if(getPos().x > m_path->getCurrentPoint().x && abs(	getPos().x - m_path->getCurrentPoint().x) > abs(	getPos().y - m_path->getCurrentPoint().y))
		{
			glRotatef(90, 0, 0, 1);
		}
		else if(getPos().y > m_path->getCurrentPoint().y )
		{
			glRotatef(180, 0, 0, 1);
		}
		else
		{
			glRotatef(0, 0, 0, 1);
		}

		

		glColor3f(.1, .1, 1);
		glBegin(GL_TRIANGLE_FAN);
			glVertex2f(radius,radius);
			glVertex2f(-radius,radius);
			glVertex2f(0, radius + 5);
		glEnd();

		static int counterForBlinkLight = 0;

		counterForBlinkLight++;

		if(counterForBlinkLight % 50 > 20 && shouldSireneBeOn)
		{
			glColor3f(1, .1, .1);
			glBegin(GL_QUADS);
				glVertex2f(radius - 2,radius - 2);
				glVertex2f(-radius + 2, radius - 2);
				glVertex2f(-radius + 2, radius - 6);
				glVertex2f(radius - 2, radius - 6);
			glEnd();
		}

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

   glPopMatrix();
}

void Police::Update(float deltaTime)
{
	move(deltaTime);
    m_pStateMachine->Update();
}

//-----------------------------------------------------------------------------

PoliceWonder* PoliceWonder::Instance()
{
  static PoliceWonder instance;

  return &instance;
}

void PoliceWonder::Enter(Police* citizen)
{
	citizen->shouldSireneBeOn = false;

	citizen->thiefCaught= false; 

	GraphNode * start =	citizen->m_graph->getNodeByPosition(citizen->getPos());
	GraphNode * end =	citizen->m_graph->getRandomNode();

	citizen->setPos(start->getPos()) ;

	AStar* astar = AStar::shortestPath(citizen->m_graph, start, end);
	citizen->setPath(astar);

	citizen->movementSpeed = 5;
}


void PoliceWonder::Execute(Police* citizen)
{  
	if(citizen->isFinishedMovement)
	{
		citizen->GetFSM()->SetCurrentState(PoliceWonder::Instance());
	}

	for (std::list<Thief*>::iterator it= citizen->getListOfThieves().begin(); it != citizen->getListOfThieves().end(); ++it)
	{
		if((*it)->GetFSM()->CurrentState() ==  ThiefRunTowardsCitizen::Instance() && ((*it)->getPos().distanceTo(citizen->getPos()) < 50) )
		{
			citizen->current_thief = (*it);
			citizen->GetFSM()->SetCurrentState(PoliceChase::Instance());
		}
	}
}


void PoliceWonder::Exit(Police* citizen)
{
}

//-----------------------------------------------------------------------------

PoliceChase* PoliceChase::Instance()
{
  static PoliceChase instance;

  return &instance;
}

void PoliceChase::Enter(Police* citizen)
{
	std::cout << "PoliceChase: " << citizen->name.str() << endl;

	citizen->shouldSireneBeOn = true;

	GraphNode * start =	citizen->m_graph->getNodeByPosition(citizen->getPos());
	GraphNode * end =	citizen->m_graph->getNodeByPosition(citizen->current_thief->getPos());
	
	AStar* astar = AStar::shortestPath(citizen->m_graph, start, end);
	citizen->setPath(astar);

	citizen->movementSpeed = 10;
}


void PoliceChase::Execute(Police* citizen)
{  
	// citizen->move();
	if(citizen->isFinishedMovement && citizen->thiefCaught == false)
	{
		citizen->GetFSM()->SetCurrentState(PoliceChase::Instance());
	}

	if(citizen->current_thief->GetFSM()->CurrentState() !=  ThiefRunTowardsCitizen::Instance() )
	{
		citizen->GetFSM()->SetCurrentState(PoliceWonder::Instance());
	}
	
	if( citizen->current_thief->isCaught)
	{
		citizen->GetFSM()->SetCurrentState(PoliceWonder::Instance());
	}

	if(citizen->current_thief->getPos().distanceTo(citizen->getPos()) < 10)
	{
		cout << "Thief caught!" << endl;

		citizen->thiefCaught = true;
		citizen->current_thief->isCaught = true;
		citizen->setPos(citizen->current_thief->getPos());

		citizen->current_thief->GetFSM()->ChangeState(ThiefFollowPolicemanToJail::Instance());

		citizen->GetFSM()->SetCurrentState(PoliceExecuteToJail::Instance());

		// mozemo da dodamo tipa da mu je ovaj dao neke pare (korumpiran) pa da ga ne odvece u zatvor!
	}
	
}


void PoliceChase::Exit(Police* citizen)
{
}


//-----------------------------------------------------------------------------

PoliceExecuteToJail* PoliceExecuteToJail::Instance()
{
  static PoliceExecuteToJail instance;

  return &instance;
}

void PoliceExecuteToJail::Enter(Police* citizen)
{
	std::cout << "PoliceExecuteToJail: " << citizen->name.str() << endl;

	GraphNode * start =	citizen->m_graph->getNodeByPosition(citizen->getPos());
	GraphNode * end =	citizen->m_graph->getNodeByPosition(citizen->m_graph->getJailEntrence()->getPos());
	
	AStar* astar = AStar::shortestPath(citizen->m_graph, start, end);
	citizen->setPath(astar);

	citizen->movementSpeed = 9;
}


void PoliceExecuteToJail::Execute(Police* citizen)
{  

	citizen->current_thief->setPos(citizen->getPos());

	// citizen->move();
	if(citizen->isFinishedMovement)
	{
		citizen->current_thief->reset();
		citizen->GetFSM()->SetCurrentState(PoliceWonder::Instance());
	}


}


void PoliceExecuteToJail::Exit(Police* citizen)
{
}

