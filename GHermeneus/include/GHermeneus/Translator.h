//
// Created by Jelle Spijker on 9/13/20.
//

#ifndef GCODEHERMENEUS_TRANSLATOR_H
#define GCODEHERMENEUS_TRANSLATOR_H

#include "GHermeneus/GCode.h"

namespace GHermeneus
{
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
class Translator
{
  public:
    using value_type = T;
    constexpr static int Size = n;

    Translator(const CmdMap<T, n>& cmd, const ParamMap<T, n>& param) : m_command_map(cmd), m_parameter_map(param){};

    /*!
     * @brief Returns the associated GCodeFunction for the requested cmd key
     * @param key the key as a string_view
     * @return GCodeFunction which will be used for the transformation of the previous State Space Vector
     */
    GCodeFunction<T, n> Cmd(const std::string_view& key)
    {
        return m_command_map.at(key);
    };

    /*!
     * Todo: this will probably change
     * @brief
     * @param key
     * @return
     */
    GCodeFunction<T, n> Param(const std::string_view& key)
    {
        return m_parameter_map.at(key);
    };

  protected:
    const CmdMap<T, n> m_command_map; //<! The command map containing the key and the corresponding function

    const ParamMap<T, n> m_parameter_map;
};
} // namespace GHermeneus

#endif // GCODEHERMENEUS_TRANSLATOR_H
