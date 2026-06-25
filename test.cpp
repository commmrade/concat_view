#include "concat.hpp"
#include <gtest/gtest.h>
#include <vector>

TEST(ConcatIterator, OperatorPlusPlusPost) {
    std::vector<int> a{1, 2, 3}, b{4, 5, 6};
    concat_view view{a, b};

    auto iter = view.begin();
    ASSERT_EQ(*iter, 1);

    iter++;
    ASSERT_EQ(*iter, 2);
    iter++;
    ASSERT_EQ(*iter, 3);
    iter++;
    ASSERT_EQ(*iter, 4);
}

TEST(ConcatIterator, OperatorPlusPlusPre) {
    std::vector<int> a{1, 2, 3}, b{4, 5, 6};
    concat_view view{a, b};

    auto iter = view.begin();
    ASSERT_EQ(*iter, 1);

    ++iter;
    ASSERT_EQ(*iter, 2);
    ++iter;
    ASSERT_EQ(*iter, 3);
    ++iter;
    ASSERT_EQ(*iter, 4);
}

TEST(ConcatIterator, EndEqualsEnd) {
    std::vector<int> a{1, 2, 3}, b{4, 5, 6};
    concat_view view{a, b};

    auto beg = view.begin();
    for (auto i = 0; i < a.size() + b.size(); ++i) {
        ++beg;
    }

    ASSERT_TRUE(beg == view.end());
}

// following tests:
// start + next range
// start + offset in start range + next range
// start + 2 ranges
// start + offfset in start range + 2 ranges

TEST(ConcatIterator, OperatorPlus) {
    std::vector<int> a{1, 2, 3}, b{4, 6}, c{9, 10, 11, 12};
    concat_view view{a, b, c};

    {
        auto beg = view.begin();
        beg += 3;
        ASSERT_EQ(*beg, 4);
    }
    {
        auto beg = view.begin();
        beg += 2;
        beg += 2;
        ASSERT_EQ(*beg, 6);
    }
    {
        auto beg = view.begin();
        beg += 5;
        ASSERT_EQ(*beg, 9);
    }
    {
        auto beg = view.begin();
        beg += 2;
        beg += 4;
        ASSERT_EQ(*beg, 10);
    }
}

TEST(ConcatIterator, OperatorMinus) {
    std::vector<int> a{1, 2, 3}, b{4, 6}, c{9, 10, 11, 12};
    concat_view view{a, b, c};

    // in-range
    {
        auto end = view.end();
        end -= 3;
        ASSERT_EQ(*end, 10);
    }
    // prev range
    {
        auto end = view.end();
        end -= 5;
        ASSERT_EQ(*end, 6);
    }
    // offset in cur range + prev range
    {
        auto end = view.end();
        end -= 2;
        end -= 4;
        ASSERT_EQ(*end, 4);
    }
    // end - 2 ranges
    {
        auto end = view.end();
        end -= 7;
        ASSERT_EQ(*end, 3);
    }
}
