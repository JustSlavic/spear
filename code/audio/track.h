#ifndef SPEAR__AUDIO_TRACK_H
#define SPEAR__AUDIO_TRACK_H

#include <base.h>


struct audio__track
{
    byte *data;
    usize size;

    uint32 channel_count;
    uint32 samples_per_second;
    uint32 bits_per_sample;
};


#endif // SPEAR__AUDIO_TRACK_H
