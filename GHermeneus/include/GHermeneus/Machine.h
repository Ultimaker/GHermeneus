//
// Created by peer23peer on 5/22/20.
//

#ifndef GHERMENEUS_MACHINE_H
#define GHERMENEUS_MACHINE_H

#include <algorithm>
#include <execution>
#include <string>
#include <vector>
#include <iostream>
#include <mutex>

#include <range/v3/distance.hpp>
#include <range/v3/view/split.hpp>
#include <range/v3/view/transform.hpp>
#include <range/v3/view/enumerate.hpp>
#include <range/v3/view/take.hpp>
#include <range/v3/view/drop.hpp>

#include "GCode.h"
#include "Instruction.h"
#include "Parameters.h"

namespace GHermeneus
{
    template<typename SSV_T, typename T>
    class Machine
    {
    public:

        Machine() = default;

        Machine(Machine<SSV_T, T>&& machine) = default;

        virtual ~Machine() = default;

        void parse(const std::string_view& GCode)
        {
            extractLines(GCode);
            cmdlines.reserve(lines.size());
            std::for_each(std::execution::par_unseq, lines.begin(), lines.end(), [&](Line& line) {
                std::lock_guard<std::mutex> guard(cmdlines_mutex);
                cmdlines.emplace_back(extractCmd(line));
            });
            std::sort(std::execution::par_unseq, cmdlines.begin(),
                      cmdlines.end()); // Todo: check performance of diff execution policies
        }

        friend std::ostream& operator<<(std::ostream& os, const Machine<SSV_T, T>& machine)
        {
            for (const auto& cmdline : machine.cmdlines)
            {
                auto &[lineno, cmd, params] = cmdline;
                os << "line: " << lineno << " command: " << cmd << " Parameters ->";
                for (const auto& param : params)
                {
                    os << " " << param.param << " = " << param.value;
                }
                os << std::endl;
            }
            return os;
        };

        friend Machine<SSV_T, T>& operator<<(Machine<SSV_T, T>& machine, const std::string_view& GCode)
        {
            machine.parse(GCode);
            return machine;
        };

    private:

        void extractLines(const std::string_view& GCode)
        {
            // TODO: keep in mind CRLF and LF
            lines = gcode
                    | ranges::views::split('\n')
                    | ranges::views::transform([](auto&& line) {
                return std::string_view(&*line.begin(), ranges::distance(line));
            })
                    | ranges::views::enumerate
                    | ranges::to_vector;
        };

        [[nodiscard]] static Instruction<SSV_T, T> extractCmd(const Line& GCodeline)
        {
#ifndef NDEBUG
            std::cout << GCodeline.first << ": " << GCodeline.second << std::endl; // Todo: Use a proper logger
#endif

            auto &&[lineno, gcode] = GCodeline;

            // Split line in instruction and comment
            auto split_line = gcode
                              | ranges::views::split(';');

            // Split the instruction into individual words
            auto split_instruction = *split_line.begin()
                                     | ranges::views::split(' ');

            // Get the Cmd
            auto cmd = split_instruction
                       | ranges::views::take(1)
                       | ranges::views::transform([&](auto&& c) {
                return std::string_view(&*c.begin(), ranges::distance(c));
            });

            // Get the values and parameters
            // TODO: use std::from_char instead of copying to string
            auto params = split_instruction
                          | ranges::views::drop(1)
                          | ranges::views::transform([](auto&& param) {
                auto identifier = param
                                  | ranges::views::take(1);
                auto val = param
                           | ranges::views::drop(1);
                auto value = std::string(&*val.begin(), ranges::distance(val));
                return Parameter<T>(std::string_view(&*identifier.begin(), ranges::distance(identifier)),
                                    std::stod(value));
            })
                          | ranges::to_vector;
            return Instruction<SSV_T, T>(lineno, *cmd.begin(), params);
        };

        std::string_view gcode;
        std::vector<Line> lines;
        std::vector<Instruction<SSV_T, T>> cmdlines;
        std::mutex cmdlines_mutex;
    };
}

#endif //GHERMENEUS_MACHINE_H
