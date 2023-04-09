#ifndef GFX_GL_X11_HPP
#define GFX_GL_X11_HPP

#include <base.hpp>

#include <X11/Xutil.h>
#include <GL/gl.h>
#include <GL/glx.h>


typedef struct __GLXFBConfigRec *GLXFBConfig;
// typedef struct __GFXcontextRec  *GLXContext;

// typedef XVisualInfo *( *PFNGLXGETVISUALFROMFBCONFIGPROC) (Display *dpy, GLXFBConfig config);

#define GLX_WINDOW_BIT                    0x00000001
#define GLX_RGBA_BIT                      0x00000001

#define GLX_X_VISUAL_TYPE                 0x22

#define GLX_TRUE_COLOR                    0x8002
#define GLX_DRAWABLE_TYPE                 0x8010
#define GLX_RENDER_TYPE                   0x8011
#define GLX_X_RENDERABLE                  0x8012

#define GLX_RGBA                          4
#define GLX_DOUBLEBUFFER                  5

#define GLX_RED_SIZE                      8
#define GLX_GREEN_SIZE                    9
#define GLX_BLUE_SIZE                     10
#define GLX_ALPHA_SIZE                    11
#define GLX_DEPTH_SIZE                    12
#define GLX_STENCIL_SIZE                  13
// #define GLX_SAMPLE_BUFFERS                100000
// #define GLX_SAMPLES                       100001

extern "C"
{
// bool glXQueryVersion(Display *dpy, int *major, int *minor);
GLXFBConfig *glXChooseFBConfig (Display *dpy, int screen, const int *attrib_list, int *nelements);
XVisualInfo *glXGetVisualFromFBConfig(Display *dpy, GLXFBConfig config);
int glXGetFBConfigAttrib(Display *dpy, GLXFBConfig config, int attribute, int *value);
const char *glXQueryExtensionsString( Display *dpy, int screen );
}

typedef void glXSwapIntervalType(Display *display, GLXDrawable drawable, int interval);
typedef GLXContext glXCreateContextAttribsType(Display*, GLXFBConfig, GLXContext, Bool, const int*);

GLOBAL glXSwapIntervalType *glXSwapInterval;
GLOBAL glXCreateContextAttribsType *glXCreateContextAttribs;


