// Knapsack class
// Version f08.1
#include <iostream>
#include <vector>
#include <fstream>
#include "d_except.h"

using namespace std;

class knapsack
{
public:
	knapsack(ifstream &fin);
	knapsack(const knapsack &);
	knapsack &operator=(const knapsack &k);
	int getCost(int) const;
	int getValue(int) const;
	int getCost() const;
	int getValue() const;
	int getNumObjects() const;
	int getCostLimit() const;
	int getCurrentItem() const;
	float getPriority(int) const;
	float bound(int currItem);
	void nextItem();
	void printSolution();
	void select(int);
	void unSelect(int);
	bool isSelected(int) const;
	bool isFathomed(int incumbent);
	vector<bool> getSelected();
	vector<int> sort() const;
	vector<int> getIndicies() const;
	void setItems(vector<int> includedIndicies);

private:
	int numObjects;
	int costLimit;
	vector<int> value;
	vector<int> cost;
	vector<bool> selected;
	int totalValue;
	int totalCost;
	int currentItem;
	int partition(vector<int> &items, int left, int right, int pivot) const;
	void quicksort(vector<int> &items, int left, int right) const;
};
