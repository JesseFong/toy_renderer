


#define HASH_INITIAL 2166136261
static u32
UIGenerateID(u32* ID, void* Data, u32 DataCount) {
    
    if(*ID == 0) {
        *ID = HASH_INITIAL;
    }
    
    u8* P = (u8*)Data;
    while(DataCount--) {
        *ID = (*ID ^ *P++) * 1677619;
    }
    return(*ID);
}

static u32
UIUniqueID(ui_state* State) {
    u32 Result = State->UniqueIDSeed;
    
    u8 Data[8] = {4, 43, 121, 2, 4, 3, 99, 63};
    Result = UIGenerateID(&Result, &Data[0], sizeof(Data));
    
    State->UniqueIDSeed = Result;
    return Result;
}

static u32
UIUniqueID(ui_window* Window) {
    return UIUniqueID(Window->State);
}

static u32
UIStringLength(char* Text) {
    u32 Result = 0;
    if(Text) {
        Result = ((u32)strlen(Text));
    }
    return Result;
}

static u32
UIRandMod(u32 Mod) {
    return(rand() % Mod);
}


static void
UIInitializeMemory(ui_state* State) {
    
    State->MemoryMax = Megabytes(64);
    State->MemoryBase = (u8*)PlatformAllocateMemory(State->MemoryMax);
    State->MemoryAt = State->MemoryBase;
    State->MemoryUsed = 0;
    State->UniqueIDSeed = 6553693;
}

static char*
UIPushString(ui_window* Window, char* String) {
    ui_state* State = Window->State;
    
    u32 Length = UIStringLength(String);
    if((State->MemoryUsed + Length) > State->MemoryMax) {
        Assert("Allocate MoreMemoryHere");
    }
    
    char* Result = (char*)State->MemoryAt;
    
    for(u32 I = 0;
        I < Length;
        I++) {
        *State->MemoryAt++ = *String++;
    }
    *State->MemoryAt++ = 0;
    State->MemoryUsed = Length + 1;
    
    return Result;
}

global_variable u32 RandomColorCounter = 0;

v3 DebugColors[] = {
    /* 00 */ {1, 0, 0},
    /* 01 */ {0, 1, 0},
    /* 02 */ {0, 0, 1},
    /* 03 */ {1, 1, 0},
    /* 04 */ {0, 1, 1},
    /* 05 */ {1, 0, 1},
    /* 06 */ {1, 0.5f, 0},
    /* 07 */ {1, 0, 0.5f},
    /* 08 */ {0.5f, 1, 0},
    /* 09 */ {0, 1, 0.5f},
    /* 10 */ {0.5f, 0, 1},
    /* 11 */ {1, 0.75f, 0.5f},
    /* 12 */ {1, 0.5f, 0.75f},
    /* 13 */ {0.75f, 1, 0.5f},
    /* 14 */ {0.5f, 1, 0.75f},
    /* 15 */ {0.5f, 0.75f, 1},
};

static v3
UIRandomColor3(u32 Index) {
    return(DebugColors[Index % ArrayCount(DebugColors)]);
}

static v4
UIRandomColor4(u32 Index, f32 Alpha = 1.0f) {
    return(V4(DebugColors[Index % ArrayCount(DebugColors)], 1.0f));
}

inline rectangle2
UIRectMinDim(v2 Min, v2 Dim) {
    rectangle2 Result;
    Result.Min = Min;
    Result.Max = Min + Dim;
    return(Result);
} 

inline rectangle2
UIRectMinMax(v2 Min, v2 Max) {
    rectangle2 Result;
    Result.Min = Min;
    Result.Max = Max;
    return(Result);
}

inline v2
UIGetCenter(rectangle2 Rect) {
    v2 Result = 0.5f*(Rect.Min + Rect.Max);
    return(Result);
}

