#include <iostream>
#include <ctime>

#include "neuron.h"

using namespace std;

void learnAndClassify(int m, Neuron& n)
{
	for(int i = 0; i < m; i++)
	{
		n.setInput( rand() % 2 );
		n.update();
	}
}

int learnAndClassifyRandomNumber(Neuron& n, bool glider)
{
	int countFired = 0;
	int m = rand() % 100;
	cout << "Classifing m = " << m << " with glider = " << glider << endl;
	for(int i = 0; i < m; i++)
	{
		n.setInput( glider );
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


int main()
{
	srand(time(0));
	Neuron n;
	//learnAndClassify(1000, n);

	int cGlider = 0;
	int cRandom = 0;
	for(int i = 0; i < 200; i++)
	{
		bool glider = rand() % 2;
		int r =	learnAndClassifyRandomNumber( n, glider );
		if( glider )
			cGlider += r;
		else
			cRandom += r;
	}

	cout << "total glider: " << cGlider << endl;
	cout << "total random: " << cRandom << endl;

	return 0;
}
