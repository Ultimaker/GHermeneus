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

#include "Instruction.h"

namespace GHermeneus
{
    template<typename T>
    class Machine
    {
    public:
        using GCodeLine = std::pair<size_t, std::string_view>;

        Machine() = default;

        void parse(const std::string_view& gcode);

        friend std::ostream& operator<<(std::ostream& os, const Machine<T>& machine);

        friend Machine<T>& operator<<(Machine<T>& machine, const std::string_view& gcode);

    private:

        void extractLines(const std::string_view& gcode);

        [[nodiscard]] static Instruction<T> extractCmd(const GCodeLine& gcodeline);

        std::string_view gcode;
        std::vector<GCodeLine> lines;
        std::vector<Instruction<T>> cmdlines;
        std::mutex cmdlines_mutex;

    };
}
#endif //GHERMENEUS_MACHINE_H
