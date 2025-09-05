#pragma once
#include "wx/wx.h"
#include "cKeyboard.h"
#include "cGrid.h"
#include "WordSelector.h"

class cMain : public wxFrame
{
public:
    cMain();
    ~cMain();

private:
    cGrid* cgrid;
    cKeyboardENG* ckeyboard_eng;
    int currentRow = 0;
    int currentCol = 0;
    int prevRow = 0;
    int prevCol = 0;
    void OnKeyboardButtonClicked(wxCommandEvent& evt);
    void OnKeyboardButtonPressed(wxKeyEvent& evt);
    void ProcessKey(const wxString& key);
    WordSelector* wordSelector;
    wxString targetWord;

    enum class LetterState
    {
        WRONG,
        WRONG_POSITION,
        CORRECT
    };

    void CheckGuess(const wxString& guess, int row);
    std::vector<LetterState> CompareWords(const wxString& guess, const wxString& target);
};

