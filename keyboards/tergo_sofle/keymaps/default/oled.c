/* Copyright 2024 Tergo Teclados
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include QMK_KEYBOARD_H

#include "oled.h"
#include "art/moon_tergo.h"
#include "art/oled_layers.h"
#include "layers.h"

#define TERGO_ENABLE_ANIMATION

#ifdef OLED_ENABLE

const uint16_t TIME_BETWEEN_FRAMES_ANIMATION = OLED_UPDATE_INTERVAL; // Deve ser 50ms para teclados split por default
const uint16_t TIME_BETWEEN_LOOPING_ANIMATION = 7000;

uint8_t frame_count = 1;
uint16_t time_between_frames = 0;
uint16_t key_timer;
bool timer_initialized = false;
bool refresh_logo_right_away = false;
char oled_blip [TERGO_SCREEN_PIXELS] = {0};

extern bool is_oled_enabled;
extern bool print_layer_number_instead_of_text;

void suspend_power_down_user(void) {
    oled_off();
}

oled_rotation_t oled_init_user(oled_rotation_t rotation) {
    return OLED_ROTATION_270;
}

void render_logo(void) {
    //ToDo Tergo: adicionar animacao de cometa

    #ifndef TERGO_ENABLE_ANIMATION

    oled_write_raw_P(moon_tergo[0], sizeof(moon_tergo[0]));

    #else

    if (!timer_initialized) {
        key_timer = timer_read();
        timer_initialized = true;
    }

    if (frame_count == 0) {
        time_between_frames = TIME_BETWEEN_LOOPING_ANIMATION;
    }
    else {
        time_between_frames = TIME_BETWEEN_FRAMES_ANIMATION;
    }

    if ((timer_elapsed(key_timer) > time_between_frames) || (refresh_logo_right_away == true)) {
        refresh_logo_right_away = false;
        oled_write_raw_P(moon_tergo[frame_count], sizeof(moon_tergo[frame_count]));

        frame_count = (frame_count + 1) % TERGO_ANIMATION_NUM_OF_FRAMES;
        key_timer = timer_read();
    }

    #endif
}

#define PERCENTAGE_STR_SIZE 5

char percentage_str[PERCENTAGE_STR_SIZE];

void print_percentage_str(uint8_t value, uint8_t max_value) {
    uint8_t percentage = (value * 100) / max_value;

    snprintf(percentage_str, PERCENTAGE_STR_SIZE, "%3d%%", percentage);
    oled_write(percentage_str, false);
    //oled_write_P(PSTR("%"), false);
}

// Showed if you have RGB and when in _ADJUST layer
void render_adjust(uint8_t col, uint8_t line) {
    oled_set_cursor(col, line);

    oled_write_ln_P(PSTR("v1.06"), false);

    line += 2;
    oled_set_cursor(col, line);

    oled_write_P(PSTR(" RGB "), false);
    line += 2;
    oled_set_cursor(col, line);

    #ifndef RGB_MATRIX_ENABLE

    oled_write_P(PSTR(" No  "), false);

    #else

    bool    enabled = rgb_matrix_is_enabled();
    uint8_t mode    = rgb_matrix_get_mode();
    HSV     hsv     = rgb_matrix_get_hsv();
    uint8_t speed   = rgb_matrix_get_speed();

    if (!enabled) {
        oled_write_P(PSTR(" OFF "), false);
    }
    else {
        oled_write_P(PSTR(" ON  "), false);
    }

    line += 2;
    oled_set_cursor(col, line);

    oled_write_P(PSTR("B"), false);
    print_percentage_str(hsv.v, RGB_MATRIX_MAXIMUM_BRIGHTNESS);
    line += 2;
    oled_set_cursor(col, line);

    oled_write_P(PSTR("M"), false);
    oled_advance_char();
    oled_write(get_u8_str(mode, ' '), false);
    line += 2;
    oled_set_cursor(col, line);

    oled_write_P(PSTR("S"), false);
    print_percentage_str(hsv.s, 255);
    line += 2;
    oled_set_cursor(col, line);

    oled_write_P(PSTR("H"), false);
    oled_advance_char();
    oled_write(get_u8_str(hsv.h, ' '), false);
    line += 2;
    oled_set_cursor(col, line);

    oled_write_P(PSTR("P"), false);
    print_percentage_str(speed, 255);
    #endif
}

static void clear_layers(void) {
    memcpy(oled_blip, oled_layers_base, TERGO_SCREEN_PIXELS);
}

static void stack_layers(const unsigned char layer[TERGO_SCREEN_PIXELS]) {
    for (ssize_t i = 0; i<TERGO_SCREEN_PIXELS; i++) {
        oled_blip[i] |= layer[i];
    }
}

static void print_status(void) {
    clear_layers();

    enum sofle_layers layer = get_highest_layer(layer_state);
    switch (layer) {
        case _BASIC:
            break;
        case _SYMBOL:
            stack_layers(oled_layers_symbol);
            break;
        case _NAV:
            stack_layers(oled_layers_nav);
            break;
        case _NUMBER:
            stack_layers(oled_layers_num);
            break;
        case _LED:
            stack_layers(oled_layers_led);
            break;
        case _MEDIA:
            stack_layers(oled_layers_media);
            break;
        default:
            break;
    }

    led_t led_usb_state = host_keyboard_led_state();
    if (led_usb_state.caps_lock) {
        stack_layers(oled_layers_ind_caps);
    }
    if (led_usb_state.num_lock) {
        stack_layers(oled_layers_ind_num);
    }
    if (led_usb_state.scroll_lock) {
        stack_layers(oled_layers_ind_num);
    }

    uint8_t mods = get_mods();
    if (mods & MOD_MASK_CTRL) {
        stack_layers(oled_layers_ind_ctrl);
    }
    if (mods & MOD_MASK_SHIFT) {
        stack_layers(oled_layers_ind_shift);
    }
    if (mods & MOD_MASK_ALT) {
        stack_layers(oled_layers_ind_alt);
    }
    if (mods & MOD_MASK_GUI) {
        stack_layers(oled_layers_ind_gui);
    }
    oled_write_raw_P(oled_blip, TERGO_SCREEN_PIXELS);
}

bool oled_task_user(void) {

    if (is_keyboard_master()) {
        // master side forces OLEDs off if variable is false
        if (!is_oled_enabled) {
            oled_off();
            return false; // do nothing else
        }
        oled_on();
    }
    if(!is_oled_on()) {
        return false;
    }

    if (is_keyboard_master()) {
        print_status();
    }
    else {
        if (get_highest_layer(layer_state) == _LED) {
            oled_clear();
            render_adjust(0, 0);
            refresh_logo_right_away = true;
            return false;
        }
        render_logo();
    }
    return false;
}

#endif
