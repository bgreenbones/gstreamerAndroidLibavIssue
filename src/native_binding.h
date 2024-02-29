#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#if _WIN32
#include <windows.h>
#else
#include <pthread.h>
#include <unistd.h>
#endif
#include <string.h>

#if _WIN32
#define FFI_PLUGIN_EXPORT __declspec(dllexport)
#else
#define FFI_PLUGIN_EXPORT
#endif


#ifdef __cplusplus
extern "C" {
#endif


typedef struct _SelectOptions {
    char** m_vOptions;
    size_t m_iSize;
} SelectOptions;

FFI_PLUGIN_EXPORT void Init(void);
FFI_PLUGIN_EXPORT void SetupPipeline(void);
FFI_PLUGIN_EXPORT void StartPipeline(void);
FFI_PLUGIN_EXPORT void FreeResource(void);
FFI_PLUGIN_EXPORT SelectOptions GetDeviceOptions(void);
FFI_PLUGIN_EXPORT void SetRenderFunction(void (*RenderFunction)(uint8_t*));
FFI_PLUGIN_EXPORT void SetVideoDimensions(int iWidth, int iHeight);


// A very short-lived native function.
//
// For very short-lived functions, it is fine to call them on the main isolate.
// They will block the Dart execution while running the native function, so
// only do this for native functions which are guaranteed to be short-lived.
// FFI_PLUGIN_EXPORT intptr_t sum(intptr_t a, intptr_t b);

// A longer lived native function, which occupies the thread calling it.
//
// Do not call these kind of native functions in the main isolate. They will
// block Dart execution. This will cause dropped frames in Flutter applications.
// Instead, call these native functions on a separate isolate.
// FFI_PLUGIN_EXPORT intptr_t sum_long_running(intptr_t a, intptr_t b);




#ifdef __cplusplus
}
#endif