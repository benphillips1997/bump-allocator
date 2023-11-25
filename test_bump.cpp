#include "bump_allocator.hpp"

int main()
{
    int bump_allocator_size = 40;
    bump_allocator bump(bump_allocator_size);
    cout << "bump allocator size is " << bump_allocator_size << " bytes" << endl;

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
    
    return 0;
}