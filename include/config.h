#pragma once

namespace Config {
    constexpr int TargetBitrate = 320000;
    constexpr int OriginalBitrate = 128000;
    constexpr float CompressionRatio = 0.25f;
    constexpr int SampleRate = 48000;
    constexpr int BitDepth = 24;
    constexpr int MaxAudioStreams = 64;
    constexpr size_t AudioBufferSize = 0x100000;
    
    constexpr const char* ResourcePakName = "resources.pak";
    constexpr const char* OriginalDllName = "libvorbisfile_64-org.dll";
}
