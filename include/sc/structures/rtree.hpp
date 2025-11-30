#pragma once

#include "rtree_node.hpp"

namespace sc
{
/**
 * @brief
 */
template<class V, Numeric T, std::size_t N>
class rtree
{
private:
    using node_t = rtree_node<V,T,N>;
    using node_pointer = node_t*;
    using node_cpointer = node_t const*;
    using box_t = bbox<T,N>;

    std::unique_ptr<node_t> m_root; // The root node of the rtree
    std::size_t m_maxEntries;       // Maximum number of entries per-node
    std::size_t m_minEntries;       // Minimum number of entries per-node

    /**
     * Choose the best leaf where to inser the new value. Among all nodes
     * the best one is always chosen among those that provides the least
     * grows in their own bounding box so to fit the input one.
     */
    node_pointer choose_leaf( node_pointer node, box_t const& in_box ) const
    {
        // If the current node is a leaf than we can return it
        if ( node->is_leaf() ) return node;

        // Otherwise, pick the node whose bbox grows the least
        node_pointer best = node->pick_best( in_box );
        return choose_leaf( best, in_box );
    }

public:
    rtree( std::size_t M ) : m_maxEntries(M), m_minEntries( std::size_t{M / 2} ) {}
    rtree( rtree const& ) = delete;
    rtree( rtree&& ) = default;

    rtree& operator=( rtree const& ) = delete;
    rtree& operator=( rtree&& ) = default;

    ~rtree() = default;

    /**
     * @brief Searches the R-tree for all values whose bounding boxes overlap a query.
     *
     * Performs a non-recursive depth-first search starting from the root. Internal nodes 
     * are traversed if their bounding boxes overlap the query, and leaf node values that 
     * overlap the query are collected in the result.
     *
     * @param query The bounding box to search for overlapping entries.
     * 
     * @return A vector of values id any.
     */
    std::vector<V> search( box_t const& query ) const noexcept
    {
        // Check if the root exists and it is not empty
        if ( m_root == nullptr ) return {};

        std::vector<V> result; // Initialize the return value
        std::vector<node_cpointer> stack = { m_root.get() };

        // Apply the non-recursive DFS for searching
        while (!stack.empty())
        {
            node_cpointer curr_node = stack.back(); stack.pop_back();
            
            // If the node is a leaf node than we need to search for values
            if ( curr_node->is_leaf() )
            {
                for ( auto const& value: curr_node->search_leafs( query ) )
                {
                    result.push_back( value );
                }

                continue;
            }

            for ( auto const& child_ptr: curr_node->search_childs( query ) )
            {
                stack.push_back( child_ptr );
            }
        }

        return result;
    }

    /**
     * 
     */
    void insert( box_t const& mbr, V const& value ) noexcept
    {
        // Check if the root exists and it is not empty. If it does not
        // then we create it as a leaf root node
        if ( m_root == nullptr )
        {
            m_root = node_t::make( mbr );
            m_root->add( value );
            return;
        }

        // Choose the best leaf that would fit this new bounding box
        node_pointer best_leaf = choose_leaf( m_root.get(), mbr );
        best_leaf->add( mbr, value );
        if ( best_leaf->size() < m_maxEntries ) return;

        // Perform splitting node. Either linear, quadratic or r*
    }
};

};