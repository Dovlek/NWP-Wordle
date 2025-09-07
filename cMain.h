#pragma once
#include "wx/wx.h"
#include <wx/simplebook.h>
#include "cWordle.h"

class cMain : public wxFrame
{
public:
    cMain();
    ~cMain();

private:
    wxSimplebook* cSimplebook;
    cWordle* wordlePanel;
};

