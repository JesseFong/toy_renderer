/* date = July 31st 2020 2:28 pm */

#ifndef WIN32_EVENTS_H
#define WIN32_EVENTS_H

/*
This method of using lParam scancodes instead of virtual keycodes comes from martins from HandMade Nework
https://handmade.network/wiki/2823-keyboard_inputs_-_scancodes,_raw_input,_text_input,_key_names
The main reason for doing this is that when using other keyboard layouts, the user will have to remap keys
ie: WASD to whatever movement keys suits their style of keyboard
*/

enum win32_scancode {
    
    WIN32_SCANCODE_ESCAPE = 0x01,
    WIN32_SCANCODE_1 = 0x02,
    WIN32_SCANCODE_2 = 0x03,
    WIN32_SCANCODE_3 = 0x04,
    WIN32_SCANCODE_4 = 0x05,
    WIN32_SCANCODE_5 = 0x06,
    WIN32_SCANCODE_6 = 0x07,
    WIN32_SCANCODE_7 = 0x08,
    WIN32_SCANCODE_8 = 0x09,
    WIN32_SCANCODE_9 = 0x0A,
    WIN32_SCANCODE_0 = 0x0B,
    WIN32_SCANCODE_MINUS = 0x0C,
    WIN32_SCANCODE_EQUALS = 0x0D,
    WIN32_SCANCODE_BACKSPACE = 0x0E,
    WIN32_SCANCODE_TAB = 0x0F,
    WIN32_SCANCODE_Q = 0x10,
    WIN32_SCANCODE_W = 0x11,
    WIN32_SCANCODE_E = 0x12,
    WIN32_SCANCODE_R = 0x13,
    WIN32_SCANCODE_T = 0x14,
    WIN32_SCANCODE_Y = 0x15,
    WIN32_SCANCODE_U = 0x16,
    WIN32_SCANCODE_I = 0x17,
    WIN32_SCANCODE_O = 0x18,
    WIN32_SCANCODE_P = 0x19,
    WIN32_SCANCODE_BRACKET_LEFT = 0x1A,
    WIN32_SCANCODE_BRACKET_RIGHT = 0x1B,
    WIN32_SCANCODE_ENTER = 0x1C,
    WIN32_SCANCODE_CONTROL_L = 0x1D,
    WIN32_SCANCODE_A = 0x1E,
    WIN32_SCANCODE_S =0x1F,
    WIN32_SCANCODE_D = 0x20,
    WIN32_SCANCODE_F = 0x21,
    WIN32_SCANCODE_G = 0x22,
    WIN32_SCANCODE_H = 0x23,
    WIN32_SCANCODE_J = 0x24,
    WIN32_SCANCODE_K = 0x25,
    WIN32_SCANCODE_L = 0x26,
    WIN32_SCANCODE_SEMICOLON = 0x27,
    WIN32_SCANCODE_APOSTROPHE = 0x28,
    WIN32_SCANCODE_GRAVE = 0x29,
    WIN32_SCANCODE_SHIFT_L = 0x2A,
    WIN32_SCANCODE_BACKSLASH = 0x2B,
    WIN32_SCANCODE_Z = 0x2C,
    WIN32_SCANCODE_X = 0x2D,
    WIN32_SCANCODE_C = 0x2E,
    WIN32_SCANCODE_V = 0x2F,
    WIN32_SCANCODE_B = 0x30,
    WIN32_SCANCODE_N = 0x31,
    WIN32_SCANCODE_M = 0x32,
    WIN32_SCANCODE_COMMA = 0x33,
    WIN32_SCANCODE_PERIOD = 0x34,
    WIN32_SCANCODE_SLASH = 0x35,
    WIN32_SCANCODE_SHIFT_R = 0x36,
    WIN32_SCANCODE_NUMPAD_MULTIPLY = 0x37,
    WIN32_SCANCODE_ALT = 0x38,
    WIN32_SCANCODE_SPACE = 0x39,
    WIN32_SCANCODE_CAPSLOCK = 0x3A,
    WIN32_SCANCODE_F1 = 0x3B,
    WIN32_SCANCODE_F2 = 0x3C,
    WIN32_SCANCODE_F3 = 0x3D,
    WIN32_SCANCODE_F4 = 0x3E,
    WIN32_SCANCODE_F5 = 0x3F,
    WIN32_SCANCODE_F6 = 0x40,
    WIN32_SCANCODE_F7 = 0x41,
    WIN32_SCANCODE_F8 = 0x42,
    WIN32_SCANCODE_F9 = 0x43,
    WIN32_SCANCODE_F10 = 0x44,
    WIN32_SCANCODE_NUMLOCK = 0x45,
    WIN32_SCANCODE_SCROLLLOCK = 0x46,
    WIN32_SCANCODE_NUMPAD_7 = 0x47,
    WIN32_SCANCODE_NUMPAD_8 = 0x48,
    WIN32_SCANCODE_NUMPAD_9 = 0x49,
    WIN32_SCANCODE_NUMPAD_MINUS = 0x4A,
    WIN32_SCANCODE_NUMPAD_4 = 0x4B,
    WIN32_SCANCODE_NUMPAD_5 = 0x4C,
    WIN32_SCANCODE_NUMPAD_6 = 0x4D,
    WIN32_SCANCODE_NUMPAD_PLUS = 0x4E,
    WIN32_SCANCODE_NUMPAD_1 = 0x4F,
    WIN32_SCANCODE_NUMPAD_2 = 0x50,
    WIN32_SCANCODE_NUMPAD_3 = 0x51,
    WIN32_SCANCODE_NUMPAD_0 = 0x52,
    WIN32_SCANCODE_NUMPAD_PERIOD = 0x53,
    WIN32_SCANCODE_ALT_PRINTSCREEN = 0x54,
    /* ALT + PRINT SCREEN. MAPVIRTUALKEYEx( VK_SNAPSHOT, MAPVK_VK_TO_VWIN32_SCANCODE_Ex, 0 ) RETURNS WIN32_SCANCODE 0x54. */
    WIN32_SCANCODE_BRACKETANGLE = 0x56, /* KEY BETWEEN THE LEFT SHIFT AND Z. */
    WIN32_SCANCODE_F11 = 0x57,
    WIN32_SCANCODE_F12 = 0x58,
    WIN32_SCANCODE_OEM_1 = 0x5A, /* VK_OEM_WSCTRL */
    WIN32_SCANCODE_OEM_2 = 0x5B, /* VK_OEM_FINISH */
    WIN32_SCANCODE_OEM_3 = 0x5C, /* VK_OEM_JUMP */
    WIN32_SCANCODE_ERASEEOF = 0x5D,
    WIN32_SCANCODE_OEM_4 = 0x5E, /* VK_OEM_BACKTAB */
    WIN32_SCANCODE_OEM_5 = 0x5F, /* VK_OEM_AUTO */
    WIN32_SCANCODE_ZOOM = 0x62,
    WIN32_SCANCODE_HELP = 0x63,
    WIN32_SCANCODE_F13 = 0x64,
    WIN32_SCANCODE_F14 = 0x65,
    WIN32_SCANCODE_F15 = 0x66,
    WIN32_SCANCODE_F16 = 0x67,
    WIN32_SCANCODE_F17 = 0x68,
    WIN32_SCANCODE_F18 = 0x69,
    WIN32_SCANCODE_F19 = 0x6A,
    WIN32_SCANCODE_F20 = 0x6B,
    WIN32_SCANCODE_F21 = 0x6C,
    WIN32_SCANCODE_F22 = 0x6D,
    WIN32_SCANCODE_F23 = 0x6E,
    WIN32_SCANCODE_OEM_6 = 0x6F, /* VK_OEM_PA3 */
    WIN32_SCANCODE_KATAKANA = 0x70,
    WIN32_SCANCODE_OEM_7 = 0x71, /* VK_OEM_RESET */
    WIN32_SCANCODE_F24 = 0x76,
    WIN32_SCANCODE_SBCSCHAR = 0x77,
    WIN32_SCANCODE_CONVERT = 0x79,
    WIN32_SCANCODE_NONCONVERT = 0x7B, /* VK_OEM_PA1 */
    
