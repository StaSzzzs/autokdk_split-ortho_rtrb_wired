#include QMK_KEYBOARD_H
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {};


#include QMK_KEYBOARD_H

static uint16_t current_cpi = 800; // 初期値
#define CPI_STEP 38
#define CPI_MIN (16 * CPI_STEP)
#define CPI_MAX (127 * CPI_STEP)

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed) {
        switch (keycode) {
            case KC_CPI_UP:
                if (current_cpi + CPI_STEP <= CPI_MAX) {
                    current_cpi += CPI_STEP;
                    pointing_device_set_cpi(current_cpi);
                }
                return false;

            case KC_CPI_DOWN:
                if (current_cpi - CPI_STEP >= CPI_MIN) {
                    current_cpi -= CPI_STEP;
                    pointing_device_set_cpi(current_cpi);
                }
                return false;

            case KC_CPI_DEFAULT:
                current_cpi = 800; // 好きなデフォルト値
                pointing_device_set_cpi(current_cpi);
                return false;

            case KC_CPI_NEXT:
                // プリセット配列を回す方式
                {
                    static const uint16_t presets[] = {400, 800, 1200, 1600};
                    static uint8_t idx = 0;
                    idx = (idx + 1) % (sizeof(presets)/sizeof(presets[0]));
                    current_cpi = presets[idx];
                    pointing_device_set_cpi(current_cpi);
                }
                return false;

            case KC_CPI_PREV:
                {
                    static const uint16_t presets[] = {400, 800, 1200, 1600};
                    static uint8_t idx = 0;
                    idx = (idx + sizeof(presets)/sizeof(presets[0]) - 1) %
                          (sizeof(presets)/sizeof(presets[0]));
                    current_cpi = presets[idx];
                    pointing_device_set_cpi(current_cpi);
                }
                return false;
        }
    }
    return true;
}




#if(0)
// レイヤー名を定義します
enum layer_names {
    _BASE = 0,    // Layer 0: 通常のQWERTY配列
    _L1_NAV,      // Layer 1: ナビゲーションとマウスキー (画像の内容)
    _L2_FUNC,     // Layer 2
    _L3_NUM,      // Layer 3
};

// Keyball61の正確なレイアウトマクロ名 (例: LAYOUT_keyball61) に置き換えてください
// #define LAYOUT_SPLITORTHO(...) LAYOUT(__VA_ARGS__) 


const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /* レイヤー 0: _BASE は省略 */
[0] = LAYOUT_SPLITORTHO(
    // 左手側
    KC_ESC,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,
    KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,
    KC_GRV,  KC_A,    KC_S,    KC_D,    KC_F,    KC_G,
    KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,
    KC_LCTL, MO(_NUM),KC_LGUI, KC_LALT, MO(_NAV), KC_SPC,

    // 右手側
             KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS,
             KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_ENT,
             KC_H,    KC_J,    KC_K,    TD(0),   TD(1),   KC_QUOT,
    KC_BSLS, KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH,
    KC_BSPC, KC_DEL,  MO(_FUNC)
),

    /* レイヤー 1: _L1_NAV (画像の内容をここに配置) */
    [1] = LAYOUT_SPLITORTHO(
    // 左手側
    _______, _______, _______, _______, KC_F1,   KC_F2,
    _______, _______, _______, _______, KC_HOME, KC_UP,
    _______, _______, _______, _______, KC_END,  KC_DOWN,
    _______, _______, _______, _______, KC_LEFT, KC_RGHT,
    _______, _______, _______, _______, _______, KC_VOLU,

    // 右手側
             KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,
             KC_PGUP, KC_INS,  KC_7,    KC_8,    KC_9,    KC_F9,
             KC_PGDN, KC_DEL,  KC_4,    KC_5,    KC_6,    KC_F10,
    KC_LGUI, KC_MUTE,KC_1,    KC_2,    KC_3,    KC_F11,
    _______, _______, KC_0,    

    /* レイヤー 2: _L2_FUNC (画像の内容をここに配置) */
    [2] = LAYOUT_SPLITORTHO(
    // 左手側
    _______, _______, _______, _______, KC_F1,   KC_F2,
    _______, _______, _______, _______, KC_HOME, KC_UP,
    _______, _______, _______, _______, KC_END,  KC_DOWN,
    _______, _______, _______, _______, KC_LEFT, KC_RGHT,
    _______, _______, _______, _______, _______, KC_VOLU,

    // 右手側（マウス操作中心）
             KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,
             KC_MS_WH_UP, KC_MS_WH_DOWN, KC_MS_BTN3, KC_F9, _______, KC_F10,
             KC_MS_BTN4, KC_MS_BTN1, KC_MS_BTN2, _______, _______, KC_F11,
    _______, _______, _______, _______, _______, KC_F12,
    _______, _______, _______, _______, _______, _______
),

    /* レイヤー 3: _L3_NUM (画像の内容をここに配置) */
    [3] = LAYOUT_SPLITORTHO(
       // 左手側
    KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,
    KC_LGUI, KC_TAB,  KC_UP,   KC_DOWN, KC_LEFT, KC_RGHT,
    _______, _______, _______, _______, KC_MS_BTN1, KC_MS_BTN2,
    _______, _______, _______, _______, _______, _______,
    _______, _______, _______, _______, _______, _______,

    // 右手側
             KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,
             _______, _______, _______, _______, _______, KC_LCTL,
             _______, _______, _______, _______, _______, KC_LALT,
    _______, _______, _______, _______, _______, KC_LSFT,
    _______, _______, _______, _______, _______, KC_BTN1
),
};
#endif
