//
// Created by Jelle Spijker on 8/7/20.
//

#ifndef GCODEHERMENEUS_MARLIN_H
#define GCODEHERMENEUS_MARLIN_H

#include "../StateSpaceVector.h"
#include "../Instruction.h"
#include "../GCode.h"
#include "../Machine.h"

namespace GHermeneus::Dialects::Marlin
{
    static SSV G0(const SSV& prev, const Parameters& param);

    static SSV G92(const SSV& prev, const Parameters& param);

    class MarlinTransform : public Transform<GCodeFunction<SSV>>
    {
        // Todo: fix initialization of unordered_map using the initializer list
        MarlinTransform() :
                cmdMap{{"G0": G0},
                       {"G1": G0},
                       {"G92": G92}},
                paramMap{}
        {};

    };

    using MarlinMachine = Machine<SSV>;

}

#endif //GCODEHERMENEUS_MARLIN_H
