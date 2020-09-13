//
// Created by Jelle Spijker on 8/8/20.
//

#ifndef GCODEHERMENEUS_PARAMETERS_H
#define GCODEHERMENEUS_PARAMETERS_H

#include <string_view>
#include <vector>

#include "GHermeneus/Utils/Concepts.h"

namespace GHermeneus
{
/*!
 * @brief A type containing the parameter key (e.q. X, Y, Z, F, E)
 * @tparam T the primitive type of the parameter value, this should be the same as the primitive type of the State Space
 * Vector. Defaults to double
 */
template <typename T = double>
requires primitive<T> struct Parameter
{
    Parameter(const std::string_view& param, const T& val) : parameter(param), value(val){};

    std::string_view parameter; //<! The Key of the parameter name
    T value;                    //<! The extracted GCode parameter value
};

template <typename T = double>
requires primitive<T> using Parameters = std::vector<Parameter<T>>; //<! Vector of Parameter of type T default = double
} // namespace GHermeneus
#endif // GCODEHERMENEUS_PARAMETERS_H
