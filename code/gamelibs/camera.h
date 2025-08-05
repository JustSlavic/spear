#ifndef _SPEAR_GAMELIBS_CAMERA_H
#define _SPEAR_GAMELIBS_CAMERA_H

#include <corelibs/base.h>
#include <corelibs/math.h>


typedef struct camera {
    vector3 position;
    vector3 forward;
    vector3 up;
} camera;


camera camera_create_at(vector3 position, vector3 forward, vector3 up);
camera camera_create_look_at(vector3 position, vector3 at, vector3 up);


#endif // _SPEAR_GAMELIBS_CAMERA_H
