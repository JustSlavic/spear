#include "opengl_win32.h"


glActiveTextureType *glActiveTexture;
// glGenFramebuffersType *glGenFramebuffers;
// glBindFramebufferType *glBindFramebuffer;
// glFramebufferTexture2DType *glFramebufferTexture2D;
// glBlitFramebufferType *glBlitFramebuffer;
// glClearBufferivType *glClearBufferiv;
glGenBuffersType *glGenBuffers;
glBindBufferType *glBindBuffer;
glBufferDataType *glBufferData;
glGenVertexArraysType *glGenVertexArrays;
glBindVertexArrayType *glBindVertexArray;
glVertexAttribPointerType *glVertexAttribPointer;
glEnableVertexAttribArrayType *glEnableVertexAttribArray;
glCreateShaderType *glCreateShader;
glShaderSourceType *glShaderSource;
glCompileShaderType *glCompileShader;
glCreateProgramType *glCreateProgram;
glAttachShaderType *glAttachShader;
glDetachShaderType *glDetachShader;
glLinkProgramType *glLinkProgram;
glUseProgramType *glUseProgram;
glGetShaderivType *glGetShaderiv;
glGetShaderInfoLogType *glGetShaderInfoLog;
glDeleteShaderType *glDeleteShader;
// glValidateProgramType *glValidateProgram;
// glGetProgramivType *glGetProgramiv;
glGetUniformLocationType *glGetUniformLocation;
glUniform1iType *glUniform1i;
glUniform1fType *glUniform1f;
glUniform2fType *glUniform2f;
glUniform3fType *glUniform3f;
glUniform4fType *glUniform4f;
glUniformMatrix4fvType *glUniformMatrix4fv;
// glTexImage2DMultisampleType *glTexImage2DMultisample;
// glGenRenderbuffersType *glGenRenderbuffers;
// glBindRenderbufferType *glBindRenderbuffer;
// glRenderbufferStorageType *glRenderbufferStorage;
// glFramebufferRenderbufferType *glFramebufferRenderbuffer;
// glCheckFramebufferStatusType *glCheckFramebufferStatus;

bool opengl_init(void)
{
#define WGL_GET_PROC_ADDRESS(NAME) NAME = (NAME##Type *) wglGetProcAddress(STRINGIFY(NAME)); if (NAME == NULL) return false;

    WGL_GET_PROC_ADDRESS(glGenBuffers);
    WGL_GET_PROC_ADDRESS(glBindBuffer);
    WGL_GET_PROC_ADDRESS(glBufferData);
    WGL_GET_PROC_ADDRESS(glGenVertexArrays);
    WGL_GET_PROC_ADDRESS(glBindVertexArray);
    WGL_GET_PROC_ADDRESS(glVertexAttribPointer);
    WGL_GET_PROC_ADDRESS(glEnableVertexAttribArray);
    WGL_GET_PROC_ADDRESS(glCreateShader);
    WGL_GET_PROC_ADDRESS(glShaderSource);
    WGL_GET_PROC_ADDRESS(glCompileShader);
    WGL_GET_PROC_ADDRESS(glCreateProgram);
    WGL_GET_PROC_ADDRESS(glAttachShader);
    WGL_GET_PROC_ADDRESS(glDetachShader);
    WGL_GET_PROC_ADDRESS(glLinkProgram);
    WGL_GET_PROC_ADDRESS(glUseProgram);
    WGL_GET_PROC_ADDRESS(glGetShaderiv);
    WGL_GET_PROC_ADDRESS(glGetShaderInfoLog);
    WGL_GET_PROC_ADDRESS(glDeleteShader);
    WGL_GET_PROC_ADDRESS(glGetUniformLocation);
    WGL_GET_PROC_ADDRESS(glUniform1i);
    WGL_GET_PROC_ADDRESS(glUniform1f);
    WGL_GET_PROC_ADDRESS(glUniform2f);
    WGL_GET_PROC_ADDRESS(glUniform3f);
    WGL_GET_PROC_ADDRESS(glUniform4f);
    WGL_GET_PROC_ADDRESS(glUniformMatrix4fv);
    WGL_GET_PROC_ADDRESS(glActiveTexture);

#undef WGL_GET_PROC_ADDRESS
    return true;
}
