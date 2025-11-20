# Event Tracker Button - Project Specification

## Project Overview

An M5Stack AtomS3-based event timestamp recorder with visual feedback. Users press a button to record timestamps of witnessed events via an external API. The device provides clear visual feedback for all states and actions, with animations to confirm system responsiveness.

## Hardware
- **Device**: M5Stack AtomS3 (ESP32-S3)
- **Display**: 128x128 pixel LCD
- **Input**: Single physical button (BtnA)
- **Connectivity**: WiFi for API communication

---

## User Experience Flow

### 1. Power-On Sequence
**State**: Device startup
**Duration**: 2-3 seconds
**Visual**:
- Animated boot sequence (e.g., expanding circle or wave effect)
- Brief display of device name/version
- Transition to WiFi connection state

**Implementation Ideas**:
- Use `fillArc()` with expanding radius
- Pulsing circle animation
- Color transition from dark to bright

---

### 2. WiFi Connection States

#### 2A. Connecting to WiFi
**State**: Attempting WiFi connection
**Visual**:
- Animated spinner/loading indicator
- Subtle text: "Connecting..."
- Continuous motion to show active status

**Implementation Ideas**:
- Rotating arc segments (spinner pattern)
- Pulsing dots
- Circular progress indicator

#### 2B. WiFi Connected - Idle State
**State**: Connected, waiting for button press
**Visual**:
- Clear "Ready" message or icon
- Ambient animation (breathing effect, slow pulse, or gentle movement)
- Status indicator (e.g., small WiFi icon, always visible)

**Implementation Ideas**:
- Breathing background using sine wave brightness modulation
- Slowly rotating subtle element
- Gentle pulsing glow around button prompt
- Default message: "Press to Record"

#### 2C. WiFi Error State
**State**: Cannot connect to WiFi
**Visual**:
- **OBVIOUS ERROR**: Red background or prominent error icon
- Clear text: "WiFi Error" or "No Connection"
- Pulsing/flashing to draw attention
- No timeout - stays in error state until resolved

**Implementation Ideas**:
- Flashing red screen (250ms on/off)
- Large X or warning icon
- Alternating red/dark background

---

### 3. Event Recording (Short Press)

#### 3A. Button Press Detection
**Trigger**: User presses button momentarily
**Action**: Send timestamp to API
**Visual Feedback**:
- Immediate visual acknowledgment on press
- Animation during API call
- Clear success/failure indication

**Animation Sequence**:
1. **Press Feedback** (50-100ms):
   - Screen flash or ripple effect from center
   - Color change to acknowledge input

2. **API Call in Progress** (duration varies):
   - Loading animation (spinner, pulsing ring, or wave)
   - Text: "Recording..."

3. **Success State** (2-3 seconds):
   - Clear visual "decay" animation
   - Green checkmark or success icon
   - Expanding success ring that fades
   - Return to idle state

**Implementation Ideas**:
- **Press**: Quick white flash + expanding ring from center
- **Processing**: Rotating arc segments (like progress indicator)
- **Success**: Green expanding ring with fade-out using brightness modulation
- **Decay**: Gradually fade success indicator over 2-3 seconds using sine curve

#### 3B. API Call Failure
**State**: API request failed (network error, timeout, etc.)
**Visual**:
- **OBVIOUS ERROR**: Red/orange error indicator
- Clear failure icon (X or warning symbol)
- Text: "Failed - Retry?"
- Pulsing error state for 3-5 seconds
- Auto-return to idle state

**Implementation Ideas**:
- Red screen with pulsing X icon
- Expanding red ring (inverse of success)
- Shake effect (simulate by redrawing at offset positions)

---

### 4. Privacy Mode (5-Second Hold)

#### 4A. Long Press Detection
**Trigger**: User holds button for 5 seconds
**Action**: Send "discard last hour" signal to API
**Visual Feedback**:
- Progress indicator showing hold duration
- Clear visual distinction from short press

**Animation Sequence**:
1. **Hold Detection** (0-5 seconds):
   - Circular progress ring filling clockwise
   - Color progression (e.g., white → yellow → orange → red)
   - Text: "Hold to Clear"
   - Ring completes at 5 seconds

2. **API Call in Progress**:
   - Different animation from regular press (e.g., pulsing instead of spinning)
   - Text: "Clearing..."
   - Distinct color (orange/yellow theme)

