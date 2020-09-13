//
// Created by Jelle Spijker on 8/7/20.
//

#ifndef GCODEHERMENEUS_MARLIN_H
#define GCODEHERMENEUS_MARLIN_H

#include <functional>
#include <string_view>
#include <vector>

#include <Eigen/Core>

#include "GHermeneus/GCode.h"
#include "GHermeneus/Machine.h"
#include "GHermeneus/StateSpaceVector.h"

namespace GHermeneus::Dialects::Marlin
{
using MarlinParameters = Parameters<double>;
using MarlinSSV = StateSpaceVector<double, 10>; // n = t, x, y, z, x_dot, y_dot, z_dot, e, e_dot, T

class MarlinTranslator : public Translator<GCodeFunction<MarlinSSV, double>>
{
  public:
    MarlinTranslator()
        : Translator<GCodeFunction<MarlinSSV, double>>({ { "G0", G0 }, { "G1", G0 }, { "G92", G92 } },
                                                      { { "temp", G0 } }){};

  protected:
    static MarlinSSV G0(const MarlinSSV& prev, const MarlinParameters& param);

    static MarlinSSV G92(const MarlinSSV& prev, const MarlinParameters& param);
};

using MarlinMachine = Machine<MarlinTranslator, MarlinSSV, double>;

} // namespace GHermeneus::Dialects::Marlin

#endif // GCODEHERMENEUS_MARLIN_H
