#include <iostream>
#include <fstream>
#include <ctime>
#include <climits>
#include <cstdlib>
#include <cmath>
#include <ctime>
#include <stack>
#include <vector>
#include <algorithm>
#include <unistd.h>
#include <sstream>

#define GRIDSIZE 101
#define BLOCKRATE 70

#define PART 5


using namespace std;

#include "Environment.cpp"
#include "Agent.cpp"

class AgentTest
{
    private:
        int gridSize;
        int blockRate;
        Environment *E;
        Environment **collection;
        /* Used for comparison */
        int* aCost;
        int* bCost;
        int* aExp;
        int* bExp;
    public:
        AgentTest(int gridSize, int blockRate);
        void generateNGridWorlds(int n);
        void loadNgridWorlds(int n);
        Location* generateRandomLocation(Environment *E);
        void repeatedForwardAStarTest();
        void repeatedBackwardAStarTest();
        void adaptiveForwardAStarTest();
        void compareTiesExample();
        void compareBreakTiesFowardRepeatedAStarTest(int n);
        void compareForwardBackwardRepeatedAStarTest(int n);
        void compareAdaptiveForwardRepeatedAStarTest(int n);
        void getResults(int part);

};

AgentTest::AgentTest(int gridSize , int blockRate )
{
    this->gridSize = gridSize;
    this->blockRate = blockRate;
}

/*
    Parameter : n
    Generates n gridworlds and store it in /gridWorlds/i.txt
    i represents the ith gridWorld i <= n
*/
void AgentTest::generateNGridWorlds(int n)
{
    collection = new Environment*[n];
    for( int i = 1 ; i <= n ; i++ )
    {
        cout << " Environment " << i << endl;
        string path = "gridWorlds\\";
        string filename = ".txt";
        ostringstream oss;
        oss << path << i << filename;
        filename = oss.str();

        ofstream output(filename.c_str());
        if( output.is_open() )
        {
            collection[i-1] = new Environment(gridSize,blockRate,false);
            collection[i-1]->writeToFile(output);
        }
        else
        {
            cout << "Unable to open the file";
        }

        output.close();

    }
}


void AgentTest::loadNgridWorlds(int n)
{
    collection = new Environment*[n];

    for( int i = 1 ; i <= n ; i++ )
    {
        cout << " Environment " << i << endl;

        string path = "gridWorlds\\";
        string filename = ".txt";
        ostringstream oss;
        oss << path << i << filename;
        filename = oss.str();

        ifstream input(filename.c_str());
        if( input.is_open() )
        {
            collection[i-1] = new Environment(gridSize,blockRate,true);
            collection[i-1]->readFromFile(input);
        }
        else
        {
            cout << "Unable to open the file";
        }

        input.close();

    }
    cout << "Finished Reading" << endl;
}

Location* AgentTest::generateRandomLocation(Environment *E)
{

    int gridSize = E->getGridSize(); //get the size of the grid.
    int x_location; //generate the x-coordinate between 0 and grid size.
    int y_location; //generate the y-coordinate between 0 and grid size.
    State* cell ; //variable to get the cell located at x_location and y_location coordinates.

    //Select a random x and y coordinates for the start position of the game.
    do{
     x_location = rand()%gridSize;
     y_location = rand()%gridSize;
     cell = E->getCell(x_location,y_location);
    }while(cell->isBlocked());

    return (new Location(x_location,y_location));
}

void AgentTest::repeatedForwardAStarTest()
{
    E = new Environment(gridSize,blockRate,false);
    Location* startLocation = generateRandomLocation(E);
    Location* targetLocation = generateRandomLocation(E);
    Agent* a = new Agent(startLocation , gridSize , true);
    a->AStarForwardSearch(targetLocation,E);
}

void AgentTest::repeatedBackwardAStarTest()
{
    E = new Environment(gridSize,blockRate,false);
    Location* startLocation = generateRandomLocation(E);
    Location* targetLocation = generateRandomLocation(E);
    Agent* a = new Agent(startLocation , gridSize , true);
    a->AStarBackwardSearch(targetLocation,E);
}

void AgentTest::adaptiveForwardAStarTest()
{
    this->gridSize = 20;
    this->blockRate = 70;

    E = new Environment(gridSize,blockRate,false);
    E->generateOutput();
    Location* startLocation = generateRandomLocation(E);
    Location* targetLocation = generateRandomLocation(E);
    Agent* a = new Agent(startLocation , gridSize , true);
    a->AdaptiveAStarForwardSearch(targetLocation,E);
}

void AgentTest::compareTiesExample()
{
    int size = 5;
    E = new Environment(size,blockRate,true);

    Location *startLocation = new Location(0,0);
    Location *targetLocation = new Location(size-1,size-1);

    cout << "Breaking ties with smaller g-value " << endl;
    Agent *a = new Agent(startLocation , size , true);
    a->AStarForwardSearch(targetLocation,E);

    cout << "Breaking ties with larger g-value " << endl;
    Agent* b = new Agent(startLocation , size , false);
    b->AStarForwardSearch(targetLocation,E);

    int aC = a->getCost();
    int bC = b->getCost();

    int aE = a->getExpandedCells();
    int bE = b->getExpandedCells();

    cout << "Expanded Cells small - large " << aE << " - " << bE << endl;
    cout << "Total Trajectory Cost small - large " << aC << " - " << bC << endl;

}

