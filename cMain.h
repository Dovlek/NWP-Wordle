#pragma once
#include "wx/wx.h"
#include "cKeyboard.h"
#include "cGrid.h"

class cMain : public wxFrame
{
public:
    cMain();
    ~cMain();

private:
    cGrid* cgrid;
    cKeyboardENG* ckeyboard_eng;
    int currentRow = 0;
    int currentCol = 0;
    void OnKeyboardButtonClicked(wxCommandEvent& evt);
};

