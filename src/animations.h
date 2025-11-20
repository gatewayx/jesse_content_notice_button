#ifndef ANIMATIONS_H
#define ANIMATIONS_H

#include <Arduino.h>
#include <M5GFX.h>

// Color utility functions
uint16_t dimColor(uint16_t color, float brightness);
uint16_t blendColor(uint16_t color1, uint16_t color2, float t);

// Breathing effect
float getBreathingBrightness(unsigned long period = 3000);

// Animation helper - get smooth sine wave value 0.0 to 1.0
float getSineWave(unsigned long period);

// Radial gradient helpers
void drawRadialGradient(M5GFX* display, int centerX, int centerY, int maxRadius,
                        uint16_t innerColor, uint16_t outerColor, float pulsePhase = 0);

// Ambient cloud effect (multi-layer radial gradients)
void drawAmbientClouds(M5GFX* display, int width, int height, unsigned long time);

// Frame timing helper
class FrameTimer {
private:
    unsigned long lastFrameTime;
    unsigned long frameInterval;

public:
    FrameTimer(int fps = 30);
    bool shouldUpdate();
    unsigned long getElapsed();
};

#endif // ANIMATIONS_H
