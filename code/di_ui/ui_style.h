/* date = June 1st 2021 7:56 pm */

#ifndef UI_STYLE_H
#define UI_STYLE_H

struct ui_style {
    v2s WindowBorderSize;
    s32 HeaderHeight;
    s32 ResizeSize;
    v4 ResizeColor;
    
    v4 WindowColor;
    v4 WindowBorderColor;
    
    v2s ButtonBorderSize;
    v4 ButtonBorderColor;
    v4 ButtonColor;
    
    v4 HotDelta;
    v4 NextSubmitDelta;
    v4 SubmitDelta;
    
    v4 LightTextColor;
    v4 DarkTextColor;
    
    v2s ToggleButtonSize;
    
    v4 SliderColor;
    f32 V3SliderRatio;
    
    v2s ScrollBarThumbOffset;
    v4 ScrollBackgroundColor;
    v4 ScrollColor;
    v4 ScrollCornerColor;
    
};

global_variable ui_style UIStyle = {
    {6, 6},                       //Window Border Size
    {26},                         //Header Height
    {20},                         //Resize Size
    {0.5f, 0.13f, 0.13f, 1.0f},   //Resize Color
    {0.1f, 0.1f, 0.1f, 1.0f},     //Window Color
    {0.05f, 0.05f, 0.05f, 1.0f},  //Window Border Color
    {2, 2},                       //ButtonBorderSize
    {0.05f, 0.05f, 0.05f, 1.0f},  //Button Border Color
    {0.25f, 0.25f, 0.25f, 0.7f},  //Button Color
    {0.04f, 0.04f, 0.04f, 0.0f},  //HotDelta
    {0.07f, 0.07f, 0.07f, 0.0f},  //NextSubmitDelta
    {0.1f,  0.1f,  0.1f,  0.0f},  //SubmitDelta
    {0.65f, 0.65f, 0.65f, 0.75f}, //Light Text Color
    {0.2f, 0.2f, 0.2f, 1.0f},     //Dark Text Color
    {2, 2},                       //ToggleButtonSize
    {0.9f, 0.1f, 0.1f, 0.35f},    //Slider Color
    {0.3f},                       //Slider Ratio
    {4, 4},                       //Scrollbar Thumb Offset
    {0.05f, 0.05f, 0.05f, 1.0f},  //Scroll Background Color
    {0.2f, 0.2f, 0.2f, 1.0f},     //Scrollbar Color
    {0.25f, 0.25f, 0.25f, 1.0f},  //Scroll Corner Color
};


global_variable b32 TestPress = false;

#endif //UI_STYLE_H
