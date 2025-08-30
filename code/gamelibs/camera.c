#include "camera.h"


camera camera_create_at(vector3 p, vector3 f, vector3 u)
{
    vector3 r = vector3_cross(f, u);

    camera result = {};
    result.position = p;
    result.forward = vector3_normalize(f);
    result.up = vector3_normalize(vector3_cross(r, f));

    return result;
}

camera camera_create_look_at(vector3 position, vector3 at, vector3 up)
{
    vector3 direction = vector3_sub(at, position);
    camera result = camera_create_at(position, direction, up);
    return result;
}