namespace gfx::gl {


struct glx_driver
{
    GLXContext glx_context;
};

static_assert(sizeof(glx_driver) < sizeof(gfx::driver));


// @todo: make another function that just parses extension string and returns
// an array of c-strings for every supported extension. Then check for equallity
// should be straight-forward.
// @note: for now while in an early development phase, consider all needed extensions
// are present on a dev machine.
bool is_opengl_extension_supported(const char *extList, const char *extension)
{
    return true;
}


bool32 create_opengl_window(int32 width, int32 height, void *w, void *d)
{
    Display* display = XOpenDisplay(NULL);
    if (display == NULL)
    {
        return false;
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
        GLX_DOUBLEBUFFER    , true,
        //GLX_SAMPLE_BUFFERS  , 1,
        //GLX_SAMPLES         , 4,
        None
    };

    // FBConfigs were added in GLX version 1.3.
    int glx_major, glx_minor;
    if (!glXQueryVersion(display, &glx_major, &glx_minor) ||
        ((glx_major == 1) && (glx_minor < 3)) || (glx_major < 1))
    {
        return false;
    }

    int x_framebuffer_config_count;
    GLXFBConfig* x_framebuffer_configs = glXChooseFBConfig(display, DefaultScreen(display), visual_attribs, &x_framebuffer_config_count);
    if (!x_framebuffer_configs)
    {
        return false;
    }

    int desired_num_sample_buffers = 0;
    int desired_num_samples = 0;
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

    Colormap x_colormap = XCreateColormap(display, RootWindow(display, x_visual_info->screen), x_visual_info->visual, AllocNone);

    XSetWindowAttributes x_window_attributes;
    x_window_attributes.colormap          = x_colormap;
    x_window_attributes.background_pixmap = 0;
    x_window_attributes.border_pixel      = 0;
    x_window_attributes.event_mask        = ExposureMask
                                          | KeyPressMask
                                          | KeyReleaseMask
                                          | PointerMotionMask
                                          | ButtonPressMask
                                          | StructureNotifyMask
                                          | ButtonReleaseMask;

    Window window = XCreateWindow(
        display, RootWindow(display, x_visual_info->screen),
        0, 0,
        width, height,
        0,
        x_visual_info->depth,
        InputOutput,
        x_visual_info->visual,
        CWBorderPixel|CWColormap|CWEventMask,
        &x_window_attributes);

    if (!window)
    {
        return false;
    }

    XFree(x_visual_info);
    XStoreName(display, window, "Spear");

    // Process window close event through event handler so XNextEvent does not fail
    Atom del_window = XInternAtom(display, "WM_DELETE_WINDOW", 0);
    XSetWMProtocols(display, window, &del_window, 1);

    // Display the window
    XMapWindow(display, window);

    // Get the default screen's GLX extension list
    const char *glxExts = glXQueryExtensionsString(display, DefaultScreen(display));

    // @note: It is not necessary to create or make current to a context before calling glXGetProcAddressARB
    glXCreateContextAttribs = (glXCreateContextAttribsType *) glXGetProcAddress((const GLubyte *) "glXCreateContextAttribsARB");
    glXSwapInterval = (glXSwapIntervalType *) glXGetProcAddress((GLubyte const *) "glXSwapIntervalEXT");

    GLXContext glx_context = 0;

    // Check for the GLX_ARB_create_context extension string and the function.
    // If either is not present, use GLX 1.3 context creation method.
    if (!is_opengl_extension_supported(glxExts, "GLX_ARB_create_context") || !glXCreateContextAttribs)
    {
        glx_context = glXCreateNewContext(display, x_framebuffer_config, GLX_RGBA_TYPE, 0, True);
    }
    else
    {
        // If it does fail, try to get a GL 4.0 context!
        int context_attribs[] =
        {
            GLX_CONTEXT_MAJOR_VERSION_ARB, 4,
            GLX_CONTEXT_MINOR_VERSION_ARB, 0,
            //GLX_CONTEXT_FLAGS_ARB        , GLX_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
            0
        };

        glx_context = glXCreateContextAttribs(display, x_framebuffer_config, 0, True, context_attribs);

        // Sync to ensure any errors generated are processed.
        XSync(display, False);

        if (glx_context)
        {
            // Ok
        }
        else
        {
            // Couldn't create GL 4.0 context.  Fall back to old-style 2.x context.
            // When a context version below 3.0 is requested, implementations will
            // return the newest context version compatible with OpenGL versions less
            // than version 3.0.

            int context_attribs[] =
            {
                GLX_CONTEXT_MAJOR_VERSION_ARB, 1,
                GLX_CONTEXT_MINOR_VERSION_ARB, 0,
                0
            };

            glx_context = glXCreateContextAttribs(display, x_framebuffer_config, 0, True, context_attribs);
        }
    }

    // Sync to ensure any errors generated are processed.
    XSync(display, false);

    if (!glx_context)
    {
        return false;
    }

    glXMakeCurrent(display, window, glx_context);

    auto *linux_window = (linux::window *) w;
    linux_window->x_display = display;
    linux_window->x_window = window;
    linux_window->x_colormap = x_colormap;

    auto *driver = (gfx::gl::glx_driver *) d;
    driver->glx_context = glx_context;

    glBlendEquation(GL_FUNC_ADD);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);

    return true;
}

