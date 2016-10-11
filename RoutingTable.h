//Justin Giguere
//Routing Table Header

#ifndef INCLUDE_RoutingTable
#define INCLUDE_RoutingTable

#include "KBucket.h"
#include "constants.h"
#include <stdint.h>

class RoutingTable {
	
	//Pre: id refers to a valid node object
	//Post: meBuckets is an array of K kBuckets, that are each empty
	RoutingTable();
	
	//Pre: The respected node is going poof
	//Post: deletes each kBucket
	~RoutingTable();
 private:
	uint32_t myId;
	
	KBucket* meBuckets;
	
	//Pre: id1 and id2 are two identifiers
	//Post: RV = id1 XOR id2
	uint32_t findDist(uint32_t id1, uint32_t id2);
	
	
	//Pre: id is some valid node or key
	//Post: RV = the nth kBucket such that
	//      d = findDist(id) where 2^n <= d < 2^n+1
	int findKBucket(uint32_t id);
	
	//Pre: target is some id, closeNodes is an array of K -1's
	//Post: RV = array of the K closest nodes to the target
	//      Less than K are returned iff less than K nodes are in the table
	//      The list is ordered by distance, that is, the closest node is
	//      at the head
	void getNodes(uint32_t target, Triple* closeNodes);
	
	//Pre: node is a valid node object not in the routing table (id)
	//     address correlates to where node is from
	//Post: RV = true if node was added to the table
	//           false otherwise
	bool addNode(uint32_t node, uint32_t address);

	//Pre: id and address represents a new node not seen before
	//Post: RV = Triple object representing this new node
	Triple* createTriple(uint32_t id, uint32_t address);
	
	//Pre: myNode exists inside meBuckets
	//Post: myNode is placed at the tail of its respected kBucket
	//      the other triples are left shifted as needed
	void updateTable(Triple* myNode);
	
};

#endif
