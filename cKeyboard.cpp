#include "cKeyboard.h"

cKeyboardENG::cKeyboardENG()
{
	keyboardString = wxT("QWERTYUIOPASDFGHJKLZXCVBNM");
	keyboardSize = keyboardString.Length();
	gridKey = new wxButton*[keyboardSize + 2];
	gridKeyRow1 = new wxBoxSizer(wxHORIZONTAL);
	gridKeyRow2 = new wxBoxSizer(wxHORIZONTAL);
	gridKeyRow3 = new wxBoxSizer(wxHORIZONTAL);
	keyLabel = new wxStaticText * [keyboardSize + 2];
	bitmapsKeys.push_back(wxBitmap("Resources/Keys/Unused.bmp", wxBITMAP_TYPE_BMP));
	bitmapsKeys.push_back(wxBitmap("Resources/Keys/BigButton.bmp", wxBITMAP_TYPE_BMP));
}

wxBoxSizer* cKeyboardENG::CreateKeyboard(wxWindow* parent)
{
	int keyPosition = 0;
	wxBoxSizer* keyboard = new wxBoxSizer(wxVERTICAL);
	wxFont::AddPrivateFont("Resources/Fonts/material-symbols-outlined.ttf");
	wxFont keyFont(18, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false);

	for (int x = 0; x < 10; x++)
	{
		gridKey[keyPosition] = new wxButton(parent, 500 + keyPosition, wxEmptyString, wxDefaultPosition, wxSize(bitmapsKeys[0].GetWidth(), bitmapsKeys[0].GetHeight()), wxBORDER_NONE);
		gridKey[keyPosition]->SetBitmap(bitmapsKeys.at(0));

		keyLabel[keyPosition] = new wxStaticText(gridKey[keyPosition], wxID_ANY, keyboardString.at(keyPosition), wxDefaultPosition, wxDefaultSize);
		keyLabel[keyPosition]->SetBackgroundColour(wxColor(129, 131, 132));
		keyLabel[keyPosition]->SetForegroundColour(wxColor(*wxWHITE));
		keyLabel[keyPosition]->SetFont(keyFont);
		keyLabel[keyPosition]->CenterOnParent();

		gridKeyRow1->Add(gridKey[keyPosition], wxSizerFlags().Border(wxALL, 2));
		keyPosition++;
	}

	for (int x = 0; x < 9; x++)
	{
		gridKey[keyPosition] = new wxButton(parent, 500 + keyPosition, wxEmptyString, wxDefaultPosition, wxSize(bitmapsKeys[0].GetWidth(), bitmapsKeys[0].GetHeight()), wxBORDER_NONE);
		gridKey[keyPosition]->SetBitmap(bitmapsKeys.at(0));

		keyLabel[keyPosition] = new wxStaticText(gridKey[keyPosition], wxID_ANY, keyboardString.at(keyPosition), wxDefaultPosition, wxDefaultSize);
		keyLabel[keyPosition]->SetBackgroundColour(wxColor(129, 131, 132));
		keyLabel[keyPosition]->SetForegroundColour(wxColor(*wxWHITE));
		keyLabel[keyPosition]->SetFont(keyFont);
		keyLabel[keyPosition]->CenterOnParent();

		gridKeyRow2->Add(gridKey[keyPosition], wxSizerFlags().Border(wxALL, 2));
		keyPosition++;
	}

	gridKey[keyPosition] = new wxButton(parent, 500 + keyPosition, wxEmptyString, wxDefaultPosition, wxSize(bitmapsKeys[1].GetWidth(), bitmapsKeys[1].GetHeight()), wxBORDER_NONE);
	gridKey[keyPosition]->SetBitmap(bitmapsKeys.at(1));

	keyLabel[keyPosition] = new wxStaticText(gridKey[keyPosition], wxID_ANY, wxT("ENTER"), wxDefaultPosition, wxDefaultSize);
	keyLabel[keyPosition]->SetBackgroundColour(wxColor(129, 131, 132));
	keyLabel[keyPosition]->SetForegroundColour(wxColor(*wxWHITE));
	keyLabel[keyPosition]->SetFont(wxFont(9, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false));
	keyLabel[keyPosition]->CenterOnParent();

	gridKeyRow3->Add(gridKey[keyPosition], wxSizerFlags().Border(wxALL, 2));
	keyPosition++;

	for (int x = 0; x < 7; x++)
	{
		gridKey[keyPosition] = new wxButton(parent, 500 + keyPosition, wxEmptyString, wxDefaultPosition, wxSize(bitmapsKeys[0].GetWidth(), bitmapsKeys[0].GetHeight()), wxBORDER_NONE);
		gridKey[keyPosition]->SetBitmap(bitmapsKeys.at(0));

		keyLabel[keyPosition] = new wxStaticText(gridKey[keyPosition], wxID_ANY, keyboardString.at(keyPosition-1), wxDefaultPosition, wxDefaultSize);
		keyLabel[keyPosition]->SetBackgroundColour(wxColor(129, 131, 132));
		keyLabel[keyPosition]->SetForegroundColour(wxColor(*wxWHITE));
		keyLabel[keyPosition]->SetFont(keyFont);
		keyLabel[keyPosition]->CenterOnParent();

		gridKeyRow3->Add(gridKey[keyPosition], wxSizerFlags().Border(wxALL, 2));
		keyPosition++;
	}

	gridKey[keyPosition] = new wxButton(parent, 500 + keyPosition, wxEmptyString, wxDefaultPosition, wxSize(bitmapsKeys[1].GetWidth(), bitmapsKeys[1].GetHeight()), wxBORDER_NONE);
	gridKey[keyPosition]->SetBitmap(bitmapsKeys.at(1));

	keyLabel[keyPosition] = new wxStaticText(gridKey[keyPosition], wxID_ANY, wxT("backspace"), wxDefaultPosition, wxDefaultSize);
	keyLabel[keyPosition]->SetBackgroundColour(wxColor(129, 131, 132));
	keyLabel[keyPosition]->SetForegroundColour(wxColor(*wxWHITE));
	keyLabel[keyPosition]->SetFont(wxFont(20, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, wxT("Material Symbols Outlined")));
	keyLabel[keyPosition]->CenterOnParent();

	gridKeyRow3->Add(gridKey[keyPosition], wxSizerFlags().Border(wxALL, 2));

	keyboard->Add(gridKeyRow1, wxSizerFlags().CenterHorizontal().Border(wxALL, 2));
	keyboard->Add(gridKeyRow2, wxSizerFlags().CenterHorizontal().Border(wxALL, 2));
	keyboard->Add(gridKeyRow3, wxSizerFlags().CenterHorizontal().Border(wxALL, 2));

	return keyboard;
}

cKeyboardENG::~cKeyboardENG()
{
	delete[]gridKey;
	delete[]keyLabel;
}