inline v2
UIConvertToPixelSpace(ui_state* State, v2 Input, u32 CoordinateType) {
    v2 Result = {};
    switch(CoordinateType) {
        
        case UI_PIXEL_SPACE: {
            Result = Input;
        } break;
        
        case UI_SCREEN_SPACE: {
            Result = Input*State->DrawRegionDimInPixels;
        } break;
        
        case UI_CLIP_SPACE: {
            Result = (Input + V2(1, 1))*(State->DrawRegionDimInPixels/2.0f);
        } break;
        
        default: {
            Assert(!"Invalid Coordinate Type");
        } break;
    }
    return Result;
}

inline rectangle2
UIConvertToPixelSpace(ui_state* State, rectangle2 Rect, u32 CoordinateType) {
    rectangle2 Result = {};
    v2 Min = UIConvertToPixelSpace(State, Rect.Min, CoordinateType);
    v2 Max = UIConvertToPixelSpace(State, Rect.Max, CoordinateType);
    Result = RectMinMax(Min, Max);
    return Result;
}

enum ui_text_alignment_flags {
    UI_TEXT_LEFT =   1 << 0,
    UI_TEXT_RIGHT =  1 << 1,
    UI_TEXT_CENTER = 1 << 2,
    UI_TEXT_TOP =    1 << 3,
    UI_TEXT_BOTTOM = 1 << 4,
};


static v2
UIDrawTextJustified(rectangle2 BoundingBox, char* Text, u32* FontSize, u32 Flags = UI_TEXT_CENTER) {
    
    f32 Padding = 0.01f;
    f32 BoundingWidth = GetWidth(BoundingBox);
    f32 BoundingHeight = GetHeight(BoundingBox);
    
    BoundingBox = AddRadiusTo(BoundingBox, V2(-Padding, -Padding));
    
    v2 TextOutAt = BoundingBox.Min;
    
    rectangle2 TextRect = SizeText(Text, TextOutAt.x, TextOutAt.y, *FontSize);
    
    v2 BoundingDim = GetDim(BoundingBox);
    v2 TextDim = GetDim(TextRect);
    
    while(TextDim.x > BoundingDim.x || TextDim.y > BoundingDim.y) {
        *FontSize = *FontSize - 1;
        if(*FontSize <= UI_MIN_FONT_SIZE) {
            
            break;
        }
        TextRect = SizeText(Text, TextOutAt.x, TextOutAt.y, *FontSize);
        TextDim = GetDim(TextRect);
    }
    
    f32 AdditionalYOffset = 0;
    if(TextRect.Min.y < BoundingBox.Min.y) {
        AdditionalYOffset = BoundingBox.Min.y - TextRect.Min.y;
        TextRect = Offset(TextRect, V2(0, AdditionalYOffset));
        
    }
    
    v2 CenterBoundingBox = UIGetCenter(BoundingBox);
    
    if(Flags & UI_TEXT_CENTER) {
        TextOutAt = CenterBoundingBox - TextDim*0.5f;
    }
    
    if(Flags & UI_TEXT_LEFT) {
        TextOutAt.x = BoundingBox.Min.x;
    }
    
    if(Flags & UI_TEXT_RIGHT) {
        TextOutAt.x = BoundingBox.Max.x - TextDim.x;
    }
    
    if(Flags & UI_TEXT_TOP) {
        TextOutAt.y = BoundingBox.Max.y - TextDim.y;
    }
    
    if(Flags & UI_TEXT_BOTTOM) {
        TextOutAt.y = BoundingBox.Min.y;
    }
    
    TextOutAt.y += AdditionalYOffset;
    return(TextOutAt);
}


inline void
UIBringToFront(ui_state* State, ui_window* Window) {
    Window->ZValue = --State->LastZValue;
    // TODO(Jesse): Check for overflow?
}

