#pragma once

#include <iostream>
#include <limits.h>
#include "d_except.h"
#include <fstream>
#include <time.h>

#include <boost/graph/adjacency_list.hpp>

#define LargeValue 99999999

using namespace std;
using namespace boost;

int const NONE = -1;  // Used to represent a node that does not exist

struct VertexProperties;
struct EdgeProperties;

typedef adjacency_list<vecS, vecS, bidirectionalS, VertexProperties, EdgeProperties> Graph;

struct VertexProperties
{
	pair<int, int> cell; // maze cell (x,y) value
	Graph::vertex_descriptor pred;
	bool visited;
	bool marked;
	int weight;
	int color;
};

// Create a struct to hold properties for each edge
struct EdgeProperties
{
	int weight;
	bool visited;
	bool marked;
};

void initializeGraph(Graph &g, ifstream &fin);
void initializeGraph(Graph &g, ifstream &fin);
void cloneGraphColors(Graph &g, Graph &c);
void setNodeColors(Graph &g, int c);
void setNodeWeights(Graph &g, int w);
void setNodeMarks(Graph &g, bool b);
int checkConflicts(Graph &g);
bool increment(Graph &g, Graph::vertex_iterator vItr, int numColors, Graph::vertex_iterator vEnd);
int exhaustiveColoring(Graph &g, int numColors, int t);
void printSolution(Graph &g, int numConflicts, string filename, string folder);
int graphColoring();
