
static v4
SetElementColor(ui_window* Window, ui_interaction Interaction, v4 Color) {
    
    v4 Result = Color;
    if(Interaction.SubmitType == UI_SUBMIT_NONE) {
        return Result;
    }
    f32 t = Window->State->CurrentTimer - Interaction.TimeSinceStateChange;
    u32 State = Interaction.State;
    
    f32 FadeTimeInv = 1.0f/0.2f;
    
    v4 HotDelta = UIStyle.HotDelta;
    v4 NextSubmitDelta = UIStyle.NextSubmitDelta;
    v4 SubmitDelta = UIStyle.SubmitDelta;
    
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
    
    v4 Light = UIStyle.LightTextColor;
    v4 Dark = UIStyle.DarkTextColor;
    
    //NOTE(Jesse):  Luminance Calculation
    f32 Lux = (0.2126*BackgroundColor.r) + (0.7152*BackgroundColor.g) + (0.0722*BackgroundColor.b);
    if(Lux > 0.45) {
        Result = Dark;
    } else {
        Result = Light;
    }
    
    return Result;
}

static b32
UIDefaultButton(ui_window* Window, u32 ID, char* Text, rectangle2 Rect, u32 SubmitType) {
    
    b32 Result = UIButtonInternal(Window, ID, Rect, SubmitType);
    ui_interaction Interaction = UIGetLastInteraction(Window);
    
    if(Interaction.State == UI_INTERACTION_STATE_CULL) {
        return false;
    }
    
    v4 ButtonColor = UIStyle.ButtonColor;
    v4 ButtonBorderColor = UIStyle.ButtonBorderColor;
    v4 TextColorToUse = SetTextColor(ButtonColor);
    v2 ButtonBorderSize = -V2(UIStyle.ButtonBorderSize);
    
    ButtonColor = SetElementColor(Window, Interaction, ButtonColor);
    UIPushRect(Window, Rect, ButtonBorderColor);
    rectangle2 ButtonRect = AddRadiusTo(Rect, ButtonBorderSize);
    UIPushRect(Window, ButtonRect, ButtonColor);
    
    if(Text) {
        v2 TextP = UILayoutTextAlignment(Window, ButtonRect, Text);
        UIPushText(Window, Text, TextP, TextColorToUse);
    }
    return Result;
}

static b32
UIButton(ui_window* Window, char* Text, rectangle2 Rect, u32 SubmitType = UI_SUBMIT_ON_RELEASE) {
    ui_id ID = Window->ID;
    ID = UIGenerateID(&ID, Text, UIStringLength(Text));
    return UIDefaultButton(Window, ID, Text, Rect, SubmitType);
}

static b32
UIButtonOutline(ui_window* Window, char* Text, rectangle2 Rect, u32 SubmitType = UI_SUBMIT_ON_RELEASE) {
    
    ui_id ID = Window->ID;
    ID = UIGenerateID(&ID, Text, UIStringLength(Text));
    b32 Result = UIButtonInternal(Window, Window->ID, Rect, SubmitType);
    ui_interaction Interaction = UIGetLastInteraction(Window);
    
    v4 OutlineColor = SetElementColor(Window, Interaction, UIStyle.ButtonBorderColor);
    v4 TextColorToUse = UIStyle.LightTextColor;
    
    UIPushRectOutline(Window, Rect, V2(4.0, 4.0), OutlineColor);
    if(Text) {
        v2 TextP = UILayoutTextAlignment(Window, Rect, Text);
        UIPushText(Window, Text, TextP, TextColorToUse);
    }
    return Result;
}

static b32
UIHeader(ui_window* Window, char* Text, rectangle2 Rect) {
    b32 Result = false;
    Result = UIButton(Window, Text, Rect, UI_SUBMIT_ON_HOLD);
    return Result;
}

