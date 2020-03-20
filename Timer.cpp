#include <thread>
// #include <chrono>
#include <iostream>
// class Timer
// {
//     int duration;
//     int timePased;

//     public: Timer(int _duration)
//     {
//         duration = _duration;
//     }

    void doWork()
    {
        std::cout << "hello";
    }

    // public: void start()
    // {
        int main()
        {
        std::thread t(doWork);
        }
//     }

//     public: void stop()
//     {

//     }
//     public: void reset()
//     {

//     }
// };
