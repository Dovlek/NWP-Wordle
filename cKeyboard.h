#pragma once
#include "wx/wx.h"
#include <vector>

class cKeyboard
{
protected:
	int keyboardSize;
	std::vector<wxStaticText*> keyLabel;
	wxString keyboardString;
	std::vector<wxButton*> gridKey;
	wxBoxSizer* gridKeyRow1;
	wxBoxSizer* gridKeyRow2;
	wxBoxSizer* gridKeyRow3;
	wxVector<wxBitmap> bitmapsKeys;

public:
	virtual wxBoxSizer* CreateKeyboard(wxWindow* parent) = 0;
	void OnKeyLabelClicked(wxMouseEvent& event);
	void UpdateKeyboardColors(const wxString& guess, const std::vector<int>& states);
	void ResetKeyboard();
	
	void BindKeyboardEvents(wxEvtHandler* handler, void (wxEvtHandler::*method)(wxCommandEvent&));
};

class cKeyboardENG : public cKeyboard
{
public:
	cKeyboardENG();
	~cKeyboardENG();
	
	wxBoxSizer* CreateKeyboard(wxWindow* parent) override;
};
