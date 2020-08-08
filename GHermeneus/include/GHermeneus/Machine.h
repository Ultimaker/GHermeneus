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
#include "GCode.h"

namespace GHermeneus
{
    template<typename SSV_T, typename T>
    class Machine
    {
    public:

        Machine() = default;

        void parse(const std::string_view& GCode);

        friend std::ostream& operator<<(std::ostream& os, const Machine<SSV_T, T>& machine);

        friend Machine<SSV_T, T>& operator<<(Machine<SSV_T, T>& machine, const std::string_view& GCode);

    private:

        void extractLines(const std::string_view& GCode);

        [[nodiscard]] static Instruction<SSV_T, T> extractCmd(const Line& GCodeline);

        std::string_view gcode;
        std::vector<Line> lines;
        std::vector<Instruction<SSV_T, T>> cmdlines;
        std::mutex cmdlines_mutex;
    };
}
#endif //GHERMENEUS_MACHINE_H
