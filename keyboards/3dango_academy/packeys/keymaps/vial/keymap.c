/* Copyright 2026 3dango-academy
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

enum keymap_layer {
    BASE,
    LAYER1,
    LAYER2,
    LAYER3
};

/* bottom 3 keys */
static bool bottom[3] = {false, false, false};

/* timer and flags */
static uint16_t combo_timer = 0;
static bool short_combo_done = false;
static bool long_combo_done = false;

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [BASE] = LAYOUT(
        KC_Q, KC_W, KC_E,
        KC_A, KC_S, KC_D,
        KC_Z, KC_X, KC_C,
        TO(BASE), TO(LAYER1), TO(LAYER2)
    ),
    [LAYER1] = LAYOUT(
        KC_VOLD, KC_MUTE, KC_VOLU,
        KC_MPRV, KC_MEDIA_PLAY_PAUSE, KC_MNXT,
        KC_LSFT, KC_LCTL, KC_LGUI,
        TO(BASE), TO(LAYER1), TO(LAYER2)
    ),
    [LAYER2] = LAYOUT(
        KC_PSCR, KC_SCRL, KC_PAUS,
        KC_INS, KC_HOME, KC_PGUP,
        KC_DEL, KC_END, KC_PGDN,
        TO(BASE), TO(LAYER1), TO(LAYER2)
    ),
    [LAYER3] = LAYOUT(
        KC_BSPC, KC_UP, KC_ENTER,
        KC_LEFT, KC_DOWN, KC_RIGHT,
        KC_LSFT, KC_PGDN, KC_PGUP,
        TO(BASE), TO(LAYER1), TO(LAYER2)
    )
};

/* reset & layer switch */
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (record->event.key.row == 3 && record->event.key.col <= 2)
        bottom[record->event.key.col] = record->event.pressed;

    if (bottom[0] && bottom[1] && bottom[2]) {
        if (combo_timer == 0) combo_timer = timer_read();
    }

    if (!bottom[0] || !bottom[1] || !bottom[2]) {
        if (combo_timer && !short_combo_done && !long_combo_done && timer_elapsed(combo_timer) < 3000) {
            layer_move(LAYER3);
            short_combo_done = true;
        }

        combo_timer = 0;
        short_combo_done = false;
        long_combo_done = false;
    }

    return true;
}

void matrix_scan_user(void) {
    if (combo_timer && !long_combo_done && timer_elapsed(combo_timer) > 3000) {
        reset_keyboard();
        long_combo_done = true;
    }
}
