#ifndef GAME_STATE_HPP
#define GAME_STATE_HPP

#include <base.hpp>

#include <array.hpp>
#include <math/integer.hpp>
#include <math/vector2.hpp>
#include <math/vector3.hpp>
#include <math/rectangle2.hpp>
#include <rs/resource_system.hpp>

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
    ENTITY_CIRCLE,
    ENTITY_ALIGNED_RECTANGLE,
    ENTITY_ORIENTED_RECTANGLE,
};

struct entity
{
    entity_type type;

    math::vector2 position;
    math::vector2 velocity;
    union
    {
        float32 radius;
        float32 width;
    };
    float32 height;
    float32 mass;
    float32 rotation;
    float32 rotational_velocity;

    math::rectangle2 aabb;
    bool32 collided;
    bool32 deleted;
};

struct world_chunk
{
    int32 x, y;
    uint32 entities[16];
    uint32 entity_count;
    world_chunk *next;
};

struct world
{
    float32 chunk_width;
    float32 chunk_height;

    world_chunk *free_list;

    world_chunk *hash_table[1024];
};

uint32 chunk_hash(uint32 chunk_x, uint32 chunk_y);
void get_chunk_coordinates(world *w, math::vector2 p, int32 *chunk_x, int32 *chunk_y);


namespace game {

struct camera {
    math::vector3 position;
};

} // namespace game


struct game_state
{
    memory::allocator game_allocator;

    world w;

    array<game::camera> cameras;
    uint32 current_camera_index;

    rs::resource_token rectangle_mesh;
    rs::resource_token rectangle_shader;
    rs::resource_token circle_shader;

    entity *entities;
    usize entities_capacity;
    usize entity_count;

    double energy;
    double energy_last_frame;
};


game::camera *get_current_camera(game_state *gs);


struct entity_ref
{
    entity *e;
    uint32 eid;
};


entity_ref push_entity(game_state *gs);
entity *get_entity(game_state *gs, uint32 eid);
world_chunk *get_new_world_chunk(game_state *gs, world *w);
void release_world_chunk(world *w, world_chunk *chunk);
world_chunk **get_world_chunk_slot(game_state *gs, world *w, int32 chunk_x, int32 chunk_y, bool32 create_slot = false);
void push_entity_in_world_chunk_slot(game_state *gs, world *w, world_chunk **slot, uint32 eid, int32 chunk_x, int32 chunk_y);
void remove_entity_from_world_chunk_slot(game_state *gs, world *w, world_chunk **slot, uint32 eid, int32 chunk_x, int32 chunk_y);
void put_entity_in_chunk(game_state *gs, world *w, entity_ref ref);
void move_entity_between_chunks(game_state *gs, world *w, uint32 eid, math::rect2 old_aabb, math::rect2 new_aabb);


// } // namespace game

#endif // GAME_STATE_HPP
