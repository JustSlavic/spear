#ifndef SPEAR_ENGINE_AUDIO_AUDIO_H
#define SPEAR_ENGINE_AUDIO_AUDIO_H

#include <corelibs/base.h>


typedef struct
{
    uint8 *data;
    uint32 size;
    volatile uint32 index_read;
    uint32 index_write;
} audio_buffer;

void spear_audio_init(audio_buffer *buffer);


#endif // SPEAR_ENGINE_AUDIO_AUDIO_H
