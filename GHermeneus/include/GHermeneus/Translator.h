//
// Created by Jelle Spijker on 9/13/20.
//

#ifndef GCODEHERMENEUS_TRANSLATOR_H
#define GCODEHERMENEUS_TRANSLATOR_H

#include <functional>
#include <unordered_map>
#include <string_view>

#include <range/v3/view/map.hpp>
#include <range/v3/algorithm/contains.hpp>

#include "GHermeneus/Utils/Concepts.h"
#include "GHermeneus/Instruction.h"
#include "GHermeneus/StateSpaceVector.h"

using namespace std::literals::string_view_literals;

namespace GHermeneus
{

/*!
 * @brief The Function type for where the conversion from specific GCode command takes place, which will transform
 * a previous State Space Function, given as parameter, to a new parameter. These are dialect specific.
 * @tparam SSV_T State Space Vector Type of the dialect
 * @tparam T primitive type used in the State Space Vector and the parameters
 */
template <typename T, int n>
requires primitive<T> && at_least_one_scalar<n> using GCodeFunction =
std::function<StateSpaceVector<T, n>(const Parameters<T>& parameters)>;

/*!
 * @brief An Unorded map with a lookup key for a specific dialect GCode command (e.q. G0, T1, M104) and a GCodeFunction
 * @tparam GCFUNC_T the GCodeFunction type
 */
template <typename T, int n>
requires primitive<T> && at_least_one_scalar<n>
using CmdMap = std::unordered_map<std::string_view, GCodeFunction<T, n>>;

/*!
 * Todo: this current format will probably not be used
 * @brief
 */
template <typename T, int n>
requires primitive<T> && at_least_one_scalar<n>
using ParamMap = std::unordered_map<std::string_view, GCodeFunction<T, n>>;

/*!
 *
 * Each dialect has to define a class which inherits from this on, the mapping of the keys takes place in the
 * constructor:
 *
 * static MarlinSSV G0(const MarlinSSV& prev, const MarlinParameters& param);
 * static MarlinSSV G92(const MarlinSSV& prev, const MarlinParameters& param);
 *
 * class MarlinTranslator : public Translator<GCodeFunction<MarlinSSV, double>>
 * {
 * public:
 *  MarlinTransform() :
 *           Transform<GCodeFunction<MarlinSSV, double>>(
 *                   {{"G0",  G0},
 *                    {"G1",  G0},
 *                    {"G92", G92}},
 *                   {{"temp", G0}})
 *   {};};
 *
 * @brief The dialect translator class, Each dialect has its own translator class, which maps the GCode commands to
 * The GCodeFunctions
 * @tparam GCFUNC_T The GCodeFunction type used by the dialect.
 */
template <typename T, int n>
requires primitive<T> && at_least_one_scalar<n>
class Translator
{
  public:
    using value_type [[maybe_unused]] = T;
    constexpr static int Size = n;

    Translator<T, n>() = default;

  protected:

    /*!
     * @brief Non-Print linear move
     *
     * Description:
     * The G0 and G1 commands add a linear move to the queue to be performed after all previous moves are completed.
     * These commands yield control back to the command parser as soon as the move is queued, but they may delay the
     * command parser while awaiting a slot in the queue.
     *
     * A linear move traces a straight line from one point to another, ensuring that the specified axes will arrive
     * simultaneously at the given coordinates (by linear interpolation). The speed may change over time following an
     * acceleration curve, according to the acceleration and jerk settings of the given axes.
     *
     * A command like G1 F1000 sets the feedrate for all subsequent moves.
     *
     * By convention, most G-code generators use G0 for non-extrusion movements (those without the E axis) and G1 for
     * moves that include extrusion. This is meant to allow a kinematic system to, optionally, do a more rapid
     * uninterpolated movement requiring much less calculation.
     *
     * For Cartesians and Deltas the G0 (rapid linear movement) command is (and must be) a direct alias for G1 (rapid
     * movement). On SCARA machines G0 does a fast non-linear move. Marlin 2.0 introduces an option to maintain a
     * separate default feedrate for G0. Note: Slicers tend to override firmware feedrates!
     *
     * Notes:
     *
     * Coordinates are given in millimeters by default. Units may be set to inches by G20.
     * In Relative Mode (G91) all coordinates are interpreted as relative, adding onto the previous position.
     * A single linear move may generate several smaller moves to the planner due to kinematics and bed leveling
     * compensation. Printing performance can be tuned by adjusting segments-per-second.
     *
     * Developer Notes:
     * Developers: Keep using G0 for non-print moves. It makes G-code more adaptable to lasers, engravers, etc.
     *
     * Usage:
     * G0 [E<pos>] [F<rate>] [X<pos>] [Y<pos>] [Z<pos>]
     * Parameters
     * [E<pos>]
     * The length of filament to feed into the extruder between the start and end point
     *
     * [F<rate>]
     * The maximum movement rate of the move between the start and end point. The feedrate set here applies to
     * ubsequent moves that omit this parameter.
     *
     * [X<pos>]
     * A coordinate on the X axis
     *
     * [Y<pos>]
     * A coordinate on the Y axis
     *
     * [Z<pos>]
     * A coordinate on the Z axis
     *
     * Source: https://marlinfw.org/docs/gcode/G000-G001.html
     *
     * @param parameters The extracted parameters
     * @return A delta StateSpaceVector
     */
    [[nodiscard, maybe_unused]] static StateSpaceVector<T, n> G0(const Parameters<T>& parameters)
    {
        return StateSpaceVector<T, n>::Ones();
    }

