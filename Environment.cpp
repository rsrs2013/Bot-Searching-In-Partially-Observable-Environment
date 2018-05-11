#define INF pow(10,9)
#include "State.cpp"

/* Environment class used to denote the environment the Agents use */
class Environment
{
	private:
		int gridSize;                                       // GridSize - nxn size
		int blockGridRate;                                  // denotes the probability of a cell being unblocked
		State ***cell;                                      // Denotes each cell in the environment

	public:
		Environment(int size, int blockRate, bool Agent);   // Constructor for the Environment class
		void generateGrid();                                // Constructs the maze using DFS with Backtracking
		void generateCells();                               // Initializes the cells in the Environment
		void generateOutput();                              // Output the Environment
		void writeToFile(ostream & output);
		void readFromFile(istream & input);
		void animateFile(Location *startLocation, Location* targetLocation);
		State* getCell(int x, int y);                       // Returns the cell based on its x,y coordinate
		State* getState(Location * l);                      // Returns the cell state based on the location class
		int getGridSize();                                  // Used to return the gridSize of the environment
		int countExplored();                                // Counts the number of explored cells by the Agent.

};


Environment::Environment(int size, int blockRate, bool Agent)
{
	gridSize = size;
	blockGridRate = blockRate;
	generateCells();
	if( !Agent )
		generateGrid();

}

int Environment::getGridSize(){

	return gridSize;
}

void Environment::generateOutput()
{
	cout << " Environment Output " << endl;
	for( int i = 0  ; i < gridSize ; i++ )
	{
		for( int j = 0 ;  j < gridSize ; j++ )
		{
			if( cell[i][j]->isBlocked() )
			{
				cout << "#";
			}
			else
			{
				cout << "_";
			}
		}
		cout << endl;
	}
}

void Environment::writeToFile(ostream & output)
{
    for( int i = 0  ; i < gridSize ; i++ )
	{
		for( int j = 0 ;  j < gridSize ; j++ )
		{
			if( cell[i][j]->isBlocked() )
			{
				output << "1 ";
			}
			else
			{
				output << "0 ";
			}
		}
		output << endl;
	}
}

void Environment::readFromFile(istream & input)
{
    for( int i = 0  ; i < gridSize ; i++ )
	{
		for( int j = 0 ;  j < gridSize ; j++ )
		{
		    char s;
		    input >> s;

			if( s == '1' )
			{
				cell[i][j]->setBlocked(true);
			}
		}
	}
}

void Environment::generateCells()
{
	cell = new State**[gridSize];
	for( int i = 0 ; i < gridSize ; i++ )
	{
		cell[i] = new State*[gridSize];
		for( int j = 0 ; j < gridSize ; j++ )
		{
			cell[i][j] = new State(new Location(i,j), gridSize );
		}
	}
}

void Environment::generateGrid()
{
	/* Choose a Random Location x and y */
	int location_x = rand() % gridSize;
	int location_y = rand() % gridSize;
	int unVisitedCells = gridSize*gridSize;

	/* Initialize the stack */
	stack<State*> visited;

	/* Set the initial location as visited and mark it as currentCell */
	State * currentCell = cell[location_x][location_y];
	cell[location_x][location_y]->setVisited(true);
	unVisitedCells--;

	while(unVisitedCells)
	{
		/*Set current cell and neighbout information */
		vector<State*> neighbour;
		//currentCell->printLocation();
		//cout << " Unvisited Cells : " << unVisitedCells << endl;
		location_x = currentCell->getLocation()->getX();
		location_y = currentCell->getLocation()->getY();

		/* Add unvisited neighbour to the list of neighbours */

		if( location_x + 1 < gridSize )
		{
			if( cell[location_x+1][location_y]->isVisited() == false )
			{
				neighbour.push_back(cell[location_x+1][location_y]);
			}
		}

		if( location_y + 1 < gridSize )
		{
			if( cell[location_x][location_y+1]->isVisited() == false )
			{
				neighbour.push_back(cell[location_x][location_y+1]);
			}
		}

		if( location_x - 1 >= 0 )
		{
			if( cell[location_x-1][location_y]->isVisited() == false )
			{
				neighbour.push_back(cell[location_x-1][location_y]);
			}
		}

		if( location_y - 1 >= 0 )
		{
			if( cell[location_x][location_y-1]->isVisited() == false )
			{
				neighbour.push_back(cell[location_x][location_y-1]);
			}
		}

		/* If there is a unvisited neighbour */
		if( neighbour.size() > 0 )
		{
			int randomNeighbour = rand() % neighbour.size();
			visited.push(currentCell);
			currentCell = neighbour[randomNeighbour];
			currentCell->setVisited(true);
			unVisitedCells--;
			if( ( rand() % 100 ) > blockGridRate )
			{
				currentCell->setBlocked(true);
			}
		}
		else
		{
			if( !visited.empty() )
			{
				currentCell = visited.top();
				visited.pop();
			}
			else
			{
				if( unVisitedCells != 0 )
				{
					for( int i = 0 ; i < gridSize ; i++ )
					{
						for( int j = 0 ; j < gridSize ; j++ )
						{
							if( cell[i][j]->isVisited() == false )
							{
								currentCell = cell[i][j];
							}
						}
					}
				}
			}
		}

		neighbour.clear();
	}
	cout << " Environment Generated" << endl;
}

State* Environment::getCell(int x , int y)
{
	return cell[x][y];
}

State* Environment::getState(Location *l )
{
	return cell[l->getX()][l->getY()];
}
