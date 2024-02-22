#ifndef GAME_CAMERA_HPP
#define GAME_CAMERA_HPP

#include <base.h>
#include <vector3.hpp>


namespace game
{


struct camera
{
    vector3 position;
    vector3 forward;
    vector3 up;

    static camera make_at(vector3 p, vector3 f, vector3 u)
    {
        auto s = cross(f, u);

        camera result;
        result.position = p;
        result.forward = f;
        result.up = cross(s, f);;
        return result;
    }

    static camera look_at(vector3 p, vector3 at, vector3 u)
    {
        camera result = make_at(p, at - p, u);
        return result;
    }
};


} // namespace game

#endif // GAME_CAMERA_HPP
