//
// Created by Jelle Spijker on 9/18/20.
//

#ifndef GCODEHERMENEUS_CONSTEXPRMAP_H
#define GCODEHERMENEUS_CONSTEXPRMAP_H

#include <array>
#include <algorithm>

namespace GHermeneus
{
template <typename Key, typename Value, std::size_t Size>
struct Map
{
    std::array<std::pair<Key, Value>, Size> data;

    [[nodiscard]] constexpr Value at(const Key& key) const
    {
        const auto itr = std::find_if(begin(data), end(data), [&key](const auto& v) { return v.first == key; });
        if (itr != end(data))
        {
            return itr->second;
        }
        else
        {
            throw std::range_error("Not Found");
        }
    }
};
}
#endif // GCODEHERMENEUS_CONSTEXPRMAP_H
