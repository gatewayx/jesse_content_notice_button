#include "animations.h"

/**
 * Dim a 16-bit RGB565 color by a brightness factor
 */
uint16_t dimColor(uint16_t color, float brightness) {
    if (brightness < 0.0) brightness = 0.0;
    if (brightness > 1.0) brightness = 1.0;

    uint8_t r = ((color >> 11) & 0x1F);
    uint8_t g = ((color >> 5) & 0x3F);
    uint8_t b = (color & 0x1F);

    r = (uint8_t)(r * brightness);
    g = (uint8_t)(g * brightness);
    b = (uint8_t)(b * brightness);

    return (r << 11) | (g << 5) | b;
}

/**
 * Blend between two RGB565 colors
 */
uint16_t blendColor(uint16_t color1, uint16_t color2, float t) {
    if (t < 0.0) t = 0.0;
    if (t > 1.0) t = 1.0;

    uint8_t r1 = (color1 >> 11) & 0x1F;
    uint8_t g1 = (color1 >> 5) & 0x3F;
    uint8_t b1 = color1 & 0x1F;

    uint8_t r2 = (color2 >> 11) & 0x1F;
    uint8_t g2 = (color2 >> 5) & 0x3F;
    uint8_t b2 = color2 & 0x1F;

    uint8_t r = r1 + (r2 - r1) * t;
    uint8_t g = g1 + (g2 - g1) * t;
    uint8_t b = b1 + (b2 - b1) * t;

    return (r << 11) | (g << 5) | b;
}

/**
 * Get breathing brightness value using sine wave
 */
float getBreathingBrightness(unsigned long period) {
    float phase = (millis() % period) / (float)period;
    return 0.5 + 0.5 * sin(phase * 2.0 * PI);
}

/**
 * Get smooth sine wave value
 */
float getSineWave(unsigned long period) {
    return getBreathingBrightness(period);
}

/**
 * Draw a simple radial gradient - smooth, no flicker
 * Only draws once, designed for static background
 */
void drawRadialGradient(M5GFX* display, int centerX, int centerY, int maxRadius,
                        uint16_t innerColor, uint16_t outerColor, float pulsePhase) {
    // Draw from outside in
    for (int r = maxRadius; r >= 0; r -= 2) {
        float t = (float)r / maxRadius;
        t = t * (0.9 + 0.1 * pulsePhase);
        if (t > 1.0) t = 1.0;

        uint16_t color = blendColor(innerColor, outerColor, t);
        display->fillCircle(centerX, centerY, r, color);
    }
}

/**
 * Draw simple pulsing radial gradient background
 * Much simpler - just one gradient that pulses gently
 */
void drawAmbientClouds(M5GFX* display, int width, int height, unsigned long time) {
    // Simple color palette
    uint16_t darkGreen = 0x0180;     // Very dark green
    uint16_t midGreen = 0x02C0;      // Dark-mid green
    uint16_t brightGreen = 0x0460;   // Medium green

    // Single slow pulse (5 second cycle)
    float phase = (time % 5000) / 5000.0;
    float pulse = 0.5 + 0.5 * sin(phase * 2.0 * PI);

    // Blend between dark and mid green based on pulse
    uint16_t centerColor = blendColor(midGreen, brightGreen, pulse * 0.6);

    // Draw single radial gradient centered
    int centerX = width / 2;
    int centerY = height / 2;

    // Simple gradient from center out
    drawRadialGradient(display, centerX, centerY, 90, centerColor, darkGreen, pulse);
}

/**
 * Frame timer constructor
 */
FrameTimer::FrameTimer(int fps) {
    frameInterval = 1000 / fps;
    lastFrameTime = 0;
}

/**
 * Check if it's time to update the next frame
 */
bool FrameTimer::shouldUpdate() {
    unsigned long currentTime = millis();
    if (currentTime - lastFrameTime >= frameInterval) {
        lastFrameTime = currentTime;
        return true;
    }
    return false;
}

/**
 * Get elapsed time since last frame update
 */
unsigned long FrameTimer::getElapsed() {
    return millis() - lastFrameTime;
}
