#ifndef LAUNCHER_H
#define LAUNCHER_H

#include <Arduino.h>
#include "lvgl.h"

// --- Launcher Globals ---
extern lv_obj_t * launcher_screen;

// --- Function Prototypes ---
void build_launcher_screen();
void switch_to_launcher();

#endif // LAUNCHER_H
