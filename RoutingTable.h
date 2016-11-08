//Justin Giguere
//Routing Table Header

#ifndef INCLUDE_RoutingTable
#define INCLUDE_RoutingTable

#include "KBucket.h"
#include "constants.h"
#include <stdint.h>
#include <fstream>
#include <mutex>

#define MAX_CHAR 80

class RoutingTable {

//CLASS INVARIENT:
// - For each bucket i, there is no more than K nodes in the bucket where each
//   node is no more than XOR distance from myID than 2^i -> 2^(i+1)

 private:
  
  uint32_t myId;            //our ID
  KBucket table[NUMBITS];   //the array that holds our buckets
	std::ofstream logOut;     //the log file for our KBuckets
  char hostName[MAX_CHAR];  //the hostname to write to the log file

  //Pre: id1 and id2 are two identifiers
  //Post: RV = id1 XOR id2
  uint32_t findDist(uint32_t id1, uint32_t id2) const ;
	
  //Pre: id and address represents a new node not seen before
  //Post: RV = Triple object representing this new node
  Triple* createTriple(uint32_t id, uint32_t address);

  //Pre: id is a valid id
  //Post: RV = true if id is in the table, otherwise false
  bool isNodeInTable(uint32_t id);

  //Pre: node is a valid node object not in the routing table (id)
  //     address correlates to where node is from
  //Post: RV = true if node was added to the table
  //           false otherwise
  bool addNode(uint32_t node, uint32_t address);
	
	// add = false -> deleting node
	// add = true -> adding node
	void log (int &i , Triple curNode, bool add = false) ;
	
	void log(int &i, uint32_t nodeID, bool add= false);
	
	void logError(string msg);

 public:
  
  //Pre: id refers to a valid node object
  //Post: table is an array of K kBuckets, that are each empty
  RoutingTable(uint32_t id);

  //Pre: The RoutingTable exists
  //Post: RV = true if no Triples exists in the kBuckets
  //      False otherwise
  bool isEmpty();

  //Pre: The Routing Table exists
  //Post: RV = true if the Routing Table is full, false otherwise
  bool isFull();

  //Pre: id is some node that belongs to a KBucket k
  //Post: RV = a copy of the oldest node in k
  Triple getOldestNode(uint32_t id);
  
  //Pre: target is some id, closeNodes is an array Triples with node = -1
  //Post: closeNodes = array of the K closest nodes to the target
  //      Less than K are returned iff less than K nodes are in the table
  //      The list is ordered by distance, that is, the closest node is
  //      at the head
  //      RV = number nodes in closeNodes
  int getKClosestNodes(uint32_t target, Triple* closeNodes);

  //Pre: nodeID exists in the table, and is known to be dead
  //Post: Removes the respected Triple from the table
  void deleteNode(uint32_t nodeID);

  //Pre: nodeID is a valid id, address is where nodeID is from
  //Post: myNode is placed at the tail of its respected kBucket
  //      the other triples are left shifted as needed
  //      RV = true if success, false otherwise
  bool updateTable(uint32_t nodeID, uint32_t address);

  //Pre: N/A
  //Post: Prints the contents of the Routing Table
  void printTable();
	
  //Pre: 0 <= index < NUMBITS
  //Post: RV = table[index]
  KBucket& operator [] (int index);
	
	//Pre: id is some valid node or key
	//Post: RV = the nth kBucket such that
	//      d = findDist(id) where 2^n <= d < 2^n+1
	int findKBucket(uint32_t id) const ;

	
	
};

#endif
