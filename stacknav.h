/*
 * stacknav.h
 * Copyright 2012 Sinan Akpolat
 *
 * Stack navigator!
 * Navigates the map using a stack (umm, vector, since STL doesn't provide iterators for stacks).
 * Finds paths between two points. Prints the shortest path found.
 *
 * This file is distributed under GNU GPLv3, see LICENSE file.
 * If you haven't received a file named LICENSE see <http://www.gnu.org/licences>
 *
 * Stack navigator is distributed WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE
 */

#ifndef _STACKNAV_H_
#define _STACKNAV_H_

using namespace std;
#include <iostream>
#include <stack>
#include <vector>
#include "map.h"

#define WALL 1 //map nodes with index 1 are walls
//bitmasks for char move::faildirs
#define NORTH_FAIL 0x1
#define EAST_FAIL  0x2
#define SOUTH_FAIL 0x4
#define WEST_FAIL  0x8

class move {
public:
	move(coord location);
	move& operator= (const move& rhsmove);
	coord loc; //where we landed after moving
	char faildirs; //directions we already tried to go from this coordinate (bitmask)
	char lastdir; //the direction we used to enter the node (when going forward, not back-tracing)
};

move::move(coord location) {
	loc = location;
	faildirs = 0;
	lastdir = '\0';
}
move& move::operator=(const move& rhsmove) {
	if(this == &rhsmove) return *this; //check for self assignment
	this->loc = rhsmove.loc;
	this->faildirs = rhsmove.faildirs;
	this->lastdir = rhsmove.lastdir;
	return *this; //dereference self pointer and return...
}

class stacknav {
public:
	stacknav(coord start, coord end, map *pmap);
	void solve_shortest();
	void printpath(vector<move> path);
	bool passable(coord nextstep); //is next step passable?
	bool passableN(map_node* nextstepN); //same thing for node pointers
	map_node *curnode; //current node

private:
	vector<move> moves; //previous moves. Used vector since stack class doesn't provide iterators
	vector<move> shpath; //latest shortest path
	bool checkmove(coord nextstep); //check if next step is valid and useful
	coord curloc; //current location
	coord start; //start pos on map
	coord end; //end pos on map
	map *pmap; //our map
	bool stuck; //We tried every possible path and we can't go anywhere else
	int splength; //last found shortest path length
	string spstring; //used to print shortest path directions
};

stacknav::stacknav(coord start, coord end, map *pmap) {
	//make sanity checks
	//TODO: add sanity checks for start and end coords. (ie are they inside map, are they passable?)
	this->start = start;
	curloc = start;
	this->end = end;
	this->pmap = pmap;
	curnode = pmap->getNode(start);
	splength = pmap->getMapArea(); //shortest path can't be longer than the number of all nodes in map
	stuck = false;
	move *newmove = new move(start);
	newmove->lastdir = '\0';
	moves.push_back(*newmove);
}

bool stacknav::passable(coord nextstep) {
	if(pmap->getNode(nextstep)->getIndex() == WALL) return false;
	else return true;
}
bool stacknav::passableN(map_node *nextstepN) {
	//if the node doesn't exist or it is a wall
	if(!nextstepN || nextstepN->getIndex() == WALL) return false;
	else return true;
}

void stacknav::printpath(vector<move> path) {
	if(path.size() == 0) return;
	vector<move>::iterator iter;
	for(iter=path.begin();iter<path.end();iter++) {
		cout << (*iter).lastdir << " " << (*iter).loc << " ";
	}
	cout << endl;
}

bool stacknav::checkmove(coord nextstep) {
	if(moves.size()==0) return true;
	vector<move>::iterator iter;
	bool notfound = true;
	if(nextstep == end) { //we have found the exit, return back to search other paths
		cout << "exit found!" << endl;
		cout << "shortest path was " << splength <<" long." << endl;
		if(splength > moves.size()) splength = moves.size()+1;
		cout << "current path is " << splength <<" long:" << endl;
		shpath = moves;
		printpath(shpath);
		return false;
	}
	if(moves.size() >= splength) { //this path can't be the shortest path
		cerr << "this path is not optimal." << endl;
		return false;
	}
	for(iter=moves.begin();iter<moves.end()-1;iter++) {
		if((*iter).loc == nextstep) { //we passed from this path, it can't be the shortest path (worse yet we get get caught in a loop)
			cerr << "we passed from this node before." << endl;
			notfound = false;
			break;
		}
	}
	return notfound;
}

