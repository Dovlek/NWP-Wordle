#include "cKeyboard.h"
#include "NinePatchScaler.h"
#include "UIScaler.h"

wxBitmap cKeyboard::DrawTextOnBitmap(const wxBitmap& baseBitmap, const wxString& text, const wxFont& font, const wxColor& textColor)
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

void cKeyboard::UpdateKeyboardColors(const wxString& guess, const std::vector<int>& states)
{
    UIScaler& scaler = UIScaler::GetInstance();
    wxFont keyFont(scaler.ScaledFontSize(18), wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false);

    for (int i = 0; i < static_cast<int>(states.size()); i++)
    {
        wxString letter = wxString(guess[i]).Upper();

        for (int keyIndex = 0; keyIndex < keyboardSize + 2; keyIndex++)
        {
            if (gridKey[keyIndex]->GetLabel() == letter)
            {
                // Track current state to avoid downgrading
                wxBitmap currentBitmap = gridKey[keyIndex]->GetBitmap();

                // Don't downgrade from correct to wrong position or wrong
                if (currentBitmap.IsSameAs(bitmapsKeys[4]))
                    continue;

                wxBitmap newBaseBitmap;
                if (states[i] == 2) // CORRECT
                {
                    newBaseBitmap = bitmapsKeys[4];
                }
                else if (states[i] == 1 && !currentBitmap.IsSameAs(bitmapsKeys[4])) // WRONG_POSITION
                {
                    newBaseBitmap = bitmapsKeys[3];
                }
                else if (states[i] == 0 && !currentBitmap.IsSameAs(bitmapsKeys[4]) && !currentBitmap.IsSameAs(bitmapsKeys[3])) // WRONG
                {
                    newBaseBitmap = bitmapsKeys[2];
                }
                else
                {
                    break; // No update needed
                }

                // Draw text on the new bitmap
                wxBitmap bitmapWithText = DrawTextOnBitmap(newBaseBitmap, letter, keyFont, wxColor(*wxWHITE));
                gridKey[keyIndex]->SetBitmap(bitmapWithText);
                gridKey[keyIndex]->Refresh();
                break;
            }
        }
    }
}

void cKeyboard::ResetKeyboard()
{
    UIScaler& scaler = UIScaler::GetInstance();
    wxFont keyFont(scaler.ScaledFontSize(18), wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false);
    wxFont enterFont = wxFont(scaler.ScaledFontSize(9), wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false);

    for (int keyIndex = 0; keyIndex < keyboardSize + 2; keyIndex++)
    {
        wxString keyLabelText = gridKey[keyIndex]->GetLabel();
        wxBitmap bitmapWithText;

        if (keyLabelText == "ENTER")
        {
            bitmapWithText = DrawTextOnBitmap(bitmapsKeys[1], keyLabelText, enterFont, wxColor(*wxWHITE));
        }
        else if (keyLabelText == "backspace")
        {
            if (hasMaterialFont)
            {
                wxFont backspaceFont = wxFont(scaler.ScaledFontSize(20), wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, wxT("Material Symbols Outlined"));
                bitmapWithText = DrawTextOnBitmap(bitmapsKeys[1], wxT("backspace"), backspaceFont, wxColor(*wxWHITE));
            }
            else
            {
                wxFont backspaceFont = wxFont(scaler.ScaledFontSize(18), wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false);
                bitmapWithText = DrawTextOnBitmap(bitmapsKeys[1], wxT("⌫"), backspaceFont, wxColor(*wxWHITE));
            }
        }
        else
        {
            bitmapWithText = DrawTextOnBitmap(bitmapsKeys[0], keyLabelText, keyFont, wxColor(*wxWHITE));
        }

        gridKey[keyIndex]->SetBitmap(bitmapWithText);
        gridKey[keyIndex]->Refresh();
    }
}

void cKeyboard::BindKeyboardEvents(wxEvtHandler* handler, void (wxEvtHandler::*method)(wxCommandEvent&))
{
    for (int i = 0; i < keyboardSize + 2; i++)
        gridKey[i]->Bind(wxEVT_BUTTON, method, handler);
}

