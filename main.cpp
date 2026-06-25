#include <algorithm>
#include <cstddef>
#include <iostream>
#include <iterator>
#include <print>
#include <ranges>
#include <vector>


template<std::ranges::input_range Range, std::size_t N>
class concat_view {
private:
    std::array<Range*, N> rs_;
public:
    template<typename ... Ranges>
    concat_view(Ranges&... rs) : rs_({&rs...}) {}

    auto begin();
    auto end();

    template<typename Iter>
    class Iterator;
};

template<typename... Ranges>
concat_view(Ranges&...) -> concat_view<std::common_type_t<Ranges...>, sizeof...(Ranges)>;

template<std::ranges::input_range Range, std::size_t N>
template<typename Iter>
class concat_view<Range, N>::Iterator {
private:
    concat_view* view_{nullptr};
    Iter iter_;
    std::size_t cur_iter_{0};
public:
    using value_type = typename Iter::value_type;
    using difference_type = typename Iter::difference_type;
    using reference = typename Iter::reference;
    using pointer = typename Iter::pointer;
    using iterator_category = typename Iter::iterator_category;

    Iterator(concat_view* view, Iter iter, const std::size_t cur_iter) : view_(view), iter_(iter), cur_iter_(cur_iter) {}

    Iterator& operator++() {
        ++iter_;

        if (cur_iter_ < N - 1 && iter_ == view_->rs_[cur_iter_]->end()) {
            iter_ = view_->rs_[cur_iter_ + 1]->begin();
            ++cur_iter_;
        }

        return *this;
    }
    Iterator operator++(int) {
        auto self = *this;
        ++(*this);
        return self;
    }

    Iterator& operator--() requires std::bidirectional_iterator<Iter> {
        if (cur_iter_ > 0 && iter_ == view_->rs_[cur_iter_]->begin()) {
            iter_ = view_->rs_[cur_iter_ - 1]->end();
            --cur_iter_;
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
        std::ptrdiff_t dist = 0;
        while ((dist = std::distance(iter_, view_->rs_[cur_iter_]->end())) <= n) {
            n -= dist;
            iter_ = view_->rs_[cur_iter_ + 1]->begin();
            ++cur_iter_;
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
        std::ptrdiff_t dist = 0;
        while ((dist = std::distance(view_->rs_[cur_iter_]->begin(), iter_)) < n) {
            n -= dist;
            iter_ = view_->rs_[cur_iter_ - 1]->end();
            --cur_iter_;
        }

        iter_ -= n;
        return *this;
    }

    typename Iter::reference operator*() {
        return *iter_;
    }

    typename Iter::reference operator[](const std::ptrdiff_t idx) requires std::random_access_iterator<Iter> {
        return *(*this + idx);
    }

    friend typename Iter::difference_type operator-(const Iterator& b, const Iterator& a) requires std::random_access_iterator<Iter> {
        typename Iter::difference_type res = 0;

        // in the same range
        if (a.cur_iter_ == b.cur_iter_) {
            return b.iter_ - a.iter_;
        }

        auto b_idx = b.cur_iter_;
        res += std::distance(b.view_->rs_[b_idx]->begin(), b.iter_) + 1; // add distance between start of current range and iter of current range.
        // +1 for correct maths
        --b_idx;

        while (b_idx != a.cur_iter_) {
            res += b.view_->rs_[b_idx]->size(); // as for ranges in the middle, just add their sizes
            --b_idx;
        }

        // we went through all the range in-between, finally, just add dsitance between end and a_iter. end() because b.iter is definitely not in this range
        res += std::distance(a.iter_, a.view_->rs_[a.cur_iter_]->end()) - 1;
        return res;
    }

    bool operator!=(const Iterator& rhs) {
        return iter_ != rhs.iter_;
    }
};

template<std::ranges::input_range Range, std::size_t N>
auto concat_view<Range, N>::begin() {
    using Iter = decltype(rs_[0]->begin());
    return Iterator<Iter>{this, rs_[0]->begin(), 0};
}

template<std::ranges::input_range Range, std::size_t N>
auto concat_view<Range, N>::end() {
    using Iter = decltype(rs_[0]->end());
    return Iterator<Iter>{this, rs_[N - 1]->end(), N - 1};
}

int main(int, char**){
    std::vector<int> a{1, 2, 3};
    std::vector<int> b{4, 5, 6};
    std::vector<int> c{7, 8, 9};

    auto view = concat_view(a, b, c);

    std::for_each(view.begin(), view.end(), [](const auto value) {
        std::println("Value: {}", value);
    });

    return 0;
}
