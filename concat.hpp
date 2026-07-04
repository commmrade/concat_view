#include <algorithm>
#include <cstddef>
#include <iostream>
#include <iterator>
#include <print>
#include <ranges>
#include <vector>


template<std::ranges::input_range Range, std::size_t N>
class concat_view : public std::ranges::view_interface<concat_view<Range, N>> {
private:
    std::array<Range*, N> rs_{};
public:
    template<typename ... Ranges>
    constexpr concat_view(Ranges&... rs) : rs_({&rs...}) {}
    ~concat_view() = default;

    constexpr concat_view(concat_view&& rhs) {
        std::swap(rs_, rhs.rs_);
    }
    constexpr concat_view& operator=(concat_view&& rhs) {
        std::swap(rs_, rhs.rs_);
        return *this;
    }

    // FIXME: somehow i should take a const ref
    constexpr concat_view(concat_view& rhs) {
        rs_ = rhs.rs_;
    }
    constexpr concat_view& operator=(const concat_view& rhs) {
        rs_ = rhs.rs_;
        return *this;
    }

    constexpr auto begin();
    constexpr auto end();

    constexpr std::size_t size() const {
        if (!rs_[0]) [[unlikely]] { // only happens after a move
            return 0;
        }

        std::size_t res = 0;
        for (const auto rng : rs_) {
            res += rng->size();
        }
        return res;
    }

    template<typename Iter>
    class Iterator;
};

template<typename... Ranges>
concat_view(Ranges&...) -> concat_view<std::common_type_t<Ranges...>, sizeof...(Ranges)>;

template<std::ranges::input_range Range, std::size_t N>
template<typename Iter>
class concat_view<Range, N>::Iterator {
private:
    const concat_view* view_{nullptr};
    Iter iter_{};
    std::size_t cur_iter_{0};

    template<typename IterD1, typename IterD2>
    constexpr static std::ptrdiff_t distance(const IterD1& first, const IterD2& last) {
        return last - first;
    }
public:
    using value_type = typename Iter::value_type;
    using difference_type = typename Iter::difference_type;
    using reference = typename Iter::reference;
    using pointer = typename Iter::pointer;
    using iterator_category = typename Iter::iterator_category;

    constexpr Iterator() = default;
    constexpr Iterator(const concat_view* view, Iter iter, const std::size_t cur_iter) : view_(view), iter_(iter), cur_iter_(cur_iter) {}

    constexpr Iterator& operator++() noexcept {
        ++iter_;

        if (cur_iter_ < N - 1 && iter_ == view_->rs_[cur_iter_]->end()) {
            iter_ = view_->rs_[cur_iter_ + 1]->begin();
            ++cur_iter_;
        }

        return *this;
    }
    constexpr Iterator operator++(int) noexcept {
        auto self = *this;
        ++(*this);
        return self;
    }

    constexpr Iterator& operator--() noexcept requires std::bidirectional_iterator<Iter> {
        if (cur_iter_ > 0 && iter_ == view_->rs_[cur_iter_]->begin()) {
            iter_ = view_->rs_[cur_iter_ - 1]->end();
            --cur_iter_;
        }
        --iter_;
        return *this;
    }
    constexpr Iterator operator--(int) noexcept requires std::bidirectional_iterator<Iter> {
        auto self = *this;
        --(*this);
        return self;
    }

    constexpr Iterator operator+(std::ptrdiff_t n) const noexcept requires std::random_access_iterator<Iter> {
        auto ret = *this;
        ret += n;
        return ret;
    }
    constexpr friend Iterator operator+(std::ptrdiff_t n, const Iterator& iter) noexcept requires std::random_access_iterator<Iter> {
        return iter + n;
    }

    constexpr Iterator& operator+=(std::ptrdiff_t n) noexcept requires std::random_access_iterator<Iter> {
        if (n < 0) {
            return *this -= -n;
        }

        std::ptrdiff_t dist = 0;
        while (cur_iter_ < N - 1 && (dist = distance(iter_, view_->rs_[cur_iter_]->end())) <= n) {
            n -= dist;
            iter_ = view_->rs_[cur_iter_ + 1]->begin();
            ++cur_iter_;
        }

        iter_ += n;
        return *this;
    }

