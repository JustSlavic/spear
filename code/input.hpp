#ifndef INPUT_HPP
#define INPUT_HPP

#include <base.hpp>


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


struct keyboard_device
{
    enum key
    {
        ESC,
        F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,
        TILDA, _1, _2, _3, _4, _5, _6, _7, _8, _9, _0, underscore, equals, backspace,
        TAB, Q, W, E, R, T, Y, U, I, O, P, BRACKET_OPEN, BRACKET_CLOSE, BACKSLASH,
        CAPS_LOCK, A, S, D, F, G, H, J, K, L, ENTER,
        SHIFT_LEFT, Z, X, C, V, B, N, M, LESS, MORE, SLASH, SHIFT_RIGHT,
        CONTROL_LEFT, MOD_LEFT, ALT_LEFT, SPACE, ALT_RIGHT, CONTROL_RIGHT,
        NUM1, NUM2, NUM3, NUM4, NUM5, NUM6, NUM7, NUM8, NUM9, NUM0,
        PAGE_UP, PAGE_DOWN, HOME, END, INSERT, DELETE,
        // @note: key_count have to be the last in enum
        KEY_COUNT
    };

    button_state buttons[KEY_COUNT];

    button_state & operator [] (key k)
    {
        return buttons[k];
    }

    button_state const & operator [] (key k) const
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

struct mouse_device
{
    enum key
    {
        LMB, MMB, RMB,
        // @note: key_count have to be the last in enum
        KEY_COUNT
    };

    button_state buttons[KEY_COUNT];
    int32 x, y;

    button_state& operator [] (key k)
    {
        return buttons[k];
    }

    button_state const& operator [] (key k) const
    {
        return buttons[k];
    }
};

struct stick_state
{
    //        1
    //        |
    //        |
    // -1 ----0---- 1
    //        |
    //        |
    //       -1
    float32 previous_x, previous_y;
    float32 x, y;
};

struct gamepad_device
{
    enum key
    {
        START, SELECT,
        A, B, X, Y,
        DPAD_UP, DPAD_LEFT, DPAD_RIGHT, DPAD_DOWN,
        LEFT_SHOULDER, RIGHT_SHOULDER,
        LEFT_TRIGGER, RIGHT_TRIGGER,
        LEFT_STICK, RIGHT_STICK,
        // @note: key_count have to be the last in enum
        KEY_COUNT
    };

    button_state buttons[KEY_COUNT];
    stick_state left_stick, right_stick;

    button_state& operator [] (key k)
    {
        return buttons[k];
    }

    button_state const& operator [] (key k) const
    {
        return buttons[k];
    }
};

struct input
{
    keyboard_device keyboard;
    mouse_device mouse;
    gamepad_device gamepads[4];
    float32 dt;
    float32 time;
};


#endif // INPUT_HPP
