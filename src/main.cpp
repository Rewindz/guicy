#include <wx/app.h>
#include <wx/wx.h>

#include "GuicyFrame.hpp"

class GuicyApp : public wxApp
{
public:
private:

    virtual bool OnInit()
    {
        auto* frame = new GuicyFrame("Guicy");
        frame->Show();
        return true;
    }
};

wxIMPLEMENT_APP(GuicyApp);
