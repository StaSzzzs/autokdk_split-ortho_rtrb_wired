#include QMK_KEYBOARD_H
#include "eeconfig.h"

#define DEFAULT_CPI 1500
#define SCROLL_DIVISOR_DEFAULT 4
#define SCROLL_DIVISOR_MIN 1
#define SCROLL_DIVISOR_MAX 16
#define SCROLL_REPORT_INTERVAL_MS 8
#define SCROLL_HORIZONTAL_DEADZONE 2
#define SCROLL_HORIZONTAL_DEADZONE_BYPASS_MS 500
#define AML_TIMEOUT_MIN 100
#define AML_TIMEOUT_MAX 1000
#define AML_TIMEOUT_QUANTUM 50

enum custom_keycodes {
    AML_TO = QK_KB_10,
    AML_I50 = QK_KB_11,
    AML_D50 = QK_KB_12,
    CK_SCROLL_SLOWER = SAFE_RANGE,
    CK_SCROLL_FASTER,
};

static uint8_t scroll_divisor = SCROLL_DIVISOR_DEFAULT;
static int32_t scroll_x_accumulator;
static int32_t scroll_y_accumulator;
static uint32_t scroll_x_deadzone_bypass_timer;
static bool scroll_x_deadzone_bypass_active;
static uint32_t scroll_report_timer;
static bool scroll_report_timer_active;

static mouse_hv_report_t clamp_scroll_delta(int32_t delta) {
    if (delta > MOUSE_REPORT_HV_MAX) {
        return MOUSE_REPORT_HV_MAX;
    }
    if (delta < MOUSE_REPORT_HV_MIN) {
        return MOUSE_REPORT_HV_MIN;
    }
    return (mouse_hv_report_t)delta;
}

void keyboard_post_init_user(void) {
    uint32_t stored_scroll_divisor = eeconfig_read_user();
    if (stored_scroll_divisor >= SCROLL_DIVISOR_MIN &&
        stored_scroll_divisor <= SCROLL_DIVISOR_MAX) {
        scroll_divisor = stored_scroll_divisor;
    }

    set_auto_mouse_enable(true);
    pointing_device_set_cpi(DEFAULT_CPI);
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (!record->event.pressed) {
        return true;
    }

    if (IS_LAYER_ON(AUTO_MOUSE_DEFAULT_LAYER)) {
        switch (keycode) {
            case MS_BTN1:
            case MS_BTN2:
            case MS_BTN3:
            case MS_BTN4:
            case MS_BTN5:
                break;
            default:
                auto_mouse_reset_trigger(true);
                break;
        }
    }

    switch (keycode) {
        case AML_TO:
            set_auto_mouse_enable(!get_auto_mouse_enable());
            return false;
        case AML_I50: {
            uint16_t timeout = get_auto_mouse_timeout() + AML_TIMEOUT_QUANTUM;
            set_auto_mouse_timeout(MIN(timeout, AML_TIMEOUT_MAX));
            return false;
        }
        case AML_D50: {
            uint16_t timeout = get_auto_mouse_timeout() - AML_TIMEOUT_QUANTUM;
            set_auto_mouse_timeout(MAX(timeout, AML_TIMEOUT_MIN));
            return false;
        }
        case CK_SCROLL_SLOWER:
            if (scroll_divisor < SCROLL_DIVISOR_MAX) {
                scroll_divisor++;
                eeconfig_update_user(scroll_divisor);
                scroll_x_accumulator = 0;
                scroll_y_accumulator = 0;
            }
            return false;
        case CK_SCROLL_FASTER:
            if (scroll_divisor > SCROLL_DIVISOR_MIN) {
                scroll_divisor--;
                eeconfig_update_user(scroll_divisor);
                scroll_x_accumulator = 0;
                scroll_y_accumulator = 0;
            }
            return false;
    }

    return true;
}