static b32
UIToggle(ui_window* Window, b32* B32, rectangle2 Rect) {
    
    rectangle2 ActualButtonRect = AddRadiusTo(Rect, -V2(UIStyle.ButtonBorderSize));
    
    ui_element El = UIBeginRectElement(Window, ActualButtonRect);
    u32 ID = U32FromPointer(B32);
    ui_interaction Interaction = UISubmitElement(Window->State, &El, ID, UI_SUBMIT_ON_RELEASE);
    
    if(Interaction.State == UI_INTERACTION_STATE_SUBMIT) {
        *B32 = !(*B32);
    }
    
    v4 ColorToUse = SetElementColor(Window, Interaction, UIStyle.ButtonColor);
    v4 ButtonBorderColor = UIStyle.ButtonBorderColor;
    v4 TextColorToUse = SetTextColor(ColorToUse);
    
    UIPushRect(Window, Rect, ButtonBorderColor);
    UIPushRect(Window, ActualButtonRect, ColorToUse);
    
    v2 ToggleButtonSize = -V2(UIStyle.ToggleButtonSize);
    rectangle2 InsideRect = AddRadiusTo(ActualButtonRect, ToggleButtonSize);
    if(*B32) {
        UIPushRect(Window, InsideRect, ButtonBorderColor);
    }
    
    return *B32;
}

static b32
UIToggle(ui_window* Window, b32* B32, char* Text, rectangle2 Rect) {
    f32 Height = GetHeight(Rect);
    rectangle2 ToggleRect = UICutLeft(&Rect, Height);
    b32 Result = UIToggle(Window, B32, ToggleRect);
    
    UIButton(Window, Text, Rect, UI_SUBMIT_NONE);
    return Result;
}

static rectangle2*
UIBeginScrollable(ui_window* Window, rectangle2 Rect, u32 AxisFlags) {
    
    Assert(Window->ScrollbarIndex <= UI_MAX_WINDOW_SCROLLBARS);
    Assert(AxisFlags);
    
    ui_scrollbar_info* Info = &Window->ScrollInfos[Window->ScrollbarIndex++];
    Info->WorkingRect = Rect;
    
    rectangle2 TempRect = Rect;
    v2 OffsetSize = {};
    if(AxisFlags & UI_YAXIS) {
        Info->ScrollbarRectY = UICutRight(&TempRect, UIStyle.ResizeSize);
        TempRect.Min.y = -MAX_SCROLL_CONTENT_SIZE;
        OffsetSize.y = Info->LastScrollOffset.y;
        Info->CornerRect = UICutBottom(&Info->ScrollbarRectY, UIStyle.ResizeSize);
        
    }
    
    if(AxisFlags & UI_XAXIS) {
        Info->ScrollbarRectX = UICutBottom(&Rect, UIStyle.ResizeSize);
        TempRect.Max.x = MAX_SCROLL_CONTENT_SIZE;
        OffsetSize.x = -Info->LastScrollOffset.x;
        Info->CornerRect = UICutRight(&Info->ScrollbarRectX, UIStyle.ResizeSize);
    }
    
    
    Info->WorkingRect = TempRect;
    Info->WorkingRect = Offset(Info->WorkingRect, OffsetSize);
    Info->PreviousClipRect = UISetClipRect(Window, Rect);
    Info->ContentRect = Info->WorkingRect;
    Info->AxisFlags = AxisFlags;
    
    Window->IsInScrollbarMode = true;
    Window->CurrentScrollbarClipRect = Rect;
    return &Info->WorkingRect;
}

