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

void vsync(bool32 turn_on)
{

}

} // namespace gfx::gl
