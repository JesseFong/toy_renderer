/* date = September 14th 2021 1:19 pm */

#ifndef SHADER_H
#define SHADER_H

struct shader {
    GLuint ID;
    
    u64 LastWriteID;
    b32 NoErrors;
};


static shader
LoadShader(app_memory* Memory, char* VertPath, char* FragPath, char* GeomPath) {
    shader Result = {};
    Result.ID = glCreateProgram();
    Result.NoErrors = true;
    
    platform_file_info CommonInfo = PlatformOpenFile("gl_shaders/common.h");
    char* CommonSource = (char*)ArenaPushSizePlusNull(&Memory->TempArena, CommonInfo.FileSize);
    PlatformReadFile(&CommonInfo, CommonInfo.FileSize, (void*)CommonSource);
    PlatformCloseFile(&CommonInfo);
    Result.LastWriteID |= CommonInfo.FileTime;
    
    platform_file_info VertInfo = PlatformOpenFile(VertPath);
    char* VertSource = (char*)ArenaPushSizePlusNull(&Memory->TempArena, VertInfo.FileSize);
    PlatformReadFile(&VertInfo, VertInfo.FileSize, (void*)VertSource);
    PlatformCloseFile(&VertInfo);
    Result.LastWriteID |= VertInfo.FileTime;
    
    platform_file_info FragInfo = PlatformOpenFile(FragPath);
    char* FragSource = (char*)ArenaPushSizePlusNull(&Memory->TempArena, FragInfo.FileSize);
    PlatformReadFile(&FragInfo, FragInfo.FileSize, (void*)FragSource);
    PlatformCloseFile(&FragInfo);
    Result.LastWriteID |= FragInfo.FileTime;
    
    b32 HasGeometryShader = false;
    char* GeomSource = 0;
    if(GeomSource) {
        HasGeometryShader = true;
        platform_file_info GeomInfo = PlatformOpenFile(GeomPath);
        GeomSource = (char*)ArenaPushSizePlusNull(&Memory->TempArena, GeomInfo.FileSize);
        PlatformReadFile(&GeomInfo, GeomInfo.FileSize, (void*)GeomSource);
        PlatformCloseFile(&GeomInfo);
        Result.LastWriteID |= GeomInfo.FileTime;
    }
    
    char* VertSourceArray[2] = {
        CommonSource,
        VertSource,
    };
    
    GLuint VertShaderID = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(VertShaderID, 2, &(GLchar*)VertSourceArray[0], 0);
    glCompileShader(VertShaderID);
    glAttachShader(Result.ID, VertShaderID);
    
    char* FragSourceArray[2] = {
        CommonSource,
        FragSource,
    };
    
    GLuint FragShaderID = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(FragShaderID, 2, &(GLchar*)FragSourceArray[0], 0);
    glCompileShader(FragShaderID);
    glAttachShader(Result.ID, FragShaderID);
    
    GLuint GeomShaderID = 0;
    if(HasGeometryShader) {
        GLuint GeomShaderID = glCreateShader(GL_GEOMETRY_SHADER);
        glShaderSource(GeomShaderID, 1, &(GLchar*)GeomSource, 0);
        glCompileShader(GeomShaderID);
        glAttachShader(Result.ID, GeomShaderID);
    }
    
    glLinkProgram(Result.ID);
    glValidateProgram(Result.ID);
    
    GLint Linked = false;
    glGetProgramiv(Result.ID, GL_LINK_STATUS, &Linked);
    if(!Linked)
    {
        GLsizei Ignored;
        char VertexErrors[4096];
        char FragmentErrors[4096];
        char GeometryErrors[4096];
        char ProgramErrors[4096];
        glGetShaderInfoLog(VertShaderID, sizeof(VertexErrors), &Ignored, VertexErrors);
        glGetShaderInfoLog(FragShaderID, sizeof(FragmentErrors), &Ignored, FragmentErrors);
        
        if(HasGeometryShader) {
            glGetShaderInfoLog(GeomShaderID, sizeof(GeometryErrors), &Ignored, GeometryErrors);
        }
        
        glGetProgramInfoLog(Result.ID, sizeof(ProgramErrors), &Ignored, ProgramErrors);
        
        Result.NoErrors = false;
    }
    
    glDeleteShader(VertShaderID);
    glDeleteShader(FragShaderID);
    if(HasGeometryShader) {
        glDeleteShader(GeomShaderID);
    }
    
    return Result;
}

static b32
ShouldShaderReload(shader* Shader, char* VertPath, char* FragPath, char* GeomPath) {
    b32 Result = false;
    
    u64 LastWriteID = {};
    platform_file_info CommonInfo = PlatformOpenFile("gl_shaders/common.h");
    PlatformCloseFile(&CommonInfo);
    LastWriteID |= CommonInfo.FileTime;
    platform_file_info VertInfo = PlatformOpenFile(VertPath);
    PlatformCloseFile(&VertInfo);
    LastWriteID |= VertInfo.FileTime;
    platform_file_info FragInfo = PlatformOpenFile(FragPath);
    PlatformCloseFile(&FragInfo);
    LastWriteID |= FragInfo.FileTime;
    
    if(GeomPath) {
        platform_file_info GeomInfo = PlatformOpenFile(GeomPath);
        PlatformCloseFile(&GeomInfo);
        LastWriteID |= GeomInfo.FileTime;
    }
    
    if(Shader->LastWriteID != LastWriteID) {
        Result = true;
    }
    
    return Result;
}


static void
FreeShader(GLuint* Handle) {
    glDeleteProgram(*Handle);
    *Handle = 0;
}

static void
MaybeReloadShader(app_memory* Memory, shader* Shader, char* VertPath, char* FragPath, char* GeomPath) {
    
    if(ShouldShaderReload(Shader, VertPath, FragPath, GeomPath)) {
        shader TempShader = LoadShader(Memory, VertPath, FragPath, GeomPath);
        if(TempShader.NoErrors) {
            FreeShader(&Shader->ID);
            *Shader = TempShader;
        }
    }
}


#endif //SHADER_H
