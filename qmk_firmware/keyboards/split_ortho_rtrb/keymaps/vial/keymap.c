#include QMK_KEYBOARD_H

#include QMK_KEYBOARD_H

// レイヤー名を定義します
enum layer_names {
    _BASE = 0,    // Layer 0: 通常のQWERTY配列
    _L1_NAV,      // Layer 1: ナビゲーションとマウスキー (画像の内容)
    _L2_FUNC,     // Layer 2
    _L3_NUM,      // Layer 3
};

// Keyball61の正確なレイアウトマクロ名 (例: LAYOUT_keyball61) に置き換えてください
#define LAYOUT_SPLITORTHO(...) LAYOUT(__VA_ARGS__) 

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /* レイヤー 0: _BASE は省略 */
    [_BASE] = LAYOUT_SPLITORTHO(
        // ここにレイヤー0のキーマップを定義 (前回の回答参照)
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS
    ),

    /* レイヤー 1: _L1_NAV (画像の内容をここに配置) */
    [_L1_NAV] = LAYOUT_SPLITORTHO(
        // 左手側: F1-F5, Navigation, Mouse Keys, Vol
        _______, _______, _______, _______, KC_F1,   KC_F2,   KC_F3,   KC_F4,
        _______, _______, _______, _______, _______, KC_HOME, KC_UP,   KC_END,
        _______, _______, _______, _______, KC_INSERT,KC_LEFT, KC_DOWN, KC_RGHT,
        _______, _______, _______, _______, _______, _______, KC_LGUI, KC_MUTE, // LGUI(K), M2(Mute)
        _______, _______, _______, _______, _______, KC_VOLU, _______, _______,

        // 右手側: F6-F11, Numpad
        KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  _______,
        KC_PGUP, _______, KC_7,    KC_8,    KC_9,    KC_F12,  _______, _______,
        KC_PGDN, _______, KC_4,    KC_5,    KC_6,    KC_PLUS, _______, _______,
        _______, KC_NUMLCK,KC_1,    KC_2,    KC_3,    KC_SLSH, _______, _______,
        _______, _______, KC_0,    KC_DOT,  KC_MINS, _______, _______, _______
    ),

    /* レイヤー 2: _L2_FUNC (画像の内容をここに配置) */
    [_L2_FUNC] = LAYOUT_SPLITORTHO(
        // 左手側
        _______, _______, _______, _______, KC_F1,   KC_F2,   KC_F3,   KC_F4,
        _______, _______, _______, _______, _______, KC_HOME, KC_UP,   KC_END,
        _______, _______, _______, _______, _______, KC_LEFT, KC_DOWN, KC_RGHT,
        _______, _______, _______, _______, _______, _______, KC_TRNS, KC_MS_BTN3, // M3
        _______, _______, _______, _______, _______, KC_MS_BTN1, _______, _______, // M1

        // 右手側
        KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  _______,
        KC_MS_WH_UP, KC_MS_WH_DOWN, KC_MS_BTN3, KC_F12,  _______, _______, _______, _______, // Mouse Wheel Up/Down, Mouse 3
        KC_MS_BTN4, KC_MS_BTN1, KC_MS_BTN2, _______, _______, _______, _______, _______, // Mouse 4, Mouse 1, Mouse 2
        _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______
    ),

    /* レイヤー 3: _L3_NUM (画像の内容をここに配置) */
    [_L3_NUM] = LAYOUT_SPLITORTHO(
        // 左手側
        _______, _______, _______, _______, KC_F1,   KC_F2,   KC_F3,   KC_F4,
        _______, _______, _______, _______, KC_LGUI, KC_TRNS, KC_LGUI, KC_TRNS, // LGui(T), (Up)
        _______, _______, _______, _______, KC_TRNS, LCG(KC_LEFT), LCG(KC_DOWN), LCG(KC_RGHT), // (Left), (Down), (Right)
        _______, _______, _______, _______, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, // Mouse Accel 0/1/2 labels are for Vial config
        _______, _______, _______, _______, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, // M1 label is for Vial config

        // 右手側
        KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  _______,
        KC_F12,  _______, LCG(KC_DOWN), LCG(KC_LEFT), LSG(KC_RGHT), _______, _______, _______, // LCG(D), LCG(Left), LSG(Right)
        _______, _______, LCG(KC_LEFT), LCG(KC_RGHT), LSG(KC_RGHT), _______, _______, _______, // LCG(Left), LCG(Right), LSG(Right)
        _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______
    )
};
