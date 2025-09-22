#ifndef SPEAR_ENGINE_AUDIO_AUDIO_H
#define SPEAR_ENGINE_AUDIO_AUDIO_H

#include <corelibs/base.h>


typedef struct
{
    void *data;
    uint32 size;
    uint32 frames_per_second;
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

    void  *data;
    uint32 size;
    uint32 R;

    bool32 enabled;
    bool32 repeat;

    double frequency;
    double volume;
    double running_t;
} spear_audio_source;

typedef struct
{
    // Settings of master audio buffer
    uint32 frames_per_second; // 44100
    uint32 channel_count;     // 2
    uint32 bits_per_sample;   // 16
    double latency; // in seconds
    // Master buffer
    void *playback_buffer;
    uint32 playback_buffer_size;
    uint32 W, R; // Write and Read indices
    // Audio sources
    spear_audio_source sources[16];
    uint32 source_count;
} spear_audio;


void spear_audio_init(spear_audio *audio,
                      uint32 frames_per_second,
                      uint32 channel_count,
                      uint32 bits_per_sample,
                      double latency,
                      void *playback_buffer,
                      uint32 playback_buffer_size);
void spear_audio_init_backend(spear_audio *audio);
void spear_audio_update(spear_audio *audio);

int spear_audio_add_source_sine_wave_generator(spear_audio *audio,
                                               double frequency,
                                               double volume);
int spear_audio_add_source_buffer(spear_audio *audio,
                                  void *data,
                                  uint32 size,
                                  bool32 repeat);


#endif // SPEAR_ENGINE_AUDIO_AUDIO_H
