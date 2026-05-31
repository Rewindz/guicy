#pragma once

#include <array>
#include <string>

#include <rz/rzutils.hpp>

struct GuicyConfig
{
    std::array<std::string, 4> recentSaves;

    void saveNextRecent(const std::string& _path)
    {
        for(auto& recent : recentSaves){
            if(recent == _path)
                return;
            if(recent.empty()){
                recent = _path;
                return;
            }
        }
        recentSaves[3] = recentSaves[2]; recentSaves[2] = recentSaves[1];
        recentSaves[1] = recentSaves[0]; recentSaves[0] = _path;
    }
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_WITH_DEFAULT(GuicyConfig, recentSaves)
