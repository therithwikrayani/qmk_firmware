#include QMK_KEYBOARD_H

// Practical 34-key QMK layout for the Ferris Sweep keyboard
// Version: 12
// Author: Rithwik Rayani
// Tap Dances from Ben Vallack's Layout

// Tap Dance Codes
enum {
	D_01,
	D_02,
	D_03,
	D_04,
	D_F1,
	D_F2,
	D_F3,
	D_F4,
	D_F5,
	D_F6,
	D_F7,
	D_F8,
	D_F9,
	D_F10,
	D_F11,
	D_F12,

};

// Keymap Definitions
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
	[0] = LAYOUT_split_3x5_2(
		KC_Q, KC_W, KC_E, KC_R, KC_T,								KC_Y, KC_U, KC_I, KC_O, KC_P, 
		KC_A, KC_S, KC_D, KC_F, KC_G,								KC_H, KC_J, KC_K, KC_L, TD(D_01), 
		KC_Z, KC_X, KC_C, KC_V, KC_B,								KC_N, KC_M, TD(D_02), TD(D_03), TD(D_04), 
		KC_LSFT, LT(4,KC_BSPC), 									LT(3,KC_SPC), TT(1)
		),
	[1] = LAYOUT_split_3x5_2(
		KC_NO, KC_MPRV, KC_MPLY, KC_MNXT, KC_BRIU, 					KC_NO, KC_HOME, KC_PGDN, KC_PGUP, KC_END, 
		KC_LSFT, KC_LCTL, KC_LGUI, KC_LALT, KC_BRID, 				KC_NO, KC_LEFT, KC_DOWN, KC_UP, KC_RGHT, 
		KC_NO, KC_VOLD, KC_MUTE, KC_VOLU, KC_NO, 					KC_NO, KC_WH_L, KC_WH_D, KC_WH_U, KC_WH_R, 
		TO(0), KC_TRNS, 											KC_TRNS, TO(2)
		),
	[2] = LAYOUT_split_3x5_2(
		KC_1, KC_2, KC_3, KC_4, KC_5,								KC_6, KC_7, KC_8, KC_9, KC_0, 
		TD(D_F1), TD(D_F2), TD(D_F3), TD(D_F4), TD(D_F5), 			TD(D_F6), TD(D_F7), TD(D_F8), TD(D_F9), TD(D_F10), 
		KC_EQL, KC_PLUS, KC_MINS, KC_ASTR, TD(D_F11), 				TD(D_F12), KC_DEL, KC_PERC, KC_LPRN, KC_RPRN, 
		TO(0), KC_TRNS, 											KC_TRNS, TO(0)
		),
	[3] = LAYOUT_split_3x5_2(
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, 				KC_6, KC_7, KC_8, KC_9, KC_0, 
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, 				KC_ENT, KC_LALT, KC_LGUI, KC_LCTL, KC_LSFT, 
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, 				KC_ESC, KC_DEL, KC_BSLS, KC_LBRC, KC_RBRC, 
		KC_SPC, KC_BSPC, 											KC_NO, KC_NO
		),
	[4] = LAYOUT_split_3x5_2(
		KC_1, KC_2, KC_3, KC_4, KC_5, 								KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, 
		KC_LSFT, KC_LCTL, KC_LGUI, KC_LALT, KC_TAB, 				KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, 
		KC_PSCR, KC_CAPS, KC_APP, KC_DEL, KC_ESC, 					KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, 
		KC_NO, KC_NO, 												KC_SPC, KC_BSPC
		)
};

// Tap Dance Tap/Hold States
enum {
    SINGLE_TAP = 1,
    SINGLE_HOLD,
    DOUBLE_TAP,
    DOUBLE_HOLD,
    DOUBLE_SINGLE_TAP,
    MORE_TAPS
};

typedef struct {
    bool is_press_action;
    uint8_t step;
} tap;

static tap dance_state = {
    .is_press_action = true,
    .step = 0
};

uint8_t dance_steps(tap_dance_state_t *state) {
	if (state->count == 1) {
		if (state->interrupted || !state->pressed) return SINGLE_TAP;
		else return SINGLE_HOLD;
	} else if (state->count == 2) {
		if (state->interrupted) return DOUBLE_SINGLE_TAP;
		else if (state->pressed) return DOUBLE_HOLD;
		else return DOUBLE_TAP;
	}
	return MORE_TAPS;
}

void on_dance(tap_dance_state_t *state, void *user_data, uint16_t tap_key, uint16_t hold_key) {
	if (state->count == 3) {
		tap_code16(tap_key);
		tap_code16(tap_key);
		tap_code16(tap_key);
	}
	if (state->count > 3) {
		tap_code16(tap_key);
	}
}

