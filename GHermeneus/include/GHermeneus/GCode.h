//
// Created by Jelle Spijker on 7/31/20.
//

#ifndef GCODEHERMENEUS_GCODE_H
#define GCODEHERMENEUS_GCODE_H

#include <string_view>

namespace GHermeneus
{
/*!
 * @brief An extracted GCode line containing the line_nr and the GCode line as a string_view
 */
using Line = std::pair<size_t, std::string_view>;

} // namespace GHermeneus

#endif // GCODEHERMENEUS_GCODE_H
