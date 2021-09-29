
static v4
SetElementColor(ui_window* Window, ui_interaction Interaction, v4 Color) {
    
    v4 Result = Color;
    if(Interaction.SubmitType == UI_SUBMIT_NONE) {
        return Result;
    }
    
    f32 t = Window->State->CurrentTimer - Interaction.TimeSinceStateChange;
    u32 State = Interaction.State;
    
    f32 FadeTimeInv = 1.0f/0.2f;
    
    v4 HotDelta = V4(0.04f, 0.04f, 0.04f, 0.0f);
    v4 NextSubmitDelta = V4(0.07f, 0.07f, 0.07f, 0.0f);
    v4 SubmitDelta = V4(0.1f, 0.1f, 0.1f, 0.0f);
    
    if(State == UI_INTERACTION_STATE_HOT) {
        t *= FadeTimeInv;
        Result = Lerp(Color, Clamp01(t), Color + HotDelta);
    } else if(State == UI_INTERACTION_STATE_NEXT_SUBMIT) {
        t *= FadeTimeInv;
        Result = Lerp(Color, Clamp01(t), Color + NextSubmitDelta);
    } else if(State == UI_INTERACTION_STATE_SUBMIT) {
        t *= FadeTimeInv;
        Result = Lerp(Color, Clamp01(t), Color + SubmitDelta);
    } 
    return Result;
}

static v4
SetTextColor(v4 BackgroundColor) {
    v4 Result = {};
    
    ui_button_style ButtonStyle = UIDefaultButtonStyle;
    v4 Light = ButtonStyle.LightTextColor;
    v4 Dark = ButtonStyle.DarkTextColor;
    
    //NOTE(Jesse): Standard Luminance Calculation
    f32 Lux = (0.2126*BackgroundColor.r) + (0.7152*BackgroundColor.g) + (0.0722*BackgroundColor.b);
    if(Lux > 0.45) {
        Result = Dark;
    } else {
        Result = Light;
    }
    
    return Result;
}

static b32
UIButtonOutline(ui_window* Window, u32 ID, char* Text, rectangle2 Rect, u32 SubmitType, u32 TextureID, u32 TextAlignment) {
    b32 Result = false;
    
    ui_button_style ButtonStyle = UIDefaultButtonStyle;
    rectangle2 ActualButtonRect = AddRadiusTo(Rect, -V2(ButtonStyle.BorderSize));
    
    ui_element El = UIBeginRectElement(Window, ActualButtonRect);
    ui_interaction Interaction = UISubmitElement(Window->State, &El, ID, SubmitType);
    
    v4 TextColorToUse = V4(1, 1, 1, 1);
    
    UIPushRectOutline(Window, Rect, V2(2, 2), SetElementColor(Window, Interaction, ButtonStyle.BorderColor), 0);
    
    if(Text) {
        u32 FontSize = ButtonStyle.FontSize;
        v2 TextP = UIDrawTextJustified(ActualButtonRect, Text, &FontSize, TextAlignment);
        UIPushText(Window, Text, FontSize, TextP, TextColorToUse);
    }
    
    if(Interaction.State == UI_INTERACTION_STATE_SUBMIT) {
        Result = true;
    }
    return Result;
}


static b32
UIButtonInternal(ui_window* Window, u32 ID, char* Text, rectangle2 Rect, u32 SubmitType, u32 TextureID, u32 TextAlignment) {
    b32 Result = false;
    
    ui_button_style ButtonStyle = UIDefaultButtonStyle;
    rectangle2 ActualButtonRect = AddRadiusTo(Rect, -V2(ButtonStyle.BorderSize));
    
    ui_element El = UIBeginRectElement(Window, ActualButtonRect);
    ui_interaction Interaction = UISubmitElement(Window->State, &El, ID, SubmitType);
    
    v4 ColorToUse = SetElementColor(Window, Interaction, ButtonStyle.Color);
    v4 TextColorToUse = SetTextColor(ColorToUse);
    
    UIPushRect(Window, Rect, ButtonStyle.BorderColor, 0);
    UIPushRect(Window, ActualButtonRect, ColorToUse, TextureID);
    
    if(Text) {
        u32 FontSize = ButtonStyle.FontSize;
        v2 TextP = UIDrawTextJustified(ActualButtonRect, Text, &FontSize, TextAlignment);
        UIPushText(Window, Text, FontSize, TextP, TextColorToUse);
    }
    
    if(Interaction.State == UI_INTERACTION_STATE_SUBMIT) {
        Result = true;
    }
    return Result;
}

