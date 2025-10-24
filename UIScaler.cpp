#include "UIScaler.h"
#include "NinePatchScaler.h"

UIScaler::UIScaler()
{
    calculateScaleFactor();
}

UIScaler& UIScaler::GetInstance()
{
    static UIScaler instance;
    return instance;
}

void UIScaler::calculateScaleFactor()
{
    // Get primary display size
    wxDisplay display;
    wxRect screenRect = display.GetGeometry();
    int screenHeight = screenRect.GetHeight();

    // Assume 1080p (1920x1080) base resolution
    const int baseHeight = 1080;

    // Calculate scale factor based on screen height
    scaleFactor = static_cast<double>(screenHeight) / baseHeight;

    // Clamp scale factor between 0.5 and 1.5
    if (scaleFactor < 0.5)
        scaleFactor = 0.5;
    else if (scaleFactor > 1.5)
        scaleFactor = 1.5;

    // wxLogMessage("Final scale factor: %.2f", scaleFactor);
}

int UIScaler::ScaledFontSize(int baseSize) const
{
    int scaled = static_cast<int>(std::lround(static_cast<double>(baseSize) * scaleFactor));
    // Ensure minimum readable font size
    return scaled < 8 ? 8 : scaled;
}

wxSize UIScaler::ScaledSize(int baseWidth, int baseHeight) const
{
    const int w = static_cast<int>(std::lround(static_cast<double>(baseWidth) * scaleFactor));
    const int h = static_cast<int>(std::lround(static_cast<double>(baseHeight) * scaleFactor));
    return wxSize(w, h);
}

int UIScaler::ScaledValue(int baseValue) const
{
    return static_cast<int>(std::lround(static_cast<double>(baseValue) * scaleFactor));
}

void UIScaler::RecalculateScaleFactor()
{
    calculateScaleFactor();
    NinePatchScaler::GetInstance().ClearCache();
}