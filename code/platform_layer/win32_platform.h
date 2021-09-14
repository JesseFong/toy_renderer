/* date = July 29th 2020 1:25 pm */

#ifndef WIN32_PLATFORM_LAYER_H
#define WIN32_PLATFORM_LAYER_H

#include <windows.h>
#include <mmdeviceapi.h>
#include <audioclient.h>

#define MAX_FILENAME_LENGTH (MAX_PATH + 1)
#define MAX_PATH_LENGTH (MAX_PATH * 2 + 1)
#define PATH_SEPARATOR '\\'
#define FILE_EXT_SEPARATOR '.'
#define WIN32_DEFAULT_TITLE L"Default Window Title"

#define WIN32_MAX_NAME_LENGTH 256
#define WIN32_MAX_BUFFER_LENGTH 2048


#if defined(APP_TYPE_WINDOW)
#pragma comment(linker, "/SUBSYSTEM:WINDOWS")
#elif defined(APP_TYPE_CONSOLE)
#pragma comment(linker, "/SUBSYSTEM:CONSOLE")
#endif


#pragma comment(lib, "user32.lib")
#include "win32_opengl.h"
#if defined(USE_OPENGL)
#pragma comment(lib, "Opengl32.lib")
#pragma comment(lib, "Gdi32.lib")
#endif


int main(int argc, char**args);

struct win32_window_state {
    HWND Handle;
    HDC DC;
    u32 Width;
    u32 Height;
    b32 IsActive;
    
    char Title[WIN32_MAX_NAME_LENGTH];
};

struct win32_sound_output {
    IAudioClient* SoundClient;
    IAudioRenderClient* SoundRenderClient;
    s32 SamplesPerSecond;
    u32 RunningSampleIndex;
    s32 BytesPerSample;
    DWORD SecondaryBufferSize;
    DWORD SafetyBytes;
    s32 LatencySampleCount;
    b32 IsInitialized;
};

struct internal_platform_state {
    HINSTANCE Instance;
    win32_window_state Window;
    win32_sound_output SoundOutput;
};

static usize
Win32StringLength(char* String) {
    usize Result = 0;
    if(String) {
        while(*String++) {
            Result++;
        }
    }
    return(Result);
}

static usize
Win32StringLength(wchar_t* String) {
    usize Result = 0;
    int Length = lstrlenW(String);
    Result = (usize)Length;
    return(Result);
}

static char
WideCharToUTF8Char(wchar_t Source) {
    char UTF8Char = {};
    s32 Success = WideCharToMultiByte(CP_UTF8, 0, &Source, 1, &UTF8Char, 1, NULL, NULL);
    if(!Success) {
        Assert(!"Unable To Convert WideChar");
    }
    return UTF8Char;
}

static wchar_t
UTF8CharToWideChar(char Source) {
    wchar_t WideChar = {};
    
    s32 Success = MultiByteToWideChar(CP_UTF8, 0, &Source, 1, &WideChar, 1);
    if(!Success) {
        Assert(!"Unable To Convert Char");
    }
    return WideChar;
}

static char*
WideStringToUTF8String(wchar_t* Source, usize MaxSourceLength, char* Dest, usize MaxDestLength) {
    Assert(Source);
    Assert(Dest);
    
    s32 RequestedLength = WideCharToMultiByte(CP_UTF8, 0, Source, -1, NULL, 0, NULL, NULL);
    usize RequiredLength = RequestedLength + 1;
    
    Assert(MaxDestLength > RequiredLength);
    WideCharToMultiByte(CP_UTF8, 0, Source, (int)MaxSourceLength, Dest, (int)MaxDestLength, NULL, NULL);
    Dest[RequestedLength] = 0;
    
    return(&Dest[RequestedLength]);
}

static char*
WideStringToUTF8String(wchar_t* Source, char* Dest, usize MaxDestLength) {
    usize SourceLength = Win32StringLength(Source);
    return(WideStringToUTF8String(Source, SourceLength, Dest, MaxDestLength));
}

static wchar_t*
UTF8StringToWideString(char* Source, usize SourceLength, wchar_t* Dest, usize MaxDestLength) {
    Assert(Source);
    Assert(Dest);
    
    s32 RequestedLength = MultiByteToWideChar(CP_UTF8, 0, Source, -1, NULL, 0);
    usize RequiredLength = RequestedLength + 1;
    
    Assert(MaxDestLength > RequiredLength);
    MultiByteToWideChar(CP_UTF8, 0, Source, (int)SourceLength, Dest, (int)MaxDestLength);
    Dest[RequestedLength] = 0;
    
    return(&Dest[RequestedLength]);
}

static wchar_t*
UTF8StringToWideString(char* Source, wchar_t* Dest, usize MaxDestLength) {
    usize SourceLength = Win32StringLength(Source);
    return(UTF8StringToWideString(Source, SourceLength, Dest, MaxDestLength));
}

static void
Win32StringCopy(wchar_t* Source, wchar_t* Dest, usize DestMaxLength) {
    Assert(Source);
    Assert(Dest);
    
    lstrcpynW(Dest, Source, (int)DestMaxLength);
};


static void
DebugOut(wchar_t* String) {
    OutputDebugStringW(String);
}

static void
DebugOut(char* String) {
    OutputDebugStringA(String);
}


#endif //WIN32_PLATFORM_LAYER_H
