#include "input.h"


uint32 input_button_get_press_count(input_button button)
{
    uint32 result = (button.transition_count + (button.is_down > 0)) / 2;
    return result;
}

uint32 input_button_get_release_count(input_button button)
{
    uint32 result = (button.transition_count - (button.is_down > 0) + 1) / 2;
    return result;
}

uint32 input_button_get_hold_count(input_button button)
{
    uint32 result = (button.transition_count + (button.is_down > 0) + 1) / 2;
    return result;
}

void input_button_update(input_button *button, bool32 is_down)
{
    button->transition_count += (button->is_down != is_down);
    button->is_down = is_down;
}

void input_button_reset_transitions(input_button *buttons, usize key_count)
{
    usize key_index;
    for (key_index = 0; key_index < key_count; key_index++)
    {
        buttons[key_index].transition_count = 0;
    }
}