report_mouse_t pointing_device_task_user(report_mouse_t mouse_report) {
    if (IS_LAYER_ON(3)) {
        int8_t x = mouse_report.x;
        int8_t y = mouse_report.y;

        if (x <= -SCROLL_HORIZONTAL_DEADZONE ||
            x >= SCROLL_HORIZONTAL_DEADZONE) {
            scroll_x_deadzone_bypass_timer = timer_read32();
            scroll_x_deadzone_bypass_active = true;
        }

        if (scroll_x_deadzone_bypass_active &&
            timer_elapsed32(scroll_x_deadzone_bypass_timer) <
                SCROLL_HORIZONTAL_DEADZONE_BYPASS_MS) {
            scroll_x_accumulator +=
                x * POINTING_DEVICE_HIRES_SCROLL_MULTIPLIER;
        } else {
            scroll_x_deadzone_bypass_active = false;
        }
        scroll_y_accumulator -=
            y * POINTING_DEVICE_HIRES_SCROLL_MULTIPLIER;

        mouse_report.h = 0;
        mouse_report.v = 0;
        if (!scroll_report_timer_active ||
            timer_elapsed32(scroll_report_timer) >=
                SCROLL_REPORT_INTERVAL_MS) {
            mouse_report.h =
                clamp_scroll_delta(scroll_x_accumulator / scroll_divisor);
            mouse_report.v =
                clamp_scroll_delta(scroll_y_accumulator / scroll_divisor);
            scroll_x_accumulator -= mouse_report.h * scroll_divisor;
            scroll_y_accumulator -= mouse_report.v * scroll_divisor;

            if (mouse_report.h != 0 || mouse_report.v != 0) {
                scroll_report_timer = timer_read32();
                scroll_report_timer_active = true;
            }
        }
        mouse_report.x = 0;
        mouse_report.y = 0;
    }

    return mouse_report;
}

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT(
        KC_ESC, KC_1, KC_2, KC_3, KC_4, KC_5,
        KC_TAB, KC_Q, KC_W, KC_E, KC_R, KC_T,
        LT(3, KC_GRAVE), KC_A, KC_S, KC_D, KC_F, KC_G,
        KC_LSFT, KC_Z, KC_X, KC_C, KC_V, KC_B,
        KC_LBRC, KC_LCTL, MO(2), KC_LGUI, KC_LALT, MO(3),
        KC_SPACE, QK_MACRO_0, KC_6, KC_7, KC_8, KC_9,
        KC_0, KC_MINUS, KC_Y, KC_U, KC_I, LT(3, KC_O),
        KC_P, KC_QUOTE, KC_H, KC_J, KC_K, LT(2, KC_L),
        LT(1, KC_SCLN), KC_ENTER, KC_RBRC, KC_N, KC_M, KC_COMMA,
        KC_DOT, KC_SLASH, KC_INTERNATIONAL_1, KC_BSPC, KC_DEL, KC_LBRC,
        KC_EQUAL
    ),
    [1] = LAYOUT(
        KC_TRNS, KC_F1, KC_F2, KC_F3, KC_F4, KC_F5,
        LGUI(KC_TAB), KC_TRNS, KC_HOME, LGUI(KC_UP), KC_END, KC_PGUP,
        KC_TRNS, KC_TRNS, LGUI(KC_LEFT), LGUI(KC_DOWN), LGUI(KC_RIGHT), KC_PGDN,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        QK_BOOT, CK_SCROLL_SLOWER, CK_SCROLL_FASTER, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, QK_MACRO_1, KC_F6, KC_F7, KC_F8, KC_F9,
        KC_F10, KC_F11, KC_TRNS, KC_TRNS, (QK_LCTL | QK_LGUI | KC_LEFT), SGUI(KC_LEFT),
        KC_F12, KC_TRNS, KC_TRNS, SGUI(KC_LEFT), (QK_LCTL | QK_LGUI | KC_RIGHT), SGUI(KC_RIGHT),
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS
    ),
    [2] = LAYOUT(
        KC_TRNS, KC_F1, KC_F2, KC_F3, KC_F4, KC_F5,
        KC_TAB, KC_TRNS, KC_HOME, KC_UP, KC_END, KC_PGUP,
        KC_TRNS, KC_TRNS, KC_LEFT, KC_DOWN, KC_RIGHT, KC_PGDN,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, QK_MACRO_3,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, QK_MACRO_1, KC_F6, KC_F7, KC_F8, KC_F9,
        KC_F10, KC_F11, KC_TRNS, KC_TRNS, MS_BTN3, KC_TRNS,
        KC_F12, KC_TRNS, MS_BTN4, MS_BTN1, MS_BTN2, KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS
    ),
    [3] = LAYOUT(
        KC_TRNS, KC_F1, KC_F2, KC_F3, KC_F4, KC_F5,
        KC_TAB, KC_TRNS, KC_HOME, KC_UP, KC_END, KC_PGUP,
        KC_TRNS, KC_TRNS, KC_LEFT, KC_DOWN, KC_RIGHT, KC_PGDN,
        AML_TO, AML_I50, AML_D50, KC_TRNS, (QK_LALT | QK_LGUI | KC_K), QK_MACRO_2,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS, KC_F6, KC_F7, KC_F8, KC_F9,
        KC_F10, KC_F11, KC_VOLU, KC_KP_7, KC_KP_8, KC_KP_9,
        KC_F12, KC_KP_MINUS, KC_VOLD, KC_KP_4, KC_KP_5, KC_KP_6,
        KC_KP_PLUS, KC_TRNS, KC_NUM, KC_KP_DOT, KC_KP_1, KC_KP_2,
        KC_KP_3, KC_KP_SLASH, KC_KP_ASTERISK, KC_KP_DOT, KC_KP_0, KC_TRNS,
        KC_TRNS
    ),
    [4] = LAYOUT(
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS, MS_BTN1, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS
    ),
};
