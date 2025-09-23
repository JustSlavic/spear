#include "audio.h"


void
spear_audio_init(spear_audio *audio,
                 uint32 frames_per_second,
                 uint32 channel_count,
                 uint32 bits_per_sample,
                 double latency,
                 void *playback_buffer,
                 uint32 playback_buffer_size)
{
    audio->frames_per_second = frames_per_second;
    audio->channel_count = channel_count;
    audio->bits_per_sample = bits_per_sample;
    audio->latency = latency;

    audio->playback_buffer = playback_buffer;
    audio->playback_buffer_size = playback_buffer_size;

    audio->W = 0;
    audio->R = 0;

    audio->source_count = 0;

    spear_audio_init_backend(audio);
}

int
spear_audio_add_source_sine_wave_generator(spear_audio *audio,
                                           double frequency,
                                           double volume)
{
    int index = -1;
    if (audio->source_count < ARRAY_COUNT(audio->sources))
    {
        index = audio->source_count++;

        audio->sources[index].tag = SpearAudioSource_Generator;
        audio->sources[index].data = NULL;
        audio->sources[index].size = 0;
        audio->sources[index].R = 0;
        audio->sources[index].enabled = true;
        audio->sources[index].repeat = true;
        audio->sources[index].frequency = frequency;
        audio->sources[index].volume = volume;
        audio->sources[index].running_t = 0;

        printf("Audio: Added source generator (index=%d)\n", index);
    }
    return index;
}

int
spear_audio_add_source_buffer(spear_audio *audio,
                              void *data,
                              uint32 size,
                              bool32 repeat)
{
    int index = -1;
    if (audio->source_count < ARRAY_COUNT(audio->sources))
    {
        index = audio->source_count++;

        audio->sources[index].tag = SpearAudioSource_Buffer;
        audio->sources[index].data = data;
        audio->sources[index].size = size;
        audio->sources[index].R = 0;
        audio->sources[index].enabled = true;
        audio->sources[index].repeat = repeat;
        audio->sources[index].frequency = 0;
        audio->sources[index].volume = 0;
        audio->sources[index].running_t = 0;

        printf("Audio: Added source buffer (index=%d)\n", index);
    }
    return index;
}

static void
spear_audio_source_generate_and_mix(spear_audio *audio,
                                    spear_audio_source *generator,
                                    void *audio_data,
                                    uint32 audio_size)
{
    if (generator->tag != SpearAudioSource_Generator)
        return;

    uint32 frame_index;
    uint32 frame_size = audio->channel_count * sizeof(sound_sample_t);
    uint32 frame_count = audio_size / frame_size;

    int16 *samples = (int16 *) audio_data;
    uint32 samples_per_period = audio->frames_per_second / generator->frequency;

    for (frame_index = 0; frame_index < frame_count; frame_index++)
    {
        double sine_value = sin(generator->running_t);
        int16 sample_value = (int16)(sine_value * generator->volume);

        *samples++ += sample_value;
        *samples++ += sample_value;

        generator->running_t += TWO_PI / samples_per_period;
        if (generator->running_t > TWO_PI)
            generator->running_t = generator->running_t - TWO_PI;
    }
}

static void
spear_audio_buffer_read_and_mix(spear_audio *audio,
                                spear_audio_source *source,
                                void *buffer,
                                uint32 buffer_size)
{
    if (buffer_size <= source->size - source->R)
    {
        int16 *source_samples = (int16 *) (source->data + source->R);
        int16 *samples = (int16 *) buffer;
        int sample_index;
        for (sample_index = 0; sample_index < (buffer_size / sizeof(sound_sample_t)); sample_index++)
        {
            *samples++ += source_samples[sample_index];
        }
        source->R += buffer_size;
    }
    else
    {
        uint32 chunk1_size = source->size - source->R;
        uint32 chunk2_size = buffer_size - chunk1_size;

        int16 *source_samples = (int16 *) (source->data + source->R);
        int16 *samples = (int16 *) buffer;
        int sample_index;
        for (sample_index = 0; sample_index < (chunk1_size / sizeof(sound_sample_t)); sample_index++)
        {
            *samples++ += source_samples[sample_index];
        }
        if (source->repeat)
        {
            source_samples = (int16 *) source->data;
            for (sample_index = 0; sample_index < (chunk2_size / sizeof(sound_sample_t)); sample_index++)
            {
                *samples++ += source_samples[sample_index];
            }
            source->R = chunk2_size;
        }
        else
        {
            source->enabled = false;
        }
    }
}


