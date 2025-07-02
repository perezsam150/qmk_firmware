#include QMK_KEYBOARD_H
#include "action_layer.h"

// Layers (Removed Unnecessary Mac Layer)
enum layers {
    WIN_BASE,
    WIN_FN
};

// Add n with tilde
enum unicode_names {
    N_TILDE_LOWER,
    N_TILDE_UPPER
};

const uint32_t PROGMEM unicode_map[] = {
    [N_TILDE_LOWER] = 0x00F1, // ñ
    [N_TILDE_UPPER] = 0x00D1  // Ñ
};

// Custom keycodes
enum custom_keycodes {
    RGB_TOGGLE = SAFE_RANGE,
    SOCD_W,
    SOCD_A,
    SOCD_S,
    SOCD_D,
    SNAP_TOGGLE
};

// SOCD flags
bool w_down = false;
bool a_down = false;
bool s_down = false;
bool d_down = false;
bool rgb_disabled = false;
bool snap_active = false;

#define KC_TASK LGUI(KC_TAB)
#define KC_FLXP LGUI(KC_E)
#define KC_HOME_COMBO LALT(LCTL(KC_A)) // For Taking Screenshots using Greenshot

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = { 
    [WIN_BASE] = LAYOUT_ansi_82(
        KC_ESC,   KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,    KC_F6,    KC_F7,    KC_F8,    KC_F9,    KC_F10,   KC_F11,     KC_F12,   KC_DEL,             KC_MUTE,
        KC_GRV,   KC_1,     KC_2,     KC_3,     KC_4,     KC_5,     KC_6,     KC_7,     KC_8,     KC_9,     KC_0,     KC_MINS,    KC_EQL,   KC_BSPC,            KC_PGUP,
        KC_TAB,   KC_Q,     SOCD_W,   KC_E,     KC_R,     KC_T,     KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,     KC_LBRC,    KC_RBRC,  KC_BSLS,            KC_PGDN,
        KC_CAPS,  SOCD_A,   SOCD_S,   SOCD_D,   KC_F,     KC_G,     KC_H,     KC_J,     KC_K,     KC_L,     KC_SCLN,  KC_QUOT,              KC_ENT,             KC_HOME_COMBO,
        KC_LSFT,            KC_Z,     KC_X,     KC_C,     KC_V,     KC_B,     KC_N,     KC_M,     KC_COMM,  KC_DOT,   KC_SLSH,              KC_RSFT,  KC_UP,
        KC_LCTL,  KC_LGUI,  KC_LALT,                                KC_SPC,                                 SNAP_TOGGLE, MO(WIN_FN), RGB_TOGGLE,  KC_LEFT,  KC_DOWN,  KC_RGHT
    ),

    [WIN_FN] = LAYOUT_ansi_82(
        _______,  KC_BRID,  KC_BRIU,  KC_TASK,  KC_FLXP,  _______,  _______,  KC_MPRV,  KC_MPLY,  KC_MNXT,  KC_MUTE,  KC_VOLD,    KC_VOLU,  _______,            _______,
        _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,    _______,  _______,            _______,
        _______,  _______,  _______,  _______,  NK_TOGG,  _______,  _______,  _______,  _______,  _______,  _______,  _______,    _______,  _______,            _______,
        _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,              _______,            _______,
        _______,            _______,  _______,  _______,  _______,  _______,  UP(N_TILDE_LOWER, N_TILDE_UPPER), _______, _______,  _______,  _______,              _______,  _______,
        _______,  _______,  _______,                                _______,                                _______,  _______,    _______,  _______,  _______,  _______
    )
};

#ifdef ENCODER_MAP_ENABLE
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
    [WIN_BASE] = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU)},
    [WIN_FN]   = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU)}
};
#endif

