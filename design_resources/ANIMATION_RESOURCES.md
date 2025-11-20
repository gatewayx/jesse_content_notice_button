# Animation Resources & Code Examples

This document contains links and code snippets from web research for implementing animations on the M5Stack AtomS3.

---

## M5GFX Official Examples

### 1. AnalogMeter Example
**Source**: https://github.com/m5stack/M5GFX/blob/master/examples/Basic/AnalogMeter/AnalogMeter.ino

**What it demonstrates**:
- Smooth sine-wave based animations
- Arc filling for gauge needles
- Selective screen updates (clipping regions)
- 35ms update interval for fluid motion

**Key Technique**:
```cpp
// Generate smooth sine wave values
float deg_to_rad = 0.017453292519943295769236907684886;
value[0] = 50 + roundf(50 * sinf((d + 0) * deg_to_rad));

// Draw animated arc
display.fillArc(x, y, r0, r1, start_angle, end_angle, color);
```

**Useful for**: Gauge animations, pointer movements, smooth value transitions

---

### 2. TFT_ArcFill Example
**Source**: https://github.com/m5stack/M5Stack/blob/master/examples/Advanced/Display/TFT_ArcFill/TFT_ArcFill.ino

**What it demonstrates**:
- Continuous rotating arc animations
- Rainbow color cycling
- Brightness modulation
- Elliptical arc drawing

**Key Functions**:
```cpp
// Rainbow color generator (cycles through 6 color states)
unsigned int rainbow(byte value);

// Brightness adjustment
unsigned int brightness(unsigned int color, int brightness);

// Animated arc filling
fillArc(x, y, start_angle, seg_count, rx, ry, width, color);
```

**Useful for**: Loading spinners, progress indicators, color transitions

---

## Sprite Animation

### M5Canvas Documentation
**Source**: https://docs.m5stack.com/en/arduino/m5gfx/m5gfx_sprite

**What it provides**:
- Pre-rendered graphics in memory
- Fast display updates with `pushSprite()`
- Transparency support
- Rotation and scaling

**Basic Usage**:
```cpp
M5Canvas sprite(&display);
sprite.createSprite(width, height);
sprite.fillRect(0, 0, width, height, color);
sprite.pushSprite(x, y);  // Draw to screen
sprite.deleteSprite();     // Free memory
```

**Useful for**: Complex animations, frame-based animation, reducing flicker

---

### Frame-Based Animation Example
**Source**: https://www.hackster.io/tommyho/m5gfx-media-player-jpg-png-bmp-gif-ab76a9

**What it demonstrates**:
- Loading multiple image frames
- Playing frames in sequence
- Creating animated effects from static images

**Technique**:
- Store multiple frames as separate images
- Load and display frames in a loop
- Control timing with `delay()` or `millis()`

**Useful for**: Complex animated sequences, icons with multiple states

---

## Progress & Loading Animations

### Circular Progress Indicators
**M5GFX Functions**:
```cpp
// Draw progress ring
void drawProgressRing(int x, int y, int radius, float progress, uint16_t color) {
    int angle = 360 * progress;  // progress = 0.0 to 1.0
    display.fillArc(x, y, radius-5, radius, 0, angle, color);
}
```

**Animation Pattern**:
```cpp
unsigned long startTime = millis();
float progress = 0.0;

void loop() {
    unsigned long elapsed = millis() - startTime;
    progress = min(1.0, elapsed / 5000.0);  // 5 second fill

    display.clear();
    drawProgressRing(64, 64, 50, progress, GREEN);
    delay(16);  // ~60fps
}
```

**Useful for**: Hold detection, loading bars, timers

---

### Spinning Loading Animation
**Technique**: Rotating arc segments

```cpp
int spinnerAngle = 0;

void drawSpinner(int x, int y, int radius, uint16_t color) {
    // Draw 3 arc segments offset by 120 degrees
    for (int i = 0; i < 3; i++) {
        int angle = (spinnerAngle + i * 120) % 360;
        display.fillArc(x, y, radius-3, radius, angle, angle+30, color);
    }
}

void loop() {
    display.clear();
    drawSpinner(64, 64, 40, BLUE);
    spinnerAngle = (spinnerAngle + 10) % 360;  // Rotate
    delay(50);  // 20fps
}
```

