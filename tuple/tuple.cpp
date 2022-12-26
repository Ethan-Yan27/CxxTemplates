#include <iostream>
#include <tuple.h>
#include <type_traits>

using std::cout;
using std::endl;
using toy::print_tuple;
using toy::Tuple;

template <class T, T v> 
using constant = typename std::integral_constant<T,v>::type;

int main()
{
    {
        auto tmp_tuple = toy::make_tuple(1, 2);
        cout << "=======================" << endl;
        cout << "Tuple<int, int>" << endl;
        cout << "tuple size :" << std::tuple_size_v<decltype(tmp_tuple)> << endl;
        cout << "sizeof : " << sizeof(decltype(tmp_tuple)) << endl;
        cout << "Elements :" ;
        print_tuple(tmp_tuple);
        cout << endl;
    }

    {
        auto tmp_tuple = toy::make_tuple(1, 2, 5, 10);
        
        cout << "=======================" << endl;
        cout << "Tuple<int, int, int, int>" << endl;
        cout << "tuple size :" << std::tuple_size_v<decltype(tmp_tuple)> << endl;
        cout << "sizeof : " << sizeof(decltype(tmp_tuple)) << endl;
        cout << "Elements :" ;
        print_tuple(tmp_tuple);
        cout << endl;
    }

    {
        auto tmp_tuple = toy::make_tuple(1, constant<int, 666>{}, 5, 10);
        
        cout << "=======================" << endl;
        cout << "Tuple<int, constant<int, 666>, int, int>;" << endl;
        cout << "tuple size :" << std::tuple_size_v<decltype(tmp_tuple)> << endl;
        cout << "sizeof : " << sizeof(decltype(tmp_tuple)) << endl;
        cout << "Elements :" ;
        print_tuple(tmp_tuple);
        cout << endl;
    }

    {

        auto tmp_tuple = toy::make_tuple(1, constant<int, 666>{}, constant<int, 777>{}, constant<int, 888>{});
        
        cout << "=======================" << endl;
        cout << "Tuple<int, constant<int, 666>, constant<int, 666>, constant<int, 666>>" << endl;
        cout << "tuple size :" << std::tuple_size_v<decltype(tmp_tuple)> << endl;
        cout << "sizeof : " << sizeof(decltype(tmp_tuple)) << endl;
        cout << "Elements :" ;
        print_tuple(tmp_tuple);
        cout << endl;
    }

    {
        auto tmp_tuple = toy::make_tuple(constant<int, 1>{}, constant<int, 2>{}, constant<int, 3>{}, constant<int, 4>{}); // No matter what is put here :)
        
        cout << "=======================" << endl;
        cout << "Tuple<constant<int, 1>, constant<int, 2>, constant<int, 3>, constant<int, 4>>" << endl;
        cout << "tuple size :" << std::tuple_size_v<decltype(tmp_tuple)> << endl;
        cout << "sizeof : " << sizeof(decltype(tmp_tuple)) << endl;
        cout << "Elements :" ;
        print_tuple(tmp_tuple);
        cout << endl;
    }

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////Operator////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

    {
        auto t1 = toy::make_tuple(1,2,3,4);
        auto t2 = toy::make_tuple(1,2,3);
        auto t3 = toy::make_tuple(1,2,3,4);
        auto t4 = toy::make_tuple(1,constant<int, 2>{},3,4);
        
        cout << "=======================" << endl;
        cout << "======Tuple Equal======" << endl;
        cout << "=======================" << endl;

        cout<<"t1 :";
        print_tuple(t1); cout<<endl;
        cout<<"t2 :";
        print_tuple(t2); cout<<endl;
        cout<<"t3 :";
        print_tuple(t3); cout<<endl;
        cout<<"t4 :";
        print_tuple(t4); cout<<endl;

        cout<< "t1!=t2 : " << (t1 != t2) <<endl;
        cout<< "t1==t3 : " << (t1 == t3) <<endl;
        cout<< "t1==t4 : " << (t1 == t4) <<endl;
        cout<< "t3==t4 : " << (t3 == t4) <<endl;
        cout<< "t{}==t{} : " << (Tuple<>{} == Tuple<>{}) <<endl;

    }

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////Operations/////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

    {
        auto t1 = toy::make_tuple(1,2,3,4,6,7,8,9,10);
        auto t2 = toy::reverse(t1);
        
        cout << "=======================" << endl;
        cout << "========Reverse========" << endl;
        cout << "=======================" << endl;

        cout<<"t1 :";
        print_tuple(t1); cout<<endl;
        cout<<"Reverse :";
        print_tuple(t2); cout<<endl;
    }

    {
        auto t1 = toy::make_tuple(1,2,3,4,6,7,8,9,10);
        auto t2 = toy::reverse(t1);
        auto t3 = toy::cat(t1, t2);
        
        cout << "=======================" << endl;
        cout << "==========Cat==========" << endl;
        cout << "=======================" << endl;

        cout<<"t1 :";
        print_tuple(t1); cout<<endl;
        cout<<"Reverse :";
        print_tuple(t2); cout<<endl;
        cout<<"Cat t1 & t2";
        print_tuple(t3); cout<<endl;
    }

    {
        auto print_lambda = [](auto a, auto b, auto c) {
            return a + b + c;
        };

        auto t1 = toy::make_tuple(1,10,100);
        auto t2 = toy::apply(print_lambda, t1);
        
        cout << "=======================" << endl;
        cout << "==========Apply========" << endl;
        cout << "=======================" << endl;

        cout<<"t1 :";
        cout<< t1 <<endl;
        cout<<"Apply :";
        cout<< t2 <<endl;
    }
}