static b32
UIButton(ui_window* Window, u32 AdditionalID, char* Text, rectangle2 Rect, u32 SubmitType = UI_SUBMIT_ON_RELEASE, u32 TextureID = 0,
         u32 TextAlignment = UI_TEXT_CENTER) {
    b32 Result = false;
    u32 ID = Window->ID;
    ID = UIGenerateID(&ID, &AdditionalID, sizeof(u32));
    Result = UIButtonInternal(Window, ID, Text, Rect, SubmitType, TextureID, TextAlignment);
    return Result;
}

static b32
UIButton(ui_window* Window, char* Text, rectangle2 Rect, u32 SubmitType = UI_SUBMIT_ON_RELEASE, u32 TextureID = 0,
         u32 TextAlignment = UI_TEXT_CENTER) {
    b32 Result = false;
    u32 ID = Window->ID;
    ID = UIGenerateID(&ID, Text, UIStringLength(Text));
    Result = UIButtonInternal(Window, ID, Text, Rect, SubmitType, TextureID, TextAlignment);
    return Result;
}

static b32
UIButton(ui_window* Window, u32 AdditionalID, rectangle2 Rect, u32 SubmitType = UI_SUBMIT_ON_RELEASE, u32 TextureID = 0,
         u32 TextAlignment = UI_TEXT_CENTER) {
    b32 Result = false;
    u32 ID = Window->ID;
    ID = UIGenerateID(&ID, &AdditionalID, sizeof(u32));
    Result = UIButton(Window, ID, 0, Rect, SubmitType, TextureID);
    return Result;
}

static b32
UIButton(ui_window* Window, char* Text, rectangle2 Rect, v4 Color, u32 SubmitType = UI_SUBMIT_NONE, u32 TextureID = 0,
         u32 TextAlignment = UI_TEXT_CENTER) {
    ui_button_style* Style = &UIDefaultButtonStyle;
    v4 PrevColor = Style->Color;
    Style->Color = Color;
    b32 Result = UIButton(Window, Text, Rect, SubmitType, TextureID, TextAlignment);
    Style->Color = PrevColor;
    return Result;
}

static b32
UIButton(ui_window* Window, rectangle2 Rect, u32 SubmitType = UI_SUBMIT_ON_RELEASE, u32 TextureID = 0) {
    b32 Result = UIButton(Window, UIUniqueID(Window->State), 0, Rect, SubmitType, TextureID, 0);
}

static b32
UIHeader(ui_window* Window, char* Text, rectangle2 Rect, u32 TextureID = 0) {
    b32 Result = false;
    Result = UIButton(Window, Text, Rect, UI_SUBMIT_ON_HOLD, TextureID);
    return Result;
}

static b32
UIToggle(ui_window* Window, b32* B32, rectangle2 Rect, u32 TextureID = 0) {
    
    ui_button_style ButtonStyle = UIDefaultButtonStyle;
    
    rectangle2 ActualButtonRect = AddRadiusTo(Rect, -V2(ButtonStyle.BorderSize));
    
    ui_element El = UIBeginRectElement(Window, ActualButtonRect);
    u32 ID = U32FromPointer(B32);
    ui_interaction Interaction = UISubmitElement(Window->State, &El, ID, UI_SUBMIT_ON_RELEASE);
    
    if(Interaction.State == UI_INTERACTION_STATE_SUBMIT) {
        *B32 = !(*B32);
    }
    
    
    v4 ColorToUse = SetElementColor(Window, Interaction, ButtonStyle.Color);
    v4 TextColorToUse = SetTextColor(ColorToUse);
    
    UIPushRect(Window, Rect, ButtonStyle.BorderColor);
    UIPushRect(Window, ActualButtonRect, ColorToUse);
    
    rectangle2 InsideRect = AddRadiusTo(ActualButtonRect, -V2(4, 4));
    UIPushRect(Window, InsideRect, ButtonStyle.BorderColor);
    if(*B32) {
        rectangle2 InsideInsideRect = AddRadiusTo(InsideRect, -V2(4, 4));
        UIPushRect(Window, InsideInsideRect, ColorToUse);
    }
    return *B32;
}

