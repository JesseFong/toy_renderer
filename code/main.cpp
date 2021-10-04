
#define APP_TYPE_WINDOW
#define WINDOW_TITLE "3D Engine"

#include "platform_layer/platform.cpp"
#include "di_ui/ui_default_includes.h"

#include "asset.h"
#include "font_loading.h"
#include "mesh_generation.h"
#include "gltf_loader.h"
#include "camera.h"
#include "main.h"
#include "shader.h"
#include "opengl.cpp"
#include "draw.cpp"

#include "gl_ui_renderer.h"
#include "renderer_panel.h"


//#include "glm/glm.hpp"

global_variable u32 GLOBALWindowWidth  = 1600;
global_variable u32 GLOBALWindowHeight = 900;

int main(int argc, char** argv) {
    
    // PLATFORM INIT
    platform_init_settings PlatformSettings = {};
    PlatformSettings.Window.Width = GLOBALWindowWidth;
    PlatformSettings.Window.Height = GLOBALWindowHeight;
    
    PlatformSettings.Window.Title = WINDOW_TITLE;
    PlatformSettings.Window.StartFullscreen = false;
    
    InitializePlatform(PlatformSettings);
    
    
    GLOBALWindowWidth = GetCurrentWindowWidth();
    GLOBALWindowHeight = GetCurrentWindowHeight();
    
    app_memory Memory = {};
    Memory.MainArena = ArenaMake();
    Memory.TempArena = ArenaMake();
    
    HDC WindowDC = GLOBALPlatformState->Internal->Window.DC;
    b32 IsInitialized = Win32InitializeOpenGL(WindowDC);
    
    InitializeOpenGL();
    InitializeOpenGLScene(&Memory, GLOBALWindowWidth, GLOBALWindowHeight);
    
    
    asset_font_atlas FontAtlas = LoadFontAtlasFromFile("C:/Windows/Fonts/arial.ttf", 64);
    GLOBALUIFontAtlas = &FontAtlas;
    Memory.UI = (ui_state*)ArenaPush(&Memory.MainArena, sizeof(ui_state));
    InitUIRenderer();
    
    while(GLOBALPlatformState->Window.IsRunning) {
        UpdateTime();
        PlatformProcessMessages();
        
        rectangle2 WindowRect = RectMinMax(V2(0, 0), V2((f32)GLOBALWindowWidth, (f32)GLOBALWindowHeight));
        input* Input = &GLOBALPlatformState->Input;
        platform_time* Time = &GLOBALPlatformState->Time;
        
        
        UIBeginFrame(Memory.UI, WindowRect, Input->Mouse.P, Input->Mouse.dP, WasPressed(KEY_MOUSE1), IsDown(KEY_MOUSE1), WentUp(KEY_MOUSE1),
                     Input->TextInputThisFrame, Input->TextInputCount, Time->TimeDeltaInSeconds, 0);
        
        
        ArenaPopTo(&Memory.TempArena, 0);
        UpdateAndRender(&Memory);
        
        RendererUIPanels(&Memory);
        
        DrawUI(Memory.UI);
        SwapBuffers(WindowDC);
    }
    
    return 0;
}