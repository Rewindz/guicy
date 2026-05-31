#pragma once
#include <filesystem>
#include <optional>

#include <wx/frame.h>
#include <rz/json/saveable.hpp>

#include "Config.hpp"

class GuicyFrame : public wxFrame
{
public:
    GuicyFrame(const wxString& title);
private:
    std::optional<std::filesystem::path> currentSavePath = std::nullopt;
    rz::Saveable<GuicyConfig> appCfg;
};
