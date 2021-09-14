/* date = July 29th 2020 1:45 pm */

#ifndef PLATFORM_H
#define PLATFORM_H

#if defined(_WIN32)
#define PLATFORM_WIN32
#define PLATFORM_NAME "Windows"
#endif

#if defined(APP_TYPE_WINDOW) && defined(APP_TYPE_CONSOLE)
#error "Only one app type must be defined"
#endif

#if !defined(APP_TYPE_WINDOW) && !defined(APP_TYPE_CONSOLE)
#define APP_TYPE_CONSOLE
#endif

struct key_state
{
    u8 State;
};

enum input_key {
    KEY_NONE,
    
    KEY_SHIFT,
    KEY_CONTROL,
    KEY_ALT,
    KEY_CAPS,
    
    KEY_TAB,
    KEY_ENTER,
    KEY_ESC,
    
    KEY_SPACE,
    
    KEY_HOME,
    KEY_END,
    
    KEY_PLUS,
    KEY_MINUS,
    KEY_EQUALS,
    KEY_BACKSPACE,
    
    KEY_LEFT,
    KEY_RIGHT,
    KEY_UP,
    KEY_DOWN,
    
    KEY_1,
    KEY_2,
    KEY_3,
    KEY_4,
    KEY_5,
    KEY_6,
    KEY_7,
    KEY_8,
    KEY_9,
    KEY_0,
    
    KEY_A,
    KEY_B,
    KEY_C,
    KEY_D,
    KEY_E,
    KEY_F,
    KEY_G,
    KEY_H,
    KEY_I,
    KEY_J,
    KEY_K,
    KEY_L,
    KEY_M,
    KEY_N,
    KEY_O,
    KEY_P,
    KEY_Q,
    KEY_R,
    KEY_S,
    KEY_T, 
    
    KEY_U,
    KEY_V,
    KEY_W,
    KEY_X,
    KEY_Y,
    KEY_Z, 
    
    KEY_F1,
    KEY_F2,
    KEY_F3,
    KEY_F4,
    KEY_F5,
    KEY_F6,
    KEY_F7,
    KEY_F8,
    KEY_F9,
    KEY_F10,
    KEY_F11,
    KEY_F12,
    
    KEY_BRACKET_LEFT,
    KEY_BRACKET_RIGHT,
    KEY_SEMICOLON,
    KEY_APOSTROPHE,
    KEY_BACKSLASH,
    
    KEY_GRAVE,
    KEY_COMMA,
    KEY_PERIOD,
    KEY_SLASH,
    
    KEY_MOUSE1,
    KEY_MOUSE2,
    KEY_MOUSE3,
    KEY_MOUSE4,
    KEY_MOUSE5,
    
    KEY_COUNT, 
};


char* KeyboardKeyToChar[] =
{
    "NONE",
    
    "SHIFT",
    "CONTROL",
    "ALT",
    "CAPS",
    
    "TAB",
    "ENTER",
    "ESC",
    
    "SPACE",
    
    "HOME",
    "END",
    
    "PLUS",
    "MINUS",
    "EQUALS",
    "BACKSPACE",
    
    "LEFT",
    "RIGHT",
    "UP",
    "DOWN",
    
    "1",
    "2",
    "3",
    "4",
    "5",
    "6",
    "7",
    "8",
    "9",
    "0",
    
    "A",
    "B",
    "C",
    "D",
    "E",
    "F",
    "G",
    "H",
    "I",
    "J",
    "K",
    "L",
    "M",
    "N",
    "O",
    "P",
    "Q",
    "R",
    "S",
    "T", 
    
    "U",
    "V",
    "W",
    "X",
    "Y",
    "Z", 
    
    "F1",
    "F2",
    "F3",
    "F4",
    "F5",
    "F6",
    "F7",
    "F8",
    "F9",
    "F10",
    "F11",
    "F12",
    
    "BRACKETLEFT",
    "BRACKETRIGHT",
    "SEMICOLON",
    "APOSTROPHE",
    "BACKSLASH",
    
    "GRAVE",
    "COMMA",
    "PERIOD",
    "SLASH", 
    
    "COUNT", 
};

#define KEY_PRESSED 0x1
#define KEY_TRANSITION_COUNT_MASK 0xfe




struct gamepad_input {
    struct game_pad_input {
        b32 IsConnected;
        b32 IsAnalog;
        f32 StickAverageX;
        f32 StickAverageY;
        
        union {
            key_state Buttons[8];
            struct {
                key_state DPadUp;
                key_state DPadDown;
                key_state DPadLeft;
                key_state DPadRight;
                
                key_state ActionUp;
                key_state ActionDown;
                key_state ActionLeft;
                key_state ActionRight;
            };
        };
    };
};

struct mouse_input {
    v2 P;
    v2 dP;
    v2 LastP;
    v2 ScreenP;
    v2 dScreenP;
    v2 ClipP;
    v2 dClipP;
    
    f32 Z;
    f32 dZ;
    f32 LastZ;
};

struct input {
    mouse_input Mouse;
    key_state Buttons[KEY_COUNT];
    char TextInputThisFrame[32];
    u32 TextInputCount;
};

enum platform_event_type {
    PLATFORM_EVENT_TYPE_NONE,
    PLATFORM_EVENT_TYPE_WINDOW,
};

