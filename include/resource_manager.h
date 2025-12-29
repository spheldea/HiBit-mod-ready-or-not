#pragma once
#include "pak_types.h"

class ResourceManager {
public:
    static ResourceManager& Instance();
    
    bool Initialize(const char* gamePath);
    void Shutdown();
    
    bool LoadResourcePak();
    
    const PakEntry* FindAudioEntry(uint64_t nameHash) const;
    bool ReadAssetData(const PakEntry* entry, void* buffer, size_t bufferSize) const;
    
    bool IsInitialized() const { return m_Initialized; }
    uint32_t GetAudioEntryCount() const { return m_AudioEntryCount; }

private:
    ResourceManager() = default;
    ~ResourceManager() = default;
    
    ResourceManager(const ResourceManager&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;
    
    uint64_t HashString(const char* str) const;
    bool ValidatePakHeader(const PakHeader& header) const;
    
    HANDLE      m_ResourcePakHandle = INVALID_HANDLE_VALUE;
    HANDLE      m_AudioPakHandle = INVALID_HANDLE_VALUE;
    PakHeader   m_ResourceHeader = {};
    PakEntry*   m_Entries = nullptr;
    uint32_t    m_AudioEntryCount = 0;
    char        m_GamePath[MAX_PATH] = {};
    bool        m_Initialized = false;
};
