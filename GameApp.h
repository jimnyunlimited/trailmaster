#ifndef GAME_APP_H
#define GAME_APP_H

#include <Arduino.h>
#include "lvgl.h"

// --- Game Globals ---
extern lv_obj_t * game_screen;

// --- Function Prototypes ---
void build_game_screen();
void switch_to_game();
void game_loop_handler();

#endif // GAME_APP_H
