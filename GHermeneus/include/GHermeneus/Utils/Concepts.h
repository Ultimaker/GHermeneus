//
// Created by Jelle Spijker on 9/13/20.
//

#ifndef GCODEHERMENEUS_CONCEPTS_H
#define GCODEHERMENEUS_CONCEPTS_H

#include <concepts>
#include <type_traits>
#include <utility>

template <typename T>
concept addable = requires(T x)
{
    x + x;
};

template <typename T>
concept subtractable = requires(T x)
{
    x - x;
};

template<typename T>
concept base_arithmetic = addable<T> && subtractable<T>;

template <typename T>
concept base_primitive = std::floating_point<T> && base_arithmetic<T>;

template <int n>
concept at_least_one_scalar = requires(int x)
{
    x > 0;
};

template <typename B, typename T>
concept is_derived = std::is_base_of<B, T>::value;

template <class T>
concept primitive = base_arithmetic<T> && requires(T x)
{
    x.setRelative(true);
};
#endif // GCODEHERMENEUS_CONCEPTS_H
