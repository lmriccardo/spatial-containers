#include <gtest/gtest.h>
#include "sc/structures/rtree.hpp"

using namespace sc;

TEST(RTreeTest, Construction) {
    rtree_node<int, int, 3> t(1, 2, 1, 2, 3, 4);
}