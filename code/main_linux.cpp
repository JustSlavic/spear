#include <base.hpp>
#include <X11/Xlib.h>

#include <game.hpp>
#include <gfx/opengl/gl.hpp>
#include <gfx/renderer.hpp>

#include <stdio.h>
#include <string.h>
#include <unistd.h>

typedef GLXContext (*glXCreateContextAttribsARBProc)(Display*, GLXFBConfig, GLXContext, Bool, const int*);


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


// Helper to check for extension string presence.  Adapted from:
//   http://www.opengl.org/resources/features/OGLextensions/
static bool isExtensionSupported(const char *extList, const char *extension)
{
    const char *start;
    const char *where, *terminator;

    /* Extension names should not have spaces. */
    where = strchr(extension, ' ');
    if (where || *extension == '\0')
        return false;

    /* It takes a bit of care to be fool-proof about parsing the
    OpenGL extensions string. Don't be fooled by sub-strings,
    etc. */
    for (start=extList;;)
    {
        where = strstr(start, extension);

        if (!where)
            break;

        terminator = where + strlen(extension);

        if (where == start || *(where - 1) == ' ')
            if (*terminator == ' ' || *terminator == '\0')
                return true;

        start = terminator;
    }

    return false;
}

static bool ctxErrorOccurred = false;
static int ctxErrorHandler(Display *dpy, XErrorEvent *ev)
{
    ctxErrorOccurred = true;
    return 0;
}

