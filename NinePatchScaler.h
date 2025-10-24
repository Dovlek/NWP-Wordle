#pragma once
#include <map>
#include <wx/wx.h>

class NinePatchScaler
{
public:
    static NinePatchScaler& GetInstance();

    // Scale a bitmap using nine-patch technique
    wxBitmap ScaleBitmap(const wxBitmap& originalBitmap, const wxSize& targetSize, int borderSize = 3) const;

    // Scale bitmaps from resource and cache them
    wxBitmap GetScaledBitmap(const wxString& resourceName, const wxSize& baseSize, const wxSize& targetSize, int borderSize = 3);

    // Clear the cache (useful when resolution changes)
    void ClearCache();

private:
    NinePatchScaler();
    ~NinePatchScaler() = default;

    // Delete copy/move to prevent copying
    NinePatchScaler(const NinePatchScaler&) = delete;
    NinePatchScaler& operator=(const NinePatchScaler&) = delete;
    NinePatchScaler(NinePatchScaler&&) = delete;
    NinePatchScaler& operator=(NinePatchScaler&&) = delete;

    // Cache for scaled bitmaps
    std::map<wxString, wxBitmap> bitmapCache;

    // Nine-patch scaling implementation
    wxBitmap performNinePatchScale(const wxBitmap& source, const wxSize& targetSize, int borderSize) const;

    // Generate cache key
    wxString generateCacheKey(const wxString& resourceName, const wxSize& targetSize, int borderSize) const;
};