static b32
UIInteractionsAreEqual(ui_interaction A, ui_interaction B) {
    b32 Result = (A.ID == B.ID);
    return(Result);
}
static b32
UIIsInRectangle(rectangle2 Rectangle, v2 Test) {
    if(Rectangle.Min.x > Rectangle.Max.x) {
        f32 Temp = Rectangle.Min.x;
        Rectangle.Min.x = Rectangle.Max.x;
        Rectangle.Max.x = Temp; 
    }
    if(Rectangle.Min.y > Rectangle.Max.y) {
        f32 Temp = Rectangle.Min.y;
        Rectangle.Min.y = Rectangle.Max.y;
        Rectangle.Max.y = Temp; 
    }
    b32 Result = ((Test.x >= Rectangle.Min.x) &&
                  (Test.y >= Rectangle.Min.y) &&
                  (Test.x < Rectangle.Max.x) &&
                  (Test.y < Rectangle.Max.y));
    
    if(Result) {
        int breakpoint = 0;
    }
    
    return(Result);
}

static b32
UIIsInCircle(circle Circle, v2 Test) {
    b32 Result = false;
    
    f32 X = (Test.x - Circle.P.x);
    f32 Y = (Test.y - Circle.P.y);
    f32 DistSq = (X*X) + (Y*Y);
    
    if(DistSq < (Circle.r * Circle.r)) {
        Result = true;
    }
    
    return Result;
}

static void
UISetCurrentHot(ui_state* State, ui_interaction* Interaction) {
    if(!UIInteractionsAreEqual(State->CurrentHot, *Interaction)) {
        Interaction->State = UI_INTERACTION_STATE_HOT;
        Interaction->TimeSinceStateChange = State->CurrentTimer;
        State->CurrentHot = *Interaction;
    }
}

static void
UISetNextInteraction(ui_state* State, ui_interaction* Interaction) {
    if(!UIInteractionsAreEqual(State->NextInteraction, *Interaction)) {
        Interaction->State = UI_INTERACTION_STATE_NEXT_SUBMIT;
        Interaction->TimeSinceStateChange = State->CurrentTimer;
        State->NextInteraction = *Interaction;
    }
}

static void
UISetCurrentInteraction(ui_state* State, ui_interaction* Interaction) {
    State->IsInteracting = true;
    
    if(!UIInteractionsAreEqual(State->CurrentInteraction, *Interaction)) {
        Interaction->State = UI_INTERACTION_STATE_SUBMIT;
        Interaction->TimeSinceStateChange = State->CurrentTimer;
        State->CurrentInteraction = *Interaction;
    }
}


static void
UIClearCurrentHot(ui_state* State) {
    State->CurrentHot = {};
}

static void
UIClearNextInteraction(ui_state* State) {
    State->NextInteraction = {};
}

static void
UIClearCurrentInteraction(ui_state* State) {
    State->CurrentInteraction = {};
    State->IsInteracting = false;
}

static b32
UIIsHot(ui_window* Window,  ui_element* El, ui_interaction Interaction) {
    ui_state* State = Window->State;
    b32 Hot = ((Interaction.SubmitType != UI_SUBMIT_NONE) && 
               (State->Focus == Window->ID) && 
               (!State->IsInteracting));
    
    if(Hot) {
        Hot = false;
        switch(El->Type) {
            
            case(UI_ELEMENT_BOUNDS_RECT): {
                Hot = UIIsInRectangle(El->Rect, State->MouseP);
            } break;
            
            default: {
                Assert(!"El Type Not Set");
            } break;
        }
    }
    
    return(Hot);
}

