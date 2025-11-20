#ifndef STRINGS_H
#define STRINGS_H

// UI Text Strings
// Edit these to customize the text displayed on the device

namespace Strings {
    // Default/Idle Screen
    inline constexpr const char* IDLE_LINE1 = "Press to";
    inline constexpr const char* IDLE_LINE2 = "Notice";

    // Button States
    inline constexpr const char* BUTTON_PRESSED = "Pressed";
    inline constexpr const char* BUTTON_RELEASED = "Saving...";
    inline constexpr const char* SAVED = "Saved";

    // Long Press States
    inline constexpr const char* PRIVACY_MODE = "PRIVACY MODE";
    inline constexpr const char* PRIVACY_MODE_LINE1 = "PRIVACY";
    inline constexpr const char* PRIVACY_MODE_LINE2 = "MODE";
    inline constexpr const char* PRIVACY_HOLD_INSTRUCTION = "Hold for privacy";
    inline constexpr const char* DEBUG_MODE = "DEBUG MENU";

    // WiFi States
    inline constexpr const char* WIFI_CONNECTING = "WiFi...";
    inline constexpr const char* WIFI_CONNECTED = "Connected!";
    inline constexpr const char* WIFI_FAILED = "WiFi Failed";
    inline constexpr const char* WIFI_NO_CONNECTION = "NO CONNECTION";
    inline constexpr const char* WIFI_RETRY = "Retrying...";

    // Serial Debug Messages
    inline constexpr const char* SERIAL_STARTUP = "Event Tracker Button - Ready";
    inline constexpr const char* SERIAL_BUTTON_PRESSED = "Button pressed";
    inline constexpr const char* SERIAL_BUTTON_RELEASED = "Button released - recording event";
    inline constexpr const char* SERIAL_PRIVACY_MODE = "Privacy mode activated (5s hold)";
    inline constexpr const char* SERIAL_PRIVACY_ACTIVATED = "Privacy webhook called";
    inline constexpr const char* SERIAL_DEBUG_MODE = "Debug mode activated (10s hold)";
    inline constexpr const char* SERIAL_DEBUG_ACTIVATED = "Debug webhook called";
    inline constexpr const char* SERIAL_TIMEOUT = "Timeout - returning to default screen";
    inline constexpr const char* SERIAL_WIFI_CONNECTING = "Connecting to WiFi...";
}

#endif
