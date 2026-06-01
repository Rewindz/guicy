#include "Calculation.hpp"
#include <map>
#include <format>

/*     DENSITY IS GRAMS PER ML (g/mL)     */
constexpr double PG_DENSITY      = 1.0373;
constexpr double VG_DENSITY      = 1.2613;
constexpr double NIC_DENSITY     = 1.00925;
constexpr double WATER_DENSITY   = 0.9982;
constexpr double FLAVOUR_DENSITY = 1.06;

/*
 * @param vgRatio normalized ratio of the vg percent of the base carrier
 * (eg. 80%VG base vgRatio=0.80)
 */
double CalcNicBaseDensity(double mgmlStr, double vgRatio)
{
    double nicGrams = mgmlStr / 1000.0;
    double nicVol = nicGrams / NIC_DENSITY;
    double baseVol = 1.0 - nicVol;
    double pgRatio = 1.0 - vgRatio;
    double vgGrams = (baseVol * vgRatio) * VG_DENSITY;
    double pgGrams = (baseVol * pgRatio) * PG_DENSITY;
    return nicGrams + vgGrams + pgGrams;
}

std::string JuiceCalc(const SaveData& _data)
{
    std::string res = "";

    double workingNicStr = _data.nicStr;
    double workingTargetStr = _data.targetStr;
    if(_data.nicUnit == NicUnit::VOLUME){
        double pureNicMgPerMl = NIC_DENSITY * 1000.0;
        workingNicStr = (workingNicStr / 100.0) * pureNicMgPerMl;
        workingTargetStr = (workingTargetStr / 100.0) * pureNicMgPerMl;
    }

    double baseNicDensity = CalcNicBaseDensity(workingNicStr, _data.nicVG / 100.0);

    double targetBaseVol = (_data.targetVol * workingTargetStr) / workingNicStr;
    double targetBaseMass = targetBaseVol * baseNicDensity;

    double nicVol = (_data.targetVol * workingTargetStr) / (1000.0 * NIC_DENSITY);
    double carrierVol = targetBaseVol - nicVol;
    double nicVGVol = carrierVol * (_data.nicVG / 100.0);
    double nicPGVol = carrierVol * (_data.nicPG / 100.0);

    double totalFlavourVol = 0.0;
    double totalFlavourMass = 0.0;
    double totalFlavourVG = 0.0;
    double totalFlavourPG = 0.0;

    std::string flavoursOutput = "";

    for(const auto& flavour : _data.flavours)
    {
        double flavourVol = _data.targetVol * (flavour.percent / 100.0);
        double flavourMass = flavourVol * FLAVOUR_DENSITY;

        flavoursOutput += std::format(" {:>13}: {:^6.2f}mL | {:^6.2f}g\n",
            flavour.name, flavourVol, flavourMass);

        double flavourPG = flavour.pg;
        double flavourVG = flavour.vg;
        if(flavourVG == 0.0 && flavourPG == 0.0)
            flavourPG = 100.0;

        totalFlavourVol += flavourVol;
        totalFlavourMass += flavourMass;
        totalFlavourVG += flavourVol * (flavourVG / 100.0);
        totalFlavourPG += flavourVol * (flavourPG / 100.0);
    }

    double ratioBaseVol = _data.targetVol - nicVol;
    double vgVolToAdd = (ratioBaseVol * (_data.targetVG / 100.0)) - nicVGVol - totalFlavourVG;
    double pgVolToAdd = (ratioBaseVol * (_data.targetPG / 100.0)) - nicVol - nicPGVol - totalFlavourPG;
    double vgMassToAdd = vgVolToAdd * VG_DENSITY;
    double pgMassToAdd = pgVolToAdd * PG_DENSITY;

    double finalVol = targetBaseVol + totalFlavourVol + vgVolToAdd + pgVolToAdd;
    double finalMass = targetBaseMass + totalFlavourMass + vgMassToAdd + pgMassToAdd;

    res = std::format(
      " Recipe {}\n"
      "=====================================\n"
      " Nicotine Base: {:^6.2f}mL | {:^6.2f}g\n"
      "{}"
      "            VG: {:^6.2f}mL | {:^6.2f}g\n"
      "            PG: {:^6.2f}mL | {:^6.2f}g\n"
      "=====================================\n"
      "         Total: {:^6.2f}mL | {:^6.2f}g\n",
      _data.name,
      targetBaseVol, targetBaseMass,
      flavoursOutput,
      vgVolToAdd, vgMassToAdd,
      pgVolToAdd, pgMassToAdd,
      finalVol, finalMass
    );


    return res;
}
