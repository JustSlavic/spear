#ifndef SPEAR_ENGINE_AUDIO_AUDIO_H
#define SPEAR_ENGINE_AUDIO_AUDIO_H

#include <corelibs/base.h>


typedef uint32 spear_audio_handle;
typedef uint32 spear_audio_instance_handle;

typedef struct
{
    void *data;
    uint32 size;
    uint32 samples_per_second;
    uint32 channel_count;
    uint32 bits_per_sample;
} spear_audio_track;

typedef enum
{
    SpearAudioSource_Invalid = 0,
    SpearAudioSource_Generator,
    SpearAudioSource_Buffer,
} spear_audio_source_tag;

typedef struct
{
    spear_audio_source_tag tag;

    // Values needed for buffer sound
    int16 *samples;
    uint32 sample_count;

    // Values needed for generator sound
    double frequency;
    double running_t;
} spear_audio_source;

typedef struct
{
    spear_audio_handle source_handle;
    bool32 enabled;
    bool32 repeat;
    double volume;

    uint32 sample_index;
    double running_t;
} spear_audio_instance;


typedef struct
{
    spear_audio_source sources[16];
    uint32 source_count;
    spear_audio_instance instances[16];
} spear_audio;

typedef struct
{
    int16 *samples;
    uint32 sample_count;
    uint32 samples_per_second;
} spear_sound_output_buffer;

void spear_audio_init(spear_audio *audio);

spear_audio_handle spear_audio_add_tone(spear_audio *audio, double frequency);
spear_audio_handle spear_audio_add_track(spear_audio *audio, int16 *samples, uint32 sample_count);

spear_audio_instance_handle spear_audio_play_once(spear_audio *audio, spear_audio_handle handle);
spear_audio_instance_handle spear_audio_play_loop(spear_audio *audio, spear_audio_handle handle);

spear_audio_source spear_audio_instance_generator_create(double frequency);
spear_audio_source spear_audio_instance_buffer_create(int16 *samples, uint32 sample_count);
spear_audio_instance_handle spear_audio_add(spear_audio *audio, spear_audio_source source);

void spear_audio_mix(spear_audio *audio, spear_sound_output_buffer *output);


#endif // SPEAR_ENGINE_AUDIO_AUDIO_H
