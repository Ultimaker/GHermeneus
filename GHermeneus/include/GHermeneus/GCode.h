//
// Created by Jelle Spijker on 7/31/20.
//

#ifndef GCODEHERMENEUS_GCODE_H
#define GCODEHERMENEUS_GCODE_H

#include <functional>
#include <string_view>
#include <unordered_map>

#include "GHermeneus/Parameters.h"
#include "GHermeneus/StateSpaceVector.h"
#include "GHermeneus/Utils/Concepts.h"

namespace GHermeneus
{
/*!
 * @brief An extracted GCode line containing the line_nr and the GCode line as a string_view
 */
using Line = std::pair<size_t, std::string_view>;

/*!
 * @brief The Function type for where the conversion from specific GCode command takes place, which will transform
 * a previous State Space Function, given as parameter, to a new parameter. These are dialect specific.
 * @tparam SSV_T State Space Vector Type of the dialect
 * @tparam T primitive type used in the State Space Vector and the parameters
 */
template <typename T, int n>
requires primitive<T> using GCodeFunction =
    std::function<StateSpaceVector<T, n>(const StateSpaceVector<T, n>&, const Parameters<T>&)>;

/*!
 * @brief An Unorded map with a lookup key for a specific dialect GCode command (e.q. G0, T1, M104) and a GCodeFunction
 * @tparam GCFUNC_T the GCodeFunction type
 */
template <typename T, int n>
using CmdMap = std::unordered_map<std::string_view, GCodeFunction<T, n>>;

/*!
 * Todo: this current format will probably not be used
 * @brief
 */
template <typename T, int n>
using ParamMap =
    std::unordered_map<std::string_view, GCodeFunction<T, n>>; // Todo this should probably not be a GCFUNC_T


} // namespace GHermeneus

#endif // GCODEHERMENEUS_GCODE_H
