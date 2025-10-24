#pragma once
#include "wx/wx.h"
#include <vector>

class cKeyboard
{
protected:
    int keyboardSize;
    wxString keyboardString;
    std::vector<wxBitmapButton*> gridKey;
    wxBoxSizer* gridKeyRow1;
    wxBoxSizer* gridKeyRow2;
    wxBoxSizer* gridKeyRow3;
    wxVector<wxBitmap> bitmapsKeys;
    bool hasMaterialFont;

public:
    virtual ~cKeyboard() = default;

    virtual wxBoxSizer* CreateKeyboard(wxWindow* parent) = 0;
    void UpdateKeyboardColors(const wxString& guess, const std::vector<int>& states);
    void ResetKeyboard();

    void BindKeyboardEvents(wxEvtHandler* handler, void (wxEvtHandler::*method)(wxCommandEvent&));

protected:
    wxBitmap DrawTextOnBitmap(const wxBitmap& baseBitmap, const wxString& text, const wxFont& font, const wxColor& textColor);
};

class cKeyboardENG : public cKeyboard
{
public:
    cKeyboardENG();
    ~cKeyboardENG();

    wxBoxSizer* CreateKeyboard(wxWindow* parent) override;
};