static b32
UIToggle(ui_window* Window, b32* B32, char* Text, rectangle2 Rect, u32 TextureID = 0) {
    f32 Height = GetHeight(Rect);
    rectangle2 ToggleRect = UICutLeft(&Rect, Height);
    
    b32 Result = UIToggle(Window, B32, ToggleRect, TextureID);
    rectangle2 ActualTextRect = AddRadiusTo(Rect, -V2(4, 4));
    UIButton(Window, Text, Rect, UI_SUBMIT_NONE);
    return Result;
}

#define MAX_SCROLL_CONTENT_SIZE 1000000.0f;
static rectangle2*
UIBeginScrollableY(ui_window* Window, rectangle2 Rect) {
    
    if(Window->ScrollbarIndex >= UI_MAX_WINDOW_SCROLLBARS) {
        Assert(!"I don't know how you made this many scrollbars in a window");
    }
    
    ui_scrollbar_info* Info = &Window->ScrollInfos[Window->ScrollbarIndex++];
    
    Info->WorkingRect = Rect;
    Info->ScrollbarRect = UICutRight(&Info->WorkingRect, 20);
    
    Info->WorkingRect.Min.y = -MAX_SCROLL_CONTENT_SIZE;
    Info->WorkingRect = Offset(Info->WorkingRect, Info->LastScrollOffset);
    
    Info->PreviousClipRect = UISetClipRect(Window, Rect);
    Info->ContentRect = Info->WorkingRect;
    
    return &Info->WorkingRect;
}

static void
UIEndScrollableY(ui_window* Window) {
    
    ui_scrollbar_info* Info = &Window->ScrollInfos[--Window->ScrollbarIndex];
    f32 HeightToCut = UIGetHeightFromRatio(&Info->WorkingRect, 1.0f);
    UICutBottom(&Info->ContentRect, HeightToCut);
    
    f32 ContentHeight = GetHeight(Info->ContentRect);
    f32 BodyHeight = GetHeight(Info->ScrollbarRect);
    f32 MaxScroll = ContentHeight - BodyHeight;
    
    v4 ScrollBackgroundColor = UIDefaultTemplateStyle.ScrollBackgroundColor;
    UIPushRect(Window, Info->ScrollbarRect, ScrollBackgroundColor);
    
    f32 MaxThumbHeight = 20.0f;
    
    rectangle2 Thumb = Info->ScrollbarRect;
    f32 ThumbHeight = BodyHeight * (BodyHeight / ContentHeight);
    
    Thumb.Max.y -= ((Info->LastScrollOffset.y) * (BodyHeight - ThumbHeight) / MaxScroll);
    Thumb.Min.y  = Thumb.Max.y - ThumbHeight;
    
    if(UIButton(Window, 4328490, Thumb, UI_SUBMIT_ON_HOLD)) {
        if(MaxScroll > 0 && BodyHeight > 0) {
            Info->LastScrollOffset.y -= (Window->State->dMouseP.y * ContentHeight / BodyHeight);
            Info->LastScrollOffset.y = Clamp(0, Info->LastScrollOffset.y, MaxScroll);
        }
    }
    UISetClipRect(Window, Info->PreviousClipRect);
}


static b32
UISliderX(ui_window* Window, f32* Value, f32 Min, f32 Max, rectangle2 Rect, char* Text = 0, u32 TextureID = 0) {
    
    b32 Result = false;
    if(!Text) {
        char* TempText = FormatString("%.2f", *Value);
        Text = UIPushString(Window, TempText);
    }
    
    u32 ID = U32FromPointer(Value);
    
    
    UIButton(Window, ID, Text, Rect, UI_SUBMIT_ON_HOLD, TextureID);
    
    ui_element El = Window->LastElement;
    ui_interaction Interaction = Window->LastInteraction;
    
    if(Interaction.State == UI_INTERACTION_STATE_SUBMIT) {
        v2 P = GetBarycentric(El.Rect, Window->State->MouseP);
        *Value = Lerp(Min, Clamp01(P.x), Max);
        Result = true;
    }
    
    v2 RectCenter = GetCenter(El.Rect);
    v2 SliderSize = V2(8, GetHeight(El.Rect));
    
    f32 Padding = SliderSize.x*0.5;
    f32 t = LinearConversion(Min, Max, 0.0f, 1.0f, *Value);
    f32 CurrentLerp = Lerp(El.Rect.Min.x + Padding, t, El.Rect.Max.x - Padding);
    rectangle2 SliderRect = RectCenterDim(V2(CurrentLerp, RectCenter.y), SliderSize);
    
    ui_template_style SliderStyle = UIDefaultTemplateStyle; 
    v4 ColorToUse = SetElementColor(Window, Interaction,  SliderStyle.SliderColor);
    rectangle2 ShadowRect = Offset(SliderRect, V2(-(t - 0.5)*8, 0));
    
    UIPushRect(Window, ShadowRect, ColorToUse - V4(0.2, 0.2, 0.2, 0.0));
    UIPushRect(Window, SliderRect, ColorToUse);
    
    return Result; 
}

