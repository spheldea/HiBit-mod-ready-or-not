#pragma once
#include <cstdint>

#pragma pack(push, 1)

struct PakHeader {
    char     Magic[4];
    uint32_t Version;
    uint32_t EntryCount;
    uint64_t IndexOffset;
    uint64_t IndexSize;
    uint8_t  Reserved[32];
};

struct PakEntry {
    uint64_t NameHash;
    uint64_t Offset;
    uint64_t CompressedSize;
    uint64_t UncompressedSize;
    uint32_t CompressionMethod;
    uint32_t Flags;
};

struct AudioAssetHeader {
    uint32_t Signature;
    uint32_t Version;
    uint32_t SampleRate;
    uint32_t Bitrate;
    uint16_t Channels;
    uint16_t BitDepth;
    uint32_t FrameCount;
    float    CompressionLevel;
    uint8_t  Reserved[16];
};

struct AudioStreamInfo {
    uint64_t AssetHash;
    uint32_t CurrentBitrate;
    uint32_t TargetBitrate;
    float    QualityMultiplier;
    bool     IsEnhanced;
    uint8_t  Padding[3];
};

#pragma pack(pop)
