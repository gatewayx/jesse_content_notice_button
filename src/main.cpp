/**
 * Event Tracker Button - Sprite-Based Comet Animation
 *
 * Features:
 * - Solid green background with sprite-based alive indicator
 * - Responsive button detection
 * - Clean comet animation with proper fading
 */

#include "M5AtomS3.h"
#include "strings.h"
#include <WiFi.h>
#include <HTTPClient.h>

// Color definitions (RGB565)
#define COLOR_BLACK       0x0000
#define COLOR_GREEN       0x03E0
#define COLOR_LIME        0xE7E8  // #D7E845 converted to RGB565
#define COLOR_BLUE        0x001F
#define COLOR_ORANGE      0xFD20
#define COLOR_RED         0xF800
#define COLOR_PURPLE      0x781F
#define COLOR_WHITE       0xFFFF

// ============================================================================
// CONFIGURATION - Edit these values
// ============================================================================

// WiFi Networks - will try in order until one connects
struct WiFiNetwork {
    const char* ssid;
    const char* password;
};

WiFiNetwork wifiNetworks[] = {
    {"GatewayX", "G@t3w@yX"},
    {"bananaz", "tsibananaz"}
};
const int numWifiNetworks = sizeof(wifiNetworks) / sizeof(wifiNetworks[0]);

// API Endpoints
const char* EVENT_URL = "https://www.postb.in/1763658444638-1653359264601?auth=12342";
const char* PRIVACY_URL = "https://www.postb.in/1763658444638-1653359264601?auth=12342&mode=privacy";

// ============================================================================
// State Management
// ============================================================================

// UI State
unsigned long lastActivityTime = 0;
const unsigned long TIMEOUT_MS = 5000;
bool showingDefaultMessage = true;

// Button State
unsigned long buttonPressStartTime = 0;
bool buttonCurrentlyPressed = false;
enum PressType { NONE, SHORT_PRESS, PRIVACY_MODE, DEBUG_MODE };
PressType currentPressType = NONE;

// WiFi State
bool wifiConnected = false;

// Comet animation
float aliveAngle = 0.0;
M5Canvas cometSprite(&AtomS3.Display);

// Generic color blending function
// opacity: 0.0 = fromColor, 1.0 = toColor
uint16_t blendColors(uint16_t fromColor, uint16_t toColor, float opacity) {
    // Extract RGB565 components from fromColor
    int fromR = (fromColor >> 11) & 0x1F;
    int fromG = (fromColor >> 5) & 0x3F;
    int fromB = fromColor & 0x1F;

    // Extract RGB565 components from toColor
    int toR = (toColor >> 11) & 0x1F;
    int toG = (toColor >> 5) & 0x3F;
    int toB = toColor & 0x1F;

    // Blend
    int r = fromR + (int)((toR - fromR) * opacity);
    int g = fromG + (int)((toG - fromG) * opacity);
    int b = fromB + (int)((toB - fromB) * opacity);

    // Combine back to RGB565
    return (r << 11) | (g << 5) | b;
}

void createCometSprite() {
    // Create sprite: 5 circles in a row with fading
    const int circleRadius = 2;
    const int circleSpacing = 5;
    const int numCircles = 5;
    const int spriteWidth = numCircles * circleSpacing + circleRadius * 2;
    const int spriteHeight = circleRadius * 2 + 2;

    cometSprite.createSprite(spriteWidth, spriteHeight);
    cometSprite.fillSprite(COLOR_LIME);  // Background matches idle screen

    // Draw 5 circles fading FROM orange TO white (visible on lime background)
    for (int i = 0; i < numCircles; i++) {
        // i=0 should be orange (tail), i=4 should be white (head)
        float opacity = (i / (float)numCircles);
        uint16_t color = blendColors(COLOR_ORANGE, COLOR_WHITE, opacity);

        int x = i * circleSpacing + circleRadius;
        int y = spriteHeight / 2;

        cometSprite.fillCircle(x, y, circleRadius, color);
    }
}

