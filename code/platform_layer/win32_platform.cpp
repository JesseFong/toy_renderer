
static void* PlatformAllocateMemory(usize Size) {
    return VirtualAlloc(0, Size, MEM_COMMIT|MEM_RESERVE, PAGE_READWRITE);
}

static void PlatformFreeMemory(void* Address, usize MemorySize) {
    VirtualFree(Address, MemorySize, MEM_RELEASE);
}

static void* PlatformReserveMemory(usize Size) {
    return VirtualAlloc(0, Size, MEM_RESERVE, PAGE_READWRITE);
}

static void PlatformCommitMemory(void* Address, usize Size) {
    VirtualAlloc(Address, Size, MEM_COMMIT, PAGE_READWRITE);
}

static void PlatformDecommitMemory(void* Address, usize Size) {
    VirtualFree(Address, Size, MEM_DECOMMIT);
}

static u64
PlatformGetFileSize(platform_file_info* Info) {
    u64 FileSize = {};
    if(PlatformNoFileErrors(Info)) {
        HANDLE Win32Handle = *(HANDLE *)&Info->Platform;
        LARGE_INTEGER Win32FileSize = {};
        GetFileSizeEx(Win32Handle, &Win32FileSize);
        FileSize = (u64)Win32FileSize.QuadPart;
    } else {
        DebugOut("PlatformGetFileSize Failed");
    }
    return FileSize;
}

static u64
PlatformGetFileTime(platform_file_info* Info) {
    u64 FileTime = {};
    if(PlatformNoFileErrors(Info)) {
        HANDLE Win32Handle = *(HANDLE *)&Info->Platform;
        FILETIME LastWriteTime = {};
        GetFileTime(Win32Handle, 0, 0, &LastWriteTime);
        FileTime = LastWriteTime.dwHighDateTime;
        FileTime = FileTime << 32;
        FileTime = FileTime|LastWriteTime.dwLowDateTime;
    } else {
        DebugOut("PlatformGetFileTime Failed");
    }
    return FileTime;
}

static b32
PlatformMakeDirectory(char* Path) {
    wchar_t WidePath[WIN32_MAX_BUFFER_LENGTH] = {};
    UTF8StringToWideString(Path, WidePath, WIN32_MAX_BUFFER_LENGTH);
    
    SECURITY_ATTRIBUTES Attrib = {};
    Attrib.nLength = sizeof(SECURITY_ATTRIBUTES);
    Attrib.lpSecurityDescriptor = NULL;
    Attrib.bInheritHandle = true;
    
    return CreateDirectoryW(WidePath, &Attrib);
}

static platform_file_info
PlatformOpenFile(char* Path) {
    platform_file_info Result = {};
    Result.FilePath = Path;
    wchar_t WidePath[WIN32_MAX_BUFFER_LENGTH] = {};
    UTF8StringToWideString(Path, WidePath, WIN32_MAX_BUFFER_LENGTH);
    
    Result.Platform = (HANDLE)CreateFileW(WidePath, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0);
    Result.NoErrors = (Result.Platform != INVALID_HANDLE_VALUE);
    
    if(Result.NoErrors) {
        Result.FileSize = PlatformGetFileSize(&Result);
        Result.FileTime = PlatformGetFileTime(&Result);
    } else {
        DebugOut("File Open Failed");
    }
    
    return Result;
}

static void
PlatformOpenFile(platform_file_info* Info) {
    *Info = PlatformOpenFile(Info->FilePath);
}

static void
PlatformCloseFile(platform_file_info* Info) {
    HANDLE Win32Handle = *(HANDLE *)&Info->Platform;
    if(Win32Handle != INVALID_HANDLE_VALUE) {
        CloseHandle(Win32Handle);
        Info->Platform = 0;
    } else {
        DebugOut("File Close Failed");
    }
}

static u64
PlatformGetLastWriteTime(char* Path) {
    u64 Result = {};
    platform_file_info Info = PlatformOpenFile(Path);
    Result =  Info.FileTime;
    PlatformCloseFile(&Info);
    return(Result);
}

