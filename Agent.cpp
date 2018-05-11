#include "BinaryHeap.cpp"
#define DEBUG 0
#define OUTPUT 0
/*  Class Agent denotes the Agent in the Environment. It has the following characteristics
    - Search Agent
    - Observability : Partial - 4 neighbouring cells
    - Algorithms : Repeated Forward and Backward A*, Adaptive A* Search
*/

class Agent
{
    private:
        /* Denotes the current location of the Agent */
        Location* currentLocation;
        /* Start location for the A* search */
        Location* startLocation;
        /* Goal location for the A* search */
        Location* goalLocation;
        /* gridSize of Agent Environment */
        int gridSize;
        /* Agent Environment */
        Environment *E;
        /* Denotes how to break ties between f values */
        bool small;
        /* Denote the total cost to reach the target/ determine the target cannot be reached */
        int cost;
        /* Sum of all expanded cells at each A* search */
        int expandedCells;

        BinaryHeap *open;
        vector<State*> closed;
        vector<State*> path;

    public:
        Agent(Location *current , int gridSize , bool small );
        vector<State*> getTreePath(State* goal, State *start );
        void updateEnvironment(State* currentState, Environment *actualEnvironment);
        void computePath(int counter);
        void AStarForwardSearch(Location* target, Environment *actualEnvironment );
        void AStarBackwardSearch(Location* target, Environment *actualEnvironment);
        void AdaptiveAStarForwardSearch( Location* target , Environment* actualEnvironment );
        void printAgentEnvironment(Location* currentLocation, Location* targetLocation );
        void printPath(vector<State*> computedPath);
        void printToFile(vector<State*> computedPath,Location *startLocation , Location* targetLocation);
        int getCost();
        int getExpandedCells();

};

/* Constructor for the Agent class
   The variable small is used to denote how to break ties
   between the same f-values in the binary heap. IF small is set
   to true then the lower g-value is given more priority and if small
   is set to be false then the higher g-value is given more priority */
Agent::Agent(Location * currentLocation , int gridSize , bool small )
{
    this->currentLocation = currentLocation;
    this->E = new Environment(gridSize,100,true);
    this->small = small;
    this->gridSize = gridSize;
    this->cost = 0;
    this->expandedCells = 0;
}

/* This function used to get the computed path from the goal state
   to the start state using the tree pointers assigned during the A* search */
vector<State*> Agent::getTreePath(State* goal , State* start )
{
    vector<State*> path;
    State* current = goal;

    /* loop till current state reaches the start state */
    while( !current->isEqual(start) )
    {
        path.push_back(current);
        current = current->getTree();
    }

    path.push_back(start);
    reverse(path.begin(), path.end());

    return path;

}

/*  This function is used to update the Agent Environment . The parameter actualEnvironment
    denotes the actual environment and the currentState represents the current state of the
    Agent in its environment. Here the observable states of the environment is its neighbouring
    four cells */

/*
    If any of the neighbouring cells are found to be blocked then update the action cost to that
    blocked cell from its neighbour to INF. By making that we are avoiding finding a path going through the blocked
    cell.
*/