bool rgb_matrix_indicators_advanced_user(uint8_t min, uint8_t max) {
    if (host_keyboard_led_state().caps_lock) { // Lights up all alphanumeric keys when caps lock is toggled
    const uint8_t caps_leds[] = {
        45, // Caps Lock key itself
        // Row 1: `1234567890-=
        15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27,
        // Row 2: Q–P and brackets
        31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42,
        // Row 3: A–L and ;
        46, 47, 48, 49, 50, 51, 52, 53, 54, 55,
        // Row 4: Z–M, , . /
        60, 61, 62, 63, 64, 65, 66, 67, 68, 69
    };
    for (uint8_t i = 0; i < sizeof(caps_leds); i++) {
        if (caps_leds[i] >= min && caps_leds[i] < max) {
            rgb_matrix_set_color(caps_leds[i], 255, 255, 255);
        }
    }
}

    if (IS_LAYER_ON(WIN_FN)) { // Dynamically lights up any key with secondary function
    for (uint8_t row = 0; row < MATRIX_ROWS; ++row) {
        for (uint8_t col = 0; col < MATRIX_COLS; ++col) {
            uint8_t index = g_led_config.matrix_co[row][col];
            if (index >= min && index < max) {
                keypos_t key = {.row = row, .col = col};
                uint16_t base_kc = keymap_key_to_keycode(WIN_BASE, key);
                uint16_t fn_kc   = keymap_key_to_keycode(WIN_FN, key);

                if (fn_kc != KC_TRNS && fn_kc != base_kc) {
                    rgb_matrix_set_color(index, 255, 255, 255);
                } else {
                    rgb_matrix_set_color(index, 0, 0, 0);
                }
            }
        }
    }
}

	if (snap_active) { // Sets WASD backlight to RED if SNAP TAP is active
        const uint8_t wasd_leds[] = { 32, 46, 47, 48 };  // W, A, S, D
        for (uint8_t i = 0; i < sizeof(wasd_leds); i++) {
            if (wasd_leds[i] >= min && wasd_leds[i] < max) {
                // Red RGB 
                rgb_matrix_set_color(wasd_leds[i], 255, 0, 0);
            }
        }
    }

    return true;
}


bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    // Detect RCTRL to toggle RGB
    if (keycode == RGB_TOGGLE && record->event.pressed) {
        if (rgb_disabled) {
            rgblight_enable();
            rgb_disabled = false;
        } else {
            rgblight_disable();
            rgb_disabled = true;
        }
        return false;
    }
	

    // Toggle Snap Tap behavior
    if (keycode == SNAP_TOGGLE && record->event.pressed) {
        snap_active = !snap_active;
        return false;
    }

    if (keycode == SOCD_W) {
        if (record->event.pressed) {
            if (snap_active && s_down) unregister_code(KC_S);
            register_code(KC_W);
            w_down = true;
        } else {
            unregister_code(KC_W);
            w_down = false;
            if (snap_active && s_down) register_code(KC_S);
        }
        return false;
    }

    if (keycode == SOCD_A) {
        if (record->event.pressed) {
            if (snap_active && d_down) unregister_code(KC_D);
            register_code(KC_A);
            a_down = true;
        } else {
            unregister_code(KC_A);
            a_down = false;
            if (snap_active && d_down) register_code(KC_D);
        }
        return false;
    }

    if (keycode == SOCD_S) {
        if (record->event.pressed) {
            if (snap_active && w_down) unregister_code(KC_W);
            register_code(KC_S);
            s_down = true;
        } else {
            unregister_code(KC_S);
            s_down = false;
            if (snap_active && w_down) register_code(KC_W);
        }
        return false;
    }

    if (keycode == SOCD_D) {
        if (record->event.pressed) {
            if (snap_active && a_down) unregister_code(KC_A);
            register_code(KC_D);
            d_down = true;
        } else {
            unregister_code(KC_D);
            d_down = false;
            if (snap_active && a_down) register_code(KC_A);
        }
        return false;
    }

    return true;
}