3. **Success State** (2-3 seconds):
   - Orange/yellow success indicator
   - Different icon from regular press
   - Clear decay animation
   - Text: "Cleared" or "Privacy Mode"
   - Return to idle

**Implementation Ideas**:
- **Progress Ring**: Use `fillArc()` with angle from 0° to 360° over 5 seconds
- **Color Progression**: Interpolate color based on percentage (0-20%: white, 20-60%: yellow, 60-100%: orange)
- **Success**: Orange/yellow expanding ring with different pattern than event success

#### 4B. Privacy Mode Failure
**State**: Privacy API call failed
**Visual**: Similar to event failure but with orange/yellow color scheme

---

### 5. Debug Menu (10-Second Hold)

#### 5A. Entering Debug Menu
**Trigger**: User holds button for 10 seconds
**Visual**:
- Extended progress ring (continues from 5-second to 10-second mark)
- Color shift at 5 seconds to indicate entering debug mode territory
- Color progression: white → yellow → orange → purple/blue

**Animation**:
- Ring fills 0-180° for first 5 seconds (privacy mode)
- Ring continues 180-360° for next 5 seconds (debug mode)
- Color changes at 180° to purple/blue
- Text changes: "Hold to Clear" → "Entering Debug"

#### 5B. Debug Menu Display
**State**: User is in debug menu
**Layout**:
```
┌─────────────────────┐
│ [Selected Item]     │ ← Current menu item (highlighted)
│                     │
│ • Info Text         │ ← Details for current item
│ • More Info         │
│                     │
│ Click: Next         │ ← Instructions
│ Hold: Select/Exit   │
└─────────────────────┘
```

**Menu Items**:
1. WiFi Status (SSID, IP, signal strength)
2. API Status (last call time, success/failure count)
3. Uptime (device runtime)
4. Memory Info (free heap, etc.)
5. **Exit to Normal** ← Returns to idle state

**Navigation**:
- **Short Press**: Advance to next menu item (cycle through)
- **Long Press (2-3 seconds)**:
  - If on "Exit", return to normal operation
  - If on other items, toggle expanded/collapsed view (if applicable)

**Visual Feedback**:
- Selected item has background highlight
- Short press: Brief flash + item change
- Long press: Progress indicator for action confirmation

**Ambient Animation**:
- Small pulsing dot or border to show device is responsive
- Different from idle state to indicate debug mode

---

### 6. Continuous "Alive" Indicator

**Requirement**: Something on screen must move regularly to prove device hasn't crashed

**Implementation Ideas** (choose one per state):

1. **Idle State**:
   - Breathing background (brightness modulation)
   - Slowly rotating small icon or dot around perimeter
   - Pulsing status indicator
   - Subtle wave animation

2. **Processing States**:
   - Spinner/loading animation serves this purpose

3. **Error States**:
   - Pulsing/flashing serves this purpose

4. **Debug Menu**:
   - Small pulsing indicator in corner
   - Blinking cursor or dot

---

## Visual Design Patterns

### Color Scheme
- **Idle/Normal**: Green theme (ready state)
- **Processing**: Blue/cyan theme (working)
- **Success**: Green theme (confirmation)
- **Warning/Privacy**: Yellow/orange theme (caution)
- **Error**: Red theme (attention required)
- **Debug**: Purple/blue theme (special mode)

### Animation Timing
- **Button Press Feedback**: 50-100ms (immediate response)
- **API Processing**: Variable (show continuous animation)
- **Success Decay**: 2-3 seconds (fade out gradually)
- **Error Display**: 3-5 seconds (then auto-dismiss or require action)
- **Hold Detection**: 5 seconds (privacy), 10 seconds (debug)
- **Breathing/Ambient**: 2-4 second cycle (slow, calming)

### Animation Techniques
1. **Expanding Rings**: Use `fillArc()` with increasing radius
2. **Fading**: Modulate brightness over time using sine curves
3. **Spinners**: Rotating arc segments using angle updates
4. **Progress Rings**: `fillArc()` with angle from 0° to 360°
5. **Pulsing**: Sine wave modulation of brightness/size
6. **Color Transitions**: Interpolate RGB values over time

---

## Technical Implementation Notes

### Animation Framework
Based on research findings, implement using:

