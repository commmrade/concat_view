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
    int cur_iter_{0};
public:
    Iterator(concat_view* view, Iter iter) : view_(view), iter_(iter) {}

    Iterator& operator++() {
        ++iter_;

        if (cur_iter_ != N - 1) {
            if (iter_ == view_->rs_[cur_iter_]->end()) {
                iter_ = view_->rs_[cur_iter_ + 1]->begin();
                ++cur_iter_;
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

            if (cur_iter_ != 0) {
                if (iter_ == view_->rs_[cur_iter_]->begin()) {
                    iter_ = view_->rs_[cur_iter_ - 1]->end();
                    --cur_iter_;
                }
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
        if (cur_iter_ < N - 1) {
            std::ptrdiff_t dist = 0;
            while ((dist = std::distance(iter_, view_->rs_[cur_iter_]->end())) <= n) {
                n -= dist;
                iter_ = view_->rs_[cur_iter_ + 1]->begin();
                ++cur_iter_;
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
        if (cur_iter_ > 0) {
            std::ptrdiff_t dist = 0;
            while ((dist = std::distance(view_->rs_[cur_iter_]->begin(), iter_)) < n) {
                n -= dist;
                iter_ = view_->rs_[cur_iter_ - 1]->end();
                --cur_iter_;
            }
        }

        iter_ -= n;
        return *this;
    }

    typename Iter::reference operator*() {
        return *iter_;
    }

    typename Iter::reference operator[](const std::ptrdiff_t idx) requires std::random_access_iterator<Iter> {
        auto temp = *this;
        temp += idx;
        return *temp;
    }

    typename Iter::difference_type operator-(const Iterator& rhs) requires std::random_access_iterator<Iter> {
        // todo: use cur_iter index
    }

    bool operator!=(const Iterator& rhs) {
        return iter_ != rhs.iter_;
    }
};

template<std::ranges::input_range Range, std::size_t N>
auto concat_view<Range, N>::begin() {
    using Iter = decltype(rs_[0]->begin());
    return Iterator<Iter>{this, rs_[0]->begin()};
}

template<std::ranges::input_range Range, std::size_t N>
auto concat_view<Range, N>::end() {
    using Iter = decltype(rs_[0]->end());
    return Iterator<Iter>{this, rs_[N - 1]->end()};
}

int main(int, char**){
    std::vector<int> a{1, 2, 3};
    std::vector<int> b{4, 5, 6};
    std::vector<int> c{7, 8, 9};

    auto view = concat_view{a, b, c};

    auto beg = view.begin();
    beg += 6;
    beg -= 6;
    auto end = view.end();

    std::println("{}", *beg);

    return 0;
}
