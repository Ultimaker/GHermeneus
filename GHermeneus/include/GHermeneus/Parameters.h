//
// Created by Jelle Spijker on 8/8/20.
//

#ifndef GCODEHERMENEUS_PARAMETERS_H
#define GCODEHERMENEUS_PARAMETERS_H

#include <string_view>
#include <vector>

namespace GHermeneus
{
/*!
 * @brief A
 * @tparam T
 */
    template<typename T = double>
    struct Parameter
    {
        Parameter(const std::string_view& param, const T& value) :
                param(param),
                value(value)
        {};

        Parameter(Parameter<T>&& parameter) noexcept:
                param{std::move(parameter.param)},
                value{parameter.value}
        {};

        Parameter(const Parameter<T>& parameter) noexcept:
                param{parameter.param},
                value{parameter.value}
        {};

        std::string_view param;
        T value;
    };

    template<typename T = double>
    using Parameters = std::vector<Parameter<T>>;  //<! Vector of Parameter of type T default = double
}
#endif //GCODEHERMENEUS_PARAMETERS_H
