//
// Created by Jelle Spijker on 8/7/20.
//

#include "../../include/GHermeneus/Dialects/Marlin.h"

namespace GHermeneus::Dialects::Marlin
{

    SSV Marlin::G0(const SSV& prev, const Parameters& param)
    {
        // Todo: transform the previous state space vector into the next according to the param
        return GHermeneus::SSV();
    }

    SSV G92(const SSV& prev, const Parameters& param)
    {
        // Todo: transform the previous state space vector into the next according to the param
        return GHermeneus::SSV();
    }
}
