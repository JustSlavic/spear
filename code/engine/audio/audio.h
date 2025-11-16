#ifndef SPEAR_ENGINE_AUDIO_AUDIO_H
#define SPEAR_ENGINE_AUDIO_AUDIO_H

#include <corelibs/base.h>


typedef struct
{
    void *data;
    uint32 size;
    uint32 samples_per_second;
    uint32 channel_count;
    uint32 bits_per_sample;
} spear_audio_buffer;

typedef enum
{
    SpearAudioSource_Invalid = 0,
    SpearAudioSource_Generator,
    SpearAudioSource_Buffer,
} spear_audio_source_tag;

typedef struct
{
    spear_audio_source_tag tag;

    // Settings set on audio source
    bool32 enabled;
    bool32 repeat;
    double volume;

    // Values needed for buffer sound
    int16 *samples;
    uint32 sample_count;
    uint32 sample_index;

    // Values needed for generator sound
    double frequency;
    double running_t;
} spear_audio_source;

typedef struct
{
    spear_audio_source sources[16];
    uint32 source_count;
} spear_audio;

typedef struct
{
    int16 *samples;
    uint32 sample_count;
    uint32 samples_per_second;
} spear_sound_output_buffer;

spear_audio_source spear_audio_source_generator_create(double frequency);
spear_audio_source spear_audio_source_buffer_create(int16 *samples, uint32 sample_count);
void spear_audio_add(spear_audio *audio, spear_audio_source source);
void spaer_audio_mix(spear_audio *audio, spear_sound_output_buffer *output);

#endif // SPEAR_ENGINE_AUDIO_AUDIO_H