**Useful for**: Loading indicators, "working" status

---

## Breathing & Pulsing Effects

### Breathing Animation
**Source**: https://thingpulse.com/breathing-leds-cracking-the-algorithm-behind-our-breathing-pattern/

**Mathematical Approach**:
```cpp
unsigned long breathingPeriod = 3000;  // 3 seconds per cycle

float getBreathingBrightness() {
    float phase = (millis() % breathingPeriod) / (float)breathingPeriod;
    // Sine wave: 0.0 to 1.0
    return 0.5 + 0.5 * sin(phase * 2 * PI);
}

void drawBreathingBackground() {
    float brightness = getBreathingBrightness();
    // Dim green: dark green (0.3) to bright green (1.0)
    float scaledBrightness = 0.3 + 0.7 * brightness;
    uint16_t color = dimColor(GREEN, scaledBrightness);
    display.fillScreen(color);
}
```

**Useful for**: Idle states, ambient animations, "alive" indicators

---

### Pulsing Circle
**Technique**: Expanding/contracting radius

```cpp
void drawPulsingCircle(int x, int y, int baseRadius, uint16_t color) {
    float phase = (millis() % 2000) / 2000.0;  // 2 second cycle
    float scale = 0.8 + 0.4 * sin(phase * 2 * PI);  // 0.8 to 1.2
    int radius = baseRadius * scale;
    display.fillCircle(x, y, radius, color);
}
```

**Useful for**: Attention-getting effects, error states

---

## Button Press Feedback

### Flash Effect
```cpp
void flashScreen(uint16_t color, int duration) {
    display.fillScreen(color);
    delay(duration);
    // Redraw normal screen
}

// Usage: flashScreen(WHITE, 50);
```

**Useful for**: Immediate button feedback

---

### Expanding Ring (Ripple Effect)
```cpp
void drawExpandingRing(int x, int y, unsigned long startTime, int duration, uint16_t color) {
    unsigned long elapsed = millis() - startTime;
    if (elapsed > duration) return;

    float progress = elapsed / (float)duration;
    int radius = 10 + (50 * progress);  // Expand from 10 to 60 pixels
    float alpha = 1.0 - progress;        // Fade out

    // Draw ring with decreasing brightness
    uint16_t fadedColor = dimColor(color, alpha);
    display.drawCircle(x, y, radius, fadedColor);
    display.drawCircle(x, y, radius-1, fadedColor);
    display.drawCircle(x, y, radius-2, fadedColor);
}
```

**Useful for**: Touch feedback, success confirmation

---

## Color Utilities

### Color Dimming
```cpp
// Dim a 16-bit RGB565 color by a factor (0.0 to 1.0)
uint16_t dimColor(uint16_t color, float brightness) {
    // Extract RGB components
    uint8_t r = ((color >> 11) & 0x1F) * brightness;
    uint8_t g = ((color >> 5) & 0x3F) * brightness;
    uint8_t b = (color & 0x1F) * brightness;

    // Recombine
    return (r << 11) | (g << 5) | b;
}
```

---

### Color Interpolation
```cpp
// Blend between two colors
uint16_t blendColor(uint16_t color1, uint16_t color2, float t) {
    // Extract RGB from color1
    uint8_t r1 = (color1 >> 11) & 0x1F;
    uint8_t g1 = (color1 >> 5) & 0x3F;
    uint8_t b1 = color1 & 0x1F;

    // Extract RGB from color2
    uint8_t r2 = (color2 >> 11) & 0x1F;
    uint8_t g2 = (color2 >> 5) & 0x3F;
    uint8_t b2 = color2 & 0x1F;

    // Interpolate
    uint8_t r = r1 + (r2 - r1) * t;
    uint8_t g = g1 + (g2 - g1) * t;
    uint8_t b = b1 + (b2 - b1) * t;

    return (r << 11) | (g << 5) | b;
}
```

