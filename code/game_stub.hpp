#ifndef GAME_STUB_HPP
#define GAME_STUB_HPP

#include <base.h>
#include <time.hpp>
#include <memory_allocator.hpp>

#include "rs/resource_system.hpp"

#include <game/camera.hpp>


struct game_state
{
    memory_allocator allocator;

    game::camera camera;
    float32 camera_speed;

    rs::token rect_mesh;
    rs::token cube_mesh;
    rs::token the_only_shader;

    timepoint exit_press_time;
};


#endif // GAME_STUB_HPP
