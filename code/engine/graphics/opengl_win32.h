#ifndef SPEAR_ENGINE_GRAPHICS_OPENGL_WIN32_H_
#define SPEAR_ENGINE_GRAPHICS_OPENGL_WIN32_H_

/* ClearBufferMask */
#define GL_DEPTH_BUFFER_BIT                             0x00000100
#define GL_STENCIL_BUFFER_BIT                           0x00000400
#define GL_COLOR_BUFFER_BIT                             0x00004000

/* Boolean */
#define GL_FALSE 0
#define GL_TRUE  1

/* DataType */
#define GL_FLOAT                                        0x1406

#define GL_INVALID_FRAMEBUFFER_OPERATION                0x0506
#define GL_UNSIGNED_INT_8_8_8_8                         0x8035
#define GL_MULTISAMPLE                                  0x809D
#define GL_MULTISAMPLE_ARB                              0x809D
#define GL_SAMPLE_ALPHA_TO_COVERAGE                     0x809E
#define GL_SAMPLE_ALPHA_TO_ONE                          0x809F
#define GL_SAMPLES                                      0x80A9
#define GL_SAMPLES_ARB                                  0x80A9
#define GL_SAMPLES_EXT                                  0x80A9
#define GL_BGR                                          0x80E0
#define GL_BGRA                                         0x80E1
#define GL_CLAMP_TO_BORDER                              0x812D
#define GL_CLAMP_TO_EDGE                                0x812F
#define GL_FRAMEBUFFER_UNDEFINED                        0x8219
#define GL_MAJOR_VERSION                                0x821B
#define GL_MINOR_VERSION                                0x821C
#define GL_DEPTH_STENCIL_ATTACHMENT                     0x821A
#define GL_TEXTURE0                                     0x84C0
#define GL_TEXTURE1                                     0x84C1
#define GL_TEXTURE2                                     0x84C2
#define GL_TEXTURE3                                     0x84C3
#define GL_DEPTH_STENCIL                                0x84F9
#define GL_UNSIGNED_INT_24_8                            0x84FA
#define GL_ARRAY_BUFFER                                 0x8892
#define GL_ELEMENT_ARRAY_BUFFER                         0x8893
#define GL_STATIC_DRAW                                  0x88E4
#define GL_DEPTH24_STENCIL8                             0x88F0
#define GL_FRAGMENT_SHADER                              0x8B30
#define GL_VERTEX_SHADER                                0x8B31
#define GL_COMPILE_STATUS                               0x8B81
#define GL_VALIDATE_STATUS                              0x8B83
#define GL_INFO_LOG_LENGTH                              0x8B84
#define GL_READ_FRAMEBUFFER                             0x8CA8
#define GL_DRAW_FRAMEBUFFER                             0x8CA9
#define GL_FRAMEBUFFER_COMPLETE                         0x8CD5
#define GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT            0x8CD6
#define GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT    0x8CD7
#define GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER           0x8CDB
#define GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER           0x8CDC
#define GL_FRAMEBUFFER_UNSUPPORTED                      0x8CDD
#define GL_COLOR_ATTACHMENT0                            0x8CE0
#define GL_DEPTH_ATTACHMENT                             0x8D00
#define GL_STENCIL_ATTACHMENT                           0x8D20
#define GL_FRAMEBUFFER                                  0x8D40
#define GL_RENDERBUFFER                                 0x8D41
#define GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE           0x8D56
#define GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS         0x8DA8
#define GL_TEXTURE_SWIZZLE_R                            0x8E42
#define GL_TEXTURE_SWIZZLE_G                            0x8E43
#define GL_TEXTURE_SWIZZLE_B                            0x8E44
#define GL_TEXTURE_SWIZZLE_A                            0x8E45
#define GL_TEXTURE_SWIZZLE_RGBA                         0x8E46
#define GL_TEXTURE_2D_MULTISAMPLE                       0x9100

// typedef uint8  GLboolean;
// typedef uint32 GLenum;
// typedef uint32 GLbitfield;
// typedef float32 GLclampf;

