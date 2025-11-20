# Implementation Roadmap

Suggested order for implementing the Event Tracker Button project.

---

## Phase 1: Core Animation Framework ⚙️

**Goal**: Build reusable animation system before implementing features.

### 1.1 Animation Helper Library
- [ ] Create `animations.h/cpp` with utility functions
- [ ] Implement `dimColor(color, brightness)` function
- [ ] Implement `blendColor(color1, color2, t)` function
- [ ] Create breathing brightness calculator
- [ ] Create frame timing helper (non-blocking)

**Files to create**:
- `src/animations.h`
- `src/animations.cpp`

**Test**: Simple breathing background in main loop

---

### 1.2 Basic Shapes & Effects
- [ ] Function: `drawProgressRing(x, y, radius, progress, color)`
- [ ] Function: `drawSpinner(x, y, radius, angle, color)`
- [ ] Function: `flashScreen(color, duration)`
- [ ] Function: `drawExpandingRing(x, y, elapsed, duration, color)`

**Test**: Each function individually with button presses

---

### 1.3 State Machine Foundation
- [ ] Define `enum UIState` (BOOT, WIFI_CONNECT, IDLE, etc.)
- [ ] Create state transition logic
- [ ] Implement state rendering dispatcher
- [ ] Add state change timestamps

**Files to create**:
- `src/ui_state.h`
- `src/ui_state.cpp`

**Test**: Cycle through states with button presses

---

## Phase 2: Basic UI States 🎨

**Goal**: Implement core visual states without networking.

### 2.1 Boot Sequence
- [ ] Expanding circle animation (0-1.5s)
- [ ] Version display
- [ ] Transition to WiFi connect state

**Test**: Device shows boot animation on power-up

---

### 2.2 Idle State
- [ ] Breathing background implementation
- [ ] "Press to Record" text display
- [ ] Ambient alive indicator (pulsing dot or rotating element)

**Test**: Idle state animates continuously, looks good

---

### 2.3 Button Press Detection
- [ ] Detect short press (< 1 second)
- [ ] Detect 5-second hold (privacy mode)
- [ ] Detect 10-second hold (debug mode)
- [ ] Visual feedback during hold (progress ring)

**Test**: Button detection works, progress ring fills correctly

---

## Phase 3: Animation Refinement 🎬

**Goal**: Polish the visual feedback for button interactions.

### 3.1 Short Press Animation
- [ ] White flash on press (50ms)
- [ ] Mock "processing" animation (spinner)
- [ ] Success animation (expanding green ring)
- [ ] Failure animation (pulsing red X)
- [ ] Return to idle after 2-3 seconds

**Test**: Press button, see full animation cycle

---

### 3.2 Long Press Animations
- [ ] Privacy mode progress ring (0-5s)
  - Color progression: white → yellow → orange
- [ ] Debug mode progress ring (5-10s)
  - Color shift at 5s to purple
- [ ] Success feedback for each mode

**Test**: Hold button, watch ring fill with correct colors

---

## Phase 4: WiFi Integration 📡

**Goal**: Connect to WiFi with visual feedback.

### 4.1 WiFi Connection
- [ ] Read WiFi credentials from config (or hardcode for testing)
- [ ] Implement WiFi connection logic
- [ ] Show "Connecting..." spinner during connection
- [ ] Transition to idle on success
- [ ] Show error state on failure

**Files to create**:
- `src/wifi_manager.h`
- `src/wifi_manager.cpp`

**Test**: Device connects to WiFi, shows appropriate animations

---

### 4.2 WiFi Error Handling
- [ ] Implement obvious error display (red, pulsing)
- [ ] Auto-retry logic with visual feedback
- [ ] Manual retry option

**Test**: Disconnect WiFi, device shows error state

---

## Phase 5: API Integration 🌐

**Goal**: Implement actual API calls for event recording.

### 5.1 API Client Setup
- [ ] Define API endpoint configuration
- [ ] Implement HTTP POST for event timestamp
- [ ] Implement HTTP POST for privacy mode
- [ ] Add timeout handling
- [ ] Add error handling

**Files to create**:
- `src/api_client.h`
- `src/api_client.cpp`
- `src/config.h` (API URL, credentials)

**Test**: Manual API calls from serial console

---

### 5.2 Event Recording Flow
- [ ] Integrate short press → API call
- [ ] Show processing animation during API call
- [ ] Show success animation on HTTP 200
- [ ] Show failure animation on error
- [ ] Log events to serial for debugging

**Test**: Press button, event is recorded, visual feedback matches result

---

### 5.3 Privacy Mode Flow
- [ ] Integrate 5-second hold → privacy API call
- [ ] Use distinct animation/color scheme
- [ ] Show success/failure appropriately

**Test**: Hold button for 5s, privacy request sent, distinct visual feedback

---

## Phase 6: Debug Menu 🐛

**Goal**: Implement 10-second hold debug menu.

### 6.1 Menu Structure
- [ ] Define menu items (WiFi, API, Uptime, Memory, Exit)
- [ ] Implement menu navigation state machine
- [ ] Render current menu item
- [ ] Handle click (next) and hold (select/exit) inputs

**Files to create**:
- `src/debug_menu.h`
- `src/debug_menu.cpp`

