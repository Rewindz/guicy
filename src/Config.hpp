#pragma once

#include <array>
#include <string>
#include <algorithm>
#include <ranges>

#include <rz/rzutils.hpp>

struct GuicyConfig
{
    std::array<std::string, 4> recentSaves;

    void saveNextRecent(const std::string& _path)
    {
        if(auto it = std::ranges::find(recentSaves, _path); it != recentSaves.end())
            return;

        std::shift_right(recentSaves.begin(), recentSaves.end(), 1);
        recentSaves[0] = _path;
    }
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_WITH_DEFAULT(GuicyConfig, recentSaves)
