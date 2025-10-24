#include "cGrid.h"
#include "NinePatchScaler.h"
#include "UIScaler.h"

wxBitmap cGrid::DrawTextOnBitmap(const wxBitmap& baseBitmap, const wxString& text, const wxFont& font, const wxColor& textColor)
{
    wxBitmap bitmap = baseBitmap.GetSubBitmap(wxRect(0, 0, baseBitmap.GetWidth(), baseBitmap.GetHeight()));
    wxMemoryDC dc(bitmap);

    dc.SetFont(font);
    dc.SetTextForeground(textColor);

    wxSize textSize = dc.GetTextExtent(text);
    int x = (bitmap.GetWidth() - textSize.GetWidth()) / 2;
    int y = (bitmap.GetHeight() - textSize.GetHeight()) / 2;

    dc.DrawText(text, x, y);
    dc.SelectObject(wxNullBitmap);

    return bitmap;
}

cGrid::cGrid(int width, int height)
{
    nFieldWidth = width;
    nFieldHeight = height;

    gridBitmap.resize(nFieldHeight * nFieldWidth);
    gridLabels.resize(nFieldHeight * nFieldWidth);

    // Load and scale bitmaps based on current resolution
    UIScaler& uiScaler = UIScaler::GetInstance();
    NinePatchScaler& ninePatchScaler = NinePatchScaler::GetInstance();

    // Base size for grid cells
    wxSize baseGridSize(64, 64);

    // Calculate scaled size
    wxSize scaledGridSize = uiScaler.ScaledSize(baseGridSize.GetWidth(), baseGridSize.GetHeight());

    // Load scaled bitmaps using nine-patch scaling
    int borderSize = 4;
    bitmapsGrid.push_back(ninePatchScaler.GetScaledBitmap(wxT("IDB_UNMARKED"), baseGridSize, scaledGridSize, borderSize));
    bitmapsGrid.push_back(ninePatchScaler.GetScaledBitmap(wxT("IDB_MARKED"), baseGridSize, scaledGridSize, borderSize));
    bitmapsGrid.push_back(ninePatchScaler.GetScaledBitmap(wxT("IDB_COLD"), baseGridSize, scaledGridSize, borderSize));
    bitmapsGrid.push_back(ninePatchScaler.GetScaledBitmap(wxT("IDB_WARM"), baseGridSize, scaledGridSize, borderSize));
    bitmapsGrid.push_back(ninePatchScaler.GetScaledBitmap(wxT("IDB_HOT"), baseGridSize, scaledGridSize, borderSize));
}

wxGridSizer* cGrid::CreateGrid(wxWindow* parent)
{
    wxGridSizer* grid = new wxGridSizer(nFieldHeight, nFieldWidth, wxSize(0, 0));

    UIScaler& scaler = UIScaler::GetInstance();
    wxFont gridFont(scaler.ScaledFontSize(22), wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false);
    int gridGap = scaler.ScaledValue(2);

    for (int x = 0; x < nFieldHeight; x++)
    {
        for (int y = 0; y < nFieldWidth; y++)
        {
            int index = x * nFieldWidth + y;

            // Create letter box
            gridBitmap[index] = new wxStaticBitmap(parent, wxID_ANY, bitmapsGrid.at(0), wxDefaultPosition, wxSize(bitmapsGrid.at(0).GetWidth(), bitmapsGrid.at(0).GetHeight()));
            gridBitmap[index]->SetMinSize(wxSize(bitmapsGrid.at(0).GetWidth(), bitmapsGrid.at(0).GetHeight()));
            gridLabels[index] = wxEmptyString;

            // Add Box to GridSizer
            grid->Add(gridBitmap[index], 1, wxALL, gridGap);
        }
    }
    return grid;
}

void cGrid::SetLetter(int row, int col, const wxString& letter)
{
    if (!IsValidPosition(row, col))
        return;

    int index = row * nFieldWidth + col;
    gridLabels[index] = letter;

    // Draw text on the base bitmap
    if (!letter.IsEmpty())
    {
        UIScaler& scaler = UIScaler::GetInstance();
        wxFont gridFont(scaler.ScaledFontSize(22), wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false);

        // Use unmarked bitmap as base for typing
        wxBitmap bitmapWithText = DrawTextOnBitmap(bitmapsGrid.at(0), letter, gridFont, wxColor(*wxWHITE));
        gridBitmap[index]->SetBitmap(bitmapWithText);
    }
    else
    {
        // If letter is empty, reset to unmarked bitmap (clear the cell)
        gridBitmap[index]->SetBitmap(bitmapsGrid.at(0)); // IDB_UNMARKED
    }
}

