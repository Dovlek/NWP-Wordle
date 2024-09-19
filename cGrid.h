#pragma once
#include "wx/wx.h"

class cGrid
{
public:
	int nFieldWidth;
	int nFieldHeight;
	wxButton** gridButton;
	wxStaticText** gridText;
	wxVector<wxBitmap> bitmapsGrid;

	cGrid(int width, int height);
	~cGrid();

	wxGridSizer* CreateGrid(wxWindow* parent);
};

