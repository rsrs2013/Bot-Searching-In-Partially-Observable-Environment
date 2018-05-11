#include <iostream>
#include <ctime>
#include <climits>
#include <cstdlib>
#include <cmath>
#include <ctime>
#include <stack>
#include <vector>
#include <algorithm>
#include <unistd.h>
using namespace std;

#include "Environment.cpp"
#include "BinaryHeap.cpp"
int main()
{
	BinaryHeap* b = new BinaryHeap(false);
	vector<State*> inp;
	for( int i = 0 ; i < 20 ; i++ )
	{
		int a;
		cout << "Enter a number : ";
		cin >> a;
		State *s = new State(new Location(i,i),10);
		s->setG(a);
		s->setF(a+a);
		s->setH(a);
		inp.push_back(s);
		b->minHeapInsert(s);
		b->printHeap();
		cout << endl;
	}

	for( int i = 0 ; i < 20 ; i++ )
	{
		b->decreaseKey(inp[i],-1*i);
	}

	cout << "\n Sorted Order is : ";
	for( int i = 0 ; i < 20 ; i++ )
	{
		State *s = b->extractMin();
		cout << "(" << s->getF() << "," << s->getG() << "," << s->getH() << ") ";
	}


}
