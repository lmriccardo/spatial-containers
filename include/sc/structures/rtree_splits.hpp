#pragma once

#include <vector>
#include <unordered_map>
#include <limits>

#include "rtree_node.hpp"
#include "sc/utils/traits.hpp"

namespace sc
{

struct quadratic_split
{
    template<class V, Numeric T, std::size_t N>
    using entry_type = rtree_node<V,T,N>::leaf_type;

    template<class V, Numeric T, std::size_t N>
    static auto split( std::vector<entry_type<V,T,N>>& entries, size_t min_fill )
    -> std::pair<std::vector<entry_type<V,T,N>>,std::vector<entry_type<V,T,N>>>
    {
        using entry_t = entry_type<V,T,N>;

        auto [ seed0, seed1 ] = pick_seeds( entries );
        std::vector<bool> used( entries.size(), false );
        used[seed0] = true;
        used[seed1] = true;

        std::vector<entry_t> group1, group2;
        group1.push_back( entries[seed0] );
        group2.push_back( entries[seed1] );

        bbox<T,N> box1 = entries[seed0].box;
        bbox<T,N> box2 = entries[seed1].box;

        size_t remaining_size = entries.size() - 2;
        while ( remaining_size > 0 )
        {
            T max_diff = std::numeric_limits<T>::lowest();
            int pick = -1;
            bool choose_g1 = true;

            for ( std::size_t k = 0; k < entries.size(); ++k )
            {
                if ( used[k] ) continue;

                // Check if we can reach the minimum number of children
                // just for the group1 or group2
                if ( group1.size() + remaining_size == min_fill )
                {
                    group1.push_back( entries[k] );
                    used[k] = true;
                    box1 = box1.merge( entries[k].box );
                    continue;
                }

                if ( group2.size() + remaining_size == min_fill )
                {
                    group2.push_back( entries[k] );
                    used[k] = true;
                    box2 = box2.merge( entries[k].box );
                    continue;
                }

                T d_group1 = enlargement( box1, entries[k].box );
                T d_group2 = enlargement( box2, entries[k].box );
                T diff = d_group1 > d_group2 ? ( d_group1 - d_group2 )
                    : ( d_group2 - d_group1 );

                if ( diff > max_diff )
                {
                    max_diff = diff;
                    pick = k;
                    choose_g1 = ( d_group1 < d_group2 );
                }
            }

            used[pick] = true;
            remaining_size--;

            if ( choose_g1 ) {
                group1.push_back( entries[pick] );
                box1 = box1.merge( entries[k].box );
            } else {
                group2.push_back( entries[k] );
                box2 = box2.merge( entries[k].box );
            }
        }
    }

private:
    /**
     * @brief Quadratic version of the peek seeds algorithm
     * 
     * It searches for the two most non-overlapping entries among all values for 
     * that leaf node and returns the indices of the two entries.
     */
    template<class V, Numeric T, std::size_t N>
    static auto pick_seeds( std::vector<entry_type<V,T,N>> const& entries )
    {
        std::size_t nof_entries = entries.size();
        T max_waste = std::numeric_limits<T>::lowest();
        std::pair<int,int> seeds{0, 1};

        for ( size_t i = 0; i < nof_entries; ++i )
        for ( size_t j = i; j < nof_entries; ++j )
        {
            const auto box3 = sc::merge( entries[i].box, entries[j].box );
            T hvol = box3.hvolume() 
                   - entries[i].box.hvolume() 
                   - entries[j].box.hvolume();

            if ( hvol >= max_waste )
            {
                max_waste = hvol;
                seeds = {i1, i2};
            }
        }

        return seeds;
    }
};

struct linear_split
{
    template<class V, Numeric T, std::size_t N>
    static auto split( std::vector<typename rtree_node<V,T,N>::leaf_type>& entries )
    {
        
    }
};

struct rstar_split
{
    template<class V, Numeric T, std::size_t N>
    static auto split( std::vector<typename rtree_node<V,T,N>::entry_type>& entries )
    {
        
    }
};

};