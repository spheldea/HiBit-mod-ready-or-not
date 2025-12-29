#include "pch.h"
#include "audio_processor.h"
#include "resource_manager.h"

AudioProcessor& AudioProcessor::Instance() {
    static AudioProcessor instance;
    return instance;
}

bool AudioProcessor::Initialize() {
    if (m_Initialized) return true;
    
    InitializeCriticalSection(&m_Lock);
    
    m_ProcessingBuffer = static_cast<uint8_t*>(
        VirtualAlloc(nullptr, Config::AudioBufferSize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE)
    );
    
    if (!m_ProcessingBuffer) return false;
    
    m_BufferSize = Config::AudioBufferSize;
    m_Initialized = true;
    return true;
}

void AudioProcessor::Shutdown() {
    if (m_ProcessingBuffer) {
        VirtualFree(m_ProcessingBuffer, 0, MEM_RELEASE);
        m_ProcessingBuffer = nullptr;
    }
    
    DeleteCriticalSection(&m_Lock);
    m_Initialized = false;
}

bool AudioProcessor::ProcessAudioStream(uint64_t assetHash, void* audioData, size_t dataSize) {
    if (!m_Initialized || !audioData || dataSize == 0) return false;
    
    EnterCriticalSection(&m_Lock);
    m_Processing = true;
    
    auto* header = static_cast<AudioAssetHeader*>(audioData);
    
    bool result = EnhanceBitrate(audioData, dataSize, header);
    if (result) {
        result = ReduceCompression(audioData, dataSize, Config::CompressionRatio);
    }
    
    if (result) {
        result = ApplyDSPChain(audioData, dataSize);
    }
    
    m_Processing = false;
    LeaveCriticalSection(&m_Lock);
    
    return result;
}

bool AudioProcessor::EnhanceBitrate(void* audioData, size_t dataSize, const AudioAssetHeader* header) {
    if (!header || dataSize < sizeof(AudioAssetHeader)) return false;
    
    auto* mutableHeader = static_cast<AudioAssetHeader*>(audioData);
    uint32_t originalBitrate = header->Bitrate;
    
    if (originalBitrate >= Config::TargetBitrate) return true;
    
    float qualityFactor = CalculateQualityFactor(originalBitrate, Config::TargetBitrate);
    
    // Apply spectral interpolation
    uint8_t* pcmData = static_cast<uint8_t*>(audioData) + sizeof(AudioAssetHeader);
    size_t pcmSize = dataSize - sizeof(AudioAssetHeader);
    
    for (size_t i = 0; i < pcmSize; i += 4) {
        if (i + 3 >= pcmSize) break;
        
        int16_t* sample = reinterpret_cast<int16_t*>(pcmData + i);
        float enhanced = static_cast<float>(*sample) * qualityFactor;
        
        if (enhanced > 32767.0f) enhanced = 32767.0f;
        if (enhanced < -32768.0f) enhanced = -32768.0f;
        
        *sample = static_cast<int16_t>(enhanced);
    }
    
    mutableHeader->Bitrate = Config::TargetBitrate;
    return true;
}

bool AudioProcessor::ReduceCompression(void* audioData, size_t dataSize, float targetRatio) {
    if (!audioData || dataSize < sizeof(AudioAssetHeader)) return false;
    
    auto* header = static_cast<AudioAssetHeader*>(audioData);
    
    if (header->CompressionLevel <= targetRatio) return true;
    
    float decompressionFactor = header->CompressionLevel / targetRatio;
    
    uint8_t* pcmData = static_cast<uint8_t*>(audioData) + sizeof(AudioAssetHeader);
    size_t pcmSize = dataSize - sizeof(AudioAssetHeader);
    
    // Dynamic range expansion
    for (size_t i = 0; i < pcmSize; i += 2) {
        if (i + 1 >= pcmSize) break;
        
        int16_t* sample = reinterpret_cast<int16_t*>(pcmData + i);
        float expanded = static_cast<float>(*sample);
        
        if (expanded > 0) {
            expanded = expanded * decompressionFactor;
        } else {
            expanded = expanded * decompressionFactor;
        }
        
        if (expanded > 32767.0f) expanded = 32767.0f;
        if (expanded < -32768.0f) expanded = -32768.0f;
        
        *sample = static_cast<int16_t>(expanded);
    }
    
    header->CompressionLevel = targetRatio;
    return true;
}

void AudioProcessor::RegisterStream(uint64_t assetHash) {
    EnterCriticalSection(&m_Lock);
    
    for (uint32_t i = 0; i < Config::MaxAudioStreams; ++i) {
        if (m_Streams[i].AssetHash == 0) {
            m_Streams[i].AssetHash = assetHash;
            m_Streams[i].CurrentBitrate = Config::OriginalBitrate;
            m_Streams[i].TargetBitrate = Config::TargetBitrate;
            m_Streams[i].QualityMultiplier = 1.0f;
            m_Streams[i].IsEnhanced = false;
            m_ActiveStreams++;
            break;
        }
    }
    
    LeaveCriticalSection(&m_Lock);
}

void AudioProcessor::UnregisterStream(uint64_t assetHash) {
    EnterCriticalSection(&m_Lock);
    
    for (uint32_t i = 0; i < Config::MaxAudioStreams; ++i) {
        if (m_Streams[i].AssetHash == assetHash) {
            memset(&m_Streams[i], 0, sizeof(AudioStreamInfo));
            if (m_ActiveStreams > 0) m_ActiveStreams--;
            break;
        }
    }
    
    LeaveCriticalSection(&m_Lock);
}

bool AudioProcessor::ApplyDSPChain(void* audioData, size_t dataSize) {
    // Placeholder for additional DSP processing
    return true;
}

bool AudioProcessor::UpsampleAudio(void* audioData, size_t dataSize, uint32_t targetRate) {
    // Placeholder for upsampling
    return true;
}

float AudioProcessor::CalculateQualityFactor(uint32_t originalBitrate, uint32_t targetBitrate) {
    if (originalBitrate == 0) return 1.0f;
    float ratio = static_cast<float>(targetBitrate) / static_cast<float>(originalBitrate);
    return 1.0f + (ratio - 1.0f) * 0.15f;
}
