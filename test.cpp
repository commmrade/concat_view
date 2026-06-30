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

    ASSERT_TRUE((std::random_access_iterator<decltype(iter)>));

    using Iter = decltype(view.begin());

    // 1. bidirectional_iterator
    static_assert(std::bidirectional_iterator<Iter>, "not bidirectional");

    // 2. iter_concept derived from random_access_iterator_tag
    // (checks what tag your iterator advertises)
    using IterConcept = std::iterator_traits<Iter>::iterator_category;
    static_assert(
        std::derived_from<IterConcept, std::random_access_iterator_tag>,
        "iter_concept not derived from random_access_iterator_tag"
    );

    // 3. totally_ordered
    static_assert(std::totally_ordered<Iter>, "not totally_ordered");

    // 4. sized_sentinel_for<Iter, Iter>  (operator- returns difference_type)
    static_assert(std::sized_sentinel_for<Iter, Iter>, "not sized_sentinel_for");

    // 5. operator+= returns Iter&
    static_assert(requires(Iter i, std::iter_difference_t<Iter> n) {
        { i += n } -> std::same_as<Iter&>;
    }, "operator+= missing or wrong return type");

    // 6. operator+ (iter + n) returns Iter
    static_assert(requires(const Iter j, std::iter_difference_t<Iter> n) {
        { j + n } -> std::same_as<Iter>;
    }, "operator+(iter,n) missing or wrong return type");

    // 7. operator+ (n + iter) returns Iter
    static_assert(requires(const Iter j, std::iter_difference_t<Iter> n) {
        { n + j } -> std::same_as<Iter>;
    }, "operator+(n,iter) missing or wrong return type");

    // 8. operator-= returns Iter&
    static_assert(requires(Iter i, std::iter_difference_t<Iter> n) {
        { i -= n } -> std::same_as<Iter&>;
    }, "operator-= missing or wrong return type");

    // 9. operator- (iter - n) returns Iter
    static_assert(requires(const Iter j, std::iter_difference_t<Iter> n) {
        { j - n } -> std::same_as<Iter>;
    }, "operator-(iter,n) missing or wrong return type");

    // 10. operator[] returns iter_reference_t<Iter>
    static_assert(requires(const Iter j, std::iter_difference_t<Iter> n) {
        { j[n] } -> std::same_as<std::iter_reference_t<Iter>>;
    }, "operator[] missing or wrong return type");
    // ASSERT_TRUE((std::random_access_iterator<decltype(iter)>));
    // ASSERT_TRUE(is_t);
}

TEST(Main, Testmain) {
    std::vector<int> vec{1, 2, 3, 4, 5};
    const auto iter = vec.begin();
    iter += 2;
}
