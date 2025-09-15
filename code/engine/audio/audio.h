#ifndef SPEAR_ENGINE_AUDIO_AUDIO_H
#define SPEAR_ENGINE_AUDIO_AUDIO_H

#include <corelibs/base.h>


typedef struct
{
    uint8 *data;
    uint32 size;
    uint32 index_read;
    uint32 index_write;
} audio_buffer;

typedef struct
{
    double frequency;
    double volume;
    double running_t;
} audio_sine_wave;

void spear_audio_init(audio_buffer *buffer);
void spear_audio_sine_wave_generate(audio_sine_wave *generator,
                                    void *audio_dat,
                                    uint32 audio_size,
                                    uint32 samples_per_second,
                                    uint32 channel_count);


#endif // SPEAR_ENGINE_AUDIO_AUDIO_H