static ui_interaction
UISubmitElement(ui_state* State, ui_element* El, u32 ID, u32 SubmitType) {
    //NOTE(Jesse): This function is mainly how things are set, the control flow of how things are unset, check UIBeginFrame();
    ui_window* Window = State->CurrentWindow;
    
    ui_interaction Interaction = {};
    Interaction.ID = ID;
    Interaction.SubmitType = SubmitType;
    
    b32 Hot = UIIsHot(Window, El, Interaction);
    
    if(UIInteractionsAreEqual(State->CurrentInteraction, Interaction)) {
        Interaction = State->CurrentInteraction;
        if(State->IsInteracting) {
            Window->LastInteraction = Interaction;
            return Interaction;
        }
    } else if(UIInteractionsAreEqual(State->CurrentHot, Interaction)) {
        Interaction = State->CurrentHot;
        if(!Hot) {
            //UIClearCurrentHot(State);
        }
    } 
    
    if(UIInteractionsAreEqual(State->NextInteraction, Interaction)) {
        Interaction = State->NextInteraction;
    }
    
    
    if(Hot) {
        UISetCurrentHot(State, &Interaction);
        switch(Interaction.SubmitType) {
            
            case(UI_SUBMIT_ON_PRESS): {
                if(State->WasPressed) {
                    UISetCurrentInteraction(State, &Interaction);
                }
            } break;
            
            case(UI_SUBMIT_ON_HOLD): {
                if(State->WasPressed) {
                    UISetCurrentInteraction(State, &Interaction);
                }
            } break;
            
            case(UI_SUBMIT_ON_RELEASE): {
                if(State->WasPressed) {
                    UISetNextInteraction(State, &Interaction);
                }
                
                if(UIInteractionsAreEqual(State->NextInteraction, Interaction) && State->WentUp) {
                    Interaction.State = UI_INTERACTION_STATE_SUBMIT;
                    UIClearNextInteraction(State);
                    //NOTE(Jesse): We don't set current interaction because we only need the submit on this frame
                }
            } break;
        }
    }
    
    if(Interaction.State == UI_INTERACTION_STATE_SUBMIT) {
        Window->WasUpdated = true;
    }
    
    Window->LastInteraction = Interaction;
    return Interaction;
}

static ui_element 
UIBeginRectElement(ui_window* Window, rectangle2 Rect){
    ui_element El = {};
    El.Type = UI_ELEMENT_BOUNDS_RECT;
    El.Rect = Rect;
    Window->LastElement = El;
    return El;
}

static void
UIPushRenderElement(ui_window* Window, ui_render_element El) {
    
    if(Window->RenderElementCount < UI_MAX_RENDER_ELEMENTS) {
        Window->RenderElements[Window->RenderElementCount++] = El;
    } else {
        Assert(!"Too Many Render Elements To Push");
    }
}

static void
UIPushTriangle(ui_window* Window, ui_tri Tri, v4 Color, u32 TextureID = 0) {
    ui_render_element RenderElement = {};
    RenderElement.Type = UI_RENDER_TRI;
    RenderElement.Tri = Tri;
    RenderElement.Color = Color;
    RenderElement.TextureID = TextureID;
    UIPushRenderElement(Window, RenderElement);
}

static void
UIPushQuad(ui_window* Window, ui_quad Quad, v4 Color, u32 TextureID = 0) {
    ui_render_element RenderElement = {};
    RenderElement.Type = UI_RENDER_QUAD;
    RenderElement.Quad = Quad;
    RenderElement.Color = Color;
    RenderElement.TextureID = TextureID;
    UIPushRenderElement(Window, RenderElement);
}

static void
UIPushRect(ui_window* Window, rectangle2 Rect, v4 Color, u32 TextureID = 0) {
    ui_quad Quad = {};
    Quad.P0 = V2(Rect.Min.x, Rect.Min.y);
    Quad.P1 = V2(Rect.Min.x, Rect.Max.y);
    Quad.P2 = V2(Rect.Max.x, Rect.Max.y);
    Quad.P3 = V2(Rect.Max.x, Rect.Min.y);
    UIPushQuad(Window, Quad, Color, TextureID);
}

static void
UIPushLine(ui_window* Window, v2 From, v2 To, f32 Thickness, v4 Color, u32 TextureID = 0) {
    ui_quad Quad = {};
    
    v2 Line = To - From;
    v2 LinePerp = Normalize(Perp(Line));
    LinePerp *= Thickness;
    
    Quad.P0 = From - LinePerp;
    Quad.P1 = From + LinePerp;
    Quad.P2 = To + LinePerp;
    Quad.P3 = To - LinePerp;
    UIPushQuad(Window, Quad, Color, TextureID);
}

