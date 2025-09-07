#pragma once
#include "wx/wx.h"
#include <wx/simplebook.h>
#include "cWordle.h"
#include "cMenu.h"

class cMain : public wxFrame
{
public:
    cMain();
    ~cMain();

private:
    wxSimplebook* cSimplebook;
    cWordle* wordlePanel;
    cMenu* menuPanel;
};

