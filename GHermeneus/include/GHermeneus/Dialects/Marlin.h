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
    using MarlinParameters = Parameters<double>;
    using MarlinSSV = StateSpaceVector<double, 10>; // n = t, x, y, z, x_dot, y_dot, z_dot, e, e_dot, T

    static MarlinSSV G0(const MarlinSSV& prev, const MarlinParameters& param);

    static MarlinSSV G92(const MarlinSSV& prev, const MarlinParameters& param);

    class MarlinTransform : public Transform<GCodeFunction<MarlinSSV, double>>
    {
    public:
        MarlinTransform() :
                Transform<GCodeFunction<MarlinSSV, double>>(
                 {{"G0", G0},
                       {"G1", G0},
                       {"G92", G92}},
                {{"temp", G0}})
        {};

    };

    using MarlinMachine = Machine<MarlinSSV, double>;

}

#endif //GCODEHERMENEUS_MARLIN_H
