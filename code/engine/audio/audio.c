#include "audio.h"


void spear_audio_init(spear_audio *audio)
{
    audio->source_count = 1;
}

spear_audio_handle spear_audio_add_tone(spear_audio *audio, double frequency)
{
    spear_audio_handle handle = 0;
    if (audio->source_count < ARRAY_COUNT(audio->sources))
    {
        spear_audio_source source = {};
        source.tag = SpearAudioSource_Generator;
        // source.enabled = false;
        // source.repeat = true;
        // source.volume = 600.0;
        source.frequency = frequency;

        handle = audio->source_count++;
        audio->sources[handle] = source;
    }
    return handle;
}

spear_audio_handle spear_audio_add_track(spear_audio *audio, int16 *samples, uint32 sample_count)
{
    spear_audio_handle handle = 0;
    if (audio->source_count < ARRAY_COUNT(audio->sources))
    {
        spear_audio_source source = {};
        source.tag = SpearAudioSource_Buffer;
        // source.enabled = false;
        // source.repeat = true;
        // source.volume = 1.0;
        source.samples = samples;
        source.sample_count = sample_count;

        handle = audio->source_count++;
        audio->sources[handle] = source;
    }
    return handle;
}

spear_audio_instance_handle spear_audio_play_once(spear_audio *audio, spear_audio_handle handle)
{
    int instance_index;
    for (instance_index = 0; instance_index < ARRAY_COUNT(audio->instances); instance_index++)
    {
        spear_audio_instance *instance = audio->instances + instance_index;
        if (instance->source_handle == 0)
        {
            instance->source_handle = handle;
            instance->enabled = true;
            instance->repeat = false;
            instance->volume = 1.0;
            instance->sample_index = 0;
            instance->running_t = 0;
            return instance_index;
        }
    }
    return 0;
}

spear_audio_instance_handle spear_audio_play_loop(spear_audio *audio, spear_audio_handle handle)
{
    int instance_index;
    for (instance_index = 0; instance_index < ARRAY_COUNT(audio->instances); instance_index++)
    {
        spear_audio_instance *instance = audio->instances + instance_index;
        if (instance->source_handle == 0)
        {
            instance->source_handle = handle;
            instance->enabled = true;
            instance->repeat = true;
            instance->volume = 1.0;
            instance->sample_index = 0;
            instance->running_t = 0;
            return instance_index;
        }
    }
    return 0;
}

spear_audio_instance_handle spear_audio_add(spear_audio *audio, spear_audio_source source)
{
    if (audio->source_count == 0) audio->source_count++;

    spear_audio_instance_handle result = 0;
    int index = audio->source_count;
    if (index < ARRAY_COUNT(audio->sources))
    {
        audio->sources[index] = source;
        audio->source_count += 1;
        result = index;
    }
    return result;
}

void spear_audio_mix(spear_audio *audio, spear_sound_output_buffer *output)
{
    uint32 instance_index, frame_index;
    {
        int16 *samples = (int16 *) output->samples;
        for (frame_index = 0; frame_index < output->sample_count; frame_index++)
        {
            *samples++ = 0;
            *samples++ = 0;
        }
    }

    for (instance_index = 0; instance_index < ARRAY_COUNT(audio->instances); instance_index++)
    {
        spear_audio_instance *instance = audio->instances + instance_index;
        if (!instance->enabled)
            continue;

        spear_audio_source *source = audio->sources + instance->source_handle;
        if (source->tag == SpearAudioSource_Generator)
        {
            int16 *samples = (int16 *) output->samples;
            double samples_per_period = output->samples_per_second / source->frequency;
            for (frame_index = 0; frame_index < output->sample_count; frame_index++)
            {
                double sine_value = sin(source->running_t);
                int16 sample_value = (int16)(sine_value * instance->volume);

                *samples++ += sample_value;
                *samples++ += sample_value;

                source->running_t += TWO_PI / samples_per_period;
                if (source->running_t > TWO_PI)
                    source->running_t = source->running_t - TWO_PI;
            }
        }
        if (source->tag == SpearAudioSource_Buffer)
        {
            int16 *samples_out = (int16 *) output->samples;
            for (frame_index = 0; frame_index < output->sample_count; frame_index++)
            {
                // @todo: stop if instance is play_once
                *samples_out++ += source->samples[instance->sample_index];
                instance->sample_index = (instance->sample_index + 1) % source->sample_count;
                *samples_out++ += source->samples[instance->sample_index];
                instance->sample_index = (instance->sample_index + 1) % source->sample_count;
            }
        }
    }
}
