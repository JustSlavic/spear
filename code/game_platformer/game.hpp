#ifndef GAME_STATE_HPP
#define GAME_STATE_HPP

#include <base.h>

#include <array.hpp>
#include <math/integer.hpp>
#include <g2.hpp>
#include <g3.hpp>
#include <math/rectangle2.hpp>
#include <rs/resource_system.hpp>
#include <ui/ui.hpp>
#if UI_EDITOR_ENABLED
#include <ui/ui_editor.hpp>
#endif


// namespace game {


#if DEBUG
struct debug_time_measurement;
extern debug_time_measurement *global_debug_measurements;
extern uint32 global_debug_call_depth;

#define DEBUG_BEGIN_TIME_MEASUREMENT(NAME) \
    uint64 debug_begin_time_measurement_##NAME##__ = DEBUG_CYCLE_COUNT(); \
    (void) 0

#define DEBUG_END_TIME_MEASUREMENT(NAME) \
    uint64 debug_end_time_measurement_##NAME##__ = DEBUG_CYCLE_COUNT(); \
    add_measurement(global_debug_measurements + DEBUG_TIME_SLOT_##NAME, debug_end_time_measurement_##NAME##__ - debug_begin_time_measurement_##NAME##__); \
    (void) 0
#else
#define DEBUG_BEGIN_TIME_MEASUREMENT(NAME)
#define DEBUG_END_TIME_MEASUREMENT(NAME)
#endif


struct entity_id
{
    uint32 index;
};

enum entity_type
{
    ENTITY_INVALID = 0,
    ENTITY_SAM,
    ENTITY_PACKAGE,
    ENTITY_GROUND,
    ENTITY_POSTBOX,
    ENTITY_STONE,
};

enum entity_flags
{
    ENTITY_STATIC       = (1 << 0),
    ENTITY_COLLIDABLE   = (1 << 1),
    ENTITY_INTERACTABLE = (1 << 2),
    ENTITY_ON_GROUND    = (1 << 3),
};

struct entity
{
    entity_type type;
    uint32 flags;

    vector2 position;
    vector2 velocity;
    float32 mass;
    float32 width;
    float32 height;

    bool32 collided;
    bool32 deleted;

};

bool32 is(entity *e, entity_type t) { return e->type == t; }
bool32 is_not(entity *e, entity_type t) { return e->type != t; }

bool32 is(entity *e, entity_flags f) { return (e->flags & f) > 0; }
bool32 is_not(entity *e, entity_flags f) { return (e->flags & f) == 0; }

void set(entity *e, entity_flags f) { e->flags = (e->flags | f); }
void unset(entity *e, entity_flags f) { e->flags = (e->flags & ~f); }

namespace game {

struct camera {
    vector3 position;
};

} // namespace game

struct sam_move
{
    vector2 acceleration;
    vector2 velocity;
    bool32 jump;
};

struct game_state
{
    memory_allocator game_allocator;
    float32 near_exit_time;

    game::camera default_camera;

    entity *sam;
    entity *postbox;
    entity *ground;

    uint32 carried_packages;
    uint32 score;

    float32 blink_time;
    float32 blink_freq;

    resource_token rectangle_mesh;
    resource_token rectangle_shader;

    resource_token rectangle_mesh_uv;
    resource_token rectangle_shader_uv;
    resource_token button_push_1_texture;
    resource_token button_push_2_texture;

    entity *entities;
    usize entities_capacity;
    usize entity_count;

    ui::system *hud;
#if UI_EDITOR_ENABLED
    ui::editor *ui_editor;
    bool32 ui_editor_enabled;
#endif
};


struct entity_ref
{
    entity *e;
    uint32 eid;
};


entity_ref push_entity(game_state *gs);
entity *get_entity(game_state *gs, uint32 eid);


#endif // GAME_STATE_HPP
