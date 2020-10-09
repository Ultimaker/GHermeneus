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
#include <utility>
#include <vector>

#include <range/v3/distance.hpp>
#include <range/v3/range/operations.hpp>
#include <range/v3/view/drop.hpp>
#include <range/v3/view/enumerate.hpp>
#include <range/v3/view/sliding.hpp>
#include <range/v3/view/split.hpp>
#include <range/v3/view/take.hpp>
#include <range/v3/view/transform.hpp>

#include <spdlog/spdlog.h>

#include "GHermeneus/Instruction.h"
#include "GHermeneus/Parameters.h"
#include "GHermeneus/Translator.h"
#include "GHermeneus/Utils/Concepts.h"

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
template <typename TRANS>
requires is_derived<Translator<typename TRANS::value_type, TRANS::Size>, TRANS>
class Machine
{
  public:
    using primitive_t = typename TRANS::value_type;
    using parameter_t = Parameter<primitive_t>;
    using ssv_t = StateSpaceVector<primitive_t, TRANS::Size>;
    using ssvs_t = std::vector<ssv_t>;
    using instruction_t = Instruction<primitive_t, TRANS::Size>;
    using instructions_t = std::vector<instruction_t>;
    using gcode_function_t = GCodeFunction<primitive_t, TRANS::Size>;

    Machine() : m_translator{ TRANS() }, m_parallel_execution{ true } {};

    /*!
     * @brief parse the GCode to the machine instruction vector
     * @param GCode a string_view containing the GCode
     */
    void parse(std::string_view gcode)
    {
        spdlog::info("*** PARSING GCODE ***");
        m_gcode = gcode;
        auto lines = extractLines(m_gcode);           // extract the individual GCode lines
        auto instructions = interpretLines(lines);    // Extract the Commands from each line
        m_ssvs = translateInstructions(instructions); // Translate the Instructions to a vector of SSV
    }

    /*!
     * @brief output the machines instruction vector to an output stream as in CSV format
     * @param os Output stream
     * @param machine A machine of type Machine<TRANS_T, SSV_T, T>
     * @return an output stream
     */
    friend std::ostream& operator<<(std::ostream& os, const Machine<TRANS>& machine)
    {
        for (const auto& ssv : machine.m_ssvs) {}
        return os;
    };

    /*!
     * @brief Output the GCode stream to the Machines Instruction Vector
     * @param machine The machine of type Machine<TRANS_T, SSV_T, T>
     * @param GCode the GCode as a string_view stream
     * @return A machine of type Machine<TRANS_T, SSV_T, T>
     */
    friend Machine<TRANS>& operator<<(Machine<TRANS>& machine, const std::string_view& GCode)
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
    friend Machine<TRANS>& operator<<(Machine<TRANS>& machine, std::ifstream& file)
    {
        machine.m_raw_gcode.assign((std::istreambuf_iterator<char>(file)), (std::istreambuf_iterator<char>()));
        std::string_view gcode{ machine.m_raw_gcode };
        machine << gcode;
        return machine;
    }

    /*!
     * @brief Allow for parallel execution, standard on
     * @param parallelExecution true if parallel execution is allowed, false for sequenced execution
     */
    void setParallelExecution(bool parallelExecution)
    {
        spdlog::info("Setting parallel execution mode t {}", parallelExecution);
        m_parallel_execution = parallelExecution;
    }

  private:
    /*!
     * @brief extract the individual lines from the GCode
     * @param GCode a string_view containing the GCode
     */
    [[nodiscard]] std::vector<Line> extractLines(std::string_view GCode)
    {
        spdlog::info("*** EXTRACTING LINES ***");
        // TODO: keep in mind CRLF and LF
        return GCode | ranges::views::split('\n') | ranges::views::transform([](auto&& line) {
                   return std::string_view(&*line.begin(), static_cast<size_t>(ranges::distance(line)));
               })
             | ranges::views::enumerate | ranges::to_vector;
    };

