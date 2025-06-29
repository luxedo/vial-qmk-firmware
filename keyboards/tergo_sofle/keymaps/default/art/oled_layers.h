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

#pragma once

#define TERGO_OLED_NUM_LAYERS 8
#define TERGO_OLED_NUM_LAYERS_FRAMES 1
#define TERGO_SCREEN_PIXELS 512

const unsigned char oled_layers_base [TERGO_SCREEN_PIXELS] PROGMEM;
const unsigned char oled_layers_ind_alt [TERGO_SCREEN_PIXELS] PROGMEM;
const unsigned char oled_layers_ind_caps [TERGO_SCREEN_PIXELS] PROGMEM;
const unsigned char oled_layers_ind_ctrl [TERGO_SCREEN_PIXELS] PROGMEM;
const unsigned char oled_layers_ind_fun [TERGO_SCREEN_PIXELS] PROGMEM;
const unsigned char oled_layers_ind_gui [TERGO_SCREEN_PIXELS] PROGMEM;
const unsigned char oled_layers_ind_num [TERGO_SCREEN_PIXELS] PROGMEM;
const unsigned char oled_layers_ind_scroll [TERGO_SCREEN_PIXELS] PROGMEM;
const unsigned char oled_layers_ind_shift [TERGO_SCREEN_PIXELS] PROGMEM;
const unsigned char oled_layers_led [TERGO_SCREEN_PIXELS] PROGMEM;
const unsigned char oled_layers_media [TERGO_SCREEN_PIXELS] PROGMEM;
const unsigned char oled_layers_nav [TERGO_SCREEN_PIXELS] PROGMEM;
const unsigned char oled_layers_num [TERGO_SCREEN_PIXELS] PROGMEM;
const unsigned char oled_layers_symbol [TERGO_SCREEN_PIXELS] PROGMEM;
