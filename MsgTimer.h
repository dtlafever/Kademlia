#ifndef INCLUDE_MsgTimer
#define INCLUDE_MsgTimer

#include <stdint.h>
#include <unistd.h>
#include <chrono>
#define dur std::chrono::duration<int>
#define tp std::chrono::system_clock::time_point

class MsgTimer {

private:
  uint32_t nodeID, nodeIP;
  dur wait_for;
  tp finish_time;


public:
  // PRE: expects an integer, t , of the number of <seconds?> to save the
  //      finish_time from the current time. the uint32_t of the nodeID we
  //      expect a response from and the IP we expect the response to come
  //      from.
  //POST: stores the time_point finish_time which is t <seconds?> from now.
  //      sets the NodeID to the node who we expect a response from, and the
  //      IP we expect the request from.
  MsgTimer(int duration, uint32_t id, uint32_t ip);

  // PRE: expects this object to be defined
  //POST: returns true iff the current time is less than the finish_time
  bool timedOut();

  // PRE: expects this object to be defined
  //POST: returns the id of the timeout object as uint32_t
  uint32_t getNodeID();

  // PRE: expects this object to be defined
  //POST: returns the ip of the timeout object as uint32_t
  uint32_t getNodeIP();

  // PRE: expects this object to be defined
  //POST: resets the finish_time to the current time plus the stored duration.
  void resetTimer();
};

#endif
