#include "MsgTimer.h"
#include <chrono>

// PRE: expects an integer, t , of the number of <seconds?> to save the
//      finish_time from the current time. the uint32_t of the nodeID we
//      expect a response from and the IP we expect the response to come
//      from.
//POST: stores the time_point finish_time which is t <seconds?> from now.
//      sets the NodeID to the node who we expect a response from, and the
//      IP we expect the request from.
MsgTimer::MsgTimer(int duration, uint32_t id, uint32_t ip) {
  dur temp(duration);
  this->wait_for = temp;
  tp now = std::chrono::system_clock::now();
  this->finish_time = this->wait_for + now;
  nodeID = id;
  nodeIP = ip;
}

// PRE: expects this object to be defined
//POST: returns true iff the current time is less than the finish_time
bool MsgTimer::timedOut() {
  bool isTimedOut;
  tp now = std::chrono::system_clock::now();
  if (now < finish_time) {
    isTimedOut = false;
  }
  else {
    isTimedOut = true;
  }
  return isTimedOut;
}

uint32_t MsgTimer::getNodeID() {
  return this->nodeID;
}

uint32_t MsgTimer::getNodeIP() {
  return this->nodeIP;
}

void MsgTimer::resetTimer() {
  tp now = std::chrono::system_clock::now();
  this->finish_time = now + this->wait_for;
}
