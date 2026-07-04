#include "concat.hpp"
#include <algorithm>
#include <concepts>
#include <forward_list>
#include <functional>
#include <gtest/gtest.h>
#include <iterator>
#include <list>
#include <print>
#include <ranges>
#include <vector>
#include <type_traits>

TEST(ConcatView, BeginAndEnd) {
    std::list<int> a{1, 2}, b{3, 4, 6, 7, 87}, c{1489, 1500, 1490, 1312};
    concat_view view{a, b, c};

    ASSERT_EQ(*view.begin(), *a.begin());
    ASSERT_EQ(*(--view.end()), *(--c.end()));
}

TEST(ConcatView, SizeCalcularedProperly) {
    std::vector<int> a{1}, b{2, 3}, c{4, 5, 6}, d{10, 10, 12};
    concat_view view{a, b, c, d};

    ASSERT_EQ(view.size(), a.size() + b.size() + c.size() + d.size());
}

TEST(ConcatView, CopyConstructor) {
    std::vector<int> a{1}, b{2, 3}, c{4, 5, 6}, d{10, 10, 12};
    concat_view view1{a, b, c, d};
    concat_view view2{view1};

    ASSERT_EQ(view1.size(), view2.size());

    auto iter1 = view1.begin();
    auto iter2 = view2.begin();
    while (iter1 != view1.end() && iter2 != view2.end()) {
        ASSERT_EQ(*iter1, *iter2);
        ++iter1;
        ++iter2;
    }
}

TEST(ConcatView, CopyAssigment) {
    std::vector<int> a{1}, b{2, 3}, c{4, 5, 6}, d{10, 10, 12};
    concat_view view1{a, b, c, d};
    concat_view<std::vector<int>, 4> view2{};
    view2 = view1;

    ASSERT_EQ(view1.size(), view2.size());

    auto iter1 = view1.begin();
    auto iter2 = view2.begin();
    while (iter1 != view1.end() && iter2 != view2.end()) {
        ASSERT_EQ(*iter1, *iter2);
        ++iter1;
        ++iter2;
    }
}

TEST(ConcatView, MoveConstructor) {
    std::vector<int> a{1}, b{2, 3}, c{4, 5, 6}, d{10, 10, 12};


    concat_view view1{a, b, c, d};
    concat_view view2{std::move(view1)};

    ASSERT_NE(view1.size(), view2.size());
}
