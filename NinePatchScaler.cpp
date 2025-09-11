#include "NinePatchScaler.h"
#include <map>

NinePatchScaler::NinePatchScaler()
{
}

NinePatchScaler& NinePatchScaler::GetInstance()
{
    static NinePatchScaler instance;
    return instance;
}

wxBitmap NinePatchScaler::ScaleBitmap(const wxBitmap& originalBitmap, const wxSize& targetSize, int borderSize) const
{
    if (!originalBitmap.IsOk() || targetSize.GetWidth() <= 0 || targetSize.GetHeight() <= 0)
        return wxBitmap();
    
    // If target size is same as original, return copy
    if (originalBitmap.GetSize() == targetSize)
        return wxBitmap(originalBitmap);
    
    return performNinePatchScale(originalBitmap, targetSize, borderSize);
}

wxBitmap NinePatchScaler::GetScaledBitmap(const wxString& resourceName, const wxSize& baseSize, const wxSize& targetSize, int borderSize)
{
    wxString cacheKey = generateCacheKey(resourceName, targetSize, borderSize);
    
    // Check if already cached
    auto it = bitmapCache.find(cacheKey);
    if (it != bitmapCache.end() && it->second.IsOk())
        return it->second;
    
    // Load original bitmap from resource
    wxBitmap originalBitmap(resourceName, wxBITMAP_TYPE_BMP_RESOURCE);
    if (!originalBitmap.IsOk())
        return wxBitmap();
    
    // Scale the bitmap
    wxBitmap scaledBitmap = ScaleBitmap(originalBitmap, targetSize, borderSize);
    
    // Cache the result
    if (scaledBitmap.IsOk())
        bitmapCache[cacheKey] = scaledBitmap;
    
    return scaledBitmap;
}

void NinePatchScaler::ClearCache()
{
    bitmapCache.clear();
}

wxBitmap NinePatchScaler::performNinePatchScale(const wxBitmap& source, const wxSize& targetSize, int borderSize) const
{
    wxImage sourceImage = source.ConvertToImage();
    if (!sourceImage.IsOk())
        return wxBitmap();
    
    int srcWidth = sourceImage.GetWidth();
    int srcHeight = sourceImage.GetHeight();
    int targetWidth = targetSize.GetWidth();
    int targetHeight = targetSize.GetHeight();
    
    // Ensure border size doesn't exceed half the source dimensions
    borderSize = wxMin(borderSize, wxMin(srcWidth / 2, srcHeight / 2));
    
    // Create target image
    wxImage targetImage(targetWidth, targetHeight);
    if (!targetImage.IsOk())
        return wxBitmap();
    
    // Calculate dimensions for nine patches
    int centerSrcWidth = srcWidth - 2 * borderSize;
    int centerSrcHeight = srcHeight - 2 * borderSize;
    int centerTargetWidth = targetWidth - 2 * borderSize;
    int centerTargetHeight = targetHeight - 2 * borderSize;
    
    // Ensure center dimensions are positive
    if (centerTargetWidth <= 0 || centerTargetHeight <= 0 || centerSrcWidth <= 0 || centerSrcHeight <= 0)
    {
        // Fallback to simple scaling if nine-patch isn't possible
        return wxBitmap(sourceImage.Scale(targetWidth, targetHeight, wxIMAGE_QUALITY_HIGH));
    }
    
    // Copy corner pixels (no scaling needed)
    // Top-left corner
    for (int y = 0; y < borderSize; y++)
    {
        for (int x = 0; x < borderSize; x++)
        {
            targetImage.SetRGB(x, y, 
                sourceImage.GetRed(x, y),
                sourceImage.GetGreen(x, y),
                sourceImage.GetBlue(x, y));
        }
    }
    
    // Top-right corner
    for (int y = 0; y < borderSize; y++)
    {
        for (int x = 0; x < borderSize; x++)
        {
            targetImage.SetRGB(targetWidth - borderSize + x, y,
                sourceImage.GetRed(srcWidth - borderSize + x, y),
                sourceImage.GetGreen(srcWidth - borderSize + x, y),
                sourceImage.GetBlue(srcWidth - borderSize + x, y));
        }
    }
    
    // Bottom-left corner
    for (int y = 0; y < borderSize; y++)
    {
        for (int x = 0; x < borderSize; x++)
        {
            targetImage.SetRGB(x, targetHeight - borderSize + y,
                sourceImage.GetRed(x, srcHeight - borderSize + y),
                sourceImage.GetGreen(x, srcHeight - borderSize + y),
                sourceImage.GetBlue(x, srcHeight - borderSize + y));
        }
    }
    
    // Bottom-right corner
    for (int y = 0; y < borderSize; y++)
    {
        for (int x = 0; x < borderSize; x++)
        {
            targetImage.SetRGB(targetWidth - borderSize + x, targetHeight - borderSize + y,
                sourceImage.GetRed(srcWidth - borderSize + x, srcHeight - borderSize + y),
                sourceImage.GetGreen(srcWidth - borderSize + x, srcHeight - borderSize + y),
                sourceImage.GetBlue(srcWidth - borderSize + x, srcHeight - borderSize + y));
        }
    }
    
    // Scale and copy edges
    // Top edge
    wxImage topEdge = sourceImage.GetSubImage(wxRect(borderSize, 0, centerSrcWidth, borderSize));
    topEdge = topEdge.Scale(centerTargetWidth, borderSize, wxIMAGE_QUALITY_HIGH);
    targetImage.Paste(topEdge, borderSize, 0);
    
    // Bottom edge
    wxImage bottomEdge = sourceImage.GetSubImage(wxRect(borderSize, srcHeight - borderSize, centerSrcWidth, borderSize));
    bottomEdge = bottomEdge.Scale(centerTargetWidth, borderSize, wxIMAGE_QUALITY_HIGH);
    targetImage.Paste(bottomEdge, borderSize, targetHeight - borderSize);
    
    // Left edge
    wxImage leftEdge = sourceImage.GetSubImage(wxRect(0, borderSize, borderSize, centerSrcHeight));
    leftEdge = leftEdge.Scale(borderSize, centerTargetHeight, wxIMAGE_QUALITY_HIGH);
    targetImage.Paste(leftEdge, 0, borderSize);
    
    // Right edge
    wxImage rightEdge = sourceImage.GetSubImage(wxRect(srcWidth - borderSize, borderSize, borderSize, centerSrcHeight));
    rightEdge = rightEdge.Scale(borderSize, centerTargetHeight, wxIMAGE_QUALITY_HIGH);
    targetImage.Paste(rightEdge, targetWidth - borderSize, borderSize);
    
    // Scale and copy center
    wxImage center = sourceImage.GetSubImage(wxRect(borderSize, borderSize, centerSrcWidth, centerSrcHeight));
    center = center.Scale(centerTargetWidth, centerTargetHeight, wxIMAGE_QUALITY_HIGH);
    targetImage.Paste(center, borderSize, borderSize);
    
    return wxBitmap(targetImage);
}

wxString NinePatchScaler::generateCacheKey(const wxString& resourceName, const wxSize& targetSize, int borderSize) const
{
    return wxString::Format("%s_%dx%d_b%d", resourceName, targetSize.GetWidth(), targetSize.GetHeight(), borderSize);
}