    /*!
     * @brief Print linear move
     *
     * Description:
     * The G0 and G1 commands add a linear move to the queue to be performed after all previous moves are completed.
     * These commands yield control back to the command parser as soon as the move is queued, but they may delay the
     * command parser while awaiting a slot in the queue.
     *
     * A linear move traces a straight line from one point to another, ensuring that the specified axes will arrive
     * simultaneously at the given coordinates (by linear interpolation). The speed may change over time following an
     * acceleration curve, according to the acceleration and jerk settings of the given axes.
     *
     * A command like G1 F1000 sets the feedrate for all subsequent moves.
     *
     * By convention, most G-code generators use G0 for non-extrusion movements (those without the E axis) and G1 for
     * moves that include extrusion. This is meant to allow a kinematic system to, optionally, do a more rapid
     * uninterpolated movement requiring much less calculation.
     *
     * For Cartesians and Deltas the G0 (rapid linear movement) command is (and must be) a direct alias for G1 (rapid
     * movement). On SCARA machines G0 does a fast non-linear move. Marlin 2.0 introduces an option to maintain a
     * separate default feedrate for G0. Note: Slicers tend to override firmware feedrates!
     *
     * Notes:
     *
     * Coordinates are given in millimeters by default. Units may be set to inches by G20.
     * In Relative Mode (G91) all coordinates are interpreted as relative, adding onto the previous position.
     * A single linear move may generate several smaller moves to the planner due to kinematics and bed leveling
     * compensation. Printing performance can be tuned by adjusting segments-per-second.
     *
     * Developer Notes:
     * Developers: Keep using G0 for non-print moves. It makes G-code more adaptable to lasers, engravers, etc.
     *
     * Usage:
     * G0 [E<pos>] [F<rate>] [X<pos>] [Y<pos>] [Z<pos>]
     * Parameters
     * [E<pos>]
     * The length of filament to feed into the extruder between the start and end point
     *
     * [F<rate>]
     * The maximum movement rate of the move between the start and end point. The feedrate set here applies to
     * subsequent moves that omit this parameter.
     *
     * [X<pos>]
     * A coordinate on the X axis
     *
     * [Y<pos>]
     * A coordinate on the Y axis
     *
     * [Z<pos>]
     * A coordinate on the Z axis
     *
     * Source: https://marlinfw.org/docs/gcode/G000-G001.html
     *
     * @param parameters The extracted parameters
     * @return A delta StateSpaceVector
     */
    [[nodiscard, maybe_unused]] static StateSpaceVector<T, n> G1(const Parameters<T>& parameters)
    {
        return G0(parameters);
    }

    /*!
     * @brief Set Axis Steps-per-unit
     *
     * Description
     * Use M92 to set the steps-per-unit for one or more axes. This setting affects how many steps will be done for
     * each unit of movement. Units will be in steps/mm unless inch mode is set with G20 (which requires
     * INCH_MODE_SUPPORT).
     *
     * Notes
     * Get the current steps-per-unit settings with M503.
     *
     * With EEPROM_SETTINGS enabled:
     *
     * This setting for all axes is saved with M500 and loaded with M501.
     * M502 resets steps-per-unit for all axes to the values from DEFAULT_AXIS_STEPS_PER_UNIT.
     *
     * Usage
     * M92 [E<steps>] [T<index>] [X<steps>] [Y<steps>] [Z<steps>]
     * Parameters
     * [E<steps>]
     * E steps per unit
     *
     * [T<index>]
     * Target extruder (Requires DISTINCT_E_FACTORS)
     *
     * [X<steps>]
     * X steps per unit
     *
     * [Y<steps>]
     * Y steps per unit
     *
     * [Z<steps>]
     * Z steps per unit
     *
     * Source: https://marlinfw.org/docs/gcode/M092.html
     *
     * @param parameters The extracted parameters
     * @return A delta StateSpaceVector
     */
    [[nodiscard, maybe_unused]] static StateSpaceVector<T, n> G92(const Parameters<T>& parameters)
    {
        return StateSpaceVector<T, n>::Ones();
    }

