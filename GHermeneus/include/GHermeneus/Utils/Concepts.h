//
// Created by Jelle Spijker on 9/13/20.
//

#ifndef GCODEHERMENEUS_CONCEPTS_H
#define GCODEHERMENEUS_CONCEPTS_H

#include <concepts>

template<typename T>
concept addable = requires (T x) { x + x; };

template<typename T>
concept subtractable = requires (T x) { x - x; };

template <typename T>
concept primitive = addable<T> && std::floating_point<T> && subtractable<T>;

#endif // GCODEHERMENEUS_CONCEPTS_H
