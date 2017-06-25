// File: p5b.cpp
// Author: Ethan Neidhart, Kevin Do
// Project 5a: Solving graph coloring using local search.  Uses the Boost Graph Library (BGL).

#include "p5b.h"

void initializeGraphs(Graph &g1, Graph &g2, ifstream &fin)
// Initialize g1 and g2 using data from fin.
{
	int n, e;
	int j, k;

	fin >> n >> e;
	Graph::vertex_descriptor v1;
	Graph::vertex_descriptor v2;

	// Add nodes.
	for (int i = 0; i < n; i++)
	{
		v1 = add_vertex(g1);
		v2 = add_vertex(g2);
	}

	for (int i = 0; i < e; i++)
	{
		fin >> j >> k;
		add_edge(j, k, g1);  // Assumes vertex list is type vecS
		add_edge(j, k, g2);
	}
}

void cloneGraphColors(Graph &g, Graph &c)
// Takes two graphs with the same nodes and sets the colors of c to the colors of g.
{
	pair<Graph::vertex_iterator, Graph::vertex_iterator> vItrRange = vertices(g);

	for (Graph::vertex_iterator vItr = vItrRange.first; vItr != vItrRange.second; ++vItr)
	{
		c[*vItr].color = g[*vItr].color;
	}
}

void setNodeWeights(Graph &g, int w)
// Set all node weights to w.
{
	pair<Graph::vertex_iterator, Graph::vertex_iterator> vItrRange = vertices(g);

	for (Graph::vertex_iterator vItr = vItrRange.first; vItr != vItrRange.second; ++vItr)
	{
		g[*vItr].weight = w;
	}
}

void setNodeColors(Graph &g, int c)
// Set all node colors to c (integer representation of a color).
{
	pair<Graph::vertex_iterator, Graph::vertex_iterator> vItrRange = vertices(g);

	for (Graph::vertex_iterator vItr = vItrRange.first; vItr != vItrRange.second; ++vItr)
	{
		g[*vItr].color = c;
	}
}

void setNodeMarks(Graph &g, bool b)
// Set all node weights to w.
{
	pair<Graph::vertex_iterator, Graph::vertex_iterator> vItrRange = vertices(g);

	for (Graph::vertex_iterator vItr = vItrRange.first; vItr != vItrRange.second; ++vItr)
	{
		g[*vItr].marked = b;
	}
}

int checkConflicts(Graph &g)
// Return the number of nodes in conflict in a graph.
// For each edge with two nodes of the same color, there is one node in conflict.
{
	int numConflicts = 0;
	setNodeMarks(g, false);
	pair<Graph::edge_iterator, Graph::edge_iterator> eItrRange = edges(g);
	for (Graph::edge_iterator eItr = eItrRange.first; eItr != eItrRange.second; ++eItr)
	{
		Graph::vertex_descriptor t = target(*eItr, g);
		Graph::vertex_descriptor s = source(*eItr, g);
		if (g[t].color == g[s].color)
		{
			numConflicts++;
		}
	}
	return numConflicts;
}

int nodeConflicts(Graph &g, Graph::vertex_iterator &v, int color)
{
	int numConflicts = 0;

	pair<Graph::adjacency_iterator, Graph::adjacency_iterator> vItrRange = adjacent_vertices(*v, g);
	for (Graph::adjacency_iterator vItr = vItrRange.first; vItr != vItrRange.second; ++vItr)
	{
		if (g[*vItr].color == color)
			numConflicts++;
	}
	return numConflicts;
}

bool increment(Graph &g, Graph::vertex_iterator vItr, int numColors, Graph::vertex_iterator vEnd)
// Used to cycle through all the different permutations of graph coloring, one at a time.
// With all vertices stored in a linear array, and all colors represented by integers,
// the graph functions in a remarkably similar way to a counter.
// Returns true if last value of counter has been reached, false otherwise.
{
	g[*vItr].color++;
	if (g[*vItr].color == numColors)
	{
		g[*vItr].color = 0;
		++vItr;
		if (vItr == vEnd)
			return true;
		else
			increment(g, vItr, numColors, vEnd);
	}
	return false;
}

int exhaustiveColoring(Graph &g, int numColors, int t)
// Increment through all permutations of the graph using the increment() function.
// Check number of conflicts, and store lowest number that has been checked.
// Return number of conflicts when done, or when time t has passed.
{
	clock_t startTime = clock();

	setNodeColors(g, 0);
	int numConflicts = checkConflicts(g);
	int tempNumConflicts = numConflicts;
	bool finished = false;
	Graph minGraph(num_vertices(g));
	cloneGraphColors(g, minGraph);

	pair<Graph::vertex_iterator, Graph::vertex_iterator> vItrRange = vertices(g);

	for (;;)
	{
		finished = increment(g, vItrRange.first, numColors, vItrRange.second);

		if (finished)
			break;

		tempNumConflicts = checkConflicts(g);

		if (tempNumConflicts < numConflicts) {
			numConflicts = tempNumConflicts;
			cloneGraphColors(g, minGraph);
		}

		if (numConflicts == 0)
			break;

		// Check if time is expired and return
		if ((clock() - startTime) / CLOCKS_PER_SEC >= t)
			break;

		//cout << "Conflicts: " << tempNumConflicts << endl;
	}

	cloneGraphColors(minGraph, g);

	return numConflicts;
}

vector<Graph::vertex_descriptor> getVertices(Graph &g)
// Get the number of vertices in a graph.
{
	vector<Graph::vertex_descriptor> nodes;

	pair<Graph::vertex_iterator, Graph::vertex_iterator> vItrRange = vertices(g);

	for (Graph::vertex_iterator vItr = vItrRange.first; vItr != vItrRange.second; ++vItr)
	{
		g[*vItr].color = -1;
		nodes.push_back(*vItr);
	}

	return nodes;
}

