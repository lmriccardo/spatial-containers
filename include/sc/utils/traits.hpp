#pragma once

#include <type_traits>
#include <concepts>

namespace sc
{

struct point_tag {};
struct bbox_tag  {};

/**
 * Concept to identify a given class a generic point object
 */
template<class P>
concept Point = requires(P p) {
typename P::tag; // Requires at least the presence of the tag
P::dim; // Requires the presence of the dim
{ p[0] } -> std::convertible_to<typename P::value_type>;
} && ( std::same_as<typename P::tag, point_tag> );

// Concepts to limit type to pointnd and bbox
template<typename U>
concept Containable =
requires { typename U::tag; } &&
( std::same_as<typename U::tag, point_tag> ||
  std::same_as<typename U::tag, bbox_tag> 
);

template<class T>
concept Numeric = std::is_arithmetic_v<T>;

template<class T, std::size_t N, class ...U>
concept ArgsNConvertible =
    ( sizeof...(U) == N ) &&
    ( (std::convertible_to<U,T>) && ... ) &&
    ( (std::is_arithmetic_v<std::decay_t<U>>) && ... );

template<class _Tp, class _Proj, class _ComparisonOp, class... _Args>
concept ProjectedComparable = std::invocable<_Proj, _Tp, _Args...> &&
std::invocable<_ComparisonOp, std::invoke_result_t<_Proj,_Tp,_Args...>,
    std::invoke_result_t<_Proj,_Tp,_Args...>>;

};