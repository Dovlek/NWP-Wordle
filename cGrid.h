#pragma once
#include "wx/wx.h"
#include <vector>

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

    void UpdateActiveRowCells(int row);
    void UpdateActiveCell(int prevRow, int prevCol, int currRow, int currCol, bool forward);
    void UpdateCellColors(int row, const std::vector<int>& states);
    void ResetGrid();
    void ReloadBitmaps();
    void RefreshAllCells();

private:
    wxBitmap DrawTextOnBitmap(const wxBitmap& baseBitmap, const wxString& text, const wxFont& font, const wxColor& textColor);
    int nFieldWidth;
    int nFieldHeight;
    std::vector<wxStaticBitmap*> gridBitmap;
    std::vector<wxString> gridLabels; // Store letter labels for game logic
    std::vector<int> gridStates; // Store cell states (0=unmarked, 1=marked, 2=cold, 3=warm, 4=hot)
    wxVector<wxBitmap> bitmapsGrid;
};
