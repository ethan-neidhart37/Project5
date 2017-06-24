#include <iostream>
#include <limits.h>
#include <list>
#include <fstream>
#include <queue>
#include <stack>
#include <vector>
#include <time.h>

#include "d_except.h"
#include "d_matrix.h"

using namespace std;

class Neighbor
{
public:
	Neighbor();
	Neighbor(int value, vector<int> indicies);
	int getValue() const;
	vector<int> getIndicies() const;

private:
	int value;
	vector<int> indicies;
};

Neighbor::Neighbor() 
{}

Neighbor::Neighbor(int value, vector<int>indicies)
{
	this->value = value;
	this->indicies = indicies;
}

int Neighbor::getValue() const
{
	return value;
}

vector<int> Neighbor::getIndicies() const
{
	return indicies;
}


