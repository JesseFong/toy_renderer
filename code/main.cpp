
#define APP_TYPE_WINDOW
#define WINDOW_TITLE "3D Engine"

#include "platform_layer/platform.cpp"
#include "mesh_generation.h"
#include "gltf_loader.h"
#include "camera.h"
#include "main.h"
#include "opengl.cpp"
#include "draw.cpp"

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
    
    OpenGL = ArenaPushStruct(&Memory.MainArena, open_gl);
    
    InitializeOpenGL();
    
    while(GLOBALPlatformState->Window.IsRunning) {
        UpdateTime();
        PlatformProcessMessages();
        
        input* Input = &GLOBALPlatformState->Input;
        platform_time* Time = &GLOBALPlatformState->Time; 
        
        ArenaPopTo(&Memory.TempArena, 0);
        UpdateAndRender(&Memory);
        SwapBuffers(WindowDC);
    }
    
    return 0;
}