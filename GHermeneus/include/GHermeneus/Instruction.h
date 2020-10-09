//
// Created by Jelle Spijker on 7/31/20.
//

#ifndef GCODEHERMENEUS_INSTRUCTION_H
#define GCODEHERMENEUS_INSTRUCTION_H

#include <string_view>
#include <utility>
#include <vector>

#include <range/v3/algorithm/sort.hpp>

#include "GHermeneus/Utils/Concepts.h"
#include "GHermeneus/Parameters.h"

namespace GHermeneus
{
/*!
 * An dialect interpreted GCode line which consist of a corresponding line number, command key (e.q. G0, M104) and
 * a vector of Parameters of type T.
 *
 * @brief An interpreted GCode function
 * @tparam SSV_T the State Space Vector type
 * @tparam T the magnitude type
 */
template <typename T, int n>
requires primitive<T> && at_least_one_scalar<n> struct Instruction
{
    Instruction() = default;

    Instruction(const size_t& line_number, const std::string_view& command, Parameters<T> parameters)
        : line_no(line_number), cmd(command), params(std::move(parameters))
    {
        ranges::sort(parameters);
    };

    size_t line_no;       //<! The line number
    std::string_view cmd; //<! The command key
    Parameters<T> params; //<! A vector of extracted parameters

    bool operator<(const Instruction<T, n>& rhs) const
    {
        return line_no < rhs.line_no;
    };

    bool operator>(const Instruction<T, n>& rhs) const
    {
        return rhs < *this;
    };

    bool operator<=(const Instruction<T, n>& rhs) const
    {
        return !(rhs < *this);
    };

    bool operator>=(const Instruction<T, n>& rhs) const
    {
        return !(*this < rhs);
    };
};
} // namespace GHermeneus

#endif // GCODEHERMENEUS_INSTRUCTION_H
