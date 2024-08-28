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
