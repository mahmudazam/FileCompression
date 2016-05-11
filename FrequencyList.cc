// NAME: MAHMUD AZAM
// STUDENT NUMBER: 11211602
// NSID: mfa256
// LECTURE SECTION: 02

// File: FrequencyList.cc
// CMPT 115 Assignment 9 (2015-2016)
// Synopsis: Defines the implementation for the FrequencyList ADT


#include "FrequencyList.h"
#include "Frequency.h"
#include "List.h"
#include "TreeNode.h"

#include <cstdlib>
#include <cstring>
#include <iostream>

using namespace std;

// CONSTRUCTORS

// createFrequencyList(message)
// create the input list for the Huffman algorithm.
// each node of the list contains a one node Tree.
// the data stored in each Tree is a Frequency.
// Pre: message:: refToChar, the message to count frequencies within
// Return: a reference to the generated list.
// post:  a new list is allocated
// return: reference to the new list

FrequencyList *createFrequencyList(const char* message){
	if (message == NULL) 
	{
		cerr << "Error in createFrequencyList(): NULL message!" << endl;
		return NULL;
	}

	if(strlen(message) > 0) {
		Frequency *f = createFrequency(message[0], 1);
		char *sub = new char[strlen(message) + 1];
		sub[0] = '\0';
		int count = 0;
		for(int i = 1; i <= strlen(message); i++) {
			if(message[i] != message[0]) {
				sub[count] = message[i];
				sub[count + 1] = '\0';
				count++;
			} else {
				setCount(f, getCount(f) + 1);
			}
		}
		TreeNode *t = createTreeNode(f);
		FrequencyList *fList = new FrequencyList;
		fList->freqs = createList();
		insert(fList->freqs, t);
		// cout << "Count of '" << getData(getData(t)) << "' is " << getCount(getData(t)) << ". " << endl;
		FrequencyList *subList = createFrequencyList(sub);
		if(subList != NULL) {
			Element *dataOut = new Element;
			while(deleteHead(subList->freqs, dataOut)) {
				insert(fList->freqs, *dataOut);
			}
			destroyFrequencyList(subList);
			delete dataOut;
		}
		delete [] sub;
		return fList;
	} else {
		return NULL;
	}
}


// remove the smallest frequency tree from the list
// Pre: l is a list containing Trees of Frequencies.
// Post: the tree with smallest frequency has been removed
//       from the list.
// return: the Tree with smallest frequency in List l.
//         Will return NULL if the list was empty.
TreeNode *remove_smallest(FrequencyList *l){
	if (l == NULL) {
		cerr << "Error in remove_smallest: given NULL list" 
		<< "  Returning NULL, but anything could happen."
		<< endl;
		return NULL;
	}
	Iterator *iter = createIterator(l->freqs);
	Element current = NULL;
	Element minFreqTreeNode = NULL;
	if(hasNext(iter)) {
		current = next(iter);
		minFreqTreeNode = current;
	} else {
		return NULL;
	}
	int min = getCount(getData(minFreqTreeNode));
	while(hasNext(iter)) {
		current = next(iter);
		if(getCount(getData(current)) < min) {
			minFreqTreeNode = current;
			min = getCount(getData(minFreqTreeNode));
		}
	}
	remove(l->freqs, minFreqTreeNode);
	destroyIterator(iter);
	return minFreqTreeNode;
}

// destroyFrequencyList(it)
// pre: it is a reference to an iterator
// post: the iterator is deallocated
void destroyFrequencyList(FrequencyList *l) {
  if (l == NULL) {
    cerr << "Error in destroyFrequencyList(): given NULL list" 
      << endl;
    return;
  }

  if (l->freqs != NULL) {
    destroyList(l->freqs);
  }
  delete l;
}




//ACCESSORS

// isEmpty(l)
// pre: l is a reference to a FrequencyList
// return: true if l is empty
bool isEmpty(FrequencyList *l){
  if (l == NULL) {
    cerr << "Error in isEmpty(): given NULL list" 
	 << "  Returning true, but anything could happen."
      << endl;
    return true;
  }
  return isEmpty(l->freqs);
}

// getNumElements(l)
// pre: l is a reference to a FrequencyList
// return: the number of elements in the list
int getNumElements(FrequencyList *l){
  if (l == NULL) {
    cerr << "Error in getNumElemnts(): given NULL list" 
	 << "  Returning 0, but anything could happen."
      << endl;
    return 0;
  }
  return getNumElements(l->freqs);
}

//MUTATORS

// deleteHead(l,elt)
// delete the head element of the list
// pre: l is a reference to a FrequencyList
// pre: elt is a reference to an element
// post: the first element is removed from the list. 
//       el stores a reference to the removed element.
// return: true if successful, false otherwise.
bool deleteHead(FrequencyList *l, Element *elt){
  if (l == NULL) {
    cerr << "Error in deleteHead(): given NULL list" 
	 << "  Returning false, but anything could happen."
      << endl;
    return false;
  }
  return deleteHead(l->freqs, elt);
}

// insert(l,d)
// pre: l is a reference to a FrequencyList
//      d is a reference to an element
// post: The data is added to the end of the list
// return: true if successful, false iotherwise
bool insert(FrequencyList *l, Element dataIn){
  if (l == NULL) {
    cerr << "Error in insert(): given NULL list" 
	 << "  Returning false, but anything could happen."
      << endl;
    return false;
  }
  return insert(l->freqs, dataIn);
}

// remove(l,d)
// pre: l is a reference to a FrequencyList
//      d is a reference to an Element
// post: if d is in the list, then it has been removed
// return: true if the element was removed, false otherwise
bool remove(FrequencyList *l, Element toRemove){
  if (l == NULL) {
    cerr << "Error in remove(): given NULL list" 
	 << "  Returning false, but anything could happen."
      << endl;
    return false;
  }
  return remove(l->freqs,toRemove);
}


// eof

