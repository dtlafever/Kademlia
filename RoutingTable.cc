#include "RoutingTable.h"
#include <stdint.h>
#include <iostream>
#include <stdio.h>
#include <cmath>
#include <fstream>
#include <unistd.h>

using namespace std;

#define LOG_FILE "Logs/bucket.log"

//Tags: Creatation, Check and Find, Modifying, Getting Stuff, Print Table

//-----------------------Creation---------------------------------

//Pre: id is the nodeID who owns this RoutingTable
//Post: table is an array of K kBuckets, that are each empty
RoutingTable::RoutingTable(uint32_t id) {
	gethostname(hostName, MAX_CHAR);
  myId = id;
	logOut.open(LOG_FILE, std::ofstream::out | std::ofstream::app);
	logOut << hostName << ": --------START NETWORK--------" << std::endl;
}

//Pre: id and address represents a new node not seen before
//Post: RV = Triple object representing this new node
Triple* RoutingTable::createTriple(uint32_t id, uint32_t address) {
  Triple* newTriple = new Triple;
  newTriple->address = address;
  newTriple->node = id;
  newTriple->port = MAINPORT;
  return (newTriple);
}

//----------------------Check and Find----------------------------------

//Pre: id is a valid id
//Post: RV = true if id is in the table, otherwise false
bool RoutingTable::isNodeInTable(uint32_t id) {
  int nthBucket = findKBucket(id);
	bool inTable = false;
	if(nthBucket != -1)
  	 inTable = table[nthBucket].isNodeInBucket(id);
  return (inTable);
}

//Pre: The RoutingTable exists
//Post: RV = true if no Triples exists in the kBuckets
//      False otherwise
bool RoutingTable::isEmpty() {
  bool empty = true;
  int index = 0;
  while (empty) {
    empty = table[index].isEmpty();
    index++;
  }
  return (empty);
}

//Pre: The Routing Table exists
//Post: RV = true if the Routing Table is full, false otherwise
bool RoutingTable::isFull() {
  bool full = true;
  int index = 0;
  while (full) {
    KBucket curBucket = table[index];
    if (curBucket.getNumTriples() != K) {
      full = false;
    }
  }
  return (full);
}

//Pre: id is some valid node or key
//Post: RV = the nth kBucket such that d = findDist(id) where 2^n <= d < 2^n+1
int RoutingTable::findKBucket(uint32_t id) const {
	if(id > pow(2, NUMBITS) - 1)
		return -1;
  uint32_t myDist = findDist(myId, id);
  uint32_t tempt = myDist;
  int twoPower = 0;
  while (tempt != 1 && tempt != 0 ) {
    tempt = tempt / 2;
    twoPower++;
  }
  //printf("in findKBucket, id = %u, dist = %u nthBucket = %d \n",
  //	 id, myDist, twoPower);
  return (twoPower);
}

//Pre: id1 and id2 are two identifiers
//Post: RV = id1 XOR id2
uint32_t RoutingTable::findDist(uint32_t id1, uint32_t id2) const{
  return (id1 ^ id2);
}

//Pre: 0 <= index < NUMBITS
//Post: RV =  a deep copy of table[index]
KBucket& RoutingTable::operator [] (int index) {
  return table[index];
}

//-----------------------Modifying Table-----------------------------------

//Pre: node is a valid node object not in the routing table (id)
//     address correlates to where node is from
//Post: RV = true if node was added to the table
//           false otherwise
bool RoutingTable::addNode(uint32_t node, uint32_t address) {
  int nthBucket = findKBucket(node);
	bool added = false;

	if(nthBucket != -1)
	{
		KBucket* currBucket = &(table[nthBucket]);
		Triple* newTriple = createTriple(node, address);

		if(currBucket->getNumTriples() < K){
			//ASSERT: the bucket is not full, add to it
			log(nthBucket, *newTriple, true);
			currBucket->addNode(newTriple);
			added = true;
		}
	}
	else
	{
		string s = "Invalid Node ID, cannot be stored in ";
		char temp[50];
		sprintf(temp, "%u", NUMBITS);
		s+= string(temp) + " bits\n";
		logError(s);
	}

  return (added);
}

//Pre: nodeID exists in the table, and is known to be dead
//Post: Removes the respected Triple from the table
void RoutingTable::deleteNode(uint32_t nodeID) {
  int nthBucket = findKBucket(nodeID);
	if(nthBucket != -1)
	{
		log(nthBucket, nodeID);
		table[nthBucket].deleteNode(nodeID);
	}
}

//Pre: nodeID is a valid id, address is where nodeID is from
//Post: myNode is placed at the tail of its respected kBucket
//      the other triples are left shifted as needed
//      RV = true if success, false otherwise
bool RoutingTable::updateTable(uint32_t nodeID, uint32_t address) {
  int nthBucket = findKBucket(nodeID);
	bool success = false;

	if (nthBucket != -1)
	{
		bool inTable = isNodeInTable(nodeID);
		if (inTable) {
			table[nthBucket].adjustNode(nodeID);
			success = true;
		}
		else {
				success = addNode(nodeID, address);
		}
	}
  return (success);
}

//----------------------Getting Stuff--------------------------------

//Pre: target is some id, closeNodes is an array Triples with node = -1
//Post: closeNodes = array of the K closest nodes to the target
//      Less than K are returned iff less than K nodes are in the table
//      The list is ordered by distance, that is, the closest node is
//      at the head
//      RV = number nodes in closeNodes
int RoutingTable::getKClosestNodes(uint32_t target, Triple* closeNodes) {
  int size = 0;
  for (int index = 0; (index < NUMBITS); index++) {
    table[index].getKClosestNodes(target, closeNodes, size);
  }
  return (size);
}

//Pre: id is some node that belongs to a KBucket k
//Post: RV = a copy of the oldest node in k
Triple RoutingTable::getOldestNode(uint32_t id) {
  int nthBucket = findKBucket(id);
	if(nthBucket != -1)
	 {
		Triple* leastRecentNode = table[nthBucket].getHead();
		Triple copy;
		copy.node = leastRecentNode->node;
		copy.address = leastRecentNode->address;
		copy.port = leastRecentNode->port;
		updateTable(leastRecentNode->node, leastRecentNode->address);
		return (copy);
	 }
	else return Triple();
}

//----------------------Print Table------------------------------

//Pre: N/A
//Post: Prints the contents of the Routing Table
void RoutingTable::printTable() {
  printf("Routing Table \n \n");
  for (int index = 0; (index < NUMBITS); index++) {
    printf("nthBucket: %d \n", index);
    table[index].printBucket();
  }
}

void RoutingTable::log(int &i, Triple curNode, bool add)
{
	gethostname(hostName, MAX_CHAR);
	if(add)
	{
		logOut<< hostName << ": Inserted Node - ";
	}
	else
	{
		logOut<< hostName << ": Deleted Node - ";
	}
	
	logOut <<"port :"<< curNode.port <<" \t node : " << 
	curNode.node << " \t address: "<< curNode.address <<endl;
}

void RoutingTable::log(int &i, uint32_t nodeID, bool add)
{
	gethostname(hostName, MAX_CHAR);
	if(add)
	{
		logOut<< hostName << ": Inserted Node - ";
	}
	else
	{
		logOut<< hostName << ": Deleted Node - ";
	}
	
	logOut << "port : ---- \t node : " << nodeID << " \t address: ----"<<endl;
}

void RoutingTable::logError(string msg)
{
	gethostname(hostName, MAX_CHAR);
	logOut << hostName << ": ERROR :  "<< msg;
}