void Agent::updateEnvironment(State* currentState, Environment *actualEnvironment)
{

    int locationX = currentState->getLocation()->getX();
    int locationY = currentState->getLocation()->getY();

    if( locationX > 0 )
    {
        /* If the left cell to the actual cell is blocked */
        if( actualEnvironment->getCell(locationX-1,locationY)->isBlocked() )
        {
            E->getCell(locationX-1,locationY)->setBlocked(true);

            /* Left action to the blocked cell cost is set to INF */
            E->getCell(locationX,locationY)->setActionCost(0,INF);

            /* Right action cost to the blocked cell is set to INF */
            if( locationX - 2 >= 0 )
            {
                E->getCell(locationX-2,locationY)->setActionCost(1,INF);
            }

            /* Up action cost to the blocked cell is set to INF */
            if( locationY - 1 >= 0 )
            {
                E->getCell(locationX-1,locationY-1)->setActionCost(2,INF);
            }

            /* Down action to the blocked cell is set to INF */
            if( locationY + 1 < gridSize )
            {
                E->getCell(locationX-1,locationY+1)->setActionCost(3,INF);
            }

        }
    }

    if( locationX < gridSize - 1 )
    {
        /* If the right cell to the current cell is blocked */
        if( actualEnvironment->getCell(locationX+1,locationY)->isBlocked() )
        {
            E->getCell(locationX+1,locationY)->setBlocked(true);

            if( locationX + 2 < gridSize )
            {
                E->getCell(locationX+2,locationY)->setActionCost(0,INF);
            }

            E->getCell(locationX,locationY)->setActionCost(1,INF);

            if( locationY - 1 >= 0 )
            {
                E->getCell(locationX+1,locationY-1)->setActionCost(2,INF);
            }

            if( locationY + 1 < gridSize )
            {
                E->getCell(locationX+1,locationY+1)->setActionCost(3,INF);
            }
        }
    }

    if( locationY < gridSize - 1 )
    {
        /* If the upward cell from the current cell is blocked */
        if( actualEnvironment->getCell(locationX,locationY+1)->isBlocked() )
        {
            E->getCell(locationX,locationY+1)->setBlocked(true);

            if( locationX + 1 < gridSize )
            {
                E->getCell(locationX+1,locationY+1)->setActionCost(0,INF);
            }

            if( locationX - 1 >= 0 )
            {
                E->getCell(locationX-1,locationY+1)->setActionCost(1,INF);
            }

            E->getCell(locationX,locationY)->setActionCost(2,INF);

            if( locationY + 2 < gridSize )
            {
                E->getCell(locationX,locationY+2)->setActionCost(3,INF);
            }
        }
    }

    if( locationY > 0 )
    {
        /* If the downward cell from the current cell is blocked */
        if( actualEnvironment->getCell(locationX,locationY-1)->isBlocked() )
        {
            E->getCell(locationX,locationY-1)->setBlocked(true);

            if( locationX + 1 < gridSize )
            {
                E->getCell(locationX+1,locationY-1)->setActionCost(0,INF);
            }

            if( locationX - 1 >= 0 )
            {
                E->getCell(locationX-1,locationY-1)->setActionCost(1,INF);
            }

            if( locationY - 2 >= 0 )
            {
                E->getCell(locationX,locationY-2)->setActionCost(2,INF);
            }

            E->getCell(locationX,locationY)->setActionCost(3,INF);
        }
    }

}

/*  Compute the unblocked persumed path from the start location to the end location
    using A* search. It sets the tree pointers for every state from the start to the goal
    so that we can use backtracking to compute the path after this function has completed its execution*/
void Agent::computePath(int counter)
{

   while( open->top() != NULL && E->getState(goalLocation)->getG() > open->top()->getF() )
    {
        State *s = open->extractMin();
        closed.push_back(s);
        //cout << "x : " << s->getLocation()->getX() << " y: " << s->getLocation()->getY() << " g: " << s->getG() << " h: " << s->getH() << endl;
        /* sccState denotes the next state based on the action succ(s,a) */
        State* succState = NULL;

        for( int i = 0  ; i < 4 ; i++ )
        {
            /* An Action in the state is valid only if its Action cost
               is not INF. This makes sure that the Agent stays within the
               grid and also the A* doesnt find any path through the blocked cell */
            if( s->getActionCost(i) != INF )
            {
                /* Get the successor state based on the Action */
                if( i == 0 ) // Action = LEFT
                {
                    succState = E->getCell(s->getLocation()->getX()-1 , s->getLocation()->getY() );
                }
                else if( i == 1 ) // Action = RIGHT
                {
                    succState = E->getCell(s->getLocation()->getX()+1 , s->getLocation()->getY() );
                }
                else if( i == 2 ) // Action = UP
                {
                    succState = E->getCell(s->getLocation()->getX() , s->getLocation()->getY()+1 );
                }
                else if( i == 3 ) // ACTION = DOWN
                {
                    succState = E->getCell(s->getLocation()->getX() , s->getLocation()->getY()-1 );
                }

                /* If we are exploring the state for the first time then the set G value to INF
                   set search to counter so it is not set INF value again and also compute H value
                   of that state since it is only recently found */

                if( succState->getSearch() < counter )
                {
                    succState->setG(INF);
                    succState->setSearch(counter);

                    if( succState->getH() == INF )
                    {
                        succState->computeH(goalLocation,1);
                    }
                }

                /* If succState has shortest path through s then update the g and tree value for the succState */
                if( succState->getG() > s->getG() + s->getActionCost(i) )
                {

                    succState->setG(s->getG() + s->getActionCost(i));
                    succState->setTree(s);

                    /* If the successor state not present in the OPEN list then add it */
                    if( open->search(succState) == -1 )
                    {
                        open->minHeapInsert(succState);
                    }
                    /* Else just decrease the g value of the succState to new G */
                    else
                    {
                        open->decreaseKey(succState,succState->getG());
                    }
                }

            }
        }

    }
}