bool32 initialize()
{
    glGenFramebuffers = (glGenFramebuffersType *) glXGetProcAddress((GLubyte const *) "glGenFramebuffers");
    glBindFramebuffer = (glBindFramebufferType *) glXGetProcAddress((GLubyte const *) "glBindFramebuffer");
    glFramebufferTexture2D = (glFramebufferTexture2DType *) glXGetProcAddress((GLubyte const *) "glFramebufferTexture2D");
    glBlitFramebuffer = (glBlitFramebufferType *) glXGetProcAddress((GLubyte const *) "glBlitFramebuffer");
    glClearBufferiv = (glClearBufferivType *) glXGetProcAddress((GLubyte const *) "glClearBufferiv");
    // glActiveTexture = (glActiveTextureType *) glXGetProcAddress((GLubyte const *) "glActiveTexture");
    glGenBuffers = (glGenBuffersType *) glXGetProcAddress((GLubyte const *) "glGenBuffers");
    glBindBuffer = (glBindBufferType *) glXGetProcAddress((GLubyte const *) "glBindBuffer");
    glBufferData = (glBufferDataType *) glXGetProcAddress((GLubyte const *) "glBufferData");
    glGenVertexArrays = (glGenVertexArraysType *) glXGetProcAddress((GLubyte const *) "glGenVertexArrays");
    glBindVertexArray = (glBindVertexArrayType *) glXGetProcAddress((GLubyte const *) "glBindVertexArray");
    glVertexAttribPointer = (glVertexAttribPointerType *) glXGetProcAddress((GLubyte const *) "glVertexAttribPointer");
    glEnableVertexAttribArray = (glEnableVertexAttribArrayType *) glXGetProcAddress((GLubyte const *) "glEnableVertexAttribArray");
    glCreateShader = (glCreateShaderType *) glXGetProcAddress((GLubyte const *) "glCreateShader");
    glShaderSource = (glShaderSourceType *) glXGetProcAddress((GLubyte const *) "glShaderSource");
    glCompileShader = (glCompileShaderType *) glXGetProcAddress((GLubyte const *) "glCompileShader");
    glCreateProgram = (glCreateProgramType *) glXGetProcAddress((GLubyte const *) "glCreateProgram");
    glAttachShader = (glAttachShaderType *) glXGetProcAddress((GLubyte const *) "glAttachShader");
    glDetachShader = (glDetachShaderType *) glXGetProcAddress((GLubyte const *) "glDetachShader");
    glLinkProgram = (glLinkProgramType *) glXGetProcAddress((GLubyte const *) "glLinkProgram");
    glUseProgram = (glUseProgramType *) glXGetProcAddress((GLubyte const *) "glUseProgram");
    glGetShaderiv = (glGetShaderivType *) glXGetProcAddress((GLubyte const *) "glGetShaderiv");
    glGetShaderInfoLog = (glGetShaderInfoLogType *) glXGetProcAddress((GLubyte const *) "glGetShaderInfoLog");
    glDeleteShader = (glDeleteShaderType *) glXGetProcAddress((GLubyte const *) "glDeleteShader");
    glValidateProgram = (glValidateProgramType *) glXGetProcAddress((GLubyte const *) "glValidateProgram");
    glGetProgramiv = (glGetProgramivType *) glXGetProcAddress((GLubyte const *) "glGetProgramiv");
    glGetUniformLocation = (glGetUniformLocationType *) glXGetProcAddress((GLubyte const *) "glGetUniformLocation");
    glUniform1i = (glUniform1iType *) glXGetProcAddress((GLubyte const *) "glUniform1i");
    glUniform1f = (glUniform1fType *) glXGetProcAddress((GLubyte const *) "glUniform1f");
    glUniform2f = (glUniform2fType *) glXGetProcAddress((GLubyte const *) "glUniform2f");
    glUniform3f = (glUniform3fType *) glXGetProcAddress((GLubyte const *) "glUniform3f");
    glUniform4f = (glUniform4fType *) glXGetProcAddress((GLubyte const *) "glUniform4f");
    glUniformMatrix4fv = (glUniformMatrix4fvType *) glXGetProcAddress((GLubyte const *) "glUniformMatrix4fv");
    glTexImage2DMultisample = (glTexImage2DMultisampleType *) glXGetProcAddress((GLubyte const *) "glTexImage2DMultisample");

    return true;
}

void vsync(void *w, bool32 turn_on)
{
    auto *linux_window = (linux::window *) w;

    GLXDrawable x_drawable = glXGetCurrentDrawable();
    int interval = 1;
    if (x_drawable)
    {
        glXSwapInterval(linux_window->x_display, x_drawable, interval);
    }
}

void swap_buffers(void *wnd)
{
    auto *linux_window = (linux::window *) wnd;
    glXSwapBuffers(linux_window->x_display, linux_window->x_window);
}


void destroy_window_and_driver(void *window, void *driver)
{
    auto *linux_window = (linux::window *) window;
    auto *x_opengl_driver = (glx_driver *) driver;

    glXMakeCurrent(linux_window->x_display, 0, 0);
    glXDestroyContext(linux_window->x_display, x_opengl_driver->glx_context);

    XDestroyWindow(linux_window->x_display, linux_window->x_window);
    XFreeColormap(linux_window->x_display, linux_window->x_colormap);
    XCloseDisplay(linux_window->x_display);
}


} // namespace gfx::gl

#endif // GFX_GL_X11_HPP
