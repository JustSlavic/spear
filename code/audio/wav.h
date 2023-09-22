#ifndef SPEAR__AUDIO_WAVE_H
#define SPEAR__AUDIO_WAVE_H

#include <base.h>
#include <memory_allocator.h>
#include <audio/track.h>


audio__track load_wav(memory_block raw_data);


#endif // SPEAR__AUDIO_WAVE_H
