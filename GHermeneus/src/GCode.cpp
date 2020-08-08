//
// Created by Jelle Spijker on 7/31/20.
//

#include "../include/GHermeneus/GCode.h"

namespace GHermeneus
{
    template<typename GCFUNC_T>
    GCFUNC_T Transform<GCFUNC_T>::Cmd(const std::string_view& key)
    {
        return cmdMap.at(key);
    }

    template<typename GCFUNC_T>
    GCFUNC_T Transform<GCFUNC_T>::Param(const std::string_view& key)
    {
        return paramMap.at(key);
    }
}
