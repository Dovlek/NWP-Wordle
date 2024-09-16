#pragma once
#include "wx/wx.h"

class cMain : public wxFrame
{
public:
	cMain();
	~cMain();

	int nFieldWidth = 5;
	int nFieldHeight = 6;
	int keyboardRows = 3;
	int keyboardColumns = 10;

	wxButton** gridButton;
	wxStaticText** gridText;
};

