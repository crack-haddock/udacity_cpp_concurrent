#ifndef TRAFFICLIGHT_H
#define TRAFFICLIGHT_H

#include <mutex>
#include <deque>
#include <condition_variable>
#include "TrafficObject.h"

// forward declarations to avoid include cycle
class Vehicle;

enum TrafficLightPhase {
    red, green
};

// FP.3 [done] Define a class „MessageQueue“ which has the public methods send and receive. 
// [done] Send should take an rvalue reference of type TrafficLightPhase whereas receive should return this type. 
// [done] Also, the class should define an std::dequeue called _queue, which stores objects of type TrafficLightPhase. 
// [done] Also, there should be an std::condition_variable as well as an std::mutex as private members. 

template <class T>
class MessageQueue {
public:
    void wait() {}
    T receive();
    void send(T &&msg);

private:
    std::deque<TrafficLightPhase> _queue;
    std::condition_variable _cnd;
    std::mutex _mut;
};

// FP.1 : 
// [done] Define a class „TrafficLight“ which is a child class of TrafficObject. 
// [done] The class shall have the public methods „void waitForGreen()“ and „void simulate()“ 
// [done] as well as „TrafficLightPhase getCurrentPhase()“, where TrafficLightPhase is an enum that can be either „red“ or „green“. 
// [done] Also, add the private method „void cycleThroughPhases()“. 
// [done] Furthermore, there shall be the private member _currentPhase which can take „red“ or „green“ as its value. 

class TrafficLight : public TrafficObject {
public:
    // constructor / desctructor
    TrafficLight();
    ~TrafficLight() {}

    // getters / setters
    TrafficLightPhase getCurrentPhase();

    // typical behaviour methods
    void waitForGreen();
    void simulate();
private:
    // typical behaviour methods
    void cycleThroughPhases();

    // FP.4b : 
    // [done] create a private member of type MessageQueue for messages of type TrafficLightPhase 
    // [done] and use it within the infinite loop to push each new TrafficLightPhase into it by calling send in conjunction with move semantics.
    std::condition_variable _condition;
    std::mutex _mutex;
    TrafficLightPhase _currentPhase;
    MessageQueue<TrafficLightPhase> _msgQ;
};

#endif