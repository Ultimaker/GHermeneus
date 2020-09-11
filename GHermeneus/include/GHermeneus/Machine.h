//
// Created by peer23peer on 5/22/20.
//

#ifndef GHERMENEUS_MACHINE_H
#define GHERMENEUS_MACHINE_H

#include <algorithm>
#include <execution>
#include <fstream>
#include <iostream>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include <range/v3/distance.hpp>
#include <range/v3/range/operations.hpp>
#include <range/v3/view/drop.hpp>
#include <range/v3/view/enumerate.hpp>
#include <range/v3/view/split.hpp>
#include <range/v3/view/take.hpp>
#include <range/v3/view/transform.hpp>

#include "GCode.h"
#include "Instruction.h"
#include "Parameters.h"

namespace GHermeneus
{
/*!
 * This is the workhorse, here the GCode is processed and converted to different output types,
 * such as protobufs, CSV text streams, Numpy objects.
 *
 * Actual implementation of this abstract type is usually done for each type GCode dialect. A Dialect is a namespace
 * such as the GHermeneus::Dialects::Marlin. Here the Machine is defined as:
 *
 *      using MarlinSSV = StateSpaceVector<double, 10>; // n = t, x, y, z, x_dot, y_dot, z_dot, e, e_dot, T
 *      using MarlinMachine = Machine<MarlinSSV, double>;
 *
 * Normal usage of a DialectMachine would then be:
 *
 *      using namespace GHermeneus::Dialects::Marlin;
 *      auto UM3 = MarlinMachine();
 *      UM3 << gcode;
 *      std::cout << UM3;
 *
 * Which will output a CSV formatted array of the instructions (not yet implemented fully)
 *
 * @brief The workhorse behind this parser it will convert GCode to Instructions and different outputs
 * @tparam SSV_T The type of the State Space Vector
 * @tparam T  The primitive type of the State Space eq. double, int etc
 */
template <typename TRANS_T, typename SSV_T, typename T>
class Machine
{
  public:
    Machine() : parallel_execution{ true } {};

    Machine(Machine<TRANS_T, SSV_T, T>&& machine) noexcept = default;

    virtual ~Machine() = default;

    /*!
     * @brief parse the GCode to the machine instruction vector
     * @param GCode a string_view containing the GCode
     */
    void parse(const std::string_view& GCode)
    {
        gcode = GCode;
        extractLines(GCode); // extract the individual GCode lines

        // Extract the Commands from each line
        std::vector<std::optional<Instruction<SSV_T, T>>> extractedCmds(lines.size());
        if (parallel_execution)
        {
            std::transform(std::execution::par, lines.begin(), lines.end(), extractedCmds.begin(), extractCmd);
        }
        else
        {
            std::transform(std::execution::seq, lines.begin(), lines.end(), extractedCmds.begin(), extractCmd);
        }
        extractedCmds.erase(std::remove_if(std::execution::seq, extractedCmds.begin(), extractedCmds.end(),
                                           [](const auto& cmd) { return !cmd; }),
                            extractedCmds.end());
        cmdlines.resize(extractedCmds.size());
        if (parallel_execution)
        {
            std::transform(std::execution::par, extractedCmds.begin(), extractedCmds.end(), cmdlines.begin(),
                           [](const auto& cmd) { return cmd.value(); });
            std::sort(std::execution::par, cmdlines.begin(), cmdlines.end());
        }
        else
        {
            std::transform(std::execution::seq, extractedCmds.begin(), extractedCmds.end(), cmdlines.begin(),
                           [](const auto& cmd) { return cmd.value(); });
        }
    }

    /*!
     * @brief output the machines instruction vector to an output stream as in CSV format
     * @param os Output stream
     * @param machine A machine of type Machine<TRANS_T, SSV_T, T>
     * @return an output stream
     */
    friend std::ostream& operator<<(std::ostream& os, const Machine<TRANS_T, SSV_T, T>& machine)
    {
        for (const Instruction<SSV_T, T>& cmdline : machine.cmdlines)
        {
            os << "line: " << cmdline.line_no << " command: " << cmdline.cmd << " Parameters ->";
            for (const Parameter<T>& param : cmdline.params)
            {
                os << " " << param.param << " = " << std::to_string(param.value);
            }
            os << std::endl;
        }
        return os;
    };

