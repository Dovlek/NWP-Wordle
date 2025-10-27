#include "cGrid.h"
#include "NinePatchScaler.h"
#include "UIScaler.h"
#include "Theme.h"

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
    gridStates.resize(nFieldHeight * nFieldWidth, 0); // Initialize all to unmarked state

    // Load and scale bitmaps based on current resolution
    UIScaler& uiScaler = UIScaler::GetInstance();
    NinePatchScaler& ninePatchScaler = NinePatchScaler::GetInstance();

    // Base size for grid cells
    wxSize baseGridSize(64, 64);

    // Calculate scaled size
    wxSize scaledGridSize = uiScaler.ScaledSize(baseGridSize.GetWidth(), baseGridSize.GetHeight());

    // Load scaled bitmaps using nine-patch scaling
    int borderSize = 4;

    // Determine suffix based on theme
    wxString suffix = (ThemeManager::Get().GetCurrentTheme() == ThemeType::LIGHT) ? wxT("_Light") : wxT("");

    bitmapsGrid.push_back(ninePatchScaler.GetScaledBitmap(wxT("IDB_UNMARKED") + suffix, baseGridSize, scaledGridSize, borderSize));
    bitmapsGrid.push_back(ninePatchScaler.GetScaledBitmap(wxT("IDB_MARKED") + suffix, baseGridSize, scaledGridSize, borderSize));
    bitmapsGrid.push_back(ninePatchScaler.GetScaledBitmap(wxT("IDB_COLD") + suffix, baseGridSize, scaledGridSize, borderSize));
    bitmapsGrid.push_back(ninePatchScaler.GetScaledBitmap(wxT("IDB_WARM") + suffix, baseGridSize, scaledGridSize, borderSize));
    bitmapsGrid.push_back(ninePatchScaler.GetScaledBitmap(wxT("IDB_HOT") + suffix, baseGridSize, scaledGridSize, borderSize));
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

            wxImage img = bitmapsGrid.at(0).ConvertToImage();
            wxBitmap cellBitmap(img);
            gridBitmap[index] = new wxStaticBitmap(parent, wxID_ANY, cellBitmap, wxDefaultPosition, wxSize(cellBitmap.GetWidth(), cellBitmap.GetHeight()));
            gridBitmap[index]->SetMinSize(wxSize(cellBitmap.GetWidth(), cellBitmap.GetHeight()));
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
        wxBitmap bitmapWithText = DrawTextOnBitmap(bitmapsGrid.at(0), letter, gridFont, ThemeManager::Get().GetTextColor());
        gridBitmap[index]->SetBitmap(bitmapWithText);
        gridBitmap[index]->Refresh();
    }
    else
    {
        wxImage img = bitmapsGrid.at(0).ConvertToImage();
        wxBitmap emptyBitmap(img);
        gridBitmap[index]->SetBitmap(emptyBitmap);
        gridBitmap[index]->Refresh();
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
            wxBitmap bitmapWithText = DrawTextOnBitmap(bitmapsGrid.at(1), label, gridFont, ThemeManager::Get().GetTextColor());
            gridBitmap[index]->SetBitmap(bitmapWithText);
            gridBitmap[index]->Refresh();
            gridStates[index] = 1; // Mark as active
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
        wxBitmap bitmapWithText = DrawTextOnBitmap(bitmapsGrid.at(1), label, gridFont, ThemeManager::Get().GetTextColor());
        gridBitmap[prevIndex]->SetBitmap(bitmapWithText);
        gridBitmap[prevIndex]->Refresh();
    }
    if (IsValidPosition(currRow, currCol) && !forward)
    {
        int currIndex = currRow * nFieldWidth + currCol;
        wxString label = gridLabels[currIndex];
        if (!label.IsEmpty())
        {
            wxBitmap bitmapWithText = DrawTextOnBitmap(bitmapsGrid.at(0), label, gridFont, ThemeManager::Get().GetTextColor());
            gridBitmap[currIndex]->SetBitmap(bitmapWithText);
            gridBitmap[currIndex]->Refresh();
        }
        else
        {
            wxImage img = bitmapsGrid.at(0).ConvertToImage();
            wxBitmap emptyBitmap(img);
            gridBitmap[currIndex]->SetBitmap(emptyBitmap);
            gridBitmap[currIndex]->Refresh();
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
        wxBitmap bitmapWithText = DrawTextOnBitmap(bitmapsGrid.at(bitmapIndex), label, gridFont, ThemeManager::Get().GetTextColor());
        gridBitmap[index]->SetBitmap(bitmapWithText);
        gridBitmap[index]->Refresh();

        // Store the state for theme refreshing
        gridStates[index] = bitmapIndex;
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
            gridStates[index] = 0; // Reset to unmarked state
            wxImage img = bitmapsGrid.at(0).ConvertToImage();
            wxBitmap emptyBitmap(img);
            gridBitmap[index]->SetBitmap(emptyBitmap);
            gridBitmap[index]->Refresh();
        }
    }
}

void cGrid::ReloadBitmaps()
{
    // Clear existing bitmaps
    bitmapsGrid.clear();

    // Reload with current theme
    UIScaler& uiScaler = UIScaler::GetInstance();
    NinePatchScaler& ninePatchScaler = NinePatchScaler::GetInstance();

    wxSize baseGridSize(64, 64);
    wxSize scaledGridSize = uiScaler.ScaledSize(baseGridSize.GetWidth(), baseGridSize.GetHeight());
    int borderSize = 4;

    // Determine suffix based on theme
    wxString suffix = (ThemeManager::Get().GetCurrentTheme() == ThemeType::LIGHT) ? wxT("_Light") : wxT("");

    bitmapsGrid.push_back(ninePatchScaler.GetScaledBitmap(wxT("IDB_UNMARKED") + suffix, baseGridSize, scaledGridSize, borderSize));
    bitmapsGrid.push_back(ninePatchScaler.GetScaledBitmap(wxT("IDB_MARKED") + suffix, baseGridSize, scaledGridSize, borderSize));
    bitmapsGrid.push_back(ninePatchScaler.GetScaledBitmap(wxT("IDB_COLD") + suffix, baseGridSize, scaledGridSize, borderSize));
    bitmapsGrid.push_back(ninePatchScaler.GetScaledBitmap(wxT("IDB_WARM") + suffix, baseGridSize, scaledGridSize, borderSize));
    bitmapsGrid.push_back(ninePatchScaler.GetScaledBitmap(wxT("IDB_HOT") + suffix, baseGridSize, scaledGridSize, borderSize));
}

void cGrid::RefreshAllCells()
{
    UIScaler& scaler = UIScaler::GetInstance();
    wxFont gridFont(scaler.ScaledFontSize(22), wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false);

    for (int i = 0; i < nFieldHeight * nFieldWidth; ++i)
    {
        wxString label = gridLabels[i];
        int state = gridStates[i];

        if (!label.IsEmpty())
        {
            wxBitmap bitmapWithText = DrawTextOnBitmap(bitmapsGrid.at(state), label, gridFont, ThemeManager::Get().GetTextColor());
            gridBitmap[i]->SetBitmap(bitmapWithText);
        }
        else
        {
            wxImage img = bitmapsGrid.at(state).ConvertToImage();
            wxBitmap emptyBitmap(img);
            gridBitmap[i]->SetBitmap(emptyBitmap);
        }
        gridBitmap[i]->Refresh();
    }
}

cGrid::~cGrid()
{
}
