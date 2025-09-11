#include "cKeyboard.h"
#include "NinePatchScaler.h"
#include "UIScaler.h"

void cKeyboard::OnKeyLabelClicked(wxMouseEvent& event)
{
    wxStaticText* label = dynamic_cast<wxStaticText*>(event.GetEventObject());
    if (!label)
        return;

    wxWindow* parentBtn = label->GetParent();
    if (parentBtn && parentBtn->IsKindOf(CLASSINFO(wxButton)))
    {
        wxCommandEvent btnEvent(wxEVT_BUTTON, parentBtn->GetId());
        btnEvent.SetEventObject(parentBtn);
        wxPostEvent(parentBtn, btnEvent);
    }
}

void cKeyboard::UpdateKeyboardColors(const wxString& guess, const std::vector<int>& states)
{
    for (int i = 0; i < states.size(); i++)
    {
        wxString letter = wxString(guess[i]).Upper();
        
        for (int keyIndex = 0; keyIndex < keyboardSize + 2; keyIndex++)
        {
            if (keyLabel[keyIndex]->GetLabel() == letter)
            {
                // Don't downgrade colors
                wxColor currentBg = keyLabel[keyIndex]->GetBackgroundColour();
                wxColor correctColor(83, 141, 78);
                wxColor wrongPosColor(181, 159, 59);
                wxColor wrongColor(58, 58, 60);
                
                if (currentBg == correctColor) 
                    continue;
                
                if (states[i] == 2) // CORRECT
                {
                    gridKey[keyIndex]->SetBitmap(bitmapsKeys[4]);
                    keyLabel[keyIndex]->SetBackgroundColour(correctColor);
                }
                else if (states[i] == 1 && currentBg != correctColor) // WRONG_POSITION
                {
                    gridKey[keyIndex]->SetBitmap(bitmapsKeys[3]);
                    keyLabel[keyIndex]->SetBackgroundColour(wrongPosColor);
                }
                else if (states[i] == 0 && currentBg != correctColor && currentBg != wrongPosColor) // WRONG
                {
                    gridKey[keyIndex]->SetBitmap(bitmapsKeys[2]);
                    keyLabel[keyIndex]->SetBackgroundColour(wrongColor);
                }
                
                keyLabel[keyIndex]->Refresh();
                break;
            }
        }
    }
}

void cKeyboard::ResetKeyboard()
{
    for (int keyIndex = 0; keyIndex < keyboardSize + 2; keyIndex++)
    {
        wxString keyLabelText = keyLabel[keyIndex]->GetLabel();
        
        if (keyLabelText == "ENTER" || keyLabelText == "backspace")
            gridKey[keyIndex]->SetBitmap(bitmapsKeys[1]); // IDB_BIGBUTTON
        else
            gridKey[keyIndex]->SetBitmap(bitmapsKeys[0]); // IDB_UNUSED
        
        keyLabel[keyIndex]->SetBackgroundColour(wxColor(129, 131, 132));
        keyLabel[keyIndex]->Refresh();
        gridKey[keyIndex]->Refresh();
    }
}