1. **M5GFX Drawing Functions**:
   - `fillArc(x, y, r0, r1, angle0, angle1, color)` - For rings, progress indicators
   - `fillCircle(x, y, r, color)` - For simple circles
   - `drawString(text, x, y)` - For text display
   - `fillScreen(color)` or `clear()` - For background updates

2. **Sprite-Based Animation** (for complex graphics):
   - Create `M5Canvas` sprites for pre-rendered elements
   - Use `pushSprite()` for fast display updates
   - Enable double-buffering to prevent flicker

3. **Timing Control**:
   - Use `millis()` for non-blocking animation timing
   - Define frame rates (e.g., 30fps = 33ms per frame)
   - Track state changes with timestamps

### Example Animation Pseudo-Code

```cpp
// Breathing effect (idle state)
unsigned long breathingPeriod = 3000; // 3 second cycle
float phase = (millis() % breathingPeriod) / (float)breathingPeriod;
float brightness = 0.5 + 0.5 * sin(phase * 2 * PI);
uint16_t color = dimColor(GREEN, brightness);

// Progress ring (hold detection)
unsigned long holdStart = millis();
unsigned long holdDuration = millis() - holdStart;
float progress = min(1.0, holdDuration / 5000.0); // 5 seconds
int angle = 360 * progress;
display.fillArc(64, 64, 50, 55, 0, angle, ORANGE);

// Expanding success ring (decay animation)
unsigned long animStart = millis();
unsigned long elapsed = millis() - animStart;
float t = elapsed / 2000.0; // 2 second animation
int radius = 10 + (50 * t);
float alpha = 1.0 - t;
uint16_t color = dimColor(GREEN, alpha);
display.drawCircle(64, 64, radius, color);
```

### Code Organization

Suggested file structure:
```
src/
├── main.cpp           # Main application logic
├── animations.h/cpp   # Animation helper functions
├── states.h/cpp       # State machine for UI states
├── api_client.h/cpp   # WiFi and API communication
└── config.h           # Constants and configuration
```

---

## Research Resources

### M5GFX Animation Examples
- **AnalogMeter Example**: Demonstrates smooth sine-wave animations with arc filling
  - Uses `fillArc()` with continuously updating angles
  - Updates every 35ms for smooth motion
  - GitHub: `m5stack/M5GFX/examples/Basic/AnalogMeter/`

- **TFT_ArcFill Example**: Shows rotating and expanding arc animations
  - Color transitions using rainbow() function
  - Demonstrates brightness modulation
  - GitHub: `m5stack/M5Stack/examples/Advanced/Display/TFT_ArcFill/`

### Breathing LED Technique
- PWM-based brightness control using sine waves
- Natural breathing pattern: `brightness = 0.5 + 0.5 * sin(phase * 2 * PI)`
- Resources: ThingPulse breathing LED algorithm

### Sprite Animation
- M5Canvas/LGFX_Sprite for pre-rendered frames
- Fast `pushSprite()` updates to main display
- Useful for complex multi-element animations
- Docs: docs.m5stack.com/en/arduino/m5gfx/m5gfx_sprite

### Progress Indicators
- Circular progress: `fillArc()` with incrementing end angle
- Can combine with color interpolation for visual progression
- Examples in M5Stack community forums (gauge projects)

---

## Next Steps

1. **Prototype Core Animations**:
   - Implement breathing background
   - Create rotating spinner
   - Test progress ring with hold detection

2. **Build State Machine**:
   - Define all states and transitions
   - Implement state-specific rendering

3. **WiFi Integration**:
   - Connect to WiFi with visual feedback
   - Handle connection errors

4. **API Client**:
   - Implement HTTP POST for event recording
   - Handle timeouts and errors gracefully

5. **Polish**:
   - Fine-tune animation timing
   - Optimize performance
   - Test all user flows

---

## Performance Considerations

- AtomS3 has ESP32-S3 @ 240MHz - plenty of power for smooth animations
- 128x128 display is small - full redraws are fast
- Use sprites for complex animations to reduce flicker
- Target 30fps (33ms per frame) for smooth motion
- Consider power consumption for always-on ambient animations

---

## Accessibility Notes

- Use high contrast for error states (red on black/white)
- Make success/failure visually distinct (not just color)
- Provide text labels in addition to icons
- Ensure animations are not too fast (avoid seizure triggers)
- Timeout error states to prevent stuck UI

---

*This specification is a living document and should be updated as development progresses.*
