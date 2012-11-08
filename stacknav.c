/*
 * stacknav.c
 * Copyright 2012 Sinan Akpolat
 *
 * c++ file which runs the stack navigator.
 *
 * This file is distributed under GNU GPLv3, see LICENSE file.
 * If you haven't received a file named LICENSE see <http://www.gnu.org/licences>
 *
 * Stack navigator is distributed WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE
 */

using namespace std;
#include <iostream>
#include "map.h"
#include "stacknav.h"

int main(int argc, char **argv) {
	//TODO: we should read the map name from argv!
	map *myMap = new map("testmap2");

	stacknav *spnav = new stacknav(myMap->getStart(), myMap->getEnd(), myMap);
	spnav->solve_shortest();

	return 0;
}