    WIN32_SCANCODE_MEDIA_PREVIOUS = 0xE010,
    WIN32_SCANCODE_MEDIA_NEXT = 0xE019,
    WIN32_SCANCODE_NUMPAD_ENTER = 0xE01C,
    WIN32_SCANCODE_CONTROL_R = 0xE01D,
    WIN32_SCANCODE_VOLUME_MUTE = 0xE020,
    WIN32_SCANCODE_LAUNCH_APP2 = 0xE021,
    WIN32_SCANCODE_MEDIA_PLAY = 0xE022,
    WIN32_SCANCODE_MEDIA_STOP = 0xE024,
    WIN32_SCANCODE_VOLUME_DOWN = 0xE02E,
    WIN32_SCANCODE_VOLUME_UP = 0xE030,
    WIN32_SCANCODE_BROWSER_HOME = 0xE032,
    WIN32_SCANCODE_NUMPAD_DIVIDE = 0xE035,
    WIN32_SCANCODE_PRINTSCREEN = 0xE037,
    /*
    WIN32_SCANCODE_PRINTSCREEN:
    - MAKE: 0xE02A 0xE037
    - BREAK: 0xE0B7 0xE0AA
    - MAPVIRTUALKEYEx( VK_SNAPSHOT, MAPVK_VK_TO_VWIN32_SCANCODE_Ex, 0 ) RETURNS WIN32_SCANCODE 0x54;
    - THERE IS NO VK_KEYDOWN WITH VK_SNAPSHOT.
    */
    WIN32_SCANCODE_ALTRIGHT = 0xE038,
    WIN32_SCANCODE_CANCEL = 0xE046, /* CTRL + PAUSE */
    WIN32_SCANCODE_HOME = 0xE047,
    WIN32_SCANCODE_ARROWUP = 0xE048,
    WIN32_SCANCODE_PAGEUP = 0xE049,
    WIN32_SCANCODE_ARROWLEFT = 0xE04B,
    WIN32_SCANCODE_ARROWRIGHT = 0xE04D,
    WIN32_SCANCODE_END = 0xE04F,
    WIN32_SCANCODE_ARROWDOWN = 0xE050,
    WIN32_SCANCODE_PAGEDOWN = 0xE051,
    WIN32_SCANCODE_INSERT = 0xE052,
    WIN32_SCANCODE_DELETE = 0xE053,
    WIN32_SCANCODE_METALEFT = 0xE05B,
    WIN32_SCANCODE_METARIGHT = 0xE05C,
    WIN32_SCANCODE_APPLICATION = 0xE05D,
    WIN32_SCANCODE_POWER = 0xE05E,
    WIN32_SCANCODE_SLEEP = 0xE05F,
    WIN32_SCANCODE_WAKE = 0xE063,
    WIN32_SCANCODE_BROWSER_SEARCH = 0xE065,
    WIN32_SCANCODE_BROWSER_FAVORITES = 0xE066,
    WIN32_SCANCODE_BROWSER_REFRESH = 0xE067,
    WIN32_SCANCODE_BROWSER_STOP = 0xE068,
    WIN32_SCANCODE_BROWSER_FORWARD = 0xE069,
    WIN32_SCANCODE_BROWSER_BACK = 0xE06A,
    WIN32_SCANCODE_LAUNCH_APP1 = 0xE06B,
    WIN32_SCANCODE_LAUNCH_EMAIL = 0xE06C,
    WIN32_SCANCODE_LAUNCH_MEDIA = 0xE06D,
    