void spear_audio_update(spear_audio *audio)
{
    uint32 channel_count = 2;
    uint32 frame_size = channel_count * sizeof(sound_sample_t);
    uint32 latency_frames = audio->latency * audio->frames_per_second;
    uint32 latency_bytes = latency_frames * channel_count * sizeof(sound_sample_t);

    uint32 R = audio->R;
    uint32 W = audio->W;
    uint32 S = audio->playback_buffer_size;
    uint32 L = (audio->R + latency_bytes) % S;

    uint32 write_until_byte = W;

    if (R < L)
    {
        // |-------R-------L-------|S
        if (W < R)
        {
            // |---W---R-------L-------|S
            // We're gonna wait
            if (S - L > 2*latency_bytes)
            {
                W = L;
                write_until_byte = (L + latency_bytes) % S;
            }
        }
        else if (W < L)
        {
            // |-------R---W---L-------|S
            write_until_byte = (L + latency_bytes) % S;
        }
        else
        {
            // |-------R-------L---W---|S
            // We're gonna wait
        }
    }
    else
    {
        // |-------L-------R-------|S
        if (W < L)
        {
            // |---W---L-------R-------|S
            write_until_byte = (L + latency_bytes) % S;
        }
        else if (W < R)
        {
            // |-L----W--------------R-|S
            // We're gonna wait
        }
        else
        {
            // |-------L-------R---W---|S
            write_until_byte = (L + latency_bytes) % S;
        }
    }

    uint32 chunk1_size = 0;
    uint32 chunk2_size = 0;

    if (write_until_byte < W)
    {
        chunk1_size = S - W;
        chunk2_size = write_until_byte;
    }
    else
    {
        chunk1_size = write_until_byte - W;
    }

    ASSERT(chunk1_size % frame_size == 0);
    ASSERT(chunk2_size % frame_size == 0);

    if (chunk1_size)
    {
        int16 *chunk1_data = (int16 *) (audio->playback_buffer + audio->W);
        memset(chunk1_data, 0, chunk1_size);

        int source_index;
        for (source_index = 0; source_index < audio->source_count; source_index++)
        {
            spear_audio_source *source = &audio->sources[source_index];
            if (!source->enabled)
                continue;
            if (source->tag == SpearAudioSource_Generator)
            {
                spear_audio_source_generate_and_mix(audio,
                    &audio->sources[source_index],
                    chunk1_data,
                    chunk1_size);
            }
            if (source->tag == SpearAudioSource_Buffer)
            {
                spear_audio_buffer_read_and_mix(audio,
                    &audio->sources[source_index],
                    chunk1_data,
                    chunk1_size);
            }
        }
    }
    if (chunk2_size)
    {
        int16 *chunk2_data = (int16 *) (audio->playback_buffer);
        memset(chunk2_data, 0, chunk2_size);

        int source_index;
        for (source_index = 0; source_index < audio->source_count; source_index++)
        {
            spear_audio_source *source = &audio->sources[source_index];
            if (!source->enabled)
                continue;
            if (source->tag == SpearAudioSource_Generator)
            {
                spear_audio_source_generate_and_mix(audio,
                    &audio->sources[source_index],
                    chunk2_data,
                    chunk2_size);
            }
            if (source->tag == SpearAudioSource_Buffer)
            {
                spear_audio_buffer_read_and_mix(audio,
                    &audio->sources[source_index],
                    chunk2_data,
                    chunk2_size);
            }
        }
    }

    audio->W = write_until_byte;
}

