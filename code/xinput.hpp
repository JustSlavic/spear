#ifndef XINPUT_HPP
#define XINPUT_HPP

#include <base.h>
#include <platform_win32.hpp>
#include <input.hpp>
#include <xinput.h>


#define XINPUT_GET_STATE(NAME) DWORD WINAPI NAME(DWORD dwUserIndex, XINPUT_STATE *pState)
#define XINPUT_SET_STATE(NAME) DWORD WINAPI NAME(DWORD dwUserIndex, XINPUT_VIBRATION *pVibration)


namespace win32
{

XINPUT_GET_STATE(get_state_stub)
{
    return ERROR_DEVICE_NOT_CONNECTED;
}

XINPUT_SET_STATE(set_state_stub)
{
    return ERROR_DEVICE_NOT_CONNECTED;
}

struct xinput
{
    typedef XINPUT_GET_STATE(get_state_t);
    typedef XINPUT_SET_STATE(set_state_t);

    dll library;

    get_state_t *get_state;
    set_state_t *set_state;

    static xinput load()
    {
        xinput result;
        result.library = win32::dll::load("xinput1_4.dll");
        if (!result.library.handle)
            result.library = win32::dll::load("xinput9_1_0.dll");
        if (!result.library.handle)
            result.library = win32::dll::load("xinput1_3.dll");

        if (result.library.handle)
        {
            result.get_state = (get_state_t *) result.library.get_function("XInputGetState");
            result.set_state = (set_state_t *) result.library.get_function("XInputSetState");
        }
        else
        {
            result.get_state = get_state_stub;
            result.set_state = set_state_stub;
        }
        return result;
    }

    void process_button(button_state* new_state, DWORD xinput_state, DWORD button_bit)
    {
        button_state  old_state = *new_state;
        new_state->is_down = (xinput_state & button_bit) == button_bit;
        new_state->transition_count = ((new_state->is_down != old_state.is_down) ? 1 : 0);
    }

    float32 process_stick_deadzone(int16 value, int16 deadzone)
    {
        if (value < -deadzone)
        {
            return (float32)(value + deadzone) / (float32)(32768 - deadzone);
        }
        else if (value > deadzone)
        {
            return (float32)(value - deadzone) / (float32)(32767 - deadzone);
        }

        return 0.f;
    }

    float32 process_trigger_deadzone(uint8 value, uint8 deadzone)
    {
        if (value < deadzone)
        {
            return (float32)(value + deadzone) / (float32)(255 - deadzone);
        }

        return 0.f;
    }

    void process_gamepad_state(gamepad_device *gamepad, int index)
    {
        XINPUT_STATE InputState;
        auto ec = XInputGetState(index, &InputState);

        if (ec != ERROR_SUCCESS) return;

        XINPUT_GAMEPAD XGamepad = InputState.Gamepad;

        process_button(&gamepad->buttons[GP_A], XGamepad.wButtons, XINPUT_GAMEPAD_A);
        process_button(&gamepad->buttons[GP_B], XGamepad.wButtons, XINPUT_GAMEPAD_B);
        process_button(&gamepad->buttons[GP_X], XGamepad.wButtons, XINPUT_GAMEPAD_X);
        process_button(&gamepad->buttons[GP_Y], XGamepad.wButtons, XINPUT_GAMEPAD_Y);

        process_button(&gamepad->buttons[GP_DPAD_UP], XGamepad.wButtons, XINPUT_GAMEPAD_DPAD_UP);
        process_button(&gamepad->buttons[GP_DPAD_DOWN], XGamepad.wButtons, XINPUT_GAMEPAD_DPAD_DOWN);
        process_button(&gamepad->buttons[GP_DPAD_LEFT], XGamepad.wButtons, XINPUT_GAMEPAD_DPAD_LEFT);
        process_button(&gamepad->buttons[GP_DPAD_RIGHT], XGamepad.wButtons, XINPUT_GAMEPAD_DPAD_RIGHT);

        process_button(&gamepad->buttons[GP_LEFT_SHOULDER], XGamepad.wButtons, XINPUT_GAMEPAD_LEFT_SHOULDER);
        process_button(&gamepad->buttons[GP_RIGHT_SHOULDER], XGamepad.wButtons, XINPUT_GAMEPAD_RIGHT_SHOULDER);

        process_button(&gamepad->buttons[GP_LEFT_STICK], XGamepad.wButtons, XINPUT_GAMEPAD_LEFT_THUMB);
        process_button(&gamepad->buttons[GP_RIGHT_STICK], XGamepad.wButtons, XINPUT_GAMEPAD_RIGHT_THUMB);

        process_button(&gamepad->buttons[GP_SELECT], XGamepad.wButtons, XINPUT_GAMEPAD_BACK);
        process_button(&gamepad->buttons[GP_START], XGamepad.wButtons, XINPUT_GAMEPAD_START);

        // @todo: process sticks
        gamepad->left_stick.x = process_stick_deadzone(XGamepad.sThumbLX, XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
        gamepad->left_stick.y = process_stick_deadzone(XGamepad.sThumbLY, XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);

        gamepad->right_stick.x = process_stick_deadzone(XGamepad.sThumbRX, XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);
        gamepad->right_stick.y = process_stick_deadzone(XGamepad.sThumbRY, XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);

        // @todo: process triggers
    }
};




} // namespace win32


#endif // XINPUT_HPP