/*
    This is the repeated forward A* search which does the following
    - Computes an unblocked persumed path from start to goal ( Start to Goal - Forward )
    - Moves along the computed path till it reaches a blocked state or goal state
    - After every move it updates the agent environment
    - If it reaches a blocked state along its path it finds a new unblocked persumed path based on the updated environment
    - Repeat the above steps 2-4 till it reaches the goal state
*/

void Agent::AStarForwardSearch(Location* target , Environment* actualEnvironment )
{
    int counter = 0;

    startLocation = currentLocation;
    goalLocation = target;

    while( E->getState(startLocation)->isEqual(E->getState(goalLocation)) == false )
    {
        /* Increase the counter by 1 */
        counter = counter + 1;

        /* Set the G value for start state to 0 and set the Search value to counter value */
        E->getState(startLocation)->setG(0);
        E->getState(startLocation)->setSearch(counter);



        /* Set the goal state G and search to INF and counter value */
        E->getState(goalLocation)->setG(INF);
        E->getState(goalLocation)->setSearch(counter);

        /* Set the OPEN and CLOSED lists to NULL
           Here the OPEN list is a binary Heap */
        open = new BinaryHeap(this->small);
        closed.clear();

        /* Compute H value for the start cell */
        E->getState(startLocation)->computeH(goalLocation,1);
        /* insert start state into the OPEN list */
        open->minHeapInsert(E->getState(startLocation));

        /* Do the A* search */
        computePath(counter);

        /* If the OPEN list is empty it means it cannot reach the target */
        if( open->isEmpty() )
        {
            cout << " Cannot reach the target :( " << endl;
            return;
        }

        /* Count the expanded cells */
        expandedCells = expandedCells + closed.size();

        /* Get the whole path the agent has to move by backtracking the tree pointers */
        vector<State*> computedPath = getTreePath(E->getState(goalLocation), E->getState(startLocation));

        if(DEBUG)
            printPath(computedPath);

        /* Move the agent till it reaches the goal state or the blocked cell along the computed path */
        State* currentState;
        for( int i = 0 ; i < computedPath.size() ; i++ )
        {
            /* Moves the agent along the path */
            currentState = computedPath[i];
            if (i > 0)
                this->cost++;
            /* Update the Environment after moving the Agent */
            updateEnvironment(currentState,actualEnvironment);

            if( DEBUG )
            {
                printAgentEnvironment(currentState->getLocation(), goalLocation );
                _sleep(1000);
            }

            if(OUTPUT)
                printToFile(computedPath,startLocation,goalLocation);
            /* If the Agent reaches the destination then return */
            if( i == computedPath.size() - 1 )
            {
                cout << "Reached the Target :) " << endl;
                return;
            }

            /* If the next step in the path is blocked then break since the cost along the paths are updated now */
            if( computedPath[i+1]->isBlocked() )
                break;
        }

        /* Set the new start location and proceed to find another unblocked path */
        currentLocation = currentState->getLocation();
        startLocation = currentLocation;
    }
}

/*
    This is the repeated forward A* search which does the following
    - Computes an unblocked persumed path from Goal to Start ( Goal to Start - Backward )
    - Moves along the computed path till it reaches a blocked state or goal state
    - After every move it updates the agent environment
    - If it reaches a blocked state along its path it finds a new unblocked persumed path based on the updated environment
    - Repeat the above steps 2-4 till it reaches the goal state
*/

