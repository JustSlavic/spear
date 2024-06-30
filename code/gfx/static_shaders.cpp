GLOBAL char const *vs_single_color = R"GLSL(
#version 410

layout (location = 0) in vec3 vertex_position;

out vec4 fragment_color;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;
uniform vec4 u_color;

void main()
{
    fragment_color = u_color;
    gl_Position = u_projection * u_view * u_model * vec4(vertex_position, 1.0);
}
)GLSL";

GLOBAL char const *fs_pass_color = R"GLSL(
#version 410

in vec4 fragment_color;
out vec4 result_color;

void main()
{
    result_color = fragment_color;
}
)GLSL";

GLOBAL char const *vs_ground = R"GLSL(
#version 410

layout (location = 0) in vec3 vertex_position;

out vec4 fragment_color;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;
uniform vec4 u_color;

void main()
{
    vec4 world_pos = u_model * vec4(vertex_position, 1.0);
    float z = clamp(world_pos.z + 0.5, 0.0, 1.0);

    fragment_color = u_color * z;
    gl_Position = u_projection * u_view * u_model * vec4(vertex_position, 1.0);
}
)GLSL";

GLOBAL char const *vs_framebuffer = R"GLSL(
#version 410

layout (location = 0) in vec3 vertex_position;

out vec2 uv_coordinates;

void main()
{
    // NDC -> UV
    // (-1,  1) -> (0, 1);    (1,  1) -> (0, 1);
    // (-1, -1) -> (0, 0);    (1, -1) -> (1, 0);
    uv_coordinates = vertex_position.xy * 0.5f + vec2(0.5f);
    gl_Position = vec4(vertex_position, 1.0);
}
)GLSL";

GLOBAL char const *fs_framebuffer = R"GLSL(
#version 410

in vec2 uv_coordinates;
out vec4 result_color;

uniform sampler2D u_framebuffer;

void main()
{
    result_color = texture(u_framebuffer, uv_coordinates);
}
)GLSL";

GLOBAL char const *vs_text = R"GLSL(
#version 410
layout (location = 0) in vec2 ui_coordinates;
layout (location = 1) in vec2 uv_coordinates;

out vec2 uv;

uniform mat4 u_model;
uniform mat4 u_projection;

void main()
{
    uv = uv_coordinates;
    gl_Position = u_projection * u_model * vec4(ui_coordinates, 0., 1.);
}
)GLSL";

GLOBAL char const *fs_text = R"GLSL(
#version 410

in vec2 uv;
out vec4 result_color;

uniform vec4 u_color;
uniform sampler2D u_texture0;

void main()
{
    result_color = u_color * texture(u_texture0, uv);
}
)GLSL";
