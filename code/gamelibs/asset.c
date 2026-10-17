#include "asset.h"


asset_hash_table_result asset_hash_table_init(asset_hash_table *ht, memory_allocator allocator, uint32 capacity)
{
    ht->ks = ALLOCATE_BUFFER(allocator, sizeof(*ht->ks) * capacity);
    ht->vs = ALLOCATE_BUFFER(allocator, sizeof(&ht->vs) * capacity);
    ht->capacity = capacity;
    ht->count = 0;
    return AssetHashTableResult_Success;
}

asset_hash_table_result asset_hash_table_get(asset_hash_table *ht, uint32 key, uint32 *out_value)
{
    ASSERT_MSG((ht->capacity & (ht->capacity - 1)) == 0, "Capacity of hash table has to be power of 2");

    uint32 hash = hash_uint32(key);
    uint32 offset = 0;
    for (; offset < ht->capacity; offset++)
    {
        uint32 index = (hash + offset) & (ht->capacity - 1);
        if (ht->ks[index] == INVALID_ASSET_HANDLE)
        {
            return AssetHashTableResult_AssetDoesNotExist;
        }
        if (ht->ks[index] == key)
        {
            *out_value = ht->vs[index];
            return AssetHashTableResult_Success;
        }
    }
    return AssetHashTableResult_AssetDoesNotExist;
}

asset_hash_table_result asset_hash_table_insert(asset_hash_table *ht, uint32 key, uint32 value)
{
    ASSERT_MSG((ht->capacity & (ht->capacity - 1)) == 0, "Capacity of hash table has to be power of 2");

    uint32 hash = hash_uint32(key);
    uint32 offset = 0;
    for (; offset < ht->capacity; offset++)
    {
        uint32 index = (hash + offset) & (ht->capacity - 1);
        if (ht->ks[index] == INVALID_ASSET_HANDLE)
        {
            ht->ks[index] = key;
            ht->vs[index] = value;
            ht->count += 1;
            return AssetHashTableResult_Success;
        }
        if (ht->ks[index] == key)
        {
            if (ht->vs[index] == value)
            {
                return AssetHashTableResult_AssetAlreadyExists;
            }
            else
            {
                return AssetHashTableResult_AssetAlreadyExistsInDifferentSlot;
            }
        }
    }

    return AssetHashTableResult_NoMoreFreeSlots;
}

asset_hash_table_result asset_hash_table_remove(asset_hash_table *ht, uint32 key)
{
    ASSERT_MSG((ht->capacity & (ht->capacity - 1)) == 0, "Capacity of hash table has to be power of 2");

    bool32 is_shifting_stage = false;
    uint32 hash = hash_uint32(key);
    uint32 offset = 0;
    for (; offset < ht->capacity; offset++)
    {
        uint32 index = (hash + offset) & (ht->capacity - 1);
        if (ht->ks[index] == INVALID_ASSET_HANDLE)
        {
            if (is_shifting_stage)
            {
                ht->count -= 1;
                return AssetHashTableResult_Success;
            }
            else
            {
                return AssetHashTableResult_AssetDoesNotExist;
            }
        }

        if (ht->ks[index] == key)
        {
            is_shifting_stage = true;
        }

        if (is_shifting_stage)
        {
            uint32 next_index = (hash + offset + 1) & (ht->capacity - 1);

            uint32 next_hash = hash_uint32(ht->ks[next_index]);
            if ((next_hash & (ht->capacity - 1)) == next_index)
            {
                ht->ks[index] = INVALID_ASSET_HANDLE;
                ht->vs[index] = 0xffffffff;
                ht->count -= 1;
                return AssetHashTableResult_Success;
            }
            else
            {
                ht->ks[index] = ht->ks[next_index];
                ht->vs[index] = ht->vs[next_index];
            }
        }
    }

    return AssetHashTableResult_Success;
}