static void
UIEndScrollable(ui_window* Window) {
    ui_scrollbar_info* Info = &Window->ScrollInfos[--Window->ScrollbarIndex];
    
    v4 ScrollBackgroundColor = UIStyle.ScrollBackgroundColor;
    v4 ScrollColor = UIStyle.ScrollColor;
    
    f32 MinThumbSize = 20;
    
    if(Info->AxisFlags & UI_YAXIS) {
        
        f32 HeightToCut = UIGetHeightFromRatio(&Info->WorkingRect, 1.0f);
        UICutBottom(&Info->ContentRect, HeightToCut);
        f32 ContentHeight = GetHeight(Info->ContentRect);
        f32 BodyHeight = GetHeight(Info->ScrollbarRectY);
        f32 MaxScroll = ContentHeight - BodyHeight;
        
        rectangle2 Thumb = Info->ScrollbarRectY;
        f32 ThumbHeight = BodyHeight * (BodyHeight / ContentHeight);
        ThumbHeight = Clamp(MinThumbSize, ThumbHeight, BodyHeight);
        Thumb.Max.y -= ((Info->LastScrollOffset.y) * (BodyHeight - ThumbHeight) / MaxScroll);
        Thumb.Min.y = Thumb.Max.y - ThumbHeight;
        
        if(UIButtonInternal(Window, 3829103, Thumb, UI_SUBMIT_ON_HOLD)) {
            if(MaxScroll > 0 && BodyHeight > 0) {
                Info->LastScrollOffset.y -= (Window->State->dMouseP.y * (ContentHeight / BodyHeight));
            }
        }
        if(ContentHeight > BodyHeight) {
            Info->LastScrollOffset.y = Clamp(0, Info->LastScrollOffset.y, MaxScroll);
        }
        
        UISetClipRect(Window, Info->ScrollbarRectY);
        UIPushRect(Window, Info->ScrollbarRectY, ScrollBackgroundColor);
        UIPushRect(Window, AddRadiusTo(Thumb, -V2(UIStyle.ScrollBarThumbOffset)), ScrollColor);
    }
    
    if(Info->AxisFlags & UI_XAXIS) {
        
        f32 WidthToCut = UIGetWidthFromRatio(&Info->WorkingRect, 1.0);
        UICutRight(&Info->ContentRect, WidthToCut);
        f32 ContentWidth = GetWidth(Info->ContentRect);
        f32 BodyWidth = GetWidth(Info->ScrollbarRectX);
        f32 MaxScroll = ContentWidth - BodyWidth;
        
        rectangle2 Thumb = Info->ScrollbarRectX;
        f32 ThumbWidth = BodyWidth * (BodyWidth / ContentWidth);
        ThumbWidth = Clamp(MinThumbSize, ThumbWidth, BodyWidth);
        Thumb.Min.x += ((Info->LastScrollOffset.x) * (BodyWidth - ThumbWidth) / MaxScroll);
        Thumb.Max.x = Thumb.Min.x + ThumbWidth;
        if(UIButtonInternal(Window, 3829321103, Thumb, UI_SUBMIT_ON_HOLD)) {
            if(MaxScroll > 0 && BodyWidth > 0) {
                Info->LastScrollOffset.x += (Window->State->dMouseP.x * (ContentWidth / BodyWidth));
            }
        }
        
        if(ContentWidth > BodyWidth) {
            Info->LastScrollOffset.x = Clamp(0, Info->LastScrollOffset.x, MaxScroll);
        }
        
        UISetClipRect(Window, Info->ScrollbarRectX);
        UIPushRect(Window, Info->ScrollbarRectX, ScrollBackgroundColor);
        UIPushRect(Window, AddRadiusTo(Thumb, -V2(UIStyle.ScrollBarThumbOffset)), ScrollColor);
    }
    
    UISetClipRect(Window, Info->CornerRect);
    UIPushRect(Window, Info->CornerRect,  UIStyle.ScrollCornerColor);
    
    UISetClipRect(Window, Info->PreviousClipRect);
    
    Window->IsInScrollbarMode = false;
    Window->CurrentScrollbarClipRect = {};
}

static b32
UISliderX(ui_window* Window, f32* Value, f32 Min, f32 Max, rectangle2 Rect, char* Text = 0) {
    
    b32 Result = false;
    if(!Text) {
        char* TempText = FormatString("%.2f", *Value);
        Text = UIPushString(Window, TempText);
    }
    
    u32 ID = U32FromPointer(Value);
    UIPushID(Window, ID);
    UIDefaultButton(Window, 333213, Text, Rect, UI_SUBMIT_ON_HOLD);
    
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
    
    v4 ColorToUse = SetElementColor(Window, Interaction, UIStyle.SliderColor);
    rectangle2 ShadowRect = Offset(SliderRect, V2(-(t - 0.5)*8, 0));
    
    UIPushRect(Window, ShadowRect, ColorToUse - V4(0.2, 0.2, 0.2, 0.0));
    UIPushRect(Window, SliderRect, ColorToUse);
    UIPopID(Window);
    return Result; 
}

