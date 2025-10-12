#ifndef _SPEAR_GAMELIBS_INPUT_H
#define _SPEAR_GAMELIBS_INPUT_H

#include <corelibs/base.h>


//
// Transition count helps detect very quick pressing.
//
//                 frame 1       frame 2
//                 +--+
//                 |  |
// Pulse press: |--+  +-------|------------|
//
//                 +-------------+
//                 |             |
// Hold:        |--+          |  +---------|
//                               ^
//                               second "press" on second frame which I want not to happen
//
// In the example #1: frame 1 { 1 press, 1 hold, 1 release }; frame 2 { 0 press, 0 hold, 0 release }.
// In the example #2: frame 1 { 1 press, 1 hold, 0 release }; frame 2 { 0 press, 1 hold, 1 release }.
//


typedef struct
{
    bool32 is_down;
    uint32 transition_count;
} input_button;

uint32 input_button_get_press_count(input_button button);
uint32 input_button_get_release_count(input_button button);
uint32 input_button_get_hold_count(input_button button);
void input_button_update(input_button *button, bool32 is_down);
void input_button_reset_transitions(input_button *buttons, usize key_count);

enum
{
    Button_None = 0,

    Mouse_Left,
    Mouse_Right,
    Mouse_Middle,
    Mouse_X1,
    Mouse_X2,

    Keyboard_Esc,
    Keyboard_F1,
    Keyboard_F2,
    Keyboard_F3,
    Keyboard_F4,
    Keyboard_F5,
    Keyboard_F6,
    Keyboard_F7,
    Keyboard_F8,
    Keyboard_F9,
    Keyboard_F10,
    Keyboard_F11,
    Keyboard_F12,

    Keyboard_A,
    Keyboard_B,
    Keyboard_C,
    Keyboard_D,
    Keyboard_E,
    Keyboard_F,
    Keyboard_G,
    Keyboard_H,
    Keyboard_I,
    Keyboard_J,
    Keyboard_K,
    Keyboard_L,
    Keyboard_M,
    Keyboard_N,
    Keyboard_O,
    Keyboard_P,
    Keyboard_Q,
    Keyboard_R,
    Keyboard_S,
    Keyboard_T,
    Keyboard_U,
    Keyboard_V,
    Keyboard_W,
    Keyboard_X,
    Keyboard_Y,
    Keyboard_Z,

    Keyboard_1,
    Keyboard_2,
    Keyboard_3,
    Keyboard_4,
    Keyboard_5,
    Keyboard_6,
    Keyboard_7,
    Keyboard_8,
    Keyboard_9,
    Keyboard_0,

    Keyboard_Up,
    Keyboard_Down,
    Keyboard_Left,
    Keyboard_Right,

    Keyboard_Button_Count
};

typedef struct
{
    int32 x, y;
} input_mouse_position;

typedef struct
{
    input_button buttons[Keyboard_Button_Count];
    input_mouse_position window_mouse;
    input_mouse_position viewport_mouse;
    int32 mouse_scroll;
} input_keyboard_and_mouse;

typedef struct
{
    //       +1
    //        |
    //        |
    // -1 ----0---- +1
    //        |
    //        |
    //       -1
    float32 x, y;
} input_gamepad_stick;

enum
{
    Gamepad_None = 0,

    Gamepad_Start,
    Gamepad_Select,
    Gamepad_A,
    Gamepad_B,
    Gamepad_X,
    Gamepad_Y,
    Gamepad_DPadUp,
    Gamepad_DPadDown,
    Gamepad_DPadLeft,
    Gamepad_DPadRight,
    Gamepad_ShoulderLeft,
    Gamepad_ShoulderRight,
    Gamepad_StickLeft,
    Gamepad_StickRight,

    // @note: key_count have to be the last in enum
    Gamepad_Count
};

typedef struct
{
    input_button buttons[Gamepad_Count];
    input_gamepad_stick stick_left;
    input_gamepad_stick stick_right;
    float32 trigger_left;
    float32 trigger_right;
} input_gamepad;

typedef struct
{
    input_keyboard_and_mouse keyboard_and_mouse;
    input_gamepad gamepads[4];
    float64 time;
    float64 dt;
} spear_input;


#endif // _SPEAR_GAMELIBS_INPUT_H
