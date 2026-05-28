#include "GuicyFrame.hpp"

#include <wx/event.h>
#include <wx/gdicmn.h>
#include <wx/panel.h>
#include <wx/sizer.h>
#include <wx/string.h>
#include <wx/spinctrl.h>
#include <wx/statbox.h>

GuicyFrame::GuicyFrame(const wxString& title) : wxFrame(nullptr, wxID_ANY, title)
{
    auto* rootPanel = new wxPanel(this);
    auto* rootSizer = new wxBoxSizer(wxHORIZONTAL);

    auto* leftColSizer = new wxBoxSizer(wxVERTICAL);
    auto* batchSizer = new wxStaticBoxSizer(wxVERTICAL, rootPanel, "Batch");

    auto* batchTargetML =
        new wxSpinCtrlDouble(batchSizer->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0.0, 10000.0, 30.0, 1.0);

    batchSizer->Add(batchTargetML, 1, wxEXPAND, 0);

    auto* nicSizer = new wxStaticBoxSizer(wxVERTICAL, rootPanel, "Base");
    auto* targetSizer = new wxStaticBoxSizer(wxVERTICAL, rootPanel, "Target");

    leftColSizer->Add(batchSizer, 0, wxEXPAND | wxBOTTOM, 10);
    leftColSizer->Add(nicSizer, 0, wxEXPAND | wxBOTTOM, 10);
    leftColSizer->Add(targetSizer, 0, wxEXPAND, 0);

    auto* rightColSizer = new wxBoxSizer(wxVERTICAL);
    auto* flavoursSizer = new wxStaticBoxSizer(wxVERTICAL, rootPanel, "Flavours");

    rightColSizer->Add(flavoursSizer, 1, wxEXPAND, 0);

    rootSizer->Add(leftColSizer, 1, wxEXPAND | wxRIGHT, 15);
    rootSizer->Add(rightColSizer, 1, wxEXPAND, 0);

    auto* paddingSizer = new wxBoxSizer(wxVERTICAL);
    paddingSizer->Add(rootSizer, 1, wxEXPAND | wxALL, 20);

    rootPanel->SetSizerAndFit(paddingSizer);
    paddingSizer->SetSizeHints(this);
}