void dance_finished(tap_dance_state_t *state, void *user_data, uint16_t tap_key, uint16_t hold_key) {
	dance_state.step = dance_steps(state);
	switch (dance_state.step) {
		case SINGLE_TAP: register_code16(tap_key); break;
		case SINGLE_HOLD: register_code16(hold_key); break;
		case DOUBLE_TAP: register_code16(tap_key); register_code16(tap_key); break;
		case DOUBLE_SINGLE_TAP: tap_code16(tap_key); register_code16(tap_key);
	}
}

void dance_reset(tap_dance_state_t *state, void *user_data, uint16_t tap_key, uint16_t hold_key) {
	wait_ms(10);
	switch (dance_state.step) {
		case SINGLE_TAP: unregister_code16(tap_key); break;
		case SINGLE_HOLD: unregister_code16(hold_key); break;
		case DOUBLE_TAP: unregister_code16(tap_key); break;
		case DOUBLE_SINGLE_TAP: unregister_code16(hold_key); break;
	}
	dance_state.step = 0;
}

// Tap Hold Macro
#define TAP_HOLD(index, tap_key, hold_key) \
	void on_dance_##index(tap_dance_state_t *state, void *user_data) { on_dance(state, user_data, tap_key, hold_key); } \
	void dance_##index##_finished(tap_dance_state_t *state, void *user_data) { dance_finished(state, user_data, tap_key, hold_key); } \
	void dance_##index##_reset(tap_dance_state_t *state, void *user_data) { dance_reset(state, user_data, tap_key, hold_key); }

TAP_HOLD(01, KC_QUOTE, KC_SCLN)
TAP_HOLD(02, KC_COMM, KC_MINS)
TAP_HOLD(03, KC_DOT, KC_EQL)
TAP_HOLD(04, KC_SLSH, KC_GRV)
TAP_HOLD(F1, KC_1, KC_F1)
TAP_HOLD(F2, KC_2, KC_F2)
TAP_HOLD(F3, KC_3, KC_F3)
TAP_HOLD(F4, KC_4, KC_F4)
TAP_HOLD(F5, KC_5, KC_F5)
TAP_HOLD(F6, KC_6, KC_F6)
TAP_HOLD(F7, KC_7, KC_F7)
TAP_HOLD(F8, KC_8, KC_F8)
TAP_HOLD(F9, KC_9, KC_F9)
TAP_HOLD(F10, KC_0, KC_F10)
TAP_HOLD(F11, KC_SLSH, KC_F11)
TAP_HOLD(F12, KC_DOT, KC_F12)

tap_dance_action_t tap_dance_actions[] = {
	[D_01] = ACTION_TAP_DANCE_FN_ADVANCED(on_dance_01, dance_01_finished, dance_01_reset),
	[D_02] = ACTION_TAP_DANCE_FN_ADVANCED(on_dance_02, dance_02_finished, dance_02_reset),
	[D_03] = ACTION_TAP_DANCE_FN_ADVANCED(on_dance_03, dance_03_finished, dance_03_reset),
	[D_04] = ACTION_TAP_DANCE_FN_ADVANCED(on_dance_04, dance_04_finished, dance_04_reset),
	[D_F1] = ACTION_TAP_DANCE_FN_ADVANCED(on_dance_F1, dance_F1_finished, dance_F1_reset),
	[D_F2] = ACTION_TAP_DANCE_FN_ADVANCED(on_dance_F2, dance_F2_finished, dance_F2_reset),
	[D_F3] = ACTION_TAP_DANCE_FN_ADVANCED(on_dance_F3, dance_F3_finished, dance_F3_reset),
	[D_F4] = ACTION_TAP_DANCE_FN_ADVANCED(on_dance_F4, dance_F4_finished, dance_F4_reset),
	[D_F5] = ACTION_TAP_DANCE_FN_ADVANCED(on_dance_F5, dance_F5_finished, dance_F5_reset),
	[D_F6] = ACTION_TAP_DANCE_FN_ADVANCED(on_dance_F6, dance_F6_finished, dance_F6_reset),
	[D_F7] = ACTION_TAP_DANCE_FN_ADVANCED(on_dance_F7, dance_F7_finished, dance_F7_reset),
	[D_F8] = ACTION_TAP_DANCE_FN_ADVANCED(on_dance_F8, dance_F8_finished, dance_F8_reset),
	[D_F9] = ACTION_TAP_DANCE_FN_ADVANCED(on_dance_F9, dance_F9_finished, dance_F9_reset),
	[D_F10] = ACTION_TAP_DANCE_FN_ADVANCED(on_dance_F10, dance_F10_finished, dance_F10_reset),
	[D_F11] = ACTION_TAP_DANCE_FN_ADVANCED(on_dance_F11, dance_F11_finished, dance_F11_reset),
	[D_F12] = ACTION_TAP_DANCE_FN_ADVANCED(on_dance_F12, dance_F12_finished, dance_F12_reset),
};
