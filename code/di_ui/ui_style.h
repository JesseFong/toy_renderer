/* date = June 1st 2021 7:56 pm */

#ifndef UI_STYLE_H
#define UI_STYLE_H

struct ui_window_style {
    v2s BorderSize;
    u32 FontSize;
    
    v4 BackgroundColor;
    v4 BorderColor;
};

struct ui_button_style {
    v2u BorderSize;
    f32 FontSize;
    v4 Color;
    v4 BorderColor;
    v4 LightTextColor;
    v4 DarkTextColor;
};

struct ui_template_style {
    v4 SliderColor;
    f32 V3SliderRatio;
    v4 ScrollBackgroundColor;
    v4 ScrollbarColor;
};

global_variable ui_window_style UIDefaultWindowStyle = {
    {4, 4},                       //Window Border Size
    {24},                         //Font Size
    
    {0.13f, 0.13f, 0.15f, 1.0f},  //Background Color
    {0.1f, 0.1f, 0.1f, 1.0f},     //Border Color
};

global_variable ui_button_style UIDefaultButtonStyle = {
    {1, 1},
    {24},
    {0.18f, 0.19f, 0.21f, 1.0f},  //Color
    {0.13f, 0.36f, 0.44f, 1.0f},  //BorderColor
    {0.8f, 0.8f, 0.8f, 1.0f},  //Light Text Color
    {0.2f, 0.2f, 0.2f, 1.0f},  //Dark Text Color
};

global_variable ui_template_style UIDefaultTemplateStyle = {
    {0.33f, 0.56f, 0.64f, 1.0f},  //Slider Color
    {0.3f},                       //SliderRatio
    {0.13f, 0.36f, 0.44f, 1.0f},  //Scroll Background Color
    {0.03f, 0.26f, 0.54f, 1.0f},  //Scrollbar Color
};


#endif //UI_STYLE_H
