#pragma once

#include <string>
#include <utility>

#include "Save.hpp"

std::unique_ptr<std::string> JuiceCalc(const SaveData& _data);