    /*!
     * @brief Interpret the Extracted GCode lines in Machine Instructions
     * @param lines a vector of GCode lines
     * @return A vector of all interpreted instructions obtained from the GCode lines
     */
    [[nodiscard]] auto interpretLines(const std::vector<Line>& lines) const
    {
        spdlog::info("*** INTERPRETING LINES ***");
        std::vector<std::optional<instruction_t>> extracted_commands(lines.size());
        if (m_parallel_execution)
        {
            std::transform(std::execution::par, lines.begin(), lines.end(), extracted_commands.begin(), extractCmd);
        }
        else
        {
            std::transform(std::execution::seq, lines.begin(), lines.end(), extracted_commands.begin(), extractCmd);
        }
        extracted_commands.erase(std::remove_if(std::execution::seq, extracted_commands.begin(),
                                                extracted_commands.end(), [](const auto& cmd) { return !cmd; }),
                                 extracted_commands.end());
        instructions_t commands(extracted_commands.size());
        if (m_parallel_execution) [[likely]]
        {
            std::transform(std::execution::par, extracted_commands.begin(), extracted_commands.end(), commands.begin(),
                           [](const auto& cmd) { return cmd.value(); });
            std::sort(std::execution::par, commands.begin(), commands.end());
        }
        else [[unlikely]]
        {
            std::transform(std::execution::seq, extracted_commands.begin(), extracted_commands.end(), commands.begin(),
                           [](const auto& cmd) { return cmd.value(); });
        }
        assert(
            ("The extracted commands should be sorted by line nr", std::is_sorted(commands.begin(), commands.end())));
        spdlog::info("Lines processed: {}", lines.size());
        spdlog::info("Instructions interpreted: {}", commands.size());
        spdlog::info("Amount of lines not interpreted: {}", lines.size() - commands.size());
        return commands;
    }

    /*!
     * @brief Extract an instruction of type Instruction<SSV_T, T> from a GCode line. If the line contains no
     *  instruction it is a std::nullopt
     * @param gcode_line string_view containing the line with command, the relevant parameters and/or a comment.
     * @return an optional instruction of type Instruction<SSV_T, T>
     */
    [[nodiscard]] static std::optional<instruction_t> extractCmd(const Line& gcode_line)
    {
        auto&& [lineno, gcode] = gcode_line;

        // Split line in instruction and comment
        auto split_line = gcode | ranges::views::split(';');

        // Split the instruction into individual words
        auto split_instruction = *split_line.begin() | ranges::views::split(' ');

        // Get the Cmd
        auto cmd_view = split_instruction | ranges::views::take(1) | ranges::views::transform([&](auto&& c) {
                            return std::string_view(&*c.begin(), static_cast<size_t>(ranges::distance(c)));
                        });
        std::string_view cmd = *cmd_view.begin();
        // Todo: also take into account comments that should be treated as a cmd
        if (cmd.empty()) // No cmd nothing to do here
        {
            spdlog::warn("No command discerned for line {}, containing: {}", gcode_line.first, gcode_line.second);
            return std::nullopt;
        }

        // Get the values and parameters
        // TODO: use std::from_char instead of copying to string
        auto params = split_instruction | ranges::views::drop(1) | ranges::views::transform([](auto&& param) {
                          auto identifier = param | ranges::views::take(1);
                          auto val = param | ranges::views::drop(1);
                          auto value = std::string(&*val.begin(), static_cast<size_t>(ranges::distance(val)));
                          return parameter_t(
                              std::string_view(&*identifier.begin(), static_cast<size_t>(ranges::distance(identifier))),
                              std::stod(value));
                      }) // Todo: how to process the conversion of text to different T types
                    | ranges::to_vector;
        return instruction_t(lineno, cmd, params);
    };

    [[nodiscard]] ssv_t initialState()
    {
        return ssv_t::Zero();
    }

    [[nodiscard]] ssvs_t translateInstructions(const instructions_t& instructions)
    {
        spdlog::info("*** TRANSLATING INSTRUCTIONS ***");
        ssvs_t ssvs(instructions.size());
        ssvs[0] = initialState();
        // Todo: make range
        for (int i = 1; i < instructions.size(); ++i)
        {
            ssv_t ssv_prev = ssvs[i - 1];
            auto ssv_delta = m_translator(instructions[i]);
            ssvs[i] = ssv_prev + ssv_delta;
        }
        return ssvs;
    }

    TRANS m_translator;        //!< The dialect translator
    bool m_parallel_execution; //!< Indicating if parsing should be done in parallel
    ssvs_t m_ssvs;             //!< A vector of the state space vectors
    std::string m_raw_gcode; //!< The raw GCode (needed to store files and make sure the data of \p gcode stays in scope
    std::string_view m_gcode; //!< A string_view with the full gcode
};
} // namespace GHermeneus

#endif // GHERMENEUS_MACHINE_H
