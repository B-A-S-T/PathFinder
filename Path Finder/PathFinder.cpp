/*
	Name: Ian Thomas
	UsrId: igt0001
	Filename igt0001_3.cpp
	
*/
#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>
#include <sstream>
#include <vector>
#include <algorithm>

using namespace std;

//Node class
class Node
{
public:
	Node(string newname);
	void setNodeName(string newname);
	string getNodeName();
	void attachNewNode(Node* newNode, int direction);
	Node* getAttachedNodes(int direction);
	Node();
private:
	string name;
	Node* attachedNodes[4];
};
// graph class
class Graph
{
public:
	Node* makeThisNode(string data);
	bool checkIfMade(vector<Node*>& vec, string data, int nodeCount);
	Node* findThisNode(vector<Node*>& vec, string data, int nodeCount);
	bool getFound();
	string getPath();
	void setStartEnd(string starting, string ending);
	void findPath(vector<Node*>& nodeVec, int count);
	void createGraph(string configureContents, int count);
	void recursiveCall(bool &foundit, Node* current, Node* prev);
	Graph();
private:
	string path;
	string first;
	string last;
	Node* current;
	bool found;
};
// System class
class System
{
public:
	bool readfile(string file);
	string getStart();
	string getEnd();
	string getPath();
	void makeGraph();
	string getContents();
	void passStartEnd();
	bool checkIfFound();
	System();
private:
	string start;
	int nodeCount;
	string end;
	string contents;
	Graph myGraph;
};
// Interface class
class Interface
{
public:
	bool displayInterface();
	Interface();
private:
	System syst;

};

int main(){

	cout << "##### Welcome to the Automatic Maze Path Finder #####\n" << endl;
	bool rep = true;
	while(rep){
		Interface inter;
		rep = inter.displayInterface();
	}
	cout << "##### Thank You for using the Automatic Maze Path Finder! #####" << endl;
	return 0;
}

//NodeClass
Node::Node() : attachedNodes(){
	name = "";
}

Node::Node(string newname) : attachedNodes(){ 
	name = newname;
}

string Node::getNodeName(){
	return name;
}

void Node::attachNewNode(Node *newNode, int direction){
	attachedNodes[direction] = newNode;
}

Node* Node::getAttachedNodes(int direction){ // gets the attached node at to direction requested
	if (attachedNodes[direction] == NULL) // no pointer in that direction
	{
		Node* p = NULL;
		return p;
	}
	else return attachedNodes[direction];
}

void Node::setNodeName(string newname){
	name = newname;
}

//Graph class
Graph::Graph(){
	path = "";
	first = "";
	last = "";
	current = NULL;
	found = false;
}

void Graph::setStartEnd(string starting, string ending){ // graph needs to know these
	first = starting;
	last = ending;
}

string Graph::getPath(){ //returns path
	return path;
}

