//
// Created by Jelle Spijker on 8/7/20.
//

#include "../../include/GHermeneus/Dialects/Marlin.h"

namespace GHermeneus::Dialects::Marlin
{
MarlinSSV G0(const MarlinSSV& prev, const MarlinParameters& param)
{
    // Todo: transform the previous state space vector into the next according to the param
    return MarlinSSV();
}

MarlinSSV G92(const MarlinSSV& prev, const MarlinParameters& param)
{
    // Todo: transform the previous state space vector into the next according to the param
    return MarlinSSV();
}
} // namespace GHermeneus::Dialects::Marlin
