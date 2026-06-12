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
    bool recipeDirty = false;
    std::optional<std::filesystem::path> currentSavePath = std::nullopt;
    rz::json::Saveable<GuicyConfig> appCfg;
};