static b32
UISliderXInt(ui_window* Window, s32* Value, s32 Min, s32 Max, rectangle2 Rect, char* Text = 0, u32 TextureID = 0) {
    
    ui_button_style ButtonStyle = UIDefaultButtonStyle;
    
    b32 Result = false;
    if(!Text) {
        char* TempText = FormatString("%d", *Value);
        Text = UIPushString(Window, TempText);
    }
    
    u32 ID = U32FromPointer(Value);
    
    UIButton(Window, ID, Text, Rect, UI_SUBMIT_ON_HOLD);
    
    ui_element El = Window->LastElement;
    ui_interaction Interaction = Window->LastInteraction;
    
    if(Interaction.State == UI_INTERACTION_STATE_SUBMIT) {
        v2 P = GetBarycentric(El.Rect, Window->State->MouseP);
        *Value = RoundF32ToS32((Lerp(Min, Clamp01(P.x), Max)));
        Result = true;
    }
    
    v2 RectCenter = GetCenter(El.Rect);
    v2 SliderSize = V2(8, GetHeight(El.Rect));
    
    f32 Padding = SliderSize.x*0.5;
    f32 t = LinearConversion((s32)Min, (s32)Max, 0.0f, 1.0f, *Value);
    f32 CurrentLerp = Lerp(El.Rect.Min.x + Padding, t, El.Rect.Max.x - Padding);
    rectangle2 SliderRect = RectCenterDim(V2(CurrentLerp, RectCenter.y), SliderSize);
    
    
    ui_template_style SliderStyle = UIDefaultTemplateStyle; 
    v4 ColorToUse = SetElementColor(Window, Interaction,  SliderStyle.SliderColor);
    rectangle2 ShadowRect = Offset(SliderRect, V2(-(t - 0.5)*8, 0));
    
    UIPushRect(Window, ShadowRect, ColorToUse - V4(0.2, 0.2, 0.2, 0.0));
    UIPushRect(Window, SliderRect, ColorToUse);
    
    return Result; 
}

static b32
UISliderY(ui_window* Window, f32* Value, f32 Min, f32 Max, rectangle2 Rect, char* Text = 0, u32 TextureID = 0) {
    
    ui_button_style ButtonStyle = UIDefaultButtonStyle;
    
    b32 Result = false;
    if(!Text) {
        char* TempText = FormatString("%.2f", *Value);
        Text = UIPushString(Window, TempText);
    }
    
    u32 ID = U32FromPointer(Value);
    UIButton(Window, ID, Text, Rect, UI_SUBMIT_ON_HOLD);
    
    ui_element El = Window->LastElement;
    ui_interaction Interaction = Window->LastInteraction;
    
    if(Interaction.State == UI_INTERACTION_STATE_SUBMIT) {
        v2 P = GetBarycentric(El.Rect, Window->State->MouseP);
        *Value = Lerp(Min, Clamp01(P.y), Max);
        Result = true;
    }
    
    v2 RectCenter = GetCenter(El.Rect);
    v2 SliderSize = V2(GetWidth(El.Rect), 8);
    
    f32 Padding = SliderSize.y*0.5;
    f32 t = LinearConversion(Min, Max, 0.0f, 1.0f, *Value);
    f32 CurrentLerp = Lerp(El.Rect.Min.y + Padding, t, El.Rect.Max.y - Padding);
    rectangle2 SliderRect = RectCenterDim(V2(RectCenter.x, CurrentLerp), SliderSize);
    
    ui_template_style SliderStyle = UIDefaultTemplateStyle; 
    v4 ColorToUse = SetElementColor(Window, Interaction,  SliderStyle.SliderColor);
    rectangle2 ShadowRect = Offset(SliderRect, V2(0, -(t - 0.5)*8));
    
    UIPushRect(Window, ShadowRect, ColorToUse - V4(0.2, 0.2, 0.2, 0.0));
    UIPushRect(Window, SliderRect, ColorToUse);
    
    return Result; 
}