    /*!
     * @brief Set Starting Acceleration
     *
     * Description
     * Set the preferred starting acceleration for moves of different types.
     *
     * Notes
     * View the current setting with M503.
     *
     * If EEPROM_SETTINGS is enabled, these are saved with M500, loaded with M501, and reset with M502.
     *
     * Legacy M204 S<accel> is deprecated. Use separate paremeters M204 P<accel> T<accel> instead.
     *
     * Usage
     * M204 [P<accel>] [R<accel>] [S<accel>] [T<accel>]
     * Parameters
     * [P<accel>]
     * Printing acceleration. Used for moves that include extrusion (i.e., which employ the current tool).
     *
     * [R<accel>]
     * Retract acceleration. Used for extruder retraction moves.
     *
     * [S<accel>]
     * Legacy parameter for move acceleration. Set both printing and travel acceleration.
     *
     * [T<accel>]
     * Travel acceleration. Used for moves that include no extrusion.
     *
     * Source: https://marlinfw.org/docs/gcode/M204.html
     *
     * @param parameters The extracted parameters
     * @return A delta StateSpaceVector
     */
    [[nodiscard, maybe_unused]] static StateSpaceVector<T, n> M204(const Parameters<T>& parameters)
    {
        return StateSpaceVector<T, n>::Ones();
    }

    /*!
     * @brief Set Starting Acceleration
     *
     * Description:
     * Set the preferred starting acceleration for moves of different types.
     *
     * Notes:
     * View the current setting with M503.
     *
     * If EEPROM_SETTINGS is enabled, these are saved with M500, loaded with M501, and reset with M502.
     *
     * Legacy M204 S<accel> is deprecated. Use separate paremeters M204 P<accel> T<accel> instead.
     *
     * Usage:
     * M204 [P<accel>] [R<accel>] [S<accel>] [T<accel>]
     * Parameters
     * [P<accel>]
     * Printing acceleration. Used for moves that include extrusion (i.e., which employ the current tool).
     *
     * [R<accel>]
     * Retract acceleration. Used for extruder retraction moves.
     *
     * [S<accel>]
     * Legacy parameter for move acceleration. Set both printing and travel acceleration.
     *
     * [T<accel>]
     * Travel acceleration. Used for moves that include no extrusion.
     *
     * Source: https://marlinfw.org/docs/gcode/M205.html
     *
     * @param parameters The extracted parameters
     * @return A delta StateSpaceVector
     */
    [[nodiscard, maybe_unused]] static StateSpaceVector<T, n> M205(const Parameters<T>& parameters)
    {
        return StateSpaceVector<T, n>::Ones();
    }

    /*!
     * @brief default function used for unknown commands
     * @param parameters The extracted parameters
     * @return A delta StateSpaceVector of zeros
     */
    [[nodiscard, maybe_unused]] static StateSpaceVector<T, n> default_func(const Parameters<T>& parameters)
    {
        return StateSpaceVector<T, n>::Zero();
    }

    static const inline CmdMap<T, n> m_command_map{
        { "G0"sv, &Translator<T, n>::G0 },
        { "G1"sv, &Translator<T, n>::G1 },
        { "G92"sv, &Translator<T, n>::G92 },
        { "M204"sv, &Translator<T, n>::M204 },
        { "M205"sv, &Translator<T, n>::M205 }
    };

  public:
    /*!
     * @brief Translate the instruction into a delta StateSpaceVector
     * @param instruction The extracted instruction from the GCode line
     * @return The delta StateSpaceVector obtained from the instruction
     */
    [[nodiscard]] StateSpaceVector<T, n> operator()(const Instruction<T, n>& instruction)
    {
        const auto contains_cmd = ranges::contains(m_command_map | ranges::views::keys, instruction.cmd);
        auto cmd = contains_cmd ? m_command_map.at(instruction.cmd) : default_func;
        return cmd(instruction.params);
    }
};

} // namespace GHermeneus

#endif // GCODEHERMENEUS_TRANSLATOR_H
