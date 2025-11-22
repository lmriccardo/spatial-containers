#pragma once

#include <limits>
#include <concepts>
#include <ranges>
#include <algorithm>
#include <functional>

#include "point.hpp"
#include "sc/utils/traits.hpp"
#include "sc/utils/functools.hpp"

namespace sc
{

// Forward declaration of the bounding box class
template<Numeric T, std::size_t N>
class bbox;

// ================================================================================
// HELPER FUNCTIONS FOR BOUNDING BOX
// ================================================================================

template<class T, std::size_t N>
constexpr bool contains( bbox<T, N> const& box1, bbox<T, N> const& box2, 
    std::size_t idx ) noexcept
{ return box2.min()[idx] >= box1.min()[idx] && box2.max()[idx] <= box1.max()[idx]; }

template<class T, std::size_t N>
constexpr bool contains( bbox<T, N> const& box1, pointnd<T, N> const& pnt, 
    std::size_t idx ) noexcept
{ return pnt[idx] >= box1.min()[idx] && pnt[idx] <= box1.max()[idx]; }

template<class T, std::size_t N, Containable U>
constexpr bool contains( bbox<T, N> const& box1, U const& obj ) noexcept
{
    return std::ranges::all_of(
        std::ranges::iota_view{std::size_t{0}, N},
        [&](std::size_t i){ return contains(box1, obj, i); }
    );
}

template<class T, std::size_t N, Containable U>
constexpr bbox<T, N> merge( bbox<T,N> const& box, U const& obj ) noexcept
{
    pointnd<T,N> new_min, new_max;
    pointnd<T,N> min_comp, max_comp;

    if constexpr ( std::same_as<typename U::tag, bbox_tag> ) {
        min_comp = obj.min(); max_comp = obj.max();
    } else {
        min_comp = obj; max_comp = obj;
    }

    new_min = compare_put( box.min(), min_comp, std::ranges::min );
    new_max = compare_put( box.max(), max_comp, std::ranges::max );
    return bbox<T, N>{ new_min, new_max };
}

template<class T, std::size_t N, Containable U>
constexpr bool overlaps( bbox<T, N> const& box1, bbox<T, N> const& box2, 
    std::size_t idx ) noexcept
{ return box1.max()[idx] < box2.min()[idx] || box2.max()[idx] < box1.min()[idx]; }

template<class T, std::size_t N, Containable U>
constexpr bool overlaps( bbox<T, N> const& box1, U const& obj ) noexcept
{
    return std::ranges::all_of(
        std::ranges::iota_view{std::size_t{0}, N},
        [&](std::size_t i){ return overlaps(box1, obj, i); }
    );
}

template<class T, std::size_t N> constexpr T
enlargement( bbox<T, N> const& box1, bbox<T, N> const& box2 ) noexcept
{
    T union_v{1}, origin_v{1};

    for (std::size_t i = 0; i < N; ++i)
    {
        T new_max = std::max(box1.max()[i], box2.max()[i]);
        T new_min = std::min(box1.min()[i], box2.min()[i]);
        union_v *= new_max - new_min;
        origin_v *= box1.max()[i] - box1.min()[i];
    }

    return union_v - origin_v;
}

// ================================================================================
// ACTUAL IMPLEMENTATION OF THE BOUNDING BOX CLASS
// ================================================================================

template<Numeric T, std::size_t N>
class bbox
{
private:
    pointnd<T, N> m_min; // The minimum corner of the bbox
    pointnd<T, N> m_max; // The maximum corner of the bbox

public:
    using tag = bbox_tag;

    constexpr bbox()
    {
        // The default constructor sets the minimum to (0,0)
        // and the maximum corner to the limits values for T
        m_min.set( T{0} );
        m_max.set( std::numeric_limits<T>::max() );
    }

    constexpr bbox( pointnd<T, N> const& min, pointnd<T, N> const& max )
    : m_min( min ), m_max( max )
    {}

    constexpr bbox(const bbox&) = default;
    constexpr bbox(bbox&&) = default;
    constexpr bbox& operator=(const bbox&) = default;
    constexpr bbox& operator=(bbox&&) = default;

    ~bbox() = default;

    constexpr pointnd<T, N> const& min() const noexcept
    { return m_min; }
    
    constexpr pointnd<T, N> const& max() const noexcept
    { return m_max; }

    void set( T min, T max, std::size_t idx )
    { m_min.set(idx, min); m_max.set(idx, max); }

    /**
     * @brief Check if a point is contained into the box
     */
    template<Containable U>
    constexpr bool contains( U const& obj ) const noexcept
    { return sc::contains(*this, obj); }

    /**
     * @brief Checks if the input box overlaps
     */
    template<Containable U>
    constexpr bool overlaps( U const& obj ) const noexcept
    { return sc::overlaps(*this, obj); }
    
    /**
     * @brief Merges the current bounding box with either an input
     * N-dimensional point or another bounding box.
     */
    template<Containable U>
    constexpr bbox<T,N> merge( U const& obj ) const noexcept
    { return sc::merge(*this, obj); }

    /**
     * @brief Perform in-place merge.
     */
    template<Containable U>
    constexpr void expand( U const& obj ) noexcept
    { *this = sc::merge(*this, obj); }

    /**
     * @brief Computes the enlargement required to fully contain another bbox.
     * 
     * This function calculates how much this bounding box would need to grow
     * in order to completely include the input bounding box `other`. The 
     * enlargement is measured as the increase in volume (or hyper-volume 
     * in N dimensions). 
     * 
     * @param other The bounding box to include.
     * @return The required enlargement as an integer value.
     */
    constexpr T enlargement( bbox<T,N> const& other ) const noexcept
    { return sc::enlargement(*this, other); }
};

template<typename T>
using bbox2d = bbox<T,2>;

template<typename T>
using bbox3d = bbox<T,3>;

};