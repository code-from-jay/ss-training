#include <iostream>
#include <cassert>

struct A {};
struct B { B& operator=(const B&) = delete; };
struct C { C(const C& other) {} };
struct D { D(const D& other) = delete; };
struct E { E(E&& other) {} };
struct F { F& operator=(const F& f) { return *this; } };
struct G { G(const G& other) = default; };

int main()
{
	// int, as a builtin literal, is CopyAssignable
	assert(std::is_copy_assignable<int>::value);
	// int[] is an aggregate type, and as such, is not CopyAssignable
	assert(!std::is_copy_assignable<int[]>::value);
	// int& is just an alias to an int, which makes it CopyAssignable
	assert(std::is_copy_assignable<int&>::value);
	// Since A is a trivial struct, it's has implicit CopyAssignment operators added
	assert(std::is_copy_assignable<A>::value);
	// B explicitly deletes the copy assignment operator, making it not CopyAssignable
	assert(!std::is_copy_assignable<B>::value);
	// Since C is CopyConstructable, a CopyAssignment operator must exist, making it CopyAssignable
	assert(std::is_copy_assignable<C>::value);
	// D deleting it's CopyConstructor doesn't stop the compiler from implicitly adding a CopyAssignment operator
	assert(std::is_copy_assignable<D>::value);
	// Since E defines a MoveConstructor, the compiler can't implicitly add a CopyAssignment operator, thus making it not CopyAssignable
	assert(!std::is_copy_assignable<E>::value);
	// F explicitly defines a CopyAssignment operator, making it CopyAssignable
	assert(std::is_copy_assignable<F>::value);
	// G is CopyAssignable for the same reason as C
	assert(std::is_copy_assignable<G>::value);
}
