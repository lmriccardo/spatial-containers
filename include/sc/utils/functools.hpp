#pragma once

#include <tuple>
#include <cstddef>
#include <algorithm>
#include <concepts>
#include <type_traits>
#include <functional>
#include <utility>

#include "traits.hpp"

namespace sc
{

/**
 * @brief Just an helper function for the for_each_pair function
 */
template<class F, class... U, std::size_t... I>
constexpr void for_each_pair_impl( F&& fn, std::tuple<U...>& t, 
    std::index_sequence<I...> )
{
    ( fn( std::get<2*I>(t), std::get<2*I+1>(t) ), ... );
}

/**
 * @brief Applies a callable to each consecutive pair of arguments.
 * 
 * Given an even number of arguments (u0, u1, u2, u3, ...),
 * this function groups them into pairs (u0,u1), (u2,u3), ...
 * and invokes the provided functor `fn` on each pair.
 * 
 * @tparam F  Type of the callable object.
 * @tparam U  Types of the forwarded arguments.
 * 
 * @param fn  A callable that accepts two arguments at a time.
 * @param u   A variadic list of arguments, whose count must be even.
 * 
 * @warning The callable `fn` must be invocable as `fn(arg_i, arg_{i+1})`.
 */
template<class F, class... U>
constexpr void for_each_pair( F&& fn, U&&... u )
{
    static_assert(sizeof...(U) % 2 == 0,
                  "Requires an even number of arguments");

    auto tup = std::forward_as_tuple(std::forward<U>(u)...);
    constexpr std::size_t pair_count = sizeof...(U) / 2;

    for_each_pair_impl(
        std::forward<F>(fn),
        tup,
        std::make_index_sequence<pair_count>{}
    );
}

/**
 * @brief Finds the element in a vector that minimizes a projected value 
 * according to a comparison operator.
 * 
 * This function iterates over the elements of `vec`, applies a projection 
 * function `proj` to each element, and returns the element whose projected 
 * value is considered "minimal" according to the binary comparison operator 
 * `comp`. Additional arguments can be forwarded to the projection function.
 * 
 * @tparam _Tp The type of elements in the vector.
 * @tparam _Proj A projection function applied to each element before comparison.
 * @tparam _ComparisonOp A binary comparison operator.
 * @tparam _Args Additional arguments forwarded to the projection function.
 * 
 * @param vec The vector of elements to search.
 * @param proj The projection function to apply to each element.
 * @param comp The comparison operator to determine the minimum.
 * @param args Optional additional arguments forwarded to `proj`.
 *
 *  @return The minimum element in the input vector.
 */
template<class _Tp, class _Proj, class _ComparisonOp, class... _Args>
requires ProjectedComparable<_Tp, _Proj, _ComparisonOp, _Args...>
constexpr _Tp min( std::vector<_Tp> const& vec, _Proj proj, 
    _ComparisonOp comp, _Args&& ...args )
{
    assert(!vec.empty()); // The vector must not be empty

    _Tp  curr_min = vec.front();
    auto best_val = std::invoke( proj, curr_min, std::forward<_Args>(args)... );

    for (auto curr_it = std::next( vec.begin() ); curr_it != vec.end(); ++curr_it)
    {
        auto _p = std::invoke( proj, *curr_it, std::forward<_Args>(args)... );
        if ( std::invoke(comp, _p, best_val) )
        {
            best_val = _p;
            curr_min = *curr_it;
        }
    }

    return curr_min;
}

/**
 * @brief Finds the minimum item in the vector according to a comparison operator.
 * 
 * @tparam _Tp The type of elements in the vector.
 * @tparam _ComparisonOp A binary comparison operator (e.g., std::less<>).
 * 
 * @param vec The vector of elements to search.
 * @param comp The comparison operator to determine the minimum.
 * 
 * @return The element in `vec` considered minimal according to `comp`.
 */
template<class _Tp, class _ComparisonOp>
constexpr _Tp min( std::vector<_Tp> const& vec, _ComparisonOp comp )
{ return sc::min( vec, std::identity{}, comp ); }

template<Point P, class Compare>
constexpr P compare_put(P const& p1, P const& p2, Compare cmp)
{
    P result{};
    std::transform(p1.begin(), p1.end(), p2.begin(), result.begin(), cmp);
    return result;
}

};