/* date = September 23rd 2021 9:51 pm */

#ifndef RENDERER_PANEL_H
#define RENDERER_PANEL_H

static void
RendererUIPanels(app_memory* Memory) {
    
    ui_state* UI = Memory->UI;
    renderer_settings* Settings = &Memory->Settings;
    scene* Scene = &Memory->Scene;
    
    rectangle2 PanelRect = RectMinMax(V2(-1.0, 0.0), V2(-0.6, 1.0));
    
    ui_window* Window = UIBeginStandardWindow(UI, "Settings", UIConvertToPixelSpace(UI, PanelRect, UI_CLIP_SPACE), 11);
    
    rectangle2* Rect = &Window->WorkingRect;
    
    if(UIButton(Window, "Show GBuffer", UICutTop(Rect, 30))) {
        Settings->DisplayGbuffer = true;
    }
    
    s32 ListResult = UIList(Window, "Light: ", UICutTop(Rect, 30));
    
    Settings->LightIndex += ListResult;
    if(Settings->LightIndex > 3) {
        Settings->LightIndex = 0;
    }
    if(Settings->LightIndex < 0) {
        Settings->LightIndex = 3;
    }
    
    point_light* Light = &Scene->PointLights[Settings->LightIndex];
    UISliderX(Window, &Light->Position.x, -20.0f, 20.0f, UICutTop(Rect, 30), "P.x");
    UISliderX(Window, &Light->Position.y, -20.0f, 20.0f, UICutTop(Rect, 30), "P.y");
    UISliderX(Window, &Light->Position.z, -20.0f, 20.0f, UICutTop(Rect, 30), "P.z");
    
    if(Settings->DisplayGbuffer) {
        
        rectangle2 SelectWindowRect = UIConvertToPixelSpace(UI, RectMinMax(V2(0, 0), V2(1, 1)), UI_SCREEN_SPACE);
        ui_window* SelectWindow = UIBeginWindow(UI, SelectWindowRect, 12, UI_WINDOW_ALWAYS_ON_BOTTOM);
        rectangle2* Rect = &SelectWindow->WorkingRect;
        
        v2 ButtonSize = UIConvertToPixelSpace(UI, V2(1.0/3.0, 1.0/3.0), UI_SCREEN_SPACE);
        u32 BufferIndex = 0;
        
        for(u32 RowIndex = 0;
            RowIndex < 3;
            RowIndex++) {
            
            rectangle2 RowRect = UICutTop(Rect, ButtonSize.y);
            
            for(u32 ColumnIndex = 0;
                ColumnIndex < 3;
                ColumnIndex++) {
                
                u32 ID = RowIndex*3 + ColumnIndex + 325617;
                UIPushID(SelectWindow, ID);
                if(UIButtonOutline(SelectWindow, 0, UICutLeft(&RowRect, ButtonSize.x))) {
                    Settings->BufferToDisplay = BufferIndex;
                    Settings->DisplayGbuffer = false;
                }
                BufferIndex++;
                UIPopID(SelectWindow);
            }
        }
    }
}


#endif //RENDERER_PANEL_H