    WIN32_SCANCODE_PAUSE = 0xE11D45,
    /*
    WIN32_SCANCODE_PAUSE:
    - MAKE: 0xE11D 45 0xE19D C5
    - MAKE IN RAW INPUT: 0xE11D 0x45
    - BREAK: NONE
    - NO REPEAT WHEN YOU HOLD THE KEY DOWN
    - THERE ARE NO BREAK SO I DON'T KNOW HOW THE KEY DOWN/UP IS ExPECTED TO WORK. RAW INPUT SENDS "KEYDOWN" AND "KEYUP" MESSAGES, AND IT APPEARS THAT THE KEYUP MESSAGE IS SENT DIRECTLY AFTER THE KEYDOWN MESSAGE (YOU CAN'T HOLD THE KEY DOWN) SO DEPENDING ON WHEN GETMESSAGE OR PEEKMESSAGE WILL RETURN MESSAGES, YOU MAY GET BOTH A KEYDOWN AND KEYUP MESSAGE "AT THE SAME TIME". IF YOU USE VK MESSAGES MOST OF THE TIME YOU ONLY GET KEYDOWN MESSAGES, BUT SOME TIMES YOU GET KEYUP MESSAGES TOO.
     - WHEN PRESSED AT THE SAME TIME AS ONE OR BOTH CONTROL KEYS, GENERATES A 0xE046 (WIN32_SCANCODE_CANCEL) AND THE STRING FOR THAT WIN32_SCANCODE IS "BREAK".
    */
    WIN32_SCANCODE_SIZE,
};