static void
PlatformReadFile(platform_file_info* Info, u64 Size, void* Dest) {
    if(PlatformNoFileErrors(Info)) {
        HANDLE Win32Handle = *(HANDLE *)&Info->Platform;
        u32 BytesToRead = SafeTruncateToU32(Size);
        DWORD BytesRead;
        
        if(ReadFile(Win32Handle, Dest, BytesToRead, &BytesRead, 0) && BytesToRead == BytesRead) {
            
        } else {
            DebugOut("File Read Failed");
        }
    } else {
        DebugOut("File Read Failed");
    }
}

static b32
PlatformDeleteFile(char* Name) {
    b32 Result = DeleteFile(Name);
    return Result;
}

static b32
PlatformRenameFile(char* OldName, char* NewName) {
    b32 Result = rename(OldName, NewName);
    return Result;
}

static void
PlatformSaveToFile(char* Path, void* Data, usize Size) {
    
    HANDLE File = {};
    wchar_t WidePath[WIN32_MAX_BUFFER_LENGTH] = {};
    UTF8StringToWideString(Path, WidePath, WIN32_MAX_BUFFER_LENGTH);
    
    File = (HANDLE)CreateFileW(WidePath, GENERIC_READ|GENERIC_WRITE, 0, 0, CREATE_ALWAYS, 0, 0);
    
    if(File != INVALID_HANDLE_VALUE) {
        
        DWORD BytesWritten = 0;
        DWORD WriteSize = (DWORD)Size;
        if(!WriteFile(File, Data, WriteSize, &BytesWritten, 0)) {
            DebugOut("Save File Failed");
        }
        CloseHandle(File);
        
    } else {
        DebugOut("Save File Failed");
    }
}

static b32
PlatformCopyFile(char* Path, char* NewPath, b32 FailIfExists) {
    b32 Result = CopyFile(Path, NewPath, FailIfExists);
    return Result;
}

static b32
PlatformUnloadDLL(platform_loaded_code* CodeInfo) {
    
    if(CodeInfo->Info.Platform) {
        CodeInfo->Info.Platform = 0;
    }
    CodeInfo->IsValid = false;
    ClearArrayCount(CodeInfo->Functions, CodeInfo->FunctionCount);
}

static b32
PlatformLoadDLL(platform_loaded_code* LoadedCode) {
    b32 Result = false;
    
    wchar_t WidePath[WIN32_MAX_BUFFER_LENGTH] = {};
    UTF8StringToWideString(LoadedCode->Info.FilePath, WidePath, WIN32_MAX_BUFFER_LENGTH);
    LoadLibraryW(WidePath);
    
    LoadedCode->Info.Platform = (void*)LoadLibraryW(WidePath);
    if(LoadedCode->Info.Platform) {
        
        LoadedCode->IsValid = true;
        for(u32 FunctionIndex = 0;
            FunctionIndex < LoadedCode->FunctionCount;
            FunctionIndex++) {
            
            void* Function = GetProcAddress((HMODULE)LoadedCode->Info.Platform, LoadedCode->FunctionNames[FunctionIndex]);
            if(Function) {
                LoadedCode->Functions[FunctionIndex] = Function;
            } else {
                LoadedCode->IsValid = false;
            }
        }
    }
    
    if(!LoadedCode->IsValid) {
        PlatformUnloadDLL(LoadedCode);
    }
    return Result;
}

static void
PlatformSleep(u32 MS) {
    Sleep(MS);
}

static platform_event_queue*
PlatformProcessMessages() {
    
    platform_event_queue* Queue = &GLOBALPlatformState->Queue;
    Queue->EventCount = 0;
    //NOTE(jesse): Order Matters for mousewheel!!!!!!!
    PrepInputBeforeMessages();
    Win32ProcessMouseMessages();
    Win32ProcessPendingMessages();
    
    return Queue;
}

static HWND
Win32GetWindowHandle() {
    HWND Handle = GLOBALPlatformState->Internal->Window.Handle;
    return Handle;
}

