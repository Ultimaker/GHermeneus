//
// Created by Jelle Spijker on 7/31/20.
//

#include "../include/GHermeneus/Instruction.h"

namespace GHermeneus
{
    bool GHermeneus::Instruction::operator<(const GHermeneus::Instruction& rhs) const
    {
        return line_no < rhs.line_no;
    }

    bool GHermeneus::Instruction::operator>(const GHermeneus::Instruction& rhs) const
    {
        return rhs < *this;
    }

    bool GHermeneus::Instruction::operator<=(const GHermeneus::Instruction& rhs) const
    {
        return !(rhs < *this);
    }

    bool GHermeneus::Instruction::operator>=(const GHermeneus::Instruction& rhs) const
    {
        return !(*this < rhs);
    }
}