unsigned int Win32ScancodeList[ ] = {
    
    WIN32_SCANCODE_ESCAPE,
    WIN32_SCANCODE_1,
    WIN32_SCANCODE_2,
    WIN32_SCANCODE_3,
    WIN32_SCANCODE_4,
    WIN32_SCANCODE_5,
    WIN32_SCANCODE_6,
    WIN32_SCANCODE_7,
    WIN32_SCANCODE_8,
    WIN32_SCANCODE_9,
    WIN32_SCANCODE_0,
    WIN32_SCANCODE_MINUS,
    WIN32_SCANCODE_EQUALS,
    WIN32_SCANCODE_BACKSPACE,
    WIN32_SCANCODE_TAB,
    WIN32_SCANCODE_Q,
    WIN32_SCANCODE_W,
    WIN32_SCANCODE_E,
    WIN32_SCANCODE_R,
    WIN32_SCANCODE_T,
    WIN32_SCANCODE_Y,
    WIN32_SCANCODE_U,
    WIN32_SCANCODE_I,
    WIN32_SCANCODE_O,
    WIN32_SCANCODE_P,
    WIN32_SCANCODE_BRACKET_LEFT,
    WIN32_SCANCODE_BRACKET_RIGHT,
    WIN32_SCANCODE_ENTER,
    WIN32_SCANCODE_CONTROL_L,
    WIN32_SCANCODE_A,
    WIN32_SCANCODE_S,
    WIN32_SCANCODE_D,
    WIN32_SCANCODE_F,
    WIN32_SCANCODE_G,
    WIN32_SCANCODE_H,
    WIN32_SCANCODE_J,
    WIN32_SCANCODE_K,
    WIN32_SCANCODE_L,
    WIN32_SCANCODE_SEMICOLON,
    WIN32_SCANCODE_APOSTROPHE,
    WIN32_SCANCODE_GRAVE,
    WIN32_SCANCODE_SHIFT_L,
    WIN32_SCANCODE_BACKSLASH,
    WIN32_SCANCODE_Z,
    WIN32_SCANCODE_X,
    WIN32_SCANCODE_C,
    WIN32_SCANCODE_V,
    WIN32_SCANCODE_B,
    WIN32_SCANCODE_N,
    WIN32_SCANCODE_M,
    WIN32_SCANCODE_COMMA,
    WIN32_SCANCODE_PERIOD,
    WIN32_SCANCODE_SLASH,
    WIN32_SCANCODE_SHIFT_R,
    WIN32_SCANCODE_NUMPAD_MULTIPLY,
    WIN32_SCANCODE_ALT,
    WIN32_SCANCODE_SPACE,
    WIN32_SCANCODE_CAPSLOCK,
    WIN32_SCANCODE_F1,
    WIN32_SCANCODE_F2,
    WIN32_SCANCODE_F3,
    WIN32_SCANCODE_F4,
    WIN32_SCANCODE_F5,
    WIN32_SCANCODE_F6,
    WIN32_SCANCODE_F7,
    WIN32_SCANCODE_F8,
    WIN32_SCANCODE_F9,
    WIN32_SCANCODE_F10,
    WIN32_SCANCODE_NUMLOCK,
    WIN32_SCANCODE_SCROLLLOCK,
    WIN32_SCANCODE_NUMPAD_7,
    WIN32_SCANCODE_NUMPAD_8,
    WIN32_SCANCODE_NUMPAD_9,
    WIN32_SCANCODE_NUMPAD_MINUS,
    WIN32_SCANCODE_NUMPAD_4,
    WIN32_SCANCODE_NUMPAD_5,
    WIN32_SCANCODE_NUMPAD_6,
    WIN32_SCANCODE_NUMPAD_PLUS,
    WIN32_SCANCODE_NUMPAD_1,
    WIN32_SCANCODE_NUMPAD_2,
    WIN32_SCANCODE_NUMPAD_3,
    WIN32_SCANCODE_NUMPAD_0,
    WIN32_SCANCODE_NUMPAD_PERIOD,
    WIN32_SCANCODE_ALT_PRINTSCREEN,
    WIN32_SCANCODE_BRACKETANGLE,
    WIN32_SCANCODE_F11,
    WIN32_SCANCODE_F12,
    WIN32_SCANCODE_OEM_1,
    WIN32_SCANCODE_OEM_2,
    WIN32_SCANCODE_OEM_3,
    WIN32_SCANCODE_ERASEEOF,
    WIN32_SCANCODE_OEM_4,
    WIN32_SCANCODE_OEM_5,
    WIN32_SCANCODE_ZOOM,
    WIN32_SCANCODE_HELP,
    WIN32_SCANCODE_F13,
    WIN32_SCANCODE_F14,
    WIN32_SCANCODE_F15,
    WIN32_SCANCODE_F16,
    WIN32_SCANCODE_F17,
    WIN32_SCANCODE_F18,
    WIN32_SCANCODE_F19,
    WIN32_SCANCODE_F20,
    WIN32_SCANCODE_F21,
    WIN32_SCANCODE_F22,
    WIN32_SCANCODE_F23,
    WIN32_SCANCODE_OEM_6,
    WIN32_SCANCODE_KATAKANA,
    WIN32_SCANCODE_OEM_7,
    WIN32_SCANCODE_F24,
    WIN32_SCANCODE_SBCSCHAR,
    WIN32_SCANCODE_CONVERT,
    WIN32_SCANCODE_NONCONVERT,
    
    WIN32_SCANCODE_MEDIA_PREVIOUS,
    WIN32_SCANCODE_MEDIA_NEXT,
    WIN32_SCANCODE_NUMPAD_ENTER,
    WIN32_SCANCODE_CONTROL_R,
    WIN32_SCANCODE_VOLUME_MUTE,
    WIN32_SCANCODE_LAUNCH_APP2,
    WIN32_SCANCODE_MEDIA_PLAY,
    WIN32_SCANCODE_MEDIA_STOP,
    WIN32_SCANCODE_VOLUME_DOWN,
    WIN32_SCANCODE_VOLUME_UP,
    WIN32_SCANCODE_BROWSER_HOME,
    WIN32_SCANCODE_NUMPAD_DIVIDE,
    WIN32_SCANCODE_PRINTSCREEN,
    WIN32_SCANCODE_ALTRIGHT,
    WIN32_SCANCODE_CANCEL,
    WIN32_SCANCODE_HOME,
    WIN32_SCANCODE_ARROWUP,
    WIN32_SCANCODE_PAGEUP,
    WIN32_SCANCODE_ARROWLEFT,
    WIN32_SCANCODE_ARROWRIGHT,
    WIN32_SCANCODE_END,
    WIN32_SCANCODE_ARROWDOWN,
    WIN32_SCANCODE_PAGEDOWN,
    WIN32_SCANCODE_INSERT,
    WIN32_SCANCODE_DELETE,
    WIN32_SCANCODE_METALEFT,
    WIN32_SCANCODE_METARIGHT,
    WIN32_SCANCODE_APPLICATION,
    WIN32_SCANCODE_POWER,
    WIN32_SCANCODE_SLEEP,
    WIN32_SCANCODE_WAKE,
    WIN32_SCANCODE_BROWSER_SEARCH,
    WIN32_SCANCODE_BROWSER_FAVORITES,
    WIN32_SCANCODE_BROWSER_REFRESH,
    WIN32_SCANCODE_BROWSER_STOP,
    WIN32_SCANCODE_BROWSER_FORWARD,
    WIN32_SCANCODE_BROWSER_BACK,
    WIN32_SCANCODE_LAUNCH_APP1,
    WIN32_SCANCODE_LAUNCH_EMAIL,
    WIN32_SCANCODE_LAUNCH_MEDIA,
    
    WIN32_SCANCODE_PAUSE
};

