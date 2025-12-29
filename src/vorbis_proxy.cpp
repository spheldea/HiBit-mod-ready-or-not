#include "pch.h"
#include "vorbis_proxy.h"
#include "audio_processor.h"
#include "config.h"

namespace VorbisProxy {
    static HMODULE g_OriginalDll = nullptr;
    
    // Function pointers
    Fn_ov_clear           p_ov_clear = nullptr;
    Fn_ov_fopen           p_ov_fopen = nullptr;
    Fn_ov_open            p_ov_open = nullptr;
    Fn_ov_open_callbacks  p_ov_open_callbacks = nullptr;
    Fn_ov_test            p_ov_test = nullptr;
    Fn_ov_test_callbacks  p_ov_test_callbacks = nullptr;
    Fn_ov_test_open       p_ov_test_open = nullptr;
    Fn_ov_bitrate         p_ov_bitrate = nullptr;
    Fn_ov_bitrate_instant p_ov_bitrate_instant = nullptr;
    Fn_ov_streams         p_ov_streams = nullptr;
    Fn_ov_seekable        p_ov_seekable = nullptr;
    Fn_ov_serialnumber    p_ov_serialnumber = nullptr;
    Fn_ov_raw_total       p_ov_raw_total = nullptr;
    Fn_ov_pcm_total       p_ov_pcm_total = nullptr;
    Fn_ov_time_total      p_ov_time_total = nullptr;
    Fn_ov_raw_seek        p_ov_raw_seek = nullptr;
    Fn_ov_pcm_seek        p_ov_pcm_seek = nullptr;
    Fn_ov_pcm_seek_page   p_ov_pcm_seek_page = nullptr;
    Fn_ov_time_seek       p_ov_time_seek = nullptr;
    Fn_ov_time_seek_page  p_ov_time_seek_page = nullptr;
    Fn_ov_raw_seek_lap    p_ov_raw_seek_lap = nullptr;
    Fn_ov_pcm_seek_lap    p_ov_pcm_seek_lap = nullptr;
    Fn_ov_pcm_seek_page_lap p_ov_pcm_seek_page_lap = nullptr;
    Fn_ov_time_seek_lap   p_ov_time_seek_lap = nullptr;
    Fn_ov_time_seek_page_lap p_ov_time_seek_page_lap = nullptr;
    Fn_ov_raw_tell        p_ov_raw_tell = nullptr;
    Fn_ov_pcm_tell        p_ov_pcm_tell = nullptr;
    Fn_ov_time_tell       p_ov_time_tell = nullptr;
    Fn_ov_info            p_ov_info = nullptr;
    Fn_ov_comment         p_ov_comment = nullptr;
    Fn_ov_read            p_ov_read = nullptr;
    Fn_ov_read_float      p_ov_read_float = nullptr;
    Fn_ov_crosslap        p_ov_crosslap = nullptr;
    Fn_ov_halfrate        p_ov_halfrate = nullptr;
    Fn_ov_halfrate_p      p_ov_halfrate_p = nullptr;

