#pragma once

#include <memory>
#include <optional>
#include <vector>
#include <tuple>
#include <limits>

#include "sc/primitives/bbox.hpp"
#include "sc/utils/traits.hpp"
#include "sc/utils/functools.hpp"

namespace sc
{

// RTree Class forward declaration for friend definition
template<class V, Numeric T, std::size_t N>
class rtree;

/**
 * @brief
 */
template<class V, Numeric T, std::size_t N>
class rtree_node
{
private:
    friend class rtree<V,T,N>;

    using value_t       = std::optional<V>;
    using self_t        = std::unique_ptr<rtree_node>;
    using box_t         = bbox<T,N>;
    using pointer       = rtree_node*;
    using const_pointer = rtree_node const*;

public:
    // rTree Node internal type for non-leaf nodes
    struct internal_type
    {
        box_t  box;   // The Bounding Box of the child
        self_t child; // A pointer to the actual child

        const_pointer operator*() const { return child.get(); };
    };

    struct leaf_type
    {
        box_t box;   // The Bounding Box of the last node
        V     value; // The value of the last node

        V operator*() const { return value; }
    };

private:

    box_t   m_box;              // An N-dimensional rectangle (the bounding box)
    pointer m_parent = nullptr; // The parent node (nullptr only for roots)

    std::vector<internal_type> m_childs; // A vector of all children of this node
    std::vector<leaf_type>     m_values; // A vector of all values for this node (if leaf)

    /**
     * Creates a new unique pointer to a new node without using
     * make_unique since the rtree node constructors are private
     */
    static self_t make( box_t const& in_box )
    { return self_t( new rtree_node( in_box ) ); }

    /**
     * Unified search functions for both leaf and internal nodes.
     */
    template<class _Tp> static auto
    search( std::vector<_Tp> const& search_v, box_t const& in_box )
    {
        using __Tp = decltype(*std::declval<_Tp>());
        
        // First we need to check if there is at least one element
        if ( search_v.empty() ) return std::vector<__Tp>{};

        std::vector<__Tp> result;
        result.reserve( search_v.size() );

        for ( auto const& element: search_v ) 
        {
            auto const& box = element.box;
            if ( box.overlaps(in_box) || in_box.contains(box) ) 
            {
                result.push_back( *element );
            }
        }

        return result;
    }

    /**
     * @brief Expand the current bounding box to fit the one in input and,
     * if anything has changed, it also calls expand on the parent node.
     */
    void expand( box_t const& in_box ) noexcept
    {
        T prev_hvol = m_box.hvolume();
        m_box.expand( in_box );
        if ( prev_hvol != m_box.hvolume() && m_parent != nullptr )
        {
            m_parent->expand( m_box );
        }
    }

    /**
     * @brief Construct an rTree Node starting from a list of bounds
     * 
     * The input list of bounds must be formed like this I1,...,In where 
     * where Ij = ( Ij1, Ij2 ), i.e., lower and upper bounds for the dimension j. 
     */
    template<class ...U>
    requires ArgsNConvertible<T, 2*N, U...>
    rtree_node( U&&... bounds )
    {
        std::size_t curr_idx{0};
        for_each_pair(
            [&]( auto&& u1, auto&& u2 ) { m_box.set(u1, u2, curr_idx++); },
            std::forward<U>(bounds)...
        );
    }

    /**
     * @brief Initialize the node with a parent and a list of bounds
     */
    template<class ...U>
    requires ArgsNConvertible<T, 2*N, U...>
    rtree_node( pointer parent, U&&... bounds )
    : rtree_node( std::forward<U>(bounds)... ) 
    { m_parent = parent; }

    /**
     * @brief Construct an rTree Node with the input bbox.
     */
    rtree_node( bbox<T,N> const& bbox, pointer parent = nullptr ) 
    : m_box(bbox), m_parent( parent ) 
    {}

    /**
     * @brief Constructor overload for moving the bounding box
     */
    rtree_node( bbox<T,N>&& bbox, pointer parent = nullptr ) 
    : m_box(std::move(bbox)), m_parent( parent ) 
    {}

    // Copy constructor is deleted given the vector of unique pointers
    rtree_node( rtree_node const& ) = delete;
    rtree_node( rtree_node&& ) = default;

    // Copy operator is deleted given the vector of unique pointers
    rtree_node& operator=( rtree_node const& ) = delete;
    rtree_node& operator=( rtree_node&& ) = default;

    /**
     * @brief Set a new parent for the current node
     */
    void set_parent( pointer parent ) noexcept
    { m_parent = parent; }

    /**
     * @brief Adds the input value with the MBR to the current node
     */
    void add( box_t const& mbr, V const& value )
    { m_values.push_back( leaf_type{ mbr, value } ); 
      expand( mbr ); }

    /**
     * @brief Adds/create new child with the MBR to the current node. This
     * function will also expand the current bbox of the parent node
     */
    void add( box_t const& mbr )
    {
        auto child_p = std::make_unique<rtree_node>( this, mbr );
        m_childs.push_back( internal_type{ mbr, std::move(child_p) } );
        expand( mbr );
    }

    /**
     * @brief Adds a new value that has the same MBR of the node
     */
    void add( V const& value ) noexcept
    { m_values.push_back( leaf_type{ m_box, value } ); }

    /**
     * @brief From among all children picks the one whose bbox grows
     * the least to fit the input bounding box.
     * 
     * @param in_box The input query bounding box region
     */
    pointer pick_best( box_t const& in_box ) const
    {
        internal_type const& first_value = m_childs.front();
        pointer best_child = first_value.child.get();
        auto best_val = first_value.box.enlargement( in_box );

        for ( auto it = std::next(m_childs.begin()); it != m_childs.end(); ++it )
        {
            auto const& curr = *it;
            auto curr_val = curr.box.enlargement( in_box );
            if ( curr_val < best_val )
            {
                best_child = curr.child.get();
                best_val = curr_val;
            }
        }

        return best_child;
    }

public:

    /**
     * @brief Tells if the current node is a leaf or not
     */
    bool is_leaf() const noexcept 
    { return !m_values.empty() && m_childs.empty(); }

    /**
     * @brief Tells if the current node is a root node or not
     */
    bool is_root() const noexcept
    { return m_parent == nullptr; }

    /**
     * @brief Returns the total number of children or values (if a leaf node)
     */
    std::size_t size() const noexcept
    { return (is_leaf()) ? m_values.size() : m_childs.size(); }

    /**
     * @brief Returns the current parent of the node
     */
    const_pointer parent() const noexcept
    { return m_parent; }

    /**
     * @brief Searches for a list of colliding children.
     * 
     * Returns a vector of children whose bounding box collides
     * with the one of the input node.
     * 
     * @param in_box The input query bounding box region
     */
    auto search_childs( bbox<T,N> const& in_box ) const
    { return search( m_childs, in_box ); }

    /**
     * @brief Searches for a list of colliding values if the node is a leaf node.
     * 
     * Returns a vector of values whose bounding box collides
     * with the one of the input node.
     * 
     * @param in_box The input query bounding box region
     */
    auto search_leafs( bbox<T,N> const& in_box ) const
    { return search( m_values, in_box ); }

    /**
     * @brief Returns the value (bbox, value) at input index
     */
    std::optional<leaf_type> value_at( std::size_t idx ) const
    {
        if ( !is_leaf() || idx >= size() ) return std::nullopt;
        return m_values[idx];
    }


};

};