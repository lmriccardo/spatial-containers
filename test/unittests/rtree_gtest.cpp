#include <gtest/gtest.h>
#include "sc/structures/rtree.hpp"

using namespace sc;
using RTree = sc::rtree<int, double, 2>;
using box2  = bbox2d<double>;

// ------------------------------------------------------------
// Helper for convenience
// ------------------------------------------------------------
static box2 make_box(double x1, double y1, double x2, double y2)
{
    return box2{{x1, y1}, {x2, y2}};
}

// ------------------------------------------------------------
// 1. Insert first element → creates root as leaf
// ------------------------------------------------------------
TEST(RTreeTest, InsertCreatesRoot)
{
    RTree tree(4);
    box2 b = make_box(0, 0, 1, 1);

    tree.insert(b, 42);

    auto result = tree.search(b);
    ASSERT_EQ(result.size(), 1);
    EXPECT_EQ(result[0], 42);
}

// 2. Insert two values → root is still leaf, both should be found
// ------------------------------------------------------------
TEST(RTreeTest, InsertTwoValuesIntoLeaf)
{
    RTree tree(4);
    tree.insert(make_box(0,0, 1,1), 1);
    tree.insert(make_box(2,2, 3,3), 2);

    auto all = tree.search(make_box(-10,-10, 10,10));
    ASSERT_EQ(all.size(), 2);
    EXPECT_TRUE(std::find(all.begin(), all.end(), 1) != all.end());
    EXPECT_TRUE(std::find(all.begin(), all.end(), 2) != all.end());
}

// ------------------------------------------------------------
// 3. Searching regions that include/exclude entries
// ------------------------------------------------------------
TEST(RTreeTest, SearchSelectsOnlyOverlappingValues)
{
    RTree tree(4);
    tree.insert(make_box(0,0, 1,1), 10);
    tree.insert(make_box(5,5, 6,6), 20);

    auto r1 = tree.search(make_box(-1,-1, 2,2));
    ASSERT_EQ(r1.size(), 1);
    EXPECT_EQ(r1[0], 10);

    auto r2 = tree.search(make_box(4,4, 7,7));
    ASSERT_EQ(r2.size(), 1);
    EXPECT_EQ(r2[0], 20);

    auto r3 = tree.search(make_box(100,100, 200,200));
    ASSERT_TRUE(r3.empty());
}

// ------------------------------------------------------------
// 4. Searching an empty tree returns empty vector
// ------------------------------------------------------------
TEST(RTreeTest, SearchEmptyTreeReturnsEmpty)
{
    RTree tree(4);
    auto r = tree.search( make_box(0,0, 1,1) );
    EXPECT_TRUE(r.empty());
}

// ------------------------------------------------------------
// 5. choose_leaf returns root when tree has only leaf root
// ------------------------------------------------------------
TEST(RTreeTest, ChooseLeafOnRoot)
{
    RTree tree(4);
    box2 b = make_box(0,0, 1,1);

    tree.insert(b, 7);

    // access private through friend if needed; otherwise test indirectly:
    auto result = tree.search(b);
    ASSERT_EQ(result.size(), 1);
    EXPECT_EQ(result[0], 7);
}

// ------------------------------------------------------------
// 6. Insert multiple values → still no split → leaf size correct
// ------------------------------------------------------------
TEST(RTreeTest, LeafStoresAllValuesBeforeSplit)
{
    RTree tree(4);
    tree.insert(make_box(0,0,1,1), 1);
    tree.insert(make_box(1,1,2,2), 2);
    tree.insert(make_box(2,2,3,3), 3);
    tree.insert(make_box(3,3,4,4), 4);

    auto all = tree.search(make_box(-10,-10, 10,10));
    ASSERT_EQ(all.size(), 4);
}