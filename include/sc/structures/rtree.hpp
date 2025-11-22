#pragma once

#include <memory>
#include <optional>
#include <vector>

#include "sc/primitives/bbox.hpp"
#include "sc/utils/traits.hpp"
#include "sc/utils/functools.hpp"

namespace sc
{

/**
 * @brief
 */
template<class V, Numeric T, std::size_t N>
class rtree_node
{
private:
    using value_t  = std::optional<V>;
    using self_t   = std::unique_ptr<rtree_node>;
    using box_t    = bbox<T,N>;
    using pointer  = rtree_node*;
    using cpointer = rtree_node const*;

    // rTree Node internal type for non-leaf nodes
    struct internal_type
    {
        box_t  box;   // The Bounding Box of the child
        self_t child; // A pointer to the actual child

        cpointer operator*() const { return child.get(); };
    };

    struct leaf_type
    {
        box_t box;   // The Bounding Box of the last node
        V     value; // The value of the last node

        V const& operator*() const { return value; }
    };

    box_t   m_box;    // An N-dimensional rectangle (the bounding box)
    bool    m_isLeaf; // Whether or not the current node is a leaf

    std::vector<internal_type> m_childs; // A vector of all children of this node
    std::vector<leaf_type>     m_values; // A vector of all values for this node (if leaf)

    /**
     * Unified search functions for both leaf and internal nodes.
     */
    template<class _Tp> static auto
    search( std::vector<_Tp> const& search_v, box_t const& in_box )
    {
        // First we need to check if there is at least one element
        if ( search_v.empty() ) return {};

        using __Tp = decltype(*search_v.front());
        std::vector<__Tp> result;
        result.reserve( search_v.size() );

        for ( auto const& element: search_v ) {
            if ( in_box.overlaps( element.box ) ) {
                result.push_back( *element );
            }
        }

        return result;
    }

public:
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
     * @brief Construct an rTree Node with the input bbox.
     */
    rtree_node( bbox<T,N> const& bbox ) : m_box(bbox) {}

    /**
     * @brief Constructor overload for moving the bounding box
     */
    rtree_node( bbox<T,N>&& bbox ) : m_box(std::move(bbox)) {}

    // Copy operator is deleted given the vector of unique pointers
    rtree_node& operator=( rtree_node const& other ) = delete;
    rtree_node& operator=( rtree_node&& other ) = default;

    /**
     * @brief Tells if the current node is a leaf or not
     */
    bool is_leaf() const noexcept 
    { return !m_values.empty() && m_childs.empty(); }

    /**
     * @brief Searches for a list of colliding children if the node is an internal
     * node or a list of values if the node is a leaf node.
     * 
     * Returns a vector of children/values whose bounding box collides
     * with the one of the input node.
     * 
     * @param in_box The input query bounding box region
     */
    auto search( bbox<T,N> const& in_box ) const
    {
        if ( is_leaf() ) return search( m_values, in_box );
        return search( m_childs, in_box );
    }

    /**
     * @brief From among all children picks the one whose bbox grows
     * the least to fit the input bounding box.
     * 
     * @param in_box The input query bounding box region
     */
    pointer pick_best( box_t const& in_box ) const
    {
        auto _proj_fn = []( internal_type const& t, box_t const& box )
        { return t.box.enlargement(box); };
        return *sc::min(m_childs, _proj_fn, std::less<>{}, in_box);
    }
};

/**
 * @brief
 */
template<class V, Numeric T, std::size_t N>
class rtree
{
private:
    using node_t = rtree_node<V,T,N>;
    using node_pointer = node_t*;
    using box_t = bbox<T,N>;

    std::unique_ptr<node_t> m_root; // The root node of the rtree

    node_pointer choose_leaf( node_pointer node, box_t const& in_box ) const
    {
        // If the current node is a leaf than we can return it
        if ( node->is_leaf() ) return node;

        // Otherwise, pick the node whose bbox grows the least
        node_pointer best = node->pick_best( in_box );
        return choose_leaf( best, in_box );
    }

public:

};

};