static b32
UISliderYInt(ui_window* Window, s32* Value, s32 Min, s32 Max, rectangle2 Rect, char* Text = 0, u32 TextureID = 0) {
    
    ui_button_style ButtonStyle = UIDefaultButtonStyle;
    
    b32 Result = false;
    if(!Text) {
        char* TempText = FormatString("%d", *Value);
        Text = UIPushString(Window, TempText);
    }
    
    u32 ID = U32FromPointer(Value);
    UIButton(Window, ID, Text, Rect, UI_SUBMIT_ON_HOLD);
    
    ui_element El = Window->LastElement;
    ui_interaction Interaction = Window->LastInteraction;
    
    if(Interaction.State == UI_INTERACTION_STATE_SUBMIT) {
        v2 P = GetBarycentric(El.Rect, Window->State->MouseP);
        *Value = RoundF32ToS32(Lerp(Min, Clamp01(P.y), Max));
        Result = true;
    }
    
    v2 RectCenter = GetCenter(El.Rect);
    v2 SliderSize = V2(GetWidth(El.Rect), 8);
    
    f32 Padding = SliderSize.y*0.5;
    f32 t = LinearConversion(Min, Max, 0.0f, 1.0f, *Value);
    f32 CurrentLerp = Lerp(El.Rect.Min.y + Padding, t, El.Rect.Max.y - Padding);
    rectangle2 SliderRect = RectCenterDim(V2(RectCenter.x, CurrentLerp), SliderSize);
    
    ui_template_style SliderStyle = UIDefaultTemplateStyle; 
    v4 ColorToUse = SetElementColor(Window, Interaction,  SliderStyle.SliderColor);
    rectangle2 ShadowRect = Offset(SliderRect, V2(0, -(t - 0.5)*8));
    
    UIPushRect(Window, ShadowRect, ColorToUse - V4(0.2, 0.2, 0.2, 0.0));
    UIPushRect(Window, SliderRect, ColorToUse);
    
    return Result; 
} 

static b32
UISliderV2(ui_window* Window, v2* Value, v2 Min, v2 Max, rectangle2 Rect, char* Text = 0, u32 TextureID = 0) {
    
    ui_button_style ButtonStyle = UIDefaultButtonStyle;
    
    b32 Result = false;
    if(!Text) {
        char* TempText = FormatString("%.2f, %.2f", Value->x, Value->y);
        Text = UIPushString(Window, TempText);
    }
    
    u32 ID = U32FromPointer(Value);
    ID += 438920;
    UIButton(Window, ID, Text, Rect, UI_SUBMIT_ON_HOLD);
    
    ui_element El = Window->LastElement;
    ui_interaction Interaction = Window->LastInteraction;
    
    if(Interaction.State == UI_INTERACTION_STATE_SUBMIT) {
        v2 P = GetBarycentric(El.Rect, Window->State->MouseP);
        Value->x = Lerp(Min.x, Clamp01(P.x), Max.y);
        Value->y = Lerp(Min.y, Clamp01(P.y), Max.x);
        Result = true;
    }
    
    v2 RectCenter = GetCenter(El.Rect);
    v2 SliderSize = V2(8, 8);
    
    f32 Padding = SliderSize.x*0.5;
    f32 tX = LinearConversion(Min.x, Max.x, 0.0f, 1.0f, Value->x);
    f32 tY = LinearConversion(Min.y, Max.y, 0.0f, 1.0f, Value->y);
    f32 CurrentLerpX = Lerp(El.Rect.Min.x + Padding, tX, El.Rect.Max.x - Padding);
    f32 CurrentLerpY = Lerp(El.Rect.Min.y + Padding, tY, El.Rect.Max.y - Padding);
    rectangle2 SliderRect = RectCenterDim(V2(CurrentLerpX, CurrentLerpY), SliderSize);
    
    ui_template_style SliderStyle = UIDefaultTemplateStyle; 
    v4 ColorToUse = SetElementColor(Window, Interaction,  SliderStyle.SliderColor);
    rectangle2 ShadowRect = Offset(SliderRect, V2(-(tX - 0.5)*8, -(tY - 0.5)*8));
    
    UIPushRect(Window, ShadowRect, ColorToUse - V4(0.2, 0.2, 0.2, 0.0));
    UIPushRect(Window, SliderRect, ColorToUse);
    
    return Result; 
}

