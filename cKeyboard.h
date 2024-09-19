#pragma once
#include "wx/wx.h"

class cKeyboard
{
public:
	int keyboardSize;
	wxStaticText** keyLabel;
	wxString keyboardString;
	wxButton** gridKey;
	wxBoxSizer* gridKeyRow1;
	wxBoxSizer* gridKeyRow2;
	wxBoxSizer* gridKeyRow3;
	wxVector<wxBitmap> bitmapsKeys;

	virtual wxBoxSizer* CreateKeyboard(wxWindow* parent) = 0;
};

class cKeyboardENG : public cKeyboard
{
public:
	cKeyboardENG();
	~cKeyboardENG();
	
	wxBoxSizer* CreateKeyboard(wxWindow* parent);
};