void drawAliveIndicator() {
    const int orbitRadius = 10;
    int centerX = AtomS3.Display.width() - 20;
    int centerY = 20;

    // Calculate position on orbit
    int x = centerX + (int)(orbitRadius * cos(aliveAngle));
    int y = centerY + (int)(orbitRadius * sin(aliveAngle));

    // Push sprite with rotation to follow the orbit tangent
    float rotationAngle = aliveAngle + PI / 2;  // Tangent to circle
    cometSprite.pushRotateZoom(x, y, rotationAngle * 180.0 / PI, 1.0, 1.0, COLOR_LIME);

    aliveAngle += 0.05;
    if (aliveAngle > TWO_PI) aliveAngle = 0.0;
}

void showDefaultScreen() {
    AtomS3.Display.fillScreen(COLOR_LIME);

    // Draw text
    AtomS3.Display.setTextColor(COLOR_BLACK);
    AtomS3.Display.drawString(Strings::IDLE_LINE1,
                              AtomS3.Display.width() / 2,
                              AtomS3.Display.height() / 2 - 12);
    AtomS3.Display.drawString(Strings::IDLE_LINE2,
                              AtomS3.Display.width() / 2,
                              AtomS3.Display.height() / 2 + 12);
}

void showDebugScreen() {
    AtomS3.Display.fillScreen(COLOR_PURPLE);
    AtomS3.Display.setTextColor(COLOR_WHITE);

    // Use smaller, more readable font for debug info
    AtomS3.Display.setFont(&fonts::FreeSans9pt7b);
    AtomS3.Display.setTextDatum(middle_center);

    // Title
    AtomS3.Display.drawString("Debug",
                              AtomS3.Display.width() / 2,
                              15);

    // WiFi SSID
    String ssid = WiFi.SSID();
    if (ssid.length() == 0) ssid = "N/A";
    AtomS3.Display.drawString("SSID:",
                              AtomS3.Display.width() / 2,
                              45);
    AtomS3.Display.drawString(ssid.c_str(),
                              AtomS3.Display.width() / 2,
                              65);

    // RSSI (signal strength)
    String rssi = String(WiFi.RSSI()) + " dBm";
    if (!wifiConnected) rssi = "N/A";
    AtomS3.Display.drawString("RSSI:",
                              AtomS3.Display.width() / 2,
                              90);
    AtomS3.Display.drawString(rssi.c_str(),
                              AtomS3.Display.width() / 2,
                              110);

    // Reset to original font
    AtomS3.Display.setFont(&fonts::Orbitron_Light_24);
}

// ============================================================================
// WiFi Connection
// ============================================================================

void connectToWiFi() {
    // LOOP FOREVER until we get a connection
    // NEVER proceed without WiFi - the user MUST know if connection failed
    while (!wifiConnected) {
        AtomS3.Display.fillScreen(COLOR_BLUE);
        AtomS3.Display.setTextColor(COLOR_WHITE);
        AtomS3.Display.drawString(Strings::WIFI_CONNECTING,
                                  AtomS3.Display.width() / 2,
                                  AtomS3.Display.height() / 2);
        Serial.println(Strings::SERIAL_WIFI_CONNECTING);

        // Try each network in order
        for (int i = 0; i < numWifiNetworks; i++) {
            Serial.print("Trying network: ");
            Serial.println(wifiNetworks[i].ssid);

            WiFi.begin(wifiNetworks[i].ssid, wifiNetworks[i].password);

            // Wait up to 10 seconds for connection
            int attempts = 0;
            while (WiFi.status() != WL_CONNECTED && attempts < 20) {
                delay(500);
                Serial.print(".");

                // Visual feedback - animate dots
                int numDots = (attempts % 3) + 1;
                String dots = "";
                for (int d = 0; d < numDots; d++) dots += ".";

                AtomS3.Display.fillScreen(COLOR_BLUE);
                AtomS3.Display.drawString(Strings::WIFI_CONNECTING,
                                          AtomS3.Display.width() / 2,
                                          AtomS3.Display.height() / 2 - 12);
                AtomS3.Display.drawString(dots.c_str(),
                                          AtomS3.Display.width() / 2,
                                          AtomS3.Display.height() / 2 + 12);

                attempts++;
            }

            if (WiFi.status() == WL_CONNECTED) {
                wifiConnected = true;
                Serial.println("\nWiFi connected!");
                Serial.print("IP address: ");
                Serial.println(WiFi.localIP());

                // Success screen
                AtomS3.Display.fillScreen(COLOR_GREEN);
                AtomS3.Display.setTextColor(COLOR_WHITE);
                AtomS3.Display.drawString(Strings::WIFI_CONNECTED,
                                          AtomS3.Display.width() / 2,
                                          AtomS3.Display.height() / 2);
                delay(1500);
                return;  // SUCCESS - exit the function
            }

            Serial.println("\nFailed to connect to this network");
            WiFi.disconnect();
            delay(500);
        }

        // All networks failed - show PERSISTENT error screen
        Serial.println("FAILED to connect to any WiFi network - RETRYING");

        // Show error screen for 5 seconds before retry
        AtomS3.Display.fillScreen(COLOR_RED);
        AtomS3.Display.setTextColor(COLOR_WHITE);
        AtomS3.Display.drawString(Strings::WIFI_FAILED,
                                  AtomS3.Display.width() / 2,
                                  AtomS3.Display.height() / 2 - 20);
        AtomS3.Display.drawString(Strings::WIFI_NO_CONNECTION,
                                  AtomS3.Display.width() / 2,
                                  AtomS3.Display.height() / 2);
        AtomS3.Display.drawString(Strings::WIFI_RETRY,
                                  AtomS3.Display.width() / 2,
                                  AtomS3.Display.height() / 2 + 20);

        // Wait 5 seconds before retrying
        for (int i = 0; i < 50; i++) {
            delay(100);
            // Allow user to see the error
        }

        // Loop will retry automatically - NEVER exit without connection
    }
}

