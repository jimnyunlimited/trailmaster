#include "Launcher.h"
#include "jimnypod.h"
#include "InclinometerApp.h"
#include "PhotoFrameApp.h"
#include "SettingsApp.h"
#include "GameApp.h"
#include "OBDApp.h"

// --- Launcher Globals ---
lv_obj_t * launcher_screen = NULL;

// --- Callbacks ---
static void app_btn_event_cb(lv_event_t * e) {
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_CLICKED) {
        int app_id = (int)(intptr_t)lv_event_get_user_data(e);
        if (app_id == 1) {
            switch_to_obd();
        } else if (app_id == 2) {
            switch_to_inclinometer();
        } else if (app_id == 3) {
            switch_to_photoframe();
        } else if (app_id == 4) {
            switch_to_game();
        } else if (app_id == 5) {
            switch_to_settings();
        }
    }
}

static lv_obj_t * create_launcher_btn(lv_obj_t * parent, const char * icon_str, const char * text, int app_id, uint32_t color_hex) {
    lv_obj_t * btn = lv_btn_create(parent);
    lv_obj_set_size(btn, 300, 90);
    lv_obj_set_style_bg_color(btn, lv_color_hex(0x1A1A1A), 0);
    lv_obj_set_style_bg_color(btn, lv_color_hex(0x333333), LV_STATE_PRESSED);
    lv_obj_set_style_radius(btn, 45, 0); // Pill shape
    lv_obj_set_style_border_width(btn, 2, 0);
    lv_obj_set_style_border_color(btn, lv_color_hex(0x333333), 0);
    lv_obj_add_event_cb(btn, app_btn_event_cb, LV_EVENT_CLICKED, (void*)(intptr_t)app_id);

    // App Icon
    lv_obj_t * icon = lv_label_create(btn);
    lv_label_set_text(icon, icon_str);
    lv_obj_set_style_text_color(icon, lv_color_hex(color_hex), 0);
    #if LV_FONT_MONTSERRAT_32
        lv_obj_set_style_text_font(icon, &lv_font_montserrat_32, 0);
    #else
        lv_obj_set_style_text_font(icon, &lv_font_montserrat_28, 0);
    #endif
    lv_obj_align(icon, LV_ALIGN_LEFT_MID, 30, 0);

    // App Name
    lv_obj_t * label = lv_label_create(btn);
    lv_label_set_text(label, text);
    lv_obj_set_style_text_color(label, lv_color_hex(0xFFFFFF), 0);
    #if LV_FONT_MONTSERRAT_24
        lv_obj_set_style_text_font(label, &lv_font_montserrat_24, 0);
    #else
        lv_obj_set_style_text_font(label, &lv_font_montserrat_20, 0);
    #endif
    lv_obj_align(label, LV_ALIGN_LEFT_MID, 80, 0);
    
    return btn;
}

void build_launcher_screen() {
    launcher_screen = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(launcher_screen, lv_color_hex(0x000000), 0); // True AMOLED Black
    
    // Enable scrolling and hide the scrollbar for a clean look
    lv_obj_add_flag(launcher_screen, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_scrollbar_mode(launcher_screen, LV_SCROLLBAR_MODE_OFF);

    // Set up a vertical flex layout
    lv_obj_set_flex_flow(launcher_screen, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(launcher_screen, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    
    // Add padding so the first and last items can be scrolled to the exact center of the round screen
    lv_obj_set_style_pad_top(launcher_screen, 188, 0);
    lv_obj_set_style_pad_bottom(launcher_screen, 188, 0);
    lv_obj_set_style_pad_row(launcher_screen, 20, 0); // 20px gap between buttons

    // --- Add Apps to Launcher (Reordered) ---
    create_launcher_btn(launcher_screen, LV_SYMBOL_DRIVE, "OBD Gauge", 1, 0xF1C40F);
    create_launcher_btn(launcher_screen, LV_SYMBOL_GPS, "Inclinometer", 2, 0xE67E22);
    create_launcher_btn(launcher_screen, LV_SYMBOL_IMAGE, "Photo Frame", 3, 0x2ECC71);
    create_launcher_btn(launcher_screen, LV_SYMBOL_KEYBOARD, "Rhino Jump", 4, 0x3498DB);
    create_launcher_btn(launcher_screen, LV_SYMBOL_SETTINGS, "Settings", 5, 0x9E9E9E);
}

// --- Screen Switchers ---
void switch_to_launcher() {
    // Stop all WiFi services when returning to launcher
    stop_photoframe_wifi();
    stop_obd_wifi();
    
    current_state = STATE_LAUNCHER;
    lv_scr_load_anim(launcher_screen, LV_SCR_LOAD_ANIM_FADE_ON, 300, 0, false);
}
