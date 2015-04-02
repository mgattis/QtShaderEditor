#ifndef CASPECTRATIO_H
#define CASPECTRATIO_H

class CAspectRatio {
private:
    float fAspect;

    float fWindowWidth;
    float fWindowHeight;
    int iPixelWidth;
    int iPixelHeight;
    float fMultiplier;

    bool bXUsePixel;
    bool bYUsePixel;
    bool bAspectChanged;

public:
    CAspectRatio();

    float getAspectRatio(int iActualPixelWidth, int iActualPixelHeight);
    float getAspectRatio();

    void setWindowWidth(float fWindowWidth);
    void setWindowHeight(float fWindowHeight);
    void setPixelWidth(int iPixelWidth);
    void setPixelHeight(int iPixelHeight);
};

#endif // CASPECTRATIO_H
