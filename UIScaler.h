#pragma once
#include "wx/wx.h"
#include <wx/display.h>

class UIScaler
{
public:
    ~UIScaler() = default;
    
    // Get the singleton instance
    static UIScaler& GetInstance();
    
    // Scaling methods
    int ScaledFontSize(int baseSize) const;
    wxSize ScaledSize(int baseWidth, int baseHeight) const;
    int ScaledValue(int baseValue) const;
    double GetScaleFactor() const { return scaleFactor; }
    
    // Force recalculation (useful if display changes)
    void RecalculateScaleFactor();

private:
    UIScaler();
    double scaleFactor;
    void calculateScaleFactor();

    // Delete copy/move to prevent copying
    UIScaler(const UIScaler&) = delete;
    UIScaler& operator=(const UIScaler&) = delete;
    UIScaler(UIScaler&&) = delete;
    UIScaler& operator=(UIScaler&&) = delete;
};