#include <cstddef>
#include <iostream>
#include <iterator>
#include <print>
#include <ranges>
#include <vector>


template<std::ranges::input_range Range>
class concat_view {
private:
    Range& r1_;
    Range& r2_;
public:
    concat_view(Range& r1, Range& r2) : r1_(r1), r2_(r2) {}

    auto begin();
    auto end();

    template<typename Iter>
    class Iterator;
};

template<std::ranges::input_range Range>
template<typename Iter>
class concat_view<Range>::Iterator {
private:
    concat_view* view_{nullptr};
    Iter iter_;
    bool cur_iter_{false};
public:
    Iterator(concat_view* view, Iter iter) : view_(view), iter_(iter) {}

    Iterator& operator++() {
        ++iter_;
        if (!cur_iter_) {
            if (iter_ == view_->r1_.end()) {
                iter_ = view_->r2_.begin();
                cur_iter_ = true;
            }
        }
        return *this;
    }
    Iterator operator++(int) {
        auto self = *this;
        ++(*this);
        return self;
    }

    Iterator& operator--() requires std::bidirectional_iterator<Iter> {
        if (cur_iter_) {
            if (iter_ == view_->r2_.begin()) {
                iter_ = view_->r1_.end();
                cur_iter_ = false;
            }
        }
        --iter_;
        return *this;
    }
    Iterator operator--(int) requires std::bidirectional_iterator<Iter> {
        auto self = *this;
        --(*this);
        return self;
    }

    Iterator operator+(std::ptrdiff_t n) requires std::random_access_iterator<Iter> {
        auto ret = *this;
        ret += n;
        return ret;
    }

    Iterator& operator+=(std::ptrdiff_t n) requires std::random_access_iterator<Iter> {
        if (!cur_iter_) {
            const auto dist = std::distance(iter_, view_->r1_.end());
            if (n >= dist) {
                n -= dist;
                iter_ = view_->r2_.begin();
                cur_iter_ = true;
            }
        }

        iter_ += n;
        return *this;
    }

    Iterator operator-(std::ptrdiff_t n) requires std::random_access_iterator<Iter> {
        auto ret = *this;
        ret -= n;
        return ret;
    }

    Iterator& operator-=(std::ptrdiff_t n) requires std::random_access_iterator<Iter> {
        if (cur_iter_) {
            const auto dist = std::distance(view_->r2_.begin(), iter_);
            if (n > dist) {
                n -= dist;
                iter_ = view_->r1_.end();
                cur_iter_ = false;
            }
        }

        iter_ -= n;
        return *this;
    }

    typename Iter::reference operator*() {
        return *iter_;
    }

    bool operator!=(const Iterator& rhs) {
        return iter_ != rhs.iter_;
    }
};

template<std::ranges::input_range Range>
auto concat_view<Range>::begin() {
    using Iter = decltype(r1_.begin());
    return concat_view<Range>::Iterator<Iter>{this, r1_.begin()};
}

template<std::ranges::input_range Range>
auto concat_view<Range>::end() {
    using Iter = decltype(r2_.end());
    return concat_view<Range>::Iterator<Iter>{this, r2_.end()};
}

int main(int, char**){
    std::vector<int> a{1, 2, 3};
    std::vector<int> b{4, 5, 6};
    std::vector<int> c{7, 8, 9};

    auto view = concat_view{a, b};
    auto beg = view.begin();
    beg += 3;
    beg -= 1;

    std::println("{}", *beg);

    return 0;
}
