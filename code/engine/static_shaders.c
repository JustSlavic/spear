static char const *vs_single_color =
GLSL_VERSION
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
GLSL_VERSION
GLSL(
    in vec4 fragment_color;
    out vec4 result_color;

    void main()
    {
        result_color = fragment_color;
    }
);

static char const *vs_textured =
GLSL_VERSION
GLSL(
    layout (location = 0) in vec3 vertex_position;
    layout (location = 1) in vec2 uv_coordinates;

    out vec2 uv;

    uniform bool is_upside_down;
    uniform mat4 u_model;
    uniform mat4 u_view;
    uniform mat4 u_projection;

    void main()
    {
        uv = uv_coordinates;
        if (is_upside_down)
            uv.y = 1.0 - uv.y;
        gl_Position = u_projection * u_view * u_model * vec4(vertex_position, 1.0);
    }
);

static char const *fs_textured =
GLSL_VERSION
GLSL(
    in vec2 uv;
    out vec4 result_color;

    uniform sampler2D u_texture0;

    void main()
    {
        vec4 tex_color = texture(u_texture0, uv);
        result_color = tex_color;
    }
);

static char const *vs_framebuffer =
GLSL_VERSION
GLSL(
    layout (location = 0) in vec3 vertex_position;
    layout (location = 1) in vec2 vertex_uv;

    out vec2 uv_coordinates;

    void main()
    {
        uv_coordinates = vertex_uv;
        gl_Position = vec4(vertex_position, 1.0);
    }
);

static char const *fs_framebuffer =
GLSL_VERSION
GLSL(
    in vec2 uv_coordinates;
    out vec4 result_color;

    uniform sampler2D u_framebuffer;
    uniform bool u_is_depth;

    void main()
    {
        if (u_is_depth)
        {
            float depth = texture(u_framebuffer, uv_coordinates).r;
            result_color = vec4(vec3(depth), 1.f);
        }
        else
        {
            result_color = texture(u_framebuffer, uv_coordinates);
        }
    }
);

static char const *vs_text =
GLSL_VERSION
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
GLSL_VERSION
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
GLSL_VERSION
GLSL(
    layout (location = 0) in vec3 vertex_position;
    layout (location = 1) in vec3 vertex_normal;

    out vec4 fragment_color;
    out vec3 fragment_position;
    out vec3 fragment_normal;
    out vec4 fragment_position_in_light_space;

    uniform mat4 u_model;
    uniform mat4 u_view;
    uniform mat4 u_projection;
    uniform vec4 u_color;

    uniform mat4 u_light_matrix;

    void main()
    {
        fragment_color = u_color;
        fragment_position = (u_model * vec4(vertex_position, 1.0)).xyz;
        fragment_normal = mat3(transpose(inverse(u_model))) * vertex_normal;
        fragment_position_in_light_space = u_light_matrix * u_model * vec4(vertex_position, 1.0);
        gl_Position = u_projection * u_view * u_model * vec4(vertex_position, 1.0);
    }
);

static char const *fs_phong =
GLSL_VERSION
GLSL(
    #define SHADOWS_USE_PCF true

    in vec4 fragment_color;
    in vec3 fragment_position;
    in vec3 fragment_normal;
    in vec4 fragment_position_in_light_space;
    out vec4 result_color;

    uniform sampler2D u_depth_map;
    uniform vec3 u_light_position;

    float shadow_factor()
    {
        vec3 ndc_position = fragment_position_in_light_space.xyz / fragment_position_in_light_space.w;
        vec3 uv_position = ndc_position * 0.5f + 0.5f;

#if SHADOWS_USE_PCF
        vec2 texel_size = 1.f / textureSize(u_depth_map, 0);
        float average_shadow = 0.f;
        for (int x = -1; x <= 1; x++)
        {
            for (int y = -1; y <= 1; y++)
            {
                float closest_distance = texture(u_depth_map, uv_position.xy + vec2(x, y) * texel_size).r;
                average_shadow += uv_position.z > closest_distance ? 0.f : 1.f;
            }
        }
        average_shadow /= 9.f;
        return average_shadow;
#else
        float closest_distance = texture(u_depth_map, uv_position.xy).r;
        return (uv_position.z > closest_distance) ? 0.0 : 1.0;
#endif
    }

    void main()
    {
        float light_strength = 1.0f;
        vec3 light_direction = normalize(u_light_position - fragment_position);

        float ambient_light = 0.25f;
        vec3 ambient_color = ambient_light * fragment_color.rgb;

        float diffuse_light = light_strength * max(dot(normalize(fragment_normal), light_direction), 0.0);
        vec3 diffuse_color = diffuse_light * fragment_color.rgb;

        // Temporarily disable the gamma correction, because colors look washed out because of that.
        // @todo: research more about it, why it is needed, when it is needed, how to do it correctly.
        result_color = vec4(ambient_color + shadow_factor() * diffuse_color, fragment_color.a); // vec4(pow(ambient_color + diffuse_color, vec3(1/2.2)), fragment_color.a);
    }
);

static char const *vs_sun =
GLSL_VERSION
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
GLSL_VERSION
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
GLSL_VERSION
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

static char const *vs_depth_map =
GLSL_VERSION
GLSL(
    layout (location = 0) in vec3 vertex_position;

    uniform mat4 u_model;
    uniform mat4 u_view;
    uniform mat4 u_projection;

    void main()
    {
        gl_Position = u_projection * u_view * u_model * vec4(vertex_position, 1.f);
    }
);

static char const *fs_depth_map =
GLSL_VERSION
GLSL(
    void main()
    {
        // gl_FragDepth = gl_FragCoord.z;
    }
);
