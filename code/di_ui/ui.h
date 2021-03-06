
/*
TODO:
-Auto Layout
-Interaction unique persistent ID's
*/

/* USAGE GUIDE
* This IMGUI requires a few things from your end:
* First, before using you need to allocate a persistent ui state
* Secondly, at the end of the frame, each window in the state will have a render element array that you draw on your end
*/


#define UI_MAX_RENDER_ELEMENTS 2048
#define UI_MAX_WINDOWS 32
#define UI_MAX_WINDOW_SCROLLBARS 16 //THIS IS PER WINDOW
#define UI_MAX_ID_STACK 32
#define UI_STARTING_FONT_SIZE 24
#define UI_MAX_TIME 4000.0f //The max ammount of time before we reset to 0
#define MAX_SCROLL_CONTENT_SIZE 1000000.0f;

enum ui_coordinate_type {
    UI_PIXEL_SPACE,
    UI_SCREEN_SPACE,
    UI_CLIP_SPACE,
};

enum ui_axis_type {
    UI_XAXIS = 1 << 0,
    UI_YAXIS = 1 << 1,
    UI_ZAXIS = 1 << 2,
};

enum ui_render_typie {
    UI_RENDER_NONE,
    UI_RENDER_QUAD,
    UI_RENDER_TEXT,
    UI_RENDER_CLIP_RECT,
    UI_RENDER_COUNT,
};

enum ui_submit_type {
    UI_SUBMIT_NONE,
    UI_SUBMIT_ON_PRESS,
    UI_SUBMIT_ON_HOLD,
    UI_SUBMIT_ON_RELEASE,
    UI_SUBMIT_COUNT,
};

enum ui_interaction_state {
    UI_INTERACTION_STATE_NONE,
    UI_INTERACTION_STATE_HOT,
    UI_INTERACTION_STATE_NEXT_SUBMIT,
    UI_INTERACTION_STATE_SUBMIT,
    UI_INTERACTION_STATE_CULL,
    UI_INTERACTION_STATE_COUNT,
};

enum ui_element_bounds_type {
    UI_ELEMENT_BOUNDS_NONE,
    UI_ELEMENT_BOUNDS_RECT,
    UI_ELEMENT_BOUNDS_COUNT,
};

typedef u32 ui_id;

struct ui_quad {
    v2 P0;
    v2 P1;
    v2 P2;
    v2 P3;
};

struct ui_text {
    v2 At;
    u32 Size;
    char* Text;
};

struct ui_interaction {
    u32 SubmitType;
    u32 State;
    f32 TimeSinceStateChange;
    ui_id ID;
};

struct ui_element {
    //ui_interaction Interaction;
    u32 Type;
    union {
        rectangle2 Rect;
    };
};

struct ui_render_element {
    u32 Type;
    v4 Color;
    f32 ZValue;
    u32 PassbackID;
    u32 TextureID;
    union {
        ui_quad Quad;
        ui_text Text;
        rectangle2 ClipRect;
    };
};

enum ui_window_flags {
    UI_WINDOW_MOVE             = 1 << 0,
    UI_WINDOW_SIZE             = 1 << 1,
    UI_WINDOW_CLOSE            = 1 << 2,
    UI_WINDOW_ALWAYS_ON_TOP    = 1 << 4,
    UI_WINDOW_ALWAYS_ON_BOTTOM = 1 << 5,
    
    UI_WINDOW_DEFAULT = UI_WINDOW_MOVE | UI_WINDOW_SIZE | UI_WINDOW_CLOSE,
    UI_WINDOW_NO_SIZE = UI_WINDOW_MOVE | UI_WINDOW_CLOSE,
};

struct ui_scrollbar_info {
    v2 LastScrollOffset;
    u32 AxisFlags;
    rectangle2 WorkingRect;
    rectangle2 ContentRect;
    rectangle2 ScrollbarRectY;
    rectangle2 ScrollbarRectX;
    rectangle2 CornerRect;
    rectangle2 PreviousClipRect;
};

struct ui_window {
    rectangle2 BodyRect;
    rectangle2 WorkingRect;
    rectangle2 CurrentClipRect;
    u32 BodyWidth;
    u32 BodyHeight;
    u32 MinWidth;
    u32 MinHeight;
    
    ui_id ID;
    f32 ZValue;
    b32 WasUpdated;
    b32 KeepAlive;
    
    u32 CurrentTexture;
    u32 CurrentPassback; //Passes back an ID when we render;
    u32 CurrentTextAlignment;
    u32 CurrentFontSize;
    
    ui_render_element RenderElements[UI_MAX_RENDER_ELEMENTS];
    u32 RenderElementCount;
    
    struct ui_state* State;
    
    ui_element LastElement;
    ui_interaction LastInteraction;
    
    b32 IsInScrollbarMode;
    rectangle2 CurrentScrollbarClipRect;
    ui_scrollbar_info ScrollInfos[UI_MAX_WINDOW_SCROLLBARS];
    u32 ScrollbarIndex;
    
    ui_id IDStack[UI_MAX_ID_STACK];
    u32 IDCount;
    
    u32 WindowFlags;
};

struct ui_render_array {
    ui_render_element RenderElements[UI_MAX_RENDER_ELEMENTS];
    u32 RenderElementCount;
};

enum ui_state_flags {
    UI_STATE_FLAG_OUTPUT_BILATERAL = 1 << 0,
};

enum ui_input_flags {
    UI_INPUT_SHIFT = 1 << 0,
    UI_INPUT_CTRL  = 1 << 1,
    UI_INPUT_ALT   = 1 << 2,
};

struct ui_state {
    u32 Flags;
    
    u32 InputFlags;
    b32 WasPressed;
    b32 IsDown;
    b32 WentUp;
    
    f32 dt;
    v2 MouseP;
    v2 dMouseP;
    v3 MouseRayOrigin;
    v3 MouseRayDirection;
    char TextInput[32];
    u32 TextInputCount;
    
    rectangle2 DrawRegionInPixels;
    v2 DrawRegionDimInPixels;
    
    f32 CurrentTimer;
    
    ui_interaction CurrentHot;
    ui_interaction NextInteraction;
    ui_interaction CurrentInteraction;
    //NOTE(Jesse): Do we want a Previous interaction? Could be used for animating when something is finished interacting 
    // Can't think of much more use than that though
    b32 IsInteracting;
    
    s32 LastZValue;
    u32 Focus;
    
    u32 WindowCount;
    ui_window* CurrentWindow;
    ui_window Windows[UI_MAX_WINDOWS];
    
    ui_render_array FinalRenderList;
    
    u8* MemoryBase;
    u8* MemoryAt;
    u32 MemoryUsed;
    u32 MemoryMax;
};

rectangle2 SizeText(char* Text, f32 TextOutX, f32 TextOutY, f32 FontSize);