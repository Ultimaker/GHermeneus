//
// Created by Jelle Spijker on 7/31/20.
//

#include "../include/GHermeneus/GCode.h"

namespace GHermeneus::GCode
{
    template<typename T>
    T Transform<T>::Cmd(const std::string_view& key)
    {
        return cmdMap.at(key);
    }

    template<typename T>
    T Transform<T>::Param(const std::string_view& key)
    {
        return paramMap.at(key);
    }
}
