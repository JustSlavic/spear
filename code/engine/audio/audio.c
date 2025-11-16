#include "audio.h"


spear_audio_source spear_audio_source_generator_create(double frequency)
{
    spear_audio_source result = {};
    result.tag = SpearAudioSource_Generator;
    result.enabled = true;
    result.repeat = true;
    result.volume = 600.0;
    result.frequency = frequency;
    result.running_t = 0.0;

    return result;
}

spear_audio_source spear_audio_source_buffer_create(int16 *samples, uint32 sample_count)
{
    spear_audio_source result = {};
    result.tag = SpearAudioSource_Buffer;
    result.enabled = true;
    result.repeat = true;
    result.volume = 1.0;
    result.samples = samples;
    result.sample_count = sample_count;
    result.sample_index = 0;

    return result;
}

void spear_audio_add(spear_audio *audio, spear_audio_source source)
{
    int index = audio->source_count;
    if (index < ARRAY_COUNT(audio->sources))
    {
        audio->sources[index] = source;
        audio->source_count += 1;
    }
}

void spaer_audio_mix(spear_audio *audio, spear_sound_output_buffer *output)
{
    uint32 source_index, frame_index;
    {
        // Clear
        int16 *samples = (int16 *) output->samples;
        for (frame_index = 0; frame_index < output->sample_count; frame_index++)
        {
            *samples++ = 0;
            *samples++ = 0;
        }
    }

    for (source_index = 0; source_index < audio->source_count; source_index++)
    {
        spear_audio_source *source = audio->sources + source_index;
        if (source->tag == SpearAudioSource_Generator)
        {
            int16 *samples = (int16 *) output->samples;
            double samples_per_period = output->samples_per_second / source->frequency;
            for (frame_index = 0; frame_index < output->sample_count; frame_index++)
            {
                double sine_value = sin(source->running_t);
                int16 sample_value = (int16)(sine_value * source->volume);

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
                *samples_out++ += source->samples[source->sample_index];
                source->sample_index = (source->sample_index + 1) % source->sample_count;
                *samples_out++ += source->samples[source->sample_index];
                source->sample_index = (source->sample_index + 1) % source->sample_count;
            }
        }
    }
}
