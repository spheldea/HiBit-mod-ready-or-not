#pragma once
#include "vorbis_types.h"

namespace VorbisProxy {
    bool Initialize();
    void Shutdown();
    bool IsLoaded();
    
    // Original function pointers
    using Fn_ov_clear = int(*)(OggVorbis_File* vf);
    using Fn_ov_fopen = int(*)(const char* path, OggVorbis_File* vf);
    using Fn_ov_open = int(*)(FILE* f, OggVorbis_File* vf, const char* initial, long ibytes);
    using Fn_ov_open_callbacks = int(*)(void* datasource, OggVorbis_File* vf, const char* initial, long ibytes, ov_callbacks callbacks);
    using Fn_ov_test = int(*)(FILE* f, OggVorbis_File* vf, const char* initial, long ibytes);
    using Fn_ov_test_callbacks = int(*)(void* datasource, OggVorbis_File* vf, const char* initial, long ibytes, ov_callbacks callbacks);
    using Fn_ov_test_open = int(*)(OggVorbis_File* vf);
    
    using Fn_ov_bitrate = long(*)(OggVorbis_File* vf, int i);
    using Fn_ov_bitrate_instant = long(*)(OggVorbis_File* vf);
    using Fn_ov_streams = long(*)(OggVorbis_File* vf);
    using Fn_ov_seekable = long(*)(OggVorbis_File* vf);
    using Fn_ov_serialnumber = long(*)(OggVorbis_File* vf, int i);
    
    using Fn_ov_raw_total = int64_t(*)(OggVorbis_File* vf, int i);
    using Fn_ov_pcm_total = int64_t(*)(OggVorbis_File* vf, int i);
    using Fn_ov_time_total = double(*)(OggVorbis_File* vf, int i);
    
    using Fn_ov_raw_seek = int(*)(OggVorbis_File* vf, int64_t pos);
    using Fn_ov_pcm_seek = int(*)(OggVorbis_File* vf, int64_t pos);
    using Fn_ov_pcm_seek_page = int(*)(OggVorbis_File* vf, int64_t pos);
    using Fn_ov_time_seek = int(*)(OggVorbis_File* vf, double pos);
    using Fn_ov_time_seek_page = int(*)(OggVorbis_File* vf, double pos);
    
    using Fn_ov_raw_seek_lap = int(*)(OggVorbis_File* vf, int64_t pos);
    using Fn_ov_pcm_seek_lap = int(*)(OggVorbis_File* vf, int64_t pos);
    using Fn_ov_pcm_seek_page_lap = int(*)(OggVorbis_File* vf, int64_t pos);
    using Fn_ov_time_seek_lap = int(*)(OggVorbis_File* vf, double pos);
    using Fn_ov_time_seek_page_lap = int(*)(OggVorbis_File* vf, double pos);
    
    using Fn_ov_raw_tell = int64_t(*)(OggVorbis_File* vf);
    using Fn_ov_pcm_tell = int64_t(*)(OggVorbis_File* vf);
    using Fn_ov_time_tell = double(*)(OggVorbis_File* vf);
    
    using Fn_ov_info = vorbis_info*(*)(OggVorbis_File* vf, int link);
    using Fn_ov_comment = vorbis_comment*(*)(OggVorbis_File* vf, int link);
    
    using Fn_ov_read = long(*)(OggVorbis_File* vf, char* buffer, int length, int bigendianp, int word, int sgned, int* bitstream);
    using Fn_ov_read_float = long(*)(OggVorbis_File* vf, float*** pcm_channels, int samples, int* bitstream);
    
    using Fn_ov_crosslap = int(*)(OggVorbis_File* vf1, OggVorbis_File* vf2);
    using Fn_ov_halfrate = int(*)(OggVorbis_File* vf, int flag);
    using Fn_ov_halfrate_p = int(*)(OggVorbis_File* vf);
    
    // Function pointer storage
    extern Fn_ov_clear           p_ov_clear;
    extern Fn_ov_fopen           p_ov_fopen;
    extern Fn_ov_open            p_ov_open;
    extern Fn_ov_open_callbacks  p_ov_open_callbacks;
    extern Fn_ov_test            p_ov_test;
    extern Fn_ov_test_callbacks  p_ov_test_callbacks;
    extern Fn_ov_test_open       p_ov_test_open;
    extern Fn_ov_bitrate         p_ov_bitrate;
    extern Fn_ov_bitrate_instant p_ov_bitrate_instant;
    extern Fn_ov_streams         p_ov_streams;
    extern Fn_ov_seekable        p_ov_seekable;
    extern Fn_ov_serialnumber    p_ov_serialnumber;
    extern Fn_ov_raw_total       p_ov_raw_total;
    extern Fn_ov_pcm_total       p_ov_pcm_total;
    extern Fn_ov_time_total      p_ov_time_total;
    extern Fn_ov_raw_seek        p_ov_raw_seek;
    extern Fn_ov_pcm_seek        p_ov_pcm_seek;
    extern Fn_ov_pcm_seek_page   p_ov_pcm_seek_page;
    extern Fn_ov_time_seek       p_ov_time_seek;
    extern Fn_ov_time_seek_page  p_ov_time_seek_page;
    extern Fn_ov_raw_seek_lap    p_ov_raw_seek_lap;
    extern Fn_ov_pcm_seek_lap    p_ov_pcm_seek_lap;
    extern Fn_ov_pcm_seek_page_lap p_ov_pcm_seek_page_lap;
    extern Fn_ov_time_seek_lap   p_ov_time_seek_lap;
    extern Fn_ov_time_seek_page_lap p_ov_time_seek_page_lap;
    extern Fn_ov_raw_tell        p_ov_raw_tell;
    extern Fn_ov_pcm_tell        p_ov_pcm_tell;
    extern Fn_ov_time_tell       p_ov_time_tell;
    extern Fn_ov_info            p_ov_info;
    extern Fn_ov_comment         p_ov_comment;
    extern Fn_ov_read            p_ov_read;
    extern Fn_ov_read_float      p_ov_read_float;
    extern Fn_ov_crosslap        p_ov_crosslap;
    extern Fn_ov_halfrate        p_ov_halfrate;
    extern Fn_ov_halfrate_p      p_ov_halfrate_p;
}
