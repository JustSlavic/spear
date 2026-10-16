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

typedef uint32 asset_handle;
#define INVALID_ASSET_HANDLE 0

typedef struct {
    uint32 *ks;
    uint32 *vs;
    uint32 capacity; /* Has to be power of 2 */
    uint32 count;    /* Track how many elements are occupied to grow hash table in time */
} asset_hash_table;

typedef enum {
    AssetHashTableResult_Success = 0,
    AssetHashTableResult_NoMoreFreeSlots = -1,
    AssetHashTableResult_AssetAlreadyExists = -2,
    AssetHashTableResult_AssetAlreadyExistsInDifferentSlot = -3,
    AssetHashTableResult_AssetDoesNotExist = -4,
} asset_hash_table_result;

asset_hash_table_result asset_hash_table_init(asset_hash_table *ht, memory_allocator alloc, uint32 capacity);
asset_hash_table_result asset_hash_table_get(asset_hash_table *ht, uint32 handle, uint32 *value);
asset_hash_table_result asset_hash_table_insert(asset_hash_table *ht, uint32 handle, uint32 value);
asset_hash_table_result asset_hash_table_remove(asset_hash_table *ht, uint32 handle);

typedef struct { uint32 handle; } mesh_handle;
typedef struct { uint32 handle; } sound_handle;
typedef struct { uint32 handle; } shader_handle;
typedef struct { uint32 handle; } texture_handle;


#endif /* SPEAR_ENGINE_RESOURCE_H_ */
