#include "CAspectRatio.h"

CAspectRatio::CAspectRatio() {
	fAspect = 1.0;
    fViewWidth = 800.0;
    fViewHeight = 600.0;

    fWindowWidth = 1.0;
    fWindowHeight = 1.0;
    iPixelWidth = 800;
    iPixelHeight = 600;
    fMultiplier = 1.0;

    bXUsePixel = false;
    bXUsePixel = false;
    bAspectChanged = true;

    iSourceWidth = 800;
    iSourceHeight = 600;
}

float CAspectRatio::getAspectRatio(int iSourceWidth, int iSourceHeight) {
    bAspectChanged = this->iSourceWidth != iSourceWidth ? true : bAspectChanged;
    bAspectChanged = this->iSourceHeight != iSourceHeight ? true : bAspectChanged;

    if (bAspectChanged) {
        this->iSourceWidth = iSourceWidth;
        this->iSourceHeight = iSourceHeight;

        if (bXUsePixel) {
            this->fViewWidth = (float)iPixelWidth;
        }
        else {
            this->fViewWidth = (float)iSourceWidth * fWindowWidth;
        }

        if (bYUsePixel) {
            this->fViewHeight = (float)iPixelHeight;
        }
        else {
            this->fViewHeight = (float)iSourceHeight * fWindowHeight;
        }

        this->fViewWidth *= fMultiplier;
        this->fViewHeight *= fMultiplier;

        fAspect = this->fViewWidth / this->fViewHeight;

        bAspectChanged = false;
    }

	return fAspect;
}

float CAspectRatio::getAspectRatio() {
	return getAspectRatio(iSourceWidth, iSourceHeight);
}

bool CAspectRatio::isAspectChanged() {
    return bAspectChanged;
}

void CAspectRatio::setWindowWidth(float fWindowWidth) {
    this->fWindowWidth = fWindowWidth;
    bXUsePixel = false;
    bAspectChanged = true;
}

void CAspectRatio::setWindowHeight(float fWindowHeight) {
    this->fWindowHeight = fWindowHeight;
    bYUsePixel = false;
    bAspectChanged = true;
}

void CAspectRatio::setPixelWidth(int iPixelWidth) {
    this->iPixelWidth = iPixelWidth;
    bXUsePixel = true;
    bAspectChanged = true;
}

void CAspectRatio::setPixelHeight(int iPixelHeight) {
    this->iPixelHeight = iPixelHeight;
    bYUsePixel = true;
    bAspectChanged = true;
}

void CAspectRatio::setMultiplier(float fMultiplier) {
    this->fMultiplier = fMultiplier;
    bAspectChanged = true;
}

float CAspectRatio::getViewWidth() {
    return fViewWidth;
}

float CAspectRatio::getViewHeight() {
    return fViewHeight;
}
