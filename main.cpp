#include <iostream>
#include <ctime>
#include <random>
#include <algorithm>

#include "neuron.h"

#include "evol.h"

using namespace std;

void printWeights( Neuron& n )
{
  cout << n.w[0] << " " << n.w[1] << " " << n.w[2] << endl;
  cout << n.w[3] << " " << n.w[4] << " " << n.w[5] << endl;
  cout << n.w[6] << " " << n.w[7] << " " << n.w[8] << "  --- >  " << n.w[9] << endl;
}

int test(Neuron& n, int pattern, bool doUpdate = true)
{
	int countFired = 0;
	int m = 40; //rand() % 100;
	cout << "Classifing m = " << m << " with pattern = " << pattern << endl;
	for(int i = 0; i < m; i++)
	{
		n.setInput( pattern );
		if( n.update( doUpdate ) ) 
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


int finalTest( Neuron& n, int patNo, bool doUpdate = true)
{
    cout << "Simultaneous Classification and Learning of Pattern No. " << patNo << endl;
	int cPattern = 0;
	int cRandom = 0;
	for(int i = 0; i < 20; i++)
	{
		int pattern = rand() % 2 * patNo;
		int r =	test( n, pattern, doUpdate );
		if( pattern != 0 )
			cPattern += r;
		else
			cRandom += r;
	}

	cout << "total pattern: " << cPattern << endl;
	cout << "total random: " << cRandom << endl;
	return cPattern;
}



pair<int, int> dualTest( Neuron& n1, Neuron& n2, int pattern)
{
	int countFired1 = 0;
	int countFired2 = 0;
	int m = rand() % 50;
	m = 40;
	cout << pattern << " -> ";
	for(int i = 0; i < m; i++)
	{
		double V1_old = n1.V0;
		double V2_old = n2.V0;
		n1.setInput( pattern );
		n2.setInput( pattern );
		//n1.a[9] = 0 ; if ( V2_old > -10 ) n1.a[9] = V2_old;
		//n2.a[9] = 0 ; if ( V1_old > -10 ) n2.a[9] = V1_old;
		//n1.w[9] = -1;
		//n2.w[9] = -1;
		int fired1 = (int) n1.update();
		int fired2 = (int) n2.update();
		if( fired1 && fired2) cout << ":";
		if( !fired1 && fired2) cout << ".";
		if( fired1 && !fired2) cout << "'";
		if( !fired1 && !fired2) cout << "-";

		countFired1 += fired1;
		countFired2 += fired2;
		// printWeights( n1 );
	}
	cout << endl;
	return pair<int, int>( countFired1, countFired2 );
}

void dualNeuronTest( Neuron& n1, Neuron& n2 )
{
    cout << "Simultaneous Classification and Learning " << endl;
	int c1 = 0;
	int c2 = 0;

	for(int i = 0; i < 20; i++)
	{
		int pattern = rand() % 2;
		if( pattern == 0 ) pattern = 0;
		pair<int, int> c =	dualTest( n1, n2, pattern );
		c1 += c.first;
		c2 += c.second;

	}

	cout << "total 1: " << c1 << endl;
	cout << "total 2: " << c2 << endl;
}


int main()
{
	srand(time(0));
	generator2.seed( time(0) );
	Neuron::Parameters p;
	//p = trainEvol();

	Neuron n;
	n.p = p;

#if 1
	Neuron n1;
	Neuron n2;

	n1.p = p;
	n2.p = p;


	printWeights( n1 );
	printWeights( n2 );

    for(int i = 0; i < 3; i++) {

		dualNeuronTest(n1, n2);
		printWeights( n1 );
		cout << endl;
		printWeights( n2 );
		cout << endl;
    }

	// double n1_a = finalTest(n1, 1, false);
	// double n1_b = finalTest(n1, 5, false);
	// double n2_a = finalTest(n2, 1, false);
	// double n2_b = finalTest(n2, 5, false);

	// cout << "Ratio 1 : " << n1_a / n1_b << endl;
	// cout << "Ratio 2 : " << n2_a / n2_b << endl;

	pair<int, int> c = dualTest(n1, n2, 1);
	cout << "N1 : " << c.first << endl;
	cout << "N2 : " << c.second << endl;
	pair<int, int> c2 = dualTest(n1, n2, 5);
	cout << "N1 : " << c2.first << endl;
	cout << "N2 : " << c2.second << endl;


#endif

#if 0
	finalTest( n, 1 );
	printWeights( n );
#endif

#if 0
	printWeights( n );
	finalTest( n, 1 );
	printWeights( n );
	finalTest( n, 1 );
	printWeights( n );
	finalTest( n, 1 );
	printWeights( n );
	finalTest( n, 5 );
	printWeights( n );
	finalTest( n, 5 );
	printWeights( n );
	finalTest( n, 5 );
	printWeights( n );
	// finalTest( n, 0 );
	// printWeights( n );
	// finalTest( n, 0 );
	// printWeights( n );
	// finalTest( n, 0 );
	// printWeights( n );
	// finalTest( n, 3 );
	// printWeights( n );
	// finalTest( n, 3 );
	// printWeights( n );
	// finalTest( n, 3 );
	// printWeights( n );
	// finalTest( n, 1 );
	// finalTest( n, 1 );
	// finalTest( n, 1 );
	// printWeights( n );
#endif


#if 0 
for(int i = 0; i < 20; i++)
{
	n.setInput(1);
	n.update();
	printWeights(n);
}

for(int i = 0; i < 20; i++)
{
	n.setInput(2);
	n.update();
	printWeights(n);
}
#endif

#if 0
for(int i = 0; i < 1000; i++)
{
	finalTest( n, 1 );
	printWeights( n );
}
#endif
	return 0;

}
