#pragma once
#include "UIScaler.h"
#include "WordSelector.h"
#include "cGrid.h"
#include "cKeyboard.h"
#include "wx/wx.h"
#include <map>
#include <set>

wxDECLARE_EVENT(wxEVT_SWITCH_TO_MENU, wxCommandEvent);

class cWordle : public wxPanel
{
public:
    cWordle(wxWindow* parent);
    ~cWordle();

    void StartNewRound();
    void ContinueFromFinishedGame();
    void ForceEndCurrentGame();
    bool IsGameInProgress() const;
    void ResetStats();
    wxString GetGameStateData() const;
    bool SetGameStateData(const wxString& data);

    enum class GameState
    {
        NOT_STARTED,
        ACTIVE,
        COMPLETED,
        COMPLETED_AWAITING_CONTINUE
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

    WordSelector wordSelector;
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
    wxTimer statusTimer;
    wxStaticText* winsText;
    wxStaticText* lossesText;
    wxStaticText* streakText;
    wxStaticText* maxStreakText;
    int wins = 0;
    int losses = 0;
    int streak = 0;
    int maxStreak = 0;
    void UpdateStatsUI();

    // Timer functionality
    wxTimer gameTimer;
    wxStaticText* timerDisplay;
    int timerRemainingSeconds = 300;
    bool timedModeEnabled = false;
    void OnTimerTick(wxTimerEvent& evt);
    void UpdateTimerDisplay();
    void StartTimer();
    void PauseTimer();
    void ResetTimer();

    // Hard mode functionality
    bool hardModeEnabled = false;
    std::map<int, std::set<wxChar>> requiredLetters;
    std::set<wxChar> mustUseLetters;
    bool ValidateHardModeGuess(const wxString& guess);
    void UpdateHardModeConstraints(const wxString& guess, const std::vector<LetterState>& states);
    void ResetHardModeConstraints();

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