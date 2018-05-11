

using namespace std;

#include "Environment.cpp"

int main()
{
	srand(time(NULL));
	Environment* E = new Environment(5,70,false);
	E->generateOutput();
	cout << INF << endl; 
	return 0;
}
