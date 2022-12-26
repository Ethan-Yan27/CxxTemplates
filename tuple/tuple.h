#include <iostream>
#include <utility>

namespace toy{

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

template <class... T>
constexpr Tuple<std::decay_t<T>...> make_tuple(T &&...t)
{
return {std::forward<T>(t)...};
}

namespace detail {

template <class T>
std::integral_constant<bool, std::tuple_size<T>::value >= 0> has_tuple_size(int);

template <class T>
std::false_type has_tuple_size(...);

} // end namespace detail

template <class T>
struct is_tuple : decltype(detail::has_tuple_size<T>(0)) {};

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

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////Operators///////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
namespace detail{

template<class... T, class... U, std::size_t... Is>
bool tuple_equal(Tuple<T...> const& t, Tuple<U...> const& u, std::integer_sequence<std::size_t, Is...>){
    static_assert(sizeof...(T) == sizeof...(U), "Size is not same!");
    return (... && (get<Is>(t) == get<Is>(u)));
}

}

template<class... T, class... U>
bool operator==(Tuple<T...> const& t, Tuple<U...> const& u) { // TODO: check if it is a tuple
    if constexpr (sizeof...(T) != sizeof...(U)) {
        return false;
    } else {
        return detail::tuple_equal(t, u,  std::index_sequence_for<T...>{});

    }
}

template<class... T, class... U>
bool operator!=(Tuple<T...> const& t, Tuple<U...> const& u) {
    return !(t == u);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////Operations//////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

namespace detail
{

template <class... T, std::size_t... Is>
auto tuple_reverse(Tuple<T...> const& t, std::index_sequence<Is...>)
{
    constexpr std::size_t Len = sizeof...(T);
    return make_tuple(get<Len - Is - 1>(t)...);
}

template <class... T, class... U, std::size_t... Is0, std::size_t... Is1>
auto tuple_cat(Tuple<T...> const& t, Tuple<U...> const& u, std::index_sequence<Is0...>, std::index_sequence<Is1...>)
{
    return make_tuple(get<Is0>(t)..., get<Is1>(u)...);
}

}

template <class... T>
auto reverse(Tuple<T...> const &t)
{
    return detail::tuple_reverse(t, std::index_sequence_for<T...>{});
}

template <class... T>
auto cat(Tuple<T...> const& t){
    return t;
}

template <class... T, class... U>
auto cat(Tuple<T...> const& t, Tuple<U...> const& u){
    return tuple_cat(t, u, std::index_sequence_for<T...>{}, std::index_sequence_for<U...>{});
}
///
// https://en.cppreference.com/w/cpp/utility/apply
// Apply : (t, f) ==> f(t0, t1, .. tn-1) 
// C++ template 2nd: Section 25.4
///

namespace detail{

template<class Tuple, class F, std::size_t... Is>
constexpr auto tuple_apply(Tuple&& t, F&& f, std::index_sequence<Is...>){
    return f(get<Is>(t)...);
}

}

template<class Tuple, class F>
constexpr auto apply(Tuple&& t, F&& f){
    return detail::tuple_apply(std::forward<Tuple>(t), std::forward<F>(f), std::make_index_sequence<std::tuple_size_v<std::remove_reference_t<Tuple>>>{});
}

////
// Usage of apply operations
////

template<typename... Ts>
std::ostream& operator<<(std::ostream& os, Tuple<Ts...> const& Tuple){
    auto lambda = [&os](Ts const&... elems)
    {
        os << "[";
        int n{0};
        ((os << (n++ == 0 ? "" : ",") << elems), ...);
        os << "]";
    };
    
    toy::apply(Tuple, lambda);
    return os;
}

///
// Transform : (t, f) ==> (f(t0), f(t1), .. f(tn-1))
///

namespace detail{

template<class Tuple, class F, class G, std::size_t... Is>
constexpr auto tuple_transform_apply(Tuple&& t, F&& f, G&& g, std::index_sequence<Is...>){
    return g(f(get<Is>(t))...);
}

}

template<class Tuple, class F>
constexpr auto transform(Tuple&& t, F&& f){
    return detail::tuple_transform_apply(std::forward<Tuple>(t), std::forward<F>(f), [](auto... elems){ return make_tuple(elems...);}, std::make_index_sequence<std::tuple_size_v<std::remove_reference_t<Tuple>>>{});
}

///
// For Each : (t, f) => f(t0),f(t1),...,f(tn)
///

// template<class Tuple, class F>
// constexpr void for_each(Tuple&& t, F&& f){
//      detail::tuple_apply(std::forward<Tuple>(t), [&](auto&&... elems) { (f(static_cast<decltype(elems)&&>(elems)),...); }, std::make_index_sequence<std::tuple_size_v<std::remove_reference_t<Tuple>>>{});
// }

///
// Flattern a tuple
// t1 :[[1,[3]],[2]]
// flattern_tuple :[1,3,2]
///

template<class T>
constexpr auto flatten_to_tuple(T const& t){ // XXX cat only supports 1/2 arguments.
    if constexpr (is_tuple<T>::value){
        return detail::tuple_transform_apply(t, [](auto const& a) { return flatten_to_tuple(a);} , [](auto const&... a){ return cat(a...);}, std::make_index_sequence<std::tuple_size_v<std::remove_reference_t<T>>>{});
    } else {
        return make_tuple(t);
    }
}


} // namespace toy

///////////////////////////////////////////////////////////////////////////////////////////////////

namespace std{

template <class... T>
struct tuple_size<toy::Tuple<T...>>
    : std::integral_constant<std::size_t, sizeof...(T)>
{};

} // namespace std