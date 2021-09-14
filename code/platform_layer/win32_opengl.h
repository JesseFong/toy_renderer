#ifndef WIN32_OPENGL_H
#define WIN32_OPENGL_H

#include "opengl_defines.h"
#include "win32_opengl_defines.h"


global_variable HGLRC GlobalWin32OpenGLContext;
static void*
RendererAllocateMemory(size_t MemorySize) {
    return VirtualAlloc(0, MemorySize, MEM_COMMIT|MEM_RESERVE, PAGE_READWRITE);
}

static void
RendererFreeMemory(void* Address, size_t MemorySize) {
    VirtualFree(Address, MemorySize, MEM_DECOMMIT);
}

static void*
RendererCopyMemory(void* Source, void* Dest, size_t Size) {
    u8* DestData = (u8*)Dest;
    u8* SourceData = (u8*)Source;
    while(Size--) {
        *DestData++ = *SourceData++;
    }
    return Dest;
}

static void*
Win32LoadOpenGLFunction(char *FunctionName) {
    void *Result = 0;
    Result = (void *)wglGetProcAddress(FunctionName);
    if (Result == 0 || (Result == (void *)0x1) || (Result == (void *)0x2)
        || (Result == (void *)0x3) || (Result == (void *)-1)) {
        HMODULE OpenglLibrary = LoadLibraryA("opengl32.dll");
        Result = (void *)GetProcAddress(OpenglLibrary, FunctionName);
        FreeLibrary(OpenglLibrary);
    }
    return Result;
}

global_variable PFNWGLCHOOSEPIXELFORMATARBPROC     wglChoosePixelFormatARB;
global_variable PFNWGLCREATECONTEXTATTRIBSARBPROC  wglCreateContextAttribsARB;
global_variable PFNWGLMAKECONTEXTCURRENTARBPROC    wglMakeContextCurrentARB;
global_variable PFNWGLSWAPINTERVALEXTPROC          wglSwapIntervalEXT;

static b32
Win32InitializeOpenGL(HDC WindowDC) {
    bool IsNotCoreProfile = false;
    
    u32 MajorVersion = 4;
    u32 MinorVersion = 5;
    
    PIXELFORMATDESCRIPTOR DesiredPixelFormat = {};
    DesiredPixelFormat.nSize = sizeof(PIXELFORMATDESCRIPTOR);
    DesiredPixelFormat.nVersion = 1;
    DesiredPixelFormat.dwFlags = PFD_DRAW_TO_WINDOW|PFD_SUPPORT_OPENGL|PFD_DOUBLEBUFFER;
    DesiredPixelFormat.iPixelType = PFD_TYPE_RGBA;
    DesiredPixelFormat.cColorBits = 32;
    DesiredPixelFormat.cStencilBits = 8;
    
    int SuggestedPixelFormatIndex = ChoosePixelFormat(WindowDC, &DesiredPixelFormat);
    PIXELFORMATDESCRIPTOR SuggestedPixelFormat;
    
    DescribePixelFormat(WindowDC, SuggestedPixelFormatIndex, sizeof(PIXELFORMATDESCRIPTOR), &SuggestedPixelFormat);
    SetPixelFormat(WindowDC, SuggestedPixelFormatIndex, &SuggestedPixelFormat);
    
    HGLRC Win32RenderContext = wglCreateContext(WindowDC);
    wglMakeCurrent(WindowDC, Win32RenderContext);
    
    
    wglChoosePixelFormatARB = (PFNWGLCHOOSEPIXELFORMATARBPROC)Win32LoadOpenGLFunction("wglChoosePixelFormatARB");
    wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)Win32LoadOpenGLFunction("wglCreateContextAttribsARB");
    wglMakeContextCurrentARB = (PFNWGLMAKECONTEXTCURRENTARBPROC)Win32LoadOpenGLFunction("wglMakeContextCurrentARB");
    wglSwapIntervalEXT = (PFNWGLSWAPINTERVALEXTPROC)Win32LoadOpenGLFunction("wglSwapIntervalEXT");
    
    
    wglMakeCurrent(WindowDC, 0);
    wglDeleteContext(Win32RenderContext);
    
    s32 AttributeList[] = {
        WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_DEBUG_BIT_ARB,
        WGL_CONTEXT_MAJOR_VERSION_ARB, MajorVersion,
        WGL_CONTEXT_MINOR_VERSION_ARB, MinorVersion,
        WGL_CONTEXT_PROFILE_MASK_ARB, IsNotCoreProfile? WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB: WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
        0,
    };
    GlobalWin32OpenGLContext = wglCreateContextAttribsARB(WindowDC, 0, AttributeList);
    if (GlobalWin32OpenGLContext) {
        if (wglMakeCurrent(WindowDC, GlobalWin32OpenGLContext) == TRUE) {
            LoadGLFunctions(Win32LoadOpenGLFunction);
            return true;
        }
    }
    
    OutputDebugStringA((char*)glGetString(GL_VENDOR));
    OutputDebugStringA((char*)glGetString(GL_RENDERER));
    OutputDebugStringA((char*)glGetString(GL_VERSION));
    OutputDebugStringA((char*)glGetString(GL_SHADING_LANGUAGE_VERSION));
    
    return false;
}

static void
ChangeVSyncSettings(b32 Enable) {
    wglSwapIntervalEXT(Enable);
}

#endif //WIN
