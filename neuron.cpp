#include <iostream>
#include <math.h>

#include <stdlib.h>

using namespace std;

class Neuron
{
public:
	#define numInputs 8
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

	void setInputCharacter( char c)
	{
		if( c == ' ') c = 0;
		a[0] = (c & 1 )> 0 ;
		a[1] = (c & 2 )> 0;
		a[2] = (c & 4 )> 0;
		a[3] = (c & 8 )> 0;
		a[4] = (c & 16) > 0;
		a[5] = (c & 32) > 0;
		a[6] = (c & 64) > 0;
		a[7] = (c & 128) > 0;
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

void train(int i, Neuron& n, char c, bool output = false)
{
		n.setInputCharacter(c);
		char k = 0;
		if (c > 63 && c < 128) k = c;
		if(output) cout << i << ": ["<<k<<"](";
		for( int j = 0; j < 8 ; j++)
			if(output) cout << n.a[j];
		if(output) cout << ") ";
		if( n.update() )
		  cout << " -->Fire!" << endl;	

		if(output) cout << n.V0;
		if(output) cout << endl;	
}

void trainString( Neuron& n, const string& input)
{
	for(int i = 0 ; i < input.size(); i++)
	{
		cout << "x";
		train( i, n, input[i]);
	}
	cout << endl;
}

void learnNoise(Neuron& n)
{
	cout << "learning noise" << endl;

	for(int i = 0; i < 1000; i++)
	{
		char c = rand() % 256;
	    cout << "x";
		train(i, n, c);

	}
}

void learnA(Neuron& n)
{
	for(int i = 0; i < 1000; i++)
	{
		char c = rand() % 4;
		switch (c)
		{
			case 0:
			case 1:
			case 2:
   			  train(i, n, rand() % 256);
   			  break;
			case 3:
   			  train(i, n, 'a');
   			  break;

		}

	}
}

void charStats(Neuron& n)
{
	for(char i = 97; i < 123; i++)
	{
		int count = 0;
		n.setInputCharacter(i);
		for( int j = 0; j < 1000; j++)
		{
			count += (int)n.update(false);
		}
		cout << i << ": " << count << endl;
	}
}
int main()
{
	cout << "Hello Neuron!" << endl;
	Neuron n;
	charStats(n);
	charStats(n);

	//learnNoise(n);

	//cout << "learn an a" << endl;

//	learnA(n);

	//string input = "   a  a  a  b  a  c  d  e 3 1 a   a  4  a 2 k s     s e 3   a  a   a k   3   a  a  a";
	//string input = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
	//string input = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXx";
	cout << "test sequence0a" << endl;
	trainString(n, "hhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhh");
	cout << "test sequence0b" << endl;
	trainString(n, "eeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee");
	cout << "test sequence0c" << endl;
	trainString(n, "yyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyy");
	charStats(n);

	cout << "test sequence1" << endl;
	trainString(n, "hey hey hey hey hey hey hey hey hey hey hey hey hey hey hey hey hey hey hey hey hey hey hey hey hey hey hey hey hey hey hey hey hey hey hey hey hey hey hey hey hey hey hey hey hey hey hey hey hey hey hey hey hey hey hey hey hey hey hey hey hey hey hey hey hey hey hey hey hey hey hey hey hey hey hey hey hey hey hey hey hey hey hey hey hey hey hey hey hey hey hey hey hey ");
	cout << "test sequence2a" << endl;
	trainString(n, "hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy");
	cout << "test sequence2b" << endl;
	trainString(n, "hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo hoooo ");
	cout << "test sequence2c" << endl;
	trainString(n, "hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy ");
	cout << endl;
	charStats(n);
	learnNoise(n);
	charStats(n);
	cout << endl << "test sequence2c" << endl;
	trainString(n, "hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy ");
	cout << endl << "test sequence2c" << endl;
	trainString(n, "hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy ");
	cout << endl << "test sequence2c" << endl;
	trainString(n, "hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy ");
	cout << endl << "test sequence2d" << endl;
	trainString(n, "hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy ");
	trainString(n, "hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy ");
	trainString(n, "hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy ");
	trainString(n, "hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy hhhheeeeyyyy ");


	charStats(n);
	return 0;
}