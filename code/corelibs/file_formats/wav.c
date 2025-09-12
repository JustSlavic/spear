#include "wav.h"


#define WAV_MAGIC_NUMBER(a, b, c, d) ((((uint32)a) << 0) | (((uint32)b) << 8) | (((uint32)c) << 16) | (((uint32)d) << 24))
enum
{
    WAV_RIFF_ID = WAV_MAGIC_NUMBER('R', 'I', 'F', 'F'),
    WAV_FMT_ID  = WAV_MAGIC_NUMBER('f', 'm', 't', ' '),
    WAV_DATA_ID = WAV_MAGIC_NUMBER('d', 'a', 't', 'a'),
};
#undef WAV_MAGIC_NUMBER

#define WAV_READ(Type, Variable, Index, Size, Error) \
        if (Index + sizeof(Variable) <= Size) {      \
            Variable = *(Type *) (data + Index);     \
            Index += sizeof(Variable);               \
        } else {                                     \
            return Error;          \
        }                                            \

char const *wav_decode_result_to_cstring(wav_decode_result result)
{
    switch (result)
    {
    case WavDecode_Success: return "WavDecode_Success";
    case WavDecode_UnexpectedEof: return "WavDecode_UnexpectedEof";
    case WavDecode_UnknownSignature: return "WavDecode_UnknownSignature";
    case WavDecode_UnexpectedEndOfChunk: return "WavDecode_UnexpectedEndOfChunk";
    case WavDecode_RiffChunkFormatIsNotWave: return "WavDecode_RiffChunkFormatIsNotWave";
    case WavDecode_BufferOverflow: return "WavDecode_BufferOverflow";
    }
    return NULL;
}

wav_decode_result wav_decode_subchunk_format(uint8 *data, uint32 subchunk_size,
    uint16 *out_audio_format,
    uint16 *out_channel_count,
    uint32 *out_sample_rate_hz,
    uint32 *out_bytes_per_second,
    uint16 *out_bytes_per_block,
    uint16 *out_bits_per_sample)
{
    uint32 index = 0;

    WAV_READ(uint16, *out_audio_format, index, subchunk_size, WavDecode_UnexpectedEndOfChunk);
    WAV_READ(uint16, *out_channel_count, index, subchunk_size, WavDecode_UnexpectedEndOfChunk);
    WAV_READ(uint32, *out_sample_rate_hz, index, subchunk_size, WavDecode_UnexpectedEndOfChunk);
    WAV_READ(uint32, *out_bytes_per_second, index, subchunk_size, WavDecode_UnexpectedEndOfChunk);
    WAV_READ(uint16, *out_bytes_per_block, index, subchunk_size, WavDecode_UnexpectedEndOfChunk);
    WAV_READ(uint16, *out_bits_per_sample, index, subchunk_size, WavDecode_UnexpectedEndOfChunk);

    return WavDecode_Success;
}

wav_decode_result wav_decode_chunk_riff(uint8 *data, uint32 chunk_size,
    void  **out_data,
    uint32 *out_data_size,
    uint16 *out_audio_format,
    uint16 *out_channel_count,
    uint32 *out_sample_rate_hz,
    uint32 *out_bytes_per_second,
    uint16 *out_bytes_per_block,
    uint16 *out_bits_per_sample)
{
    uint32 index = 0;
    uint32 format;

    WAV_READ(uint32, format, index, chunk_size, WavDecode_UnexpectedEndOfChunk);
    if (format != 0x45564157) // "WAVE"
    {
        return WavDecode_RiffChunkFormatIsNotWave;
    }

    while (index < chunk_size)
    {
        uint32 subchunk_signature, subchunk_size;
        WAV_READ(uint32, subchunk_signature, index, chunk_size, WavDecode_UnexpectedEndOfChunk);
        WAV_READ(uint32, subchunk_size, index, chunk_size, WavDecode_UnexpectedEndOfChunk);

        if (subchunk_signature == WAV_FMT_ID)
        {
            wav_decode_result decode_result = wav_decode_subchunk_format(
                data + index,
                subchunk_size,
                out_audio_format,
                out_channel_count,
                out_sample_rate_hz,
                out_bytes_per_second,
                out_bytes_per_block,
                out_bits_per_sample);

            if (decode_result != WavDecode_Success)
            {
                return decode_result;
            }
        }
        else if (subchunk_signature == WAV_DATA_ID)
        {
            *out_data = data + index;
            *out_data_size = subchunk_size;
        }
        else
        {
            fprintf(stderr, "Error: Wav decoder encountered unknown subchunk signature: 0x%x\n", subchunk_signature);
            return WavDecode_UnknownSignature;
        }
        index += subchunk_size;
    }

    return WavDecode_Success;
}

