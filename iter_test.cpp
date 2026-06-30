#include "concat.hpp"
#include <algorithm>
#include <concepts>
#include <forward_list>
#include <gtest/gtest.h>
#include <iterator>
#include <list>
#include <print>
#include <ranges>
#include <vector>
#include <type_traits>

TEST(ConcatNormalIter, PrecrementInRange) {
    std::list<std::string> a{"aaa", "fdfsfsfs", "zzz"}, b{"Dsdasd", "dsssss", "dscc", "ds"};
    concat_view view{a, b};

    auto iter = view.begin();
    ++iter;
    ++iter;
    ASSERT_EQ(*iter, "zzz");
}

TEST(ConcatNormalIter, PrecrementBetweenRange) {
    std::forward_list<int> a{1, 2, 3}, b{9, 10, 11}, c{10, 11, 12};
    concat_view view{a, b, c};

    auto iter = view.begin();
    ++iter;
    ++iter;
    ++iter;
    ASSERT_EQ(*iter, 9);
}

TEST(ConcatNormalIter, PrecrementToEnd) {
    std::vector<int> a{1, 2, 3}, b{9, 9};
    concat_view view{a, b};

    auto iter = view.begin();
    ++iter;
    ++iter;
    ++iter;
    ++iter;
    ++iter;
    ASSERT_EQ(iter, view.end());
}

TEST(ConcatNormalIter, DecrementInRange) {
    std::vector<int> a{1, 2, 3}, b{9, 9};
    concat_view view{a, b};

    auto iter = view.begin();
    ++iter; ++iter;
    --iter; --iter;
    ASSERT_EQ(iter, view.begin());
}

TEST(ConcatNormalIter, DecrementBetweenRange) {
    std::vector<int> a{1, 2, 3}, b{9, 9};
    concat_view view{a, b};

    auto iter = view.begin();
    ++iter;
    ++iter;
    ++iter;
    --iter;
    ASSERT_EQ(*iter, 3);
}

TEST(ConcatNormalIter, AdvanceByInRange) {
    std::vector<int> a{1, 2, 3, 4}, b{9, 10, 11};
    concat_view view(a, b);

    auto iter = view.begin();
    iter += 3;
    ASSERT_EQ(*iter, 4);
}

TEST(ConcatNormalIter, AdvanceThroughOneRange) {
    std::vector<int> a{1, 2, 3, 4}, b{9, 10, 11};
    concat_view view(a, b);

    auto iter = view.begin();
    iter += 4;
    ASSERT_EQ(*iter, 9);
}

TEST(ConcatNormalIter, AdvanceThroughSeveralRanges) {
    std::vector<int> a{1, 2, 3, 4}, b{9, 10, 11}, c{20, 21, 22, 34};
    concat_view view(a, b, c);

    auto iter = view.begin();
    iter += 10;
    ASSERT_EQ(*iter, 34);
}

TEST(ConcatNormalIter, AdvanceToEndThroughSeveralRanges) {
    std::vector<int> a{1, 2, 3, 4}, b{9, 10, 11}, c{20, 21, 22, 34};
    concat_view view(a, b, c);

    auto iter = view.begin();
    iter += 11;
    ASSERT_EQ(iter, view.end());
}

TEST(ConcatNormalIter, AdvanceByNegative) {
    std::vector<int> a{1, 2, 3, 4}, b{9, 10, 11}, c{20, 21, 22, 34};
    concat_view view(a, b, c);

    auto iter = view.begin();
    iter += 4;
    iter += -4;
    ASSERT_EQ(iter, view.begin());
}

TEST(ConcatNormalIter, DecrementByInRange) {
    std::vector<int> a{1, 2, 3, 4}, b{9, 10, 11};
    concat_view view(a, b);

    auto iter = view.begin();
    iter += 3;
    iter -= 3;
    ASSERT_EQ(iter, view.begin());
}

TEST(ConcatNormalIter, DecrementByThroughOneRange) {
    std::vector<int> a{1, 2, 3, 4}, b{9, 10, 11};
    concat_view view(a, b);

    auto iter = view.begin();
    iter += 5;
    iter -= 5;
    ASSERT_EQ(iter, view.begin());
}

TEST(ConcatNormalIter, DecrementByThroughSeveralRanges) {
    std::vector<int> a{1, 2, 3, 4}, b{9, 10, 11}, c{20, 21, 22, 34};
    concat_view view(a, b, c);

    auto iter = view.begin();
    iter += 8;
    iter -= 8;
    ASSERT_EQ(iter, view.begin());
}

TEST(ConcatNormalIter, DecrementByNegative) {
    std::vector<int> a{1, 2, 3, 4}, b{9, 10, 11}, c{20, 21, 22, 34};
    concat_view view(a, b, c);

    auto iter = view.begin();
    iter -= -7;
    ASSERT_EQ(*iter, 20);
}

TEST(ConcatNormalIter, BracketsForward) {
    std::vector<int> a{1, 2, 3, 4}, b{9, 10, 11}, c{20, 21, 22, 34};
    concat_view view(a, b, c);

    auto iter = view.begin();
    ASSERT_EQ(iter[8], 21);
}