/*
 * The function that seeks shortest paths.
 * If only I didn't use different links and functions for each direction this code would look far more elegant
 * It consist of three parts:
 * 1) First it checks if it can go to any of the four directions from its current position
 * 2) If it can't it tries to go back the way it came
 * 3) If that fails too (because whole map was searched and there is nowhere else to go) it returns
 */

void stacknav::solve_shortest() {
	while (!stuck){
		/*
		 * First part, for each direction the conditions in if statements translate to:
		 * 1) If we moved the opposite direction last turn, going back is meaningless (and causes a loop), go elsewhere if you can.
		 * 2) If we already moved some direction and did what we can do there, it is marked in faildirs bitmask. We don't need to check that direction again.
		 * 3) Is there a wall in that direction? Can we move there?
		 * 4) Check
		 * 		* if we found the exit?
		 * 		* if we are still moving in a shorter path than before?
		 * 		* if we passed from the node we are planning to enter before?
		 */
		if(moves.back().lastdir != 's' && !(moves.back().faildirs & NORTH_FAIL) && passableN(curnode->northN()) && checkmove(curnode->north())){
			//if we moved south last turn, moving back north is meaningless
			//if moving north from this location failed before we won't try it again
			//if we have a wall at north we can't move there
			//finally we may have passed from the location to the north or may be we are no longer on the shortest path etc.
			//move north
			curnode = curnode->northN();
			curloc = curnode->getCoord();
			move *newmove = new move(curloc);
			newmove->lastdir = 'n';  //which means we've entered this node by going north from our previous position
			cerr << "n " << curloc << endl;	//print move to commandline
			moves.push_back(*newmove); //push the move to the top of stack
		}
		else if(moves.back().lastdir != 'w' && !(moves.back().faildirs & EAST_FAIL) && passableN(curnode->eastN()) && checkmove(curnode->east())){
			//move east
			curnode = curnode->eastN();
			curloc = curnode->getCoord();
			move *newmove = new move(curloc);
			newmove->lastdir = 'e';
			cerr << "e " << curloc << endl;
			moves.push_back(*newmove);
		}
		else if(moves.back().lastdir != 'n' && !(moves.back().faildirs & SOUTH_FAIL) && passableN(curnode->southN()) && checkmove(curnode->south())){
			//move south
			curnode = curnode->southN();
			curloc = curnode->getCoord();
			move *newmove = new move(curloc);
			newmove->lastdir = 's';
			cerr << "s " << curloc << endl;
			moves.push_back(*newmove);
		}
		else if(moves.back().lastdir != 'e' && !(moves.back().faildirs & WEST_FAIL) && passableN(curnode->westN()) && checkmove(curnode->west())){
			//move west
			curnode = curnode->westN();
			curloc = curnode->getCoord();
			move *newmove = new move(curloc);
			newmove->lastdir = 'w';
			cerr << "w " << curloc << endl;
			moves.push_back(*newmove);
		}
		 //if all four directions fail, go back
		else if(moves.back().lastdir == 'n') {
			//go back south
			curnode = curnode->southN();
			curloc = curnode->getCoord();
			moves.pop_back(); //pop the move from stack
			moves.back().faildirs |= NORTH_FAIL; //mark the direction so that we won't try going there again
			cerr << "took a step back to " << moves.back().loc << ", faildirs bitmask is " << (int) moves.back().faildirs << endl;
		}
		else if(moves.back().lastdir == 'e') {
			//go back west
			curnode = curnode->westN();
			curloc = curnode->getCoord();
			moves.pop_back();
			moves.back().faildirs |= EAST_FAIL;
			cerr << "took a step back to " << moves.back().loc << ", faildirs bitmask is " << (int) moves.back().faildirs << endl;
		}
		else if(moves.back().lastdir == 's') {
			//go back north
			curnode = curnode->northN();
			curloc = curnode->getCoord();
			moves.pop_back();
			moves.back().faildirs |= SOUTH_FAIL;
			cerr << "took a step back to " << moves.back().loc << ", faildirs bitmask is " << (int) moves.back().faildirs << endl;
		}
		else if(moves.back().lastdir == 'w') {
			//go back east
			curnode = curnode->eastN();
			curloc = curnode->getCoord();
			moves.pop_back();
			moves.back().faildirs |= WEST_FAIL;
			cerr << "took a step back to " << moves.back().loc << ", faildirs bitmask is " << (int) moves.back().faildirs << endl;
		}
		else {
			//we have tried everything and we can't move anywhere else
			cerr << "I can't go anywhere else!" << endl;
			cout << "shortest path I found is "<< splength << " long: " << endl;
			printpath(shpath);
			stuck = true;
		}

	}
	return;
}

#endif
