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
using MarlinPrimitive = double;
constexpr int MarlinStateVariables = 10;
using MarlinParameters = Parameters<MarlinPrimitive>; // n = t, x, y, z, x_dot, y_dot, z_dot, e, e_dot, T
using MarlinSSV = StateSpaceVector<MarlinPrimitive, MarlinStateVariables>;

class MarlinTranslator : public Translator<MarlinPrimitive, MarlinStateVariables>
{
  public:
    MarlinTranslator()
        : Translator<MarlinPrimitive, MarlinStateVariables>({ { "G0", G0 }, { "G1", G0 }, { "G92", G92 } },
                                                            { { "temp", G0 } }){};

  protected:
    static MarlinSSV G0(const MarlinSSV& prev, const MarlinParameters& param);

    static MarlinSSV G92(const MarlinSSV& prev, const MarlinParameters& param);
};

using MarlinMachine = Machine<MarlinTranslator>;

} // namespace GHermeneus::Dialects::Marlin

#endif // GCODEHERMENEUS_MARLIN_H
