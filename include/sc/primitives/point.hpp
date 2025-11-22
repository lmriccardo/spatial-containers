#pragma once

#include <cstddef>
#include <array>
#include <type_traits>
#include <concepts>

#include "sc/utils/traits.hpp"

namespace sc
{

/**
 * @brief A generic N-dimensional point
 * @tparam T The data type of array elements
 * @tparam N The size of the array (the dimension)
 */
template <typename T, std::size_t N>
class pointnd
{
protected:
    std::array<T, N> m_array;

public:
    using tag = point_tag;
    using value_type = T;
    
    static constexpr std::size_t dim = N;

    constexpr pointnd() : m_array{} {}; // Default constructor
    
    // Construct with a default value
    template<class V>
    requires std::convertible_to<V,T>
    constexpr explicit pointnd( V&& default_val ) 
    { set<V>( std::forward<V>(default_val) ); }

    constexpr pointnd( pointnd const& other ) = default;
    constexpr pointnd( pointnd&& other ) = default;
    constexpr pointnd& operator=( pointnd const& other ) = default;
    constexpr pointnd& operator=( pointnd&& other ) = default;

    // Construct from an initializer list
    pointnd( std::initializer_list<T> init )
    { std::copy( init.begin(), init.end(), m_array.begin() ); }

    // Fill the entire array with the given value
    template<class V>
    requires std::convertible_to<V,T>
    constexpr void set( V&& value ) 
    { m_array.fill( T( std::forward<V>(value) ) ); }

    // Put the input value at the given position
    template<class V>
    requires std::convertible_to<V,T>
    constexpr void set( std::size_t idx, V&& value ) 
    { m_array[idx] = T( std::forward<V>(value) ); }

    // Returns a const reference to elements in the array
    constexpr T const& operator[]( std::size_t idx ) const 
    { return m_array[idx]; }

    // Returns a modifiable reference to elements in the array
    constexpr T& operator[]( std::size_t idx ) 
    { return m_array[idx]; }

    constexpr auto begin() noexcept 
    { return std::begin(m_array); }

    constexpr auto end() noexcept
    { return std::end(m_array); }
    
    constexpr auto begin() const noexcept 
    { return std::begin(m_array); }
    
    constexpr auto end() const noexcept
    { return std::end(m_array); }
};

/**
 * @brief A 2D point using the N-dimensional point class
 * @tparam T The data type of the elements
 */
template<class T>
struct point2d : public pointnd<T,2>
{
    constexpr point2d() = default;

    template<class V>
    requires std::convertible_to<V,T>
    constexpr explicit point2d( V&& default_val )
    : pointnd<T,2>( std::forward<V>(default_val) )
    {}

    template<class V>
    requires std::convertible_to<V,T>
    constexpr point2d(V&& x_val, V&& y_val)
    : pointnd<T,2>()
    {
        this->m_array[0] = std::forward<V>(x_val);
        this->m_array[1] = std::forward<V>(y_val);
    }

    constexpr point2d( point2d const& other ) noexcept
    : pointnd<T,2>( other )
    {}

    constexpr point2d( point2d&& other ) noexcept
    : pointnd<T,2>( std::move(other) )
    {}

    constexpr point2d& operator=(point2d const& other) noexcept = default;
    constexpr point2d& operator=(point2d&& other) noexcept = default;

    constexpr T& x() noexcept
    { return this->m_array[0]; }

    constexpr T& y() noexcept
    { return this->m_array[1]; }

    constexpr T const& x() const noexcept
    { return this->m_array[0]; }

    constexpr T const& y() const noexcept
    { return this->m_array[1]; }
};

/**
 * @brief A 3D point using the N-dimensional point class
 * @tparam T The data type of the elements
 */
template<class T>
struct point3d : public pointnd<T,3>
{
    constexpr point3d() = default;

    template<class V>
    requires std::convertible_to<V,T>
    constexpr explicit point3d( V&& default_val )
    : pointnd<T,3>( std::forward<V>(default_val) )
    {}

    template<class V>
    requires std::convertible_to<V,T>
    constexpr point3d(V&& x_val, V&& y_val, V&& z_val)
    : pointnd<T,3>()
    {
        this->m_array[0] = std::forward<V>(x_val);
        this->m_array[1] = std::forward<V>(y_val);
        this->m_array[2] = std::forward<V>(z_val);
    }

    constexpr point3d( point3d const& other ) noexcept
    : pointnd<T,3>( other )
    {}

    constexpr point3d( point3d&& other ) noexcept
    : pointnd<T,3>( std::move(other) )
    {}

    constexpr point3d& operator=(point3d const& other) noexcept = default;
    constexpr point3d& operator=(point3d&& other) noexcept = default;

    constexpr T& x() noexcept
    { return this->m_array[0]; }

    constexpr T& y() noexcept
    { return this->m_array[1]; }
    
    constexpr T& z() noexcept
    { return this->m_array[2]; }

    constexpr T const& x() const noexcept
    { return this->m_array[0]; }

    constexpr T const& y() const noexcept
    { return this->m_array[1]; }

    constexpr T const& z() const noexcept
    { return this->m_array[2]; }
};

};