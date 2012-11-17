/*
 * map.cpp
 *
 *  Created on: Sep 26, 2012
 *      Author: sinan
 */

// map file is a text file for now
// first line = dimensions in form # x #
// second line = start point
// third line = end point
// map bitmap
using namespace std;
#include <iostream>
#include "map.h"
#include "stacknav.h"

int main(int argc, char **argv) {

	map *myMap = new map("testmap1");
	cout << *myMap << endl;

	stacknav *spnav = new stacknav(myMap->getStart(), myMap->getEnd(), myMap);
	spnav->solve_shortest();
	digraph<coord> *myDig = new digraph<coord>;

	myMap->convert_map2dig(myDig);

	myDig->printGraph();
	myDig->solveShortestPath(myMap->coord2ind(myMap->getStart()),myMap->coord2ind(myMap->getEnd()));

	cout << endl << myMap->coord2ind(myMap->getStart()) << ", " <<  myMap->coord2ind(myMap->getEnd()) << endl;
	cout << myMap->getStart() << ", " << myMap->getEnd() << endl;

	return 0;
}











