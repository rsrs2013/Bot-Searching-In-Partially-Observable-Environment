class BinaryHeap
{
	private:
		vector<State*> heap;
		bool smaller_g;
	public:
		BinaryHeap(bool small);
		int Parent( int i );
		int Left( int i );
		int Right( int i );
		void minHeapify(int i);
		State* heapMinimum();
		State* extractMin();
		State* top();
		void decreaseKey(State *s , double value );
		void minHeapInsert(State *s);
		int search(State *s);
		bool isEmpty();
		void printHeap();
		bool compareState( State* a , State *b );
};

BinaryHeap::BinaryHeap(bool small)
{
	smaller_g = small;
}


int BinaryHeap::Parent( int i )
{
	return (i-1)/2;
}

int BinaryHeap::Left( int i )
{
	return (2*i)+1;
}

int BinaryHeap::Right( int i )
{
	return 2*i+2;
}

void BinaryHeap::minHeapify( int i )
{
	int l = Left(i);
	int r = Right(i);

	int minimum = i;

	if( l < heap.size() )
	{
		if( compareState( heap[i] , heap[l] ) )
			minimum = l;
		else
			minimum = i;
	}

	if( r < heap.size() && compareState( heap[minimum] , heap[r] ) )
		minimum = r;

	if( minimum != i )
	{
		State* temp = heap[i];
		heap[i] = heap[minimum];
		heap[minimum] = temp;
		minHeapify(minimum);
	}
}

State* BinaryHeap::heapMinimum()
{
	if( heap.size() )
		return heap[0];

	return NULL;
}

State* BinaryHeap::extractMin()
{
	if( heap.size() == 0 )
	{
		cout << "Error no element is in heap" << endl;
		return NULL;
	}

	State* min = heap[0];
	heap[0] = heap[heap.size() - 1];
	heap.pop_back();
	//cout << "\n Heap now: ";
	//printHeap();
	minHeapify(0);
	//cout << "\n Heap After minHeapify : ";
	//printHeap();
	return min;

}

State* BinaryHeap::top()
{
	if( heap.size() != 0 )
		return heap[0];
	else
		return NULL;
}

int BinaryHeap::search(State *s)
{
	for(int i = 0 ; i < heap.size() ; i++ )
	{
		if( s->getLocation()->getX() == heap[i]->getLocation()->getX() && s->getLocation()->getY() == heap[i]->getLocation()->getY() )
		{
			return i;
		}
	}

	return -1;
}

void BinaryHeap::decreaseKey(State * s , double value )
{
	int index;

	index = search(s);

	heap[index]->setG(value);

	while( index > 0 && compareState( heap[Parent(index)] , heap[index] ) )
	{
		State* temp = heap[Parent(index)];
		heap[Parent(index)] = heap[index];
		heap[index] = temp;
		index = Parent(index);
	}

}

void BinaryHeap::minHeapInsert(State *s)
{

	heap.push_back(s);

	int index = heap.size() - 1;

	while( index > 0 && compareState( heap[Parent(index)] , heap[index] ) )
	{
		State* temp = heap[Parent(index)];
		heap[Parent(index)] = heap[index];
		heap[index] = temp;
		index = Parent(index);
	}
}

bool BinaryHeap::isEmpty()
{
	if( heap.size() == 0 )
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool BinaryHeap::compareState( State * a , State *b )
{
	if( a->getF() > b->getF() )
	{
		return true;
	}
	else if( a->getF() < b->getF() )
	{
		return false;
	}
	else if( a->getF() == b->getF() )
	{
		if( smaller_g )
		{
			if( a->getG() > b->getG() )
			{
				return true;
			}
			else if( a->getG() == b->getG() )
			{
				//int r = rand() % 2;
				//if( r == 0 )
                    return true;
                //else
                //    return false;
			}
			else
			{
			    return false;
			}
		}
		else
		{
			if( a->getG() < b->getG() )
			{
				return true;
			}
			else if( a->getG() == b->getG() )
			{
				//int r = rand() % 2;
				//if( r == 0 )
                    return true;
                //else
                //    return false;
			}
			else
			{
			    return false;
			}

		}
	}
}

void BinaryHeap::printHeap()
{
	for( int i = 0 ; i < heap.size() ; i++ )
	{
		cout << "(" << heap[i]->getF() << "," << heap[i]->getG() << "," << heap[i]->getH() << ") ";
	}
}
