#include <iostream>
#include <assert.h>

using namespace std;


class bump_allocator
{
private:
    char *memory;
    size_t allocator_size;
    size_t offset;
    int count;
public:
    bump_allocator() = delete;

    bump_allocator(size_t s)
    {
        allocator_size = s;
        memory = new char[allocator_size];
        offset = 0;
        count = 0;
    }

    template <class T>
    T* alloc(size_t args)
    {
        // cout << "Offset before: " << offset << endl;

        size_t size = args * sizeof(T);

        // check alignment
        size_t align = 0;
        if (!(offset % alignof(T) == 0)) {
            // cout << "Not aligned" << endl;
            align = (alignof(T) - (offset % alignof(T)));
        }

        // cout << "Align: " << align << endl;

        // if there is not enough space left then return nullptr
        if ((offset + align + size) > allocator_size) {
            cout << "Not enough memory" << endl;
            return nullptr;
        }

        // make new pointer at the next space and return
        T *mem = (T*)(memory + offset + align);
        offset += align + size; // move offset up
        count++;
        // cout << "Offset after: " << offset << endl;
        return mem;
    }

    template <class T>
    void dealloc(T *ptr)
    {
        assert(count > 0);

        // cout << "Deallocate called" << endl;
        count--;
        
        // if all allocations have been deallocated then reset the heap to the start
        if (count == 0) {
            offset = 0;
            cout << "Allocator reset" << endl;
        }
    }
};