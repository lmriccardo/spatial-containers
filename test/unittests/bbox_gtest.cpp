#include <gtest/gtest.h>
#include <string>
#include "sc/primitives/bbox.hpp"

using namespace sc;

TEST(BBoxTest, ConstructionNumericType) {
    // Valid numeric type
    bbox<int, 4> box_int;
    bbox<double, 3> box_double;
    bbox<float, 3> box_float;
    bbox<std::size_t, 3> box_size_t;
}

TEST(BBoxTest, ConstructionFromPoint2D) {
    // Valid numeric type
    point2d<int> min{3};
    point2d<int> max{5};
    bbox<int, 2> box_int( min, max );
}

TEST(BBoxTest, ConstructionFromPoint3D) {
    // Valid numeric type
    point3d<int> min{3};
    point3d<int> max{5};
    bbox<int, 3> box_int( min, max );
}

TEST(BBoxTest, CopyOperator) {
    constexpr std::size_t Dim = 10;

    // Valid numeric type
    pointnd<int, Dim> min{3};
    pointnd<int, Dim> max{5};
    
    bbox<int, Dim> box_int( min, max );

    bbox<int, Dim> box_int_2;
    box_int_2 = box_int;
}

TEST(BBoxTest, CopyConstructor) {
    constexpr std::size_t Dim = 10;

    // Valid numeric type
    pointnd<int, Dim> min{3};
    pointnd<int, Dim> max{5};
    
    bbox<int, Dim> box_int( min, max );
    bbox<int, Dim> box_int_2(box_int);
}

TEST(BBoxTest, MoveOperator) {
    constexpr std::size_t Dim = 10;

    // Valid numeric type
    pointnd<int, Dim> min{3};
    pointnd<int, Dim> max{5};

    bbox<int, Dim> box_int( min, max );

    bbox<int, Dim> box_int_2;
    box_int_2 = std::move(box_int);
}

TEST(BBoxTest, MoveConstructor) {
    constexpr std::size_t Dim = 10;

    // Valid numeric type
    pointnd<int, Dim> min{3};
    pointnd<int, Dim> max{5};
    
    bbox<int, Dim> box_int( min, max );
    bbox<int, Dim> box_int_2(std::move(box_int));
}

TEST(BBoxTest, PointNDContains) {
    pointnd<int, 4> p{1, 2, 3, 4};
    bbox<int, 4> box({0, 0, 0, 0}, {5, 5, 5, 5});

    EXPECT_TRUE(box.contains(p));

    pointnd<int, 4> outside{6, 2, 3, 4};
    EXPECT_FALSE(box.contains(outside));
}

TEST(BBoxTest, Point2DContains) {
    point2d<int> p{3, 4};
    bbox<int, 2> box({0, 0}, {5, 5});

    EXPECT_TRUE(box.contains(p));

    point2d<int> outside{6, 1};
    EXPECT_FALSE(box.contains(outside));
}

TEST(BBoxTest, Point3DContains) {
    point3d<int> p{2, 3, 4};
    bbox<int, 3> box({0, 0, 0}, {5, 5, 5});

    EXPECT_TRUE(box.contains(p));

    point3d<int> outside{1, 6, 2};
    EXPECT_FALSE(box.contains(outside));
}

TEST(BBoxTest, BBoxContainsBBox) {
    bbox<int, 2> outer({0, 0}, {10, 10});
    bbox<int, 2> inner({2, 3}, {5, 6});
    bbox<int, 2> overlapping({5, 5}, {12, 12});

    EXPECT_TRUE(outer.contains(inner));
    EXPECT_FALSE(outer.contains(overlapping));
}

TEST(BBoxTest, BBoxContainsPointAndBBoxMixed) {
    bbox<int, 3> box({0, 0, 0}, {10, 10, 10});
    point3d<int> inside_point{5, 5, 5};
    point3d<int> outside_point{11, 5, 5};
    bbox<int, 3> inner_box({1,1,1}, {2,2,2});
    bbox<int, 3> outer_box({5,5,5}, {12,12,12});

    EXPECT_TRUE(box.contains(inside_point));
    EXPECT_FALSE(box.contains(outside_point));
    EXPECT_TRUE(box.contains(inner_box));
    EXPECT_FALSE(box.contains(outer_box));
}

