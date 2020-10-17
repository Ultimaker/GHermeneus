//
// Created by Jelle Spijker on 10/9/20.
//

#ifndef GCODEHERMENEUS_CASSERT_H
#define GCODEHERMENEUS_CASSERT_H

#include <cassert>

#define assertm(exp, msg)  assert(((void)msg, exp))

#endif // GCODEHERMENEUS_CASSERT_H
