#pragma once
#include <filesystem>
#include <optional>

#include <wx/frame.h>


class GuicyFrame : public wxFrame
{
public:
    GuicyFrame(const wxString& title);
private:
    std::optional<std::filesystem::path> currentSavePath = std::nullopt;
};
