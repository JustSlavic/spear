#include "wav.h"


struct wav__header
{
    uint32 chunk_id;
    uint32 chunk_size;
    uint8  format[4];
};

struct wav__format
{
    uint32 subchunk1_id;
    uint32 subchunk1_size;
    uint16 audio_format;
    uint16 channel_count;
    uint32 sample_rate;
    uint32 byte_rate;
    uint16 block_align;
    uint16 bits_per_sample;
};

struct wav__data
{
    uint32 subchunk2_id;
    uint32 subchunk2_size;
};


#define WAV_MAGIC_NUMBER(a, b, c, d) ((((uint32)a) << 0) | (((uint32)b) << 8) | (((uint32)c) << 16) | (((uint32)d) << 24))

enum
{
    WAV_RIFF_ID = WAV_MAGIC_NUMBER('R', 'I', 'F', 'F'),
    WAV_FMT_ID  = WAV_MAGIC_NUMBER('f', 'm', 't', ' '),
    WAV_DATA_ID = WAV_MAGIC_NUMBER('d', 'a', 't', 'a'),
};

#undef WAV_MAGIC_NUMBER


audio__track load_wav(memory_block raw_data)
{
    audio__track result = {};

    if (raw_data.size > 0)
    {
        byte *data = raw_data.memory;

        wav__header *wav_header = NULL;
        wav__format *wav_format = NULL;
        wav__data   *wav_data   = NULL;

        while (true)
        {
            uint32 header_id = *(uint32 *) data;
            if (header_id == WAV_RIFF_ID)
            {
                wav_header = (wav__header *) data;
                data += sizeof(wav__header);
            }
            else if (header_id == WAV_FMT_ID)
            {
                wav_format = (wav__format *) data;
                data += (2 * sizeof(uint32) + wav_format->subchunk1_size);
            }
            else if (header_id == WAV_DATA_ID)
            {
                wav_data = (wav__data *) data;
                data += sizeof(wav__data);
                break;
            }
            else
            {
                return result;
            }
        }

        result.samples_per_second = wav_format->sample_rate;
        result.channel_count = wav_format->channel_count;
        result.bits_per_sample = wav_format->bits_per_sample;

        result.size = wav_data->subchunk2_size;
        result.data = data;
    }

    return result;
}
