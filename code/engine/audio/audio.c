#include "audio.h"


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
