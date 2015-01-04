#pragma once 

#include <iostream>
#include <math.h>

#include <stdlib.h>

using namespace std;

class Neuron
{
public:
	#define numInputs 9
	#define gamma 1.0
	double w[numInputs];
	double a[numInputs];
	double V0;
	double tD;
	double tP;
	double Heaviside( double x )
	{
		return 1.0 - 1.0 / (1.0+exp(x*100.0));
	}
	double dHeaviside_dx( double x )
	{
		return 100.0 * exp(100.0 * x) / pow( exp(100.0*x)+ 1.0, 2.0);
	}
	double DiracDelta(double x) { return dHeaviside_dx(x); }

	double sumIncomming()
	{
		double sum = 0;
		for(int j = 0; j < numInputs; j++)
		{
			sum += a[j]*w[j];
		}
		return sum;
	}

	double calc_dw_dt( int i )
	{
		double sum1 = V0 - tD + sumIncomming();
		double sum2 = tP - V0 - sumIncomming();
		double dEdw =
a[i]*gamma*(sum1)*DiracDelta(sum1) 
+ a[i]*gamma*Heaviside(sum1)
- a[i]*(sum2)*DiracDelta(sum2)
- a[i]*Heaviside(sum2)
		;
		return -0.1 * dEdw;// * ( a[i] * gamma * dHeaviside_dx( sum1 ) - a[i] * dHeaviside_dx( sum2 );
	}

	void updateWeights()
	{
		for(int i = 0; i < numInputs; i++)
			w[i] += calc_dw_dt(i);
	}

	double rnd()
	{
		return rand() % 2;
	}

	void setInput( bool isGlider )
	{
		if( isGlider ) 
		{
			a[0] = 0; a[1] = 1; a[2] = 0;
			a[3] = 0; a[4] = 0; a[5] = 1;
			a[3] = 1; a[4] = 1; a[5] = 1;
		}
		else
		{
			a[0] = rnd(); a[1] = rnd(); a[2] = rnd();
			a[3] = rnd(); a[4] = rnd(); a[5] = rnd();
			a[6] = rnd(); a[7] = rnd(); a[8] = rnd();
		}
	}

	bool update( bool doWeightUpdate = true)
	{
		double V = V0 * 0.99 + sumIncomming() * 1.0;
		bool hasFired = false;
		if( doWeightUpdate ) updateWeights();
		if( V > 1.5)
		{
			V -= 3.0;
			hasFired = true;
		} 
		V0 = V;
		return hasFired;
	}

	Neuron() :
	  V0(0),
	  tD(1.0),
	  tP(-1.0)
	{
		for(int i = 0; i < numInputs; i++)
		{
			w[i] = 0.5;
			a[i] = 0.0;
		}
	}
};

void learnAndClassify(int m, Neuron& n)
{
	for(int i = 0; i < m; m++)
	{
		n.setInput( rand() % 2 );
		n.update();
	}
}

