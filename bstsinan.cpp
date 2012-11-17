using namespace std;
#include <iostream>
#include "bstsinan.h"

int main(int argc, char** argv) {
	bnode<char> *node1 = new bnode<char>('a');
	bnode<char> *node2 = new bnode<char>('b');
	bnode<char> *node3 = new bnode<char>('c');
	bnode<char> *node4 = new bnode<char>('d');
	bnode<char> *node5 = new bnode<char>('e');
	bnode<char> *node6 = new bnode<char>('f');
	bnode<char> *node7 = new bnode<char>('g');


	bstsinan<char> *chartree = new bstsinan<char>(node1);
	chartree->root->addLnode(node2);
	chartree->root->addRnode(node3);
	node2->addLnode(node4);
	node2->addRnode(node5);
	node3->addLnode(node6);
	node3->addRnode(node7);

	cout << "top node: " << chartree->root->data << endl;
	cout << "left node: " << chartree->root->lnode->data << " right node: " << chartree->root->rnode->data << endl;
	cout << "left node: " << chartree->root->lnode->lnode->data << " right node: " << chartree->root->lnode->rnode->data;
	cout << " left node: " << chartree->root->rnode->lnode->data << " right node: " << chartree->root->rnode->rnode->data << endl;

	bnode<char> *newroot = new bnode<char>('z');
	cerr << "DEEP COPY NODE1 TO NEWROOT!" << endl;
	*newroot = *node1;
	bstsinan<char> *chartree2 = new bstsinan<char>(newroot);


	cout << "top node: " << chartree2->root->data << endl;
	cout << "left node: " << chartree2->root->lnode->data << " right node: " << chartree2->root->rnode->data << endl;
	cout << "left node: " << chartree2->root->lnode->lnode->data << " right node: " << chartree2->root->lnode->rnode->data;
	cout << " left node: " << chartree2->root->rnode->lnode->data << " right node: " << chartree2->root->rnode->rnode->data << endl;


	delete chartree;
	delete chartree2;

	return 0;
}