struct keymap_result {
    int AppKeyCode;
    char* Name;
};
#define KEYMAP(ScanCode, AppCode) \
case((ScanCode)): {\
Result.AppKeyCode = AppCode;\
Result.Name = #AppCode;\
} break\

static keymap_result 
Win32ScancodeToAppKeycode(u32 Scancode) {
    keymap_result Result = {};
    switch(Scancode) {
        KEYMAP(0, KEY_NONE);
        KEYMAP(WIN32_SCANCODE_SHIFT_L, KEY_SHIFT);
        KEYMAP(WIN32_SCANCODE_SHIFT_R, KEY_SHIFT);
        KEYMAP(WIN32_SCANCODE_CONTROL_L, KEY_CONTROL);
        KEYMAP(WIN32_SCANCODE_CONTROL_R, KEY_CONTROL);
        KEYMAP(WIN32_SCANCODE_ALT, KEY_ALT);
        KEYMAP(WIN32_SCANCODE_CAPSLOCK, KEY_CAPS);
        KEYMAP(WIN32_SCANCODE_TAB, KEY_TAB);
        KEYMAP(WIN32_SCANCODE_ENTER, KEY_ENTER);
        KEYMAP(WIN32_SCANCODE_NUMPAD_ENTER, KEY_ENTER);
        KEYMAP(WIN32_SCANCODE_ESCAPE, KEY_ESC);
        KEYMAP(WIN32_SCANCODE_SPACE, KEY_SPACE);
        KEYMAP(WIN32_SCANCODE_HOME, KEY_HOME);
        KEYMAP(WIN32_SCANCODE_END, KEY_END);
        KEYMAP(WIN32_SCANCODE_NUMPAD_PLUS, KEY_PLUS);
        KEYMAP(WIN32_SCANCODE_NUMPAD_MINUS, KEY_MINUS);
        KEYMAP(WIN32_SCANCODE_EQUALS, KEY_EQUALS);
        KEYMAP(WIN32_SCANCODE_BACKSPACE, KEY_BACKSPACE);
        
        KEYMAP(WIN32_SCANCODE_ARROWLEFT, KEY_LEFT);
        KEYMAP(WIN32_SCANCODE_ARROWRIGHT, KEY_RIGHT);
        KEYMAP(WIN32_SCANCODE_ARROWUP, KEY_UP);
        KEYMAP(WIN32_SCANCODE_ARROWDOWN, KEY_DOWN);
        
        KEYMAP(WIN32_SCANCODE_1, KEY_1);
        KEYMAP(WIN32_SCANCODE_2, KEY_2);
        KEYMAP(WIN32_SCANCODE_3, KEY_3);
        KEYMAP(WIN32_SCANCODE_4, KEY_4);
        KEYMAP(WIN32_SCANCODE_5, KEY_5);
        KEYMAP(WIN32_SCANCODE_6, KEY_6);
        KEYMAP(WIN32_SCANCODE_7, KEY_7);
        KEYMAP(WIN32_SCANCODE_8, KEY_8);
        KEYMAP(WIN32_SCANCODE_9, KEY_9);
        KEYMAP(WIN32_SCANCODE_0, KEY_0);
        KEYMAP(WIN32_SCANCODE_NUMPAD_1, KEY_1);
        KEYMAP(WIN32_SCANCODE_NUMPAD_2, KEY_2);
        KEYMAP(WIN32_SCANCODE_NUMPAD_3, KEY_3);
        KEYMAP(WIN32_SCANCODE_NUMPAD_4, KEY_4);
        KEYMAP(WIN32_SCANCODE_NUMPAD_5, KEY_5);
        KEYMAP(WIN32_SCANCODE_NUMPAD_6, KEY_6);
        KEYMAP(WIN32_SCANCODE_NUMPAD_7, KEY_7);
        KEYMAP(WIN32_SCANCODE_NUMPAD_8, KEY_8);
        KEYMAP(WIN32_SCANCODE_NUMPAD_9, KEY_9);
        KEYMAP(WIN32_SCANCODE_NUMPAD_0, KEY_0);
        
        KEYMAP(WIN32_SCANCODE_A, KEY_A);
        KEYMAP(WIN32_SCANCODE_B, KEY_B);
        KEYMAP(WIN32_SCANCODE_C, KEY_C);
        KEYMAP(WIN32_SCANCODE_D, KEY_D);
        KEYMAP(WIN32_SCANCODE_E, KEY_E);
        KEYMAP(WIN32_SCANCODE_F, KEY_F);
        KEYMAP(WIN32_SCANCODE_G, KEY_G);
        KEYMAP(WIN32_SCANCODE_H, KEY_H);
        KEYMAP(WIN32_SCANCODE_I, KEY_I);
        KEYMAP(WIN32_SCANCODE_J, KEY_J);
        KEYMAP(WIN32_SCANCODE_K, KEY_K);
        KEYMAP(WIN32_SCANCODE_L, KEY_L);
        KEYMAP(WIN32_SCANCODE_M, KEY_M);
        KEYMAP(WIN32_SCANCODE_N, KEY_N);
        KEYMAP(WIN32_SCANCODE_O, KEY_O);
        KEYMAP(WIN32_SCANCODE_P, KEY_P);
        KEYMAP(WIN32_SCANCODE_Q, KEY_Q);
        KEYMAP(WIN32_SCANCODE_R, KEY_R);
        KEYMAP(WIN32_SCANCODE_S, KEY_S);
        KEYMAP(WIN32_SCANCODE_T, KEY_T); 
        KEYMAP(WIN32_SCANCODE_U, KEY_U);
        KEYMAP(WIN32_SCANCODE_V, KEY_V);
        KEYMAP(WIN32_SCANCODE_W, KEY_W);
        KEYMAP(WIN32_SCANCODE_X, KEY_X);
        KEYMAP(WIN32_SCANCODE_Y, KEY_Y);
        KEYMAP(WIN32_SCANCODE_Z, KEY_Z); 
        
        KEYMAP(WIN32_SCANCODE_F1, KEY_F1);
        KEYMAP(WIN32_SCANCODE_F2, KEY_F2);
        KEYMAP(WIN32_SCANCODE_F3, KEY_F3);
        KEYMAP(WIN32_SCANCODE_F4, KEY_F4);
        KEYMAP(WIN32_SCANCODE_F5, KEY_F5);
        KEYMAP(WIN32_SCANCODE_F6, KEY_F6);
        KEYMAP(WIN32_SCANCODE_F7, KEY_F7);
        KEYMAP(WIN32_SCANCODE_F8, KEY_F8);
        KEYMAP(WIN32_SCANCODE_F9, KEY_F9);
        KEYMAP(WIN32_SCANCODE_F10, KEY_F10);
        KEYMAP(WIN32_SCANCODE_F11, KEY_F11);
        KEYMAP(WIN32_SCANCODE_F12, KEY_F12);
        
        KEYMAP(WIN32_SCANCODE_BRACKET_LEFT, KEY_BRACKET_LEFT);
        KEYMAP(WIN32_SCANCODE_BRACKET_RIGHT, KEY_BRACKET_RIGHT);
        KEYMAP(WIN32_SCANCODE_SEMICOLON, KEY_SEMICOLON);
        KEYMAP(WIN32_SCANCODE_APOSTROPHE, KEY_APOSTROPHE);
        KEYMAP(WIN32_SCANCODE_BACKSLASH, KEY_BACKSLASH);
        
        KEYMAP(WIN32_SCANCODE_GRAVE, KEY_GRAVE);
        KEYMAP(WIN32_SCANCODE_COMMA, KEY_COMMA);
        KEYMAP(WIN32_SCANCODE_PERIOD, KEY_PERIOD);
        KEYMAP(WIN32_SCANCODE_NUMPAD_PERIOD, KEY_PERIOD);
        KEYMAP(WIN32_SCANCODE_SLASH, KEY_SLASH); 
        default: {
            Result = {};
        } break;
    }
    return Result;
}