TEST(ConcatNormalIter, BracketsBackward) {
    std::vector<int> a{1, 2, 3, 4}, b{9, 10, 11}, c{20, 21, 22, 34};
    concat_view view(a, b, c);

    auto iter = view.end();
    ASSERT_EQ(iter[-view.size()], 1);
}

TEST(ConcatNormalIter, IterSubIter) {
    std::vector<int> a{1, 2, 3, 4}, b{9, 10, 11}, c{20, 21, 22, 34};
    concat_view view(a, b, c);

    ASSERT_EQ(view.end() - view.begin(), view.size());
}

TEST(ConcatNormalIter, IterSubIterOneRange) {
    std::vector<int> a{1, 2, 3, 4}, b{9, 10, 11}, c{20, 21, 22, 34};
    concat_view view(a, b, c);

    auto iter = view.begin();
    iter += 3;
    ASSERT_EQ(iter - view.begin(), 3);
}

TEST(ConcatNormalIter, Comparison) {
    std::vector<int> a{1, 2, 3, 4}, b{9, 10, 11}, c{20, 21, 22, 34};
    concat_view view(a, b, c);

    auto beg = view.begin();
    auto end = view.end();

    ASSERT_EQ(beg, beg);
    ASSERT_NE(beg, end);
    ASSERT_EQ(end, end);
}

TEST(ConcatNormalIter, LessThanInRange) {
    std::vector<int> a{1, 2, 3, 4}, b{9, 10, 11}, c{20, 21, 22, 34};
    concat_view view(a, b, c);

    auto iter = view.begin();
    iter += 2;
    ASSERT_LT(view.begin(), iter);
}

TEST(ConcatNormalIter, LessThanBetweenRange) {
    std::vector<int> a{1, 2, 3, 4}, b{9, 10, 11}, c{20, 21, 22, 34};
    concat_view view(a, b, c);

    auto iter = view.begin();
    iter += 5;
    ASSERT_LT(view.begin(), iter);
}

TEST(ConcatNormalIter, LessThanBetweenRangeOtherWayAround) {
    std::vector<int> a{1, 2, 3, 4}, b{9, 10, 11}, c{20, 21, 22, 34};
    concat_view view(a, b, c);

    auto iter = view.begin();
    iter += 5;
    ASSERT_FALSE(iter < view.begin());
}


TEST(ConcatNormalIter, LessEqualThanInRange) {
    std::vector<int> a{1, 2, 3, 4}, b{9, 10, 11}, c{20, 21, 22, 34};
    concat_view view(a, b, c);

    auto iter = view.begin();
    iter += 2;
    ASSERT_LE(view.begin(), iter);
}

TEST(ConcatNormalIter, LessEqualThanBetweenRange) {
    std::vector<int> a{1, 2, 3, 4}, b{9, 10, 11}, c{20, 21, 22, 34};
    concat_view view(a, b, c);

    auto iter = view.begin();
    iter += 5;
    ASSERT_LE(view.begin(), iter);
}

TEST(ConcatNormalIter, LessEqualThanBetweenRangeOtherWayAround) {
    std::vector<int> a{1, 2, 3, 4}, b{9, 10, 11}, c{20, 21, 22, 34};
    concat_view view(a, b, c);

    auto iter = view.begin();
    iter += 5;
    ASSERT_FALSE(iter <= view.begin());
}


TEST(ConcatNormalIter, GreaterThanInRange) {
    std::vector<int> a{1, 2, 3, 4}, b{9, 10, 11}, c{20, 21, 22, 34};
    concat_view view(a, b, c);

    auto iter = view.begin();
    iter += 2;
    ASSERT_GT(iter, view.begin());
}

TEST(ConcatNormalIter, GreaterThanBetweenRange) {
    std::vector<int> a{1, 2, 3, 4}, b{9, 10, 11}, c{20, 21, 22, 34};
    concat_view view(a, b, c);

    auto iter = view.begin();
    iter += 5;
    ASSERT_GT(iter, view.begin());
}

TEST(ConcatNormalIter, GreaterThanBetweenRangeOtherWayAround) {
    std::vector<int> a{1, 2, 3, 4}, b{9, 10, 11}, c{20, 21, 22, 34};
    concat_view view(a, b, c);

    auto iter = view.begin();
    iter += 5;
    ASSERT_FALSE(view.begin() > iter);
}


TEST(ConcatNormalIter, GreaterEqualThanInRange) {
    std::vector<int> a{1, 2, 3, 4}, b{9, 10, 11}, c{20, 21, 22, 34};
    concat_view view(a, b, c);

    auto iter = view.begin();
    iter += 2;
    ASSERT_GE(iter, view.begin());
}

TEST(ConcatNormalIter, GreaterEqualThanBetweenRange) {
    std::vector<int> a{1, 2, 3, 4}, b{9, 10, 11}, c{20, 21, 22, 34};
    concat_view view(a, b, c);

    auto iter = view.begin();
    iter += 5;
    ASSERT_GE(iter, view.begin());
}

TEST(ConcatNormalIter, GreaterEqualThanBetweenRangeOtherWayAround) {
    std::vector<int> a{1, 2, 3, 4}, b{9, 10, 11}, c{20, 21, 22, 34};
    concat_view view(a, b, c);

    auto iter = view.begin();
    iter += 5;
    ASSERT_FALSE(view.begin() >= iter);
}
