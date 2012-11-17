#ifndef _BSTSINAN_H_
#define _BSTSINAN_H_

//i'm writing my first container class :)

/*
 * Well, I read chapters 12 and 15 on Larry Nyhoff's
 * "ADTs, Data Structures and Problem Solving with C++"
 * and realized this is nothing.
 */

using namespace std;
#include <iostream>

//generic type
template <typename elementType>
class bnode {
public:
	bnode(elementType data, bnode* upnode = 0);
	bnode();
	//we can write this shortly as:
	//bnode(elementType data) : data(data), upnode(0), lnode(0), rnode(0) {}
	bnode(const bnode& org); //copy constructor from original, just copies pointers
	~bnode();
	//TODO: write an = operator (which allocates new mem for new node ie. deep copy)
	bnode& operator= (const bnode& rhsbnode);
	bnode *addLnode(bnode *newNode);
	bnode *addRnode(bnode *newNode);
	//bnode *copynode(bnode *orgNode); //use = operator deep copies instead
	elementType data;
	bnode *lnode;
	bnode *rnode;
	bnode *upnode;
};

template <typename elementType>
class bstsinan {
public:
	bstsinan(bnode<elementType> *newroot);
	bstsinan(const bstsinan& orgtree);
	~bstsinan();
	//TODO: write an = operator
	bnode<elementType>* root;
};

template <typename elementType>
bnode<elementType>::bnode(elementType data, bnode<elementType>* upnode) {
	this->data = data;
	this->upnode = upnode;
	cerr << this << " is created with upnode " << this->upnode << " and data " << this->data << endl;
	this->lnode = 0;
	this->rnode = 0;
}
template <typename elementType>
bnode<elementType>::bnode() {
	//this->data = elementType;
	this->upnode = 0;
	this->lnode = 0;
	this->rnode = 0;
}

template <typename elementType>
bnode<elementType>::bnode(const bnode& org) {
	this->data = org.data;
	cerr << "copying node:" << &org;
	this->upnode = org.upnode;
	this->lnode = org.lnode;
	this->rnode = org.rnode;
	if(org.lnode) {
		cerr << "will shallowly copy lnode @ " << org.lnode;
		*(this->lnode) = *(org.lnode); //copy recursively
	}
	if(org.rnode) {
		cerr << "will shallowly copy rnode @ " << org.rnode;
		*(this->rnode) = *(org.rnode); //copy recursively
	}
}


template <typename elementType>
bnode<elementType>::~bnode() {
	if(lnode != 0) {
		cerr << "deleting lnode: " << lnode << endl;
		delete lnode;
	}
	if(rnode != 0) {
		cerr << "deleting rnode: " << rnode << endl;
		delete rnode;
	}
}

template <typename elementType>
bnode<elementType>& bnode<elementType>::operator=(const bnode<elementType>& rhsbnode) {
	bnode<elementType> *newnode = this;
	//newnode = new bnode<elementType>(rhsbnode.data, rhsbnode.upnode);
	newnode->data = rhsbnode.data;
	newnode->upnode = rhsbnode.upnode;
	cerr << "new node has data " << newnode->data << endl;
	cerr << "new node " << newnode << " with upnode " << newnode->upnode << " created, moving to leaves." << endl;
	//newnode is  connected to original node's parent, the topmost node's upnode should be zeroed.
	//it should be zeroed in the line calling this recursive function most likely

	if(rhsbnode.lnode) {
		newnode->lnode = new bnode<elementType>();
		cerr << "deep copying " << rhsbnode.lnode << " to lnode of " << newnode << endl;
		*(newnode->lnode) = *(rhsbnode.lnode);
	}
	else {
		cerr << "no left node for " << newnode << endl;
		newnode->lnode = 0;
	}
	if(rhsbnode.rnode) {
		newnode->rnode = new bnode<elementType>();
		cerr << "deep copying " << rhsbnode.rnode << " to rnode of " << newnode << endl;
		*(newnode->rnode) = *(rhsbnode.rnode);
	}
	else {
		cerr << "no right node for " << newnode << endl;
		newnode->rnode = 0;
	}
	return *(newnode);
	//return 0;
}

template <typename elementType>
bnode<elementType>* bnode<elementType>::addLnode(bnode<elementType> *newNode) {
	lnode = newNode;
	lnode->upnode = this;
	cerr << "new node " << lnode << " added as left child of " << this << endl;
	return lnode; //so that it will be easier to add nodes one after another
}

template <typename elementType>
bnode<elementType>* bnode<elementType>::addRnode(bnode<elementType> *newNode) {
	rnode = newNode;
	rnode->upnode = this;
	cerr << "new node " << rnode << " added as right child of " << this << endl;
	return rnode; //so that it will be easier to add nodes one after another
}

template <typename elementType>
bstsinan<elementType>::bstsinan(bnode<elementType> *newroot) {
	this->root = newroot;
}

template <typename elementType>
bstsinan<elementType>::~bstsinan() {
	//destroy everything
	delete root;
	cerr << "root node deleted" << endl;
}

template <typename elementType>
bstsinan<elementType>::bstsinan(const bstsinan& orgtree) {
	//copy everything, this should be enough
	cerr << "copying whole tree!" << endl;
	*(this->root) = *(orgtree.root);
}




#endif