// ============================================================================
// HTTP Requests
// ============================================================================

void makeHTTPRequest(const char* url) {
    if (!wifiConnected) {
        Serial.println("WiFi not connected, skipping HTTP request");
        return;
    }

    HTTPClient http;
    http.begin(url);

    Serial.print("Making GET request to: ");
    Serial.println(url);

    int httpResponseCode = http.GET();

    if (httpResponseCode > 0) {
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
        String payload = http.getString();
        Serial.println(payload);
    } else {
        Serial.print("Error on HTTP request: ");
        Serial.println(httpResponseCode);
    }

    http.end();
}

void setup() {
    Serial.begin(115200);
    delay(100);

    // Initialize M5AtomS3
    auto cfg = M5.config();
    AtomS3.begin(cfg);

    // Configure display
    AtomS3.Display.setRotation(2);  // Flip 180 degrees
    AtomS3.Display.setTextColor(COLOR_WHITE);
    AtomS3.Display.setTextDatum(middle_center);
    AtomS3.Display.setFont(&fonts::Orbitron_Light_24);
    AtomS3.Display.setTextSize(1);

    // Create the comet sprite once
    createCometSprite();

    Serial.println(Strings::SERIAL_STARTUP);

    // Connect to WiFi
    connectToWiFi();

    // Show initial screen
    showDefaultScreen();
}