#define KEY_PRESSED 0x1
#define KEY_TRANSITION_COUNT_MASK 0xfe

static usize
Win32GetScancodeOffset(u32 Scancode) {
    usize Result = Scancode;
    usize Group0End = WIN32_SCANCODE_NONCONVERT;
    usize Group1Start = WIN32_SCANCODE_MEDIA_PREVIOUS;
    usize Group1End = WIN32_SCANCODE_LAUNCH_MEDIA;
    usize Group2Start = WIN32_SCANCODE_PAUSE;
#if 0
    if(Scancode >= Group2Start) {
        Result = 0;
        // TODO(Jesse): Just for key_pause???
        Assert(!"Now is the time to fix this key");
    } else if (Scancode >= Group1Start) {
        Result = Group0End + (Scancode - Group1Start);
    }
    
    
    Assert(Result <= 0xFF);
#endif
    
    return(Result);
}

static void ClearKeys();

static void
Win32SyncKeys() {
    usize Index = 0;
    while(Index < sizeof(Win32ScancodeList) / sizeof(Win32ScancodeList[0])) {
        
        u32 Scancode = Win32ScancodeList[Index];
        if(Scancode == 0x45) {
            Scancode = 0xE045;
        } else if(Scancode == 0xE11D45) {
            Scancode = 0x45;
        }
        
        usize Win32Offset = Win32GetScancodeOffset(Scancode);
        u32 Vk = MapVirtualKeyEx(Scancode, MAPVK_VSC_TO_VK_EX, 0);
        u32 KeyState = GetAsyncKeyState(Vk);
        
        keymap_result Result = Win32ScancodeToAppKeycode((u32)Win32Offset);
        GLOBALPlatformState->Input.Buttons[Result.AppKeyCode].State = ((KeyState & (0x1 << 15)) > 0) ? KEY_PRESSED : 0;
        
        Index++;
    }
}