bool Graph::getFound(){ //returns if path was found
	return found;
}
// Creates the graph while also finding the path
// Used a vector for instead of a dynamic array, I understand the concept but had some trouble
// so switch over to using the vector; If I used a dynamic array I know I would need to delete at the end
void Graph::createGraph(string configureContents, int count){
    string buf = "";  // line stream
    Node* temp = NULL;	
    int number = 0;
    Node* current = NULL;
    vector<Node*> nodeVec; //store all the pointers in the graph in vec for easy access;
    int items = 0;
    string node = "";
    stringstream ss(configureContents); // Insert the string from the file into a stream
    while(ss){ // while the stream is not empty
    	getline(ss, buf);
    	stringstream buffer(buf); // create a buffer for the line
    	string center; 
    	int cell = 0;
    	buffer >> center; // center node or first on line
    	if(!checkIfMade(nodeVec, center, count)){ // check if the node already exist in our vector
    		current = makeThisNode(center); // if not, make a node 
    		nodeVec.push_back(current); // store into the vector
    		items++;
    	}
    	else{
    		current = findThisNode(nodeVec, center, count); // the center node has already been made; so find it
    	}
    	while(cell < 4){ // take in the adition string on the line the links to other nodes
    		buffer >> node;
    		if(node != "*"){ //check for it being null
    			if(!(checkIfMade(nodeVec, node, count))){ // check to see if this node exist in the array; if not make it
    				temp = makeThisNode(node);
    				nodeVec.push_back(temp);
    			}
    			else{
    				temp = findThisNode(nodeVec, node, count); // if it does find this node
    			}
    			current->attachNewNode(temp, cell); // attach this node to the current/center node in the direction
    		}
    		cell++; // direction 0 - 1 - 2 - 3
    	}
    }
    //our graph is created, time to find the path
    findPath(nodeVec, count);
}
// make the node that will be attached while also stored in node array
Node* Graph::makeThisNode(string data){ // makes a node and returns it
	Node* p = new Node(data);
	return p;

}
//check for the node in the node vector
bool Graph::checkIfMade(vector<Node*>& vec, string data, int nodeCount){
	for (vector<Node*>::iterator it = vec.begin(); it < vec.end(); ++it)
	{
		if ((*it)->getNodeName() == data)
		{
			return true;
		}
	}
	return false;
}
// find this node in the node array and return its the pointer that points to it
Node* Graph::findThisNode(vector<Node*>& vec, string data, int nodeCount){
	for (vector<Node*>::iterator it = vec.begin(); it < vec.end(); ++it)
	{
		if (data.find((*it)->getNodeName()) != std::string::npos)
		{
			return (*it);
		}
	}
}
// finds the path between two nodes
void Graph::findPath(vector<Node*>& nodeVec, int count){
	string starter = this->first;
	Node* startNode = findThisNode(nodeVec, starter, 1);// starting node for search it will exist so find it
	bool foundit = false;
	Node* prev = NULL; // need a prev node for backtrack
	recursiveCall(foundit, startNode, prev); // foundit passed by reference for recursion
	if (!path.empty()) // something is in path it was found
	{
		found = true;
	}
}
void Graph::recursiveCall(bool &foundit, Node* current, Node* prev){
	int count = 0; // needed for to cycle through the node directions
	bool allvisited = false;
	//base case
	if (this->last.find(current->getNodeName()) != std::string::npos) // check if this is the node we are looking for
	{
		path += current->getNodeName() + " ";
		foundit = true;
		return;
	}

	bool visited[4] = {false, false, false, false}; // all to false
	while(!allvisited && !foundit){ // while we haven't visited all the nodes and we havent found it
		Node* next = current->getAttachedNodes(count); // get the node for the direction we first look
		if(prev == NULL){ // for the center node, it's prev will always be NULL
			goto skip;
		}
		if(next == NULL){ // deals with direction we can go
			goto skip;
		}
		if (prev->getNodeName().find(next->getNodeName()) != std::string::npos) // check if the node we are looking at is the direction we came from
		{
			visited[count] = true;
			count++;
			goto finish;
		}
skip:	if(next == NULL) // there isn't a node in this direction
		{
			visited[count] = true;
			count++;
			goto finish;
		}
		else{
			recursiveCall(foundit, next, current); // next is the node in the direction we can go; current will be prev of the next
			visited[count] = true;
			count++;
		} 
finish:	if ((visited[0] == true) && (visited[1] == true) && (visited[2] == true) && (visited[3] == true)) // all directions have been looked at
		{
			allvisited = true;
		}
	}
		if (foundit) //after the while loop has finished and if the end node was found then we need to add this value to path before backtracking
		{
			path += current->getNodeName() + " ";
		}

}


//system class
bool System::readfile(string file){
	ifstream configure; // file input stream
	int count = 0;
	file += ".txt";
	string temp = "";
	ifstream check(file.c_str());
	if(!check.is_open()){ // check to see if his file exist
		return false;
	}
	else{
		check.close();
		configure.open(file.c_str());
		while(!configure.eof()){
			getline(configure, temp); 
			if(!(temp.length() == 1)){
				if(count == 0){ // how many nodes will be in the graph
					nodeCount = atoi(temp.c_str()); // string to int
					count++;
					continue;
				}
				else if(count == 1){ // start node
					start = temp;
					count++;
					continue;
				}
				else if(count == 2){ // end node
					end = temp;
					count++;
					continue;
				}
				else { // rest of node lines
					contents += temp + '\n';
				}
			}
		}
	}
	configure.close();
	return true;
}

void System::passStartEnd(){
	myGraph.setStartEnd(start, end);
}

void System::makeGraph(){
	myGraph.createGraph(contents, nodeCount);
}

string System::getContents(){
	return contents;
}

string System::getStart(){
	return start;
}

string System::getEnd(){
	return end;
}

string System::getPath(){
	return myGraph.getPath();
}

bool System::checkIfFound(){
	return myGraph.getFound();
}

System::System(){
	contents = "";
	start = "";
	end = "";
	myGraph = Graph();
}

Interface::Interface(){
	syst = System();
}

bool Interface::displayInterface(){
	string input = "";
		cout << "Enter the name of the Maze configuration file: ";
		cin >> input;
		if (input == "Quit")
		{
			return false;
		}
		if(syst.readfile(input) == false){
			cout << "This file name doesnt exist" << endl;
			return true;
		}
		else{
			cout << "The Start node is: " << syst.getStart() << endl;
			cout << "The Destination Node is: " << syst.getEnd() << endl;
			cout << "Find a path from the Start to Destination node...." << endl;
			syst.passStartEnd();
			syst.makeGraph();
			string reverseIt = syst.getPath();
			string newPath = "";
			string temp = "";
			stringstream rev(reverseIt);
			while(rev >> temp){
				temp += " ";
				newPath.insert(0, temp);
			}
			if(syst.checkIfFound()) {
				cout << "Congratulations: found a path successfully." << endl;
				cout << "The path is: " << newPath << endl << endl;
			}
			else{
				cout << "Unsuccessful: No path can be found." << endl << endl;
			}	
		}
	return true;
}

