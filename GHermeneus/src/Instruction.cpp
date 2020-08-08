//
// Created by Jelle Spijker on 7/31/20.
//

#include "../include/GHermeneus/Instruction.h"

namespace GHermeneus
{
    template<typename SSV_T, typename T>
    bool Instruction<SSV_T, T>::operator<(const Instruction<SSV_T, T>& rhs) const
    {
        return line_no < rhs.line_no;
    }

    template<typename SSV_T, typename T>
    bool Instruction<SSV_T, T>::operator>(const Instruction<SSV_T, T>& rhs) const
    {
        return rhs < *this;
    }

    template<typename SSV_T, typename T>
    bool Instruction<SSV_T, T>::operator<=(const Instruction<SSV_T, T>& rhs) const
    {
        return !(rhs < *this);
    }

    template<typename SSV_T, typename T>
    bool Instruction<SSV_T, T>::operator>=(const Instruction<SSV_T, T>& rhs) const
    {
        return !(*this < rhs);
    }

    template<typename SSV_T, typename T>
    SSV_T Instruction<SSV_T, T>::operator()(const Transform<GCodeFunction<SSV_T, T>>& transform)
    {
        return transform.Cmd(cmd, params);
    }
}