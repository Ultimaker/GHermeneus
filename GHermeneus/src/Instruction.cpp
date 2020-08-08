//
// Created by Jelle Spijker on 7/31/20.
//

#include "../include/GHermeneus/Instruction.h"

namespace GHermeneus
{
    template<typename T>
    bool Instruction<T>::operator<(const Instruction<T>& rhs) const
    {
        return line_no < rhs.line_no;
    }

    template<typename T>
    bool Instruction<T>::operator>(const Instruction<T>& rhs) const
    {
        return rhs < *this;
    }

    template<typename T>
    bool Instruction<T>::operator<=(const Instruction<T>& rhs) const
    {
        return !(rhs < *this);
    }

    template<typename T>
    bool Instruction<T>::operator>=(const Instruction<T>& rhs) const
    {
        return !(*this < rhs);
    }
}