static void
_UIPushClipRect(ui_window* Window, rectangle2 Rect) {
    ui_render_element RenderElement = {};
    RenderElement.Type = UI_RENDER_CLIP_RECT;
    RenderElement.ClipRect = Rect;
    UIPushRenderElement(Window, RenderElement);
}

static rectangle2
UISetClipRect(ui_window* Window, rectangle2 NewClipRect) {
    rectangle2 PrevClipRect = Window->CurrentClipRect;
    Window->CurrentClipRect = NewClipRect;
    _UIPushClipRect(Window, NewClipRect);
    return PrevClipRect;
}

static void
UIPushRectOutline(ui_window* Window, rectangle2 Rect, v2 Thickness, v4 Color, u32 TextureID = 0) {
    v2 Dim = GetDim(Rect);
    if(Length(Dim) > 0) {
        UIPushRect(Window, UIRectMinDim(Rect.Min, V2(Thickness.x, Dim.y)), Color, TextureID);
        UIPushRect(Window, UIRectMinDim(V2(Rect.Min.x, (Rect.Max.y - Thickness.y)), V2(Dim.x, Thickness.y)), Color, TextureID);
        UIPushRect(Window, UIRectMinDim(V2(Rect.Max.x - Thickness.x, Rect.Min.y), V2(Thickness.x, Dim.y)), Color, TextureID);
        UIPushRect(Window, UIRectMinDim(Rect.Min, V2(Dim.x, Thickness.y)), Color, TextureID);
    }
}


static void
UIPushText(ui_window* Window, char* Text, u32 FontSize, v2 TextAt, v4 Color, u32 TextureID = 0) {
    
    ui_render_element RenderElement = {};
    RenderElement.Type = UI_RENDER_TEXT;
    RenderElement.Text = Text;
    RenderElement.FontSize = FontSize;
    RenderElement.TextAt = TextAt;
    RenderElement.Color = Color;
    RenderElement.TextureID = TextureID;
    
    if(Window->RenderElementCount < UI_MAX_RENDER_ELEMENTS) {
        Window->RenderElements[Window->RenderElementCount++] = RenderElement;
    } else {
        Assert(!"Too Many Render Elements To Push");
    }
}

static ui_element
UIGetLastElement(ui_window* Window) {
    ui_element Result;
    Result = Window->LastElement;
    return Result;
}

static ui_window*
GetWindowFromID(ui_state* State, u32 ID) {
    ui_window* Found = 0;
    for(u32 WindowIndex = 0;
        WindowIndex < State->WindowCount;
        WindowIndex++) {
        ui_window* Test = &State->Windows[WindowIndex];
        if(Test->ID == ID) {
            Found = Test;
        }
    }
    return(Found);
}

inline void
AddWindow(ui_state* State, ui_window Window) {
    Assert(State->WindowCount < UI_MAX_WINDOWS);
    State->Windows[State->WindowCount++] = Window;
}

inline void
RemoveWindow(ui_state* State, u32 WindowID) {
    Assert(State->WindowCount > 0);
    b32 Found = false;
    for(u32 WindowIndex = 0;
        WindowIndex < State->WindowCount;
        WindowIndex++) {
        
        ui_window* Test = &State->Windows[WindowIndex];
        
        if(Test->ID == WindowID) {
            Found = true;
            
            while(WindowIndex < (State->WindowCount - 1)) {
                Test = &State->Windows[WindowIndex];ui_window* Next = &State->Windows[WindowIndex + 1];
                *Test = *Next;
                WindowIndex++;
            }
        }
    }
    
    if(Found) {
        State->WindowCount--;
    } else {
        Assert(!"Could not find window to remove");
    }
}

