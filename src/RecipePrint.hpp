#pragma once

#include <wx/print.h>
#include <wx/printdlg.h>
#include <wx/string.h>

class RecipePrintout : public wxPrintout
{
public:
    RecipePrintout(const wxString& _title, const wxString& _textToPrint)
        : wxPrintout(_title), text(_textToPrint)
    {}

    bool OnPrintPage(int pageNum) override
    {
        auto* dc = GetDC();
        if(!dc)
            return false;
        auto font = wxFontInfo(36).Family(wxFONTFAMILY_TELETYPE);
        dc->SetFont(font);
        dc->DrawText(text, 100, 100);
        return true;
    }

    bool HasPage(int pageNum) override
    {
        return pageNum == 1;
    }

    void GetPageInfo(int* minPage, int* maxPage, int* pageSelFrom, int* pageSelTo) override
    {
        *pageSelTo = *pageSelFrom = *maxPage = *minPage = 1;
    }

private:
    wxString text;
};