// typedef void glActiveTextureType(GLenum texture);
// typedef void glGenFramebuffersType(isize n, uint32 *ids);
// typedef void glBindFramebufferType(GLenum target, uint32 framebuffer);
// typedef void glFramebufferTexture2DType(GLenum target, GLenum attachment, GLenum textarget, uint32 texture, int32 level);
// typedef void glBlitFramebufferType(int32 srcX0, int32 srcY0, int32 srcX1, int32 srcY1, int32 dstX0, int32 dstY0, int32 dstX1, int32 dstY1, GLbitfield mask, GLenum filter);
// typedef void glClearBufferivType(GLenum buffer, int32 drawbuffer, int32 const *value);
typedef void glGenBuffersType(isize n, uint32 *buffers);
typedef void glBindBufferType(GLenum target, uint32 buffer);
typedef void glBufferDataType(GLenum target, intptr size, const void *data, GLenum usage);
typedef void glGenVertexArraysType(isize n, uint32 *arrays);
typedef void glBindVertexArrayType(uint32 array);
typedef void glVertexAttribPointerType(uint32 index, int32 size, GLenum type, GLboolean normalized, isize stride, const void *pointer);
typedef void glEnableVertexAttribArrayType(uint32 index);
typedef uint32 glCreateShaderType(GLenum shaderType);
typedef void glShaderSourceType(uint32 shader, isize count, char const **string, int32 const *length);
typedef void glCompileShaderType(uint32 shader);
typedef uint32 glCreateProgramType(void);
typedef void glAttachShaderType(uint32 program, uint32 shader);
typedef void glDetachShaderType(uint32 program, uint32 shader);
typedef void glLinkProgramType(uint32 program);
typedef void glUseProgramType(uint32 program);
typedef void glGetShaderivType(uint32 shader, GLenum pname, int32 *params);
typedef void glGetShaderInfoLogType(uint32 shader, GLsizei maxLength, GLsizei *length, char *infoLog); // GLsizei is int on windows and int64 on Mac?
typedef void glDeleteShaderType(uint32 shader);
// typedef void glValidateProgramType(uint32 program);
// typedef void glGetProgramivType(uint32 program, GLenum pname, int32 *params);
typedef int32 glGetUniformLocationType(uint32 program, char const *uniform_name);
typedef void glUniform1iType(int32 location, int32 v0);
typedef void glUniform1fType(int32 location, float32 v0);
typedef void glUniform2fType(int32 location, float32 v0, float32 v1);
typedef void glUniform3fType(int32 location, float32 v0, float32 v1, float32 v2);
typedef void glUniform4fType(int32 location, float32 v0, float32 v1, float32 v2, float32 v3);
typedef void glUniformMatrix4fvType(int32 location, size_t count, bool transpose, float32 const *value);
// typedef void glTexImage2DMultisampleType(GLenum target, isize samples, GLenum internalformat, isize width, isize height, GLboolean fixedsamplelocations);
// typedef void glGenRenderbuffersType(isize n, uint32 *renderbuffers);
// typedef void glBindRenderbufferType(GLenum target, uint32 renderbuffer);
// typedef void glRenderbufferStorageType(GLenum target, GLenum internalformat, isize width, isize height);
// typedef void glFramebufferRenderbufferType(GLenum target, GLenum attachment, GLenum renderbuffertarget, uint32 renderbuffer);
// typedef GLenum glCheckFramebufferStatusType(GLenum target);

// extern glActiveTextureType *glActiveTexture;
// extern glGenFramebuffersType *glGenFramebuffers;
// extern glBindFramebufferType *glBindFramebuffer;
// extern glFramebufferTexture2DType *glFramebufferTexture2D;
// extern glBlitFramebufferType *glBlitFramebuffer;
// extern glClearBufferivType *glClearBufferiv;
extern glGenBuffersType *glGenBuffers;
extern glBindBufferType *glBindBuffer;
extern glBufferDataType *glBufferData;
extern glGenVertexArraysType *glGenVertexArrays;
extern glBindVertexArrayType *glBindVertexArray;
extern glVertexAttribPointerType *glVertexAttribPointer;
extern glEnableVertexAttribArrayType *glEnableVertexAttribArray;
extern glCreateShaderType *glCreateShader;
extern glShaderSourceType *glShaderSource;
extern glCompileShaderType *glCompileShader;
extern glCreateProgramType *glCreateProgram;
extern glAttachShaderType *glAttachShader;
extern glDetachShaderType *glDetachShader;
extern glLinkProgramType *glLinkProgram;
extern glUseProgramType *glUseProgram;
extern glGetShaderivType *glGetShaderiv;
extern glGetShaderInfoLogType *glGetShaderInfoLog;
extern glDeleteShaderType *glDeleteShader;
// extern glValidateProgramType *glValidateProgram;
// extern glGetProgramivType *glGetProgramiv;
extern glGetUniformLocationType *glGetUniformLocation;
extern glUniform1iType *glUniform1i;
extern glUniform1fType *glUniform1f;
extern glUniform2fType *glUniform2f;
extern glUniform3fType *glUniform3f;
extern glUniform4fType *glUniform4f;
extern glUniformMatrix4fvType *glUniformMatrix4fv;
// extern glTexImage2DMultisampleType *glTexImage2DMultisample;
// extern glGenRenderbuffersType *glGenRenderbuffers;
// extern glBindRenderbufferType *glBindRenderbuffer;
// extern glRenderbufferStorageType *glRenderbufferStorage;
// extern glFramebufferRenderbufferType *glFramebufferRenderbuffer;
// extern glCheckFramebufferStatusType *glCheckFramebufferStatus;

bool opengl_init(void);


#endif // SPEAR_ENGINE_GRAPHICS_OPENGL_WIN32_H_
