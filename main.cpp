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


void finalTest(Neuron::Parameters p)
{
	Neuron n;
	n.p = p;

	int cPattern = 0;
	int cRandom = 0;
	for(int i = 0; i < 200; i++)
	{
		int pattern = rand() % 2 * 2;
		int r =	test( n, pattern );
		if( pattern != 0 )
			cPattern += r;
		else
			cRandom += r;
	}

	cout << "total pattern: " << cPattern << endl;
	cout << "total random: " << cRandom << endl;
}

int main()
{
	srand(time(0));
	Neuron::Parameters p;
	//p = trainEvol();
	finalTest( p );

	return 0;
}
