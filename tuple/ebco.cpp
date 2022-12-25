#include <iostream>
#include <vector>
#include <string>
#include <type_traits>
#include "tuple.h"

using namespace std;

template <class T>
struct A
{
};

struct B
{
    A<integral_constant<int, 1>> a0;
    A<integral_constant<int, 2>> a1;
};

struct C : A<integral_constant<int, 1>>, A<integral_constant<int, 2>>
{
};

// struct D : A<integral_constant<int, 1>>, C
// {
// }; // warning: direct base 'A<integral_constant<int, 1>>' is inaccessible due to ambiguity

int main()
{
    // EBCO
    cout << sizeof(A<integral_constant<int, 1>>) << endl; // 1
    cout << sizeof(B) << endl;                            // 2
    cout << sizeof(C) << endl;                            // 1
    // cout << sizeof(D) << endl;                            // 2 : comment out this case to remove the warning.
}