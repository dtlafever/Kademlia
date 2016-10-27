#include "MsgTimer.h"

// PRE: expects an integer, t , of the number of <seconds?> to save the
//      finish_time from the current time. the uint32_t of the nodeID we
//      expect a response from and the IP we expect the response to come
//      from.
//POST: stores the time_point finish_time which is t <seconds?> from now.
//      sets the NodeID to the node who we expect a response from, and the
//      IP we expect the request from.
MsgTimer::MsgTimer(int duration, uint32_t id, uint32_t ip) {
  timer::duration<int> temp(duration);
  wait_for = temp;
  timer_clock::time_point now = timer_clock::now();
  finish_time = wait_for + now;
}

// PRE: expects this object to be defined
//POST: returns true iff the current time is less than the finish_time
bool MsgTimer::timedOut() {
  bool isTimedOut;
  timer_clock::time_point now = timer_clock::now();
  if (now < finish_time) {
    isTimedOut = false;
  else {
    isTimedOut = true;
  }
  return isTimedOut;
}

uint32_t MsgTimer::getNodeID() {
  return nodeID;
}

uint32_t MsgTimer::getNodeIP() {
  return nodeIP;
}

void MsgTimer::resetTimer() {
  timer_clock::time_point now = timer_clock::now();
  finish_time = now + wait_for;
}
