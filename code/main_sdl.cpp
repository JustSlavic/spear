#include <stdio.h>
#include <SDL2/SDL.h>
#define GL_SILENCE_DEPRECATION
#include <OpenGL/gl3.h>


char const *vertex_shader_source = R"GLSL(
#version 410

layout (location = 0) in vec2 vertex_position;

void main()
{
    gl_Position = vec4(vertex_position, 0.0, 1.0);
}
)GLSL";

char const *fragment_shader_source = R"GLSL(
#version 410

out vec4 result_color;

void main()
{
    result_color = vec4(1.0, 0.0, 1.0, 1.0);
}
)GLSL";


int compile_shader(char const *source_code, int shader_type)
{
    int id = glCreateShader(shader_type);
    glShaderSource(id, 1, (char const **) &source_code, (int const *) NULL);
    glCompileShader(id);

    int successful;
    glGetShaderiv(id, GL_COMPILE_STATUS, &successful);
    if (successful == GL_FALSE)
    {
        int length = 0;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, (int *) &length);

        auto message = malloc(length + 1);
        memset(message, 0, length + 1);

        glGetShaderInfoLog(id, length, &length, (char *) message);
        glDeleteShader(id);

        printf("Could not compile shader: \"%s\"\n", (char *)message);
        free(message);

        return 0;
    }

    return id;
}

int compile_shaders()
{
    int vs = compile_shader(vertex_shader_source, GL_VERTEX_SHADER);
    int fs = compile_shader(fragment_shader_source, GL_FRAGMENT_SHADER);

    int id = glCreateProgram();
    glUseProgram(0);
    glAttachShader(id, vs);
    glAttachShader(id, fs);
    glLinkProgram(id);
    glDetachShader(id, vs);
    glDetachShader(id, fs);

    return id;
}


struct framebuffer
{
    unsigned int framebuffer_id;
    unsigned int color_texture_id;
    unsigned int depth_stencil_id;
};


framebuffer create_framebuffer(int width, int height)
{
    unsigned int texture_id = 0;
    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glBindTexture(GL_TEXTURE_2D, 0);

    unsigned int render_buffer_id = 0;
    glGenRenderbuffers(1, &render_buffer_id);
    glBindRenderbuffer(GL_RENDERBUFFER, render_buffer_id);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    unsigned int id = 0;
    glGenFramebuffers(1, &id);
    glBindFramebuffer(GL_FRAMEBUFFER, id);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture_id, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, render_buffer_id);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    auto completenes_status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    switch (completenes_status)
    {
    case GL_FRAMEBUFFER_UNDEFINED: printf("Specified framebuffer is the default read or draw framebuffer, but the default framebuffer does not exist.\n"); break;
    case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT: printf("Any of the framebuffer attachment points are framebuffer incomplete.\n"); break;
    case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT: printf("The framebuffer does not have at least one image attached to it.\n"); break;
    case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER: printf("The value of GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE is GL_NONE for any color attachment point(s) named by GL_DRAW_BUFFERi.\n"); break;
    case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER: printf("GL_READ_BUFFER is not GL_NONE and the value of GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE is GL_NONE for the color attachment point named by GL_READ_BUFFER.\n"); break;
    case GL_FRAMEBUFFER_UNSUPPORTED: printf("The combination of internal formats of the attached images violates an implementation-dependent set of restrictions.\n"); break;
    case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE: printf("The value of GL_RENDERBUFFER_SAMPLES is not the same for all attached renderbuffers; if the value of GL_TEXTURE_SAMPLES is the not same for all attached textures; or, if the attached images are a mix of renderbuffers and textures, the value of GL_RENDERBUFFER_SAMPLES does not match the value of GL_TEXTURE_SAMPLES.\n");
        printf("    OR\n");
        printf("The value of GL_TEXTURE_FIXED_SAMPLE_LOCATIONS is not the same for all attached textures; or, if the attached images are a mix of renderbuffers and textures, the value of GL_TEXTURE_FIXED_SAMPLE_LOCATIONS is not GL_TRUE for all attached textures.\n"); break;
    case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS: printf("Any framebuffer attachment is layered, and any populated attachment is not layered, or if all populated color attachments are not from textures of the same target.\n"); break;
    case GL_FRAMEBUFFER_COMPLETE: printf("Framebuffer created correctly\n"); break;
    }
    if(completenes_status != GL_FRAMEBUFFER_COMPLETE)
    {
        printf("Error: Framebuffer is not complete!\n");
        return {};
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    framebuffer result;
    result.framebuffer_id = id;
    result.color_texture_id = texture_id;
    result.depth_stencil_id = render_buffer_id;
    return result;
}


