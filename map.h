/*
 * map.h
 * Copyright 2012 Sinan Akpolat
 *
 * A 2D orthogonal discrete map class.
 * By discrete I mean the map consists of solid nodes and coordinates are all integers.
 * By orthogonal I mean adjacent nodes are placed in one of the four directions,
 * north, east, south, west with respect to each other.
 * It uses a vector to store map data and navigates nodes by calculating their
 * index with (x+mapwidth*y). Pretty clumsy but gets the job done.
 *
 * This file is distributed under GNU GPLv3, see LICENSE file.
 * If you haven't received a file named LICENSE see <http://www.gnu.org/licences>
 *
 * This map class is distributed WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE
 */

#ifndef _MAP_H_
#define _MAP_H_

using namespace std;
#include <iostream>
#include <fstream> //for reading text files
#include <cstdlib> //atoi(), for reading numbers from text files
#include <vector>

class coord {	//integer coordinates, like a point
public:
	coord(int x, int y);
	coord();
	int getx();
	int gety();
	coord& operator+ (const coord& rhscoord);
	coord& operator= (const coord& rhscoord);
	bool operator==(const coord& rhscoord);
	bool operator!=(const coord& rhscoord);
	friend ostream& operator<< (ostream& os, const coord& ocoord);
protected:
	int x;
	int y;
};

class map;

class map_node {
public:
	map_node(map* pmap, coord loc, char index = -1);
	coord getCoord();
	char getIndex();
	map* getMap();
	coord north();
	coord south();
	coord west();
	coord east();
	//and for the sake of chaining:
	map_node* northN();
	map_node* southN();
	map_node* westN();
	map_node* eastN();

private:
	char index; //is it a wall or something else?
	//0 path, 1 wall, -1 null node (for map borders)
	coord loc;
	map *pmap; //pointer to the map object node belongs to...

	//TODO: let's use index to store weighted graphs (0=wall, positive value = weight)
	//TODO: also change its name and make it unsigned int

};

class map {
public:
	map(coord dim, coord start, coord end); //init an empty map object
	map(coord dim, coord start, coord end, char *indexData); //read map from binary char array (not text)
	map(coord dim, char *indexData); //read map from binary char array (not text)
	map(char *filename); //read map from a text file
	~map(); //free the nodes vector
	map(const map& orig); //copy constructor, makes deep copies by allocating a new vector
	int getMapWidth();
	int getMapHeight();
	int getMapArea();
	coord getDim();
	coord getStart();
	coord getEnd();
	map_node *getNode(coord nodecoord);
	friend ostream& operator<<(ostream& os, map& omap);

private:
	vector<map_node> *mapData; //delete this when map object is destroyed
	coord dim; //dimensions of map
	coord start; //starting position
	coord end;	//end position
};

coord::coord(int x, int y) {
	this->x=x;
	this->y=y;
}

coord::coord() {
	x=0;
	y=0;
}

int coord::getx() { return this->x; }
int coord::gety() { return this->y; }

coord& coord::operator=(const coord& rhscoord) {
	if(this == &rhscoord) return *this; //check for self assignment
	//isn't rhs shallow copied? So why worry about self assignment?
	this->x = rhscoord.x;
	this->y = rhscoord.y;
	return *this; //dereference self pointer and return...
}

coord& coord::operator+(const coord& rhscoord) {
	coord result_coord(0,0);
	result_coord.x = this->x + rhscoord.x;
	result_coord.y = this->y + rhscoord.y;
	return result_coord;
	//TODO: this invokes a warning, how to correct this? I need to return that local var.
}

bool coord::operator ==(const coord& rhscoord) {
	if(x == rhscoord.x && y == rhscoord.y) return true;
	else return false;
}

bool coord::operator !=(const coord& rhscoord) {
	if(x == rhscoord.x && y == rhscoord.y) return false;
	else return true;
}

ostream& operator<<(ostream& os, const coord& ocoord) {
	os << "(" << ocoord.x << "," << ocoord.y << ")";
	return os;
}

map_node::map_node(map* pmap, coord loc, char index) {
	this->pmap = pmap;
	this->loc = loc;
	this->index = index;
}

char map_node::getIndex() {return this->index; }
coord map_node::getCoord() {return this->loc; }
map* map_node::getMap() {return this->pmap; }

coord map_node::east() {
	if(this->loc.getx() < this->pmap->getMapWidth()-1) {
		return this->loc + coord(1,0);
	}
	else return coord(-1,-1);
}
coord map_node::west() {
	if(this->loc.getx() > 0) {
		return this->loc + coord(-1,0);
	}
	else return coord(-1,-1);
}
coord map_node::north() {
	if(this->loc.gety()>0) {
		return this->loc + coord(0,-1);
	}
	else return coord(-1,-1);
}
coord map_node::south() {
	if(this->loc.gety() < this->pmap->getMapHeight()-1) {
		return this->loc + coord(0,1);
	}
	else return coord(-1,-1);
}