global_variable b32 GLOBALWindowIsActive = true;
LRESULT CALLBACK
Win32WindowCallback(HWND Window, UINT Message, WPARAM WParam, LPARAM LParam) {
    
    platform_state* PlatformState = GLOBALPlatformState;
    internal_platform_state* InternalState = PlatformState->Internal;
    
    LRESULT Result = 0;
    switch (Message) {
        case WM_QUIT:
        case WM_DESTROY: {
            PlatformState->Window.IsRunning = false;
        } break;
        
        case WM_WINDOWPOSCHANGING:
        {
        } break;
        
        case WM_SYSCOMMAND: {
            /* Remove beeping sound when ALT + some key is pressed. */
            if(WParam == SC_KEYMENU) {
                Result = 0;
            } else {
                Result = DefWindowProcW(Window, Message, WParam, LParam );
            }
        } break;
        
        case WM_SETFOCUS:{
            Win32SyncKeys(); // TODO(NAME):
        } break;
        
        case WM_KILLFOCUS: {
            ClearKeys(); // TODO(NAME): 
        } break;
        
        case WM_ACTIVATE: {
            
            if(WParam == WA_ACTIVE || WParam == WA_CLICKACTIVE) {
                PushPlatformWindowEvent(PLATFORM_EVENT_WINDOW_TYPE_ACTIVE);
                InternalState->Window.IsActive = true;
            } else if(WParam == WA_INACTIVE) {
                PushPlatformWindowEvent(PLATFORM_EVENT_WINDOW_TYPE_INACTIVE);
                InternalState->Window.IsActive = false;
            }
        } break;
        
        case WM_SIZE: {
            
			DWORD NewWidth = LOWORD(LParam);
			DWORD NewHeight = HIWORD(LParam);
            
            if(WParam == SIZE_MAXIMIZED) {
                PushPlatformWindowEvent(PLATFORM_EVENT_WINDOW_TYPE_MAXIMIZED, V2u((u32)NewWidth, (u32)NewHeight));
            } else if(WParam == SIZE_MINIMIZED) {
                PushPlatformWindowEvent(PLATFORM_EVENT_WINDOW_TYPE_MINIMIZED, V2u((u32)NewWidth, (u32)NewHeight));
            } else if(WParam == SIZE_RESTORED) {
                PushPlatformWindowEvent(PLATFORM_EVENT_WINDOW_TYPE_RESTORED);
            }
            
        } break;
        
        case WM_PAINT: {
            PAINTSTRUCT Paint;
            HDC DeviceContext = BeginPaint(Window, &Paint);
        } break;
        
        default: {
            Result = DefWindowProcW(Window, Message, WParam, LParam);
        } break;
    }
    return Result;
}

static void
Win32ProcessMouseMessages() {
    
    internal_platform_state* InternalState = GLOBALPlatformState->Internal;
    if(!InternalState->Window.IsActive) {return;}
    
    platform_state* PlatformState = GLOBALPlatformState;
    PlatformState->Input.Mouse.dZ = 0;
    //NOTE(jesse): MouseWheel handled in message queue so we reset dZ
    
    POINT MouseP;
    GetCursorPos(&MouseP);
    ScreenToClient(PlatformState->Internal->Window.Handle, &MouseP);
    
    f32 MouseX = (f32)MouseP.x;
    s32 TestForUnderflow = (PlatformState->Window.ClientHeight - 1);
    f32 MouseY = (f32)(TestForUnderflow - MouseP.y);
    
    v2 LastP = PlatformState->Input.Mouse.P;
    v2 P = V2(MouseX, MouseY);
    v2 dP = P - LastP;
    
    PlatformState->Input.Mouse.LastP = LastP;
    PlatformState->Input.Mouse.P = P;
    PlatformState->Input.Mouse.dP = dP;
    
    f32 WindowWidth = (f32)PlatformState->Window.ClientWidth;
    f32 WindowHeight = (f32)PlatformState->Window.ClientHeight;
    
    f32 MouseU = Clamp01MapToRange(0, P.x, WindowWidth);
    f32 MouseV = Clamp01MapToRange(0, P.y, WindowHeight);
    PlatformState->Input.Mouse.ScreenP = V2(MouseU, MouseV);
    PlatformState->Input.Mouse.dScreenP.x = dP.x / WindowWidth;
    PlatformState->Input.Mouse.dScreenP.y = dP.y / WindowHeight;
    
    PlatformState->Input.Mouse.ClipP.x = (MouseU * 2.0f) - 1.0f;
    PlatformState->Input.Mouse.ClipP.y = (MouseV * 2.0f) - 1.0f;
    PlatformState->Input.Mouse.dClipP.x = dP.x / WindowWidth * 2.0f;
    PlatformState->Input.Mouse.dClipP.y = dP.y / WindowHeight * 2.0f;
    
    DWORD MouseButtonID[5] =
    {
        VK_LBUTTON,
        VK_RBUTTON,
        VK_MBUTTON,
        VK_XBUTTON1,
        VK_XBUTTON2,
    };
    for(u32 ButtonIndex = 0;
        ButtonIndex < ArrayCount(MouseButtonID);
        ++ButtonIndex) {
        
        short Win32KeyState = GetKeyState(MouseButtonID[ButtonIndex]);
        u8 Pressed = ((Win32KeyState & (1 << 15)) > 0) ? KEY_PRESSED : 0;
        
        u32 KeyCode = (ButtonIndex + KEY_MOUSE1);
        ProcessInputMessage(KeyCode, Pressed);
    }
}

