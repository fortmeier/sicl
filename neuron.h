#pragma once 

#include <iostream>
#include <math.h>

#include <stdlib.h>

using namespace std;

class Neuron
{
public:
	#define numInputs 9
	double w[numInputs];
	double a[numInputs];

	double V0;
	struct Parameters {

		double tD;
		double tP;
		double activation;
		double drop;
		double decay;
		double fitness;
		double gamma;
		double jota;
		Parameters() :
		  tD(1.0),
		  tP(-1.0),
		  activation(1.5),
		  drop(3.0),
	          decay(0.99),
		  gamma(0.015),
		  jota(0.1)
		{ }
	} p;

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
		double sum1 = V0 - p.tD + sumIncomming();
		double sum2 = p.tP - V0 - sumIncomming();
		double dEdw =
a[i]*p.gamma*(sum1)*DiracDelta(sum1) 
+ a[i]*p.gamma*Heaviside(sum1)
- a[i]*(sum2)*DiracDelta(sum2)
- a[i]*Heaviside(sum2)
		;
		return -p.jota * dEdw;// * ( a[i] * gamma * dHeaviside_dx( sum1 ) - a[i] * dHeaviside_dx( sum2 );
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

	void setInput( int pattern )
	{
		switch( pattern )
		{
			case 0: // random
			a[0] = rnd(); a[1] = rnd(); a[2] = rnd();
			a[3] = rnd(); a[4] = rnd(); a[5] = rnd();
			a[6] = rnd(); a[7] = rnd(); a[8] = rnd();
			break;
			case 1: // glider
			a[0] = 0; a[1] = 1; a[2] = 0;
			a[3] = 0; a[4] = 0; a[5] = 1;
			a[6] = 1; a[7] = 1; a[8] = 1;
			break;
			case 2: // cross
			a[0] = 0; a[1] = 1; a[2] = 0;
			a[3] = 1; a[4] = 1; a[5] = 1;
			a[6] = 0; a[7] = 1; a[8] = 0;
			break;
			case 3: // castle
			a[0] = 1; a[1] = 0; a[2] = 1;
			a[3] = 0; a[4] = 0; a[5] = 0;
			a[6] = 1; a[7] = 0; a[8] = 1;
			break;
			case 4: // snake
			a[0] = 1; a[1] = 1; a[2] = 0;
			a[3] = 0; a[4] = 0; a[5] = 0;
			a[6] = 0; a[7] = 1; a[8] = 1;
			break;
		}
		
	}

	bool update( bool doWeightUpdate = true)
	{
		double V = V0 * p.decay + sumIncomming() * 1.0;
		bool hasFired = false;
		if( doWeightUpdate ) updateWeights();
		if( V > p.activation)
		{
			V -= p.drop;
			hasFired = true;
		} 
		V0 = V;
		return hasFired;
	}

	Neuron() :
	  V0(0)
	{
		for(int i = 0; i < numInputs; i++)
		{
			w[i] = 0.5;
			a[i] = 0.0;
		}
	}
};



