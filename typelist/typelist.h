#include <iostream>
#include <type_traits>
using namespace std;

template<typename T>
class AddSpace
{
  private:
    T const& ref;                  // refer to argument passed in constructor
  public:
    AddSpace(T const& r): ref(r) {
    }
    friend std::ostream& operator<< (std::ostream& os, AddSpace<T> s) {
      return os << s.ref << ' ';   // output passed argument and a space
    }
};

template<class T, T... Values>
struct ValueList {
    static constexpr int size = sizeof...(Values);
    using value_type = T;

    static void display(){
        if constexpr ( size != 0){
            (cout << ... << AddSpace(Values)) << endl;
        } else {
            cout << "Empty ValueList" <<endl;
        }
    }
};


///////////////////////////////////////////////////////////////////////////////////////////////////

template<class T>
struct IsEmpty{
    static constexpr bool value = false;
};

template<class T, T... Values>
struct IsEmpty<ValueList<T, Values...>> {
    static constexpr bool value = sizeof...(Values) == 0;
};

///////////////////////////////////////////////////////////////////////////////////////////////////
template<class T>
struct Front;

template<class T, T Head, T... Tails>
struct Front<ValueList<T, Head, Tails...>> {
    using type = integral_constant<T, Head>;
    static constexpr T value = Head;
};

///////////////////////////////////////////////////////////////////////////////////////////////////
template<class T>
struct PopFront;

template<class T, T Head, T... Tails>
struct PopFront<ValueList<T, Head, Tails...>> {
    using type = ValueList<T, Tails...>;
};

///////////////////////////////////////////////////////////////////////////////////////////////////
template<class T, class New>
struct PushFront;

template<class T, T... Values, class New>
struct PushFront<ValueList<T, Values...>, New > {
    using type = ValueList<T, New::value , Values...>;
};

///////////////////////////////////////////////////////////////////////////////////////////////////
template<class T, class New>
struct PushBack;

template<class T, T... Values, class New>
struct PushBack<ValueList<T, Values...>, New > {
    using type = ValueList<T, Values..., New::value>;
};


///////////////////////////////////////////////////////////////////////////////////////////////////
template<class T, int N>
struct Get : Get<typename PopFront<T>::type, N-1> {
};

template<class T>
struct Get<T, 0> : Front<T>{
};

template<int N, class T>
auto my_get(T Values){
    return Get<T, N>::type::value;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////Algorithm////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

template<class T, bool isEmpty = IsEmpty<T>::value>
struct LargestValue;

template<class T>
struct LargestValue<T, true>{
    using type = integral_constant<typename T::value_type, -9999>;
    static constexpr typename T::value_type value = type::value;

};

template<class T>
struct LargestValue<T, false>{
private:
    using contender = typename Front<T>::type;
    using rest = typename LargestValue<typename PopFront<T>::type>::type;
public:
    using type = std::conditional_t< (contender::value > rest::value), contender, rest>;
    static constexpr typename T::value_type value = type::value;
};

///////////////////////////////////////////////////////////////////////////////////////////////////

template<class T, bool isEmpty = IsEmpty<T>::value>
struct Reverse;

template<class T>
struct Reverse<T, true>{
    using type = T;
};

template<class T>
struct Reverse<T, false>{
private:
    using Head = typename Front<T>::type;
    using Tail = typename Reverse<typename PopFront<T>::type>::type;
public:
    using type = typename PushBack<Tail, Head>::type;
};

///////////////////////////////////////////////////////////////////////////////////////////////////
template<class T>
struct PopBack;

template<class T>
struct PopBack{
    using type = typename Reverse< typename PopFront<typename Reverse<T>::type>::type >::type;
};

///////////////////////////////////////////////////////////////////////////////////////////////////

template<class T, T A, T B>
struct AddConst{
    static constexpr T value = A + B;
};

template<class T,  template<class VT, VT X, VT Y> class Func, bool isEmpty = IsEmpty<T>::value>
struct Transfrom;

template<class T, T... Values, template<class VT, VT X, VT Y> class Func>
struct Transfrom< ValueList<T, Values...>, Func, false>{
    using type = ValueList< T, Func<T, Values, 10>::value...>;
};

