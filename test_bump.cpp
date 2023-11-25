#include "bump_allocator.hpp"

int main()
{
    int bump_allocator_size = 40;
    bump_allocator bump(bump_allocator_size);
    cout << "bump allocator size is " << bump_allocator_size << " bytes" << endl;

    // allocate memory using bump allocator
    int cCount = 5;
    char *pChar = bump.alloc<char>(cCount);
    for (int i = 0; i < cCount; i++) {
        *(pChar + i) = 'a' + i;
        cout << *(pChar + i) << " ";
    }
    cout << endl;

    int iCount = 5;
    int *pInt = bump.alloc<int>(iCount);
    for (int i = 0; i < iCount; i++) {
        *(pInt + i) = i + 1;
        cout << *(pInt + i) << " ";
    }
    cout << endl;

    double *pDouble = bump.alloc<double>(1);
    *pDouble = 4.01574;
    cout << *pDouble << endl;


    
    // deallocate memory
    bump.dealloc(pChar);

    bump.dealloc(pInt);

    bump.dealloc(pDouble);


    // allocate memory
    int dCount = 4;
    double *d = bump.alloc<double>(dCount);
    for (int i = 0; i < dCount; i++) {
        *(d + i) = 3.1567 * (i + 1);
        cout << *(d + i) << " ";
    }
    cout << endl;

    cCount = 8;
    char *c = bump.alloc<char>(cCount);
    for (int i = 0; i < cCount; i++) {
        *(c + i) = 'z' - i;
        cout << *(c + i) << " ";
    }
    cout << endl;


    return 0;
}