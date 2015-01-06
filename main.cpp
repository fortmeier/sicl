#include <iostream>
#include <ctime>
#include <random>
#include <algorithm>

#include "neuron.h"

#include "evol.h"

using namespace std;

int test(Neuron& n, int pattern)
{
	int countFired = 0;
	int m = rand() % 100;
	cout << "Classifing m = " << m << " with pattern = " << pattern << endl;
	for(int i = 0; i < m; i++)
	{
		n.setInput( pattern );
		if( n.update() ) 
		{
			cout << "!";
			countFired++;
		}
		else
		{
			cout << ".";
		}
	}
	cout << endl;
	return countFired;
}


void finalTest( Neuron& n, int patNo )
{
    cout << "Simultaneous Classification and Learning of Pattern No. " << patNo << endl;
	int cPattern = 0;
	int cRandom = 0;
	for(int i = 0; i < 20; i++)
	{
		int pattern = rand() % 2 * patNo;
		int r =	test( n, pattern );
		if( pattern != 0 )
			cPattern += r;
		else
			cRandom += r;
	}

	cout << "total pattern: " << cPattern << endl;
	cout << "total random: " << cRandom << endl;
}

void printWeights( Neuron& n )
{
  cout << n.w[0] << " " << n.w[1] << " " << n.w[2] << endl;
  cout << n.w[3] << " " << n.w[4] << " " << n.w[5] << endl;
  cout << n.w[6] << " " << n.w[7] << " " << n.w[8] << endl;
}

int main()
{
	srand(time(0));
	Neuron::Parameters p;
	//p = trainEvol();

	Neuron n;
	n.p = p;

#if 0
	finalTest( n, 1 );
	printWeights( n );
	finalTest( n, 1 );
	printWeights( n );
	finalTest( n, 1 );
	printWeights( n );
#endif

#if 1
	finalTest( n, 1 );
	finalTest( n, 1 );
	finalTest( n, 1 );
	printWeights( n );
	finalTest( n, 3 );
	finalTest( n, 3 );
	finalTest( n, 3 );
	printWeights( n );
#endif
	return 0;
}
