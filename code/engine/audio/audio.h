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
    // Settings of master audio buffer
    uint32 frames_per_second; // 44100
    uint32 channel_count;     // 2
    uint32 bits_per_sample;   // 16
    double latency; // in seconds
    // Master buffer
    void  *playback_buffer;
    uint32 playback_buffer_size; // in bytes
    uint32 W, R; // Write and Read indices
    // Audio sources
    // spear_audio_source sources[16];
    // uint32 source_count;
} spear_audio;


void spear_audio_init(spear_audio *audio,
                      void  *playback_buffer,
                      uint32 playback_buffer_size,
                      uint32 frames_per_second,
                      uint32 channel_count,
                      uint32 bits_per_sample);

void spear_audio_init_backend(spear_audio *audio);


typedef struct
{
    double frequency;
    double volume;
    double running_t;
} audio_sine_wave;

void spear_audio_sine_wave_generate(audio_sine_wave *generator,
                                    void *audio_data,
                                    uint32 audio_size,
                                    uint32 samples_per_second,
                                    uint32 channel_count);
void *spear_audio_buffer_get(audio_buffer *buffer,
                             uint32 requested_size,
                             uint32 *out_audio_size);
void spear_audio_buffer_read(audio_buffer *buffer,
                             void *audio_data,
                             uint32 audio_size);


#endif // SPEAR_ENGINE_AUDIO_AUDIO_H
