//
// Created by peer23peer on 5/22/20.
//

#ifndef GHERMENEUS_MACHINE_H
#define GHERMENEUS_MACHINE_H

#include <string>
#include <vector>
#include <iostream>
#include <mutex>
#include <string_view>

namespace GHermeneus
{
    struct Parameter
    {
        Parameter(const std::string_view& param, const double& value)
                : param(param), value(value)
        {};
        const std::string_view param;
        const double value;
    };

    using CmdLine = std::tuple<size_t, std::string_view, std::vector<Parameter>>;

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

        [[nodiscard]] static CmdLine extractCmd(const GCodeLine& gcodeline);

        std::string_view gcode;
        std::vector<GCodeLine> lines;
        std::vector<CmdLine> cmdlines;
        std::mutex cmdlines_mutex;

    };
}
#endif //GHERMENEUS_MACHINE_H
