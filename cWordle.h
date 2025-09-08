#pragma once
#include "wx/wx.h"
#include "cKeyboard.h"
#include "cGrid.h"
#include "WordSelector.h"

wxDECLARE_EVENT(wxEVT_SWITCH_TO_MENU, wxCommandEvent);

class cWordle : public wxPanel
{
public:
    cWordle(wxWindow* parent);
    ~cWordle();

    void StartNewRound();
    bool IsGameInProgress() const;

    enum class GameState
    {
        NOT_STARTED,
        ACTIVE,
        COMPLETED
    };

private:
    cGrid* cgrid;
    cKeyboardENG* ckeyboard_eng;
    wxBoxSizer* gameSizer;
    wxButton* backButton;
    int currentRow = 0;
    int currentCol = 0;
    int prevRow = 0;
    int prevCol = 0;
    GameState gameState = GameState::NOT_STARTED;
    
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

    wxStaticText* statusMessage;
    wxTimer* statusTimer;
    wxStaticText* winsText;
    wxStaticText* lossesText;
    wxStaticText* streakText;
    int wins = 0;
    int losses = 0;
    int streak = 0;
    void UpdateStatsUI();

    void ShowStatusMessage(const wxString& message, const wxColor& color = wxColor(*wxWHITE));
    void HideStatusMessage(wxTimerEvent& evt);
    void ShowGameEndDialog(bool won);

    enum
    {
        ID_ACCEL_ENTER,
        ID_ACCEL_BACKSPACE,
        ID_ACCEL_ESCAPE
    };

    void OnAcceleratorPressed(wxCommandEvent& evt);
    void OnBackButtonClicked(wxCommandEvent& evt);
    void OnBackButtonEnter(wxMouseEvent& evt);
    void OnBackButtonLeave(wxMouseEvent& evt);
};