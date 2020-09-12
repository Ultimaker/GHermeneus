//
// Created by Jelle Spijker on 7/31/20.
//

#ifndef GCODEHERMENEUS_INSTRUCTION_H
#define GCODEHERMENEUS_INSTRUCTION_H

#include <string_view>
#include <utility>
#include <vector>

#include "GCode.h"

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
template <typename SSV_T, typename T>
struct Instruction
{
    explicit Instruction() : line_no{} {};

    Instruction(const size_t& line_number, const std::string_view& command, Parameters<T> parameters)
        : line_no(line_number), cmd(command), params(std::move(parameters)){};

    Instruction(Instruction<SSV_T, T>&& instruction) noexcept
        : line_no(instruction.line_no), cmd(instruction.cmd), params(std::move(instruction.params)){};

    Instruction(const Instruction<SSV_T, T>& instruction) noexcept
        : line_no(instruction.line_no), cmd(instruction.cmd), params(instruction.params){};

    const size_t line_no;       //<! The line number
    const std::string_view cmd; //<! The command key
    Parameters<T> params;       //<! A vector of extracted parameters

    Instruction<SSV_T, T>& operator=(const Instruction<SSV_T, T>& rhs)
    {
        return *this;
    }

    bool operator<(const Instruction<SSV_T, T>& rhs) const
    {
        return line_no < rhs.line_no;
    };

    bool operator>(const Instruction<SSV_T, T>& rhs) const
    {
        return rhs < *this;
    };

    bool operator<=(const Instruction<SSV_T, T>& rhs) const
    {
        return !(rhs < *this);
    };

    bool operator>=(const Instruction<SSV_T, T>& rhs) const
    {
        return !(*this < rhs);
    };

    /*!
     * Returns the delta State Space Vector according to a dialect translator.
     * The previous extracted cmd key is used to look up the corresponding function. To which the previous extracted
     * parameters and their value are passed. It will return the delta State Space Vector which can be added to the
     * n - 1  State Space Vector or subtracted from the n + 1 State Space Vector.
     *
     * Todo: Keep in mind that the delta State Space Vector might also be an absolute State Space Vector
     *
     * @brief returns the delta State Space Vector according to a dialect translator
     * @param The Transform<GCodeFunction<SSV_T, T>> instance where the GCode Dialect is coupled to a GCodeFunction
     * @return The Delta State Space Vector of type SSV_T
     */
    SSV_T operator()(const Translator<GCodeFunction<SSV_T, T>>& translator)
    {
        return translator.Cmd(cmd, params);
    };
};
} // namespace GHermeneus

#endif // GCODEHERMENEUS_INSTRUCTION_H
