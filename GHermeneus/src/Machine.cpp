//
// Created by peer23peer on 5/22/20.
//

#include "../include/GHermeneus/Machine.h"

#include <algorithm>
#include <execution>

#include <range/v3/distance.hpp>
#include <range/v3/numeric/accumulate.hpp>
#include <range/v3/view/split.hpp>
#include <range/v3/view/transform.hpp>
#include <range/v3/view/enumerate.hpp>
#include <range/v3/view/take.hpp>
#include <range/v3/view/drop.hpp>

namespace GHermeneus
{
    std::ostream& operator<<(std::ostream& os, const Machine& machine)
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

    Machine& operator<<(Machine& machine, const std::string_view& gcode)
    {
        machine.parse(gcode);
        return machine;
    }

    void Machine::parse(const std::string_view& gcode)
    {
        extractLines(gcode);
        cmdlines.reserve(lines.size());
        std::for_each(std::execution::par_unseq, lines.begin(), lines.end(), [&](auto line){
            std::lock_guard<std::mutex> guard(cmdlines_mutex);
            cmdlines.push_back(extractCmd(line));
        });
        // TODO: Sort according to line number, or maybe use std::execution::par
    }

    void Machine::extractLines(const std::string_view& gcode)
    {
        // TODO: keep in mind CRLF and LF
        lines = gcode
                | ranges::view::split('\n')
                | ranges::view::transform([](auto&& line) {
            return std::string_view(&*line.begin(), ranges::distance(line));
        })
        | ranges::view::enumerate
        | ranges::to_vector;
    }

    CmdLine Machine::extractCmd(const GCodeLine& gcodeline)
    {
        // TODO: split comment
        auto &&[lineno, gcode] = gcodeline;
        // Split the line into individual words
        auto split_words = gcode
                | ranges::views::split(' ');

        // Get the Cmd enum
        auto cmd = split_words
                | ranges::views::take(1)
                | ranges::views::transform([&](auto&& c) {
            return static_cast<Cmd>(cmdHash(std::string_view(&*c.begin(), ranges::distance(c))));
        });

        // Get the values and parameters
        // TODO: use std::from_char instead of copying to string
        auto params = split_words
                | ranges::views::drop(1)
                | ranges::views::transform([](auto&& param) {
                    auto identifier = param
                            | ranges::views::take(1);
                    auto val = param
                            | ranges::views::drop(1);
                    auto value = std::string(&*val.begin(), ranges::distance(val));
                    return Parameter(static_cast<Param>(*identifier.begin()), std::stod(value));
                })
                | ranges::to_vector;
        return std::make_tuple(lineno, *cmd.begin(), params);
    }

    size_t Machine::cmdHash(const std::string_view& cmd)
    {
        return static_cast<size_t>(ranges::accumulate(cmd, 0));
    }
}