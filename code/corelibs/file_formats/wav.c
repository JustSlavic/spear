#include "wav.h"


#define WAV_MAGIC_NUMBER(a, b, c, d) ((((uint32)a) << 0) | (((uint32)b) << 8) | (((uint32)c) << 16) | (((uint32)d) << 24))
enum
{
    WAV_WAVE_ID = WAV_MAGIC_NUMBER('W', 'A', 'V', 'E'),
    WAV_RIFF_ID = WAV_MAGIC_NUMBER('R', 'I', 'F', 'F'),
    WAV_FMT_ID  = WAV_MAGIC_NUMBER('f', 'm', 't', ' '),
    WAV_DATA_ID = WAV_MAGIC_NUMBER('d', 'a', 't', 'a'),
    WAV_CUE_ID  = WAV_MAGIC_NUMBER('c', 'u', 'e', ' '),
    WAV_LIST_ID = WAV_MAGIC_NUMBER('L', 'I', 'S', 'T'),
    WAV_BEXT_ID = WAV_MAGIC_NUMBER('b', 'e', 'x', 't'),
    WAV_LGWV_ID = WAV_MAGIC_NUMBER('L', 'G', 'W', 'V'),
};
#undef WAV_MAGIC_NUMBER

#define WAV_READ_DEBUG 0
#if WAV_READ_DEBUG
#define WAV_READ_DEBUG_LOG(...) printf(__VA_ARGS__)
#else
#define WAV_READ_DEBUG_LOG(...)
#endif

#define WAV_READ(Type, Variable, Index, Size, Error) \
        if (Index + sizeof(Variable) <= Size) {      \
            Variable = *(Type *) (data + Index);     \
            WAV_READ_DEBUG_LOG("[WavDecoder]: %s = %lld\n", #Variable, (int64) Variable); \
            Index += sizeof(Variable);               \
        } else {                                     \
            WAV_READ_DEBUG_LOG("[WavDecoder]: Error while reading variable %s (index=%d; variable_size=%lu; size=%llu)\n", #Variable, Index, sizeof(Type), (usize) Size); \
            return Error;                            \
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

wav_decode_result wav_decode_subchunk_fmt(uint8 *data, uint32 subchunk_size,
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

wav_decode_result wav_decode_subchunk_bext(uint8 *data, uint32 subchunk_size)
{
    /*
        Chunk 'bext' - Broadcast Extension

        https://tech.ebu.ch/docs/tech/tech3285.pdf

        char[256] - Description
        char[32]  - Originator
        char[32]  - OriginatorReference
        char[10]  - OriginationDate
        char[8]   - OriginationTime
        uint64    - TimeReference
        uint16    - Version
        char[64]  - UMID
        uint16    - LoudnessValue
        uint16    - LoudnessRange
        uint16    - MaxTruePeakLevel
        uint16    - MaxMomentaryLoudness
        uint16    - MaxShortTermLoudness
        char[180] - Reserved
        char[]    - CodingHistory
    */
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
    if (format != WAV_WAVE_ID)
    {
        return WavDecode_RiffChunkFormatIsNotWave;
    }

    while (index < chunk_size)
    {
        uint32 subchunk_signature, subchunk_size;
        if (index + sizeof(uint32) <= chunk_size)
        {
            subchunk_signature = *(uint32 *) (data + index);
            WAV_READ_DEBUG_LOG("[WavDecoder]: subchunk_signature = '%.*s'\n", 4, (char *) &subchunk_signature);
            index += sizeof(uint32);
        }
        else
        {
            WAV_READ_DEBUG_LOG("[WavDecoder]: Error while reading variable subchunk_signature (index=%d; variable_size=%lu; size=%u)\n", index, sizeof(uint32), chunk_size);
            return WavDecode_UnexpectedEndOfChunk;
        }
        WAV_READ(uint32, subchunk_size, index, chunk_size, WavDecode_UnexpectedEndOfChunk);

        if (subchunk_signature == WAV_FMT_ID)
        {
            wav_decode_result decode_result = wav_decode_subchunk_fmt(
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
        else if (subchunk_signature == WAV_CUE_ID
              || subchunk_signature == WAV_LIST_ID
              || subchunk_signature == WAV_LGWV_ID)
        {}
        else if (subchunk_signature == WAV_BEXT_ID)
        {
            /*
                @note: For some reason the file from the Internet
                    contained wrong subchunk size == 603.
                    It did not include the null-terminated string for
                    This is stupid, I should make fixer for it.
                @todo: Make little program that will load WAV file and
                    allow me to delete subchunks, or rewrite some
                    of the parts in it.
            */
            if (subchunk_size == 603)
                subchunk_size = 604;
            wav_decode_subchunk_bext(data + index, subchunk_size);
        }
        else
        {
            REPORT_ERROR("Wav decoder encountered unknown subchunk signature: 0x%x ('%.*s')",
                subchunk_signature, 4, (char *) &subchunk_signature);
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

        if (index + sizeof(uint32) <= file_size)
        {
            chunk_signature = *(uint32 *) (data + index);
            WAV_READ_DEBUG_LOG("[WavDecoder]: chunk_signature = '%.*s'\n", 4, (char *) &chunk_signature);
            index += sizeof(uint32);
        }
        else
        {
            return WavDecode_UnexpectedEof;
        }

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
                // @todo: it's ok for now because we do only one RIFF chunk
                // sound_data += samples_size;
                // sound_size -= samples_size;

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

