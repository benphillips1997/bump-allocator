#include "bump_allocator.hpp"
#include "simpletest/simpletest.h"

using namespace std;

char const *groups[] = {
    "simple",
	"struct",
	"exceedMemory",
	"failThenPass",
	"padding",
	"deallocReset",
	"deallocReset2",
	"deallocAllocUp",
	"onlyBumpUp",
	"onlyBumpUp2",
	"assignValues",
	"reassignValues",
};

// check that nullptr is returned when memory is full
DEFINE_TEST_G(SimpleTest, simple)
{
    bump_allocator bump(20);

	// should fill memory
    int *x = bump.alloc<int>(5);
	TEST_MESSAGE(x != nullptr, "Should have successfully allocated");

	// should fail and return nullptr
	char *c = bump.alloc<char>(1);
	TEST_MESSAGE(c == nullptr, "Should have failed to allocate");
}

// check that a struct is padded correctly
DEFINE_TEST_G(StructTest, struct)
{
	bump_allocator bump(32);

	struct my_struct {
		char c;
		int x;
		int y;
		double d;
	};

	char *ch = bump.alloc<char>(5);
	TEST_MESSAGE(ch != nullptr, "Should have successfully allocated");

	my_struct *myStruct = bump.alloc<my_struct>(1);
	TEST_MESSAGE(myStruct != nullptr, "Should have successfully allocated");

	char *s = bump.alloc<char>(1);
	TEST_MESSAGE(s == nullptr, "Should have failed to allocate");
}

// check that exceeding the memory limit will fail
DEFINE_TEST_G(ExceedMemoryTest, exceedMemory)
{
	bump_allocator bump(sizeof(int) * 100);

	int *x = bump.alloc<int>(101);
	TEST_MESSAGE(x == nullptr, "Should have failed to allocate");
}

// checks that after a failed allocation, it will not affect following allocations
DEFINE_TEST_G(FailThenPassTest, failThenPass)
{
	bump_allocator bump(sizeof(double) * 10);

	double *d = bump.alloc<double>(11);
	TEST_MESSAGE(d == nullptr, "Should have failed to allocate");

	double *e = bump.alloc<double>(10);
	TEST_MESSAGE(e != nullptr, "Should have successfully allocated");
}

// check different types all pad correctly
DEFINE_TEST_G(PaddingTest, padding)
{
	bump_allocator bump(48);

	int *i = bump.alloc<int>(1); // 4 bytes
	TEST_MESSAGE(i != nullptr, "Should have successfully allocated");

	double *d = bump.alloc<double>(1); // with padding total to 16 bytes
	TEST_MESSAGE(d != nullptr, "Should have successfully allocated");

	char *c = bump.alloc<char>(1); // total to 17 bytes
	TEST_MESSAGE(c != nullptr, "Should have successfully allocated");

	struct my_struct {
		char c;
		int x;
		int y;
		int z;
	};
	my_struct *myStruct = bump.alloc<my_struct>(1); // with padding total to 36 bytes
	TEST_MESSAGE(myStruct != nullptr, "Should have successfully allocated");

	double *e = bump.alloc<double>(1); // with padding should total 48 bytes;
	TEST_MESSAGE(e != nullptr, "Should have successfully allocated");

	char *s = bump.alloc<char>(1); // should go over the memory limit if padding was correct
	TEST_MESSAGE(s == nullptr, "Should have failed to allocate");
}

// check allocator resets when allocation count is 0
DEFINE_TEST_G(DeallocResetTest, deallocReset)
{
	bump_allocator bump(sizeof(int) * 4);

	int *x = bump.alloc<int>(4);
	TEST_MESSAGE(x != nullptr, "Should have successfully allocated");

	bump.dealloc(x);

	int *y = bump.alloc<int>(4);
	TEST_MESSAGE(y != nullptr, "Should have successfully allocated");
}

// another tests to check allocator resets when allocation count reaches 0
DEFINE_TEST_G(DeallocResetTest2, deallocReset2)
{
	bump_allocator bump((sizeof(int) * 10) + (sizeof(double) * 10) + (sizeof(string) * 10));

	// allocate ints
	int *x = bump.alloc<int>(4);
	TEST_MESSAGE(x != nullptr, "Should have successfully allocated");

	int *y = bump.alloc<int>(1);
	TEST_MESSAGE(y != nullptr, "Should have successfully allocated");

	int *z = bump.alloc<int>(3);
	TEST_MESSAGE(z != nullptr, "Should have successfully allocated");

	// allocate doubles
	double *d = bump.alloc<double>(10);
	TEST_MESSAGE(d != nullptr, "Should have successfully allocated");

	// allocate string
	string *s = bump.alloc<string>(5);
	TEST_MESSAGE(s != nullptr, "Should have successfully allocated");

	string *t = bump.alloc<string>(5);
	TEST_MESSAGE(t != nullptr, "Should have successfully allocated");

	// deallocate all pointers
	bump.dealloc(x);
	bump.dealloc(y);
	bump.dealloc(z);

	bump.dealloc(d);

	bump.dealloc(s);
	bump.dealloc(t);

	// allocate ints
	int *i = bump.alloc<int>(10);
	TEST_MESSAGE(i != nullptr, "Should have successfully allocated");

	// allocate doubles
	double *e = bump.alloc<double>(3);
	TEST_MESSAGE(e != nullptr, "Should have successfully allocated");
	
	double *f = bump.alloc<double>(7);
	TEST_MESSAGE(f != nullptr, "Should have successfully allocated");

	// allocate strings
	string *ss = bump.alloc<string>(2);
	TEST_MESSAGE(ss != nullptr, "Should have successfully allocated");

	string *st = bump.alloc<string>(6);
	TEST_MESSAGE(st != nullptr, "Should have successfully allocated");

	string *str = bump.alloc<string>(2);
	TEST_MESSAGE(str != nullptr, "Should have successfully allocated");
}

