//
// Created by Jelle Spijker on 8/7/20.
//

#ifndef GCODEHERMENEUS_STATESPACEVECTOR_H
#define GCODEHERMENEUS_STATESPACEVECTOR_H

#include <Eigen/Dense>

namespace GHermeneus
{
    template<typename T, int n = 10> // n = t, x, y, z, x_dot, y_dot, z_dot, e, e_dot, T
    using StateSpaceVector = Eigen::Matrix<T, n, 1>;
}

#endif //GCODEHERMENEUS_STATESPACEVECTOR_H
