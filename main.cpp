#include "includes.hpp"

DWORD WINAPI initialize(HINSTANCE module) {
    interfaces::initialize();
    hooks::initialize();

    return TRUE;
}

DWORD WINAPI deinitialize() {
    hooks::shutdown();

    return TRUE;
}


BOOL APIENTRY DllMain(HMODULE module, DWORD  reason_for_call, LPVOID reserved) {

    switch (reason_for_call) {
    case DLL_PROCESS_ATTACH: {
        DisableThreadLibraryCalls(module);
        CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)initialize, NULL, NULL, NULL);
        break;
    }

    case DLL_PROCESS_DETACH: {
        deinitialize();
        break;
    }
    }

    return TRUE;
}