enum platform_event_window_type {
    PLATFORM_EVENT_WINDOW_TYPE_NONE,
    PLATFORM_EVENT_WINDOW_TYPE_RESIZED,
    PLATFORM_EVENT_WINDOW_TYPE_ACTIVE,
    PLATFORM_EVENT_WINDOW_TYPE_INACTIVE,
    PLATFORM_EVENT_WINDOW_TYPE_MINIMIZED,
    PLATFORM_EVENT_WINDOW_TYPE_MAXIMIZED,
    PLATFORM_EVENT_WINDOW_TYPE_RESTORED,
    PLATFORM_EVENT_WINDOW_TYPE_POSITION,
    PLATFORM_EVENT_WINDOW_TYPE_CLOSED,
    PLATFORM_EVENT_WINDOW_TYPE_SHOWN,
    PLATFORM_EVENT_WINDOW_TYPE_HIDDEN,
};

struct platform_event_window {
    platform_event_window_type Type;
    union {
        v2u Size;
        v2u Position;
    };
};

struct platform_event {
    u32 Type;
    union {
        platform_event_window Window;
    };
};

struct platform_event_queue {
    platform_event Events[2048];
    u32 EventCount;
};

struct platform_window {
    u32 Width;
    u32 Height;
    u32 ClientWidth;
    u32 ClientHeight;
    u32 RefreshRate;
    b32 Fullscreen;
    b32 IsRunning;
    char* Title;
};

struct platform_window_settings {
    u32 Width;
    u32 Height;
    char* Title;
    b32 StartFullscreen;
};

struct platform_time {
    u64 ClockFrequency;
    u64 FrameBeginClock;
    u64 FrameEndClock;
    f32 FrameMS;
    f32 FramesPerSecond;
    f32 TimeDeltaInSeconds;
    f32 SecondsSinceStartup;
};

struct platform_system_info {
    u32 ProcessorCount;
};

struct platform_init_settings {
    platform_window_settings Window;
};

struct platform_sound_output {
    s32 SamplesPerSecond;
    s32 SampleCount;
    s16* Samples;
};

struct platform_state {
    platform_system_info SystemInfo;
    platform_window Window;
    platform_time Time;
    input Input;
    platform_sound_output SoundOutput;
    
    platform_event_queue Queue;
    struct internal_platform_state* Internal;
    
    //NOTE(jesse): Defined in respective platform files ie. win32_platform.h
};

#define PlatformNoFileErrors(Handle) ((Handle)->NoErrors)
struct platform_file_info {
    char* FilePath;
    u64 FileSize;
    u64 FileTime;
    void* Platform;
    b32 NoErrors;
};

struct platform_loaded_code {
    platform_file_info Info;
    b32 IsValid;
    
    u32 FunctionCount;
    char** FunctionNames;
    void** Functions;
};

// NOTE(Jesse): Functions Proveded by the OS platform file
//
//
static b32                      PlatformInit();
static void*                    PlatformAllocateMemory(usize MemorySize);
static void                     PlatformFreeMemory(void* Address, usize MemorySize);
static void*                    PlatformReserveMemory(usize MemorySize);
static void                     PlatformCommitMemory(void* Address, usize Size);
static void                     PlatformDecommitMemory(void* Address, usize Size);
static platform_event_queue*    PlatformProcessMessages();
static u64                      PlatformGetCycleCount();
static void                     PlatformSleep(u32 MS);
static b32                      PlatformMakeDirectory(char* Path);
static platform_file_info       PlatformOpenFile(char* Path);
static void                     PlatformOpenFile(platform_file_info* Info);
static void                     PlatformCloseFile(platform_file_info* Info);
static u64                      PlatformGetLastWriteTime(char* Path);
static void                     PlatformReadFile(platform_file_info* Info, u64 Size, void* Dest);
static u64                      PlatformGetFileSize(platform_file_info* Info);
static u64                      PlatformGetFileTime(platform_file_info* Info);
static b32                      PlatformDeleteFile(char* Name);
static b32                      PlatformCopyFile(char* OldName, char* NewName, b32 FailIfExists);
static void                     PlatformSaveToFile(char* Path, void* Data, usize Size);
static platform_window          PlatformCreateWindow(platform_window_settings Settings);
static b32                      PlatformSetWindowTitle(char* Title);
static void                     PlatformSetFullScreen(b32 Fullscreen);
static void                     PlatformToggleFullscreen();
static void                     PlatformCreateSoundBuffer();
static void                     PlatformFillSoundBuffer(u32 SamplesToWrite, platform_sound_output* SourceBuffer);

// NOTE(Jesse): Functions Provided by Platform.cpp
static void                     PushPlatformEvent(platform_event Event);
static void                     PushPlatformWindowEvent(u32 WindowEventType, v2u Size = V2u(0, 0), v2u Position = V2u(0, 0));
static f64                      TimeInSeconds(u64 ClockDifference);
static f64                      TimeInMS(u64 ClockDifference);
static void                     UpdateTime();
static u32                      GetCurrentWindowWidth();
static u32                      GetCurrentWindowHeight();
static v2u                      GetCurrentWindowSize();
static void                     ClearKeys();
static void                     PrepInputBeforeMessages();
static void                     ProcessInputMessage(u32 KeyCode, u8 Pressed);
static void                     ProcessCharMessage(char UTF8Char);
static key_state                GetStateFromKeyCode(u32 KeyCode);
static b32                      IsDown(u32 KeyCode);
static b32                      WasPressed(u32 KeyCode);
static b32                      WentUp(u32 KeyCode);
global_variable platform_state* GLOBALPlatformState = 0;



#endif //PLATFORM_H
