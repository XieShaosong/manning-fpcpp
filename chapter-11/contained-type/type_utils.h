#ifndef UTILS_H
#define UTILS_H


// Utility class for debugging deduced types
template <typename T>
class error;


// Utility meta-function for detecting type validity
template <typename ...T>
using void_t = void;

// If you are using an older compiler,
// replace the above definition with the following:
//     template<typename... Ts> struct make_void { typedef void type;};
//     template<typename... Ts> using void_t = typename make_void<Ts...>::type;


// Meta-function that returns the type of an element
// in an iterable collection
template <typename T>
using contained_type =
    std::remove_cv_t<
        std::remove_reference_t<
            decltype(*std::begin(std::declval<T>()))
        >
    >;


// Meta-function that returns a type with references stripped
template <typename T>
struct remove_reference {
    using type = T;
};

template <typename T>
struct remove_reference<T&> {
    using type = T;
};

template <typename T>
struct remove_reference<T&&> {
    using type = T;
};

template <typename T>
using remove_reference_t = typename remove_reference<T>::type;


// Meta-function that detects whether a type
// has a nested `value_type` type definition
template < typename C
         , typename = void_t<>
         >
struct has_value_type : std::false_type {};

template<typename C>
struct has_value_type<C, void_t<typename C::value_type>> : std::true_type {};


template < typename C
         , typename = void_t<>
         >
struct is_iterable : std::false_type {};

template <typename C>
struct is_iterable
    <C, void_t < decltype(*std::begin(std::declval<C>()))
               , decltype(std::end(std::declval<C>()))
               >>
    : std::true_type {};


// Meta function that always returns false
// Useful with static_assert and if-constexpr
template <typename...>
struct false_: std::false_type {};





#endif /* !UTILS_H */
