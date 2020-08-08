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
     * a vector of Parameters of type double.
     *
     * @brief An interpreted GCode function
     * @tparam SSV_T the State Space Vector type
     * @tparam T the magnitude type
     */
    template<typename SSV_T, typename T>
    struct Instruction
    {
        Instruction(const size_t& line_no, const std::string_view& cmd, Parameters<T> params) :
                line_no(line_no),
                cmd(cmd),
                params(std::move(params))
        {};

        Instruction(Instruction<SSV_T, T>&& instruction) noexcept:
                line_no(instruction.line_no),
                cmd(instruction.cmd),
                params(instruction.params)
        {};

        const size_t line_no;
        const std::string_view cmd;
        const Parameters<T> params;

        bool operator<(const Instruction<SSV_T, T>& rhs) const;

        bool operator>(const Instruction<SSV_T, T>& rhs) const;

        bool operator<=(const Instruction<SSV_T, T>& rhs) const;

        bool operator>=(const Instruction<SSV_T, T>& rhs) const;

        /*!
         * Returns the delta State Space Vector according to a dialect transform.
         * The previous extracted cmd key is used to look up the corresponding function. To which the previous extracted
         * parameters and their value are passed. It will return the delta State Space Vector which can be added to the
         * n - 1  State Space Vector or subtracted from the n + 1 State Space Vector.
         *
         * @brief returns the delta State Space Vector according to a dialect transform
         * @param The Transform<GCodeFunction<SSV_T, T>> instance where the GCode Dialect is coupled to a GCodeFunction
         * @return The Delta State Space Vector of type SSV_T
         */
        SSV_T operator()(const Transform<GCodeFunction<SSV_T, T>>& transform);
    };
}

#endif //GCODEHERMENEUS_INSTRUCTION_H