wav_decode_result wav_extract_size(void *file_data, usize file_size, usize *out_sound_size)
{
    uint8 *data = (uint8 *) file_data;
    uint32 index = 0;

    // Accept only one RIFF chunk
    // while (index < file_size)
    {
        uint32 chunk_signature, chunk_size;

        WAV_READ(uint32, chunk_signature, index, file_size, WavDecode_UnexpectedEof);
        WAV_READ(uint32, chunk_size, index, file_size, WavDecode_UnexpectedEof);

        if (chunk_signature == WAV_RIFF_ID)
        {
            uint16 audio_format, channel_count, bytes_per_block, bits_per_sample;
            uint32 sample_rate_hz, bytes_per_second;
            void *samples_pointer;
            uint32 samples_size;
            wav_decode_result decode_result = wav_decode_chunk_riff(
                data + index,
                chunk_size,
                &samples_pointer,
                &samples_size,
                &audio_format,
                &channel_count,
                &sample_rate_hz,
                &bytes_per_second,
                &bytes_per_block,
                &bits_per_sample);

            if (decode_result == WavDecode_Success)
            {
                *out_sound_size = samples_size;
            }
            else
            {
                return decode_result;
            }
        }
        else
        {
            fprintf(stderr, "Error: Wav decoder encountered unknown signature: 0x%x\n", chunk_signature);
            return WavDecode_UnknownSignature;
        }
        index += chunk_size;
    }

    return WavDecode_Success;
}

wav_decode_result wav_decode(void *file_data, usize file_size,
                             void *sound_data, usize sound_size,
                             uint32 *out_channel_count,
                             uint32 *out_samples_per_second,
                             uint32 *out_bits_per_sample)
{
    uint8 *data = (uint8 *) file_data;
    uint32 index = 0;

    // Accept only one RIFF chunk
    // while (index < file_size)
    {
        uint32 chunk_signature, chunk_size;

        WAV_READ(uint32, chunk_signature, index, file_size, WavDecode_UnexpectedEof);
        WAV_READ(uint32, chunk_size, index, file_size, WavDecode_UnexpectedEof);

        if (chunk_signature == WAV_RIFF_ID)
        {
            uint16 audio_format, channel_count, bytes_per_block, bits_per_sample;
            uint32 sample_rate_hz, bytes_per_second;
            void *samples_pointer;
            uint32 samples_size;
            wav_decode_result decode_result = wav_decode_chunk_riff(
                data + index,
                chunk_size,
                &samples_pointer,
                &samples_size,
                &audio_format,
                &channel_count,
                &sample_rate_hz,
                &bytes_per_second,
                &bytes_per_block,
                &bits_per_sample);

            if (decode_result == WavDecode_Success)
            {
                if (sound_size < (usize) samples_size)
                {
                    return WavDecode_BufferOverflow;
                }
                memcpy(sound_data, samples_pointer, samples_size);
                sound_data += samples_size;
                sound_size -= samples_size;

                if (out_channel_count) *out_channel_count = channel_count;
                if (out_samples_per_second) *out_samples_per_second = sample_rate_hz;
                if (out_bits_per_sample) *out_bits_per_sample = bits_per_sample;
            }
            else
            {
                return decode_result;
            }
        }
        else
        {
            fprintf(stderr, "Error: Wav decoder encountered unknown signature: 0x%x\n", chunk_signature);
            return WavDecode_UnknownSignature;
        }
        index += chunk_size;
    }

    return WavDecode_Success;
}

#undef WAV_READ

