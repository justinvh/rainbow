#pragma once
#ifndef RAINBOW_UTILS_ITERTOOLS_HPP
#define RAINBOW_UTILS_ITERTOOLS_HPP

#include <iterator>
#include <type_traits>

namespace rb {

template <class Iter, class Tag>
using has_tag =
    std::is_convertible<
        typename std::iterator_traits<Iter>::iterator_category,
        Tag>;

template <typename Iter>
using has_output_iterator_tag = has_tag<Iter, std::output_iterator_tag>;

template <typename Iter>
using is_output_iterator =
    std::enable_if<has_output_iterator_tag<Iter>::value>;
}

#endif // RAINBOW_UTILS_ITERTOOLS_HPP

