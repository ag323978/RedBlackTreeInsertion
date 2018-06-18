/*
 * Author: Aryan Gupta
 * Red-Black Tree (RBT): this program allows the user to create a RBT by inserting nodes manually, 
 * through a file, and allows them to print the nodes in the current tree
 */

#include "Node.h"
#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <cmath>

using namespace std;

//declare methods
Node * addNode(Node * head, Node * newNode);
void rotateLeft(Node *& root, Node *& newNode);
void rotateRight(Node *& root, Node *& newNode);
void correctTree(Node *& root, Node *& newNode);
void print(Node * head, int space);

int main(){
  Node* head = NULL;
  cout << "'ADD' to add a number." << endl;
  cout << "'PRINT' to print the tree." << endl; 
  cout << "'READ' to read in numbers from a file." << endl;
  cout << "Or 'EXIT' to exit the program" << endl;
  cout << '\n';
  
  while(1){
    cout << "Enter a command: ";
    char* command = new char[10];
    cin.getline(command, 10);
    int number = 0;
    if(strcmp(command, "ADD") == 0){
      //allows adding 1 number to the tree
      cout << "Enter the number you want to add: ";
      cin >> number;
      cin.get();
      //makes a new node with the number to be added
      Node* pt = new Node();
      pt->setContent(number);
      //if this is the first time (creating the tree) this function still works because a NULL pointer for head was made
      head = addNode(head, pt);
      //calls the method to fix the tree after every insertion
      correctTree(head, pt);
    }
    else if(strcmp(command, "PRINT") == 0){
      if(head == NULL){
	cout << "There is nothing in the tree" << endl;
      }else{
	print(head, 0);
	cout << '\n';
      }
    }
    else if(strcmp(command, "READ") == 0){
      int count = 0;
      int * stor = new int[1000];
      //gets filename
      cout << "Enter the filename: ";
      char* filename = new char[50];
      cin.getline(filename, 50);
      ifstream inFile;
      inFile.open(filename);
      //puts all into the storage of numbers starting from 1
      char* input2  = new char[400];		
      inFile.getline(input2, 400);
      char* token = strtok(input2, ",");
      
      while(token != NULL){
	//changes it to int and puts it in the array starting from 1
	stor[count] = atoi(token);
	//deletes token
	token = strtok(NULL, ", ");
	count ++;
      }
      //for each item in the storage call the addNode and then fixTree, one by one
      for(int i =0; i< count; i++){
	Node* pt = new Node();
	pt->setContent(stor[i]);
	head = addNode(head, pt);
	correctTree(head, pt);
      }
    }
    else if(strcmp(command, "EXIT") == 0){
      exit(0);
    }
  }	
} // int main() '}'

//inserts a node in the tree
Node * addNode(Node* head, Node* newNode){
  if(head==NULL){
    //the root of the new tree is the new node to be added
    return newNode;
  }else if(head->getContent()>=newNode->getContent()){
    head->setLeft(addNode(head->getLeft(), newNode));
    //set parent of the new node
    head->getLeft()->setParent(head);
  }else if(head->getContent()<newNode->getContent()){
    head->setRight(addNode(head->getRight(), newNode));
    //set parent of the new node
    head->getRight()->setParent(head);
  }
  return head;
}

// performs a left rotation
void rotateLeft(Node *& root, Node *& newNode) {
  //makes a right node of the new node for easier rotation
  Node * right = newNode->getRight();
  newNode->setRight(right->getLeft());
  //checks if the node to be set actually exist before proceding with rotation
  if (newNode->getRight() != NULL){
    newNode->getRight()->setParent(newNode);
  }
  right->setParent(newNode->getParent());
  
  if (newNode->getParent() == NULL){
    root = right;
  }
  else if (newNode == newNode->getParent()->getLeft()){
    newNode->getParent()->setLeft(right);
  }
  else{
    newNode->getParent()->setRight(right);
  }
  right->setLeft(newNode);
  newNode->setParent(right);
}

//performs a right rotation
void rotateRight(Node *& root, Node *& newNode) {
  Node * left = newNode->getLeft();
  newNode->setLeft(left->getRight());
  if (newNode->getLeft() != NULL){
    newNode->getLeft()->setParent(newNode);
  }
  left->setParent(newNode->getParent());
  
  if (newNode->getParent() == NULL){
    root = left;
  }
  else if (newNode ==  newNode->getParent()->getLeft()){
    newNode->getParent()->setLeft(left);
  }
  else{
    newNode->getParent()->setRight(left);
  }
  left->setRight(newNode);
  newNode->setParent(left);
}

//call this function to fix the tree after every insertion
void correctTree(Node *& root, Node *& newNode){
  //makes parent and grandparent of newly inserted node (helps with checking uncle and if parent is left or right)
  Node * parent = NULL;
  Node * grandParent = NULL;
  while ((newNode != root) && (newNode->getColor() != 2) && (newNode->getParent()->getColor() == 1)) {
      //set variables
      parent = newNode->getParent();
      grandParent = newNode->getParent()->getParent();
      //if the parent is the left of grandparent
      if (parent == grandParent->getLeft()) {
	  Node * uncle = grandParent->getRight();
	  // if the uncle is red recolor the nodes
	  if (uncle != NULL && uncle->getColor() == 1) {
	    grandParent->setColor(1);
	    parent->setColor(2);
	    uncle->setColor(2);
	    newNode = grandParent;
	  }
	  else {
	    //  if the new node is the right of parent rotate the tree left
	    if (newNode == parent->getRight()) {
	      rotateLeft(root, parent);
	      newNode = parent;
	      parent = newNode->getParent();
	    }
	    rotateRight(root, grandParent);
	    int parentColor = parent->getColor();
	    int grandParentColor = grandParent->getColor();
	    parent->setColor(grandParentColor);
	    grandParent->setColor(parentColor);
	    newNode = parent;
	  }
        }
      
      // otherwise the parent is right of grandparent
      else {
	Node * uncle = grandParent->getLeft();
	// if the uncle is read then recolor
	if ((uncle != NULL) && (uncle->getColor() == 1)) {
	  grandParent->setColor(1);
	  parent->setColor(2);
	  uncle->setColor(2);
	  newNode = grandParent;
	}
	else {
	  //the new node is left of its parent
	  if (newNode == parent->getLeft()) {
	    rotateRight(root, parent);
	    newNode = parent;
	    parent = newNode->getParent();
	  }
	  rotateLeft(root, grandParent);
	  int parentColor = parent->getColor();
	  int grandParentColor = grandParent->getColor();
	  parent->setColor(grandParentColor);
	  grandParent->setColor(parentColor);
	  newNode = parent;
	}
      }
  }
  //set the root to be black (because the root must always be black in the end)
  root->setColor(2);
  
}

//prints the tree spacing to make things look clear
void print(Node * head, int space){
  if (head == NULL) {
    return;
  }
  space += 10;
  print(head->getRight(), space);
  cout << '\n';
  for (int i = 10; i < space; i++)
    cout << " ";
  
  if(head->getColor()==1){
    cout <<"red: " << head->getContent();
  }else{
    cout << "black: " << head->getContent();
  }
  print(head->getLeft(), space);
}

