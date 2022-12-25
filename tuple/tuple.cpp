#include <iostream>
#include <tuple.h>
#include <type_traits>

using std::cout;
using std::endl;

template <class T, T v> 
using constant = typename std::integral_constant<T,v>::type;

int main()
{
    {
        using tuple_type = Tuple<int, int>;
        tuple_type tmp_tuple(1, 2);
        cout << "=======================" << endl;
        cout << "Tuple<int, int>" << endl;
        cout << "tuple size :" << std::tuple_size_v<tuple_type> << endl;
        cout << "sizeof : " << sizeof(tuple_type) << endl;
        cout << "Elements :" ;
        print_tuple(tmp_tuple);
        cout << endl;
    }

    {
        using tuple_type = Tuple<int, int, int, int>;
        tuple_type tmp_tuple(1, 2, 5, 10);
        
        cout << "=======================" << endl;
        cout << "Tuple<int, int, int, int>" << endl;
        cout << "tuple size :" << std::tuple_size_v<tuple_type> << endl;
        cout << "sizeof : " << sizeof(tuple_type) << endl;
        cout << "Elements :" ;
        print_tuple(tmp_tuple);
        cout << endl;
    }

    {
        using tuple_type = Tuple<int, constant<int, 666>, int, int>;
        tuple_type tmp_tuple(1, constant<int, 666>{}, 5, 10);
        
        cout << "=======================" << endl;
        cout << "Tuple<int, constant<int, 666>, int, int>;" << endl;
        cout << "tuple size :" << std::tuple_size_v<tuple_type> << endl;
        cout << "sizeof : " << sizeof(tuple_type) << endl;
        cout << "Elements :" ;
        print_tuple(tmp_tuple);
        cout << endl;
    }

    {
        using tuple_type = Tuple<int, constant<int, 666>, constant<int, 777>, constant<int, 888>>;
        tuple_type tmp_tuple(1, constant<int, 666>{}, constant<int, 777>{}, constant<int, 888>{});
        
        cout << "=======================" << endl;
        cout << "Tuple<int, constant<int, 666>, constant<int, 666>, constant<int, 666>>" << endl;
        cout << "tuple size :" << std::tuple_size_v<tuple_type> << endl;
        cout << "sizeof : " << sizeof(tuple_type) << endl;
        cout << "Elements :" ;
        print_tuple(tmp_tuple);
        cout << endl;
    }

    {
        using tuple_type = Tuple<constant<int, 1>, constant<int, 2>, constant<int, 3>, constant<int, 4>>;
        tuple_type tmp_tuple(1, constant<int, 666>{}, 5, 10); // No matter what is put here :)
        
        cout << "=======================" << endl;
        cout << "Tuple<constant<int, 1>, constant<int, 2>, constant<int, 3>, constant<int, 4>>" << endl;
        cout << "tuple size :" << std::tuple_size_v<tuple_type> << endl;
        cout << "sizeof : " << sizeof(tuple_type) << endl;
        cout << "Elements :" ;
        print_tuple(tmp_tuple);
        cout << endl;
    }
}