inline ui_window*
UIBeginWindow(ui_state* State, rectangle2 Rect, u32 ID, ui_window_style Style, u32 WindowFlags) {
    
    b32 Found = false;
    
    u32 WindowIndex = 0;
    //NOTE(jesse): Replace with has function?
    
    while(WindowIndex < State->WindowCount) {
        ui_window* Test = &State->Windows[WindowIndex];
        if(Test->ID == ID) {
            Found = true;
            break;
        }
        WindowIndex++;
    }
    
    ui_window* Result;
    if(Found) {
        Result = &State->Windows[WindowIndex];
        Result->WorkingRect = Result->BodyRect;
    } else {
        Result = &State->Windows[State->WindowCount++];
        Result->BodyRect = Rect;
        Result->WorkingRect = Rect;
        Result->ID = ID;
        Result->Style = Style;
        Result->WindowFlags = WindowFlags;
        
        if(WindowFlags & ~UI_WINDOW_ALWAYS_ON_BOTTOM) {
            UIBringToFront(State, Result);
        }
    }
    
    Result->KeepAlive = true; 
    Result->WasUpdated = false;
    Result->RenderElementCount = 0;
    
    
    State->CurrentWindow = Result;
    Result->State = State;
    
    return(Result);
}

inline ui_window*
UIEndWindow(ui_window* Window) {
    
}

static u32
SetFocus(ui_state* State) {
    u32 Result = 0;
    f32 LowestZValue = F32MAX;
    for(u32 WindowIndex = 0;
        WindowIndex < State->WindowCount;
        WindowIndex++) {
        ui_window* Test = &State->Windows[WindowIndex];
        if(UIIsInRectangle(Test->BodyRect, State->MouseP)) {
            if(Test->ZValue < LowestZValue) {
                LowestZValue = Test->ZValue;
                Result = Test->ID;
            }
        }
    }
    return(Result);
}

inline void
UIBeginFrame(ui_state* State, rectangle2 DrawRegionInPixels, v2 PixelMouseP, v2 dPixelMouseP, b32 WasPressed, b32 IsDown, b32 WentUp, char* TextInput, u32 TextInputCount, b32 ShiftPressed, b32 CtrlPressed, b32 AltPressed, f32 dt, u32 Flags) {
    
    if(WasPressed) {
        int Break = 0;
    }
    RandomColorCounter = 0;
    
    State->Flags = Flags;
    
    if(ShiftPressed) {
        State->InputFlags |= UI_INPUT_SHIFT;
    }
    if(CtrlPressed) {
        State->InputFlags |= UI_INPUT_ALT;
    }
    if(AltPressed) {
        State->InputFlags |= UI_INPUT_ALT;
    }
    
    State->WasPressed = WasPressed;
    State->IsDown = IsDown;
    State->WentUp = WentUp;
    State->dt = dt;
    
    State->MouseP = PixelMouseP;
    State->dMouseP = dPixelMouseP;
    
    for(u32 CharIndex = 0;
        CharIndex < TextInputCount;
        CharIndex++) {
        State->TextInput[CharIndex] = TextInput[CharIndex];
    }
    State->TextInputCount = TextInputCount;
    
    State->DrawRegionInPixels = DrawRegionInPixels;
    State->DrawRegionDimInPixels = GetDim(DrawRegionInPixels);
    
    
    if(State->IsInteracting){
        if(State->WentUp) {
            UIClearCurrentInteraction(State);
        }
        
        if(State->CurrentInteraction.SubmitType == UI_SUBMIT_ON_PRESS && State->IsDown) {
            UIClearCurrentInteraction(State);
        }
    }
    
    State->Focus = SetFocus(State);
    
    //NOTE(Jesse): If we do not touch the window in code control flow this frame we remove it from the list and assume it was deleted/temporary
    for(u32 WindowIndex = 0;
        WindowIndex < State->WindowCount;
        WindowIndex++) {
        ui_window* Window = &State->Windows[WindowIndex];
        Window->KeepAlive = false;
    }
    
    State->MemoryUsed = 0;
    State->MemoryAt = State->MemoryBase;
}



struct sort_entry
{
    u32 Key;
    u32 Index;
};

inline void
Swap(sort_entry* A, sort_entry* B) {
    sort_entry Temp = *B;
    *B = *A;
    *A = Temp;
}

