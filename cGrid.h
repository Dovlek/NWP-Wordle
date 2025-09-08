#pragma once
#include "wx/wx.h"

class cGrid
{
public:
	cGrid(int width, int height);
	~cGrid();
	wxGridSizer* CreateGrid(wxWindow* parent);

	void SetLetter(int row, int col, const wxString& letter);
	wxString GetLetter(int row, int col) const;
	bool IsValidPosition(int row, int col) const;

	int GetWidth() const { return nFieldWidth; }
	int GetHeight() const { return nFieldHeight; }

	void UpdateActiveCell(int prevRow, int prevCol, int currRow, int currCol, bool forward);
	void UpdateCellColors(int row, const std::vector<int>& states);
	void ResetGrid();

private:
	int nFieldWidth;
	int nFieldHeight;
	wxButton** gridButton;
	wxStaticText** gridText;
	wxVector<wxBitmap> bitmapsGrid;
};

