# External Links & Resources

Curated collection of external resources discovered during research for the Event Tracker Button project.

---

## Official M5Stack Documentation

### M5GFX Library
- **Main Documentation**: https://docs.m5stack.com/en/arduino/m5gfx/m5gfx
- **API Functions Reference**: https://docs.m5stack.com/en/arduino/m5gfx/m5gfx_functions
- **Sprite Management**: https://docs.m5stack.com/en/arduino/m5gfx/m5gfx_sprite
- **Canvas Documentation**: https://docs.m5stack.com/en/arduino/m5gfx/m5gfx_canvas

### M5AtomS3
- **Product Page**: https://docs.m5stack.com/en/core/AtomS3
- **Hardware Specs**: ESP32-S3, 128x128 LCD, WiFi/BLE
- **Quick Start Guide**: Included in official docs

---

## GitHub Repositories

### M5Stack Official Examples

#### M5GFX Library
- **Repository**: https://github.com/m5stack/M5GFX
- **AnalogMeter Example**: https://github.com/m5stack/M5GFX/blob/master/examples/Basic/AnalogMeter/AnalogMeter.ino
  - *Smooth sine-wave animations, arc filling*
- **All Examples**: https://github.com/m5stack/M5GFX/tree/master/examples

#### M5AtomS3 Library
- **Repository**: https://github.com/m5stack/M5AtomS3
- **Button Example**: https://github.com/m5stack/M5AtomS3/blob/main/examples/Basics/button/button.ino
- **All Examples**: https://github.com/m5stack/M5AtomS3/tree/main/examples

#### M5Stack (older library, still useful)
- **TFT_ArcFill Example**: https://github.com/m5stack/M5Stack/blob/master/examples/Advanced/Display/TFT_ArcFill/TFT_ArcFill.ino
  - *Rotating arcs, rainbow colors, brightness modulation*
- **Sprite Examples**: https://github.com/m5stack/M5Stack/tree/master/examples/Advanced/Display/Sprite

### Related Libraries
- **LovyanGFX** (basis for M5GFX): https://github.com/lovyan03/LovyanGFX
- **TFT_eSPI** (alternative graphics library): https://github.com/Bodmer/TFT_eSPI
- **FastLED** (LED effects, already in dependencies): https://github.com/FastLED/FastLED
- **JLed** (LED animation library): https://github.com/jandelgado/jled

---

## Community Resources

### M5Stack Community Forum
- **Main Forum**: https://community.m5stack.com/
- **Cool Animations Thread**: https://community.m5stack.com/topic/964/cool-animations-on-m5stack-by-hongkong-fans
- **GIF Display Discussion**: https://community.m5stack.com/topic/7378/how-to-display-an-animated-gif-on-m5stack-screens-using-arduino-c
- **Gauges Project**: https://community.m5stack.com/topic/3190/m5stack-gauges-for-boats

### Arduino Forums
- **TFT_eSPI Animated Dials**: https://forum.arduino.cc/t/tft_espi-new-example-for-animated-dials/643382
- **TFT Animations**: https://forum.arduino.cc/t/animations-on-a-tft-screen-mission-complete/410648

---

## Tutorial Articles & Blog Posts

### Animation Techniques
- **Breathing LEDs Algorithm**: https://thingpulse.com/breathing-leds-cracking-the-algorithm-behind-our-breathing-pattern/
  - *Mathematical approach to natural breathing patterns*
- **Basic TFT Animation Tutorial**: https://bytesnbits.co.uk/basic-animation-spi-touchscreen/
  - *Fundamentals of SPI TFT animations*

### ESP32-Specific
- **Breathing LED Tutorial (SaludPCB)**: https://saludpcb.com/breathing-led-smooth-fade-effect-tutorial/
- **ESP32 Starter Kit - Breathing LED**: https://docs.sunfounder.com/projects/esp32-starter-kit/en/latest/micropython/basic_projects/py_fade.html

### M5Stack Projects
- **M5GFX Media Player**: https://www.hackster.io/tommyho/m5gfx-media-player-jpg-png-bmp-gif-ab76a9
  - *Frame-based animation, image loading*
- **Analog Voltage Meter**: https://www.hackster.io/lukasmaximus89/analog-style-digital-voltage-meter-on-m5stack-22d9b4
  - *Gauge implementation example*

---

## Code Examples & Snippets

### GitHub Gists & Repositories
- **ESP32 TFT Progress Bar Clock**: https://github.com/nmai/esp32-tft-progress-bar-clock
- **ESP32 S3 Lilygo Animations**: https://github.com/doctorpartlow/esp32s3lilygoanimations
- **WS2812B LED Strip Effects**: https://github.com/jipixz/WS2812B-led-strip-effects
- **LED Effects Library**: https://github.com/yknivag/LEDEffectsLibrary

### Wokwi Simulator
- **TFT Display Animation Boilerplate**: https://wokwi.com/projects/404870001798046721
  - *Interactive simulation of TFT animations*

---

## Unofficial Guides

### M5Stack Lang-Ship Guide (Japanese, but useful)
- **Basic Drawing Functions**: https://m5stack.lang-ship.com/howto/m5gfx/basic/
- **M5GFX Overview**: https://m5stack.lang-ship.com/catalog/libraries/m5stack/other/m5gfx/

---

## Design Inspiration

### Animation Patterns
While not ESP32-specific, these provide good UX patterns:

- **Material Design - Motion**: https://m3.material.io/styles/motion/overview
  - *Google's motion design principles*
- **iOS Human Interface Guidelines - Animation**: https://developer.apple.com/design/human-interface-guidelines/motion
  - *Apple's animation best practices*

### Color Theory
- **RGB565 Color Picker**: http://www.barth-dev.de/online/rgb565-color-picker/
  - *Tool for converting colors to RGB565 format*

---

## Development Tools

### PlatformIO
- **Official Docs**: https://docs.platformio.org/
- **ESP32 Platform**: https://docs.platformio.org/en/latest/platforms/espressif32.html

### Libraries
- **M5AtomS3 Library Manager**: `m5stack/M5AtomS3@^1.0.0`
- **M5Unified Library Manager**: `m5stack/M5Unified@^0.1.16`
- **M5GFX Library Manager**: `m5stack/M5GFX@^0.1.16`
- **FastLED Library Manager**: `fastled/FastLED@^3.7.0`

---

## Video Resources

*Note: Video links not captured during research, but YouTube searches for "M5Stack animation" or "ESP32 TFT animation" yield good results*

Suggested search terms:
- "M5Stack AtomS3 examples"
- "M5GFX animation tutorial"
- "ESP32 TFT display animation"
- "Arduino circular progress bar"

---

## Additional Reading

### Technical Background
- **RGB565 Format**: 16-bit color (5 bits red, 6 bits green, 5 bits blue)
- **PWM for Brightness**: Pulse Width Modulation fundamentals
- **Non-Blocking Code**: Using millis() instead of delay()
- **State Machines**: Organizing complex UI flows

### Performance
- **ESP32-S3 Datasheet**: 240MHz dual-core, 512KB SRAM
- **Display Refresh Rates**: Target 30-60fps for smooth animation
- **Memory Management**: Sprite buffers, heap monitoring

---

## Notes on Source Quality

✅ **Highly Recommended**:
- All official M5Stack documentation and examples
- ThingPulse breathing LED algorithm
- M5Stack community forum discussions

⚠️ **Use with Caution**:
- Generic TFT examples may need adaptation for M5GFX
- Check dates on forum posts (M5GFX is relatively new)
- Test all code snippets before integrating

---

*This document will be updated as new resources are discovered during implementation.*