static void
UISort(u32 Count, sort_entry* First, sort_entry* Temp) {
    
    if(Count == 0) {
        return;
    } else if(Count == 1) {
        
    } else if(Count == 2) {
        sort_entry* A = First;
        sort_entry* B = First + 1;
        if(A->Key > B->Key) {
            Swap(A, B);
        }
    } else {
        u32 Half0 = Count / 2;
        u32 Half1 = Count - Half0;
        
        Assert(Half0 >= 1);
        Assert(Half1 >= 1);
        
        sort_entry *InHalf0 = First;
        sort_entry *InHalf1 = First + Half0;
        sort_entry *End = First + Count;
        
        UISort(Half0, InHalf0, Temp);
        UISort(Half1, InHalf1, Temp);
        
        sort_entry *ReadHalf0 = InHalf0;
        sort_entry *ReadHalf1 = InHalf1;
        
        sort_entry *Out = Temp;
        for(u32 Index = 0;
            Index < Count;
            ++Index) {
            if(ReadHalf0 == InHalf1) {
                *Out++ = *ReadHalf1++;
            }
            else if(ReadHalf1 == End) {
                *Out++ = *ReadHalf0++;
            }
            else if(ReadHalf0->Key < ReadHalf1->Key) {
                *Out++ = *ReadHalf0++;
            } else {
                *Out++ = *ReadHalf1++;
            }
        }
        Assert(Out == (Temp + Count));
        Assert(ReadHalf0 == InHalf1);
        Assert(ReadHalf1 == End);
        
        for(u32 Index = 0;
            Index < Count;
            ++Index) {
            First[Index] = Temp[Index];
        }
    }
}


inline void
UIUpdateTime(ui_state* State) {
    State->CurrentTimer += State->dt;
    if(State->CurrentTimer > UI_MAX_TIME) {
        State->CurrentTimer = 0.0f;
    }
}

inline ui_render_array*
UIEndFrame(ui_state* State) {
    
    UIUpdateTime(State);
    
    for(u32 WindowIndex = 0;
        WindowIndex < State->WindowCount;
        WindowIndex++) {
        ui_window* Window = &State->Windows[WindowIndex];
        if(!Window->KeepAlive) {
            RemoveWindow(State, Window->ID);
        }
    }
    
    ui_render_array* Result = &State->FinalRenderList;
    
    sort_entry Entries[UI_MAX_WINDOWS] = {};
    sort_entry Temp[UI_MAX_WINDOWS] = {};
    
    u32 WindowsBroughtToFront = 0;
    for(u32 WindowIndex = 0;
        WindowIndex < State->WindowCount;
        WindowIndex++) {
        
        ui_window* Window = &State->Windows[WindowIndex];
        
        
        if(Window->WasUpdated && (Window->WindowFlags & ~UI_WINDOW_ALWAYS_ON_BOTTOM)) {
            UIBringToFront(State, Window);
            WindowsBroughtToFront++;
            Assert(WindowsBroughtToFront < 2);
            Window->WasUpdated = false;
        }
        
        Entries[WindowIndex].Key = -Window->ZValue;
        Entries[WindowIndex].Index = WindowIndex;
    }
    
    UISort(State->WindowCount, &Entries[0], &Temp[0]);
    
    
    f32 ZValue = 0;
    u32 RenderElementCount = 0;
    for(u32 WindowIndex = 0;
        WindowIndex < State->WindowCount;
        WindowIndex++) {
        
        u32 Index = Entries[WindowIndex].Index;
        ui_window* Window = &State->Windows[Index];
        
        
        ZValue -= 5;
        for(u32 RenderIndex = 0;
            RenderIndex < Window->RenderElementCount;
            RenderIndex++) { 
            
            ui_render_element* RenderEl = &Window->RenderElements[RenderIndex];
            RenderEl->ZValue = ZValue ;
            ZValue -= 0.1;
            
            Result->RenderElements[RenderElementCount++]= *RenderEl;
        }
    }
    
    Result->RenderElementCount = RenderElementCount;
    return(Result);
}

