#pragma once

#include <chrono>
#include <iostream>

using namespace std;
using chrono::high_resolution_clock;
using chrono::duration_cast;
using chrono::duration;
using chrono::milliseconds;


class benchmark
{
public:
    // times a function(void)
    template <class T>
    void time_function(T func)
    {
        // time passed function
        auto start = high_resolution_clock::now();
        
        func();

        auto stop = high_resolution_clock::now();

        duration<double, milli> time = stop - start;
        cout << time.count() << "ms" << endl;
    }

    // times a function with arguments
    template <class T, class... Args>
    void time_function(T func, Args... args)
    {
        // time passed function
        auto start = high_resolution_clock::now();

        // call funcion with multiple arguments
        forward<T>(func)(forward<Args>(args)...);

        auto stop = high_resolution_clock::now();

        duration<double, milli> time = stop - start;
        cout << time.count() << "ms" << endl;
    }
};