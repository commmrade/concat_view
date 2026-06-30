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

TEST(ConcatIterator, ForwardIter) {
    std::vector<int> a{1, 2, 3}, b{4, 5, 6};
    concat_view view{a, b};

    concat_view view2{view};

    // auto newly = view | std::ranges::views::transform([](auto& val) {
    //     return val += 2;
    // });

    // std::ranges::for_each(newly, [](const auto val) {
    //     std::println("val: {}", val);
    // });
}
