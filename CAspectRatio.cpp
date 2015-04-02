#include "CAspectRatio.h"

CAspectRatio::CAspectRatio() {
	fAspect = 1.0;

    fWindowWidth = 1.0;
    fWindowHeight = 1.0;
    iPixelWidth = 800;
    iPixelHeight = 600;

    fMultiplier = 1.0;

    bXUsePixel = false;
    bXUsePixel = false;
}

float CAspectRatio::getAspectRatio(int iActualPixelWidth, int iActualPixelHeight) {
    float runningX;
    float runningY;

    if (bXUsePixel) {
        runningX = (float)iPixelWidth;
    }
    else {
        runningX = (float)iActualPixelWidth * fWindowWidth;
    }

    if (bYUsePixel) {
        runningY = (float)iPixelHeight;
    }
    else {
        runningY = (float)iActualPixelHeight * fWindowHeight;
    }

    runningX *= fMultiplier;
    runningY *= fMultiplier;

	fAspect = runningX / runningY;
	return fAspect;
}

float CAspectRatio::getAspectRatio() {
	return fAspect;
}

void CAspectRatio::setWindowWidth(float fWindowWidth) {
    this->fWindowWidth = fWindowWidth;
    bXUsePixel = false;
}

void CAspectRatio::setWindowHeight(float fWindowHeight) {
    this->fWindowHeight = fWindowHeight;
    bYUsePixel = false;
}

void CAspectRatio::setPixelWidth(int iPixelWidth) {
    this->iPixelWidth = iPixelWidth;
    bXUsePixel = true;
}

void CAspectRatio::setPixelHeight(int iPixelHeight) {
    this->iPixelHeight = iPixelHeight;
    bYUsePixel = true;
}
