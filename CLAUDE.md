# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

**Event Tracker Button** - An M5Stack AtomS3-based device for recording event timestamps via button press, with rich visual feedback and animations.

This is a PlatformIO-based embedded systems project targeting the M5Stack AtomS3 board (ESP32-S3 based). The project uses the Arduino framework.

### Key Features
- WiFi-connected API calls for event recording
- Single button interface with multiple interaction modes:
  - Short press: Record event timestamp
  - 5-second hold: Privacy mode (discard last hour)
  - 10-second hold: Debug menu
- Rich animations for all states (boot, loading, success, error)
- Breathing background and ambient "alive" indicators

## Build and Development Commands

### Building
```bash
pio run
```

### Upload to Device
```bash
pio run --target upload
```

### Clean Build
```bash
pio run --target clean
```

### Monitor Serial Output
```bash
pio device monitor
```

### Build and Upload in One Command
```bash
pio run --target upload && pio device monitor
```

### Testing
```bash
pio test
```

## Architecture

**Target Platform**: M5Stack AtomS3 (ESP32-S3 microcontroller)
- Platform: espressif32
- Framework: Arduino
- Board: m5stack-atoms3

**Project Structure**:
- `src/main.cpp` - Main application code with standard Arduino `setup()` and `loop()` functions
- `include/` - Header files for the project
- `lib/` - Project-specific libraries
- `test/` - Unit tests (run via PlatformIO test framework)
- `platformio.ini` - PlatformIO configuration file defining build environment and settings
- `design_resources/` - Complete design documentation (specs, mockups, resources)

**Development Environment**: This project uses PlatformIO IDE extension for VSCode. All build, upload, and testing operations should be performed through PlatformIO commands (`pio` CLI or `~/.platformio/penv/bin/platformio` on macOS).

## Design Documentation

**IMPORTANT**: Before implementing features, review the design documentation in `design_resources/`:

### Essential Documents
1. **[PROJECT_SPEC.md](design_resources/PROJECT_SPEC.md)** - Complete specification of all UI states, animations, and user flows
2. **[IMPLEMENTATION_ROADMAP.md](design_resources/IMPLEMENTATION_ROADMAP.md)** - Step-by-step implementation guide with phases and estimates
3. **[VISUAL_MOCKUPS.md](design_resources/VISUAL_MOCKUPS.md)** - ASCII mockups of all screens and animations
4. **[ANIMATION_RESOURCES.md](design_resources/ANIMATION_RESOURCES.md)** - Code examples and techniques for M5GFX animations
5. **[EXTERNAL_LINKS.md](design_resources/EXTERNAL_LINKS.md)** - Curated links to examples, docs, and tutorials

### Quick Reference
- **Color Scheme**: Green (idle), Blue (processing), Orange (warning), Red (error), Purple (debug)
- **Animation Timing**: 30fps target, breathing cycle 3s, success decay 2.5s
- **Display**: 128x128 pixels, RGB565 color, M5GFX library
- **Key Libraries**: M5AtomS3, M5Unified, M5GFX, FastLED

## Dependencies

Libraries are defined in `platformio.ini`:
- `m5stack/M5AtomS3@^1.0.0` - AtomS3 board support
- `m5stack/M5Unified@^0.1.16` - Unified M5Stack API
- `m5stack/M5GFX@^0.1.16` - Graphics library for animations
- `fastled/FastLED@^3.7.0` - LED control (required dependency)

## Animation Framework

The project uses M5GFX for all visual elements. Key functions:
- `fillArc(x, y, r0, r1, angle0, angle1, color)` - For progress rings and gauges
- `fillCircle(x, y, r, color)` - For simple circles
- `drawString(text, x, y)` - For text display
- `M5Canvas` sprites - For complex pre-rendered animations

**Pattern**: Use `millis()` for non-blocking animations, target 30fps (33ms per frame), use sprites to reduce flicker.