static b32
UISliderXInt(ui_window* Window, s32* Value, s32 Min, s32 Max, rectangle2 Rect, char* Text = 0) {
    
    b32 Result = false;
    if(!Text) {
        char* TempText = FormatString("%d", *Value);
        Text = UIPushString(Window, TempText);
    }
    
    u32 ID = U32FromPointer(Value);
    UIPushID(Window, ID);
    UIDefaultButton(Window, 3313, Text, Rect, UI_SUBMIT_ON_HOLD);
    
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
    
    
    v4 ColorToUse = SetElementColor(Window, Interaction,  UIStyle.SliderColor);
    rectangle2 ShadowRect = Offset(SliderRect, V2(-(t - 0.5)*8, 0));
    
    UIPushRect(Window, ShadowRect, ColorToUse - V4(0.2, 0.2, 0.2, 0.0));
    UIPushRect(Window, SliderRect, ColorToUse);
    UIPopID(Window);
    return Result; 
}

static b32
UISliderY(ui_window* Window, f32* Value, f32 Min, f32 Max, rectangle2 Rect, char* Text = 0) {
    
    b32 Result = false;
    if(!Text) {
        char* TempText = FormatString("%.2f", *Value);
        Text = UIPushString(Window, TempText);
    }
    
    u32 ID = U32FromPointer(Value);
    UIPushID(Window, ID);
    UIDefaultButton(Window, 1337, Text, Rect, UI_SUBMIT_ON_HOLD);
    
    ui_element El = Window->LastElement;
    ui_interaction Interaction = Window->LastInteraction;
    
    if(Interaction.State == UI_INTERACTION_STATE_SUBMIT) {
        v2 P = GetBarycentric(El.Rect, Window->State->MouseP);
        *Value = Lerp(Min, Clamp01(P.y), Max);
        Result = true;
        TestPress = true;
    }
    
    v2 RectCenter = GetCenter(El.Rect);
    v2 SliderSize = V2(GetWidth(El.Rect), 8);
    
    f32 Padding = SliderSize.y*0.5;
    f32 t = LinearConversion(Min, Max, 0.0f, 1.0f, *Value);
    f32 CurrentLerp = Lerp(El.Rect.Min.y + Padding, t, El.Rect.Max.y - Padding);
    rectangle2 SliderRect = RectCenterDim(V2(RectCenter.x, CurrentLerp), SliderSize);
    
    v4 ColorToUse = SetElementColor(Window, Interaction,  UIStyle.SliderColor);
    rectangle2 ShadowRect = Offset(SliderRect, V2(0, -(t - 0.5)*8));
    
    UIPushRect(Window, ShadowRect, ColorToUse - V4(0.2, 0.2, 0.2, 0.0));
    UIPushRect(Window, SliderRect, ColorToUse);
    UIPopID(Window);
    return Result; 
}


static b32
UISliderYInt(ui_window* Window, s32* Value, s32 Min, s32 Max, rectangle2 Rect, char* Text = 0) {
    
    b32 Result = false;
    if(!Text) {
        char* TempText = FormatString("%d", *Value);
        Text = UIPushString(Window, TempText);
    }
    
    u32 ID = U32FromPointer(Value);
    UIPushID(Window, ID);
    UIDefaultButton(Window, 5213, Text, Rect, UI_SUBMIT_ON_HOLD);
    
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
    
    v4 ColorToUse = SetElementColor(Window, Interaction, UIStyle.SliderColor);
    rectangle2 ShadowRect = Offset(SliderRect, V2(0, -(t - 0.5)*8));
    
    UIPushRect(Window, ShadowRect, ColorToUse - V4(0.2, 0.2, 0.2, 0.0));
    UIPushRect(Window, SliderRect, ColorToUse);
    UIPopID(Window);
    return Result; 
} 

