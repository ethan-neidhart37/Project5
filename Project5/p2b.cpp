// File: p1b.cpp
// Author: Ethan Neidhart, Kevin Do
// Code to read graph instances from a file.  Uses the Boost Graph Library (BGL).

#include "p2b.h"

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

int getDegree(Graph::vertex_descriptor &v, Graph &g)
// Get the number of degrees in a graph.
{
	int degree = 0;

	Graph::vertex_descriptor targetNode;
	Graph::vertex_descriptor sourceNode;
	pair<Graph::edge_iterator, Graph::edge_iterator> eItrRange = edges(g);

	// Loop through graph's edges
	for (Graph::edge_iterator eItr = eItrRange.first; eItr != eItrRange.second; ++eItr)
	{
		targetNode = target(*eItr, g);
		sourceNode = source(*eItr, g);

		if (targetNode == v || sourceNode == v)
		{
			degree++;
		}
	}

	return degree;
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

int getBestColor(int colorSet, Graph::vertex_descriptor &v, Graph &g)
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
			if (color == colorSet)
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
				if (color == colorSet)
				{
					return 0;
				}
			}
		}
	}
}

return color;
}

int partition(vector<Graph::vertex_descriptor>&a, Graph &g, int left, int right)
// Graph from highest to lowest priority using partition with recursion
{
	int i = left, j = right;
	Graph::vertex_descriptor temp;
	Graph::vertex_descriptor pivot = a.at((left + right) / 2);

	// Partition
	while (i <= j)
	{
		// Less than pivot, move
		while (getDegree(a.at(j), g) >= getDegree(pivot, g) && i < pivot)
		{
			i++;
		}

		// More than pivot, move
		while (getDegree(a.at(j), g) < getDegree(pivot, g))
		{
			j--;
		}

		//check pivot
		if (i <= j)
		{
			temp = a.at(i);
			a.at(i) = a.at(j);
			a.at(j) = temp;
			i++;
			j--;
		}
	}

	// Recurse parition on left
	if (left < j)
	{
		partition(a, g, left, j);
	}

	// Recurse partition on right
	if (i < right)
	{
		partition(a, g, i, right);
	}
}

int quicksortColor(Graph &g, vector<Graph::vertex_descriptor>& vertexVector)
{
	int i = 0;
	int j = 1;
	int min = 0;
	int select = 0;
	vector<Graph::vertex_descriptor> newVertex;
	vector<bool> slectVertex(vertexVector.size(), false);

	while (i < vertexVector.size()) - 1)
	{
		min = getDegree(vertexVector.at(i), g);
		location = i;
		for (int j = 0; j < vertexVector.size() - 1; j++)
		{
			if (!vertexUsed.at(j) && min > getDegree(vertexVector.at(j), g))
			{
				min = getDegree(vertexVector.at(j), g);
				location = j;
			}
		}

		selectVertex.at(select) = true;
		newVertex.push_back(vertexVector.at(select));

		i++;
	}

	return newVertex;
}

int greedyColor(Graph &g, int colorSet)
{
	//cout << endl << "Performing Greedy";
	int color = -1;
	vector<Graph::vertex_descriptor> vertexVector = getVertices(g);

	for (int i = 0; i < vertexVector.size(); i++)
	{
		color = getBestColor(colorSet, vertexVector[i], g);
		setColor(color, vertexVector[i], g);
	}

	return getConflict(g);
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

