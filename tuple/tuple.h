#include <iostream>
#include <utility>

namespace toy{

template <class T>
using remove_cvref_t = std::remove_cv_t<std::remove_reference_t<T>>;

template<class Tuple>
using make_seq_for_tuple = std::make_index_sequence<std::tuple_size_v<remove_cvref_t<Tuple>>>;

template<class T, int begin>
struct make_seq_range_impl;

template<std::size_t... Is, int begin>
struct make_seq_range_impl<std::index_sequence<Is...> , begin> {
    using type = std::index_sequence<(Is+begin)...>;
};

template<int begin, int end> // [begin, end)
using make_seq_range = typename make_seq_range_impl<std::make_index_sequence< (end-begin > 0) ? (end-begin) : 0>, begin>::type;

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
using TupleBase = detail::TupleBase<std::index_sequence_for<T...>, T...>;

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

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////Get element////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////
// Experiment : perfect forward
// Error: : note: candidate function [with I = 1, T = <int, int>] not viable: expects an "rvalue" for 1st argumen
// As mentioned in C++ templates P94: Note that T must really be the name of a template params. Depending on a template param is not sufficient.

// template<std::size_t I, class... T>
// auto get(Tuple<T...> &&t){
//     static_assert(I < sizeof...(T), "I is out of index");
//     return detail::getv<I>(std::forward<Tuple<T...>>(t)); // TODO: test rvalue.
// }

template<std::size_t I, class T>
auto get(T&& t){
    static_assert(I < std::tuple_size<remove_cvref_t<T>>::value, "I is out of index");
    return detail::getv<I>(std::forward<T>(t)); 
}

template<std::size_t I0, std::size_t I1, std::size_t... Is, class T>
auto get(T&& t){
    return get<I1, Is...> (get<I0>(std::forward<T>(t))); 
}

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
struct is_tuple : decltype(detail::has_tuple_size<toy::remove_cvref_t<T>>(0)) {};

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

namespace detail{

template<class T, class X, std::size_t... Is0, std::size_t... Is1, std::size_t... Is2>
constexpr auto construct(T const&t, X const& x, std::index_sequence<Is0...>,   std::index_sequence<Is1...>,  std::index_sequence<Is2...>){
    // return toy::make_tuple(get<Is0>(t)... , (void(Is1), x)... , get<Is2>(t)... );
    return toy::make_tuple(get<Is0>(t)... , (void(Is1), x)...,  get<Is2>(t)...);

}

} // end namespace detail

// Insert element into the Nth position of the tuple
template<int N, class T, class X>
constexpr auto insert(T const& t, X const& x){
    return detail::construct(t, x, make_seq_range<0, N>{}, std::index_sequence<0>{}, make_seq_range<N, std::tuple_size<T>::value>{});
}

// remove element into the Nth position of the tuple
template<int N, class T>
constexpr auto remove(T const& t){
    return detail::construct(t, 0, make_seq_range<0, N>{}, std::index_sequence<>{}, make_seq_range<N+1, std::tuple_size<T>::value>{});
}

// replace the Nth element of the tuple
template<int N, class T, class X>
constexpr auto replace(T const& t, X const& x){
    return detail::construct(t, x, make_seq_range<0, N>{}, std::index_sequence<0>{}, make_seq_range<N+1, std::tuple_size<T>::value>{});
}

// replace the Nth element of the tuple
template<int N, class X>
constexpr auto repeat(X const& x){
    return detail::construct(0, x, std::index_sequence<>{}, make_seq_range<0, N>{}, std::index_sequence<>{});
}

// replace the Nth element of the tuple
template<class T, class X>
constexpr auto repeat_like(T const& t, X const& x){
    if constexpr(is_tuple<T>::value){
        return transform(t, [&](auto const& a){ return repeat_like(a, x); });
    } else {
        return x;
    }
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
    return detail::tuple_apply(std::forward<Tuple>(t), std::forward<F>(f), make_seq_for_tuple<Tuple>{});
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
    return detail::tuple_transform_apply(std::forward<Tuple>(t), std::forward<F>(f), [](auto... elems){ return make_tuple(elems...);}, make_seq_for_tuple<Tuple>{});
}

///
// For Each : (t, f) => f(t0),f(t1),...,f(tn)
///

// template<class Tuple, class F>
// constexpr void for_each(Tuple&& t, F&& f){
//      detail::tuple_apply(std::forward<Tuple>(t), [&](auto&&... elems) { (f(static_cast<decltype(elems)&&>(elems)),...); }, make_seq_for_tuple<Tuple>{});
// }

///
// Flattern a tuple
// t1 :[[1,[3]],[2]]
// flattern_tuple :[1,3,2]
///

template<class T>
constexpr auto flatten_to_tuple(T const& t){ // XXX cat only supports 1/2 arguments.
    if constexpr (is_tuple<T>::value){
        return detail::tuple_transform_apply(t, [](auto const& a) { return flatten_to_tuple(a);} , [](auto const&... a){ return cat(a...);}, make_seq_for_tuple<T>{});
    } else {
        return make_tuple(t);
    }
}

template<class T>
constexpr auto front(T&& t){
    if constexpr (is_tuple<T>::value){
        return front(get<0>(std::forward<T>(t)));
    } else {
        return std::forward<T>(t);
    }
}

template<class T>
constexpr auto back(T&& t){
    if constexpr (is_tuple<T>::value){
        constexpr int N = std::tuple_size<remove_cvref_t<T>>::value;
        return back(get<N-1>(std::forward<T>(t)));
    } else {
        return std::forward<T>(t);
    }
}

// template <class T>
// constexpr auto unwrap(T const& t)  /// XXX: do not have general usage of this function.
// {
//   if constexpr (is_tuple<T>::value) {
//     if constexpr (std::tuple_size<T>::value == 1) {
//       return unwrap(get<0>(t));
//     } else {
//       return t;
//     }
//   } else {
//     return t;
//   }
// }

///
// zip2_by -- A guided zip for rank-2 tuples
//   Take a tuple like ((A,a),((B,b),(C,c)),d) 
//   and produce a tuple ((A,(B,C)),(a,(b,c),d)) 
//   where the rank-2 modes are selected by the terminals of the guide (X,(X,X))
///

namespace detail {

template <class T, class TG, std::size_t... Is, std::size_t... Js>
constexpr auto zip2_by(T const& t, TG const& guide, std::index_sequence<Is...>, std::index_sequence<Js...>)
{
  // zip2_by produces the modes like ((A,a),(B,b),...)
  auto split = make_tuple(zip2_by(get<Is>(t), get<Is>(guide))...);
  std::cout<<"split: "<<split<< "guide:" <<guide <<std::endl;

  // Rearrange and append missing modes from t to make ((A,B,...),(a,b,...,x,y))
  return make_tuple(make_tuple(get<Is,0>(split)...),
                    make_tuple(get<Is,1>(split)..., get<Js>(t)...));
}

} // end namespace detail

template <class T, class TG>
constexpr auto zip2_by(T const& t, TG const& guide)
{
  if constexpr (is_tuple<TG>::value) {
    constexpr int TR = std::tuple_size<T>::value; // 3
    constexpr int GR = std::tuple_size<TG>::value; // 2
    static_assert(TR >= GR, "Mismatched ranks");
    return detail::zip2_by(t, guide,
                           make_seq_range< 0, GR>{},
                           make_seq_range<GR, TR>{});
  } else {
    // For X in the guide, the corresponding tuple element must be rank-2
    static_assert(std::tuple_size<T>::value == 2, "Mismatched ranks");
    return t;
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