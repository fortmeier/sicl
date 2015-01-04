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

int learnAndClassifyRandomNumber(Neuron& n, int pattern)
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


int main()
{
	srand(time(0));
	Neuron n;
	//learnAndClassify(1000, n);

	int cGlider = 0;
	int cCross = 0;
	int cCastle = 0;
	int cSnake = 0;
	int cRandom = 0;
	for(int i = 0; i < 200; i++)
	{
		int pattern = rand() % 7;
		if( pattern == 6 || pattern == 5 ) pattern = 1;
		int r =	learnAndClassifyRandomNumber( n, pattern );
		switch( pattern )
		{
			case 0:
				cRandom += r;
				break;
			case 1:
				cGlider += r;
				break;
			case 2:
				cCross += r;
				break;
			case 3:
				cCastle += r;
				break;
			case 4:
				cSnake += r;
				break;
		}
	}

	cout << "total glider: " << cGlider << endl;
	cout << "total cross: " << cCross << endl;
	cout << "total castle: " << cCastle << endl;
	cout << "total snake: " << cSnake << endl;
	cout << "total random: " << cRandom << endl;

	return 0;
}
