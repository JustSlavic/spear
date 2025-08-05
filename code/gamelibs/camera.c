#include "camera.h"


camera camera_create_at(vector3 p, vector3 f, vector3 u)
{
    vector3 r = v3f_cross(f, u);

    camera result = {};
    result.position = p;
    result.forward = v3f_normalize(f);
    result.up = v3f_normalize(v3f_cross(r, f));

    return result;
}

camera camera_create_look_at(vector3 position, vector3 at, vector3 up)
{
    vector3 direction = v3f_sub(at, position);
    camera result = camera_create_at(position, direction, up);
    return result;
}