void AgentTest::compareBreakTiesFowardRepeatedAStarTest(int n)
{
    aCost = new int[n];
    bCost = new int[n];
    aExp = new int[n];
    bExp = new int[n];

    ofstream outputCost("Results\\Part_2_Cost.txt");
    ofstream outputExpand("Results\\Part_2_Expand.txt");

    outputCost << "Small G - Large G" << endl;
    outputExpand << "Small G - Large G " << endl;

    for( int i = 0 ; i < n ; i++ )
    {
        cout << "---------------------------------" << endl;
        cout << "Environment " << i+1 << endl;
        E = collection[i];

        Location* startLocation = generateRandomLocation(E);
        Location* targetLocation = generateRandomLocation(E);

        cout << "Breaking ties with smaller g-value " << endl;
        Agent *a = new Agent(startLocation , gridSize , true);
        a->AStarForwardSearch(targetLocation,E);

        cout << "Breaking ties with larger g-value " << endl;
        Agent* b = new Agent(startLocation , gridSize , false);
        b->AStarForwardSearch(targetLocation,E);

        aCost[i] = a->getCost();
        bCost[i] = b->getCost();

        aExp[i] = a->getExpandedCells();
        bExp[i] = b->getExpandedCells();
        cout << "Expanded Cells small - large " << aExp[i] << " - " << bExp[i] << endl;
        cout << "Total Trajectory Cost small - large " << aCost[i] << " - " << bCost[i] << endl;
        cout << "------------------------------------" << endl;
    }

    for( int i = 0 ; i < n ; i++ )
    {
        outputExpand << aExp[i] << "\t" << bExp[i] << endl;
        outputCost << aCost[i] << "\t" << bCost[i] << endl;
    }

}

void AgentTest::compareForwardBackwardRepeatedAStarTest(int n)
{

    aCost = new int[n];
    bCost = new int[n];
    aExp = new int[n];
    bExp = new int[n];

    ofstream outputCost("Results\\Part_3_Cost.txt");
    ofstream outputExpand("Results\\Part_3_Expand.txt");

    outputCost << "Forward - Backward" << endl;
    outputExpand << "Forward - Backward" << endl;


    for( int i = 0 ; i < n ; i++ )
    {

        cout << "---------------------------------" << endl;
        cout << "Environment " << i+1 << endl;

        E = collection[i];
        Location* startLocation = generateRandomLocation(E);
        Location* targetLocation = generateRandomLocation(E);

        cout << "Forward A* Search " << endl;
        Agent *a = new Agent(startLocation , gridSize , false);
        a->AStarForwardSearch(targetLocation,E);

        cout << "Backward A* Search " << endl;
        Agent* b = new Agent(startLocation , gridSize , false);
        b->AStarBackwardSearch(targetLocation,E);

        aCost[i] = a->getCost();
        bCost[i] = b->getCost();

        aExp[i] = a->getExpandedCells();
        bExp[i] = b->getExpandedCells();


        cout << "Expanded Cells forward - backward " << aExp[i] << " - " << bExp[i] << endl;
        cout << "Total Trajectory Cost forward - backward " << aCost[i] << " - " << bCost[i] << endl;
        cout << "------------------------------------" << endl;

    }

    for( int i = 0 ; i < n ; i++ )
    {
        outputExpand << aExp[i] << "\t" << bExp[i] << endl;
        outputCost << aCost[i] << "\t" << bCost[i] << endl;
    }
}

void AgentTest::compareAdaptiveForwardRepeatedAStarTest(int n)
{

    aCost = new int[n];
    bCost = new int[n];
    aExp = new int[n];
    bExp = new int[n];


    ofstream outputCost("Results\\Part_5_Cost.txt");
    ofstream outputExpand("Results\\Part_5_Expand.txt");

    outputCost << "Forward - Adaptive" << endl;
    outputExpand << "Forward - Adaptive" << endl;

    for( int i = 0 ; i < n ; i++ )
    {
        E = collection[i];

        cout << "---------------------------------" << endl;
        cout << "Environment " << i+1 << endl;


        Location* startLocation = generateRandomLocation(E);
        Location* targetLocation = generateRandomLocation(E);

        cout << "Forward A* Search " << endl;
        Agent *a = new Agent(startLocation , gridSize , false);
        a->AStarForwardSearch(targetLocation,E);

        aCost[i] = a->getCost();
        aExp[i] = a->getExpandedCells();


        cout << "Adaptive Forward A* Search " << endl;
        Agent* b = new Agent(startLocation , gridSize , false);
        b->AdaptiveAStarForwardSearch(targetLocation,E);

        bCost[i] = b->getCost();
        bExp[i] = b->getExpandedCells();


        cout << "Expanded Cells forward - adaptive " << aExp[i] << " - " << bExp[i] << endl;
        cout << "Total Trajectory Cost forward - adaptive " << aCost[i] << " - " << bCost[i] << endl;
        cout << "------------------------------------" << endl;

    }


    for( int i = 0 ; i < n ; i++ )
    {
        outputExpand << aExp[i] << "\t" << bExp[i] << endl;
        outputCost << aCost[i] << "\t" << bCost[i] << endl;
    }
}

int main()
{
    srand(time(NULL));

    int gridSize = GRIDSIZE;
    int blockRate = BLOCKRATE;
    int N = 50;

    AgentTest* test = new AgentTest(gridSize,blockRate);

    if(PART == 0)
    {
        test->generateNGridWorlds(N);
    }

    if(PART == 1 )
    {
        test->compareTiesExample();
    }

    if( PART == 2 )
    {
        test->loadNgridWorlds(N);
        test->compareBreakTiesFowardRepeatedAStarTest(N);
    }

    if( PART == 3 )
    {
        test->loadNgridWorlds(N);
        test->compareForwardBackwardRepeatedAStarTest(N);
    }
    if( PART == 4 )
    {
        test->repeatedForwardAStarTest();
    }
    if(PART == 5)
    {
        test->loadNgridWorlds(N);
        test->compareAdaptiveForwardRepeatedAStarTest(N);
    }

    return 0;
}

