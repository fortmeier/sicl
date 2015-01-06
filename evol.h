#pragma once

#include <iostream>
#include <ctime>
#include <random>
#include <algorithm>

#include "neuron.h"

using namespace std;

std::default_random_engine generator;

int learnAndClassifyRandomNumber(Neuron& n, int pattern)
{
	int countFired = 0;
	int m = rand() % 20;
	//cout << "Classifing m = " << m << " with pattern = " << pattern << endl;
	for(int i = 0; i < m; i++)
	{
		n.setInput( pattern );
		if( n.update() ) 
		{
			//cout << "!";
			countFired++;
		}
		else
		{
			//cout << ".";
		}
	}
	//cout << endl;
	return countFired;
}

double randSigned()
{
	static std::normal_distribution<double> distribution(0.0,1.0);
	return distribution( generator );
}

void mutate(Neuron::Parameters& p)
{
	p.tD += randSigned() * 0.1;
	p.tP += randSigned() * 0.1;
	p.activation += randSigned() * 0.1;
	p.drop += randSigned() * 0.1;
	p.decay += randSigned() * 0.01;
	p.gamma += randSigned() * 0.05;
	p.jota += randSigned() * 0.05;
}

double mutateGenepool( vector<Neuron::Parameters>& genepool )
{
	for(int i = 0; i < genepool.size() / 2; i++)
	{
		mutate( genepool[i] );
	}
}

void printParams( Neuron::Parameters& p )
{
	cout << p.tD << " ";
	cout << p.tP << " ";
	cout << p.activation << " ";
	cout << p.drop << " ";
	cout << p.decay << " ";
	cout << p.gamma << " ";
	cout << p.jota << " ";
	cout << "= " << p.fitness << " ";
	cout << endl;
}

void printGenepool( vector<Neuron::Parameters>& genepool )
{
	for( auto p : genepool )
	{
		printParams( p );
	}
}

double evaluateFitness( Neuron::Parameters& p )
{
	Neuron n;
	n.p = p;

	int noise = 0;
	int object = rand() % 4 + 1;
	int cObject = 0;
	int cNoise = 0;
	for(int i = 0; i < 100; i++)
	{
		int pattern = rand() % 2;
		int r =	0;
		switch( pattern )
		{
			case 0:
				noise = rand()%4+1;
				r = learnAndClassifyRandomNumber( n, noise );
				cNoise += r;
				break;
			case 1:
				r = learnAndClassifyRandomNumber( n, object );
				cObject += r;
				break;
		}
	}

	//cout << cRandom << "/" << cGlider << " = " <<	cRandom / (cGlider + 1.0) << endl;	

	return cNoise - cObject;	

	//return cRandom / (cGlider + 1.0);	
}

double evaluateGenepool( vector<Neuron::Parameters>& genepool )
{
	for( auto& p : genepool )
	{
		p.fitness = evaluateFitness( p );
	}
}

bool compareParams( const Neuron::Parameters& p1, const Neuron::Parameters& p2 )
{ 
  return p1.fitness > p2.fitness;
}

void copyGoodOnes( vector<Neuron::Parameters>& genepool )
{
	for(int i = 0; i < genepool.size() / 2; i++)
	{
		genepool[i+genepool.size() / 2] = genepool[i];
	}
}

Neuron::Parameters trainEvol()
{
	
	int indivduals = 100;
	vector<Neuron::Parameters> genepool( indivduals );

	for(int i = 0; i < 100; i++)
	{
		copyGoodOnes( genepool );
		mutateGenepool( genepool );
		evaluateGenepool( genepool );
		sort( genepool.begin(), genepool.end(), compareParams );
		cout << i << ": " << genepool[0].fitness << endl;
	}
	printGenepool( genepool );

	return genepool[0];
}

