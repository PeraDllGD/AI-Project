#include "StdAfx.h"

#include "Grid.h"


void breathFirstSearch(int currentRow, int currentColumn, GraphNode*** maze, int rows, int columns) {
    /* 4 random directions */
    int* randDirections = new int[4];

	// cout << "Current -> " << currentRow << ":" << currentColumn << endl;

    /* Initialize with 1-4 */
    for(int i = 0; i < 4; i++) {
        randDirections[i] = i + 1;
    }

    /* Shuffle */
    random_shuffle(&randDirections[0], &randDirections[3]);

	
    /* Examine each direction */
    for(int i = 0; i < sizeof(randDirections); i++) {
        
		//cout << "randDirections -> " << randDirections[i] << endl;


		switch(randDirections[i]) {
            
			/* Up */
            case 1:
                /* Whether 2 cells up is out or not */

				if(maze[currentRow - 1][currentColumn]->getTerrain() == GraphNode::terrainType::building && rand() % 5 == 0) {
                    maze[currentRow - 1][currentColumn]->setTerain(GraphNode::terrainType::road);
                }

                if(currentRow - 2 <= 0)
                    continue;
				if(maze[currentRow - 2][currentColumn]->getTerrain() == GraphNode::terrainType::building) {
                    maze[currentRow - 2][currentColumn]->setTerain(GraphNode::terrainType::road);
                    maze[currentRow - 1][currentColumn]->setTerain(GraphNode::terrainType::road);
                    breathFirstSearch(currentRow - 2, currentColumn, maze, rows, columns);
                }

				

                break;
            
			/* Right */
            case 2:
                /* Whether 2 cells right is out or not */

				if(maze[currentRow][currentColumn + 1]->getTerrain() == GraphNode::terrainType::building && rand() % 5 == 0) {
                    maze[currentRow][currentColumn + 1]->setTerain(GraphNode::terrainType::road);
                }

                if(currentColumn + 2 >= columns - 1)
                    continue;
                
				if(maze[currentRow][currentColumn + 2]->getTerrain() == GraphNode::terrainType::building) {
                    maze[currentRow][currentColumn + 2]->setTerain(GraphNode::terrainType::road);
                    maze[currentRow][currentColumn + 1]->setTerain(GraphNode::terrainType::road);
                    breathFirstSearch(currentRow, currentColumn + 2, maze, rows, columns);
                }

				

                break;
            
			/* Down */
            case 3:
                /* Whether 2 cells down is out or not */

				if(maze[currentRow + 1][currentColumn]->getTerrain() == GraphNode::terrainType::building && rand() % 5 == 0) {
                    maze[currentRow + 1][currentColumn]->setTerain(GraphNode::terrainType::road);
                }

				if(currentRow + 2 >= rows - 1)
                    continue;
                if(maze[currentRow + 2][currentColumn]->getTerrain() == GraphNode::terrainType::building) {
                    maze[currentRow + 2][currentColumn]->setTerain(GraphNode::terrainType::road);
                    maze[currentRow + 1][currentColumn]->setTerain(GraphNode::terrainType::road);
                    breathFirstSearch(currentRow + 2, currentColumn, maze, rows, columns);
                }

				

                break;
            /* Left */
            case 4:
                /* Whether 2 cells up is out or not */

				if(maze[currentRow][currentColumn - 1]->getTerrain() == GraphNode::terrainType::building && rand() % 5 == 0) {
                    maze[currentRow][currentColumn - 1]->setTerain(GraphNode::terrainType::road);
                }

                if(currentColumn - 2 <= 0)
                    continue;
                if(maze[currentRow][currentColumn - 2]->getTerrain() == GraphNode::terrainType::building) {
                    maze[currentRow][currentColumn - 2]->setTerain(GraphNode::terrainType::road);
                    maze[currentRow][currentColumn - 1]->setTerain(GraphNode::terrainType::road);
                    breathFirstSearch(currentRow, currentColumn - 2, maze, rows, columns);
                }

				
                break;
        }
    }
}

