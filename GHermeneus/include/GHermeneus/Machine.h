//
// Created by peer23peer on 5/22/20.
//

#ifndef GHERMENEUS_MACHINE_H
#define GHERMENEUS_MACHINE_H

#include <string>
#include <vector>
#include <iostream>
#include <mutex>

namespace GHermeneus
{
    enum Cmd : size_t
    {
        G0 = 119,
        G1 = 120,
        M140 = 208
    };

    enum Param : char
    {
        E = 'E',
        F = 'F',
        S = 'S',
        X = 'X',
        Y = 'Y',
        Z = 'Z',
    };

    struct Parameter
    {
        Parameter(const Param& param, const double& value)
                : param(param), value(value)
        {};
        const Param param;
        const double value;
    };

    using CmdLine = std::tuple<size_t, Cmd, std::vector<Parameter>>;

    class Machine
    {
    public:
        using GCodeLine = std::pair<size_t, std::string_view>;

        Machine() = default;

        void parse(const std::string_view& gcode);

        friend std::ostream& operator<<(std::ostream& os, const Machine& machine);

        friend Machine& operator<<(Machine& machine, const std::string_view& gcode);

    private:

        void extractLines(const std::string_view& gcode);

        [[nodiscard]] CmdLine extractCmd(const GCodeLine& gcodeline);

        [[nodiscard]] size_t cmdHash(const std::string_view& cmd);

        std::string_view gcode;
        std::vector<GCodeLine> lines;
        std::vector<CmdLine> cmdlines;
        std::mutex cmdlines_mutex;

    };
}
#endif //GHERMENEUS_MACHINE_H
