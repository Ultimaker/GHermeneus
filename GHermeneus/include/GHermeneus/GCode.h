//
// Created by Jelle Spijker on 7/31/20.
//

#ifndef GCODEHERMENEUS_GCODE_H
#define GCODEHERMENEUS_GCODE_H

#include <unordered_map>
#include <string_view>

namespace GHermeneus::GCode
{
    using CmdMap_t = std::unordered_map<std::string_view, int>; // Todo map to cmd_handler_functions
    using ParamMap_t = std::unordered_map<std::string_view, int>; // Todo map to cmd_handler_functions

    static const CmdMap_t Cmd
            {
                    {"G0",  1},
                    {"G1",  2},
                    {"G92", 3},
                    {"M82", 4},
                    {"T0",  5}
            };

    static const ParamMap_t Param
            {
                    {"E", 1},
                    {"F", 1},
                    {"S", 1},
                    {"X", 1},
                    {"Y", 1},
                    {"Z", 1}
            };

}

#endif //GCODEHERMENEUS_GCODE_H
