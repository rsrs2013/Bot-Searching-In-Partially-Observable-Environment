#include "Location.cpp"
class State
{
	private:
		/* Basic Properties of the state */
		Location *location;
		/* Used for the maze generation */
		bool visited;
		/* True if the cell is blocked */
		bool block;

		/* Used for the A* Search */
		double f;
		double g;
		double h;
		double search;
		State* tree;
		enum Actions { LEFT , RIGHT , UP , DOWN };
		int* actionCost;

	public:
        /* Constructor for initializing the State */
		State(Location* l , int gridSize );

		/* Set and Get methods for visited member */
		bool isVisited();
		void setVisited(bool value);

		/* Set and Get Methods for block member */
		bool isBlocked();
		void setBlocked(bool value);

		/* Set and Get methods for member f */
		double getF();
		double setF(double value);

		/* Set and Get methods for member f */
		double getG();
		double setG(double value);

		/* Set, Get and compute methods for the member h */
		double getH();
		double setH(double value);
		void computeH(Location* target , int type ); // Type represents the Heuristic type ex: Manhattan, Eucledian etc...
        void computeHNew(double gd);

        /* Set and Get methods for the search member */
		double getSearch();
		double setSearch(double value);

		/* Returns the Action Cost for the action i */
		/* action = 0 means LEFT
           action = 1 means RIGHT
           action = 2 means UP
           action = 3 means DOWN */
		double getActionCost(int action);

		/* Set Action cost to INF if we found a blocked cell */
		void setActionCost(int action, int value);

		/* Used for backtracking to get the computed path */
		State* getTree();
		void setTree(State* s);

		/* Print the location of this state */
		void printLocation();
		Location* getLocation();

		/* Returns true if both states are equal */
		bool isEqual(State* s);

};

/* Constructor for the State Class
   Initializes all variables of this class
   Sets Action Cost to INF if the State is at
   the grid edges */
State::State(Location* l , int gridSize )
{
	this->location = l;
	this->g = 0;
	this->f = 0;
	this->h = INF;
	this->search = 0;
	this->visited = false;
	this->block = false;
	this->tree = NULL;
	actionCost =  new int[4];

	if( l->getX() == 0 )
		actionCost[0] = INF;
	else
		actionCost[0] = 1;

	if( l->getX() == gridSize - 1 )
		actionCost[1] = INF;
	else
		actionCost[1] = 1;

	if( l->getY() == gridSize - 1 )
		actionCost[2] = INF;
	else
		actionCost[2] = 1;

	if( l->getY() == 0 )
		actionCost[3] = INF;
	else
		actionCost[3] = 1;
}

bool State::isVisited()
{
	return visited;
}

bool State::isBlocked()
{
	return block;
}

void State::setVisited(bool value)
{
	visited = value;
}

void State::setBlocked(bool value)
{
	block = value;
}

double State::getF()
{
	return f;
}

double State::getG()
{
	return g;
}

double State::getH()
{
	return h;
}

double State::getSearch()
{
	return search;
}

double State::setF(double value)
{
	f = value;
}

double State::setG(double value)
{
	g = value;
	f = g + h;
}

double State::setH(double value)
{
	h = value;
	f = g + h;
}

void State::computeHNew(double gd)
{
    //cout << "Old h : " << h << " new h : " << gd - g << endl;
    setH(gd-g);
}

double State::setSearch(double value)
{
	search = value;
}

double State::getActionCost(int action)
{
	return actionCost[action];
}

void State::setActionCost(int action, int value)
{
	actionCost[action] = value;
}

void State::computeH(Location * target , int type )
{
	/* Type 1 corresponds to the Manhattan Distance */
	if( type == 1 )
	{
		double distance = abs(target->getX() - location->getX()) + abs(target->getY() - location->getY());
		setH(distance);
	}
}

State* State::getTree()
{
	return tree;
}

void State::setTree(State *s )
{
	this->tree = s;
}

void State::printLocation()
{
	cout << "(" << location->getX() << "," << location->getY() << ")";
}

Location* State::getLocation()
{
	return location;
}

bool State::isEqual(State *s)
{
	if( this->location->getX() == s->getLocation()->getX() && this->location->getY() == s->getLocation()->getY() )
	{
		return true;
	}
	else
	{
		return false;
	}
}
