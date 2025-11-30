#include <gtest/gtest.h>
#include <vector>
#include <tuple>
#include <string>
#include <algorithm>
#include <cmath>

#include "sc/utils/functools.hpp"

using namespace sc;

// ================================================================
// for_each_pair Tests
// ================================================================

TEST(ForEachPair, SimplePairs)
{
    std::vector<std::pair<int,int>> out;

    for_each_pair(
        [&](auto a, auto b){ out.emplace_back(a,b); },
        1, 2,   3, 4,   5, 6
    );

    ASSERT_EQ(out.size(), 3);
    EXPECT_EQ(out[0], (std::pair{1,2}));
    EXPECT_EQ(out[1], (std::pair{3,4}));
    EXPECT_EQ(out[2], (std::pair{5,6}));
}

TEST(ForEachPair, WorksWithDifferentTypes)
{
    std::vector<std::string> out;

    for_each_pair(
        [&](auto a, auto b){
            out.push_back(std::to_string(a) + ":" + b);
        },
        1, "A",   2, "B",   3, "C"
    );

    ASSERT_EQ(out.size(), 3);
    EXPECT_EQ(out[0], "1:A");
    EXPECT_EQ(out[1], "2:B");
    EXPECT_EQ(out[2], "3:C");
}

TEST(ForEachPair, MutatesExternalState)
{
    int sum = 0;

    for_each_pair(
        [&](auto a, auto b){ sum += a * b; },
        1, 2, 3, 4
    );

    EXPECT_EQ(sum, 1*2 + 3*4);
}

// ================================================================
// min(vec, proj, comp) Tests
// ================================================================

TEST(MinProj, FindsMinimumByProjection)
{
    std::vector<std::pair<int,int>> v{
        {10,5}, {2,1}, {4,9}, {0,5}
    };

    auto res = min(
        v,
        [](auto p){ return p.first + p.second; },
        std::less<>()
    );

    EXPECT_EQ(res.first, 2);
    EXPECT_EQ(res.second, 1);
}

TEST(MinProj, CustomProjectionWithArgs)
{
    std::vector<int> v{10, 3, 6, 1};

    auto res = min(
        v,
        [](int x, int offset){ return std::abs(x - offset); },
        std::less<>(),
        5         // projection argument
    );

    // distance from 5: {5,2,1,4} -> min is 6
    EXPECT_EQ(res, 6);
}

TEST(MinProj, HandlesSingleElementVector)
{
    std::vector<int> v{42};

    auto res = min(v, std::identity{}, std::less<>{});

    EXPECT_EQ(res, 42);
}

// ================================================================
// min(vec, comp) Tests
// ================================================================

TEST(MinSimple, DefaultIdentityMin)
{
    std::vector<int> v{9, 2, 5, 1, 7};

    auto res = min(v, std::less<>{});

    EXPECT_EQ(res, 1);
}

TEST(MinSimple, MaxUsingGreaterComp)
{
    std::vector<int> v{9, 2, 5, 1, 7};

    auto res = min(v, std::greater<>{});

    EXPECT_EQ(res, 9);
}

// ================================================================
// compare_put Tests
// ================================================================

struct Point3 : public std::array<int,3> {};

TEST(ComparePut, ElementwiseComparison)
{
    Point3 p1{1, 50, 3};
    Point3 p2{2, 20, 4};

    auto result = compare_put(p1, p2,
        [](int a, int b){ return std::min(a,b); }
    );

    EXPECT_EQ(result[0], 1);
    EXPECT_EQ(result[1], 20);
    EXPECT_EQ(result[2], 3);
}

TEST(ComparePut, ElementwiseMax)
{
    Point3 p1{1, 5, 3};
    Point3 p2{2, 1, 4};

    auto result = compare_put(p1, p2,
        [](int a, int b){ return std::max(a,b); }
    );

    EXPECT_EQ(result[0], 2);
    EXPECT_EQ(result[1], 5);
    EXPECT_EQ(result[2], 4);
}
