#include "pch.h"
#include "resource_manager.h"
#include "audio_processor.h"
#include "vorbis_proxy.h"

static HMODULE g_Module = nullptr;

static bool InitializeMod() {
    char gamePath[MAX_PATH];
    GetModuleFileNameA(nullptr, gamePath, MAX_PATH);
    
    char* lastSlash = strrchr(gamePath, '\\');
    if (lastSlash) *lastSlash = '\0';
    
    if (!VorbisProxy::Initialize()) {
        return false;
    }
    
    if (!ResourceManager::Instance().Initialize(gamePath)) {
        VorbisProxy::Shutdown();
        return false;
    }
    
    if (!AudioProcessor::Instance().Initialize()) {
        ResourceManager::Instance().Shutdown();
        VorbisProxy::Shutdown();
        return false;
    }
    
    return true;
}

static void ShutdownMod() {
    AudioProcessor::Instance().Shutdown();
    ResourceManager::Instance().Shutdown();
    VorbisProxy::Shutdown();
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID reserved) {
    switch (reason) {
        case DLL_PROCESS_ATTACH:
            g_Module = hModule;
            DisableThreadLibraryCalls(hModule);
            InitializeMod();
            break;
            
        case DLL_PROCESS_DETACH:
            if (reserved == nullptr) {
                ShutdownMod();
            }
            break;
    }
    
    return TRUE;
}


