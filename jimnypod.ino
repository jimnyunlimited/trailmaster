/**
 * Trailmaster OS - App Launcher, Inclinometer, & Photo Frame
 * Waveshare ESP32-S3-Touch-AMOLED-1.43
 */

#include <Arduino.h>
#include <FFat.h>
#include "lcd_bsp.h"
#include "FT3168.h"
#include "i2c_bsp.h"
#include "lvgl.h"
#include "jimnypod.h"
#include "Launcher.h"
#include "InclinometerApp.h"
#include "PhotoFrameApp.h"
#include "SettingsApp.h"
#include <math.h>
#include <vector>

// --- Global State Machine ---
AppState current_state = STATE_LAUNCHER;

void setup() {
    Serial.begin(115200);
    delay(1000);

    Serial.println("\n╔════════════════════════════════════════╗");
    Serial.println("║ TRAILMASTER OS - Multi App Environment  ║");
    Serial.println("╚════════════════════════════════════════╝\n");

    if (ESP.getPsramSize() == 0) {
        Serial.println("⚠ FATAL ERROR: PSRAM IS NOT DETECTED!");
        while(1) { delay(1000); }
    }

    I2C_master_Init(); 
    Touch_Init();
    lcd_lvgl_Init();

    // Init IMU
    inclinometer_setup_imu();

    // Photo Frame Setup (Includes PSRAM allocation and WiFi)
    photoframe_setup();

    // Mount FFat
    if (FFat.begin(true)) {
        Serial.printf("✓ FFat Mounted! Total space: %u KB\n", FFat.totalBytes() / 1024);
    }

    // Build Screens
    build_launcher_screen();
    build_inclinometer_screen();
    build_photoframe_screen();
    build_settings_screen();
    setup_brightness_overlay();

    // Start on Launcher
    switch_to_launcher();
}

void loop() {
    // Handle App Specific Logic
    inclinometer_loop_handler();
    photoframe_loop_handler();

    lv_timer_handler();
    delay(5);
}
