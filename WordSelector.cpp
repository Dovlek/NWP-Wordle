#include "WordSelector.h"
#include "EmbeddedResources.h"
#include <algorithm>
#include <ctime>

WordSelector::WordSelector() : rng(static_cast<unsigned int>(std::time(nullptr)))
{
    LoadWordsFromResource();
}

bool WordSelector::LoadWordsFromResource()
{
    words.clear();

    // Load words from embedded array
    for (size_t i = 0; i < EMBEDDED_WORDS_COUNT; i++)
    {
        wxString word(EMBEDDED_WORDS[i], wxConvUTF8);
        words.push_back(word);
    }

    return !words.empty();
}

wxString WordSelector::GetRandomWord()
{
    if (words.empty())
        return wxEmptyString;

    std::uniform_int_distribution<size_t> dist(0, words.size() - 1);
    size_t randomIndex = dist(rng);

    return words[randomIndex];
}#include "cMain.h"
#include "EmbeddedResources.h"
#include <wx/mstream.h>
#ifdef __WXMSW__
    #include <dwmapi.h>
    #include <windows.h>
    #pragma comment(lib, "dwmapi.lib")
#endif

// Platform-specific initialization
#ifdef __WXMSW__
static void EnableDarkTitleBar(wxFrame* frame)
{
    HWND hwnd = frame->GetHWND();
    if (hwnd)
    {

bool WordSelector::IsValidWord(const wxString& word) const
{
    if (words.empty())
        return false;

    return std::find(words.begin(), words.end(), word) != words.end();
}