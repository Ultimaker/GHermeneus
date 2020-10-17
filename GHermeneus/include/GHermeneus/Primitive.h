//
// Created by Jelle Spijker on 10/17/20.
//

#ifndef GCODEHERMENEUS_PRIMITIVE_H
#define GCODEHERMENEUS_PRIMITIVE_H

#include <compare>

#include "Utils/Concepts.h"

namespace GHermeneus
{
template <typename T>
requires base_primitive<T> class Primitive
{
  public:
    constexpr Primitive<T>() = default;

    constexpr explicit Primitive<T>(const T value) : m_value{ value } {};

    auto operator<=>(const Primitive<T>&) const = default;

    constexpr operator T() const
    {
        return m_value;
    }

    constexpr Primitive<T>& operator+=(const Primitive<T>& rhs)
    {
        m_value += rhs.m_value;
        return *m_value;
    }

    constexpr Primitive<T>& operator-=(const Primitive<T>& rhs)
    {
        m_value -= rhs.m_value;
        return *m_value;
    }

    constexpr Primitive<T> operator+(const Primitive<T>& rhs) const
    {
        return Primitive<T>(m_value + rhs.m_value);
    }

    constexpr Primitive<T> operator-(const Primitive<T>& rhs) const
    {
        return Primitive<T>(m_value - rhs.m_value);
    }

  private:
    T m_value;
};
} // namespace GHermeneus
#endif // GCODEHERMENEUS_PRIMITIVE_H
