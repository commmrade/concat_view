#include "concat.hpp"
#include <concepts>
#include <gtest/gtest.h>
#include <iterator>
#include <vector>
#include <type_traits>

TEST(ConcatIterator, OperatorPlusPlusPost) {
    std::vector<int> a{1, 2, 3}, b{4, 5, 6};
    concat_view view{a, b};

    ASSERT_EQ(view.size(), 6);

    auto iter = view.begin();
    ASSERT_EQ(*(2 + iter), 3);
    ASSERT_EQ(*(iter + 2), 3);

    (iter + 1) - iter;

    ASSERT_TRUE((std::is_same_v<decltype(iter)::iterator_category, std::random_access_iterator_tag>));
    ASSERT_TRUE((std::totally_ordered<decltype(iter)>));
    ASSERT_TRUE((std::sized_sentinel_for<decltype(iter), decltype(iter)>));
    ASSERT_TRUE((requires {
        std::declval<decltype(iter)>() - std::declval<decltype(iter)>();
    }));

    constexpr auto is_t = std::random_access_iterator<decltype(iter)>;
    // ASSERT_TRUE(is_t);
}

TEST(Main, Testmain) {
    std::vector<int> vec{1, 2, 3, 4, 5};
    auto iter = vec.begin();
}
