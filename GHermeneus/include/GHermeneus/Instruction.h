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
    template<typename T>
    struct Parameter
    {
        Parameter(const std::string_view& param, const T& value) :
                param(param),
                value(value)
        {};

        Parameter(Parameter&& parameter) noexcept:
                param(parameter.param),
                value(parameter.value)
        {};

        const std::string_view param;
        const T value;
    };

    using Parameters = std::vector<Parameter<double>>;

    template<typename T>
    struct Instruction
    {
        Instruction(const size_t& line_no, const std::string_view& cmd, Parameters params) :
                line_no(line_no),
                cmd(cmd),
                params(std::move(params))
        {};

        Instruction(Instruction&& instruction) noexcept:
                line_no(instruction.line_no),
                cmd(instruction.cmd),
                params(instruction.params)
        {};

        const size_t line_no;
        const std::string_view cmd;
        const Parameters params;

        bool operator<(const Instruction& rhs) const;

        bool operator>(const Instruction& rhs) const;

        bool operator<=(const Instruction& rhs) const;

        bool operator>=(const Instruction& rhs) const;

        T operator(const T& prev, const Transform<GCodeFunction<T>>& transform)()
        {
            T value = transform.Cmd(cmd, params);
            return value;
        }
    };
}

#endif //GCODEHERMENEUS_INSTRUCTION_H
