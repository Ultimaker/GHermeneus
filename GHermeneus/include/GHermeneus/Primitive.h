//
// Created by Jelle Spijker on 10/17/20.
//

#ifndef GCODEHERMENEUS_PRIMITIVE_H
#define GCODEHERMENEUS_PRIMITIVE_H

#include <compare>

#include <Eigen/Core>

#include "Utils/Concepts.h"

namespace GHermeneus
{
template <typename T>
requires base_primitive<T> class Primitive
{
  public:
    using value_type [[maybe_unused]] = T;

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
        return *this;
    }

    constexpr Primitive<T>& operator-=(const Primitive<T>& rhs)
    {
        m_value -= rhs.m_value;
        return *this;
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

namespace Eigen
{
template<typename T>
requires base_primitive<T>
struct NumTraits<GHermeneus::Primitive<T>> : NumTraits<T>
{
    using Real = GHermeneus::Primitive<T>;
    using NonInteger = GHermeneus::Primitive<T>;
    using Nested = GHermeneus::Primitive<T>;

    enum {
        IsComplex = 0,
        IsInteger = 0,
        IsSigned = 1,
        RequireInitialization = 0,
        ReadCost = 1,
        AddCost = 3,
        MulCost = 3
    };
};

}

#endif // GCODEHERMENEUS_PRIMITIVE_H
