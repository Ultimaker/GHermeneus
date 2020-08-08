//
// Created by peer23peer on 5/22/20.
//

#include <algorithm>
#include <execution>

#include <range/v3/distance.hpp>
#include <range/v3/view/split.hpp>
#include <range/v3/view/transform.hpp>
#include <range/v3/view/enumerate.hpp>
#include <range/v3/view/take.hpp>
#include <range/v3/view/drop.hpp>

#include "../include/GHermeneus/Machine.h"
#include "../include/GHermeneus/GCode.h"
#include "../include/GHermeneus/Parameters.h"

namespace GHermeneus
{
    template<typename SSV_T, typename T>
    std::ostream& operator<<(std::ostream& os, const Machine<SSV_T, T>& machine)
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
    }

    template<typename SSV_T, typename T>
    Machine<SSV_T, T>& operator<<(Machine<SSV_T, T>& machine, const std::string_view& GCode)
    {
        machine.parse(GCode);
        return machine;
    }

    template<typename SSV_T, typename T>
    void Machine<SSV_T, T>::parse(const std::string_view& GCode)
    {
        extractLines(GCode);
        cmdlines.reserve(lines.size());
        std::for_each(std::execution::par_unseq, lines.begin(), lines.end(), [&](auto line){
            std::lock_guard<std::mutex> guard(cmdlines_mutex);
            cmdlines.emplace_back(extractCmd(line));
        });
        std::sort(std::execution::par_unseq, cmdlines.begin(), cmdlines.end()); // Todo: check performance of diff execution policies
    }

    template<typename SSV_T, typename T>
    void Machine<SSV_T, T>::extractLines(const std::string_view& GCode)
    {
        // TODO: keep in mind CRLF and LF
        lines = gcode
                | ranges::views::split('\n')
                | ranges::views::transform([](auto&& line) {
            return std::string_view(&*line.begin(), ranges::distance(line));
        })
                | ranges::views::enumerate
                | ranges::to_vector;
    }

    template<typename SSV_T, typename T>
    Instruction<SSV_T, T> Machine<SSV_T, T>::extractCmd(const Line& GCodeline)
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
            return Parameter<T>(std::string_view(&*identifier.begin(), ranges::distance(identifier)), std::stod(value));
        })
                      | ranges::to_vector;
        return std::make_tuple(lineno, *cmd.begin(), params);
    }
}