//
// Created by Jelle Spijker on 7/31/20.
//

#ifndef GCODEHERMENEUS_GCODE_H
#define GCODEHERMENEUS_GCODE_H

#include <unordered_map>
#include <string_view>
#include <functional>

#include "Parameters.h"

namespace GHermeneus
{
    using Line = std::pair<size_t, std::string_view>;

    template<typename SSV_T, typename T>
    using GCodeFunction = std::function<SSV_T(const SSV_T&, const Parameters<T>&)>;

    template<typename GCFUNC_T>
    using CmdMap = std::unordered_map<std::string_view, GCFUNC_T>;

    template<typename GCFUNC_T>
    using ParamMap = std::unordered_map<std::string_view, GCFUNC_T>; // Todo this should probably not be a GCFUNC_T

    template<typename GCFUNC_T>
    class Transform
    {
    public:
        Transform<GCFUNC_T>(const CmdMap<GCFUNC_T>& cmd, const ParamMap<GCFUNC_T>& param) :
                cmdMap(cmd), paramMap(param)
        {};

        GCFUNC_T Cmd(const std::string_view& key)
        {
            return cmdMap.at(key);
        };

        GCFUNC_T Param(const std::string_view& key)
        {
            return paramMap.at(key);
        };

    protected:
        const CmdMap<GCFUNC_T> cmdMap;

        const ParamMap<GCFUNC_T> paramMap;
    };
}

#endif //GCODEHERMENEUS_GCODE_H
