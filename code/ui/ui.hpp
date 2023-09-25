#ifndef UI_HPP
#define UI_HPP

#include <base.h>
#include <memory.h>

#include <array.hpp>
#include <execution_context.hpp>
#include <g2.hpp>
#include <math/transform.hpp>
#include <input.hpp>

//
// @todo
// - ui__id for user saving, that can be turned into internal ui__handle,
//   so the user will preserve correct ids when parts of UI are deleted.
// - Ellipse
// - Gradients
// - Textures
//   - Stretch to size
//   - Original size with transparent background
//   - Repeat
//   - Reflect
//   - Repeat last pixel mode
// - Text
//   - Fonts
//   - Size
//   - Alignment (Left, Right, Center)
// - Filters
//   - Gaussian blur
//   - Glow
//   - Shadow
// - Alpha masks
// - Stencil masks
// - Animation functions
//   - Exp(-x)
//   - Sin/Cos
//   - Arbitrary functions
//   - Bezier curves
// - Render targets
//   - MainUI render target
//   - Additional render targets
// - Blending modes
//   - Add
//   - Multiply
//   - Overlay
// - 9-Scale
// - Bezier curves (Paths)
// - Serializer/deserializer from a text-based file
// - Editor
//   - Moving elements around
//   - Adding/removing/reordering children
//   - Adding/removing filters
//   - Saving/loading from file
//   - Moving in and out from editor mode so you could test the UI right away
//

enum anim_t
{
    UI_ANIM_POSITION_X = 1,
    UI_ANIM_POSITION_Y,
    UI_ANIM_SCALE_X,
    UI_ANIM_SCALE_Y,
    UI_ANIM_ROTATION,
    UI_ANIM_WIDTH,
    UI_ANIM_HEIGHT,
    UI_ANIM_COLOR_R,
    UI_ANIM_COLOR_G,
    UI_ANIM_COLOR_B,
    UI_ANIM_COLOR_A,

    UI_ANIM_NORMAL  = 0x00000000,
    UI_ANIM_LOOP    = 0x01000000,
    UI_ANIM_PPONG   = 0x02000000,

    UI_ANIM_PROPERTY_MASK = 0x00ffffff,
    UI_ANIM_TYPE_MASK     = 0xff000000,
};

enum type_t
{
    UI_NONE = 0,

    UI_ROOT,
    UI_ELEMENT,
    UI_SHAPE,
    UI_IMAGE,
    UI_HOVERABLE,
    UI_CLICKABLE,
    UI_ANIMATION,
};


namespace ui {


struct system;

struct handle
{
    type_t type;
    uint32 index;

    operator bool() const;
};

handle null_handle();
handle make_handle(type_t, uint32 index);
bool32 operator == (handle a, handle b);
bool32 operator != (handle a, handle b);

struct attach;
struct attach_iterator
{
    attach *p;

    attach_iterator& operator++();
    attach_iterator operator++(int);
    bool operator==(attach_iterator other);
    bool operator!=(attach_iterator other);
    handle operator*() const;
    handle operator[](int) const;
    operator bool();
};


typedef void callback(system *, handle);

struct hover_callbacks
{
    callback *on_enter;
    callback *on_leave;

    callback *on_enter_internal;
    callback *on_leave_internal;
};

struct click_callbacks
{
    callback *on_press;
    callback *on_release;

    callback *on_press_internal;
    callback *on_release_internal;
};

attach_iterator begin(attach_iterator it);
attach_iterator end(attach_iterator it);
attach_iterator iterate_attaches(system *s, handle parent);

system *initialize(memory_block ui_memory);
void set_string_id_storage(system *s, string_id_storage *storage);
void set_resource_rectangle_mesh(system *s, rs::resource_token mesh);
void set_resource_rectangle_shader(system *s, rs::resource_token shader);
void set_resource_rectangle_mesh_uv(system *s, rs::resource_token mesh);
void set_resource_rectangle_shader_uv(system *s, rs::resource_token shader);
void update(system *s, input_state *input);
void render(execution_context *context, system *s);

handle make_group(system *s);
handle make_group(system *s, handle parent);
handle make_shape(system *s);
handle make_shape(system *s, handle parent);
handle make_image(system *s);
handle make_image(system *s, handle parent);
hover_callbacks *make_hoverable(system *s, handle owner);
click_callbacks *make_clickable(system *s, handle owner);

bool get_visible(system *s, handle h);

void set_position(system *s, handle h, vector2 position);
void set_scale(system *s, handle h, vector2 scale);
void set_rotation(system *s, handle h, float32 rotation);
void set_color(system *s, handle h, vector4 color);
void set_texture(system *s, handle h, rs::resource_token token);
void set_visible(system *s, handle h, bool is_visible);

void animate(system *s, handle h, string_id id, uint32 a, uint32 duration_frames, float32 start_value, float32 final_value);
void play_animation(system *s, char const *cstr);
void play_animation(system *s, string_id id);
void stop_animation(system *s, char const *cstr);
void stop_animation(system *s, string_id id);


} // namespace ui

#endif // UI_HPP
    
