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

// Use this class for knapsack (not sure about coloring) to parametize a neighbor of a point by its value and indicies.
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