    bool Initialize() {
        if (g_OriginalDll) return true;
        
        g_OriginalDll = LoadLibraryA(Config::OriginalDllName);
        if (!g_OriginalDll) return false;
        
        #define LOAD_FUNC(name) p_##name = reinterpret_cast<Fn_##name>(GetProcAddress(g_OriginalDll, #name))
        
        LOAD_FUNC(ov_clear);
        LOAD_FUNC(ov_fopen);
        LOAD_FUNC(ov_open);
        LOAD_FUNC(ov_open_callbacks);
        LOAD_FUNC(ov_test);
        LOAD_FUNC(ov_test_callbacks);
        LOAD_FUNC(ov_test_open);
        LOAD_FUNC(ov_bitrate);
        LOAD_FUNC(ov_bitrate_instant);
        LOAD_FUNC(ov_streams);
        LOAD_FUNC(ov_seekable);
        LOAD_FUNC(ov_serialnumber);
        LOAD_FUNC(ov_raw_total);
        LOAD_FUNC(ov_pcm_total);
        LOAD_FUNC(ov_time_total);
        LOAD_FUNC(ov_raw_seek);
        LOAD_FUNC(ov_pcm_seek);
        LOAD_FUNC(ov_pcm_seek_page);
        LOAD_FUNC(ov_time_seek);
        LOAD_FUNC(ov_time_seek_page);
        LOAD_FUNC(ov_raw_seek_lap);
        LOAD_FUNC(ov_pcm_seek_lap);
        LOAD_FUNC(ov_pcm_seek_page_lap);
        LOAD_FUNC(ov_time_seek_lap);
        LOAD_FUNC(ov_time_seek_page_lap);
        LOAD_FUNC(ov_raw_tell);
        LOAD_FUNC(ov_pcm_tell);
        LOAD_FUNC(ov_time_tell);
        LOAD_FUNC(ov_info);
        LOAD_FUNC(ov_comment);
        LOAD_FUNC(ov_read);
        LOAD_FUNC(ov_read_float);
        LOAD_FUNC(ov_crosslap);
        LOAD_FUNC(ov_halfrate);
        LOAD_FUNC(ov_halfrate_p);
        
        #undef LOAD_FUNC
        
        return true;
    }
    
    void Shutdown() {
        if (g_OriginalDll) {
            FreeLibrary(g_OriginalDll);
            g_OriginalDll = nullptr;
        }
    }
    
    bool IsLoaded() {
        return g_OriginalDll != nullptr;
    }
}

// Enhanced audio processing for PCM data
static void EnhanceAudioData(char* buffer, long bytesRead, int word, int channels) {
    if (!buffer || bytesRead <= 0) return;
    
    int sampleSize = word;
    int frameSize = sampleSize * channels;
    long frameCount = bytesRead / frameSize;
    
    float qualityMultiplier = static_cast<float>(Config::TargetBitrate) / Config::OriginalBitrate;
    qualityMultiplier = 1.0f + (qualityMultiplier - 1.0f) * 0.1f;
    
    if (word == 2) {
        int16_t* samples = reinterpret_cast<int16_t*>(buffer);
        long sampleCount = bytesRead / sizeof(int16_t);
        
        for (long i = 0; i < sampleCount; ++i) {
            float sample = static_cast<float>(samples[i]) * qualityMultiplier;
            if (sample > 32767.0f) sample = 32767.0f;
            if (sample < -32768.0f) sample = -32768.0f;
            samples[i] = static_cast<int16_t>(sample);
        }
    }
}

