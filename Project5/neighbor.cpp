#include "neighbor.h"

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