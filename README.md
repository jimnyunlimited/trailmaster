# Trailmaster OS

An application-based environment for the **Waveshare ESP32-S3-Touch-AMOLED-1.43** board. This project mimics a mobile OS with a central launcher and modular apps for off-roading and digital display.

## 🚀 Hardware Overview
*   **MCU:** ESP32-S3 (Dual-core, 240MHz)
*   **Display:** 1.43" AMOLED (466x466 resolution)
*   **Touch:** Capacitive touch controller (FT3168)
*   **Sensors:** QMI8658 6-axis IMU (Accelerometer + Gyroscope)
*   **Storage:** FFat (Flash Fat) for image storage
*   **Memory:** PSRAM utilized for high-speed display buffering

---

## 🏗 System Architecture

The software is built on a **Modular Micro-App Architecture**. Each application is isolated into its own `.h` and `.cpp` files to ensure maintainability and scalability.

*   **`jimnypod.ino`**: The main orchestrator. It initializes hardware and triggers the setup/loop hooks for all modules.
*   **`jimnypod.h`**: The global communication bridge. It defines the `AppState` machine and shared navigation functions.
*   **`Launcher`**: A vertical scrolling menu with pill-shaped buttons.
*   **`Micro-Apps`**: Independent modules (`Inclinometer`, `PhotoFrame`, `Settings`) that handle their own UI building, logic, and callbacks.

---

## 📱 Application Guide

### 1. 📐 Inclinometer
A real-time attitude indicator for off-roading.
*   **Logic**: Uses a complementary filter to combine accelerometer and gyroscope data.
*   **UI**: Features a rotating ground line and a pitch "ladder" indicator.
*   **Short Tap**: Calibrates/Zeroes the sensor at the current position.
*   **Long Press**: Returns to the App Launcher.

### 2. 🖼 Photo Frame
A digital slideshow with over-the-air image syncing.
*   **WiFi Sync**: On launch, the device creates a SoftAP named `Trailmaster_Sync` (Password: `password123`).
*   **Web Portal**: Scan the QR code or go to `http://trailmaster.io` to access the built-in image cropper and uploader.
*   **Gestures**:
    *   **Swipe Left/Right**: Manually browse images.
    *   **Swipe Up**: Open the "Delete Photo" dialog.
    *   **Long Press**: Returns to the App Launcher.

### 3. ⚙ Settings
System-level configurations.
*   **Brightness**: Adjusts the AMOLED display brightness via a system-layer black overlay.
*   **Persistence**: Uses the `Preferences` library to save your brightness level even after power-off.
*   **Long Press**: Returns to the App Launcher.

### 4. 🎮 Rhino Jump
A persistent high-score game with precise physics.
*   **Gameplay**: Tap to jump over obstacles.
*   **Countdown**: 3-second lead-in before each round starts.
*   **Best Score**: Your all-time record is saved to flash.
*   **Long Press**: Returns to the App Launcher.

### 5. 🏎 OBD Gauge
Real-time vehicle diagnostics via WiFi OBDII adapter.
*   **Connectivity**: Connects to an ELM327 WiFi adapter (`WiFi_OBDII`).
*   **Data**: Displays Speed (km/h), RPM (with visual arc), Engine Temperature, and Battery Voltage.
*   **Long Press**: Returns to the App Launcher.

---

## 🛠 Developer Guide: Adding a New App

The modular structure makes it easy to add new features. Follow these steps:

### 1. Create the Module
Create two new files: `MyApp.h` and `MicroApp.cpp`.
*   In `.h`, define `build_myapp_screen()`, `switch_to_myapp()`, and optional `myapp_loop_handler()`.
*   In `.cpp`, define the `lv_obj_t * myapp_screen` and build the UI using LVGL.

### 2. Update the State Machine
In `jimnypod.h`, add your app to the `AppState` enum:
```cpp
enum AppState { STATE_LAUNCHER, ..., STATE_MYAPP };
void switch_to_myapp();
```

### 3. Add to the Launcher
In `Launcher.cpp`, add a button to the `build_launcher_screen()` function:
```cpp
create_launcher_btn(launcher_screen, LV_SYMBOL_DUMMY, "My New App", APP_ID_NEXT, 0xHEXCOLOR);
```
Update `app_btn_event_cb` to handle the new ID and call `switch_to_myapp()`.

### 4. Register in Main Sketch
In `jimnypod.ino`:
*   Include `MyApp.h`.
*   Call `build_myapp_screen()` in `setup()`.
*   Call `myapp_loop_handler()` in `loop()` if required.

---

## ⚙ Installation & Setup

1.  **Board Manager**: Install `esp32` by Espressif Systems (v2.0.11 or higher recommended).
2.  **Required Libraries**:
    *   **LVGL** (v8.3.x)
    *   **WiFi, WebServer, DNSServer** (Standard ESP32 libs)
    *   **FFat, Preferences** (Standard ESP32 libs)
3.  **Arduino IDE Board Settings**:
    *   **Board**: `ESP32S3 Dev Module`
    *   **USB CDC On Boot**: `Enabled`
    *   **Flash Size**: `16MB`
    *   **Partition Scheme**: `16M Flash (3MB APP / 9.9MB FATFS)`
    *   **PSRAM**: `OPI PSRAM`
    *   **Upload Speed**: `921600`
    *   **Erase All Flash Before Sketch Upload**: `Enabled` (Required for the first FFat initialization)
