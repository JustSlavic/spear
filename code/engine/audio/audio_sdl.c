#include "audio.h"


void my_audio_callback(void *userdata, uint8 *buffer_to_write_to, int requested_length)
{
    if (userdata)
    {
        audio_buffer *audio = (audio_buffer *) userdata;
        if (audio->data == NULL)
        {
            memset(buffer_to_write_to, 0, requested_length);
            return;
        }

        if (audio->index_read + requested_length < audio->size)
        {
            memcpy(buffer_to_write_to, audio->data + audio->index_read, requested_length);
            audio->index_read += requested_length;
        }
        else
        {
            uint32 chunk1_size = audio->size - audio->index_read;
            uint32 chunk2_size = requested_length - chunk1_size;

            memcpy(buffer_to_write_to, audio->data + audio->index_read, chunk1_size);
            memcpy(buffer_to_write_to + chunk1_size, audio->data, chunk2_size);

            audio->index_read = chunk2_size;
        }
    }
}


void spear_audio_init(audio_buffer *buffer)
{
    SDL_AudioSpec sdl_spec = {};
    sdl_spec.format = AUDIO_S16LSB;
    sdl_spec.freq = 44100;
    sdl_spec.channels = 2;
    sdl_spec.samples = 0;
    sdl_spec.size = 0;
    sdl_spec.callback = my_audio_callback;
    sdl_spec.userdata = buffer;

    if (SDL_OpenAudio(&sdl_spec, NULL) < 0)
    {
        printf("SOUND ERROR COULD NOT OPEN AUDIO\n");
        return;
    }

    SDL_PauseAudio(0);
}