// check that making two allocations then deallocating and allocing until memory is full only moves the allocator up and does not reset it
DEFINE_TEST_G(DeallocAllocUp, deallocAllocUp)
{
	bump_allocator bump(32);

	int *x = bump.alloc<int>(2);
	TEST_MESSAGE(x != nullptr, "Should have successfully allocated");

	double *d = bump.alloc<double>(1);
	TEST_MESSAGE(d != nullptr, "Should have successfully allocated");

	bump.dealloc(d);

	int *y = bump.alloc<int>(2);
	TEST_MESSAGE(y != nullptr, "Should have successfully allocated");

	bump.dealloc(y);

	double *e = bump.alloc<double>(1);
	TEST_MESSAGE(e != nullptr, "Should have successfully allocated");

	bump.dealloc(e);

	int *z = bump.alloc<int>(1);
	TEST_MESSAGE(z == nullptr, "Should have failed to allocate");
}

// after deallocating an allocation (but not all of them), checks that allocator does not allow more allocations before the allocation count reaches 0
DEFINE_TEST_G(OnlyBumpUpTest, onlyBumpUp)
{
	bump_allocator bump(sizeof(int) * 8);

	int *x = bump.alloc<int>(1);
	TEST_MESSAGE(x != nullptr, "Should have successfully allocated");

	int *y = bump.alloc<int>(5);
	TEST_MESSAGE(y != nullptr, "Should have successfully allocated");

	int *z = bump.alloc<int>(2);
	TEST_MESSAGE(z != nullptr, "Should have successfully allocated");

	bump.dealloc(x);

	int *i = bump.alloc<int>(1);
	TEST_MESSAGE(i == nullptr, "Should have failed to allocate");
}

// same as above test but deallocate last two pointers instead of first one
DEFINE_TEST_G(OnlyBumpUpTest2, onlyBumpUp2)
{
	bump_allocator bump(sizeof(int) * 8);

	int *x = bump.alloc<int>(1);
	TEST_MESSAGE(x != nullptr, "Should have successfully allocated");

	int *y = bump.alloc<int>(5);
	TEST_MESSAGE(y != nullptr, "Should have successfully allocated");

	int *z = bump.alloc<int>(2);
	TEST_MESSAGE(z != nullptr, "Should have successfully allocated");

	bump.dealloc(y);
	bump.dealloc(z);

	int *i = bump.alloc<int>(1);
	TEST_MESSAGE(i == nullptr, "Should have failed to allocate");
}

// check that allocating and assigning values works correctly
DEFINE_TEST_G(AssignValuesTest, assignValues)
{
	int charCount = 10;
	char letters[] = {"abcdefghij"};

	bump_allocator bump(sizeof(char) * charCount);
	
	char *s = bump.alloc<char>(charCount);
	TEST_MESSAGE(s != nullptr, "Should have successfully allocated");

	for (int i = 0; i < charCount; i++) {
		*(s + i) = letters[i];
	}

	for (int i = 0; i < charCount; i++) {
		TEST(s[i] == 'a' + i);
	}
}

// after allocating and assigning values check that deallocating and then allocating and assigning different values works correctly
DEFINE_TEST_G(ReassignValuesTest, reassignValues)
{
	int charCount = 10;
	char letters[] = {"abcdefghij"};
	int intCount = 10;
	int numbers[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

	bump_allocator bump(sizeof(int) * intCount);
	
	char *s = bump.alloc<char>(charCount);
	TEST_MESSAGE(s != nullptr, "Should have successfully allocated");

	for (int i = 0; i < charCount; i++) {
		*(s + i) = letters[i];
	}

	for (int i = 0; i < charCount; i++) {
		TEST_MESSAGE(s[i] == 'a' + i, "Values should be equal");
	}

	bump.dealloc(s);

	int *x = bump.alloc<int>(intCount);
	TEST_MESSAGE(x != nullptr, "Should have successfully allocated");

	for (int i = 0; i < intCount; i++) {
		*(x + i) = i + 1;
	}

	for (int i = 0; i < intCount; i++) {
		TEST_MESSAGE(x[i] == numbers[i], "Values should be equal");
	}
}


int main()
{	
	bool pass = true;

	for (auto group : groups) {
		pass &= TestFixture::ExecuteTestGroup(group, TestFixture::Verbose);
	}

	return pass ? 0 : 1;
}