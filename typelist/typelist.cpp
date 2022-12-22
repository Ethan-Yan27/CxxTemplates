#include <iostream>
#include <type_traits>
#include "typelist.h"
using namespace std;

int main(){
    using empty_list = ValueList<int>;
    using list1 = ValueList<int, 0>;
    using list2 = ValueList<int, 0, 2, 4, 5, 8, 12>;
    {
        cout << "================================" << endl;
        cout << "IsEmpty Unit Tests" << endl;
        using empty_list = ValueList<int>;
        using list1 = ValueList<int, 0>;
        using list2 = ValueList<int, 0, 2, 4, 5, 8, 12>;
        cout << IsEmpty<empty_list>::value << endl;
        cout << IsEmpty<list1>::value << endl;
        cout << IsEmpty<list2>::value << endl;
    }

    {
        cout << "================================" << endl;
        cout << "Front Unit Tests" << endl;
        using list1 = ValueList<int, 3>;
        using list2 = ValueList<int, 2, 4, 5, 8, 12>;

        cout << "list1:"; list1::display();
        cout << Front<list1>::value << endl;

        cout << "list2:"; list2::display();
        cout << Front<list2>::value << endl;
    }

    {
        cout << "================================" << endl;
        cout << "PopFront Unit Tests" << endl;
        using list1 = ValueList<int, 3>;
        using list2 = ValueList<int, 2, 4, 5, 8, 12>;
        
        cout << "list1: "; list1::display();
        PopFront<list1>::type::display();
        cout << endl;

        cout << "list2: "; list2::display();
        PopFront<list2>::type::display();
        cout << endl;
    }

    {
        cout << "================================" << endl;
        cout << "PushFront Unit Tests" << endl;
        using empty_list = ValueList<int>;
        using list1 = ValueList<int, 3>;
        using list2 = ValueList<int, 2, 4, 5, 8, 12>;
        
        cout << "empty_list: "; empty_list::display();
        PushFront<empty_list, integral_constant<int, 100>>::type::display();
        cout << endl;

        cout << "list1: "; list1::display();
        PushFront<list1, integral_constant<int, 100>>::type::display();
        cout << endl;

        cout << "list2: "; list2::display();
        PushFront<list2, integral_constant<int, 12>>::type::display();
        cout << endl;
    }

    {
        cout << "================================" << endl;
        cout << "Pushback Unit Tests" << endl;
        using empty_list = ValueList<int>;
        using list1 = ValueList<int, 3>;
        using list2 = ValueList<int, 2, 4, 5, 8, 12>;
        
        cout << "empty_list: "; empty_list::display();
        PushFront<empty_list, integral_constant<int, 100>>::type::display();
        cout << endl;

        cout << "list1: "; list1::display();
        PushBack<list1, integral_constant<int, 100>>::type::display();
        cout << endl;

        cout << "list2: "; list2::display();
        PushBack<list2, integral_constant<int, 12>>::type::display();
        cout << endl;
    }

    {
        cout << "================================" << endl;
        cout << "Index Unit Tests" << endl;
        using list1 = ValueList<int, 0, 1, 2, 3, 4, 5, 6, 7>;
        
        cout << "list: "; list1::display();
        cout << "get<1>: ";
        cout<< Get<list1, 1>::value <<endl;
        cout<< my_get<1>(list1{}) <<endl;


        cout << "get<5>: ";
        cout<< Get<list1, 5>::value <<endl;
        cout<< my_get<5>(list1{}) <<endl;
        cout << endl;
    }

    {
        cout << "================================" << endl;
        cout << "largetValue Unit Tests" << endl;
        using list1 = ValueList<int, 0, 34, 234,121,2342,123,42,654,765,313>;
        
        cout << "list: "; list1::display();
        cout << LargestValue<list1>::value<< endl;
    }


    {
        cout << "================================" << endl;
        cout << "Reverse Unit Tests" << endl;
        using list1 = ValueList<int, 0, 34, 234,121,2342,123,42,654,765,313>;
        
        cout << "list: " << endl; list1::display();
        Reverse<list1>::type::display();
    }

    {
        cout << "================================" << endl;
        cout << "PopBack Unit Tests" << endl;
        using list1 = ValueList<int, 0, 34, 234,121,2342,123,42,654,765,313>;
        
        cout << "list: " << endl; list1::display();
        PopBack<list1>::type::display();
    }

    {
        cout << "================================" << endl;
        cout << "PopBack Unit Tests" << endl;
        using list1 = ValueList<int, 0, 34, 234,121,2342,123,42,654,765,313>;
        
        cout << "list: " << endl; list1::display();
        PopBack<list1>::type::display();
    }

    {
        cout << "================================" << endl;
        cout << "Transform Unit Tests" << endl;
        using list1 = ValueList<int, 0, 34, 234,121,2342,123,42,654,765,313>;
        
        cout << "list: " << endl; list1::display();
        Transfrom<list1, AddConst>::type::display();
    }
}