**Useful for**: Smooth color transitions, progress indicators

---

## Timing & Non-Blocking Animation

### Frame-Rate Control
```cpp
unsigned long lastFrameTime = 0;
const int FRAME_RATE = 30;  // 30 fps
const int FRAME_INTERVAL = 1000 / FRAME_RATE;

void loop() {
    unsigned long currentTime = millis();

    if (currentTime - lastFrameTime >= FRAME_INTERVAL) {
        lastFrameTime = currentTime;

        // Update animation state
        updateAnimation();

        // Redraw screen
        drawFrame();
    }
}
```

---

### State-Based Animation
```cpp
enum AnimState {
    ANIM_IDLE,
    ANIM_LOADING,
    ANIM_SUCCESS,
    ANIM_ERROR
};

AnimState currentAnim = ANIM_IDLE;
unsigned long animStartTime = 0;

void startAnimation(AnimState newState) {
    currentAnim = newState;
    animStartTime = millis();
}

void updateAnimation() {
    unsigned long elapsed = millis() - animStartTime;

    switch (currentAnim) {
        case ANIM_IDLE:
            drawBreathingBackground();
            break;
        case ANIM_LOADING:
            drawSpinner(64, 64, 40, BLUE);
            break;
        case ANIM_SUCCESS:
            drawExpandingRing(64, 64, animStartTime, 2000, GREEN);
            if (elapsed > 2000) {
                startAnimation(ANIM_IDLE);
            }
            break;
        case ANIM_ERROR:
            drawPulsingCircle(64, 64, 30, RED);
            if (elapsed > 3000) {
                startAnimation(ANIM_IDLE);
            }
            break;
    }
}
```

---

## M5GFX Color Constants

```cpp
// Common colors (RGB565 format)
#define BLACK       0x0000
#define NAVY        0x000F
#define DARKGREEN   0x03E0
#define DARKCYAN    0x03EF
#define MAROON      0x7800
#define PURPLE      0x780F
#define OLIVE       0x7BE0
#define LIGHTGREY   0xC618
#define DARKGREY    0x7BEF
#define BLUE        0x001F
#define GREEN       0x07E0
#define CYAN        0x07FF
#define RED         0xF800
#define MAGENTA     0xF81F
#define YELLOW      0xFFE0
#define WHITE       0xFFFF
#define ORANGE      0xFD20
#define GREENYELLOW 0xAFE5
#define PINK        0xF81F
```

---

## Performance Tips

1. **Minimize Full Screen Redraws**:
   - Use sprites for complex elements
   - Only redraw changed areas
   - Use clipping rectangles

2. **Optimize Draw Calls**:
   - Batch similar operations
   - Use filled shapes instead of outlines when possible
   - Pre-calculate values outside of loops

3. **Frame Rate**:
   - Target 30fps (33ms per frame) for smooth animation
   - Use `millis()` instead of `delay()` for non-blocking timing
   - Check `display.displayBusy()` before updates

4. **Memory Management**:
   - Delete sprites when not in use
   - Reuse sprite buffers when possible
   - Monitor heap memory on ESP32

---

## Additional Resources

### Documentation
- **M5GFX API Reference**: https://docs.m5stack.com/en/arduino/m5gfx/m5gfx_functions
- **M5GFX GitHub**: https://github.com/m5stack/M5GFX
- **M5AtomS3 Docs**: https://docs.m5stack.com/en/core/AtomS3

### Community Examples
- **M5Stack Forums**: https://community.m5stack.com/
- **M5Stack GitHub Examples**: https://github.com/m5stack/M5AtomS3/tree/main/examples
- **Hackster.io M5Stack Projects**: https://www.hackster.io/m5stack/projects

### Related Libraries
- **FastLED** (for LED strip effects): Already included in project dependencies
- **LovyanGFX** (M5GFX is based on this): https://github.com/lovyan03/LovyanGFX
- **TFT_eSPI** (alternative graphics library): https://github.com/Bodmer/TFT_eSPI

---

*Use this document as a reference when implementing animations in the event tracker project.*
