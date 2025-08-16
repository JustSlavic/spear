static char const *vs_single_color =
"#version 410\n"
GLSL(
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
);

static char const *fs_pass_color =
"#version 410\n"
GLSL(
    in vec4 fragment_color;
    out vec4 result_color;

    void main()
    {
        result_color = fragment_color;
    }
);

static char const *vs_ground =
"#version 410\n"
GLSL(
    layout (location = 0) in vec3 vertex_position;

    out vec4 fragment_color;

    uniform mat4 u_model;
    uniform mat4 u_view;
    uniform mat4 u_projection;
    uniform vec4 u_color;

    void main()
    {
        float z = clamp(vertex_position.z + 0.5, 0.0, 1.0);

        fragment_color = u_color * z;
        gl_Position = u_projection * u_view * u_model * vec4(vertex_position, 1.0);
    }
);

static char const *vs_framebuffer =
"#version 410\n"
GLSL(
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
);

static char const *fs_framebuffer =
"#version 410\n"
GLSL(
    in vec2 uv_coordinates;
    out vec4 result_color;

    uniform sampler2D u_framebuffer;

    void main()
    {
        result_color = texture(u_framebuffer, uv_coordinates);
    }
);

static char const *vs_text =
"#version 410\n"
GLSL(
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
);

static char const *fs_text =
"#version 410\n"
GLSL(
    in vec2 uv;
    out vec4 result_color;

    uniform vec4 u_color;
    uniform sampler2D u_texture0;

    void main()
    {
        vec4 tex_color = texture(u_texture0, uv);
        result_color = u_color * tex_color;
    }
);

static char const *vs_phong =
"#version 410\n"
GLSL(
    layout (location = 0) in vec3 vertex_position;
    layout (location = 1) in vec3 vertex_normal;

    out vec4 fragment_color;
    out vec3 fragment_position;
    out vec3 fragment_normal;

    uniform mat4 u_model;
    uniform mat4 u_view;
    uniform mat4 u_projection;
    uniform vec4 u_color;

    void main()
    {
        fragment_color = u_color;
        fragment_position = (u_model * vec4(vertex_position, 1.0)).xyz;
        fragment_normal = mat3(transpose(inverse(u_model))) * vertex_normal;
        gl_Position = u_projection * u_view * u_model * vec4(vertex_position, 1.0);
    }
);

static char const *fs_phong =
"#version 410\n"
GLSL(
    in vec4 fragment_color;
    in vec3 fragment_position;
    in vec3 fragment_normal;
    out vec4 result_color;

    void main()
    {
        vec3 light_position = vec3(0.f, 0.f, 0.f);
        vec3 light_direction = normalize(light_position - fragment_position);

        float ambient_light = 0.f;
        vec3 ambient_color = ambient_light * fragment_color.rgb;

        float diffuse_light = max(dot(normalize(fragment_normal), light_direction), 0.0);
        vec3 diffuse_color = diffuse_light * fragment_color.rgb;

        result_color = vec4(pow(ambient_color + diffuse_color, vec3(1/2.2)), fragment_color.a);
    }
);

static char const *vs_sun =
"#version 410\n"
GLSL(
    layout (location = 0) in vec3 vertex_position;
    layout (location = 1) in vec3 vertex_normal;

    out vec3 fragment_position;
    out vec3 fragment_normal;

    uniform mat4 u_model;
    uniform mat4 u_view;
    uniform mat4 u_projection;

    void main()
    {
        fragment_position = (u_model * vec4(vertex_position, 1.0)).xyz;
        fragment_normal = mat3(transpose(inverse(u_model))) * vertex_normal;
        gl_Position = u_projection * u_view * u_model * vec4(vertex_position, 1.0);
    }
);

static char const *fs_sun =
"#version 410\n"
GLSL(
    in vec3 fragment_position;
    in vec3 fragment_normal;

    out vec4 result_color;

    void main()
    {
        vec3 sun_color = vec3(1.0, 1.0, 1.0);
        result_color = vec4(sun_color, 1.0);
    }
);

static char const *vs_frame =
"#version 400\n"
GLSL(
    layout (location = 0) in vec2 vertex_position;
    layout (location = 1) in vec2 vertex_displacement_weight;

    out vec4 fragment_color;

    uniform mat4 u_model;
    uniform mat4 u_view;
    uniform mat4 u_projection;
    uniform vec4 u_color;

    // Use negative sign for inner border
    // and positive sign for outer border
    uniform float u_width;
    uniform float u_height;

    void main()
    {
        vec4 p = u_projection * u_view * u_model * vec4(vertex_position, 0.0, 1.0);
        vec4 d = u_projection * vec4(vertex_displacement_weight, 0.0, 0.0);
        fragment_color = u_color;

        vec4 displacement = vec4(d.x * u_width, d.y * u_height, 0.f, 0.f);
        // Add border width to the vertices that need to move
        // We should subtract the displacement to follow the rule written above
        p -= displacement;
        gl_Position = p;
    }
);
