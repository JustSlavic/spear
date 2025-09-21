#include "audio.h"

void
spear_audio_init(spear_audio *audio,
                     void  *playback_buffer,
                     uint32 playback_buffer_size,
                     uint32 frames_per_second,
                     uint32 channel_count,
                     uint32 bits_per_sample)
{
    audio->frames_per_second = frames_per_second;
    audio->channel_count = channel_count;
    audio->bits_per_sample = bits_per_sample;
    audio->latency = 1.0 / 30.0;
    audio->playback_buffer = playback_buffer;
    audio->playback_buffer_size = playback_buffer_size;
    audio->W = 0;
    audio->R = 0;
    // audio->source_count = 0;

    spear_audio_init_backend(audio);
}

void
spear_audio_sine_wave_generate(audio_sine_wave *generator,
                               void *audio_data,
                               uint32 audio_size,
                               uint32 samples_per_second,
                               uint32 channel_count)
{
    uint32 frame_index;
    uint32 frame_size = channel_count * sizeof(sound_sample_t);
    uint32 frame_count = audio_size / frame_size;

    int16 *samples = (int16 *) audio_data;
    uint32 samples_per_period = samples_per_second / generator->frequency;

    for (frame_index = 0; frame_index < frame_count; frame_index++)
    {
        double sine_value = sin(generator->running_t);
        int16 sample_value = (int16)(sine_value * generator->volume);

        *samples++ = sample_value;
        *samples++ = sample_value;

        generator->running_t += TWO_PI / samples_per_period;
        if (generator->running_t > TWO_PI)
            generator->running_t = generator->running_t - TWO_PI;
    }
}

void *spear_audio_buffer_get(audio_buffer *buffer,
                             uint32 requested_size,
                             uint32 *out_audio_size)
{
    if (buffer->index_read + requested_size > buffer->size)
    {
        requested_size = buffer->size - buffer->index_read;
    }
    *out_audio_size = requested_size;
    void *result = buffer->data + buffer->index_read;
    return result;
}

void
spear_audio_buffer_read(audio_buffer *buffer,
                        void *audio_data,
                        uint32 audio_size)
{
    if (audio_size <= buffer->size - buffer->index_read)
    {
        memcpy(audio_data, buffer->data + buffer->index_read, audio_size);
        buffer->index_read += audio_size;
    }
    else
    {
        uint32 chunk1_size = buffer->size - buffer->index_read;
        uint32 chunk2_size = audio_size - chunk1_size;

        memcpy(audio_data, buffer->data + buffer->index_read, chunk1_size);
        memcpy(audio_data + chunk1_size, buffer->data, chunk2_size);

        buffer->index_read = chunk2_size;
    }
}
