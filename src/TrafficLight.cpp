#include <iostream>
#include <random>
#include <functional>
#include "TrafficLight.h"

/* Implementation of class "MessageQueue" */
template <typename T>
T MessageQueue<T>::receive()
{
    // FP.5a : The method receive should use std::unique_lock<std::mutex> and _condition.wait() 
    // to wait for and receive new messages and pull them from the queue using move semantics. 
    // The received object should then be returned by the receive function. 
    std::unique_lock<std::mutex> mLock(_mut);
    _cnd.wait(mLock);

    auto msg = _queue.back();
    _queue.pop_back();

    return msg;
}

template <typename T>
void MessageQueue<T>::send(T &&msg)
{
    // FP.4a : The method send should use the mechanisms std::lock_guard<std::mutex> 
    // as well as _condition.notify_one() to add a new message to the queue and afterwards send a notification.
    std::lock_guard<std::mutex> mLock(_mut);
    _queue.emplace_back(msg);
    _cnd.notify_one();
}


/* Implementation of class "TrafficLight" */

TrafficLight::TrafficLight()
{
    _currentPhase = TrafficLightPhase::red;
}

void TrafficLight::waitForGreen()
{
    // FP.5b : add the implementation of the method waitForGreen, in which an infinite while-loop 
    // runs and repeatedly calls the receive function on the message queue. 
    // Once it receives TrafficLightPhase::green, the method returns.
    while (true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));

        if (_msgQ.receive() == (TrafficLightPhase)TrafficLightPhase::green)
            return;
    }
}

TrafficLightPhase TrafficLight::getCurrentPhase()
{
    return _currentPhase;
}

void TrafficLight::simulate()
{
    // [done] FP.2b : Finally, the private method „cycleThroughPhases“ should be started in a thread when the public method „simulate“ is called. To do this, use the thread queue in the base class.
    threads.emplace_back(std::thread(std::bind(&TrafficLight::cycleThroughPhases, this)));
}

// virtual function which is executed in a thread
void TrafficLight::cycleThroughPhases()
{
    // FP.2a : Implement the function with an infinite loop that measures the time between two loop cycles 
    // and toggles the current phase of the traffic light between red and green and sends an update method 
    // to the message queue using move semantics. The cycle duration should be a random value between 4 and 6 seconds. 
    // [has no purpose in my version] Also, the while-loop should use std::this_thread::sleep_for to wait 1ms between two cycles.
    while (true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        int rand = (random() % 3) + 4;
        //std::cout << "rand = " << rand << "\n";
        
        std::this_thread::sleep_for(std::chrono::milliseconds(1000 * rand));

        std::lock_guard<std::mutex> mLock(_mutex);
        _currentPhase = _currentPhase == TrafficLightPhase::red ? TrafficLightPhase::green : TrafficLightPhase::red;
        _msgQ.send(std::move(_currentPhase));
    };

    // note: I realise the instructions hint at doing it using something like the below method, but when I do all the lights 
    // just change at the same time and continue to alternate at the same time indefinitely.
    // I can see the rand num gen'd differs between threads, so I do not know what causes this

    /*
    std::random_device rd;
    std::mt19937 gen(rd());

    auto startTime = std::chrono::high_resolution_clock::now();

    while (true) {
        std::uniform_real_distribution<double> dist(4.0, 6.0);
        // Check elapsed time since the last toggle
        auto currentTime = std::chrono::high_resolution_clock::now();
        auto elapsedTime = std::chrono::duration_cast<std::chrono::duration<double>>(currentTime - startTime).count();

        if (elapsedTime >= dist(gen)) {
            //std::cout << "rand GEN = " << dist(gen) << "  elaps = " << elapsedTime << "\n";
            //std::unique_lock<std::mutex> mLock(_mutex);
            // Toggle the boolean variable
            _currentPhase = _currentPhase == TrafficLightPhase::red ? TrafficLightPhase::green : TrafficLightPhase::red;
            _msgQ.send(std::move(_currentPhase));
            //mLock.unlock();
            // Reset the start time
            startTime = std::chrono::high_resolution_clock::now();
        }

        // Continue with the loop
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    */
}
