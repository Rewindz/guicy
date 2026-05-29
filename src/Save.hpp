#pragma once
#include "nlohmann/json.hpp"
#include <string>
#include <vector>

#include <rz/json/json.hpp>

enum class NicUnit
{
    VOLUME = 0,
    WEIGHT
};

NLOHMANN_JSON_SERIALIZE_ENUM(NicUnit,
    {
        {NicUnit::VOLUME, "VOLUME"},
        {NicUnit::WEIGHT, "WEIGHT"}
    })

struct FlavourData
{
    std::string name;
    double percent = 0.0, vg = 0.0, pg = 0.0;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_WITH_DEFAULT(FlavourData, name, percent, vg, pg)

struct SaveData
{
    std::string name;
    double targetVol = 0.0, nicStr = 0.0, nicVG = 0.0,
        nicPG = 0.0, targetStr = 0.0, targetVG = 0.0, targetPG = 0.0;
    NicUnit nicUnit;
    std::vector<FlavourData> flavours;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_WITH_DEFAULT(SaveData, name, targetVol, nicStr, nicVG, nicPG,
    targetStr, targetVG, targetPG, nicUnit, flavours)
