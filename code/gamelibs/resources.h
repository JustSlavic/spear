#ifndef SPEAR_ENGINE_RESOURCE_H_
#define SPEAR_ENGINE_RESOURCE_H_

#include <corelibs/base.h>


typedef enum
{
    AssetStatus_Empty,
    AssetStatus_Loading,
    AssetStatus_Ready,
    AssetStatus_Failed,
} asset_status;

typedef struct { uint32 handle; } mesh_handle;
typedef struct { uint32 handle; } sound_handle;
typedef struct { uint32 handle; } shader_handle;
typedef struct { uint32 handle; } texture_handle;


#endif /* SPEAR_ENGINE_RESOURCE_H_ */