TEST(BBoxTest, MergeWithPoint2D) {
    bbox2d<int> box({1, 2}, {4, 5});
    point2d<int> pt{3, 6};

    auto merged = box.merge(pt);

    EXPECT_EQ(merged.min()[0], 1);
    EXPECT_EQ(merged.min()[1], 2);
    EXPECT_EQ(merged.max()[0], 4);
    EXPECT_EQ(merged.max()[1], 6);

    // original box not modified
    EXPECT_EQ(box.max()[1], 5);
}

TEST(BBoxTest, MergeWithBBox3D) {
    bbox3d<int> box1({0, 1, 2}, {3, 4, 5});
    bbox3d<int> box2({-1, 2, 0}, {2, 6, 7});

    auto merged = box1.merge(box2);

    EXPECT_EQ(merged.min()[0], -1);
    EXPECT_EQ(merged.min()[1], 1);
    EXPECT_EQ(merged.min()[2], 0);

    EXPECT_EQ(merged.max()[0], 3);
    EXPECT_EQ(merged.max()[1], 6);
    EXPECT_EQ(merged.max()[2], 7);

    // original boxes unchanged
    EXPECT_EQ(box1.min()[0], 0);
    EXPECT_EQ(box2.max()[2], 7);
}

// --- Expand Tests (in-place modification) ---

TEST(BBoxTest, ExpandWithPoint2D) {
    bbox2d<int> box({1, 2}, {4, 5});
    point2d<int> pt{0, 6};

    box.expand(pt);

    EXPECT_EQ(box.min()[0], 0);
    EXPECT_EQ(box.min()[1], 2);
    EXPECT_EQ(box.max()[0], 4);
    EXPECT_EQ(box.max()[1], 6);
}

TEST(BBoxTest, ExpandWithBBox3D) {
    bbox3d<int> box1({0, 1, 2}, {3, 4, 5});
    bbox3d<int> box2({-2, 0, 3}, {2, 6, 7});

    box1.expand(box2);

    EXPECT_EQ(box1.min()[0], -2);
    EXPECT_EQ(box1.min()[1], 0);
    EXPECT_EQ(box1.min()[2], 2);

    EXPECT_EQ(box1.max()[0], 3);
    EXPECT_EQ(box1.max()[1], 6);
    EXPECT_EQ(box1.max()[2], 7);
}

TEST(BBoxTest, Enlargement2D) {
    bbox2d<int> box({0, 0}, {2, 2});
    bbox2d<int> same({0, 0}, {2, 2});
    bbox2d<int> larger({1, 1}, {4, 5});
    bbox2d<int> outside({3, 0}, {5, 2});

    EXPECT_EQ(box.enlargement(same), 0);
    EXPECT_EQ(box.enlargement(larger), 16);
    EXPECT_EQ(box.enlargement(outside), 6);
}

TEST(BBoxTest, Enlargement3D) {
    bbox3d<int> box({0,0,0}, {2,2,2});
    bbox3d<int> overlap({1,1,1}, {3,3,3});

    EXPECT_EQ(box.enlargement(overlap), 27 - 8);

    bbox3d<int> contained({0,0,0}, {1,1,1});
    EXPECT_EQ(box.enlargement(contained), 0);
}

TEST(BBoxTest, EnlargementWithPoint) {
    bbox2d<int> box({0,0}, {2,2});
    bbox2d<int> pt({3,3}, {3,3});
    EXPECT_EQ(box.enlargement(pt), 5);
}

TEST(BBoxTest, EnlargementNoChange) {
    bbox2d<int> box({0,0}, {2,2});
    bbox2d<int> inside({1,1}, {2,2});
    EXPECT_EQ(box.enlargement(inside), 0);
}
