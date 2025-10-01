#ifndef SPEAR_CORELIBS_FILE_FORMATS_WAV_H
#define SPEAR_CORELIBS_FILE_FORMATS_WAV_H

#include <corelibs/base.h>


typedef enum
{
    WavDecode_Success = 0,
    WavDecode_UnexpectedEof = -1,
    WavDecode_UnknownSignature = -2,
    WavDecode_UnexpectedEndOfChunk = -3,
    WavDecode_RiffChunkFormatIsNotWave = -4,
    WavDecode_BufferOverflow = -5,
} wav_decode_result;


char const *wav_decode_result_to_cstring(wav_decode_result result);
wav_decode_result wav_extract_size(void *file_data, usize file_size, usize *out_sound_size);
wav_decode_result wav_decode(void *file_data, usize file_size,
                             void *sound_data, usize sound_size,
                             uint32 *out_channel_count,
                             uint32 *out_samples_per_second,
                             uint32 *out_bits_per_sample);


#endif // SPEAR_CORELIBS_FILE_FORMATS_WAV_H