    constexpr Iterator operator-(std::ptrdiff_t n) const noexcept requires std::random_access_iterator<Iter> {
        auto ret = *this;
        ret -= n;
        return ret;
    }
    constexpr friend Iterator operator-(std::ptrdiff_t n, const Iterator& iter) noexcept requires std::random_access_iterator<Iter> {
        return iter - n;
    }

    constexpr Iterator& operator-=(std::ptrdiff_t n) noexcept requires std::random_access_iterator<Iter> {
        if (n < 0) {
            return *this += -n;
        }

        std::ptrdiff_t dist = 0;
        while (cur_iter_ > 0 && (dist = distance(view_->rs_[cur_iter_]->begin(), iter_)) < n) {
            n -= dist;
            iter_ = view_->rs_[cur_iter_ - 1]->end();
            --cur_iter_;
        }

        iter_ -= n;
        return *this;
    }

    constexpr reference operator*() const noexcept {
        return *iter_;
    }

    constexpr reference operator[](const std::ptrdiff_t idx) const noexcept requires std::random_access_iterator<Iter> {
        return *(*this + idx);
    }

    constexpr friend difference_type operator-(const Iterator& b, const Iterator& a) noexcept requires std::random_access_iterator<Iter> {
        difference_type res = 0;

        // in the same range
        if (a.cur_iter_ == b.cur_iter_) {
            return b.iter_ - a.iter_;
        }

        auto b_idx = b.cur_iter_;
        res += distance(b.view_->rs_[b_idx]->begin(), b.iter_) + 1; // add distance between start of current range and iter of current range.
        // +1 for correct maths
        --b_idx;

        while (b_idx != a.cur_iter_) {
            res += b.view_->rs_[b_idx]->size(); // as for ranges in the middle, just add their sizes
            --b_idx;
        }

        // we went through all the range in-between, finally, just add dsitance between end and a_iter. end() because b.iter is definitely not in this range
        res += distance(a.iter_, a.view_->rs_[a.cur_iter_]->end()) - 1;
        return res;
    }

    constexpr friend bool operator==(const Iterator& lhs, const Iterator& rhs) noexcept {
        return lhs.iter_ == rhs.iter_;
    }
    constexpr friend bool operator!=(const Iterator& lhs, const Iterator& rhs) noexcept {
        return lhs.iter_ != rhs.iter_;
    }

    constexpr friend bool operator<(const Iterator& lhs, const Iterator& rhs) noexcept requires std::random_access_iterator<Iter> {
        if (lhs.cur_iter_ == rhs.cur_iter_) {
            return lhs.iter_ < rhs.iter_;
        }
        return lhs.cur_iter_ < rhs.cur_iter_;
    }
    constexpr friend bool operator>(const Iterator& lhs, const Iterator& rhs) noexcept requires std::random_access_iterator<Iter> {
        if (lhs.cur_iter_ == rhs.cur_iter_) {
            return lhs.iter_ > rhs.iter_;
        }
        return lhs.cur_iter_ > rhs.cur_iter_;
    }
    constexpr friend bool operator<=(const Iterator& lhs, const Iterator& rhs) noexcept requires std::random_access_iterator<Iter> {
        if (lhs.cur_iter_ == rhs.cur_iter_) {
            return lhs.iter_ <= rhs.iter_;
        }
        return lhs.cur_iter_ < rhs.cur_iter_;
    }
    constexpr friend bool operator>=(const Iterator& lhs, const Iterator& rhs) noexcept requires std::random_access_iterator<Iter> {
        if (lhs.cur_iter_ == rhs.cur_iter_) {
            return lhs.iter_ >= rhs.iter_;
        }
        return lhs.cur_iter_ > rhs.cur_iter_;
    }
};

template<std::ranges::input_range Range, std::size_t N>
constexpr auto concat_view<Range, N>::begin() {
    using Iter = decltype(rs_[0]->begin());
    return Iterator<Iter>{this, rs_[0]->begin(), 0};
}

template<std::ranges::input_range Range, std::size_t N>
constexpr auto concat_view<Range, N>::end() {
    using Iter = decltype(rs_[0]->end());
    return Iterator<Iter>{this, rs_[N - 1]->end(), N - 1};
}
