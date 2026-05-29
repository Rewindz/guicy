#include "GuicyFrame.hpp"

#include <filesystem>
#include <utility>
#include <string>

#include <wx/event.h>
#include <wx/gdicmn.h>
#include <wx/combobox.h>
#include <wx/listbase.h>
#include <wx/radiobut.h>
#include <wx/panel.h>
#include <wx/sizer.h>
#include <wx/string.h>
#include <wx/spinctrl.h>
#include <wx/statbox.h>
#include <wx/stattext.h>
#include <wx/dataview.h>
#include <wx/button.h>
#include <wx/vector.h>
#include <wx/menu.h>
#include <wx/filedlg.h>
#include <wx/msgdlg.h>
#include <wx/textctrl.h>
#include <wx/font.h>

#include <rz/rzutils.hpp>

#include "Save.hpp"
#include "rz/json/json.hpp"

constexpr double BATCHVOL_DEFAULT = 60.0;
constexpr const char* BATCHNAME_DEFAULT = "Untitled Recipe";

constexpr double NICSTR_DEFAULT = 10.0;
constexpr double NICVG_DEFAULT = 0.0;
constexpr double NICPG_DEFAULT = 100.0;
constexpr int NICUNIT_DEFAULT = std::to_underlying(NicUnit::WEIGHT);

constexpr double TARGETSTR_DEFAULT = 3.0;
constexpr double TARGETVG_DEFAULT = 70.0;
constexpr double TARGETPG_DEFAULT = 30.0;

