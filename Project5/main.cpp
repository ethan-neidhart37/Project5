// Project 5
// Contains code to run files p5a.cpp and p5b.cpp

#include <iostream>
#include "p5a.h"
#include "p5b.h"

int main() {
	// Run p5a.cpp/p1b.cpp

	int num;
	char select;
	bool loopEnd = false;

	while (loopEnd == false)
	{
		do
		{
			std::cout << "[1] knapsack \n[2] graph coloring \n[3] quit\n";
			std::cout << "Select which project problem to run: ";
			std::cin >> num;

			switch (num)
			{
			case 1: knapsackRun(); break;
			case 2: graphColoring(); break;
			case 3: exit(0);
			default: std::cout << "Please select correct input. [1/2/3]"; break;
			}
		} while (num != 1 && num != 2);

		do
		{
			std::cout << "\nTry another project problem? [y/n]: ";
			std::cin >> select;

			switch (select)
			{
			case 'y': loopEnd = false; break;
			case 'n': loopEnd = true; break;
			default: std::cout << "Please select correct input '[y/n]"; break;
			}
			std::cout << "\n";
		} while (select != 'y' && select != 'n');
	}

	std::cin.get();
	return 0;
}