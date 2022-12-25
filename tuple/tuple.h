#include <iostream>
#include <utility>

namespace detail {

///////////////////////////////////////////////////////////////////////////////////////////////////

template <std::size_t I, class T, bool IsEmpty = std::is_empty_v<T>>
struct TupleElt;

template <std::size_t I, class T>
struct TupleElt<I, T, true>
{
    TupleElt(){};

    template <class U>
    TupleElt(U const &u){};

};

template <std::size_t N, class T>
constexpr T getv(TupleElt<N, T, true> const&)
{ return {}; }

template <std::size_t I, class T>
struct TupleElt<I, T, false>
{
    TupleElt() : t_{} {};

    template <class U>
    TupleElt(U const &u) : t_(u){};

    T t_;
};

template <std::size_t N, class T>
constexpr T& getv(TupleElt<N, T, false> & x)
{ return x.t_; }

template <std::size_t N, class T>
constexpr T const& getv(TupleElt<N, T, false> const& x)
{ return x.t_; }

template <std::size_t N, class T>
constexpr T&& getv(TupleElt<N, T, false> && x)
{ return static_cast<T&&>(x.t_); }

///////////////////////////////////////////////////////////////////////////////////////////////////

template<class Idx, class... T>
struct TupleBase;

template<std::size_t... I, class... T>
struct TupleBase <std::index_sequence<I...>, T...> : TupleElt<I, T>... { // Empty base class optimization (EBCO)

    TupleBase() {}

    template<class... U>
    TupleBase(U const&... u): TupleElt<I, T>(u)... {}

    template<class... U>
    TupleBase(TupleBase<std::index_sequence<I...>, U...> const& u): TupleElt<I, T>(getv(static_cast<TupleElt<I, T> const&>(u)))...  {}

};

} // namespace detail

template <class... T>
using TupleBase = detail::TupleBase<std::make_index_sequence<sizeof...(T)>, T...>;

template <class... T>
struct Tuple: TupleBase<T...> {
    // Default constructor
    Tuple() {}
    
    // Paramterized constructor
    template<class... U>
    Tuple(U const& ...  u): TupleBase<T...>{u...} {}
    
    // Copy constructor
    template<class... U>
    Tuple(Tuple<U...> const& u): TupleBase<T...>{static_cast<TupleBase<T...> const&>(u)} {}
};

template<std::size_t I, class... T>
auto get(Tuple<T...> const& t){
    static_assert(I < sizeof...(T), "I is out of index");
    return detail::getv<I>(t); 
}

template<std::size_t I, class... T>
auto get(Tuple<T...> &t){
    static_assert(I < sizeof...(T), "I is out of index");
    return detail::getv<I>(t); 
}

template<std::size_t I, class... T>
auto get(Tuple<T...> && t){
    static_assert(I < sizeof...(T), "I is out of index");
    return detail::getv<I>(static_cast<Tuple<T...> &&>(t)); 
}

////////////////////////
// Experiment : perfect forward
// Error: : note: candidate function [with I = 1, T = <int, int>] not viable: expects an "rvalue" for 1st argumen
// As mentioned in C++ templates P94: Note that T must really be the name of a template params. Depending on a template param is not sufficient.

// template<std::size_t I, class... T>
// auto get(Tuple<T...> &&t){
//     static_assert(I < sizeof...(T), "I is out of index");
//     return detail::getv<I>(std::forward<Tuple<T...>>(t)); // TODO: test rvalue.
// }
///////////////////////////////////////////////////////////////////////////////////////////////////

namespace std{

template <class... T>
struct tuple_size<Tuple<T...>>
    : std::integral_constant<std::size_t, sizeof...(T)>
{};

}
///////////////////////////////////////////////////////////////////////////////////////////////////

template<class... T, std::size_t... Is>
void print_tuple_core(Tuple<T...> const& t, std::integer_sequence<std::size_t, Is...>){
    // Fold expression : https://en.cppreference.com/w/cpp/language/fold
    // Unary right : ( Exp Op ...) = (E1 op (... op (EN-1 op EN)))
    // Exp  = (std::cout << (Is == 0 ? "" : ",") << get<Is>(t))
    // Op = ,
    // (cout<< E1, (cout<<E2, (cout << E3...(cout<<En-1, cout<<En))))
    ((std::cout << (Is == 0 ? "" : ",") << get<Is>(t)), ...);  
}

template<class... T>
void print_tuple(Tuple<T...> const& t){
    std::cout << "(";
    print_tuple_core(t, std::index_sequence_for<T...>{});
    std::cout << ")";
}