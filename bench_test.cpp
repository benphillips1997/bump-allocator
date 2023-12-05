#include <iostream>
#include "bump_allocator.hpp"
#include "bump_down_allocator.hpp"
#include "benchmark.hpp"


void bump_up_test(int iterations)
{
    for (int i = 0; i < iterations; i++) {
        bump_allocator bump(sizeof(double) * 20);

        char *c = bump.alloc<char>(3);
        int *x = bump.alloc<int>(5);

        bump.dealloc(c);
        bump.dealloc(x);

        char *s = bump.alloc<char>(5);
        double *d = bump.alloc<double>(12);

        bump.dealloc(s);
        bump.dealloc(d);
    }

    struct my_struct {
        char ch;
        int i;
        int j;
        int k;
        double db;
    };

    for (int i = 0; i < iterations; i++) {
        bump_allocator bump(sizeof(my_struct) * 60);

        char *s = bump.alloc<char>(7);
        my_struct *myStruct = bump.alloc<my_struct>(50);
    }
}

void bump_down_test(int iterations)
{
    for (int i = 0; i < iterations; i++) {
        bump_down_allocator bump(sizeof(double) * 20);

        char *c = bump.alloc<char>(3);
        int *x = bump.alloc<int>(5);

        bump.dealloc(c);
        bump.dealloc(x);

        char *s = bump.alloc<char>(5);
        double *d = bump.alloc<double>(12);

        bump.dealloc(s);
        bump.dealloc(d);
    }

    struct my_struct {
        char ch;
        int i;
        int j;
        int k;
        double db;
    };

    for (int i = 0; i < iterations; i++) {
        bump_down_allocator bump(sizeof(my_struct) * 60);

        char *s = bump.alloc<char>(7);
        my_struct *myStruct = bump.alloc<my_struct>(50);
    }
}

int main()
{
    benchmark bench;

    int iterations = 100000000;
    cout << "Number of iterations: " << iterations << endl;
    cout << "Bump up: ";
    bench.time_function(bump_up_test, iterations);
    cout << "Bump down: ";
    bench.time_function(bump_down_test, iterations);

    return 0;
}