Grid::Grid(int collumn, int row, int cellSize) : GameObject(entityTypes::terrain, Vector2D<double>(-1, -1), 0) {
    
	cout << "Creating Grid" << endl;

	graph = new Graph(collumn, row, cellSize);

	vector<GraphNode*> nodes;

    int index = 0;

	GraphNode*** map = new GraphNode**[row];
	for (int i = 0; i < row; i++)
	{
		map[i] = new GraphNode*[collumn];
	}

	for (int i = 0; i < row; i++) {
		for (int j = 0; j < collumn; j++) {

			GraphNode::terrainType terrain = GraphNode::terrainType::building;

			// GraphNode::terrainType terrain = static_cast<GraphNode::terrainType>(rand() % GraphNode::terrainType::water);

            GraphNode* node = new GraphNode(index,
                                        Vector2D<double>(i * cellSize + cellSize / 2,
                                                         j * cellSize + cellSize / 2),
                                        terrain, cellSize);
            index++;
            nodes.push_back(node);
			map[i][j] = node;
        }
    }

	breathFirstSearch(row / 2, collumn / 2, map, row, collumn);

	// detect jail
	for (int i = row / 2 - 1; i < row / 2 + 2; i++) 
	{
        for (int j = collumn / 2 - 1; j < collumn / 2 + 1; j++) 
		{
			map[i][j]->setTerain(GraphNode::terrainType::jail);
		}
	}

	// make entrence to jail
	map[row / 2][collumn / 2 + 1]->setTerain(GraphNode::terrainType::jail);

	map[row / 2][collumn / 2 + 2]->setTerain(GraphNode::terrainType::road);

	graph->setJailEntrence(map[row / 2][collumn / 2 + 2]);

	// add police station
	for (int i = 0; i < 5; i++) 
	{
		map[i][collumn - 1]->setTerain(GraphNode::terrainType::police_station);
	}

	map[0][collumn - 2]->setTerain(GraphNode::terrainType::police_station);
	map[1][collumn - 2]->setTerain(GraphNode::terrainType::police_station);
	map[3][collumn - 2]->setTerain(GraphNode::terrainType::police_station);
	map[4][collumn - 2]->setTerain(GraphNode::terrainType::police_station);

	map[0][collumn - 3]->setTerain(GraphNode::terrainType::police_station);
	map[4][collumn - 3]->setTerain(GraphNode::terrainType::police_station);

	// add road to station

	for (int i = 1; i < 6; i++) 
	{
		map[2][collumn - 1 - i]->setTerain(GraphNode::terrainType::road);
	}
	
	graph->setPoliceStation(map[2][collumn - 2]);

	// map[row / 2][collumn / 2 + 2]

	graph->setNodes(nodes);

	for (int i = 0; i < row; i++) {
        for (int j = 0; j < collumn; j++) {
            
			if (!map[i][j]->isRoad()) 
				continue;

            for (int x = -1; x <= 1; x++) {
                for (int y = -1; y <= 1; y++) {

					if(x == 0 && y == 0)
					{
						continue;
					}
						
                    if (i + x >= 0 &&
                        i + x < row &&
                        j + y >= 0 &&
                        j + y < collumn 
                        && map[i + x][j + y]->isRoad()
						) {

						if(	(x == -1 && y == -1) ||
							(x == 1 && y == -1) ||
							(x == -1 && y == 1) ||
							(x == 1 && y == 1))
						{
							continue;
						}
							
                        graph->addEdge(GraphEdge(map[i][j],
                                                 map[i + x][j + y],
                                                 map[i][j]->getTerrain()));

						if(x == -1 && y == 0)
						{
							map[i][j]->isLeft = true;
							map[i + x][j + y]->isRight = true;
						}
						else if(x == 1 && y == 0)
						{
							map[i][j]->isRight = true;
							map[i + x][j + y]->isLeft = true;
						}
						else if(x == 0 && y == 1)
						{
							map[i][j]->isTop = true;
							map[i + x][j + y]->isBottom = true;
						}
						else if(x == 0 && y == -1)
						{
							map[i][j]->isBottom = true;
							map[i + x][j + y]->isTop = true;
						}

                    }
					else
					{
						// cout << "NIJE DODAT" << endl;
					}
                }
            }
        }
    }

	for (auto i = 0; i < row; i++) {
		delete[] map[i];
	}

	delete[] map;
}

void Grid::render() const {
    graph->render();
}