cKeyboardENG::cKeyboardENG()
{
    keyboardString = wxT("QWERTYUIOPASDFGHJKLZXCVBNM");
    keyboardSize = keyboardString.Length();

    gridKey.resize(keyboardSize + 2);

    gridKeyRow1 = nullptr;
    gridKeyRow2 = nullptr;
    gridKeyRow3 = nullptr;
    hasMaterialFont = false;

    // Load and scale bitmaps based on current resolution
    UIScaler& uiScaler = UIScaler::GetInstance();
    NinePatchScaler& ninePatchScaler = NinePatchScaler::GetInstance();

    // Base sizes for keyboard buttons
    wxSize baseKeySize(44, 58);    // Regular key size
    wxSize baseBigKeySize(66, 58); // Enter/Backspace key size

    // Calculate scaled sizes
    wxSize scaledKeySize = uiScaler.ScaledSize(baseKeySize.GetWidth(), baseKeySize.GetHeight());
    wxSize scaledBigKeySize = uiScaler.ScaledSize(baseBigKeySize.GetWidth(), baseBigKeySize.GetHeight());

    // Load scaled bitmaps using nine-patch scaling
    int borderSize = 4;
    bitmapsKeys.push_back(ninePatchScaler.GetScaledBitmap(wxT("IDB_UNUSED"), baseKeySize, scaledKeySize, borderSize));
    bitmapsKeys.push_back(ninePatchScaler.GetScaledBitmap(wxT("IDB_BIGBUTTON"), baseBigKeySize, scaledBigKeySize, borderSize));
    bitmapsKeys.push_back(ninePatchScaler.GetScaledBitmap(wxT("IDB_USED"), baseKeySize, scaledKeySize, borderSize));
    bitmapsKeys.push_back(ninePatchScaler.GetScaledBitmap(wxT("IDB_ALMOST"), baseKeySize, scaledKeySize, borderSize));
    bitmapsKeys.push_back(ninePatchScaler.GetScaledBitmap(wxT("IDB_CORRECT"), baseKeySize, scaledKeySize, borderSize));
}

