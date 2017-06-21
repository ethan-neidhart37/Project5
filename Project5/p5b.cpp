// File: p5b.cpp
// Author: Ethan Neidhart, Kevin Do
// Project 5a: Solving graph coloring using local search.  Uses the Boost Graph Library (BGL).

#include "p5b.h"

void initializeGraph(Graph &g, ifstream &fin)
// Initialize g using data from fin.
{
	int n, e;
	int j, k;

	fin >> n >> e;
	Graph::vertex_descriptor v;

	// Add nodes.
	for (int i = 0; i < n; i++)
		v = add_vertex(g);

	for (int i = 0; i < e; i++)
	{
		fin >> j >> k;
		add_edge(j, k, g);  // Assumes vertex list is type vecS
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
			if (!g[t].marked)
			{
				numConflicts++;
				g[t].marked = true;
			}
			if (!g[s].marked)
			{
				numConflicts++;
				g[s].marked = true;
			}
		}
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

void printSolution(Graph &g, int numConflicts, string filename, string folder)
// Print the solution found.
{
	string filepath = folder + "output\\" + filename + ".output";
	ofstream myfile;
	myfile.open((filepath).c_str());

	myfile << "Total Conflicts: " << numConflicts << endl;
	for (int counter = 0; counter < num_vertices(g); ++counter)
	{
		myfile << counter << ": " << g[counter].color << endl;
	}
	myfile.close();
}

int graphColoring()
// Exhaustively finds the most efficient graph coloring solution (in 10 minute limit)
// Takes in an input file for graph, results in output file
{
	char x;
	ifstream fin;
	string fileName;

	// Read the name of the graph from the keyboard or
	// hard code it here for testing.

	// Hard-coded location of files so that only file name must be entered
	string fileFolder = "C:\\Users\\Ethan\\Documents\\GitHub\\Algorithms\\Project1\\Project1\\color\\";

	//fileName = "color12-3";

	cout << "Enter filename: ";
	cin >> fileName;

	string filePath = fileFolder + "input\\" + fileName + ".input";

	fin.open((filePath).c_str());
	if (!fin)
	{
		cerr << "Cannot open " << filePath << endl;
		exit(1);
	}

	try
	{
		cout << "Reading graph" << endl;
		Graph g;
		int numColors;
		int numConflicts = -1;
		fin >> numColors;
		initializeGraph(g, fin);

		cout << "Num nodes: " << num_vertices(g) << endl;
		cout << "Num edges: " << num_edges(g) << endl;
		cout << endl;

		// cout << g;

		numConflicts = exhaustiveColoring(g, numColors, 600);
		printSolution(g, numConflicts, fileName, fileFolder);
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

