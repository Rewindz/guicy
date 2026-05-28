#include "GuicyFrame.hpp"

#include <wx/event.h>
#include <wx/gdicmn.h>
#include <wx/combobox.h>
#include <wx/radiobut.h>
#include <wx/panel.h>
#include <wx/sizer.h>
#include <wx/string.h>
#include <wx/spinctrl.h>
#include <wx/statbox.h>
#include <wx/stattext.h>
#include <wx/stringimpl.h>

GuicyFrame::GuicyFrame(const wxString& title) : wxFrame(nullptr, wxID_ANY, title)
{

    auto lblCtrl = [](wxWindow* parent, const wxString& labelText, wxWindow* control) -> wxBoxSizer*
        {
            auto* sizer = new wxBoxSizer(wxHORIZONTAL);
            auto* label = new wxStaticText(parent, wxID_ANY, labelText, wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT);
            label->SetMinSize(wxSize(120, -1));
            sizer->Add(label, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 10);
            sizer->Add(control, 1, wxEXPAND, 0);
            return sizer;
        };

    auto* rootPanel = new wxPanel(this);
    auto* rootSizer = new wxBoxSizer(wxHORIZONTAL);

    auto* leftColSizer = new wxBoxSizer(wxVERTICAL);
    auto* batchSizer = new wxStaticBoxSizer(wxVERTICAL, rootPanel, "Batch");

    auto* batchTargetML =
        new wxSpinCtrlDouble(batchSizer->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0.0, 10000.0, 30.0, 1.0);

    wxString nicStrOpts[] = {"Volume (%)", "Weight (mg/mL)"};
    auto* nicStrSel =
        new wxComboBox(batchSizer->GetStaticBox(), wxID_ANY, nicStrOpts[1], wxDefaultPosition, wxDefaultSize, 2, nicStrOpts, wxCB_READONLY);

    batchSizer->Add(lblCtrl(batchSizer->GetStaticBox(), "Target mL", batchTargetML), 1, wxEXPAND | wxALL, 5);
    batchSizer->Add(lblCtrl(batchSizer->GetStaticBox(), "Nic Strength Unit", nicStrSel), 1, wxEXPAND | wxALL, 5);

    auto* nicSizer = new wxStaticBoxSizer(wxVERTICAL, rootPanel, "Base");
    auto* nicStrVal =
        new wxSpinCtrlDouble(nicSizer->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0.0, 100.0, 10.0, 1.0);
    auto* nicVGVal =
        new wxSpinCtrlDouble(nicSizer->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0.0, 100.0, 0.0, 1.0);
    auto* nicPGVal =
        new wxSpinCtrlDouble(nicSizer->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0.0, 100.0, 100.0, 1.0);

    auto nicVGPG_EVT = [nicVGVal, nicPGVal](wxSpinDoubleEvent& event) -> void
        {
            if(event.GetEventObject() == nicVGVal){
                nicPGVal->SetValue(100.0 - event.GetValue());
            } else {
                nicVGVal->SetValue(100.0 - event.GetValue());
            }
        };

    nicVGVal->Bind(wxEVT_SPINCTRLDOUBLE, nicVGPG_EVT);
    nicPGVal->Bind(wxEVT_SPINCTRLDOUBLE, nicVGPG_EVT);

    nicSizer->Add(lblCtrl(nicSizer->GetStaticBox(), "Nic Strength", nicStrVal), 1, wxEXPAND | wxALL, 5);
    nicSizer->Add(lblCtrl(nicSizer->GetStaticBox(), "Nic VG%", nicVGVal), 1, wxEXPAND | wxALL, 5);
    nicSizer->Add(lblCtrl(nicSizer->GetStaticBox(), "Nic PG%", nicPGVal), 1, wxEXPAND | wxALL, 5);

    auto* targetSizer = new wxStaticBoxSizer(wxVERTICAL, rootPanel, "Target");
    auto* targetStrVal =
        new wxSpinCtrlDouble(targetSizer->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0.0, 100.0, 10.0, 1.0);
    auto* targetVGVal =
        new wxSpinCtrlDouble(targetSizer->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0.0, 100.0, 70.0, 1.0);
    auto* targetPGVal =
        new wxSpinCtrlDouble(targetSizer->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0.0, 100.0, 30.0, 1.0);

    auto targetVGPG_EVT = [targetVGVal, targetPGVal](wxSpinDoubleEvent& event) -> void
        {
            if(event.GetEventObject() == targetVGVal){
                targetPGVal->SetValue(100.0 - event.GetValue());
            } else {
                targetVGVal->SetValue(100.0 - event.GetValue());
            }
        };

    targetVGVal->Bind(wxEVT_SPINCTRLDOUBLE, nicVGPG_EVT);
    targetPGVal->Bind(wxEVT_SPINCTRLDOUBLE, nicVGPG_EVT);

    targetSizer->Add(lblCtrl(targetSizer->GetStaticBox(), "Strength", targetStrVal), 1, wxEXPAND | wxALL, 5);
    targetSizer->Add(lblCtrl(targetSizer->GetStaticBox(), "VG%", targetVGVal), 1, wxEXPAND | wxALL, 5);
    targetSizer->Add(lblCtrl(targetSizer->GetStaticBox(), "PG%", targetPGVal), 1, wxEXPAND | wxALL, 5);


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
