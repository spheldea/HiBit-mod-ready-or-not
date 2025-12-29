#include "pch.h"
#include "resource_manager.h"
#include "config.h"

ResourceManager& ResourceManager::Instance() {
    static ResourceManager instance;
    return instance;
}

bool ResourceManager::Initialize(const char* gamePath) {
    if (m_Initialized) return true;
    
    strncpy(m_GamePath, gamePath, MAX_PATH - 1);
    m_GamePath[MAX_PATH - 1] = '\0';
    
    if (!LoadResourcePak()) return false;
    
    m_Initialized = true;
    return true;
}

void ResourceManager::Shutdown() {
    if (m_ResourcePakHandle != INVALID_HANDLE_VALUE) {
        CloseHandle(m_ResourcePakHandle);
        m_ResourcePakHandle = INVALID_HANDLE_VALUE;
    }
    
    if (m_AudioPakHandle != INVALID_HANDLE_VALUE) {
        CloseHandle(m_AudioPakHandle);
        m_AudioPakHandle = INVALID_HANDLE_VALUE;
    }
    
    if (m_Entries) {
        VirtualFree(m_Entries, 0, MEM_RELEASE);
        m_Entries = nullptr;
    }
    
    m_Initialized = false;
}

bool ResourceManager::LoadResourcePak() {
    char pakPath[MAX_PATH];
    snprintf(pakPath, MAX_PATH, "%s\\%s", m_GamePath, Config::ResourcePakName);
    
    m_ResourcePakHandle = CreateFileA(
        pakPath,
        GENERIC_READ,
        FILE_SHARE_READ,
        nullptr,
        OPEN_EXISTING,
        FILE_FLAG_SEQUENTIAL_SCAN,
        nullptr
    );
    
    if (m_ResourcePakHandle == INVALID_HANDLE_VALUE) return false;
    
    DWORD bytesRead;
    if (!ReadFile(m_ResourcePakHandle, &m_ResourceHeader, sizeof(PakHeader), &bytesRead, nullptr)) {
        return false;
    }
    
    return ValidatePakHeader(m_ResourceHeader);
}

const PakEntry* ResourceManager::FindAudioEntry(uint64_t nameHash) const {
    if (!m_Entries || m_AudioEntryCount == 0) return nullptr;
    
    // Binary search
    uint32_t left = 0;
    uint32_t right = m_AudioEntryCount - 1;
    
    while (left <= right) {
        uint32_t mid = left + (right - left) / 2;
        
        if (m_Entries[mid].NameHash == nameHash) {
            return &m_Entries[mid];
        }
        
        if (m_Entries[mid].NameHash < nameHash) {
            left = mid + 1;
        } else {
            if (mid == 0) break;
            right = mid - 1;
        }
    }
    
    return nullptr;
}

bool ResourceManager::ReadAssetData(const PakEntry* entry, void* buffer, size_t bufferSize) const {
    if (!entry || !buffer || bufferSize < entry->CompressedSize) return false;
    
    LARGE_INTEGER offset;
    offset.QuadPart = static_cast<LONGLONG>(entry->Offset);
    
    if (!SetFilePointerEx(m_ResourcePakHandle, offset, nullptr, FILE_BEGIN)) {
        return false;
    }
    
    DWORD bytesRead;
    return ReadFile(m_ResourcePakHandle, buffer, 
        static_cast<DWORD>(entry->CompressedSize), &bytesRead, nullptr) && 
        bytesRead == entry->CompressedSize;
}

uint64_t ResourceManager::HashString(const char* str) const {
    uint64_t hash = 14695981039346656037ULL;
    while (*str) {
        hash ^= static_cast<uint64_t>(*str++);
        hash *= 1099511628211ULL;
    }
    return hash;
}

bool ResourceManager::ValidatePakHeader(const PakHeader& header) const {
    return header.Magic[0] == 'P' && 
           header.Magic[1] == 'A' && 
           header.Magic[2] == 'K' && 
           header.Magic[3] == 'F';
}
