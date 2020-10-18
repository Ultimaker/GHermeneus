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

    constexpr Primitive<T>(const T value, const bool relative) : m_value{ value }, m_relative{ relative } {};

    [[nodiscard]] constexpr auto operator<=>(const Primitive<T>&) const = default;

    [[nodiscard]] constexpr operator T() const
    {
        return m_value;
    }

    constexpr auto& operator+=(const Primitive<T>& rhs)
    {
        if (rhs.m_relative)
        {
            m_value += rhs.m_value;
        }
        else [[likely]]
        {
            m_value = rhs.m_value;
        }
        return *this;
        ;
    }

    constexpr auto& operator-=(const Primitive<T>& rhs)
    {
        if (rhs.m_relative)
        {
            m_value -= rhs.m_value;
        }
        else [[likely]]
        {
            m_value = rhs.m_value;
        }
        return *this;
    }

    [[nodiscard]] constexpr auto operator+(const Primitive<T>& other) const
    {
        if (other.m_relative)
        {
            return Primitive<T>(m_value + other.m_value);
        }
        return Primitive<T>(other.m_value);
    }

    [[nodiscard]] constexpr auto operator-(const Primitive<T>& other) const
    {
        if (other.m_relative)
        {
            return Primitive<T>(m_value - other.m_value);
        }
        return Primitive<T>(other.m_value);
    }

    [[nodiscard]] constexpr bool getRelative() const
    {
        return m_relative;
    }

    constexpr void setRelative(const bool relative)
    {
        m_relative = relative;
    }

  private:
    T m_value{ 0. };
    bool m_relative{ true };
};
} // namespace GHermeneus

namespace Eigen
{
template <typename T>
requires base_primitive<T> struct NumTraits<GHermeneus::Primitive<T>> : NumTraits<T>
{
    using Real = GHermeneus::Primitive<T>;
    using NonInteger = GHermeneus::Primitive<T>;
    using Nested = GHermeneus::Primitive<T>;

    enum
    {
        IsComplex = 0,
        IsInteger = 0,
        IsSigned = 1,
        RequireInitialization = 0,
        ReadCost = 1,
        AddCost = 3,
        MulCost = 3
    };
};

} // namespace Eigen

#endif // GCODEHERMENEUS_PRIMITIVE_H
