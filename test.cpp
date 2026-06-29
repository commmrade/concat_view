#include "concat.hpp"
#include <gtest/gtest.h>
#include <iterator>
#include <vector>

TEST(ConcatIterator, OperatorPlusPlusPost) {
    std::vector<int> a{1, 2, 3}, b{4, 5, 6};
    concat_view view{a, b};

    ASSERT_EQ(view.size(), 6);

    auto iter = view.begin();
    ASSERT_EQ(*(2 + iter), 3);
    ASSERT_EQ(*(iter + 2), 3);

    constexpr auto is_t = std::random_access_iterator<decltype(iter)>;
}

TEST(Main, Testmain) {
    std::vector<int> vec{1, 2, 3, 4, 5};
    auto iter = vec.begin();
    iter -= -2;
    ASSERT_EQ(*iter, 3);
}
