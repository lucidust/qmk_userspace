#include QMK_KEYBOARD_H

/* Key Combos */
enum combos {
    WE_ESC,
    SD_TAB,
    UI_BSPC,
    IO_DEL,
    JL_RALT,
    XC_COPY,
    CV_PASTE,
    XV_CUT,
    UO_ENT,
    WR_SPC
};

const uint16_t PROGMEM we_combo[] = {KC_W, KC_E, COMBO_END};
const uint16_t PROGMEM sd_combo[] = {KC_S, KC_D, COMBO_END};
const uint16_t PROGMEM ui_combo[] = {KC_U, KC_I, COMBO_END};
const uint16_t PROGMEM io_combo[] = {KC_I, KC_O, COMBO_END};
const uint16_t PROGMEM jl_combo[] = {KC_J, KC_L, COMBO_END};
const uint16_t PROGMEM xc_combo[] = {KC_X, KC_C, COMBO_END};
const uint16_t PROGMEM cv_combo[] = {KC_C, KC_V, COMBO_END};
const uint16_t PROGMEM xv_combo[] = {KC_X, KC_V, COMBO_END};
const uint16_t PROGMEM uo_combo[] = {KC_U, KC_O, COMBO_END};
const uint16_t PROGMEM wr_combo[] = {KC_W, KC_R, COMBO_END};

combo_t key_combos[] = {
    [WE_ESC]       = COMBO(we_combo, KC_ESC),
    [SD_TAB]       = COMBO(sd_combo, KC_TAB),
    [UI_BSPC]      = COMBO(ui_combo, KC_BSPC),
    [IO_DEL]       = COMBO(io_combo, KC_DEL),
    [JL_RALT]      = COMBO(jl_combo, KC_RALT),
    [XC_COPY]      = COMBO(xc_combo, C(KC_C)),
    [CV_PASTE]     = COMBO(cv_combo, C(KC_V)),
    [XV_CUT]       = COMBO(xv_combo, C(KC_X)),
    [UO_ENT]       = COMBO(uo_combo, KC_ENT),
    [WR_SPC]       = COMBO(wr_combo, KC_SPC),
};

bool combo_should_trigger(uint16_t combo_index, combo_t *combo, uint16_t keycode, keyrecord_t *record) {
    return get_highest_layer(layer_state) == 0;
}

/*
 * Disabled symbol combos, kept here for easy restoration:
 *
 * enum combos:
 *     JK_LPRN,
 *     KL_RPRN,
 *     MCOMM_LBRC,
 *     COMMDOT_RBRC,
 *
 * combo arrays:
 *     const uint16_t PROGMEM jk_combo[] = {KC_J, KC_K, COMBO_END};
 *     const uint16_t PROGMEM kl_combo[] = {KC_K, KC_L, COMBO_END};
 *     const uint16_t PROGMEM mcomm_combo[] = {KC_M, KC_COMM, COMBO_END};
 *     const uint16_t PROGMEM commdot_combo[] = {KC_COMM, KC_DOT, COMBO_END};
 *
 * combo map:
 *     [JK_LPRN]      = COMBO(jk_combo, KC_NO),
 *     [KL_RPRN]      = COMBO(kl_combo, KC_NO),
 *     [MCOMM_LBRC]   = COMBO(mcomm_combo, KC_LBRC),
 *     [COMMDOT_RBRC] = COMBO(commdot_combo, KC_RBRC),
 *
 * process_combo_event behavior:
 *     case JK_LPRN:
 *         tap_code16(shifted ? KC_LT : KC_LPRN);
 *         break;
 *     case KL_RPRN:
 *         tap_code16(shifted ? KC_GT : KC_RPRN);
 *         break;
 */

/* Tap Dance */
// tap_dance_action_t tap_dance_actions[] = {};

/* Key Overrides */
const key_override_t comma_semicolon_override = ko_make_basic(MOD_MASK_SHIFT, KC_COMM, KC_SCLN);
const key_override_t dot_colon_override       = ko_make_basic(MOD_MASK_SHIFT, KC_DOT, KC_COLN);

const key_override_t *key_overrides[] = {
    &comma_semicolon_override,
    &dot_colon_override,
};

layer_state_t layer_state_set_user(layer_state_t state) {
    return update_tri_layer_state(state, 1, 2, 5);
}

/* Keymaps */
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT_60_ansi_split_bs_rshift(
        QK_GESC, KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_BSLS, KC_GRV,
        KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_BSPC,
        KC_LCTL, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,          KC_ENT,
        KC_LSFT,          KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT, KC_DEL,
        MO(1),   KC_LGUI, KC_LALT,                                     KC_SPC,                    MO(2),   MO(3),   LT(4, KC_APP), KC_RCTL
    ),
    [1] = LAYOUT_60_ansi_split_bs_rshift(
        KC_TRNS, KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, MS_WHLL, MS_WHLD, MS_WHLU, MS_WHLR, KC_CAPS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, KC_INS,  KC_TRNS,          KC_TRNS,
        KC_TRNS,          KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_HOME, KC_PGDN, KC_PGUP, KC_END,  KC_DEL,  KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS,                                     KC_TRNS,                   KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS
    ),
    [2] = LAYOUT_60_ansi_split_bs_rshift(
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_EXLM, KC_AT,   KC_HASH, KC_DLR,  KC_GRV,  KC_TRNS, KC_TRNS, KC_TRNS, KC_LPRN, KC_RPRN, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_PERC, KC_CIRC, KC_AMPR, KC_ASTR, KC_BSLS, KC_TRNS, KC_LSFT, KC_LCTL, KC_LALT, KC_LGUI, KC_TRNS,          KC_TRNS,
        KC_TRNS,          KC_LBRC, KC_RBRC, KC_MINS, KC_EQL,  KC_TRNS, KC_TRNS, KC_LT,   KC_GT,   KC_LCBR, KC_RCBR, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS,                                     KC_TRNS,                   KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS
    ),
    [3] = LAYOUT_60_ansi_split_bs_rshift(
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_F12,  KC_F7,   KC_F8,   KC_F9,   KC_PSCR, KC_NUM,  KC_MPRV, KC_MNXT, KC_MPLY, KC_MUTE, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_F11,  KC_F4,   KC_F5,   KC_F6,   KC_SCRL, KC_TRNS, KC_LSFT, KC_LCTL, KC_LALT, KC_LGUI, KC_TRNS,          KC_TRNS,
        KC_TRNS,          KC_F10,  KC_F1,   KC_F2,   KC_F3,   KC_PAUS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS,                                     KC_TRNS,                   KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS
    ),
    [4] = LAYOUT_60_ansi_split_bs_rshift(
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS, KC_7,    KC_8,    KC_9,    KC_0,    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS, KC_4,    KC_5,    KC_6,    KC_TRNS, KC_TRNS, KC_LSFT, KC_LCTL, KC_LALT, KC_LGUI, KC_TRNS,          KC_TRNS,
        KC_TRNS,          KC_TRNS, KC_1,    KC_2,    KC_3,    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS,                                     KC_TRNS,                   KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS
    ),
    [5] = LAYOUT_60_ansi_split_bs_rshift(
        QK_BOOT, QK_RBT,  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,          KC_TRNS,
        KC_TRNS,          KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS,                                     KC_TRNS,                   KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS
    )
};
