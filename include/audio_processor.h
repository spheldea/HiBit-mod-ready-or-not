#pragma once
#include "pak_types.h"
#include "config.h"

class AudioProcessor {
public:
    static AudioProcessor& Instance();
    
    bool Initialize();
    void Shutdown();
    
    bool ProcessAudioStream(uint64_t assetHash, void* audioData, size_t dataSize);
    bool EnhanceBitrate(void* audioData, size_t dataSize, const AudioAssetHeader* header);
    bool ReduceCompression(void* audioData, size_t dataSize, float targetRatio);
    
    void RegisterStream(uint64_t assetHash);
    void UnregisterStream(uint64_t assetHash);
    
    uint32_t GetActiveStreamCount() const { return m_ActiveStreams; }
    bool IsProcessing() const { return m_Processing; }

private:
    AudioProcessor() = default;
    ~AudioProcessor() = default;
    
    AudioProcessor(const AudioProcessor&) = delete;
    AudioProcessor& operator=(const AudioProcessor&) = delete;
    
    bool ApplyDSPChain(void* audioData, size_t dataSize);
    bool UpsampleAudio(void* audioData, size_t dataSize, uint32_t targetRate);
    float CalculateQualityFactor(uint32_t originalBitrate, uint32_t targetBitrate);
    
    AudioStreamInfo m_Streams[Config::MaxAudioStreams] = {};
    uint32_t        m_ActiveStreams = 0;
    uint8_t*        m_ProcessingBuffer = nullptr;
    size_t          m_BufferSize = 0;
    bool            m_Processing = false;
    bool            m_Initialized = false;
    CRITICAL_SECTION m_Lock = {};
};
