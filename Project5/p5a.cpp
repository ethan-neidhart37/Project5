// File: p5a.cpp
// Authors: Kevin Do, Ethan Neidhart
// Project 5a: Solving knapsack using local search

#include "p5a.h"

using namespace std;

void branchAndBound(knapsack &k, int time)
// Branch and bound algorithm to solve the knapsack problem.
// Branching is binary: one branch to take the "next" object, one branch to skip it
{
	clock_t startTime = clock();
	knapsack incumbent(k);
	vector<int> items = k.sort();
	stack<knapsack> nodes;
	k.bound(items[k.getCurrentItem()]);
	nodes.push(k);

	while (!nodes.empty() && (clock() - startTime) / CLOCKS_PER_SEC < time)
	{
		knapsack current = nodes.top();
		nodes.pop();
		bool isFathomed = current.isFathomed(incumbent.getValue());

		// If node is fathomed, legal, and better than incumbent, it becomes the new incumbent
		if (isFathomed && current.getCost() <= current.getCostLimit() && current.getValue() > incumbent.getValue())
		{
			incumbent = current;
		}
		// If node is fathomed and not new incumbent, ignore it
		// If node is not fathomed, branch on next item
		else if (!isFathomed)
		{
			int item = items[current.getCurrentItem()];
			knapsack yes(current);
			knapsack no(current);

			yes.select(item);
			yes.nextItem();
			no.nextItem();

			if (yes.bound(yes.getCurrentItem()) >= no.bound(no.getCurrentItem()))
			{
				nodes.push(no);
				nodes.push(yes);
			}
			else
			{
				nodes.push(yes);
				nodes.push(no);
			}
		}
	}

	k = incumbent;
}

void greedyKnapsack(knapsack &k)
// Greedy algorithm to solve knapsack problem by grabbing highest priority items that will fit
{
	int limit = k.getCostLimit();
	int cost = 0;
	vector<int> items = k.sort();

	// The first item in this list now contains the item number of the highest priority knapsack item

	for (int i = 0; i < k.getNumObjects(); i++)
	{
		int item = items[i];

		if (cost + k.getCost(item) <= limit)
		{
			k.select(item);
			cost += k.getCost(item);
		}

		if (cost == limit)
			return;
	}
}

Neighbor greedyKnapsackN(knapsack &k)
// Greedy algorithm to solve knapsack problem by grabbing highest priority items that will fit
{
	int limit = k.getCostLimit();
	int cost = 0;
	vector<int> items = k.sort();

	// The first item in this list now contains the item number of the highest priority knapsack item

	for (int i = 0; i < k.getNumObjects(); i++)
	{
		int item = items[i];

		if (cost + k.getCost(item) <= limit)
		{
			k.select(item);
			cost += k.getCost(item);
		}

		if (cost == limit)
			return;
	}

	return Neighbor(k.getValue(), k.getIndicies());
}


Neighbor greedyKnapsackN(knapsack &k, vector<int> indicies, int j)
{
	int limit = k.getCostLimit();
	int cost = 0;
	vector<int> items = k.sort();

	k.setItems(indicies);
	k.unSelect(j);

	// The first item in this list now contains the item number of the highest priority knapsack item

	for (int i = 0; i < k.getNumObjects(); i++)
	{
		int item = items[i];

		if (cost + k.getCost(item) <= limit)
		{
			k.select(item);
			cost += k.getCost(item);
		}

		if (cost == limit)
			return;
	}

	return Neighbor(k.getValue(), k.getIndicies());
}

Neighbor bestNeighbor(knapsack &k, Neighbor &currentNeighbor)
{
	Neighbor newNeighbor;
	Neighbor bestNeighbor = currentNeighbor;

	vector<int> indicies(currentNeighbor.getIndicies());

	for (int i = 0; i < indicies.size(); i++)
	{
		// Greedy knapsack with current set of items
		newNeighbor = greedyKnapsackN(k, indicies, indicies[i]);

		if (newNeighbor.getValue > bestNeighbor.getValue())
		{
			bestNeighbor = newNeighbor;
		}
	}

	return bestNeighbor;
}

void steepestDecent(knapsack &k)
{
	vector<int> indicies;
	Neighbor currentNeighbor(0, indicies);

	Neighbor nextNeighbor = greedyKnapsackN(k);

	while (currentNeighbor.getValue() < nextNeighbor.getValue())
	{
		currentNeighbor = nextNeighbor;
		nextNeighbor = bestNeighbor(k, currentNeighbor);
	}

	k.setItems(currentNeighbor.getIndicies());
}

void knapsackOutput(knapsack & k)
// Writes the results of the algorithm to an output file
{
	string fileName = "knapsack/output/knapsack" + to_string(k.getNumObjects()) + ".output";

	ofstream myfile;
	myfile.open(fileName);

	myfile << "Number of Objects: " << k.getNumObjects() << endl;
	myfile << "Total value: " << k.getValue() << endl;
	myfile << "Total cost: " << k.getCost() << endl;

	// Print out objects in the solution
	for (int i = 0; i < k.getNumObjects(); i++)
	{
		if (k.isSelected(i))
		{
			// Some unnecessary formatting to make output file look nicer
			if (i < 10)
			{
				myfile << i << "   " << k.getValue(i) << " " << k.getCost(i) << endl;
			}
			else
			{
				myfile << i << "  " << k.getValue(i) << " " << k.getCost(i) << endl;
			}
		}
	}
	myfile << endl;
	myfile.close();
}

void knapsackRun()
// Runs an algorithm to solve the knapsack problem
{
	ifstream fin;
	string fileName;

	// Read the name of the graph from the keyboard or
	// hard code it here for testing.

	// fileName = "knapsack/input/knapsack16.input";

	cout << "Enter filename" << endl;
	cin >> fileName;
	string filePath = "knapsack/input/" + fileName + ".input";

	fin.open(filePath.c_str());
	if (!fin)
	{
		cerr << "Cannot open " << fileName << endl;
		exit(1);
	}

	try
	{
		knapsack k(fin);

		//exhaustiveKnapsack(k, 600);
		//greedyKnapsack(k);
		branchAndBound(k, 600);

		// Write solution to output file
		knapsackOutput(k);

		cout << endl << "Best solution" << endl;
		k.printSolution();
	}

	catch (indexRangeError &ex)
	{
		cout << ex.what() << endl; exit(1);
	}
	catch (rangeError &ex)
	{
		cout << ex.what() << endl; exit(1);
	}

	fin.close();
}
