#include "audio.h"


void my_audio_callback(void *userdata, uint8 *buffer_to_write_to, int requested_length)
{
    if (userdata)
    {
        spear_audio *audio = (spear_audio *) userdata;
        if (audio->playback_buffer == NULL)
        {
            memset(buffer_to_write_to, 0, requested_length);
            return;
        }

        if (audio->R + requested_length < audio->playback_buffer_size)
        {
            memcpy(buffer_to_write_to, audio->playback_buffer + audio->R, requested_length);
            audio->R += requested_length;
        }
        else
        {
            uint32 chunk1_size = audio->playback_buffer_size - audio->R;
            uint32 chunk2_size = requested_length - chunk1_size;

            memcpy(buffer_to_write_to, audio->playback_buffer + audio->R, chunk1_size);
            memcpy(buffer_to_write_to + chunk1_size, audio->playback_buffer, chunk2_size);

            audio->R = chunk2_size;
        }
    }
}


void spear_audio_init_backend(spear_audio *audio)
{
    SDL_AudioSpec sdl_spec = {};
    sdl_spec.format = AUDIO_S16LSB;
    sdl_spec.freq = 44100;
    sdl_spec.channels = 2;
    sdl_spec.samples = 0;
    sdl_spec.size = 0;
    sdl_spec.callback = my_audio_callback;
    sdl_spec.userdata = audio;

    if (SDL_OpenAudio(&sdl_spec, NULL) < 0)
    {
        printf("SOUND ERROR COULD NOT OPEN AUDIO\n");
        return;
    }

    SDL_PauseAudio(0);
}
