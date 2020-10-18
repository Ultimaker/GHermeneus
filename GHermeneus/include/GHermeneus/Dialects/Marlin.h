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
#include "GHermeneus/Translator.h"
#include "GHermeneus/Primitive.h"

namespace GHermeneus
{
using MarlinPrimitive = Primitive<double>;
constexpr int MarlinStateVariables = 10;
using MarlinParameters = Parameters<MarlinPrimitive>; // n = t, x, y, z, x_dot, y_dot, z_dot, e, e_dot, T
using MarlinSSV = StateSpaceVector<MarlinPrimitive, MarlinStateVariables>;
using MarlinTranslator = Translator<MarlinPrimitive, MarlinStateVariables>;
using MarlinMachine = Machine<MarlinTranslator>;
}


#endif // GCODEHERMENEUS_MARLIN_H
