
#include "platform_includes.h"

static void
ClearKeys() {
    ClearArray(GLOBALPlatformState->Input.Buttons);
    GLOBALPlatformState->Input.Mouse = {};
};

static void
PrepInputBeforeMessages() {
    
    input* Input = &GLOBALPlatformState->Input;
    u32 ScancodeIndex = 0;
    while(ScancodeIndex < KEY_COUNT) {
        Input->Buttons[ScancodeIndex].State &= KEY_PRESSED;
        ScancodeIndex++;
    }
    Input->TextInputCount = 0;
}

static void
ProcessInputMessage(u32 KeyCode, u8 Pressed) {
    key_state OldState = GLOBALPlatformState->Input.Buttons[KeyCode];
    u8 Transition = (OldState.State & KEY_PRESSED) != Pressed;
    u8 NewState = 0;
    NewState = OldState.State & KEY_TRANSITION_COUNT_MASK;
    NewState += (Transition << 1);
    NewState |= Pressed;
    GLOBALPlatformState->Input.Buttons[KeyCode].State = NewState;
}

static void
ProcessCharMessage(char UTF8Char) {
    input* Input = &GLOBALPlatformState->Input;
    Input->TextInputThisFrame[Input->TextInputCount++] = UTF8Char;
}

static key_state
GetStateFromKeyCode(u32 KeyCode) {
    
    key_state State = GLOBALPlatformState->Input.Buttons[KeyCode];
    return(State);
}

static b32 IsDown(u32 KeyCode) {
    key_state State = GetStateFromKeyCode(KeyCode); 
    b32 Result = State.State & KEY_PRESSED;
    return(Result);
}

static b32 WasPressed(u32 KeyCode) {
    key_state State = GetStateFromKeyCode(KeyCode); 
    b32 Result = (State.State & KEY_PRESSED) && (State.State & KEY_TRANSITION_COUNT_MASK);
    return(Result);
}

static b32 WentUp(u32 KeyCode) {
    key_state State = GetStateFromKeyCode(KeyCode); 
    b32 Result = !(State.State & KEY_PRESSED) && (State.State & KEY_TRANSITION_COUNT_MASK);
    return(Result);
}
static void
PushPlatformEvent(platform_event Event) {
    platform_event_queue* Queue = &GLOBALPlatformState->Queue;
    Assert(Queue->EventCount < ArrayCount(Queue->Events));
    Queue->Events[Queue->EventCount++] = Event;
}

static void
PushPlatformWindowEvent(u32 WindowEventType, v2u Size, v2u Position) {
    platform_event Event = {};
    Event.Type = PLATFORM_EVENT_TYPE_WINDOW;
    Event.Window.Type = (platform_event_window_type)WindowEventType;
    Event.Window.Size = Size;
    Event.Window.Position = Position;
    PushPlatformEvent(Event);
}

static f64
TimeInSeconds(u64 ClockDifference) {
    return(ClockDifference / (f64)GLOBALPlatformState->Time.ClockFrequency);
}

static f64
TimeInMS(u64 ClockDifference) {
    return((ClockDifference * 1000.0f) / (f64)GLOBALPlatformState->Time.ClockFrequency);
}

static void
UpdateTime() {
    platform_time* Time = &GLOBALPlatformState->Time;
    Time->FrameEndClock = PlatformGetCycleCount();
    u64 CyclesElapsed = Time->FrameEndClock - Time->FrameBeginClock;
    Time->FrameBeginClock = Time->FrameEndClock;
    Time->FrameMS = TimeInMS(CyclesElapsed);
    Time->TimeDeltaInSeconds = Time->FrameMS/1000.0f;
    Time->SecondsSinceStartup += Time->TimeDeltaInSeconds;
    Time->FramesPerSecond = 1 / (Time->TimeDeltaInSeconds);
}

static u32
GetCurrentWindowWidth() {
    return GLOBALPlatformState->Window.ClientWidth;
}

static u32
GetCurrentWindowHeight() {
    return GLOBALPlatformState->Window.ClientHeight;
}

static v2u
GetCurrentWindowSize() {
    return V2u(GLOBALPlatformState->Window.ClientWidth, GLOBALPlatformState->Window.ClientHeight);
}

static void
InitializePlatform(platform_init_settings Settings) {
    
    GLOBALPlatformState = (platform_state*)malloc(sizeof(platform_state));
    *GLOBALPlatformState = {};
    GLOBALPlatformState->Internal = (internal_platform_state*)malloc(sizeof(internal_platform_state));
    *GLOBALPlatformState->Internal = {};
    
    if(PlatformInit()) {
        GLOBALPlatformState->SystemInfo = PlatformGetSystemInfo();
        GLOBALPlatformState->Window = PlatformCreateWindow(Settings.Window);
    } else {
        Assert(!"Platform was unable to init");
    }
    
}