wxBoxSizer* cKeyboardENG::CreateKeyboard(wxWindow* parent)
{
    int keyPosition = 0;

    gridKeyRow1 = new wxBoxSizer(wxHORIZONTAL);
    gridKeyRow2 = new wxBoxSizer(wxHORIZONTAL);
    gridKeyRow3 = new wxBoxSizer(wxHORIZONTAL);

    wxBoxSizer* keyboard = new wxBoxSizer(wxVERTICAL);

    // Try loading Material Symbols font if it exists
    if (wxFileExists("Resources/Fonts/material-symbols-outlined.ttf"))
    {
        hasMaterialFont = wxFont::AddPrivateFont("Resources/Fonts/material-symbols-outlined.ttf");
    }
    else
    {
        hasMaterialFont = false;
    }

    UIScaler& scaler = UIScaler::GetInstance();
    wxFont keyFont(scaler.ScaledFontSize(18), wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false);
    int keyGap = scaler.ScaledValue(2);

    for (int x = 0; x < 10; x++)
    {
        wxString label = keyboardString.at(keyPosition);
        wxBitmap bitmapWithText = DrawTextOnBitmap(bitmapsKeys.at(0), label, keyFont, wxColor(*wxWHITE));

        gridKey[keyPosition] = new wxBitmapButton(parent, 500 + keyPosition, bitmapWithText, wxDefaultPosition, wxSize(bitmapsKeys[0].GetWidth(), bitmapsKeys[0].GetHeight()), wxBORDER_NONE);
        gridKey[keyPosition]->SetLabel(label);
        gridKey[keyPosition]->SetMinSize(wxSize(bitmapsKeys[0].GetWidth(), bitmapsKeys[0].GetHeight()));
        gridKey[keyPosition]->SetCursor(wxCursor(wxCURSOR_HAND));

        gridKeyRow1->Add(gridKey[keyPosition], wxSizerFlags().Border(wxALL, keyGap));
        keyPosition++;
    }

    for (int x = 0; x < 9; x++)
    {
        wxString label = keyboardString.at(keyPosition);
        wxBitmap bitmapWithText = DrawTextOnBitmap(bitmapsKeys.at(0), label, keyFont, wxColor(*wxWHITE));

        gridKey[keyPosition] = new wxBitmapButton(parent, 500 + keyPosition, bitmapWithText, wxDefaultPosition, wxSize(bitmapsKeys[0].GetWidth(), bitmapsKeys[0].GetHeight()), wxBORDER_NONE);
        gridKey[keyPosition]->SetLabel(label);
        gridKey[keyPosition]->SetMinSize(wxSize(bitmapsKeys[0].GetWidth(), bitmapsKeys[0].GetHeight()));
        gridKey[keyPosition]->SetCursor(wxCursor(wxCURSOR_HAND));

        gridKeyRow2->Add(gridKey[keyPosition], wxSizerFlags().Border(wxALL, keyGap));
        keyPosition++;
    }

    wxFont enterFont = wxFont(scaler.ScaledFontSize(9), wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false);
    wxBitmap enterBitmap = DrawTextOnBitmap(bitmapsKeys.at(1), wxT("ENTER"), enterFont, wxColor(*wxWHITE));

    gridKey[keyPosition] = new wxBitmapButton(parent, 500 + keyPosition, enterBitmap, wxDefaultPosition, wxSize(bitmapsKeys[1].GetWidth(), bitmapsKeys[1].GetHeight()), wxBORDER_NONE);
    gridKey[keyPosition]->SetLabel(wxT("ENTER"));
    gridKey[keyPosition]->SetMinSize(wxSize(bitmapsKeys[1].GetWidth(), bitmapsKeys[1].GetHeight()));
    gridKey[keyPosition]->SetCursor(wxCursor(wxCURSOR_HAND));

    gridKeyRow3->Add(gridKey[keyPosition], wxSizerFlags().Border(wxALL, keyGap));
    keyPosition++;

    for (int x = 0; x < 7; x++)
    {
        wxString label = keyboardString.at(keyPosition - 1);
        wxBitmap bitmapWithText = DrawTextOnBitmap(bitmapsKeys.at(0), label, keyFont, wxColor(*wxWHITE));

        gridKey[keyPosition] = new wxBitmapButton(parent, 500 + keyPosition, bitmapWithText, wxDefaultPosition, wxSize(bitmapsKeys[0].GetWidth(), bitmapsKeys[0].GetHeight()), wxBORDER_NONE);
        gridKey[keyPosition]->SetLabel(label);
        gridKey[keyPosition]->SetMinSize(wxSize(bitmapsKeys[0].GetWidth(), bitmapsKeys[0].GetHeight()));
        gridKey[keyPosition]->SetCursor(wxCursor(wxCURSOR_HAND));

        gridKeyRow3->Add(gridKey[keyPosition], wxSizerFlags().Border(wxALL, keyGap));
        keyPosition++;
    }

    wxFont backspaceFont;
    wxString backspaceText;
    if (hasMaterialFont)
    {
        backspaceFont = wxFont(scaler.ScaledFontSize(20), wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, wxT("Material Symbols Outlined"));
        backspaceText = wxT("backspace");
    }
    else
    {
        backspaceFont = wxFont(scaler.ScaledFontSize(18), wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false);
        backspaceText = wxT("⌫");
    }
    wxBitmap backspaceBitmap = DrawTextOnBitmap(bitmapsKeys.at(1), backspaceText, backspaceFont, wxColor(*wxWHITE));

    gridKey[keyPosition] = new wxBitmapButton(parent, 500 + keyPosition, backspaceBitmap, wxDefaultPosition, wxSize(bitmapsKeys[1].GetWidth(), bitmapsKeys[1].GetHeight()), wxBORDER_NONE);
    gridKey[keyPosition]->SetLabel(wxT("backspace"));
    gridKey[keyPosition]->SetMinSize(wxSize(bitmapsKeys[1].GetWidth(), bitmapsKeys[1].GetHeight()));
    gridKey[keyPosition]->SetCursor(wxCursor(wxCURSOR_HAND));

    gridKeyRow3->Add(gridKey[keyPosition], wxSizerFlags().Border(wxALL, keyGap));

    int rowBorder = scaler.ScaledValue(2);
    keyboard->Add(gridKeyRow1, wxSizerFlags().CenterHorizontal().Border(wxALL, rowBorder));
    keyboard->Add(gridKeyRow2, wxSizerFlags().CenterHorizontal().Border(wxALL, rowBorder));
    keyboard->Add(gridKeyRow3, wxSizerFlags().CenterHorizontal().Border(wxALL, rowBorder));

    return keyboard;
}

cKeyboardENG::~cKeyboardENG()
{
    gridKeyRow1 = nullptr;
    gridKeyRow2 = nullptr;
    gridKeyRow3 = nullptr;
}