GuicyFrame::GuicyFrame(const wxString& title) : wxFrame(nullptr, wxID_ANY, title)
{

    auto* menuBar = new wxMenuBar();
    auto* fileMenu = new wxMenu();

    fileMenu->Append(wxID_OPEN, "&Open\tCtrl-O", "Local a saved recipe");
    fileMenu->Append(wxID_SAVE, "&Save\tCtrl-S", "Save the current recipe");
    fileMenu->Append(wxID_SAVEAS, "Save &As..\tCtrl-Shift-S", "Save recipe as a new file");

    fileMenu->AppendSeparator();
    fileMenu->Append(wxID_EXIT, "E&xit\tAlt-X", "Quit the application");

    menuBar->Append(fileMenu, "&File");
    this->SetMenuBar(menuBar);

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

    auto* recipeNameVal =
        new wxTextCtrl(batchSizer->GetStaticBox(), wxID_ANY, BATCHNAME_DEFAULT,
           wxDefaultPosition, wxDefaultSize, wxTE_CAPITALIZE);

    auto* batchTargetML =
        new wxSpinCtrlDouble(batchSizer->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition,
            wxDefaultSize, wxSP_ARROW_KEYS, 0.0, 10000.0, BATCHVOL_DEFAULT, 1.0);

    wxString nicStrOpts[] = {"Volume (%)", "Weight (mg/mL)"};
    auto* nicStrSel =
        new wxComboBox(batchSizer->GetStaticBox(), wxID_ANY, nicStrOpts[NICUNIT_DEFAULT],
            wxDefaultPosition, wxDefaultSize, 2, nicStrOpts, wxCB_READONLY);

    batchSizer->Add(recipeNameVal, 1, wxEXPAND | wxALL, 5);
    batchSizer->Add(lblCtrl(batchSizer->GetStaticBox(), "Target mL", batchTargetML), 1, wxEXPAND | wxALL, 5);
    batchSizer->Add(lblCtrl(batchSizer->GetStaticBox(), "Nic Strength Unit", nicStrSel), 1, wxEXPAND | wxALL, 5);

    auto* nicSizer = new wxStaticBoxSizer(wxVERTICAL, rootPanel, "Base");
    auto* nicStrVal =
        new wxSpinCtrlDouble(nicSizer->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition,
            wxDefaultSize, wxSP_ARROW_KEYS, 0.0, 100.0, NICSTR_DEFAULT, 1.0);
    auto* nicVGVal =
        new wxSpinCtrlDouble(nicSizer->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition,
            wxDefaultSize, wxSP_ARROW_KEYS, 0.0, 100.0, NICVG_DEFAULT, 1.0);
    auto* nicPGVal =
        new wxSpinCtrlDouble(nicSizer->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition,
            wxDefaultSize, wxSP_ARROW_KEYS, 0.0, 100.0, NICPG_DEFAULT, 1.0);

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
        new wxSpinCtrlDouble(targetSizer->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition,
            wxDefaultSize, wxSP_ARROW_KEYS, 0.0, 100.0, TARGETSTR_DEFAULT, 1.0);
    auto* targetVGVal =
        new wxSpinCtrlDouble(targetSizer->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition,
            wxDefaultSize, wxSP_ARROW_KEYS, 0.0, 100.0, TARGETVG_DEFAULT, 1.0);
    auto* targetPGVal =
        new wxSpinCtrlDouble(targetSizer->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition,
            wxDefaultSize, wxSP_ARROW_KEYS, 0.0, 100.0, TARGETPG_DEFAULT, 1.0);

    auto targetVGPG_EVT = [targetVGVal, targetPGVal](wxSpinDoubleEvent& event) -> void
        {
            if(event.GetEventObject() == targetVGVal){
                targetPGVal->SetValue(100.0 - event.GetValue());
            } else {
                targetVGVal->SetValue(100.0 - event.GetValue());
            }
        };

    targetVGVal->Bind(wxEVT_SPINCTRLDOUBLE, targetVGPG_EVT);
    targetPGVal->Bind(wxEVT_SPINCTRLDOUBLE, targetVGPG_EVT);

    targetSizer->Add(lblCtrl(targetSizer->GetStaticBox(), "Strength", targetStrVal), 1, wxEXPAND | wxALL, 5);
    targetSizer->Add(lblCtrl(targetSizer->GetStaticBox(), "VG%", targetVGVal), 1, wxEXPAND | wxALL, 5);
    targetSizer->Add(lblCtrl(targetSizer->GetStaticBox(), "PG%", targetPGVal), 1, wxEXPAND | wxALL, 5);


    leftColSizer->Add(batchSizer, 0, wxEXPAND | wxBOTTOM, 10);
    leftColSizer->Add(nicSizer, 0, wxEXPAND | wxBOTTOM, 10);
    leftColSizer->Add(targetSizer, 0, wxEXPAND, 0);

    auto* rightColSizer = new wxBoxSizer(wxVERTICAL);
    auto* flavoursSizer = new wxStaticBoxSizer(wxVERTICAL, rootPanel, "Flavours");

    auto* flavoursList =
        new wxDataViewListCtrl (flavoursSizer->GetStaticBox(), wxID_ANY);

    flavoursList->AppendTextColumn("Name", wxDATAVIEW_CELL_EDITABLE, 100, wxALIGN_CENTER);
    flavoursList->AppendTextColumn("Percentage", wxDATAVIEW_CELL_EDITABLE, 50, wxALIGN_CENTER);

    flavoursList->Bind(wxEVT_DATAVIEW_ITEM_EDITING_DONE, [](wxDataViewEvent& event){
        if(event.GetColumn() == 1){
            double parsed;
            if(!event.GetValue().GetString().ToDouble(&parsed)){
                event.Veto();
            } else {
                event.SetValue(wxVariant(wxString::Format("%.2f", parsed)));
            }
        }
    });

    auto* flavoursBtnSizer = new wxBoxSizer(wxHORIZONTAL);
    auto* addFlavourBtn = new wxButton(flavoursSizer->GetStaticBox(), wxID_ANY, "Add");
    auto* delFlavourBtn = new wxButton(flavoursSizer->GetStaticBox(), wxID_ANY, "Remove");

    addFlavourBtn->Bind(wxEVT_BUTTON, [flavoursList](wxCommandEvent& event){
        flavoursList->AppendItem({"New Flavour", "0.00"});
    });

    delFlavourBtn->Bind(wxEVT_BUTTON, [flavoursList](wxCommandEvent& event){
        if(int idx = flavoursList->GetSelectedRow(); idx >= 0)
            flavoursList->DeleteItem(idx);
    });

    flavoursBtnSizer->Add(addFlavourBtn, 0, wxRIGHT, 10);
    flavoursBtnSizer->Add(delFlavourBtn, 0, 0, 0);

    flavoursSizer->Add(flavoursList, 1, wxEXPAND | wxALL, 5);
    flavoursSizer->Add(flavoursBtnSizer, 0, wxALIGN_RIGHT | wxLEFT | wxRIGHT | wxBOTTOM , 5);

    auto* submitBtn = new wxButton(rootPanel, wxID_ANY, "Submit");

    auto* resultsSizer = new wxStaticBoxSizer(wxVERTICAL, rootPanel, "Final Recipe");
    auto* resultsText = new wxTextCtrl(resultsSizer->GetStaticBox(), wxID_ANY, wxEmptyString,
        wxDefaultPosition, wxSize(-1, 200), wxTE_MULTILINE | wxTE_READONLY | wxTE_DONTWRAP);

    {
        wxFont monoFont = wxFontInfo(11).Family(wxFONTFAMILY_TELETYPE);
        resultsText->SetFont(monoFont);
    }

    submitBtn->Bind(wxEVT_BUTTON, [=](wxCommandEvent& event){

    });

    resultsSizer->Add(resultsText, 1, wxEXPAND | wxALL, 5);

    rightColSizer->Add(flavoursSizer, 1, wxEXPAND, 0);
    rightColSizer->Add(submitBtn, 0, wxEXPAND | wxTOP, 5);

    rootSizer->Add(leftColSizer, 1, wxEXPAND | wxRIGHT, 15);
    rootSizer->Add(rightColSizer, 1, wxEXPAND, 0);
    rootSizer->Add(resultsSizer, 1, wxEXPAND, 0);

    auto* paddingSizer = new wxBoxSizer(wxVERTICAL);
    paddingSizer->Add(rootSizer, 1, wxEXPAND | wxALL, 20);

    rootPanel->SetSizerAndFit(paddingSizer);
    paddingSizer->SetSizeHints(this);

    auto loadFromSave = [=](const SaveData& saveData) -> void {
        recipeNameVal->SetValue(saveData.name);
        batchTargetML->SetValue(saveData.targetVol);
        nicStrSel->SetValue(nicStrOpts[std::to_underlying(saveData.nicUnit)]);
        nicStrVal->SetValue(saveData.nicStr);
        nicVGVal->SetValue(saveData.nicVG);
        nicPGVal->SetValue(saveData.nicPG);
        targetStrVal->SetValue(saveData.targetStr);
        targetVGVal->SetValue(saveData.targetVG);
        targetPGVal->SetValue(saveData.targetPG);

        for(const auto& flavour : saveData.flavours)
        {
            flavoursList->AppendItem({flavour.name, wxString::Format("%.2f", flavour.percent)});
        }

    };

    auto saveFromWidgets = [=]() -> SaveData {
        SaveData data;
        data.name = recipeNameVal->GetValue().ToStdString();
        data.targetVol = batchTargetML->GetValue();
        data.nicUnit = nicStrSel->GetValue() == nicStrOpts[std::to_underlying(NicUnit::VOLUME)] ?
            NicUnit::VOLUME : NicUnit::WEIGHT;
        data.nicStr = nicStrVal->GetValue();
        data.nicVG = nicVGVal->GetValue();
        data.nicPG = nicPGVal->GetValue();
        data.targetStr = targetStrVal->GetValue();
        data.targetVG = targetVGVal->GetValue();
        data.targetPG = targetPGVal->GetValue();

        for(int row = 0; row < flavoursList->GetItemCount(); row++){
            FlavourData flav;
            flav.name = flavoursList->GetTextValue(row, 0).ToStdString();
            wxString percent = flavoursList->GetTextValue(row, 1);
            percent.ToDouble(&flav.percent);
            data.flavours.push_back(flav);
        }

        return data;
    };

    auto clearWidgets = [=]() -> void {
        recipeNameVal->SetValue(BATCHNAME_DEFAULT);
        batchTargetML->SetValue(BATCHVOL_DEFAULT);
        nicStrSel->SetValue(nicStrOpts[NICUNIT_DEFAULT]);
        nicStrVal->SetValue(NICSTR_DEFAULT);
        nicVGVal->SetValue(NICVG_DEFAULT);
        nicPGVal->SetValue(NICPG_DEFAULT);
        targetStrVal->SetValue(TARGETSTR_DEFAULT);
        targetVGVal->SetValue(TARGETVG_DEFAULT);
        targetPGVal->SetValue(TARGETPG_DEFAULT);
        flavoursList->DeleteAllItems();
    };

    this->Bind(wxEVT_MENU, [this, clearWidgets, loadFromSave](wxCommandEvent& event){
        wxFileDialog openFileDlg(this, "Open Recipe File", "", "",
            "Recipe files (*.rcp)|*.rcp|All files (*.*)|*.*", wxFD_OPEN | wxFD_FILE_MUST_EXIST);

        if(openFileDlg.ShowModal() == wxID_CANCEL)
            return;

        std::filesystem::path filePath(openFileDlg.GetPath().ToStdString());
        auto saveData = rz::LoadObjectFromJsonFile_OBJ<SaveData>(filePath, [](const std::string& error){
            wxMessageBox(error, "Load Error", wxOK | wxICON_ERROR);
        });

        if(!saveData)
            return;

        currentSavePath = filePath;
        clearWidgets();

        loadFromSave(*saveData);

    }, wxID_OPEN);

    auto saveFn = [this, saveFromWidgets](const std::filesystem::path& path){
        rz::WriteObjToJsonFile(saveFromWidgets(), *currentSavePath, 0, [](const std::string& error){
            wxMessageBox(error, "Save Error", wxOK | wxICON_ERROR);
        });
    };

    this->Bind(wxEVT_MENU, [this, saveFn](wxCommandEvent& event){
        if(currentSavePath){
            saveFn(*currentSavePath);
        } else {
            wxCommandEvent saveas(wxEVT_MENU, wxID_SAVEAS);
            this->GetEventHandler()->ProcessEvent(saveas);
        }
    }, wxID_SAVE);

    this->Bind(wxEVT_MENU, [this, saveFn](wxCommandEvent& event){
        wxFileDialog saveFileDlg(this, "Save Recipe As", "", "",
            "Recipe files (*.rcp)|*.rcp|All files (*.*)|*.*", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

        if(saveFileDlg.ShowModal() == wxID_CANCEL)
            return;

        currentSavePath = saveFileDlg.GetPath().ToStdString();
        saveFn(*currentSavePath);

    }, wxID_SAVEAS);

    this->Bind(wxEVT_MENU, [this](wxCommandEvent& event){
        this->Close(true);
    }, wxID_EXIT);

}