// Exported proxy functions
extern "C" {

__declspec(dllexport) int ov_clear(OggVorbis_File* vf) {
    if (VorbisProxy::p_ov_clear) return VorbisProxy::p_ov_clear(vf);
    return -1;
}

__declspec(dllexport) int ov_fopen(const char* path, OggVorbis_File* vf) {
    if (VorbisProxy::p_ov_fopen) return VorbisProxy::p_ov_fopen(path, vf);
    return -1;
}

__declspec(dllexport) int ov_open(FILE* f, OggVorbis_File* vf, const char* initial, long ibytes) {
    if (VorbisProxy::p_ov_open) return VorbisProxy::p_ov_open(f, vf, initial, ibytes);
    return -1;
}

__declspec(dllexport) int ov_open_callbacks(void* datasource, OggVorbis_File* vf, const char* initial, long ibytes, ov_callbacks callbacks) {
    if (VorbisProxy::p_ov_open_callbacks) return VorbisProxy::p_ov_open_callbacks(datasource, vf, initial, ibytes, callbacks);
    return -1;
}

__declspec(dllexport) int ov_test(FILE* f, OggVorbis_File* vf, const char* initial, long ibytes) {
    if (VorbisProxy::p_ov_test) return VorbisProxy::p_ov_test(f, vf, initial, ibytes);
    return -1;
}

__declspec(dllexport) int ov_test_callbacks(void* datasource, OggVorbis_File* vf, const char* initial, long ibytes, ov_callbacks callbacks) {
    if (VorbisProxy::p_ov_test_callbacks) return VorbisProxy::p_ov_test_callbacks(datasource, vf, initial, ibytes, callbacks);
    return -1;
}

__declspec(dllexport) int ov_test_open(OggVorbis_File* vf) {
    if (VorbisProxy::p_ov_test_open) return VorbisProxy::p_ov_test_open(vf);
    return -1;
}

__declspec(dllexport) long ov_bitrate(OggVorbis_File* vf, int i) {
    if (VorbisProxy::p_ov_bitrate) {
        long bitrate = VorbisProxy::p_ov_bitrate(vf, i);
        return (bitrate > 0) ? Config::TargetBitrate : bitrate;
    }
    return -1;
}

__declspec(dllexport) long ov_bitrate_instant(OggVorbis_File* vf) {
    if (VorbisProxy::p_ov_bitrate_instant) {
        long bitrate = VorbisProxy::p_ov_bitrate_instant(vf);
        return (bitrate > 0) ? Config::TargetBitrate : bitrate;
    }
    return -1;
}

__declspec(dllexport) long ov_streams(OggVorbis_File* vf) {
    if (VorbisProxy::p_ov_streams) return VorbisProxy::p_ov_streams(vf);
    return -1;
}

__declspec(dllexport) long ov_seekable(OggVorbis_File* vf) {
    if (VorbisProxy::p_ov_seekable) return VorbisProxy::p_ov_seekable(vf);
    return -1;
}

__declspec(dllexport) long ov_serialnumber(OggVorbis_File* vf, int i) {
    if (VorbisProxy::p_ov_serialnumber) return VorbisProxy::p_ov_serialnumber(vf, i);
    return -1;
}

__declspec(dllexport) int64_t ov_raw_total(OggVorbis_File* vf, int i) {
    if (VorbisProxy::p_ov_raw_total) return VorbisProxy::p_ov_raw_total(vf, i);
    return -1;
}

__declspec(dllexport) int64_t ov_pcm_total(OggVorbis_File* vf, int i) {
    if (VorbisProxy::p_ov_pcm_total) return VorbisProxy::p_ov_pcm_total(vf, i);
    return -1;
}

__declspec(dllexport) double ov_time_total(OggVorbis_File* vf, int i) {
    if (VorbisProxy::p_ov_time_total) return VorbisProxy::p_ov_time_total(vf, i);
    return -1.0;
}

__declspec(dllexport) int ov_raw_seek(OggVorbis_File* vf, int64_t pos) {
    if (VorbisProxy::p_ov_raw_seek) return VorbisProxy::p_ov_raw_seek(vf, pos);
    return -1;
}

__declspec(dllexport) int ov_pcm_seek(OggVorbis_File* vf, int64_t pos) {
    if (VorbisProxy::p_ov_pcm_seek) return VorbisProxy::p_ov_pcm_seek(vf, pos);
    return -1;
}

__declspec(dllexport) int ov_pcm_seek_page(OggVorbis_File* vf, int64_t pos) {
    if (VorbisProxy::p_ov_pcm_seek_page) return VorbisProxy::p_ov_pcm_seek_page(vf, pos);
    return -1;
}

__declspec(dllexport) int ov_time_seek(OggVorbis_File* vf, double pos) {
    if (VorbisProxy::p_ov_time_seek) return VorbisProxy::p_ov_time_seek(vf, pos);
    return -1;
}

__declspec(dllexport) int ov_time_seek_page(OggVorbis_File* vf, double pos) {
    if (VorbisProxy::p_ov_time_seek_page) return VorbisProxy::p_ov_time_seek_page(vf, pos);
    return -1;
}

__declspec(dllexport) int ov_raw_seek_lap(OggVorbis_File* vf, int64_t pos) {
    if (VorbisProxy::p_ov_raw_seek_lap) return VorbisProxy::p_ov_raw_seek_lap(vf, pos);
    return -1;
}

__declspec(dllexport) int ov_pcm_seek_lap(OggVorbis_File* vf, int64_t pos) {
    if (VorbisProxy::p_ov_pcm_seek_lap) return VorbisProxy::p_ov_pcm_seek_lap(vf, pos);
    return -1;
}

__declspec(dllexport) int ov_pcm_seek_page_lap(OggVorbis_File* vf, int64_t pos) {
    if (VorbisProxy::p_ov_pcm_seek_page_lap) return VorbisProxy::p_ov_pcm_seek_page_lap(vf, pos);
    return -1;
}

__declspec(dllexport) int ov_time_seek_lap(OggVorbis_File* vf, double pos) {
    if (VorbisProxy::p_ov_time_seek_lap) return VorbisProxy::p_ov_time_seek_lap(vf, pos);
    return -1;
}

__declspec(dllexport) int ov_time_seek_page_lap(OggVorbis_File* vf, double pos) {
    if (VorbisProxy::p_ov_time_seek_page_lap) return VorbisProxy::p_ov_time_seek_page_lap(vf, pos);
    return -1;
}

__declspec(dllexport) int64_t ov_raw_tell(OggVorbis_File* vf) {
    if (VorbisProxy::p_ov_raw_tell) return VorbisProxy::p_ov_raw_tell(vf);
    return -1;
}

__declspec(dllexport) int64_t ov_pcm_tell(OggVorbis_File* vf) {
    if (VorbisProxy::p_ov_pcm_tell) return VorbisProxy::p_ov_pcm_tell(vf);
    return -1;
}

__declspec(dllexport) double ov_time_tell(OggVorbis_File* vf) {
    if (VorbisProxy::p_ov_time_tell) return VorbisProxy::p_ov_time_tell(vf);
    return -1.0;
}

__declspec(dllexport) vorbis_info* ov_info(OggVorbis_File* vf, int link) {
    if (VorbisProxy::p_ov_info) return VorbisProxy::p_ov_info(vf, link);
    return nullptr;
}

__declspec(dllexport) vorbis_comment* ov_comment(OggVorbis_File* vf, int link) {
    if (VorbisProxy::p_ov_comment) return VorbisProxy::p_ov_comment(vf, link);
    return nullptr;
}

__declspec(dllexport) long ov_read(OggVorbis_File* vf, char* buffer, int length, int bigendianp, int word, int sgned, int* bitstream) {
    if (!VorbisProxy::p_ov_read) return -1;
    
    long bytesRead = VorbisProxy::p_ov_read(vf, buffer, length, bigendianp, word, sgned, bitstream);
    
    if (bytesRead > 0 && buffer) {
        vorbis_info* info = VorbisProxy::p_ov_info ? VorbisProxy::p_ov_info(vf, -1) : nullptr;
        int channels = info ? 2 : 2;
        EnhanceAudioData(buffer, bytesRead, word, channels);
    }
    
    return bytesRead;
}

__declspec(dllexport) long ov_read_float(OggVorbis_File* vf, float*** pcm_channels, int samples, int* bitstream) {
    if (VorbisProxy::p_ov_read_float) return VorbisProxy::p_ov_read_float(vf, pcm_channels, samples, bitstream);
    return -1;
}

__declspec(dllexport) int ov_crosslap(OggVorbis_File* vf1, OggVorbis_File* vf2) {
    if (VorbisProxy::p_ov_crosslap) return VorbisProxy::p_ov_crosslap(vf1, vf2);
    return -1;
}

__declspec(dllexport) int ov_halfrate(OggVorbis_File* vf, int flag) {
    if (VorbisProxy::p_ov_halfrate) return VorbisProxy::p_ov_halfrate(vf, flag);
    return -1;
}

__declspec(dllexport) int ov_halfrate_p(OggVorbis_File* vf) {
    if (VorbisProxy::p_ov_halfrate_p) return VorbisProxy::p_ov_halfrate_p(vf);
    return -1;
}

} // extern "C"
