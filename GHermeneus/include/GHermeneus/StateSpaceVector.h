//
// Created by Jelle Spijker on 8/7/20.
//

#ifndef GCODEHERMENEUS_STATESPACEVECTOR_H
#define GCODEHERMENEUS_STATESPACEVECTOR_H

#include <Eigen/Dense>

namespace GHermeneus
{
    template<typename T, int n = 11>
    using StateSpaceVector = Eigen::Matrix<T, n, 1>;

    using SSV = StateSpaceVector<double>;
}

#endif //GCODEHERMENEUS_STATESPACEVECTOR_H