    /*!
     * @brief Output the GCode stream to the Machines Instruction Vector
     * @param machine The machine of type Machine<TRANS_T, SSV_T, T>
     * @param GCode the GCode as a string_view stream
     * @return A machine of type Machine<TRANS_T, SSV_T, T>
     */
    friend Machine<TRANS_T, SSV_T, T>& operator<<(Machine<TRANS_T, SSV_T, T>& machine, const std::string_view& GCode)
    {
        machine.parse(GCode);
        return machine;
    };

    /*!
     * @brief Output the GCode filestream to the Machines Instruction Vector
     * @param machine The machine of type Machine<TRANS_T, SSV_T, T>
     * @param file Text file stream
     * @return A machine of type Machine<TRANS_T, SSV_T, T>
     */
    friend Machine<TRANS_T, SSV_T, T>& operator<<(Machine<TRANS_T, SSV_T, T>& machine, std::ifstream& file)
    {
        machine.raw_gcode.assign((std::istreambuf_iterator<char>(file)), (std::istreambuf_iterator<char>()));
        std::string_view gcode{ machine.raw_gcode };
        machine << gcode;
        return machine;
    }

    /*!
     * @brief Allow for parallel execution, standard on
     * @param parallelExecution true if parallel execution is allowed, false for sequenced execution
     */
    void setParallelExecution(bool parallelExecution)
    {
        Machine::parallel_execution = parallel_execution;
    }

  private:
    /*!
     * @brief extract the individual lines from the GCode
     * @param GCode a string_view containing the GCode
     */
    void extractLines(const std::string_view& GCode)
    {
        // TODO: keep in mind CRLF and LF
        lines = gcode | ranges::views::split('\n') | ranges::views::transform([](auto&& line) {
                    return std::string_view(&*line.begin(), ranges::distance(line));
                })
                | ranges::views::enumerate | ranges::to_vector;
    };

    /*!
     * @brief Extract an instruction of type Instruction<SSV_T, T> from a GCode line. If the line contains no
     *  instruction it is a std::nullopt
     * @param GCodeline string_view containing the line with command, the relevant parameters and/or a comment.
     * @return an optional instruction of type Instruction<SSV_T, T>
     */
    [[nodiscard]] static std::optional<Instruction<SSV_T, T>> extractCmd(const Line& GCodeline)
    {
#ifndef NDEBUG
        std::cout << GCodeline.first << ": " << GCodeline.second << std::endl; // Todo: Use a proper logger like SPDlog
#endif
        auto&& [lineno, gcode] = GCodeline;

        // Split line in instruction and comment
        auto split_line = gcode | ranges::views::split(';');

        // Split the instruction into individual words
        auto split_instruction = *split_line.begin() | ranges::views::split(' ');

        // Get the Cmd
        auto cmd_view = split_instruction | ranges::views::take(1) | ranges::views::transform([&](auto&& c) {
                            return std::string_view(&*c.begin(), ranges::distance(c));
                        });
        std::string_view cmd = *cmd_view.begin();
        // Todo: also take into account comments that should be treated as a cmd
        if (cmd.empty()) // No cmd nothing to do here
        {
            return std::nullopt;
        }

        // Get the values and parameters
        // TODO: use std::from_char instead of copying to string
        auto params = split_instruction | ranges::views::drop(1) | ranges::views::transform([](auto&& param) {
                          auto identifier = param | ranges::views::take(1);
                          auto val = param | ranges::views::drop(1);
                          auto value = std::string(&*val.begin(), ranges::distance(val));
                          return Parameter<T>(std::string_view(&*identifier.begin(), ranges::distance(identifier)),
                                              std::stod(value));
                      }) // Todo: how to process the conversion of text to different T types
                      | ranges::to_vector;
        return Instruction<SSV_T, T>(lineno, cmd, params);
    };

    std::string raw_gcode;   //!< The raw GCode (needed to store files and make sure the data of \p gcode stays in scope
    std::string_view gcode;  //!< A string_view with the full gcode
    std::vector<Line> lines; //!< A vector of Lines, a Line is a pair with the line number and the string_view
    std::vector<Instruction<SSV_T, T>> cmdlines; //!< A vector of instructions converted from the lines
    bool parallel_execution;                     //!< Indicating if parsing should be done in parallel
    const TRANS_T translator;                    //!< The dialect translator
};
} // namespace GHermeneus

#endif // GHERMENEUS_MACHINE_H
