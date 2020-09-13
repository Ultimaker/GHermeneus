//
// Created by Jelle Spijker on 8/7/20.
//

#ifndef GCODEHERMENEUS_STATESPACEVECTOR_H
#define GCODEHERMENEUS_STATESPACEVECTOR_H

#include <Eigen/Dense>

#include "GHermeneus/Utils/Concepts.h"

namespace GHermeneus
{
/*!
 * The State Space Vector consists of the variables that describe the state of the machine at
 * a certain time, these are usually:
 *  - t: time (this is a required parameter, since the GCode only contains movements and actions)
 *  - x: x coordinates of the printhead relative to 0
 *  - y: y coordinates of the printhead relative to 0
 *  - z: z coordinates of the printhead relative to the bed
 *  - x_dot: velocity of the printhead
 *  - y_dot: velocity of the printhead
 *  - z_dot: velocity of the bed
 *  - e: the extrusion position
 *  - e_dot: the velocity of the extrusion
 *  - T: temperature of the liquefier
 *  - T_bed: temperature of the bed
 *
 * @brief The State Space Vector describing the machine state
 * @tparam T the primitive type of the Eigen::Matrix
 * @tparam n the number of variables describing the state
 */
template <typename T, int n> // n = t, x, y, z, x_dot, y_dot, z_dot, e, e_dot, T, T_bed
requires primitive<T> using StateSpaceVector = Eigen::Matrix<T, n, 1>;
} // namespace GHermeneus

#endif // GCODEHERMENEUS_STATESPACEVECTOR_H
