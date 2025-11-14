#include <gtest/gtest.h>
#include "sc/primitives/point.hpp"

using namespace sc;

// ==================== pointnd Tests ====================
TEST(PointNDTest, DefaultConstructor) {
    pointnd<int, 3> p;
    EXPECT_EQ(p[0], 0);
    EXPECT_EQ(p[1], 0);
    EXPECT_EQ(p[2], 0);
}

TEST(PointNDTest, ValueConstructorAndSet) {
    pointnd<int, 3> p(5); // fill all with 5
    EXPECT_EQ(p[0], 5);
    EXPECT_EQ(p[1], 5);
    EXPECT_EQ(p[2], 5);

    p.set(0, 10);
    p.set(1, 20);
    p.set(2, 30);
    EXPECT_EQ(p[0], 10);
    EXPECT_EQ(p[1], 20);
    EXPECT_EQ(p[2], 30);

    p.set(7); // fill all with 7
    for ( auto const& v: p )
    { EXPECT_EQ(v, 7); }
}

TEST(PointNDTest, CopyAndMove) {
    pointnd<int, 3> p1(1);
    pointnd<int, 3> p2(p1); // copy constructor
    EXPECT_EQ(p2[0], 1);
    EXPECT_EQ(p2[1], 1);
    EXPECT_EQ(p2[2], 1);

    pointnd<int, 3> p3(std::move(p1)); // move constructor
    EXPECT_EQ(p3[0], 1);
    EXPECT_EQ(p3[1], 1);
    EXPECT_EQ(p3[2], 1);

    pointnd<int, 3> p4;
    p4 = p3; // copy assignment
    EXPECT_EQ(p4[0], 1);
    EXPECT_EQ(p4[1], 1);
    EXPECT_EQ(p4[2], 1);
}

// ==================== point2d Tests ====================
TEST(Point2DTest, DefaultAndValueConstructor) {
    point2d<int> p;
    EXPECT_EQ(p.x(), 0);
    EXPECT_EQ(p.y(), 0);

    point2d<int> p_val(5);
    EXPECT_EQ(p_val.x(), 5);
    EXPECT_EQ(p_val.y(), 5);

    point2d<int> p_two(2, 3);
    EXPECT_EQ(p_two.x(), 2);
    EXPECT_EQ(p_two.y(), 3);

    // Modify values
    p_two.x() = 10;
    p_two.y() = 20;
    EXPECT_EQ(p_two.x(), 10);
    EXPECT_EQ(p_two.y(), 20);
}

TEST(Point2DTest, CopyAndMove) {
    point2d<int> p(1, 2);
    point2d<int> p_copy(p);
    EXPECT_EQ(p_copy.x(), 1);
    EXPECT_EQ(p_copy.y(), 2);

    point2d<int> p_move(std::move(p));
    EXPECT_EQ(p_move.x(), 1);
    EXPECT_EQ(p_move.y(), 2);

    point2d<int> p_assign;
    p_assign = p_copy;
    EXPECT_EQ(p_assign.x(), 1);
    EXPECT_EQ(p_assign.y(), 2);
}

// ==================== point3d Tests ====================
TEST(Point3DTest, ConstructorsAndAccessors) {
    point3d<int> p;
    EXPECT_EQ(p.x(), 0);
    EXPECT_EQ(p.y(), 0);
    EXPECT_EQ(p.z(), 0);

    point3d<int> p_val(5);
    EXPECT_EQ(p_val.x(), 5);
    EXPECT_EQ(p_val.y(), 5);
    EXPECT_EQ(p_val.z(), 5);

    point3d<int> p_three(1, 2, 3);
    EXPECT_EQ(p_three.x(), 1);
    EXPECT_EQ(p_three.y(), 2);
    EXPECT_EQ(p_three.z(), 3);

    // Modify values
    p_three.x() = 10;
    p_three.y() = 20;
    p_three.z() = 30;
    EXPECT_EQ(p_three.x(), 10);
    EXPECT_EQ(p_three.y(), 20);
    EXPECT_EQ(p_three.z(), 30);
}

TEST(Point3DTest, CopyAndMove) {
    point3d<int> p(1, 2, 3);
    point3d<int> p_copy(p);
    EXPECT_EQ(p_copy.x(), 1);
    EXPECT_EQ(p_copy.y(), 2);
    EXPECT_EQ(p_copy.z(), 3);

    point3d<int> p_move(std::move(p));
    EXPECT_EQ(p_move.x(), 1);
    EXPECT_EQ(p_move.y(), 2);
    EXPECT_EQ(p_move.z(), 3);

    point3d<int> p_assign;
    p_assign = p_copy;
    EXPECT_EQ(p_assign.x(), 1);
    EXPECT_EQ(p_assign.y(), 2);
    EXPECT_EQ(p_assign.z(), 3);
}

// ==================== Constexpr test ====================
TEST(PointConstexprTest, CompileTimeInitialization) {
    constexpr pointnd<int, 2> pn(7);
    static_assert(pn[0] == 7 && pn[1] == 7, "pointnd constexpr failed");

    constexpr point2d<int> p2(7); // works if you use accessor methods
    static_assert(p2.x() == 7 && p2.y() == 7, "point2d constexpr failed");

    constexpr point3d<int> p3(7);
    static_assert(p3.x() == 7 && p3.y() == 7 && p3.z() == 7, "point3d constexpr failed");
}
