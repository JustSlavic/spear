#include <base.hpp>
#include <X11/Xlib.h>


#define KEYCODE_ESC                  9
#define KEYCODE_W                    25
#define KEYCODE_A                    38
#define KEYCODE_S                    39
#define KEYCODE_D                    40
#define KEYCODE_Z                    52
#define KEYCODE_SPACE                65
#define KEYCODE_F1                   67
#define KEYCODE_F2                   68
#define KEYCODE_F3                   69
#define KEYCODE_F4                   70
#define KEYCODE_F5                   71
#define KEYCODE_F6                   72
#define KEYCODE_F7                   73
#define KEYCODE_F8                   74
#define KEYCODE_F9                   75
#define KEYCODE_F10                  76
#define KEYCODE_F11                  95
#define KEYCODE_F12                  96
#define KEYCODE_UP                   111
#define KEYCODE_LEFT                 113
#define KEYCODE_RIGHT                114
#define KEYCODE_DOWN                 116
#define KEYCODE_CTRL                 37
#define KEYCODE_SHIFT                50

#define MOUSE_LMB                    1
#define MOUSE_MMB                    2
#define MOUSE_RMB                    3
#define MOUSE_WHEEL_UP               4
#define MOUSE_WHEEL_DOWN             5
#define MOUSE_MB_1                   8
#define MOUSE_MB_2                   9

#define GAMEPAD_EVENT_BUTTON         0x01    /* button pressed/released */
#define GAMEPAD_EVENT_AXIS           0x02    /* joystick moved */
#define GAMEPAD_EVENT_INIT           0x80    /* initial state of device */

#define GAMEPAD_AXIS_MIN             (-32767)
#define GAMEPAD_AXIS_MAX             (32767)

#define GAMEPAD_LEFT_THUMB_DEADZONE  7849
#define GAMEPAD_RIGHT_THUMB_DEADZONE 8689
#define GAMEPAD_TRIGGER_DEADZONE     7710


GLOBAL bool32 running = true;


int main(int argc, char **argv, char **env)
{
    Display* display = XOpenDisplay(NULL);
    if (display == NULL)
    {
        return 1;
    }

    int screen = XDefaultScreen(display);

    Window window = XCreateSimpleWindow(
        display,
        RootWindow(display, screen),
        0, // WIN_X,
        0, // WIN_Y,
        800,
        600,
        1, // WIN_BORDER,
        BlackPixel(display, screen),
        WhitePixel(display, screen));

    // Process window close event through event handler so XNextEvent does not fail
    Atom del_window = XInternAtom(display, "WM_DELETE_WINDOW", 0);
    XSetWMProtocols(display, window, &del_window, 1);

    // Select kind of events we are interested in
    XSelectInput(display, window,
        ExposureMask |
        KeyPressMask |
        KeyReleaseMask |
        PointerMotionMask |
        ButtonPressMask |
        ButtonReleaseMask);

    // Display the window
    XMapWindow(display, window);

    while (running)
    {
        XEvent event;
        while (XPending(display))
        {
            XNextEvent(display, &event);

            //
            // @note: X11 key-repeat BS:
            // The X-Server always sends you repeated keys. It's bad, because I want
            // to be precise when player hits/releases the keyboard key.
            // The event stream looks like this:
            //
            //    ... [real-press] ... [repeat-release]-[repeat-press]  [repeat-release]-[repeat-press]  [real-release] ...
            //
            // Repeat events always go in pairs: RELEASE-PRESS, and always have the same timestamp.
            // I detect that in the code below, and just skip them.
            //
            XEvent next_event;
            if (XPending(display))
            {
                // @note: XPeekEvent is blocking call. XPending makes sure I call XPeekEvent only when
                // there are events present in the queue.
                XPeekEvent(display, &next_event);
                if ((event.type == KeyRelease) && (next_event.type == KeyPress))
                {
                    if (event.xkey.time == next_event.xkey.time)
                    {
                        XNextEvent(display, &next_event); // Poll next event from the queue
                        continue; // Skip
                    }
                }
            }
        }

        switch (event.type)
        {
            case MotionNotify:
            {
            }
            break;

            case ButtonPress:
            case ButtonRelease:
            {
            }
            break;

            case KeyPress:
            case KeyRelease:
            {
                bool32 is_down = (event.type == KeyPress);
                UNUSED(is_down);
                if (event.xkey.keycode == KEYCODE_ESC)
                {
                    running = false;
                }
            }
            break;

            // printf("keycode release: %d\n", e.keycode);
            // print_binary(e.state);
            // printf("\n");
            // printf(
            //     "[ B1 B2 B3 B4 B5 Shift Lock Control M1 M2 M3 M4 M5 ]\n"
            //     "[ %2d %2d %2d %2d %2d %5d %4d %7d %2d %2d %2d %2d %2d ]\n",
            //     (e.state & Button1Mask) > 0, // LMB
            //     (e.state & Button2Mask) > 0, // MMB
            //     (e.state & Button3Mask) > 0, // RMB
            //     (e.state & Button4Mask) > 0, //
            //     (e.state & Button5Mask) > 0, //
            //     (e.state & ShiftMask) > 0,   // Shift
            //     (e.state & LockMask) > 0,    // CapsLock
            //     (e.state & ControlMask) > 0, //
            //     (e.state & Mod1Mask) > 0,    //
            //     (e.state & Mod2Mask) > 0,    // NumLock
            //     (e.state & Mod3Mask) > 0,    //
            //     (e.state & Mod4Mask) > 0,    // Win/Super Key
            //     (e.state & Mod5Mask) > 0);   //

            case ClientMessage:
                running = false;
                break;
            case Expose:
                int x = event.xexpose.x;
                int y = event.xexpose.y;
                int w = event.xexpose.width;
                int h = event.xexpose.height;
                UNUSED(x); UNUSED(y); UNUSED(w); UNUSED(h);
                // @todo: get new window buffer size
                // linux_resize_screen_buffer(&screen_buffer, x + w, y + h);
                break;
        }
    }

    XDestroyWindow(display, window);
    XCloseDisplay(display);

    return 0;
}