static b32
UISliderV2(ui_window* Window, v2* Value, v2 Min, v2 Max, rectangle2 Rect, char* Text = 0) {
    
    b32 Result = false;
    if(!Text) {
        char* TempText = FormatString("%.2f, %.2f", Value->x, Value->y);
        Text = UIPushString(Window, TempText);
    }
    
    u32 ID = U32FromPointer(Value);
    UIPushID(Window, ID);
    UIDefaultButton(Window, 2213, Text, Rect, UI_SUBMIT_ON_HOLD);
    
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
    
    v4 ColorToUse = SetElementColor(Window, Interaction, UIStyle.SliderColor);
    rectangle2 ShadowRect = Offset(SliderRect, V2(-(tX - 0.5)*8, -(tY - 0.5)*8));
    
    UIPushRect(Window, ShadowRect, ColorToUse - V4(0.2, 0.2, 0.2, 0.0));
    UIPushRect(Window, SliderRect, ColorToUse);
    UIPopID(Window);
    return Result; 
}

static b32
UISliderV3(ui_window* Window, v3* Value, v3 Min, v3 Max, rectangle2 Rect) {
    b32 Result = false;
    
    f32 SliderRatio = UIStyle.V3SliderRatio;
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

static s32
UIList(ui_window* Window, char* Label, rectangle2 Rect) {
    s32 Result = 0;
    f32 ButtonCut = UIGetWidthFromRatio(&Rect, 0.15f);
    
    if(UIButton(Window, "<", UICutLeft(&Rect, ButtonCut))) {
        Result = -1;
    }
    if(UIButton(Window, ">", UICutRight(&Rect, ButtonCut))) {
        Result = 1;
    }
    
    UIButton(Window, Label, Rect, UI_SUBMIT_NONE);
    return Result;
}

static b32
UIFloatLabel(ui_window* Window, char* Label, f32 Float, rectangle2 Rect) {
    b32 Result = false;
    f32 PreviewCut = UIGetWidthFromRatio(&Rect, 0.33f);
    Result |= UIButton(Window, UIPushFormatString(Window, "%0.3f", Float), UICutRight(&Rect, PreviewCut));
    Result |= UIButton(Window, Label, Rect);
    return Result;
}

static b32
UIIntLabel(ui_window* Window, char* Label, s32 Int, rectangle2 Rect) {
    b32 Result = false;
    f32 PreviewCut = UIGetWidthFromRatio(&Rect, 0.33f);
    Result |= UIButton(Window, UIPushFormatString(Window, "%d", Int), UICutRight(&Rect, PreviewCut));
    Result |= UIButton(Window, Label, Rect);
    return Result;
}

static b32
UIV2Label(ui_window* Window, char* Label, v2 V2, rectangle2 Rect) {
    b32 Result = false;
    f32 PreviewCut = UIGetWidthFromRatio(&Rect, 0.33f);
    Result |= UIButton(Window, UIPushFormatString(Window, "%0.2f, %0.2f", V2.x, V2.y), UICutRight(&Rect, PreviewCut));
    Result |= UIButton(Window, Label, Rect);
    return Result;
}

static b32
UIV2IntLabel(ui_window* Window, char* Label, s32 IntA, s32 IntB, rectangle2 Rect) {
    b32 Result = false;
    f32 PreviewCut = UIGetWidthFromRatio(&Rect, 0.33f);
    Result |= UIButton(Window, UIPushFormatString(Window, "%d, %d", IntA, IntB), UICutRight(&Rect, PreviewCut));
    Result |= UIButton(Window, Label, Rect);
    return Result;
}

static b32
UIColorLabel(ui_window* Window, char* Label, v4* Color, rectangle2 Rect) {
    b32 Result = UIButton(Window, Label, UICutLeftRatio(&Rect, 0.75));
    
    v4 PrevColor = UIStyle.ButtonColor;
    UIStyle.ButtonColor = *Color;
    Result |= UIButton(Window, 0, Rect, UI_SUBMIT_NONE);
    UIStyle.ButtonColor = PrevColor;
    return Result;
}

static b32
UITextureLabel(ui_window* Window, char* Label, u32 TextureID, rectangle2 Rect) {
    b32 Result = UIButton(Window, Label, UICutLeftRatio(&Rect, 0.75));
    v4 PrevColor = UIStyle.ButtonColor;
    UIStyle.ButtonColor = V4(1, 1, 1, 1);
    UISetTextureID(Window, TextureID);
    Result |= UIButton(Window, 0, Rect, UI_SUBMIT_NONE);
    UISetTextureID(Window, 0);
    UIStyle.ButtonColor = PrevColor;
    return Result;
}


static ui_window*
UIBeginStandardWindow(ui_state* State, char* Title, rectangle2 Rect, u32 Flags = UI_WINDOW_DEFAULT) {
    
    rectangle2 WorkingWindowRect = AddRadiusTo(Rect, -V2(UIStyle.WindowBorderSize));
    
    ui_id ID = 0;
    ID = UIGenerateID(&ID, Title, UIStringLength(Title));
    ui_window* Result = UIBeginWindow(State, WorkingWindowRect, ID, Flags);
    
    //NOTE(Jesse):Because the ui_state controls the window after the first call, we have to grab the rect and add the bordersize again
    
    
    if(Flags & UI_WINDOW_MOVE) {
        rectangle2 HeaderRect = UIGetTop(&Result->BodyRect, UIStyle.HeaderHeight);
        if(UIButtonInternal(Result, Result->ID + 832910, AddRadiusTo(HeaderRect, V2(2, 2)), UI_SUBMIT_ON_HOLD)) {
            Result->BodyRect = Offset(Result->BodyRect, State->dMouseP);
        }
    }
    
    if(Flags & UI_WINDOW_SIZE) {
        rectangle2 SizeRect = UIGetBottom(&Result->BodyRect, UIStyle.ResizeSize);
        SizeRect = UICutRight(&SizeRect, UIStyle.ResizeSize);
        if(UIButtonInternal(Result, Result->ID + 438290432, AddRadiusTo(SizeRect, V2(2, 2)), UI_SUBMIT_ON_HOLD)) {
            Result->BodyRect.Max.x += State->dMouseP.x;
            Result->BodyRect.Min.y += State->dMouseP.y;
            if(GetWidth(Result->BodyRect) < Result->MinWidth) {
                Result->BodyRect.Max.x = Result->BodyRect.Min.x + Result->MinWidth;
            }
            if(GetHeight(Result->BodyRect) < Result->MinHeight) {
                Result->BodyRect.Min.y = Result->BodyRect.Max.y - Result->MinHeight;
            }
        }
    }
    
    Result->WorkingRect = Result->BodyRect;
    Result->BodyWidth = GetWidth(Result->BodyRect);
    Result->BodyHeight = GetHeight(Result->BodyRect);
    
    rectangle2 WindowDrawRect = AddRadiusTo(Result->BodyRect, V2(UIStyle.WindowBorderSize));
    UISetClipRect(Result, WindowDrawRect);
    UIPushRect(Result, WindowDrawRect, UIStyle.WindowBorderColor);
    //
    if(Flags & UI_WINDOW_SIZE) {
        rectangle2 SizeRect = UIGetBottom(&WindowDrawRect, UIStyle.ResizeSize);
        SizeRect = UICutRight(&SizeRect, UIStyle.ResizeSize);
        UIPushRect(Result, SizeRect, UIStyle.ResizeColor);
        UIPushRect(Result, AddRadiusTo(SizeRect, -V2(2, 2)), V4(0, 0, 0, 0.5));
    }
    //
    UIPushRect(Result, Result->BodyRect, UIStyle.WindowColor);
    
    rectangle2 HeaderRect = UICutTop(&Result->WorkingRect, UIStyle.HeaderHeight);
    UIPushRect(Result, HeaderRect, UIStyle.WindowBorderColor);
    if(Title) {
        v2 TextP = UILayoutTextAlignment(Result, HeaderRect, Title);
        UIPushText(Result, Title, TextP, SetTextColor(UIStyle.WindowBorderColor));
    }
    return Result;
}