void Agent::AStarBackwardSearch(Location *target , Environment* actualEnvironment )
{
    int counter = 0;

    /* Compute path will have startLocation as target and goalLocation as Agent location */
    startLocation = target;
    goalLocation = currentLocation;

    while( E->getState(startLocation)->isEqual(E->getState(goalLocation)) == false )
    {
        /* Increase the counter by 1 */
        counter = counter + 1;

        /* Set the start state G and search to 0 and counter value */
        E->getState(startLocation)->setG(0);
        E->getState(startLocation)->setSearch(counter);



        /* Set the goal state G and search to INF and counter value */
        E->getState(goalLocation)->setG(INF);
        E->getState(goalLocation)->setSearch(counter);

        /* Set the OPEN and CLOSED lists to NULL
           Here the OPEN list is a binary Heap */
        open = new BinaryHeap(small);
        closed.clear();

        /* Compute H value for the start cell */
        E->getState(startLocation)->computeH(goalLocation,1);
        /* insert start state into the OPEN list */
        open->minHeapInsert(E->getState(startLocation));

        /* Compute the path from the goal to the start location */
        computePath(counter);

        /* If OPEN list is empty then the target cannot be reached */
        if( open->isEmpty() )
        {
            cout << " Cannot reach the target " << endl;
            return;
        }

        /* Count the number of expanded cells */
        expandedCells = expandedCells + closed.size();

        /* Get the computed path using the tree pointers in the State class */
        vector<State*> computedPath = getTreePath(E->getState(goalLocation), E->getState(startLocation));

        if(DEBUG)
            printPath(computedPath);

        /* Reverse the computed path since we are moving the agent from the start to goal */
        reverse(computedPath.begin(), computedPath.end());

        /* currentState represents the state of the Agent */
        State* currentState;
        /* Move the agent along the computed path from the goal state */
        for( int i = 0 ; i < computedPath.size() ; i++ )
        {
            /* Moving the agent */
            currentState = computedPath[i];
            /* Increase the cost by 1 */
            this->cost++;
            /* Update the Environment after moving */
            updateEnvironment(currentState,actualEnvironment);

            if( DEBUG )
            {
                printAgentEnvironment(currentState->getLocation(), startLocation );
                _sleep(1000);
            }

            if(OUTPUT)
                printToFile(computedPath,goalLocation,startLocation);
            /* If reached the target then return */
            if( i == computedPath.size() - 1 )
            {
                cout << endl << "Reached the Target :) " << endl;
                return;
            }

            /* If the next cell in the path is blocked then break and then find an unblocked persumed path */
            if( computedPath[i+1]->isBlocked() )
                break;
        }

        currentLocation = currentState->getLocation();
        goalLocation = currentLocation;
    }
}

void Agent::AdaptiveAStarForwardSearch(Location* target , Environment *actualEnvironment )
{
    int counter = 0;

    startLocation = currentLocation;
    goalLocation = target;

    while( E->getState(startLocation)->isEqual(E->getState(goalLocation)) == false )
    {
        /* Increase the counter by 1 */
        counter = counter + 1;

        /* Set the G value for start state to 0 and set the Search value to counter value */
        E->getState(startLocation)->setG(0);
        E->getState(startLocation)->setSearch(counter);



        /* Set the goal state G and search to INF and counter value */
        E->getState(goalLocation)->setG(INF);
        E->getState(goalLocation)->setSearch(counter);

        /* Set the OPEN and CLOSED lists to NULL
           Here the OPEN list is a binary Heap */
        open = new BinaryHeap(small);
        closed.clear();

        /* Compute H value for the start cell */
        if( E->getState(startLocation)->getH() == INF )
            E->getState(startLocation)->computeH(goalLocation,1);

        /* insert start state into the OPEN list */
        open->minHeapInsert(E->getState(startLocation));

        /* Do the A* search */
        computePath(counter);

        /* If the OPEN list is empty it means it cannot reach the target */
        if( open->isEmpty() )
        {
            cout << " Cannot reach the target " << endl;
            return;
        }

        /* Count the expanded cells */
        expandedCells = expandedCells + closed.size();

        /* Get the whole path the agent has to move by backtracking the tree pointers */
        vector<State*> computedPath = getTreePath(E->getState(goalLocation), E->getState(startLocation));

        /* The last State of the computedPath is the goal state */
        /* computedPath[computedPath.size()-1]->getG() will give the goal distance from the start i.e. gd(start) */

        if(DEBUG)
            printPath(computedPath);

        /* Move the agent till it reaches the goal state or the blocked cell along the computed path */
        State* currentState;
        for( int i = 0 ; i < computedPath.size() ; i++ )
        {
            /* Moves the agent along the path */
            currentState = computedPath[i];
            this->cost++;
            /* Update the Environment after moving the Agent */
            updateEnvironment(currentState,actualEnvironment);

            if( DEBUG )
            {
                printAgentEnvironment(currentState->getLocation(), goalLocation );
                _sleep(1000);
            }

            if(OUTPUT)
                printToFile(computedPath,startLocation,goalLocation);
            /* If the Agent reaches the destination then return */
            if( i == computedPath.size() - 1 )
            {
                cout << endl << "Reached the Target :) " << endl;
                return;
            }

            /* If the next step in the path is blocked then break since the cost along the paths are updated now */
            if( computedPath[i+1]->isBlocked() )
                break;
        }

        State* closedState;
        for( int i = 0 ; i < closed.size() ; i++ )
        {
            closed[i]->computeHNew(E->getState(goalLocation)->getG());
            //closed[i]->printLocation();
        }

        /* Set the new start location and proceed to find another unblocked path */
        currentLocation = currentState->getLocation();
        startLocation = currentLocation;
    }
}

