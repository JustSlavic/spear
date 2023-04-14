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
//               +------------+
//               |            |
// Hold:      |--+         |  +---------|
//                            ^
//                            second "press" on second frame which I want not to happen
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


struct keyboard
{
    enum key
    {
        esc,
        f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12,
        tilda, _1, _2, _3, _4, _5, _6, _7, _8, _9, _0, underscore, equals, backspace,
        tab, q, w, e, r, t, y, u, i, o, p, bracket_open, bracket_close, backslash,
        caps_lock, a, s, d, f, g, h, j, k, l, enter,
        left_shift, z, x, c, v, b, n, m, angle_open, angle_close, slash, right_shift,
        left_control, left_mod, left_alt, space, right_alt, right_control,
        num_1, num_2, num_3, num_4, num_5, num_6, num_7, num_8, num_9, num_0,
        page_up, page_down, home, end, insert, _delete,
        // @note: key_count have to be the last in enum
        key_count,
    };

    button_state buttons[key_count];

    button_state & operator [] (key k)
    {
        return buttons[k];
    }

    button_state const & operator [] (key k) const
    {
        return buttons[k];
    }
};


void reset_transitions(keyboard *kb)
{
    for (int key_index = 0; key_index < keyboard::key_count; key_index++)
    {
        kb->buttons[key_index].transition_count = 0;
    }
}


struct input_devices
{
    keyboard keyboard_device;
    // @todo: mouse
    // @todo: gamepads (at least 4)
};


#endif // INPUT_HPP
