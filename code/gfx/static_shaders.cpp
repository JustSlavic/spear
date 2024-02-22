GLOBAL char const *vs_single_color = R"GLSL(
#version 400

layout (location = 0) in vec3 vertex_position;

out vec4 fragment_color;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;
uniform vec4 u_color;

void main()
{
    fragment_color = u_color;
    gl_Position = u_projection * u_view * u_model * vec4(vertex_position, 1.0);;
}
)GLSL";


GLOBAL char const *fs_pass_color = R"GLSL(
#version 400

in vec4 fragment_color;
out vec4 result_color;

void main()
{
    result_color = fragment_color;
}
)GLSL";