int getConflict(Graph &g)
// Same implementation as getDegree but searching for conflicts
{
	int conflict = 0;

	Graph::vertex_descriptor targetNode;
	Graph::vertex_descriptor sourceNode;
	pair<Graph::edge_iterator, Graph::edge_iterator> eItrRange = edges(g);

	for (Graph::edge_iterator eItr = eItrRange.first; eItr != eItrRange.second; ++eItr)
	{
		targetNode = target(*eItr, g);
		sourceNode = source(*eItr, g);

		if (g[targetNode].color == g[sourceNode].color)
		{
			conflict++;
		}
	}

	return conflict;
}

void setColor(int color, Graph::vertex_descriptor &v, Graph &g)
// Set a color to graph.
{
	g[v].color = color;
}

int getBestColor(int numColors, Graph::vertex_descriptor &v, Graph &g)
// Return best color pair from a set of colors.
{
	int color = 1;

	Graph::vertex_descriptor targetNode;
	Graph::vertex_descriptor sourceNode;
	pair<Graph::edge_iterator, Graph::edge_iterator> eItrRange = edges(g);

	for (Graph::edge_iterator eItr = eItrRange.first; eItr != eItrRange.second; ++eItr)
	{
		targetNode = target(*eItr, g);
		sourceNode = source(*eItr, g);

		if (targetNode == v)
		{
			if (g[sourceNode].color == g[v].color)
			{
				color++;
				if (color == numColors)
				{
					return 0;
				}
			}
		}

		else
		{
			if (sourceNode == v)
			{
				if (g[targetNode].color == g[v].color)
				{
					color++;
					if (color == numColors)
					{
						return 0;
					}
				}
			}
		}
	}

	return color;
}

int greedyColor(Graph &g, int numColors)
{
	//cout << endl << "Performing Greedy";
	int color = -1;
	vector<Graph::vertex_descriptor> vertexVector = getVertices(g);

	for (int i = 0; i < vertexVector.size(); i++)
	{
		color = getBestColor(numColors, vertexVector[i], g);
		setColor(color, vertexVector[i], g);
	}

	return getConflict(g);
}

void neighborhood(Graph &g, Graph::vertex_iterator &v, int numColors)
// Search for a better color for node
{
	int initialColor = g[*v].color;
	int numConflicts = nodeConflicts(g, v, initialColor);

	for (int color = 0; color < numColors && numConflicts > 0; color++)
	{
		if (color == initialColor)
			continue;

		int tempConflicts = nodeConflicts(g, v, color);
		if (tempConflicts < numConflicts)
		{
			numConflicts = tempConflicts;
			g[*v].color = color;
		}
	}
}

int steepestDescent(Graph &g, int numColors, int time)
{
	clock_t startTime = clock();
	greedyColor(g, numColors);

	/*pair<Graph::vertex_iterator, Graph::vertex_iterator> vItrRange = vertices(g);
	for (Graph::vertex_iterator vItr = vItrRange.first; vItr != vItrRange.second && ((clock() - startTime) / CLOCKS_PER_SEC < time); ++vItr)
		neighborhood(g, vItr, numColors);*/

	return checkConflicts(g);
}

int randomColor(Graph &g, int numColors)
{
	pair<Graph::vertex_iterator, Graph::vertex_iterator> vItrRange = vertices(g);
	for (Graph::vertex_iterator vItr = vItrRange.first; vItr != vItrRange.second; ++vItr)
		g[*vItr].color = rand() % numColors;

	return checkConflicts(g);
}

void printSolution(Graph &g, int numConflicts, string filename)
// Print the solution found.
{
	string filepath = "color/output/" + filename + ".output";
	ofstream myfile;
	myfile.open((filepath).c_str());

	myfile << "Total Conflicts: " << numConflicts << endl;
	for (int counter = 0; counter < num_vertices(g); ++counter)
	{
		myfile << counter << ": " << g[counter].color << endl;
	}
	myfile.close();
}

void graphColoring()
// Finds a graph coloring solution
// Takes in an input file for graph, results in output file
{
	char x;
	ifstream fin;
	string fileName;

	// Read the name of the graph from the keyboard or
	// hard code it here for testing.

	//fileName = "color12-3";

	cout << "Enter filename" << endl;
	cin >> fileName;
	string filePath = "color/input/" + fileName + ".input";

	fin.open((filePath).c_str());
	if (!fin)
	{
		cerr << "Cannot open " << filePath << endl;
		exit(1);
	}

	try
	{
		cout << "Reading graph" << endl;
		Graph g1;
		Graph g2;
		int numColors;
		int numConflicts = -1;
		fin >> numColors;
		initializeGraphs(g1, g2, fin);
	
		greedyColor(g2, numColors);

		cout << "Num nodes: " << num_vertices(g1) << endl;
		cout << "Num edges: " << num_edges(g1) << endl;
		cout << endl;

		// Part 1: Steepest Descent
		numConflicts = steepestDescent(g1, numColors, 300);
		printSolution(g1, numConflicts, fileName + "part1");

		// Part 2: Other Algorithm (Random)
		numConflicts = randomColor(g2, numColors);
		printSolution(g2, numConflicts, fileName + "part2");
	}
	catch (indexRangeError &ex)
	{
		cout << ex.what() << endl; exit(1);
	}
	catch (rangeError &ex)
	{
		cout << ex.what() << endl; exit(1);
	}

}