cKeyboardENG::cKeyboardENG()
{
    keyboardString = wxT("QWERTYUIOPASDFGHJKLZXCVBNM");
    keyboardSize = keyboardString.Length();

    gridKey = new wxButton*[keyboardSize + 2];
    keyLabel = new wxStaticText*[keyboardSize + 2];

    gridKeyRow1 = nullptr;
    gridKeyRow2 = nullptr;
    gridKeyRow3 = nullptr;

    // Load and scale bitmaps based on current resolution
    UIScaler& uiScaler = UIScaler::GetInstance();
    NinePatchScaler& ninePatchScaler = NinePatchScaler::GetInstance();
    
    // Base sizes for keyboard buttons
    wxSize baseKeySize(44, 58);      // Regular key size
    wxSize baseBigKeySize(66, 58);   // Enter/Backspace key size
    
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
    wxFont::AddPrivateFont("Resources/Fonts/material-symbols-outlined.ttf");
    
    UIScaler& scaler = UIScaler::GetInstance();
    wxFont keyFont(scaler.ScaledFontSize(18), wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false);
    int keyGap = scaler.ScaledValue(2);

    for (int x = 0; x < 10; x++)
    {
        gridKey[keyPosition] = new wxButton(parent, 500 + keyPosition, wxEmptyString, wxDefaultPosition, wxSize(bitmapsKeys[0].GetWidth(), bitmapsKeys[0].GetHeight()), wxBORDER_NONE);
        gridKey[keyPosition]->SetBitmap(bitmapsKeys.at(0));
        gridKey[keyPosition]->SetCursor(wxCursor(wxCURSOR_HAND));

        keyLabel[keyPosition] = new wxStaticText(gridKey[keyPosition], wxID_ANY, keyboardString.at(keyPosition), wxDefaultPosition, wxDefaultSize);
        keyLabel[keyPosition]->SetBackgroundColour(wxColor(129, 131, 132));
        keyLabel[keyPosition]->SetForegroundColour(wxColor(*wxWHITE));
        keyLabel[keyPosition]->SetFont(keyFont);
        keyLabel[keyPosition]->CenterOnParent();
        keyLabel[keyPosition]->Bind(wxEVT_LEFT_DOWN, &cKeyboard::OnKeyLabelClicked, this);
        keyLabel[keyPosition]->SetCursor(wxCursor(wxCURSOR_HAND));

        gridKeyRow1->Add(gridKey[keyPosition], wxSizerFlags().Border(wxALL, keyGap));
        keyPosition++;
    }

    for (int x = 0; x < 9; x++)
    {
        gridKey[keyPosition] = new wxButton(parent, 500 + keyPosition, wxEmptyString, wxDefaultPosition, wxSize(bitmapsKeys[0].GetWidth(), bitmapsKeys[0].GetHeight()), wxBORDER_NONE);
        gridKey[keyPosition]->SetBitmap(bitmapsKeys.at(0));
        gridKey[keyPosition]->SetCursor(wxCursor(wxCURSOR_HAND));

        keyLabel[keyPosition] = new wxStaticText(gridKey[keyPosition], wxID_ANY, keyboardString.at(keyPosition), wxDefaultPosition, wxDefaultSize);
        keyLabel[keyPosition]->SetBackgroundColour(wxColor(129, 131, 132));
        keyLabel[keyPosition]->SetForegroundColour(wxColor(*wxWHITE));
        keyLabel[keyPosition]->SetFont(keyFont);
        keyLabel[keyPosition]->CenterOnParent();
        keyLabel[keyPosition]->Bind(wxEVT_LEFT_DOWN, &cKeyboard::OnKeyLabelClicked, this);
        keyLabel[keyPosition]->SetCursor(wxCursor(wxCURSOR_HAND));

        gridKeyRow2->Add(gridKey[keyPosition], wxSizerFlags().Border(wxALL, keyGap));
        keyPosition++;
    }

    gridKey[keyPosition] = new wxButton(parent, 500 + keyPosition, wxEmptyString, wxDefaultPosition, wxSize(bitmapsKeys[1].GetWidth(), bitmapsKeys[1].GetHeight()), wxBORDER_NONE);
    gridKey[keyPosition]->SetBitmap(bitmapsKeys.at(1));
    gridKey[keyPosition]->SetCursor(wxCursor(wxCURSOR_HAND));

    keyLabel[keyPosition] = new wxStaticText(gridKey[keyPosition], wxID_ANY, wxT("ENTER"), wxDefaultPosition, wxDefaultSize);
    keyLabel[keyPosition]->SetBackgroundColour(wxColor(129, 131, 132));
    keyLabel[keyPosition]->SetForegroundColour(wxColor(*wxWHITE));
    keyLabel[keyPosition]->SetFont(wxFont(scaler.ScaledFontSize(9), wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false));
    keyLabel[keyPosition]->CenterOnParent();
    keyLabel[keyPosition]->Bind(wxEVT_LEFT_DOWN, &cKeyboard::OnKeyLabelClicked, this);
    keyLabel[keyPosition]->SetCursor(wxCursor(wxCURSOR_HAND));

    gridKeyRow3->Add(gridKey[keyPosition], wxSizerFlags().Border(wxALL, keyGap));
    keyPosition++;

    for (int x = 0; x < 7; x++)
    {
        gridKey[keyPosition] = new wxButton(parent, 500 + keyPosition, wxEmptyString, wxDefaultPosition, wxSize(bitmapsKeys[0].GetWidth(), bitmapsKeys[0].GetHeight()), wxBORDER_NONE);
        gridKey[keyPosition]->SetBitmap(bitmapsKeys.at(0));
        gridKey[keyPosition]->SetCursor(wxCursor(wxCURSOR_HAND));

        keyLabel[keyPosition] = new wxStaticText(gridKey[keyPosition], wxID_ANY, keyboardString.at(keyPosition-1), wxDefaultPosition, wxDefaultSize);
        keyLabel[keyPosition]->SetBackgroundColour(wxColor(129, 131, 132));
        keyLabel[keyPosition]->SetForegroundColour(wxColor(*wxWHITE));
        keyLabel[keyPosition]->SetFont(keyFont);
        keyLabel[keyPosition]->CenterOnParent();
        keyLabel[keyPosition]->Bind(wxEVT_LEFT_DOWN, &cKeyboard::OnKeyLabelClicked, this);
        keyLabel[keyPosition]->SetCursor(wxCursor(wxCURSOR_HAND));

        gridKeyRow3->Add(gridKey[keyPosition], wxSizerFlags().Border(wxALL, keyGap));
        keyPosition++;
    }

    gridKey[keyPosition] = new wxButton(parent, 500 + keyPosition, wxEmptyString, wxDefaultPosition, wxSize(bitmapsKeys[1].GetWidth(), bitmapsKeys[1].GetHeight()), wxBORDER_NONE);
    gridKey[keyPosition]->SetBitmap(bitmapsKeys.at(1));
    gridKey[keyPosition]->SetCursor(wxCursor(wxCURSOR_HAND));

    keyLabel[keyPosition] = new wxStaticText(gridKey[keyPosition], wxID_ANY, wxT("backspace"), wxDefaultPosition, wxDefaultSize);
    keyLabel[keyPosition]->SetBackgroundColour(wxColor(129, 131, 132));
    keyLabel[keyPosition]->SetForegroundColour(wxColor(*wxWHITE));
    keyLabel[keyPosition]->SetFont(wxFont(scaler.ScaledFontSize(20), wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, wxT("Material Symbols Outlined")));
    keyLabel[keyPosition]->CenterOnParent();
    keyLabel[keyPosition]->Bind(wxEVT_LEFT_DOWN, &cKeyboard::OnKeyLabelClicked, this);
    keyLabel[keyPosition]->SetCursor(wxCursor(wxCURSOR_HAND));

    gridKeyRow3->Add(gridKey[keyPosition], wxSizerFlags().Border(wxALL, keyGap));

    int rowBorder = scaler.ScaledValue(2);
    keyboard->Add(gridKeyRow1, wxSizerFlags().CenterHorizontal().Border(wxALL, rowBorder));
    keyboard->Add(gridKeyRow2, wxSizerFlags().CenterHorizontal().Border(wxALL, rowBorder));
    keyboard->Add(gridKeyRow3, wxSizerFlags().CenterHorizontal().Border(wxALL, rowBorder));

    return keyboard;
}

cKeyboardENG::~cKeyboardENG()
{
    delete[] gridKey;
    delete[] keyLabel;
    gridKey = nullptr;
    keyLabel = nullptr;
    gridKeyRow1 = nullptr;
    gridKeyRow2 = nullptr;
    gridKeyRow3 = nullptr;
}