void Agent::printToFile(vector<State*> computedPath,Location *startLocation , Location* targetLocation)
{
    fstream out;
    out.open("Animate\\Execution.txt", fstream::out | fstream::app);

    ostringstream oss;
    oss << "{\"grid_size\":" << gridSize << ",\"blocked_cells\":[";

    for( int i = 0 ; i < gridSize ; i++ )
    {
        for( int j = 0 ; j < gridSize ; j++ )
        {
            if( E->getCell(i,j)->isBlocked() )
                oss << "[" << i << "," << j << "],";
        }
    }

    oss << "[-1,-1]],";

    oss << "\"start_location\":[" << startLocation->getX() << "," << startLocation->getY() << "],";
    oss << "\"target_location\":[" << targetLocation->getX() << "," << targetLocation->getY() << "],";
    oss << "\"path\":[";

    for( int i = 0 ; i < computedPath.size() ; i++ )
    {
        oss << "[" << computedPath[i]->getLocation()->getX() << "," << computedPath[i]->getLocation()->getY() << "],";
    }

    oss << "[-1,-1]]}";



    string s = oss.str();
    out << s << endl;

}

/*
    Used to print the Agent Environment.
    The currentLocation parameter denotes the Agent location
    The targetLocation paramter denotes the target location.
*/
void Agent::printAgentEnvironment(Location* currentLocation , Location* targetLocation )
{
    for( int i = 0  ; i < gridSize ; i++ )
	{
		for( int j = 0 ;  j < gridSize ; j++ )
		{
		    /* Blocked cell is denoted by hash # */
			if( E->getCell(i,j)->isBlocked() )
			{
				cout << "#";
			}
			/* Agent location by A */
			else if(currentLocation->getX() == i && currentLocation->getY() == j )
			{
				cout << "A";
			}
			/* Target location by T */
			else if( targetLocation->getX() == i && targetLocation->getY() == j )
			{
			    cout << "T";
			}
			else
			{
			    cout << " ";
			}
		}
		cout << endl;
	}
    /* Denotes the end of the environment */
	cout << "-----" << endl;
}

/*
    Used to print the computed path for debugging purposes
*/
void Agent::printPath(vector<State*> computedPath)
{
    for( int i = 0 ; i < computedPath.size() ; i++ )
    {
        cout << "(" << computedPath[i]->getLocation()->getX() << "," << computedPath[i]->getLocation()->getY() << ")";
    }
    cout << endl;
}

/* Used to get the totalCost taken by the agent to reach from the target to the Goal */
int Agent::getCost()
{
    return cost;
}

/* Used to return the number of expanded cells */
int Agent::getExpandedCells()
{
    return expandedCells;
}
