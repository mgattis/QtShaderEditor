#ifndef CASPECTRATIO_H
#define CASPECTRATIO_H

class CAspectRatio {
private:
    // Output stuff.
    float fAspect;
    float fViewWidth;
    float fViewHeight;

    // user input.
    float fWindowWidth;
    float fWindowHeight;
    int iPixelWidth;
    int iPixelHeight;
    float fMultiplier;

    // Internal stuff.
    bool bXUsePixel;
    bool bYUsePixel;
    bool bAspectChanged;

    // Remeber the last thing we gave it.
    int iSourceWidth;
    int iSourceHeight;

public:
    CAspectRatio();

    float getAspectRatio(int iSourceWidth, int iSourceHeight);
    float getAspectRatio();
    bool isAspectChanged();

    void setWindowWidth(float fWindowWidth);
    void setWindowHeight(float fWindowHeight);
    void setPixelWidth(int iPixelWidth);
    void setPixelHeight(int iPixelHeight);
    void setMultiplier(float fMultiplier);

    float getViewWidth();
    float getViewHeight();
};

#endif // CASPECTRATIO_H
