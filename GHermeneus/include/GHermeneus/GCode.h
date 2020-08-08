//
// Created by Jelle Spijker on 7/31/20.
//

#ifndef GCODEHERMENEUS_GCODE_H
#define GCODEHERMENEUS_GCODE_H

#include <unordered_map>
#include <string_view>

#include "Instruction.h"

namespace GHermeneus
{
    template<typename T>
    using GCodeFunction = std::function<static T(const T&, const Parameters&)>;

    template<typename T>
    using CmdMap = std::unordered_map<std::string_view, T>;

    template<typename T>
    using ParamMap = std::unordered_map<std::string_view, T>;

    template<typename T>
    class Transform
    {
    public:
        virtual Transform<T>() {};

        T Cmd(const std::string_view& key);

        T Param(const std::string_view& key);

    protected:
        const CmdMap<T> cmdMap;

        const ParamMap<T> paramMap;
    };
}

#endif //GCODEHERMENEUS_GCODE_H