map_node* map_node::eastN() {
	if(this->loc.getx() < this->pmap->getMapWidth()-1) {
		return this->getMap()->getNode(this->loc + coord(1,0));
	}
	else return 0;
}
map_node* map_node::westN() {
	if(this->loc.getx() > 0) {
		return this->getMap()->getNode(this->loc + coord(-1,0));
	}
	else return 0;
}
map_node* map_node::northN() {
	if(this->loc.gety()>0) {
		return this->getMap()->getNode(this->loc + coord(0,-1));
	}
	else return 0;
}
map_node* map_node::southN() {
	if(this->loc.gety() < this->pmap->getMapHeight()-1) {
		return this->getMap()->getNode(this->loc + coord(0,1));
	}
	else return 0;
}

map::map(coord dim, coord start, coord end) {
	this->dim = dim;
	this->start = start;
	this->end = end; // i don't need a operator= since i only need to copy the data part of coords, no pointers are involved.
	mapData = new vector<map_node>;
}

map::map(coord dim, coord start, coord end, char* indexData) {
	this->dim = dim;
	this->start = start;
	this->end = end; // i don't need a operator= since i only need to copy the data part of coords, no pointers are involved.
	int i, j;
	mapData = new vector<map_node>;
	for(j=0; j<dim.gety(); j++) {
		for(i=0; i<dim.getx(); i++)
			mapData->push_back(map_node(this, coord(i,j), indexData[i+dim.getx()*j]));
			//we don't need a copy constructor for map_node class to push instances of it
			//Copying it's members' values is enough

	}
}

map::map(char *filename) { //read map from a text file
	string readstr;
	int cutstr; //position to cut strings so that we can access x and y dimensions
	string strx; //coord.x string
	string stry; //coord.y string
	char index;
	int i,j;

	ifstream mapfile(filename);
	if(!mapfile) {
		cerr << "Cannot open map file " << filename << endl;
		return; //how can a constructor fail?
	}

	//read dimensions
	getline(mapfile, readstr);
	cutstr = readstr.find_first_of(' ');
	//TODO: error log
	strx = readstr.substr(0, cutstr);
	stry = readstr.substr(cutstr); //from cutstr to the end
	cout << "dimensions seem to be x: "<< strx << " y: " << stry << endl;
	this->dim = coord(atoi(strx.c_str()), atoi(stry.c_str()));
	//read start
	getline(mapfile, readstr);
	cutstr = readstr.find_first_of(' ');
	//TODO: error log
	strx = readstr.substr(0, cutstr);
	stry = readstr.substr(cutstr); //from cutstr to the end
	cout << "start pos seem to be x: "<< strx << " y: " << stry << endl;
	this->start = coord(atoi(strx.c_str()), atoi(stry.c_str()));
	//read end
	getline(mapfile, readstr);
	cutstr = readstr.find_first_of(' ');
	//TODO: error log
	strx = readstr.substr(0, cutstr);
	stry = readstr.substr(cutstr); //from cutstr to the end
	cout << "end pos seem to be x: "<< strx << " y: " << stry << endl;
	this->end = coord(atoi(strx.c_str()), atoi(stry.c_str()));

	//read mapdata and create nodes immediately
	mapData = new vector<map_node>;
	for(j=0; j<dim.gety(); j++) {
		getline(mapfile, readstr);
		cout << "read line: " << readstr << endl;
		for(i=0; i<dim.getx(); i++) {
			if(readstr.data()[i] == '0') index = 0;
			else index = 1;
			mapData->push_back(map_node(this, coord(i,j), index));
		}
	}
	cout << "and the map looks something like this:" << endl;
	cout << *this;
}

int map::getMapHeight() {return this->dim.gety();}
int map::getMapWidth() {return this->dim.getx();}
int map::getMapArea() {return (this->dim.getx()) * (this->dim.gety());}
coord map::getDim() {return this->dim;}
coord map::getStart() {return this->start;}
coord map::getEnd() {return this->end;}

map::~map() {
	delete this->mapData;
}

map::map(const map& orig) { //copy constructor, makes deep copies by allocating a new vector
	this->mapData = new vector<map_node>;
	this->mapData = orig.mapData;
	this->dim = orig.dim;
	this->start = orig.start;
	this->end = orig.end;
}


map_node* map::getNode(coord nodecoord) {
	int nodeIndex = nodecoord.getx()+this->getMapWidth()*nodecoord.gety();
	map_node *theNode = &(this->mapData->at(nodeIndex));
	return theNode;
}

ostream& operator<<(ostream& os, map& omap) {
	int i,j;
	os << "Dimensions: " << omap.dim << endl << "Start coord: " << omap.start << endl << "End coord: " << omap.end << endl;
	os << "vector capacity: " << omap.mapData->capacity() << endl << "vector size: " << omap.mapData->size() << endl;
	for(i=0;i<omap.getMapHeight(); i++) {
		for(j=0; j<omap.getMapWidth(); j++) {
			os << (int) omap.mapData->at(i*omap.getMapHeight()+j).getIndex();
		}
		os << endl;
	}
	return os;
}

//I'll need a operator= for map class though since map data will be held in a memory block pointed by a pointer.
//I'll also need a copy constructor

#endif
