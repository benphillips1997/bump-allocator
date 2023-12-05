#pragma once

#include <iostream>
#include <assert.h>

using namespace std;


class bump_down_allocator
{
private:
    char *memory;
    char *end;
    size_t allocator_size;
    size_t offset;
    int count;
public:
    bump_down_allocator() = delete;

    bump_down_allocator(size_t s)
    {
        allocator_size = s;
        memory = new char[allocator_size];
        end = memory + allocator_size;
        offset = 0;
        count = 0;
    }

    template <class T>
    T* alloc(size_t args)
    {
        // cout << "Offset before: " << offset << endl;

        volatile size_t size = args * sizeof(T);

        // check alignment
        volatile size_t tAlign = alignof(T);
        volatile size_t align = (tAlign - (offset & (tAlign - 1))) & (tAlign - 1);

        // cout << "Align: " << align << endl;

        // increase offset
        offset += align + size;

        // if not enough memory return nullptr and revert offset
        if (offset > allocator_size) {
            offset -= align - size;
            return nullptr;
        }

        // make new pointer at the next space
        T *mem = (T*)(end - offset);
        count++;
        // cout << "Offset after: " << offset << endl;
        return mem;
    }

    template <class T>
    void dealloc(T *ptr)
    {
        //assert(count > 0);

        // cout << "Deallocate called" << endl;
        count--;
        
        // if all allocations have been deallocated then reset the heap to the start
        if (count == 0) {
            offset = 0;
            // cout << "Allocator reset" << endl;
        }
    }
};