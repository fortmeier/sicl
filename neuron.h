#pragma once 

#include <iostream>
#include <math.h>

#include <stdlib.h>

using namespace std;

std::default_random_engine generator2;

class Neuron
{
public:

	double randSigned()
	{
		static std::normal_distribution<double> distribution(0.0,1.0);
		return distribution( generator2 );
	}

	#define numInputs 10
	double w[numInputs];
	double a[numInputs];

	double V0;

	bool hasFired;

	double sum;

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
		  activation(1.0),
		  drop(2.0),
	          decay(0.01),
		  gamma(1.5),
		  jota(0.1)
		{ }
		// Parameters() :
		//   tD(1.0),
		//   tP(-1.0),
		//   activation(1.0),
		//   drop(2.0),
	 //          decay(0.99),
		//   gamma(0.01),
		//   jota(0.1)
		// { }
	} p;

	double Heaviside( double x )
	{
		return 1.0 - 1.0 / (1.0+exp(x*100.0));
	}
	double dHeaviside_dx( double x )
	{
		//if( x == 0 ) return 1.0;
		//return 0.0;
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
		double sum1 = V0 - p.tD + sum;
		double sum2 = p.tP - V0 - sum;
		double dEdw =
a[i]*p.gamma*(sum1)*DiracDelta(sum1) 
+ a[i]*p.gamma*Heaviside(sum1)
- a[i]*(sum2)*DiracDelta(sum2)
- a[i]*Heaviside(sum2)
		;
		// cout<<"sum1: " << sum1 << endl;
		// cout<<"sum2: " << sum2 << endl;

		// cout<<"DiracDelta1: " << DiracDelta(sum1) << endl;
		// cout<<"Heaviside1: " << Heaviside(sum1) << endl;
		// cout<<"DiracDelta2: " << DiracDelta(sum2) << endl;
		// cout<<"Heaviside2: " << Heaviside(sum2) << endl;
		return -p.jota * dEdw;// * ( a[i] * gamma * dHeaviside_dx( sum1 ) - a[i] * dHeaviside_dx( sum2 );
	}

	double calc_dw_dt_alt( int i )
	{
	  return -2*a[i]*p.gamma*(V0 + sum);
    }

	double calc_dw_dt_alt2( int i )
	{
		// E = 1-1/(((Vt-1)*10.1)^2+1)
		// -> 2*a1*gamma**2*(V0 + a1*w1 + a2*w2 + a3*w3 - 1)/(gamma**2*(V0 + a1*w1 + a2*w2 + a3*w3 - 1)**2 + 1)**2
		double alpha = V0 + sum - 1.0;
		double beta = (p.gamma*p.gamma*alpha*alpha + 1.0);
		double dEdw = 2*a[i]*p.gamma*p.gamma*(V0 + sum - 1.0)/(beta*beta);
		return -p.jota * dEdw;
    }

	double calc_dw_dt_alt3( int i )
	{
		// E = 1-1/(((Vt-1)*10.1)^2+1)
		// -> 2*a1*gamma**2*(V0 + a1*w1 + a2*w2 + a3*w3 - 1)/(gamma**2*(V0 + a1*w1 + a2*w2 + a3*w3 - 1)**2 + 1)**2
		double alpha = V0 + sum - 1.0;
		double beta = (p.gamma*p.gamma*alpha*alpha + 1.0);
		double dEdw = 2*a[i]*p.gamma*p.gamma*(V0 + sum - 1.0)/(beta*beta);
		if( V0 + sum < 0) dEdw = 0;
		return -p.jota * dEdw;
    }

	void updateWeights()
	{
		sum = sumIncomming();
		double dw_dt[numInputs];
		for(int i = 0; i < numInputs; i++)
			dw_dt[i] = calc_dw_dt_alt3(i);
		for(int i = 0; i < numInputs; i++)
		{
			//cout << dw_dt[i] << endl;
			w[i] += dw_dt[i];
			//if( w[i] < 0.5 ) w[i] += 0.0013;

		}

	}

	double rnd()
	{
		return rand() % 2;
	}

	double rndSgn()
	{
		return rand() % 3 - 1;
	}

	void setInput( int pattern )
	{
		switch( pattern )
		{
			case 0: // random
			a[0] = rndSgn(); a[1] = rndSgn(); a[2] = rndSgn();
			a[3] = rndSgn(); a[4] = rndSgn(); a[5] = rndSgn();
			a[6] = rndSgn(); a[7] = rndSgn(); a[8] = rndSgn();
			break;
			case 1: // glider
			a[0] = -1; a[1] = 1; a[2] = -1;
			a[3] = -1; a[4] = -1; a[5] = 1;
			a[6] = 1; a[7] = 1; a[8] = 1;
			break;
			case 2: // cross
			a[0] = -1; a[1] = 1; a[2] = -1;
			a[3] = 1; a[4] = 1; a[5] = 1;
			a[6] = -1; a[7] = 1; a[8] = -1;
			break;
			case 3: // castle
			a[0] = 1; a[1] = -1; a[2] = 1;
			a[3] = -1; a[4] = -1; a[5] = -1;
			a[6] = 1; a[7] = -1; a[8] = 1;
			break;
			case 4: // snake
			a[0] = 1; a[1] = 1; a[2] = -1;
			a[3] = -1; a[4] = -1; a[5] = -1;
			a[6] = -1; a[7] = 1; a[8] = 1;
			break;
			case 5: // anti glider
			a[0] = 1; a[1] = -1; a[2] = 1;
			a[3] = 1; a[4] = 1; a[5] = -1;
			a[6] = -1; a[7] = -1; a[8] = -1;
			break;
		}
		
	}

	bool update( bool doWeightUpdate = true)
	{
		double V = V0 - p.decay + sumIncomming();
		hasFired = false;
		if( doWeightUpdate ) updateWeights();
		if( V > p.activation)
		{
			V = p.activation - p.drop;
			hasFired = true;
		}
		if( V < p.activation - p.drop)
		{
			V = p.activation - p.drop;
		} 

		V0 = V;
		return hasFired;
	}

	Neuron() :
	  V0(0),
	  hasFired(false)
	{
		for(int i = 0; i < numInputs; i++)
		{
			w[i] = randSigned() * 0.1;
			a[i] = 0.0;
		}
	}
};