int main(int argc, char **argv, char **env)
{
    Display* display = XOpenDisplay(NULL);
    if (display == NULL)
    {
        return 1;
    }

// Get a matching FB config
    int visual_attribs[] =
    {
        GLX_X_RENDERABLE    , True,
        GLX_DRAWABLE_TYPE   , GLX_WINDOW_BIT,
        GLX_RENDER_TYPE     , GLX_RGBA_BIT,
        GLX_X_VISUAL_TYPE   , GLX_TRUE_COLOR,
        GLX_RED_SIZE        , 8,
        GLX_GREEN_SIZE      , 8,
        GLX_BLUE_SIZE       , 8,
        GLX_ALPHA_SIZE      , 8,
        GLX_DEPTH_SIZE      , 24,
        GLX_STENCIL_SIZE    , 8,
        GLX_DOUBLEBUFFER    , True,
        //GLX_SAMPLE_BUFFERS  , 1,
        //GLX_SAMPLES         , 4,
        None
    };

    int glx_major, glx_minor;

    // FBConfigs were added in GLX version 1.3.
    if (!glXQueryVersion(display, &glx_major, &glx_minor) ||
        ((glx_major == 1) && (glx_minor < 3)) || (glx_major < 1))
    {
        printf("Invalid GLX version");
        exit(1);
    }

    printf("GLX Version %d.%d\n", glx_major, glx_minor);

    printf("Getting matching framebuffer configs\n");
    int x_framebuffer_config_count;
    GLXFBConfig* x_framebuffer_configs = glXChooseFBConfig(display, DefaultScreen(display), visual_attribs, &x_framebuffer_config_count);
    if (!x_framebuffer_configs)
    {
        printf("Failed to retrieve a framebuffer config\n");
        exit(1);
    }
    printf("Found %d matching FB configs.\n", x_framebuffer_config_count);

    // Pick the FB config/visual with the most samples per pixel
    printf("Getting XVisualInfos\n");

    int desired_num_sample_buffers = 1;
    int desired_num_samples = 4;
    XVisualInfo *x_visual_info = NULL;
    GLXFBConfig x_framebuffer_config = {};

    for (int i = 0; i < x_framebuffer_config_count; ++i)
    {
        XVisualInfo *vi = glXGetVisualFromFBConfig(display, x_framebuffer_configs[i]);
        if (vi)
        {
            int sample_buffers, samples;
            glXGetFBConfigAttrib(display, x_framebuffer_configs[i], GLX_SAMPLE_BUFFERS, &sample_buffers);
            glXGetFBConfigAttrib(display, x_framebuffer_configs[i], GLX_SAMPLES, &samples);

            if ((sample_buffers == desired_num_sample_buffers) && (samples == desired_num_samples))
            {
                printf("  Matching fbconfig %d, visual ID 0x%2lx: SAMPLE_BUFFERS = %d,"
                    " SAMPLES = %d\n",
                    i, vi->visualid, sample_buffers, samples);
                x_visual_info = vi;
                x_framebuffer_config = x_framebuffer_configs[i];
            }
            else
            {
                XFree(vi);
            }
        }
    }

    // Be sure to free the FBConfig list allocated by glXChooseFBConfig()
    XFree(x_framebuffer_configs);

    printf("Creating colormap\n");
    Colormap x_colormap = XCreateColormap(display, RootWindow(display, x_visual_info->screen), x_visual_info->visual, AllocNone);

    XSetWindowAttributes swa;
    swa.colormap          = x_colormap;
    swa.background_pixmap = 0;
    swa.border_pixel      = 0;
    swa.event_mask        = StructureNotifyMask;

    printf("Creating window\n");
    Window window = XCreateWindow(
        display, RootWindow(display, x_visual_info->screen),
        0, 0,
        800, 600,
        0,
        x_visual_info->depth,
        InputOutput,
        x_visual_info->visual,
        CWBorderPixel|CWColormap|CWEventMask,
        &swa);

    if (!window)
    {
        printf("Failed to create window.\n");
        exit(1);
    }

    // Done with the visual info data
    XFree(x_visual_info);

    XStoreName(display, window, "GL 3.0 Window");

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

    // Get the default screen's GLX extension list
    const char *glxExts = glXQueryExtensionsString(display, DefaultScreen(display));

    // NOTE: It is not necessary to create or make current to a context before
    // calling glXGetProcAddressARB
    glXCreateContextAttribsARBProc glXCreateContextAttribsARB = 0;
    glXCreateContextAttribsARB = (glXCreateContextAttribsARBProc)
    glXGetProcAddressARB((const GLubyte *) "glXCreateContextAttribsARB");

    GLXContext ctx = 0;

    // Install an X error handler so the application won't exit if GL 3.0
    // context allocation fails.
    //
    // Note this error handler is global.  All display connections in all threads
    // of a process use the same error handler, so be sure to guard against other
    // threads issuing X commands while this code is running.
    ctxErrorOccurred = false;
    int (*oldHandler)(Display*, XErrorEvent*) = XSetErrorHandler(&ctxErrorHandler);

    // Check for the GLX_ARB_create_context extension string and the function.
    // If either is not present, use GLX 1.3 context creation method.
    if (!isExtensionSupported(glxExts, "GLX_ARB_create_context") || !glXCreateContextAttribsARB)
    {
        printf("glXCreateContextAttribsARB() not found ... using old-style GLX context\n");
        ctx = glXCreateNewContext(display, x_framebuffer_config, GLX_RGBA_TYPE, 0, True);
    }
    else
    {
        // If it does fail, try to get a GL 3.0 context!
        int context_attribs[] =
        {
            GLX_CONTEXT_MAJOR_VERSION_ARB, 4,
            GLX_CONTEXT_MINOR_VERSION_ARB, 0,
            //GLX_CONTEXT_FLAGS_ARB        , GLX_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
            0
        };

        printf("Creating context\n");
        ctx = glXCreateContextAttribsARB(display, x_framebuffer_config, 0, True, context_attribs);

        // Sync to ensure any errors generated are processed.
        XSync(display, False);
        if (!ctxErrorOccurred && ctx)
            printf("Created GL 4.0 context\n");
        else
        {
            // Couldn't create GL 3.0 context.  Fall back to old-style 2.x context.
            // When a context version below 3.0 is requested, implementations will
            // return the newest context version compatible with OpenGL versions less
            // than version 3.0.
            // GLX_CONTEXT_MAJOR_VERSION_ARB = 1
            context_attribs[1] = 1;
            // GLX_CONTEXT_MINOR_VERSION_ARB = 0
            context_attribs[3] = 0;

            ctxErrorOccurred = false;

            printf("Failed to create GL 3.0 context ... using old-style GLX context\n");
            ctx = glXCreateContextAttribsARB(display, x_framebuffer_config, 0, True, context_attribs);
        }
    }

    // Sync to ensure any errors generated are processed.
    XSync(display, false);

    // Restore the original error handler
    XSetErrorHandler(oldHandler);

    if (ctxErrorOccurred || !ctx)
    {
        printf("Failed to create an OpenGL context\n");
        exit(1);
    }

    // Verifying that context is a direct context
    if (! glXIsDirect (display, ctx))
    {
        printf("Indirect GLX rendering context obtained\n");
    }
    else
    {
        printf("Direct GLX rendering context obtained\n");
    }

    printf("Making context current\n");
    glXMakeCurrent(display, window, ctx);

    gfx::initialize(gfx::graphics_api::opengl);

    memory_block global_memory;
    global_memory.memory = malloc(MEGABYTES(5));
    global_memory.size = MEGABYTES(5);

    memory::allocator global_allocator;
    memory::initialize_memory_arena(&global_allocator, global_memory.memory, global_memory.size);

    memory_block game_memory = ALLOCATE_BLOCK_(&global_allocator, MEGABYTES(1));
    memory_block scratchpad_memory = ALLOCATE_BLOCK_(&global_allocator, MEGABYTES(1));
    memory_block renderer_memory = ALLOCATE_BLOCK_(&global_allocator, MEGABYTES(1));
    memory_block resource_memory = ALLOCATE_BLOCK_(&global_allocator, MEGABYTES(1));
    memory_block string_id_memory = ALLOCATE_BLOCK_(&global_allocator, MEGABYTES(1));

    execution_context context = {};
    context.resource_storage.resource_count = 1; // Consider 0 resource being null-resource, indicating the lack of it.

    memory::initialize_memory_arena(&context.temporary_allocator, scratchpad_memory.memory, scratchpad_memory.size);
    memory::initialize_memory_arena(&context.renderer_allocator, renderer_memory.memory, renderer_memory.size);
    memory::initialize_memory_heap(&context.resource_storage.heap, resource_memory.memory, resource_memory.size);
    memory::initialize_memory_arena(&context.strid_storage.arena, string_id_memory.memory, string_id_memory.size);

    initialize_memory(&context, game_memory);
    gfx::set_clear_color(0, 0, 0, 1);

    auto view = math::matrix4::identity();
    float32 aspect_ratio = 16.0f / 9.0f;
    auto projection = gfx::make_projection_matrix_fov(math::to_radians(60), aspect_ratio, 0.05f, 100.0f);

    input_devices input = {};

    while (running)
    {
        reset_transitions(&input.keyboard_device);

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
            //     (e.state & Mod4Mask) > 0,    // window/Super Key
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

        gfx::clear();

        update_and_render(&context, game_memory, &input, 0.333);

        for (usize cmd_index = context.next_execution_command_index;
             cmd_index < context.execution_command_queue_size;
             cmd_index = (cmd_index + 1) % ARRAY_COUNT(context.execution_command_queue))
        {
            auto cmd = pop_execution_command(&context);
            switch (cmd.type)
            {
                case execution_command::exit:
                {
                    running = false;
                }
                break;

            default:
                break;
            }
        }

        for (usize cmd_index = 0; cmd_index < context.render_command_queue_size; cmd_index++)
        {
            auto *cmd = context.render_command_queue + cmd_index;
            switch (cmd->type)
            {
                case gfx::render_command::command_type::setup_projection_matrix:
                break;

                case gfx::render_command::command_type::setup_camera:
                {
                    view = gfx::make_look_at_matrix(cmd->setup_camera.camera_position, cmd->setup_camera.look_at_position, cmd->setup_camera.camera_up_direction);
                }
                break;

                case gfx::render_command::command_type::draw_background:
                {
                    gfx::draw_background(&context, cmd);
                }
                break;

                case gfx::render_command::command_type::draw_mesh_1:
                {
                    gfx::draw_polygon_simple(&context,
                        cmd->draw_mesh_1.mesh_token, cmd->draw_mesh_1.shader_token,
                        cmd->draw_mesh_1.model, view, projection,
                        math::vector4::zero());
                }
                break;

                case gfx::render_command::command_type::draw_mesh_with_color:
                {
                    gfx::draw_polygon_simple(&context,
                        cmd->draw_mesh_with_color.mesh_token, cmd->draw_mesh_with_color.shader_token,
                        cmd->draw_mesh_with_color.model, view, projection,
                        cmd->draw_mesh_with_color.color);
                }
                break;
            }
        }
        context.render_command_queue_size = 0;
        memory::reset_allocator(&context.temporary_allocator);

        glXSwapBuffers (display, window);
    }

    glXMakeCurrent(display, 0, 0);
    glXDestroyContext(display, ctx);

    XDestroyWindow(display, window);
    XFreeColormap(display, x_colormap);
    XCloseDisplay(display);

    return 0;
}

#include <gfx/renderer.cpp>
#include <memory/allocator.cpp>
#include <string_id.cpp>
#include <game.cpp>
