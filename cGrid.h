#pragma once
#include "wx/wx.h"

class cGrid
{
public:
	cGrid(int width, int height);
	~cGrid();
	wxGridSizer* CreateGrid(wxWindow* parent);

	void SetLetter(int row, int col, const wxString& letter);
	bool IsValidPosition(int row, int col) const;

	int GetWidth() const { return nFieldWidth; }
	int GetHeight() const { return nFieldHeight; }

private:
	int nFieldWidth;
	int nFieldHeight;
	wxButton** gridButton;
	wxStaticText** gridText;
	wxVector<wxBitmap> bitmapsGrid;
};

