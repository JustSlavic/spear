#ifndef INPUT_HPP
#define INPUT_HPP

#include <base.h>


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

struct button_state
{
    bool32 is_down;
    uint32 transition_count;
};


INLINE uint32 get_press_count(button_state button)
{
    uint32 result = (button.transition_count + (button.is_down > 0)) / 2;
    return result;
}

INLINE uint32 get_release_count(button_state button)
{
    uint32 result = (button.transition_count - (button.is_down > 0) + 1) / 2;
    return result;
}

INLINE uint32 get_hold_count(button_state button)
{
    uint32 result = (button.transition_count + (button.is_down > 0) + 1) / 2;
    return result;
}

INLINE void process_button_state(button_state *button, bool32 is_down)
{
    if (button->is_down != is_down)
    {
        button->is_down = is_down;
        button->transition_count += 1;
    }
}

enum keyboard_key
{
    KB_ESC, KB_F1, KB_F2, KB_F3, KB_F4, KB_F5, KB_F6, KB_F7, KB_F8, KB_F9, KB_F10, KB_F11, KB_F12,
    KB_TILDA, KB_1, KB_2, KB_3, KB_4, KB_5, KB_6, KB_7, KB_8, KB_9, KB_0, KB_UNDERSCORE, KB_EQUALS, KB_BACKSPACE,
    KB_TAB, KB_Q, KB_W, KB_E, KB_R, KB_T, KB_Y, KB_U, KB_I, KB_O, KB_P, KB_BRACKET_OPEN, KB_BRACKET_CLOSE, KB_BACKSLASH,
    KB_CAPS_LOCK, KB_A, KB_S, KB_D, KB_F, KB_G, KB_H, KB_J, KB_K, KB_L, KB_ENTER,
    KB_SHIFT_LEFT, KB_Z, KB_X, KB_C, KB_V, KB_B, KB_N, KB_M, KB_ANGLE_OPEN, KB_ANGLE_CLOSE, KB_SLASH, KB_SHIFT_RIGHT,
    KB_CONTROL_LEFT, KB_SUPER_LEFT, KB_ALT_LEFT, KB_SPACE, KB_ALT_RIGHT, KB_CONTROL_RIGHT,
    KB_NUM1, KB_NUM2, KB_NUM3, KB_NUM4, KB_NUM5, KB_NUM6, KB_NUM7, KB_NUM8, KB_NUM9, KB_NUM0,
    KB_PAGE_UP, KB_PAGE_DOWN, KB_HOME, KB_END,KB_INSERT, KB_DELETE,
    // @note: key_count have to be the last in enum
    KB_KEY_COUNT
};

struct keyboard_device
{

    button_state buttons[KB_KEY_COUNT];

    button_state & operator [] (keyboard_key k)
    {
        return buttons[k];
    }

    button_state const & operator [] (keyboard_key k) const
    {
        return buttons[k];
    }
};

void reset_transitions(button_state *buttons, usize key_count)
{
    for (usize key_index = 0; key_index < key_count; key_index++)
    {
        buttons[key_index].transition_count = 0;
    }
}

enum mouse_key
{
    MOUSE_LEFT, MOUSE_MIDDLE, MOUSE_RIGHT,
    // @note: key_count have to be the last in enum
    MOUSE_KEY_COUNT
};

struct mouse_device
{

    button_state buttons[MOUSE_KEY_COUNT];
    int32 x, y;

    button_state& operator [] (mouse_key k)
    {
        return buttons[k];
    }

    button_state const& operator [] (mouse_key k) const
    {
        return buttons[k];
    }
};

struct stick_state
{
    //       +1
    //        |
    //        |
    // -1 ----0---- +1
    //        |
    //        |
    //       -1
    float32 previous_x, previous_y;
    float32 x, y;
};

enum gamepad_key
{
    GP_START, GP_SELECT,
    GP_A, GP_B, GP_X, GP_Y,
    GP_DPAD_UP, GP_DPAD_LEFT, GP_DPAD_RIGHT, GP_DPAD_DOWN,
    GP_LEFT_SHOULDER, GP_RIGHT_SHOULDER,
    GP_LEFT_TRIGGER, GP_RIGHT_TRIGGER,
    GP_LEFT_STICK, GP_RIGHT_STICK,
    // @note: key_count have to be the last in enum
    GP_KEY_COUNT
};

struct gamepad_device
{

    button_state buttons[GP_KEY_COUNT];
    stick_state left_stick, right_stick;

    button_state& operator [] (gamepad_key k)
    {
        return buttons[k];
    }

    button_state const& operator [] (gamepad_key k) const
    {
        return buttons[k];
    }
};

struct input_state
{
    keyboard_device keyboard;
    mouse_device mouse;
    gamepad_device gamepads[4];
    float32 dt;
    float32 time;
};


#endif // INPUT_HPP