static void
Win32ProcessPendingMessages() {
    
    platform_state* PlatformState = GLOBALPlatformState;
    MSG Message = {};
    for(;;) {
        
        BOOL GotMessage = FALSE;
        {
            // NOTE(Jesse): We avoid asking for WM_PAINT and WM_MOUSEMOVE messages
            // here so that Windows will not generate them (they are generated on-demand)
            // since we don't actually care about either.
            GotMessage = PeekMessage(&Message, 0, 0, WM_PAINT - 1, PM_REMOVE);
            if(!GotMessage) {
                GotMessage = PeekMessage(&Message, 0, WM_PAINT + 1, WM_MOUSEMOVE - 1, PM_REMOVE);
                if(!GotMessage) {
                    GotMessage = PeekMessage(&Message, 0, WM_MOUSEMOVE + 1, 0xFFFFFFFF, PM_REMOVE);
                }
            }
        }
        if(!GotMessage) {
            break;
        }
        switch(Message.message) {
            
            case WM_CLOSE: {
            } break;
            
            case WM_MOUSEWHEEL: {
                s8 dMouseZ = (s8)GET_WHEEL_DELTA_WPARAM(Message.wParam);
                dMouseZ = dMouseZ/120; //WHEEL DELTA STEPS IN 120 I HAVE NO IDEA WHY WHO CARES MICROSOFT NEEDS IT TO BE THAT, BLESSINGS BE TO RAYMOND CHEN
                PlatformState->Input.Mouse.LastZ = PlatformState->Input.Mouse.Z;
                PlatformState->Input.Mouse.Z = PlatformState->Input.Mouse.LastZ + (f32)dMouseZ;
                PlatformState->Input.Mouse.dZ = PlatformState->Input.Mouse.Z - PlatformState->Input.Mouse.LastZ;
            } break;
            
            case WM_SYSKEYDOWN:
            case WM_SYSKEYUP:
            case WM_KEYDOWN:
            case WM_KEYUP: {
                
                u32 Scancode = (Message.lParam >> 16) & 0xFF;
                u32 IsExtended = (Message.lParam >> 24) & 0x1;
                
                u8 Pressed = ((~Message.lParam) >> 31) & KEY_PRESSED;
                
                if(IsExtended) {
                    if(Scancode != 0x45) { Scancode |= 0xE000; }
                } else {
                    
                    if(Scancode == 0x45) {
                        Scancode = 0xE11D45;
                    } else if(Scancode == 0x54){
                        Scancode = 0xE037;
                    }
                }
                
                u32 Offset = (u32)Win32GetScancodeOffset(Scancode);
                
                keymap_result Result = Win32ScancodeToAppKeycode(Offset);
                ProcessInputMessage(Result.AppKeyCode, Pressed);
                
                TranslateMessage(&Message);
                
            } break;
            
            case WM_CHAR: {
                /*
WM_CHAR is generated when WM_KEYDOWN message are passed to TranslateMessage;
wParam is UTF-16.
If the codepoint is 4 bytes, there are 2 WM_CHAR message, one with the high surrogate and one with the low surrogate.
https://msdn.microsoft.com/en-us/library/windows/desktop/ms646276(v=vs.85).aspx
*/
                u32 C = (u32)Message.wParam;
                u16 UTF16HiSurrogateStart = 0xD800;
                u16 UTF16LoSurrogateStart = 0xDC00;
                u16 UTF16SurrogateEnd = 0xDFFF;
                u16 HighSurrogate = 0;
                if(C >= UTF16HiSurrogateStart && C < UTF16LoSurrogateStart){
                    HighSurrogate = (u16)C;
                } else {
                    if(C >= UTF16LoSurrogateStart && C <= UTF16SurrogateEnd) {
                        u16 LowSurrogate = (u16)C;
                        C = (HighSurrogate - UTF16HiSurrogateStart) << 10;
                        C |= (LowSurrogate - UTF16LoSurrogateStart);
                        C += 0x10000;
                        
                        HighSurrogate = 0;
                    }
                    
                    char UTF8Char = WideCharToUTF8Char((wchar_t)C);
                    ProcessCharMessage(UTF8Char);
                }
                
            } break;
            
            default: {
                TranslateMessage(&Message);
                DispatchMessageA(&Message);
            } break;
        }
    }
}

#endif //WIN32_EVENTS_H