wxString cGrid::GetLetter(int row, int col) const
{
    if (!IsValidPosition(row, col))
        return wxEmptyString;

    int index = row * nFieldWidth + col;

    return gridLabels[index];
}

bool cGrid::IsValidPosition(int row, int col) const
{
    return row >= 0 && row < nFieldHeight && col >= 0 && col < nFieldWidth;
}

void cGrid::UpdateActiveRowCells(int row)
{
    if (row < 0 || row >= nFieldHeight)
        return;

    UIScaler& scaler = UIScaler::GetInstance();
    wxFont gridFont(scaler.ScaledFontSize(22), wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false);

    for (int col = 0; col < nFieldWidth; ++col)
    {
        int index = row * nFieldWidth + col;
        wxString label = gridLabels[index];
        if (!label.IsEmpty())
        {
            wxBitmap bitmapWithText = DrawTextOnBitmap(bitmapsGrid.at(1), label, gridFont, wxColor(*wxWHITE));
            gridBitmap[index]->SetBitmap(bitmapWithText);
        }
    }
}

void cGrid::UpdateActiveCell(int prevRow, int prevCol, int currRow, int currCol, bool forward)
{
    UIScaler& scaler = UIScaler::GetInstance();
    wxFont gridFont(scaler.ScaledFontSize(22), wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false);

    if (IsValidPosition(prevRow, prevCol) && forward)
    {
        int prevIndex = prevRow * nFieldWidth + prevCol;
        wxString label = gridLabels[prevIndex];
        wxBitmap bitmapWithText = DrawTextOnBitmap(bitmapsGrid.at(1), label, gridFont, wxColor(*wxWHITE));
        gridBitmap[prevIndex]->SetBitmap(bitmapWithText);
    }
    if (IsValidPosition(currRow, currCol) && !forward)
    {
        int currIndex = currRow * nFieldWidth + currCol;
        wxString label = gridLabels[currIndex];
        if (!label.IsEmpty())
        {
            wxBitmap bitmapWithText = DrawTextOnBitmap(bitmapsGrid.at(0), label, gridFont, wxColor(*wxWHITE));
            gridBitmap[currIndex]->SetBitmap(bitmapWithText);
        }
        else
        {
            gridBitmap[currIndex]->SetBitmap(bitmapsGrid.at(0));
        }
    }
}

void cGrid::UpdateCellColors(int row, const std::vector<int>& states)
{
    UIScaler& scaler = UIScaler::GetInstance();
    wxFont gridFont(scaler.ScaledFontSize(22), wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false);

    for (int col = 0; col < nFieldWidth; ++col)
    {
        int index = row * nFieldWidth + col;
        int bitmapIndex = 0;

        switch (states[col])
        {
        case 0:              // WRONG
            bitmapIndex = 2; // IDB_COLD
            break;
        case 1:              // WRONG_POSITION
            bitmapIndex = 3; // IDB_WARM
            break;
        case 2:              // CORRECT
            bitmapIndex = 4; // IDB_HOT
            break;
        default:
            break;
        }

        wxString label = gridLabels[index];
        wxBitmap bitmapWithText = DrawTextOnBitmap(bitmapsGrid.at(bitmapIndex), label, gridFont, wxColor(*wxWHITE));
        gridBitmap[index]->SetBitmap(bitmapWithText);
        gridBitmap[index]->Refresh();
    }
}

void cGrid::ResetGrid()
{
    for (int x = 0; x < nFieldHeight; x++)
    {
        for (int y = 0; y < nFieldWidth; y++)
        {
            int index = x * nFieldWidth + y;

            gridLabels[index] = wxEmptyString;
            gridBitmap[index]->SetBitmap(bitmapsGrid.at(0)); // IDB_UNMARKED
            gridBitmap[index]->Refresh();
        }
    }
}

cGrid::~cGrid()
{
}