static b32
PlatformSetWindowTitle(char* Title) {
    wchar_t WindowTitleBuffer[WIN32_MAX_BUFFER_LENGTH] = {};
    HWND Handle = Win32GetWindowHandle();
    UTF8StringToWideString(Title, WindowTitleBuffer, WIN32_MAX_BUFFER_LENGTH);
    
    return SetWindowTextW(Handle, WindowTitleBuffer);
}


static v2u
PlatformGetWindowDimension() {
    v2u Result = {};
    internal_platform_state* InternalState = GLOBALPlatformState->Internal;
    RECT ClientRect;
    GetClientRect(InternalState->Window.Handle, &ClientRect);
    Result.Width = ClientRect.right - ClientRect.left;
    Result.Height = ClientRect.bottom - ClientRect.top;
    return Result;
}

static void
PlatformSetFullscreen(b32 Fullscreen) {
    
    platform_state* PlatformState = GLOBALPlatformState;
    WINDOWPLACEMENT LastWindowPlacement = {};
    
    HWND Handle = PlatformState->Internal->Window.Handle;
    LONG WindowStyle = GetWindowLongW(Handle, GWL_STYLE);
    
    if(Fullscreen) {
        
        if(PlatformState->Window.Fullscreen) {
            Assert(WindowStyle & WS_OVERLAPPEDWINDOW);
            return;
        }
        
        MONITORINFO MonitorInfo = {};
        MonitorInfo.cbSize = sizeof(MONITORINFO);
        
        b32 PlacementBool = GetWindowPlacement(Handle, &LastWindowPlacement);
        b32 InfoBool = GetMonitorInfoW(MonitorFromWindow(Handle, MONITOR_DEFAULTTOPRIMARY), &MonitorInfo);
        if(PlacementBool && InfoBool) {
            
            SetWindowLong(Handle, GWL_STYLE, WindowStyle & ~WS_OVERLAPPEDWINDOW); 
            
            SetWindowPos(Handle, HWND_TOP,
                         MonitorInfo.rcMonitor.left,
                         MonitorInfo.rcMonitor.top,
                         MonitorInfo.rcMonitor.right -
                         MonitorInfo.rcMonitor.left,
                         MonitorInfo.rcMonitor.bottom -
                         MonitorInfo.rcMonitor.top,
                         SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
            
            PlatformState->Window.Fullscreen = true;
        }
    } else {
        
        if(!PlatformState->Window.Fullscreen) {
            Assert(!(WindowStyle & WS_OVERLAPPEDWINDOW));
            return;
        }
        
        SetWindowLong(Handle, GWL_STYLE,
                      WindowStyle | WS_OVERLAPPEDWINDOW);
        SetWindowPlacement(Handle, &LastWindowPlacement);
        SetWindowPos(Handle, 0, 0, 0, PlatformState->Window.Width, PlatformState->Window.Height,
                     SWP_NOMOVE | SWP_NOZORDER |
                     SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
        
        // TODO(NAME): Handle Position after coming out of fullscreen
        
        PlatformState->Window.Fullscreen = false;
    }
}

static void
PlatformToggleFullscreen() {
    platform_state* PlatformState = GLOBALPlatformState;
    PlatformSetFullscreen(!PlatformState->Window.Fullscreen);
}


static void
PlatformSetWindowAlpha(f32 Alpha) {
    
    u8 WindowsAlpha = (u8)Lerp(0.0f, Clamp01(Alpha), 255.0f);
    HWND Handle = Win32GetWindowHandle();
    SetLayeredWindowAttributes(Handle, 0, WindowsAlpha, LWA_ALPHA);
}

static platform_window
PlatformCreateWindow(platform_window_settings Settings) {
    
    internal_platform_state* InternalState = GLOBALPlatformState->Internal;
    platform_window Result = {};
    
    WNDCLASSEXW WindowClass = {};
    WindowClass.cbSize = sizeof(WindowClass);
    WindowClass.style = CS_VREDRAW|CS_HREDRAW;
    WindowClass.lpfnWndProc = WNDPROC(Win32WindowCallback);
    WindowClass.cbClsExtra = 0;
    WindowClass.hInstance = GetModuleHandleW(NULL);
    WindowClass.hIcon = 0;// TODO(Jesse): 
    WindowClass.hIconSm = 0;// TODO(Jesse): 
    WindowClass.hCursor = LoadCursorW(0, (LPCWSTR)IDC_ARROW);
    WindowClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    WindowClass.lpszMenuName = 0;// TODO(Jesse): 
    WindowClass.lpszClassName = L"WindowClassW";
    WindowClass.style |= CS_OWNDC;
    
    if(RegisterClassExW(&WindowClass)) {
        
        wchar_t WindowTitleBuffer[WIN32_MAX_BUFFER_LENGTH] = {};
        if(Settings.Title) {
            UTF8StringToWideString(Settings.Title, WindowTitleBuffer, WIN32_MAX_BUFFER_LENGTH);
        } else {
            wchar_t* DefaultTitle = WIN32_DEFAULT_TITLE;
            Win32StringCopy(DefaultTitle, WindowTitleBuffer, ArrayCount(WindowTitleBuffer));
        }
        wchar_t* WindowTitle = WindowTitleBuffer;
        
        DWORD EXStyle = WS_EX_OVERLAPPEDWINDOW; //WS_EX_TOPMOST
        DWORD Style = WS_OVERLAPPED | WS_VISIBLE | WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU;
        RECT WindowSize = {};
        {
            HDC ScreenDeviceContext = GetWindowDC(0);
            int ScreenWidth = GetDeviceCaps(ScreenDeviceContext, HORZRES);
            int ScreenHeight = GetDeviceCaps(ScreenDeviceContext, VERTRES);
            
            int WindowWidth = Settings.Width;
            int WindowHeight = Settings.Height;
            
            int WindowLeft = (ScreenWidth - WindowWidth) / 2;
            int WindowTop = (ScreenHeight - WindowHeight) / 2;
            
            WindowSize = {WindowLeft, WindowTop, WindowLeft + WindowWidth, WindowTop + WindowHeight};
            AdjustWindowRect(&WindowSize, Style, FALSE);
            
            ReleaseDC(0, ScreenDeviceContext);
        }
        
        int ActualWidth = WindowSize.right - WindowSize.left;
        int ActualHeight = WindowSize.bottom - WindowSize.top;
        
        HWND Window = CreateWindowExW(EXStyle,
                                      WindowClass.lpszClassName,
                                      WindowTitle,
                                      Style,
                                      WindowSize.left,
                                      WindowSize.top,
                                      ActualWidth,
                                      ActualHeight,
                                      0,
                                      0,
                                      WindowClass.hInstance,
                                      0);
        
        //Default window pos/style
        SetWindowLong(Window, GWL_STYLE, (Style | WS_OVERLAPPEDWINDOW) & ~WS_THICKFRAME);
        SetWindowPos(Window, 0, 0, 0, ActualWidth, ActualHeight, SWP_NOMOVE | SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
        SetWindowLong(Window, GWL_EXSTYLE, WS_EX_LAYERED);
        
        DWORD Error = GetLastError();
        ShowWindow(Window, SW_SHOW);
        
        InternalState->Window.Handle = Window;
        InternalState->Window.DC = GetDC(Window);
        InternalState->Window.Width = ActualWidth;
        InternalState->Window.Height = ActualHeight;
        
        RECT ClientRect;
        GetClientRect(Window, &ClientRect);
        Result.ClientWidth = ClientRect.right - ClientRect.left;
        Result.ClientHeight = ClientRect.bottom - ClientRect.top;
        
        Result.Width = ActualWidth;
        Result.Height = ActualHeight;
        Result.Title = Settings.Title;
        Result.Fullscreen = false;
        
        if(Settings.StartFullscreen) {
            PlatformSetFullscreen(true);
            Result.Fullscreen = true;
        }
        
        Result.IsRunning = true;
        
    } else {
        Assert(!"Could not register class");
    }
    return Result;
}

static platform_system_info
PlatformGetSystemInfo() {
    platform_system_info Result = {};
    SYSTEM_INFO Info;
    GetSystemInfo(&Info);
    
    Result.ProcessorCount = Info.dwNumberOfProcessors;
    
    return Result;
}

static void
Win32InitWASAPI(win32_sound_output *SoundOutput) {
    
    if(!(CoInitializeEx(NULL, COINIT_SPEED_OVER_MEMORY) == S_OK)) {
        Assert(!"CoInitialize Failed");
    }
    
    IMMDeviceEnumerator* Enumerator;
    if(!(CoCreateInstance(__uuidof(MMDeviceEnumerator), nullptr, CLSCTX_ALL, IID_PPV_ARGS(&Enumerator)) == S_OK)) {
        Assert(!"CoCreateInstance Failed");
    }
    
    IMMDevice* Device;
    if(!(Enumerator->GetDefaultAudioEndpoint(eRender, eConsole, &Device) == S_OK)) {
        Assert(!"GetDefaultAudioEndpoint");
    }
    
    if(!(Device->Activate(__uuidof(IAudioClient), CLSCTX_ALL, NULL, (LPVOID*)&SoundOutput->SoundClient) == S_OK)) {
        Assert(!"Device Activation Failed");
    }
    
    WAVEFORMATEXTENSIBLE WaveFormat;
    
    WaveFormat.Format.cbSize = sizeof(WaveFormat);
    WaveFormat.Format.wFormatTag = WAVE_FORMAT_EXTENSIBLE;
    WaveFormat.Format.wBitsPerSample = 16;
    WaveFormat.Format.nChannels = 2;
    WaveFormat.Format.nSamplesPerSec = (DWORD)SoundOutput->SamplesPerSecond;
    WaveFormat.Format.nBlockAlign = (WORD)(WaveFormat.Format.nChannels * WaveFormat.Format.wBitsPerSample / 8);
    WaveFormat.Format.nAvgBytesPerSec = WaveFormat.Format.nSamplesPerSec * WaveFormat.Format.nBlockAlign;
    WaveFormat.Samples.wValidBitsPerSample = 16;
    WaveFormat.dwChannelMask = KSAUDIO_SPEAKER_STEREO;
    WaveFormat.SubFormat = KSDATAFORMAT_SUBTYPE_PCM;
    
    REFERENCE_TIME BufferDuration = 10000000ULL * SoundOutput->SecondaryBufferSize / SoundOutput->SamplesPerSecond;
    // NOTE(Jesse):Buffer size in 100 nanoseconds
    if(!(SoundOutput->SoundClient->Initialize(AUDCLNT_SHAREMODE_SHARED, AUDCLNT_STREAMFLAGS_NOPERSIST, BufferDuration, 0, &WaveFormat.Format, nullptr) == S_OK)) {
        Assert(!"Clent Initialization Failed");
    }
    
    if(!(SoundOutput->SoundClient->GetService(IID_PPV_ARGS(&SoundOutput->SoundRenderClient)) == S_OK)) {
        Assert(!"Get Service failed, Whatever that means");
    }
    
    u32 SoundFrameCount;
    if(!(SoundOutput->SoundClient->GetBufferSize(&SoundFrameCount) == S_OK)) {
        Assert(!"Buffer Size Retreval Failed");
    }
    
    u32 SamplesToWrite = SoundOutput->SecondaryBufferSize;
    BYTE* SoundBufferData;
    if(SUCCEEDED(SoundOutput->SoundRenderClient->GetBuffer(SamplesToWrite, &SoundBufferData))) {
        s16 SourceSample = 256;
        s16* DestSample = (s16*)SoundBufferData;
        for(u32 SampleIndex = 0;
            SampleIndex < SamplesToWrite;
            ++SampleIndex) {
            *DestSample++ = SourceSample; 
            *DestSample++ = SourceSample; 
            ++SoundOutput->RunningSampleIndex;
        }
        
        SoundOutput->SoundRenderClient->ReleaseBuffer(SamplesToWrite, 0);
    }
    
    // Check if we got what we requested (better would to pass this value back as real buffer size)
    Assert(SamplesToWrite <= SoundFrameCount);
}

static void
PlatformCreateSoundBuffer() {
    platform_state* PlatformState = GLOBALPlatformState;
    s32 SamplesPerSecond = 48000;
    u32 FramesOfAudioLatency = 5;
    PlatformState->Internal->SoundOutput.SamplesPerSecond = SamplesPerSecond;
    PlatformState->Internal->SoundOutput.BytesPerSample = sizeof(s16)*2;
    PlatformState->Internal->SoundOutput.SecondaryBufferSize = SamplesPerSecond;
    PlatformState->Internal->SoundOutput.LatencySampleCount = (u32)(FramesOfAudioLatency*(SamplesPerSecond / 60));
    PlatformState->SoundOutput.SamplesPerSecond = SamplesPerSecond;
    PlatformState->SoundOutput.Samples = (s16*)PlatformAllocateMemory(SamplesPerSecond);
    Win32InitWASAPI(&PlatformState->Internal->SoundOutput);
}


static void
PlatformFillSoundBuffer() {
    platform_state* PlatformState = GLOBALPlatformState;
    win32_sound_output* SoundOutput = &PlatformState->Internal->SoundOutput;
    
    u32 SamplesToWrite = 0;
    u32 SoundPaddingSize;
    if (SUCCEEDED(SoundOutput->SoundClient->GetCurrentPadding(&SoundPaddingSize))) {
        SamplesToWrite = (int)(SoundOutput->SecondaryBufferSize - SoundPaddingSize);
        if (SamplesToWrite > (u32)SoundOutput->LatencySampleCount) {
            SamplesToWrite = SoundOutput->LatencySampleCount;
        }
    }
    
    PlatformState->SoundOutput.SampleCount = SamplesToWrite;
    
    s16* SourceBuffer = PlatformState->SoundOutput.Samples;
    
    BYTE* SoundBufferData;
    if(SUCCEEDED(SoundOutput->SoundRenderClient->GetBuffer(SamplesToWrite, &SoundBufferData))) {
        s16* SourceSample = SourceBuffer;
        s16* DestSample = (s16*)SoundBufferData;
        for(u32 SampleIndex = 0;
            SampleIndex < SamplesToWrite;
            ++SampleIndex) {
            *DestSample++ = *SourceSample++; 
            *DestSample++ = *SourceSample++; 
            ++SoundOutput->RunningSampleIndex;
        }
        SoundOutput->SoundRenderClient->ReleaseBuffer(SamplesToWrite, 0);
    } else {
        Assert(!"Wasn't able to retreve buffer");
    }
}

struct program_args {
    int Count;
    char** ArgArray;
};

static u64
PlatformGetCycleCount() {
    u64 Result = {};
    LARGE_INTEGER Counter;
    QueryPerformanceCounter(&Counter);
    Result = (u64)Counter.QuadPart;
    return(Result);
}

static b32
PlatformInit() {
    platform_state* PlatformState = GLOBALPlatformState;
    b32 Result = false;
    
    LARGE_INTEGER PerformanceFrequency;
    QueryPerformanceFrequency(&PerformanceFrequency);
    
    PlatformState->Time.ClockFrequency = (u64)PerformanceFrequency.QuadPart;
    PlatformState->Time.FrameBeginClock = PlatformGetCycleCount();
    
    Result = true;
    return(Result);
}

int APIENTRY
wWinMain(HINSTANCE Instance, HINSTANCE PrevInstance, LPWSTR CommandLine, int ShowCode) {
    program_args Args = {};
    int result = main(Args.Count, Args.ArgArray);
    ExitProcess(0);
}

#if 0
void __stdcall WinmainCRTStartup(void) {
    LPSTR argsA = GetCommandLineA();
	int result = WinMain(GetModuleHandleA(0), 0, argsA, 0);
	ExitProcess(result);
}
#endif