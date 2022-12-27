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
        auto t1 = toy::make_tuple(1,2,3,4,6,7,8,9,10);
        auto t2 = toy::insert<5>(t1, 99);
        
        cout << "=======================" << endl;
        cout << "========insert=========" << endl;
        cout << "=======================" << endl;

        cout<<"t1       :";
        print_tuple(t1); cout<<endl;
        cout<<"insert   :";
        print_tuple(t2); cout<<endl;
    }

    {
        auto t1 = toy::make_tuple(1,2,3,4,6,7,8,9,10);
        auto t2 = toy::remove<3>(t1);
        
        cout << "=======================" << endl;
        cout << "========remove=========" << endl;
        cout << "=======================" << endl;

        cout<<"t1       :";
        print_tuple(t1); cout<<endl;
        cout<<"remove   :";
        print_tuple(t2); cout<<endl;
    }

    {
        auto t1 = toy::make_tuple(1,2,3,4,6,7,8,9,10);
        auto t2 = toy::replace<5>(t1, 99);
        
        cout << "=======================" << endl;
        cout << "========replace=========" << endl;
        cout << "=======================" << endl;

        cout<<"t1       :";
        print_tuple(t1); cout<<endl;
        cout<<"replace  :";
        print_tuple(t2); cout<<endl;
    }

    {
        auto t1 = toy::repeat<10>('x');
        
        cout << "=======================" << endl;
        cout << "========repeat=========" << endl;
        cout << "=======================" << endl;

        cout<<"repeat   :";
        print_tuple(t1); cout<<endl;
    }

    {
        auto t1 = toy::make_tuple(toy::make_tuple(1, toy::make_tuple(3)),toy::make_tuple(2, toy::make_tuple(4)));
        auto t2 = toy::repeat_like(t1,"x");

        
        cout << "=======================" << endl;
        cout << "========repeat_like====" << endl;
        cout << "=======================" << endl;

        cout << t1 <<endl;
        cout <<"repeat_like  :";
        cout << t2 <<endl;
    }

    {
        auto print_lambda = [](auto const& ...a) { // lambda supports params pack which is different with variadic arguments.
            return (a + ...); 
        };

        auto t1 = toy::make_tuple(1,10,100,12,34,4,1,4,5,12,43);
        auto t2 = toy::apply(t1, print_lambda);
        
        cout << "=======================" << endl;
        cout << "==========Apply========" << endl;
        cout << "=======================" << endl;

        cout<<"t1 :";
        cout<< t1 <<endl;
        cout<<"Apply :";
        cout<< t2 <<endl;
    }

    {
        auto lambda = [](auto const& a) { // lambda supports params pack which is different with variadic arguments.
            return (a + 1); 
        };

        auto t1 = toy::make_tuple(1,10,100,12,34,4,1,4,5,12,43);
        auto t2 = toy::transform(t1, lambda);
        
        cout << "=======================" << endl;
        cout << "=========Transform=====" << endl;
        cout << "=======================" << endl;

        cout<<"t1 :";
        cout<< t1 <<endl;
        cout<<"Transform :";
        cout<< t2 <<endl;
    }

    // {
    //     auto lambda = [&](auto a) { // lambda supports params pack which is different with variadic arguments.
    //         a = a * 2;
    //         cout<<a;
    //     };

    //     auto t1 = toy::make_tuple(3,4,5,3,2,1234,15,9);
        
    //     cout << "=======================" << endl;
    //     cout << "=========for_each=====" << endl;
    //     cout << "=======================" << endl;

    //     cout<<"t1 :";
    //     cout<< t1 <<endl;
    //     cout<<"for_each :";
    //     toy::for_each(t1, lambda); cout<<endl;
    // }

    {
        auto t1 = toy::make_tuple(toy::make_tuple(1, toy::make_tuple(3)),toy::make_tuple(2));
        
        cout << "=======================" << endl;
        cout << "====flattern_tuple=====" << endl;
        cout << "=======================" << endl;

        cout<<"t1 :";
        cout<< t1 <<endl;
        cout<<"flattern_tuple :";
        cout<< toy::flatten_to_tuple(t1)<<endl;
    }

    {
        auto t1 = toy::make_tuple(toy::make_tuple(1, toy::make_tuple(3)),toy::make_tuple(2));
        
        cout << "=======================" << endl;
        cout << "========front==========" << endl;
        cout << "=======================" << endl;

        cout<<"t1 :";
        cout<< t1 <<endl;
        cout<<"front :";
        cout<< toy::front(t1)<<endl;
    }

    {
        auto t1 = toy::make_tuple(toy::make_tuple(1, toy::make_tuple(3)),toy::make_tuple(2, toy::make_tuple(4)));
        
        cout << "=======================" << endl;
        cout << "===========back========" << endl;
        cout << "=======================" << endl;

        cout<<"t1 :";
        cout<< t1 <<endl;
        cout<<"back :";
        cout<< toy::back(t1)<<endl;
    }

    // {
    //     auto t1 = toy::make_tuple(toy::make_tuple(toy::make_tuple(toy::make_tuple(333), 999)));
    //     auto t2 = toy::make_tuple(toy::make_tuple(toy::make_tuple(toy::make_tuple(333))));
        
    //     cout << "=======================" << endl;
    //     cout << "===========unwarp========" << endl;
    //     cout << "=======================" << endl;

    //     cout<<"t1 :";
    //     cout<< t1 <<endl;
    //     cout<<"unwarp :";
    //     cout<< toy::unwrap(t1)<<endl;

    //     cout<<"t2 :";
    //     cout<< t2 <<endl;
    //     cout<<"unwarp :";
    //     cout<< toy::unwrap(t2)<<endl;
    // }

    {
        // ((A,a),((B,b),(C,c)),d) 
        auto t1 = toy::make_tuple(toy::make_tuple("A", "a"), toy::make_tuple(toy::make_tuple("B", "b"), toy::make_tuple("C", "c")), "d");
        auto guide = toy::make_tuple("X", toy::make_tuple("X", "X"));
        auto guide1 = toy::make_tuple("X", "X");

        
        cout << "=======================" << endl;
        cout << "========zip2_by========" << endl;
        cout << "=======================" << endl;

        cout<<"t1       :";
        cout<< t1 <<endl;
        cout<<"Guide    :";
        cout<< guide <<endl;
        cout<<"zip2_by :";
        cout<< toy::zip2_by(t1,guide)<<endl;
    }

}