static b32
UISliderV3(ui_window* Window, v3* Value, v3 Min, v3 Max, rectangle2 Rect) {
    b32 Result = false;
    
    f32 SliderRatio = UIDefaultTemplateStyle.V3SliderRatio;
    f32 SpacingRatio = (1.0 - (SliderRatio*3.0))/2.0;
    f32 SliderWidth = UIGetWidthFromRatio(&Rect, SliderRatio);
    f32 SpacingWidth = UIGetWidthFromRatio(&Rect, SpacingRatio);
    
    if(UISliderY(Window, &Value->x, Min.x, Max.x, UICutLeft(&Rect, SliderWidth))) {
        Result = true;
    }
    UICutLeft(&Rect, SpacingWidth);
    if(UISliderY(Window, &Value->y, Min.y, Max.y, UICutLeft(&Rect, SliderWidth))) {
        Result = true;
    }
    UICutLeft(&Rect, SpacingWidth);
    if(UISliderY(Window, &Value->z, Min.z, Max.z, UICutLeft(&Rect, SliderWidth))) {
        Result = true;
    }
    return Result;
}

static b32
UIColorPreview(ui_window* Window, char* Label, v4* Color, rectangle2 Rect) {
    b32 Result = UIButton(Window, Label, UICutLeftRatio(&Rect, 0.9));
    UIButton(Window, 0, Rect, *Color, UI_SUBMIT_NONE);
    return Result;
}

enum ui_list_result {
    UI_LIST_NONE = 0,
    UI_LIST_NEXT = 1,
    UI_LIST_PREV = 2,
};

static ui_list_result
UIList(ui_window* Window, char* Label, rectangle2 Rect) {
    ui_list_result Result = UI_LIST_NONE;
    f32 ButtonCut = UIGetWidthFromRatio(&Rect, 0.15f);
    
    if(UIButton(Window, "<", UICutLeft(&Rect, ButtonCut))) {
        Result = UI_LIST_PREV;
    }
    if(UIButton(Window, ">", UICutRight(&Rect, ButtonCut))) {
        Result = UI_LIST_NEXT;
    }
    
    UIButton(Window, Label, Rect, UI_SUBMIT_NONE);
    return Result;
}

static ui_window*
UIBeginStandardWindow(ui_state* State, char* Title, rectangle2 Rect, u32 ID, u32 Flags = UI_WINDOW_DEFAULT) {
    
    ui_window_style Style = UIDefaultWindowStyle;
    rectangle2 WorkingWindowRect = AddRadiusTo(Rect, -V2(Style.BorderSize));
    ui_window* Result = UIBeginWindow(State, WorkingWindowRect, ID, Style, Flags);
    
    
    //NOTE(Jesse):Because the ui_state controls the window after the first call, we have to grab the rect and add the bordersize again
    
    rectangle2 WindowDrawRect = AddRadiusTo(Result->BodyRect, V2(Style.BorderSize));
    UISetClipRect(Result, WindowDrawRect);
    
    
    UIPushRect(Result, WindowDrawRect, Style.BorderColor);
    UIPushRect(Result, Result->BodyRect, Style.BackgroundColor);
    
    rectangle2* WorkingRect = &Result->WorkingRect;
    if(UIHeader(Result, Title, UICutTop(WorkingRect, 40))) {
        Result->BodyRect = Offset(Result->BodyRect, State->dMouseP);
    }
    
    if(Flags & UI_WINDOW_SIZE) {
        rectangle2 BottomRect = UICutBottom(&Result->WorkingRect, 20);
        UICutBottom(&Result->WorkingRect, 4);
        rectangle2 SizeRect = UICutRight(&BottomRect, 20);
        if(UIButton(Result, "+", SizeRect, UI_SUBMIT_ON_HOLD)) {
            Result->BodyRect.Max.x += State->dMouseP.x;
            Result->BodyRect.Min.y += State->dMouseP.y;
        }
    }
    
    return Result;
}