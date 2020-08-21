//
// Created by Jelle Spijker on 7/31/20.
//

#ifndef GCODEHERMENEUS_GCODE_H
#define GCODEHERMENEUS_GCODE_H

#include <unordered_map>
#include <string_view>
#include <functional>

#include "Parameters.h"

namespace GHermeneus
{
    /*!
     * @brief An extracted GCode line containing the line_nr and the GCode line as a string_view
     */
    using Line = std::pair<size_t, std::string_view>;

    /*!
     * @brief The Function type for where the conversion from specific GCode command takes place, which while transform
     * a previous State Space Function, given as parameter, to a new parameter. These are dialect specific.
     * @tparam SSV_T State Space Vector Type of the dialect
     * @tparam T primative type used in the State Space Vector and the parameters
     */
    template<typename SSV_T, typename T>
    using GCodeFunction = std::function<SSV_T(const SSV_T&, const Parameters<T>&)>;

    /*!
     * @brief An Unorded map with a lookup key for a specific dialect GCode command (e.q. G0, T1, M104) and a GCodeFucntion
     * @tparam GCFUNC_T the GCodeFunction type
     */
    template<typename GCFUNC_T>
    using CmdMap = std::unordered_map<std::string_view, GCFUNC_T>;

    /*!
     * Todo: this current format will probably not be used
     * @brief
     */
    template<typename GCFUNC_T>
    using ParamMap = std::unordered_map<std::string_view, GCFUNC_T>; // Todo this should probably not be a GCFUNC_T

    /*!
     *
     * Each dialect has to define a class which inherits from this on, the mapping of the keys takes place in the
     * constructor:
     *
     * static MarlinSSV G0(const MarlinSSV& prev, const MarlinParameters& param);
     * static MarlinSSV G92(const MarlinSSV& prev, const MarlinParameters& param);
     *
     * class MarlinTransform : public Transform<GCodeFunction<MarlinSSV, double>>
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
     * @brief The dialect transform class, Each dialect has its own transform class, which maps the GCode commands to
     * The GCodeFunctions
     * @tparam GCFUNC_T The GCodeFunction type used by the dialect.
     */
    template<typename GCFUNC_T>
    class Transform
    {
    public:
        Transform<GCFUNC_T>(const CmdMap<GCFUNC_T>& cmd, const ParamMap<GCFUNC_T>& param) :
                cmdMap(cmd), paramMap(param)
        {};

        /*!
         * @brief Returns the associated GCodeFunction for the requested cmd key
         * @param key the key as a string_view
         * @return GCodeFunction which will be used for the transformation of the previous State Space Vector
         */
        GCFUNC_T Cmd(const std::string_view& key)
        {
            return cmdMap.at(key);
        };

        /*!
         * Todo: this will probably change
         * @brief
         * @param key
         * @return
         */
        GCFUNC_T Param(const std::string_view& key)
        {
            return paramMap.at(key);
        };

    protected:
        const CmdMap<GCFUNC_T> cmdMap; //<! The command map containing the key and the corresponding function

        const ParamMap<GCFUNC_T> paramMap;
    };
}

#endif //GCODEHERMENEUS_GCODE_H