int main()
{
    int width = 800;
    int height = 600;

    int err = SDL_Init(SDL_INIT_VIDEO);
    if (err < 0)
    {
        printf("Could not initialize SDL2: \"%s\"\n", SDL_GetError());
        exit(1);
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    auto window = SDL_CreateWindow(
        "window",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        width, height,
        SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
    if (window == NULL)
    {
        printf("Could not create SDL Window: \"%s\"\n", SDL_GetError());
        exit(2);
    }

    auto context = SDL_GL_CreateContext(window);
    if (context == NULL)
    {
        printf("Could not create OpenGL context: \"%s\"\n", SDL_GetError());
        exit(4);
    }

    SDL_GL_MakeCurrent(window, context);

    SDL_version compiled;
    SDL_VERSION(&compiled);
    printf("Compiled against SDL2 v.%d.%d.%d\n", (int)compiled.major, (int)compiled.minor, (int)compiled.patch);

    SDL_version linked;
    SDL_GetVersion(&linked);
    printf("Linked against SDL2 v.%d.%d.%d\n", (int)linked.major, (int)linked.minor, (int)linked.patch);

    printf("glGetString(GL_SHADING_LANGUAGE_VERSION) -> %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

    GLubyte const *v = glGetString(GL_VERSION);
    printf("glGetString(GL_VERSION) -> %s\n", v);

    int shader_program = compile_shaders();

    GLuint vertex_array_id = 0;
    GLuint vertex_buffer_id = 0;
    GLuint index_buffer_id = 0;

    // Rectangle mesh
    {
        float vbo_data[] = {
            -1.0f, -1.0f,
             1.0f, -1.0f,
             0.0f,  1.0f,
        };

        int ibo_data[] = {
            0, 1, 2, // first triangle
        };

        glGenBuffers(1, &vertex_buffer_id);
        printf("vertex_buffer_id = %d\n", vertex_buffer_id);
        glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_id);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vbo_data), vbo_data, GL_STATIC_DRAW);

        glGenBuffers(1, &index_buffer_id);
        printf("index_buffer_id = %d\n", index_buffer_id);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_id);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ibo_data), ibo_data, GL_STATIC_DRAW);

        glGenVertexArrays(1, &vertex_array_id);
        printf("vertex_array_id = %d\n", vertex_array_id);
        glBindVertexArray(vertex_array_id);
        glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_id);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(
                0,                 // Index
                2,                 // Count
                GL_FLOAT,          // Type
                GL_FALSE,          // Normalized?
                2*sizeof(float),   // Stride
                (void *) 0);       // Offset
    }

    framebuffer render_target = create_framebuffer(width, height);
    glBindFramebuffer(GL_FRAMEBUFFER, render_target.framebuffer_id);
    glClearColor(1.f, 1.f, 0.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    err = glGetError();
    switch (err) {
        case 0: printf("NO ERROR\n"); break;

        case GL_INVALID_ENUM: printf("GL_INVALID_ENUM\n"); return -1;
        case GL_INVALID_VALUE: printf("GL_INVALID_VALUE\n"); return -1;
        case GL_INVALID_OPERATION: printf("GL_INVALID_OPERATION\n"); return -1;
        case GL_INVALID_FRAMEBUFFER_OPERATION: printf("GL_INVALID_FRAMEBUFFER_OPERATION\n"); return -1;
        case GL_OUT_OF_MEMORY: printf("GL_OUT_OF_MEMORY\n"); return -1;
        // case GL_STACK_UNDERFLOW: printf("GL_STACK_UNDERFLOW\n"); return -1;
        // case GL_STACK_OVERFLOW: printf("GL_STACK_OVERFLOW\n"); return -1;
        default: printf("Unknown error!\n"); return -1;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    bool running = true;
    while (running)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event) != 0)
        {
            switch (event.type)
            {
                case SDL_QUIT:
                    running = false;
                break;

                case SDL_KEYUP:
                case SDL_KEYDOWN:
                {
                    switch (event.key.keysym.scancode)
                    {
                        case SDL_SCANCODE_ESCAPE:
                            break;

                        default:
                            break;
                    }
                }
                break;
            }
        }

        glUseProgram(shader_program);
        glBindVertexArray(vertex_array_id);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_id);

        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, NULL);

        SDL_GL_SwapWindow(window);
    }


    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);

    SDL_Quit();
    return 0;
}
