#pragma once
#include "wx/wx.h"

class cKeyboard
{
protected:
	int keyboardSize;
	wxStaticText** keyLabel;
	wxString keyboardString;
	wxButton** gridKey;
	wxBoxSizer* gridKeyRow1;
	wxBoxSizer* gridKeyRow2;
	wxBoxSizer* gridKeyRow3;
	wxVector<wxBitmap> bitmapsKeys;

public:
	virtual wxBoxSizer* CreateKeyboard(wxWindow* parent) = 0;
};

class cKeyboardENG : public cKeyboard
{
public:
	wxBoxSizer* CreateKeyboard(wxWindow* parent);
	cKeyboardENG();
	~cKeyboardENG();
};