**Test**: Enter debug menu, navigate with button

---

### 6.2 Menu Items Implementation
- [ ] WiFi Status (SSID, IP, signal strength)
- [ ] API Status (last call time, success/fail counts)
- [ ] Uptime display
- [ ] Memory info (free heap)
- [ ] Exit to normal operation

**Test**: Each menu item displays correct information

---

### 6.3 Debug Menu Visuals
- [ ] Highlight selected item
- [ ] Show "alive" indicator (pulsing dot)
- [ ] Smooth transitions between items

**Test**: Menu looks good, easy to navigate

---

## Phase 7: Polish & Optimization ✨

**Goal**: Final refinements and performance tuning.

### 7.1 Performance Optimization
- [ ] Profile animation frame rates
- [ ] Optimize sprite usage if needed
- [ ] Reduce unnecessary redraws
- [ ] Check memory usage

**Test**: Smooth animations, no lag or flicker

---

### 7.2 Edge Cases
- [ ] Handle button release during hold animations
- [ ] Handle WiFi disconnect during operation
- [ ] Handle API timeout gracefully
- [ ] Test rapid button presses

**Test**: Device handles all edge cases without crashing

---

### 7.3 Visual Polish
- [ ] Fine-tune animation timing
- [ ] Adjust colors for readability
- [ ] Ensure text is legible in all states
- [ ] Test ambient animations are noticeable but not distracting

**Test**: Subjective - does it look good?

---

### 7.4 Code Cleanup
- [ ] Add code comments
- [ ] Refactor duplicated code
- [ ] Organize includes
- [ ] Remove debug serial prints (or add debug flag)

**Test**: Code review, readability

---

## Phase 8: Testing & Validation ✅

**Goal**: Comprehensive testing of all features.

### 8.1 Feature Testing
- [ ] Boot sequence works every time
- [ ] WiFi connection reliable
- [ ] Event recording works (short press)
- [ ] Privacy mode works (5-second hold)
- [ ] Debug menu accessible (10-second hold)
- [ ] All animations smooth and correct

---

### 8.2 Long-Running Tests
- [ ] Device runs for 24+ hours without issues
- [ ] WiFi reconnects after router restart
- [ ] Memory doesn't leak over time
- [ ] Animations stay smooth over time

---

### 8.3 User Testing
- [ ] Someone unfamiliar with the device can use it
- [ ] Error states are obvious
- [ ] Success feedback is clear
- [ ] Hold durations feel natural

---

## Phase 9: Documentation 📚

**Goal**: Document the final implementation.

### 9.1 Code Documentation
- [ ] Add docstrings to all public functions
- [ ] Document state machine in comments
- [ ] Create inline comments for complex logic

---

### 9.2 User Documentation
- [ ] Create USER_GUIDE.md
  - How to use the device
  - What each animation means
  - Troubleshooting common issues

---

### 9.3 Developer Documentation
- [ ] Update CLAUDE.md with project-specific info
- [ ] Document API contract
- [ ] Add deployment instructions
- [ ] Document WiFi configuration process

---

## Estimated Timeline

| Phase | Description | Estimated Time |
|-------|-------------|----------------|
| 1 | Animation Framework | 4-6 hours |
| 2 | Basic UI States | 3-4 hours |
| 3 | Animation Refinement | 4-6 hours |
| 4 | WiFi Integration | 2-3 hours |
| 5 | API Integration | 3-4 hours |
| 6 | Debug Menu | 3-4 hours |
| 7 | Polish & Optimization | 4-6 hours |
| 8 | Testing | 2-4 hours |
| 9 | Documentation | 2-3 hours |
| **Total** | | **27-40 hours** |

*Times are estimates for an experienced developer. Adjust based on familiarity with M5Stack/ESP32.*

---

## Getting Started - Quick Wins 🚀

Want to start coding right away? Here are the quick wins to build momentum:

1. **Breathing Background** (30 min)
   - Implement sine wave brightness calculation
   - Apply to fillScreen() in loop
   - See immediate visual result

2. **Progress Ring** (1 hour)
   - Implement fillArc() wrapper
   - Test with button hold detection
   - Very satisfying to see it work!

3. **Spinner Animation** (1 hour)
   - Rotating arc segments
   - Can be reused for all "loading" states

4. **Flash Effect** (15 min)
   - Simplest button feedback
   - Feels great when it works

Start with these, and you'll have the building blocks for everything else!

---

## Dependency Checklist

Before starting implementation, ensure you have:

- [x] PlatformIO project initialized
- [x] M5AtomS3 library installed
- [x] M5Unified library installed
- [x] M5GFX library installed
- [x] FastLED library installed
- [x] WiFi credentials prepared
- [ ] API endpoint URL defined
- [ ] Test plan created

---

## Notes

- **Iterative Development**: Don't try to implement everything at once. Build incrementally.
- **Test Early, Test Often**: After each small feature, test it on hardware.
- **Version Control**: Use git to commit after each working feature.
- **Serial Debugging**: Add debug prints early, disable them later with a flag.
- **Have Fun**: Building visual feedback is satisfying - enjoy the process!

---

*This roadmap is a guide, not a strict plan. Adjust based on your discoveries and priorities.*