void loop() {
    // ALWAYS update button state first
    AtomS3.update();

    // ========================================================================
    // BUTTON PRESS DETECTION
    // ========================================================================

    // Start tracking button press
    if (AtomS3.BtnA.wasPressed()) {
        buttonCurrentlyPressed = true;
        buttonPressStartTime = millis();
        currentPressType = NONE;
        showingDefaultMessage = false;

        AtomS3.Display.fillScreen(COLOR_BLUE);
        AtomS3.Display.setTextColor(COLOR_WHITE);
        AtomS3.Display.drawString(Strings::BUTTON_PRESSED,
                                  AtomS3.Display.width() / 2,
                                  AtomS3.Display.height() / 2);
        Serial.println(Strings::SERIAL_BUTTON_PRESSED);
        lastActivityTime = millis();
    }

    // Monitor hold duration while button is pressed
    if (buttonCurrentlyPressed && AtomS3.BtnA.isPressed()) {
        unsigned long pressDuration = millis() - buttonPressStartTime;

        // Check for 10-second hold (Debug Mode)
        if (pressDuration >= 10000 && currentPressType != DEBUG_MODE) {
            currentPressType = DEBUG_MODE;
            showDebugScreen();
            Serial.println(Strings::SERIAL_DEBUG_MODE);
        }
        // Check for 3-second hold (Privacy Mode)
        else if (pressDuration >= 3000 && currentPressType != PRIVACY_MODE && currentPressType != DEBUG_MODE) {
            currentPressType = PRIVACY_MODE;
            AtomS3.Display.fillScreen(COLOR_RED);
            AtomS3.Display.setTextColor(COLOR_WHITE);
            AtomS3.Display.setTextSize(0.8);
            AtomS3.Display.drawString(Strings::PRIVACY_MODE_LINE1,
                                      AtomS3.Display.width() / 2,
                                      20);
            AtomS3.Display.drawString(Strings::PRIVACY_MODE_LINE2,
                                      AtomS3.Display.width() / 2,
                                      40);
            AtomS3.Display.setTextSize(1);
            Serial.println(Strings::SERIAL_PRIVACY_MODE);
        }

        // Visual feedback: show progress ring at TOP of screen for holds < 3 seconds
        if (pressDuration < 3000) {
            // Show instruction text at top
            AtomS3.Display.setTextSize(0.5);
            AtomS3.Display.drawString(Strings::PRIVACY_HOLD_INSTRUCTION,
                                      AtomS3.Display.width() / 2,
                                      10);
            AtomS3.Display.setTextSize(1);

            // Draw a small progress circle at top that fills over 3 seconds
            int arcAngle = map(pressDuration, 0, 3000, 0, 360);
            int centerX = AtomS3.Display.width() / 2;
            int centerY = 35;  // Near top of screen
            AtomS3.Display.drawArc(centerX, centerY, 12, 15, 0, arcAngle, COLOR_ORANGE);
        }
    }

    // ========================================================================
    // BUTTON RELEASE HANDLING
    // ========================================================================

    if (AtomS3.BtnA.wasReleased() && buttonCurrentlyPressed) {
        buttonCurrentlyPressed = false;
        unsigned long pressDuration = millis() - buttonPressStartTime;

        // Determine final press type if not already set
        if (currentPressType == NONE) {
            currentPressType = SHORT_PRESS;
        }

        // Handle based on press type
        switch (currentPressType) {
            case SHORT_PRESS:
                // Normal event recording
                AtomS3.Display.fillScreen(COLOR_ORANGE);
                AtomS3.Display.setTextColor(COLOR_WHITE);
                AtomS3.Display.drawString(Strings::BUTTON_RELEASED,
                                          AtomS3.Display.width() / 2,
                                          AtomS3.Display.height() / 2);
                Serial.println(Strings::SERIAL_BUTTON_RELEASED);

                // Make HTTP request
                makeHTTPRequest(EVENT_URL);

                // Show "Saved" confirmation
                AtomS3.Display.fillScreen(COLOR_GREEN);
                AtomS3.Display.setTextColor(COLOR_WHITE);
                AtomS3.Display.drawString(Strings::SAVED,
                                          AtomS3.Display.width() / 2,
                                          AtomS3.Display.height() / 2);
                delay(800);  // Show "Saved" for 800ms
                break;

            case PRIVACY_MODE:
                // Privacy mode webhook
                Serial.println(Strings::SERIAL_PRIVACY_ACTIVATED);
                makeHTTPRequest(PRIVACY_URL);
                delay(1000);
                break;

            case DEBUG_MODE:
                // Debug mode - keep showing until user presses button again
                // Don't clear or timeout the debug screen
                Serial.println(Strings::SERIAL_DEBUG_ACTIVATED);
                // Wait for next button press to exit
                while (!AtomS3.BtnA.wasPressed()) {
                    AtomS3.update();
                    delay(10);
                }
                break;

            default:
                break;
        }

        lastActivityTime = millis();
        currentPressType = NONE;
    }

    // ========================================================================
    // TIMEOUT AND IDLE STATE
    // ========================================================================

    // Check for timeout - return to default screen
    if (!showingDefaultMessage && !buttonCurrentlyPressed &&
        (millis() - lastActivityTime >= TIMEOUT_MS)) {
        showingDefaultMessage = true;
        Serial.println(Strings::SERIAL_TIMEOUT);
        showDefaultScreen();
    }

    // Update alive indicator when in idle state
    if (showingDefaultMessage) {
        static unsigned long lastUpdate = 0;
        if (millis() - lastUpdate >= 50) {  // Update at ~20fps
            drawAliveIndicator();
            lastUpdate = millis();
        }
    }
}
