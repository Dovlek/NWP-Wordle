#pragma once
#include "wx/wx.h"

enum class ThemeType
{
    DARK,
    LIGHT
};

class ThemeManager
{
public:
    // Get singleton instance
    static ThemeManager& Get();

    // Theme control
    ThemeType GetCurrentTheme() const { return currentTheme; }
    void SetTheme(ThemeType theme);

    // Color getters for UI elements
    wxColor GetBackgroundColor() const;
    wxColor GetTextColor() const;
    wxColor GetButtonColor() const;
    wxColor GetButtonHoverColor() const;
    wxColor GetButtonFocusColor() const;
    wxColor GetButtonDisabledColor() const;
    wxColor GetInputColor() const;
    wxColor GetToggleCheckedColor() const;
    wxColor GetHelpTextColor() const;

    // Colors that stay the same across themes
    wxColor GetErrorColor() const { return wxColor(255, 100, 100); }
    wxColor GetSuccessColor() const { return wxColor(100, 255, 100); }
    wxColor GetTimerGreenColor() const { return wxColor(100, 255, 100); }
    wxColor GetTimerOrangeColor() const { return wxColor(255, 165, 0); }
    wxColor GetTimerRedColor() const { return wxColor(255, 100, 100); }

private:
    ThemeManager();
    ~ThemeManager() = default;

    // Delete copy/move constructors
    ThemeManager(const ThemeManager&) = delete;
    ThemeManager& operator=(const ThemeManager&) = delete;
    ThemeManager(ThemeManager&&) = delete;
    ThemeManager& operator=(ThemeManager&&) = delete;

    ThemeType currentTheme;
};
