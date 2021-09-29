
global_variable GLenum OpenGLAllColorAttachments[] = {
    GL_COLOR_ATTACHMENT0,
    GL_COLOR_ATTACHMENT1,
    GL_COLOR_ATTACHMENT2,
    GL_COLOR_ATTACHMENT3,
    GL_COLOR_ATTACHMENT4,
    GL_COLOR_ATTACHMENT5,
    GL_COLOR_ATTACHMENT6,
    GL_COLOR_ATTACHMENT7,
    GL_COLOR_ATTACHMENT8,
    GL_COLOR_ATTACHMENT9,
    GL_COLOR_ATTACHMENT10,
    GL_COLOR_ATTACHMENT11,
    GL_COLOR_ATTACHMENT12,
    GL_COLOR_ATTACHMENT13,
    GL_COLOR_ATTACHMENT14,
    GL_COLOR_ATTACHMENT15,
    GL_COLOR_ATTACHMENT16,
    GL_COLOR_ATTACHMENT17,
    GL_COLOR_ATTACHMENT18,
    GL_COLOR_ATTACHMENT19,
    GL_COLOR_ATTACHMENT20,
    GL_COLOR_ATTACHMENT21,
    GL_COLOR_ATTACHMENT22,
    GL_COLOR_ATTACHMENT23,
    GL_COLOR_ATTACHMENT24,
    GL_COLOR_ATTACHMENT25,
    GL_COLOR_ATTACHMENT26,
    GL_COLOR_ATTACHMENT27,
    GL_COLOR_ATTACHMENT28,
    GL_COLOR_ATTACHMENT29,
    GL_COLOR_ATTACHMENT30,
    GL_COLOR_ATTACHMENT31,
};

#define GL_DEBUG_CALLBACK(Name) void WINAPI Name(GLenum Source,GLenum Type,GLuint ID,GLenum Severity,GLsizei Length,const GLchar* Message,const void *userParam)
GL_DEBUG_CALLBACK(OpenGLDebugMessageCallback) {
    
    char* _Source;
    char* _Type;
    char* _Severity;
    
    switch (Source) {
        case GL_DEBUG_SOURCE_API: _Source = "API"; break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM: _Source = "WINDOW SYSTEM"; break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER: _Source = "SHADER COMPILER"; break;
        case GL_DEBUG_SOURCE_THIRD_PARTY: _Source = "THIRD PARTY"; break;
        case GL_DEBUG_SOURCE_APPLICATION: _Source = "APPLICATION"; break;
        case GL_DEBUG_SOURCE_OTHER: _Source = "UNKNOWN"; break;
        
        default:_Source = "UNKNOWN";break;
    }
    
    switch (Type) {
        case GL_DEBUG_TYPE_ERROR: _Type = "ERROR"; break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: _Type = "DEPRECATED BEHAVIOR"; break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: _Type = "UDEFINED BEHAVIOR"; break;
        case GL_DEBUG_TYPE_PORTABILITY: _Type = "PORTABILITY";break;
        case GL_DEBUG_TYPE_PERFORMANCE:_Type = "PERFORMANCE";break;
        case GL_DEBUG_TYPE_OTHER:_Type = "OTHER";break;
        
        case GL_DEBUG_TYPE_MARKER:_Type = "MARKER";break;
        default:_Type = "UNKNOWN";break;
    }
    
    switch (Severity) {
        case GL_DEBUG_SEVERITY_HIGH:_Severity = "HIGH";break;
        case GL_DEBUG_SEVERITY_MEDIUM:_Severity = "MEDIUM";break;
        case GL_DEBUG_SEVERITY_LOW:_Severity = "LOW";break;
        case GL_DEBUG_SEVERITY_NOTIFICATION:_Severity = "NOTIFICATION";break;
        default:_Severity = "UNKNOWN";break;
    }
    if(Severity != GL_DEBUG_SEVERITY_NOTIFICATION) {
        char* Out = FormatString("%d: %s of %s severity, raised from %s: %s\n",
                                 ID, _Type, _Severity, _Source, Message);
        DebugOut(Out);
    }
}

global_variable f32 GLOBALScreenQuadVertices[] = {
    // positions        // texture Coords
    -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
    -1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
    1.0f,  -1.0f, 0.0f, 1.0f, 0.0f,
    1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
};

global_variable f32 GLOBALCubemapVertices[] = {
    // positions          
    -1.0f,  1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,
    1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    
    -1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,
    
    1.0f, -1.0f, -1.0f,
    1.0f, -1.0f,  1.0f,
    1.0f,  1.0f,  1.0f,
    1.0f,  1.0f,  1.0f,
    1.0f,  1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,
    
    -1.0f, -1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    1.0f,  1.0f,  1.0f,
    1.0f,  1.0f,  1.0f,
    1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,
    
    -1.0f,  1.0f, -1.0f,
    1.0f,  1.0f, -1.0f,
    1.0f,  1.0f,  1.0f,
    1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f, -1.0f,
    
    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
    1.0f, -1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
    1.0f, -1.0f,  1.0f
};

struct model_geometry_info {
    GLuint VAO;
    GLuint PBuffer;
    GLuint UVBuffer;
    GLuint NBuffer;
    GLuint TangentsBuffer;
    GLuint ColorBuffer;
    GLuint IndexBuffer;
    
    u32 MaxVertexCount;
    u32 MaxIndexCount;
    
    u32 CurrentVertexCount;
    u32 CurrentIndexCount;
};


struct framebuffer {
    u32 ID;
    u32 Width;
    u32 Height;
    u32 ColorAttachmentCount;
    GLuint ColorTarget[ArrayCount(OpenGLAllColorAttachments)];
    GLuint DepthTarget;
};

struct registered_texture {
    
};

struct registered_mesh {
    u32 VertexCount;
    u32 IndexCount;
    u32 BaseVertex;
    u32 BaseIndex;
};

struct draw_element_indirect_command {
    u32 Count;
    u32 InstanceCount;
    u32 FirstIndex;
    u32 BaseVertex;
    u32 BaseInstance;
};

//
//
//
//


static GLuint
CompileShader(u32 VertSourceCount, GLchar** VertSource, u32 FragSourceCount, GLchar** FragSource, b32* NoError) {
    
    GLuint ProgramID = glCreateProgram();
    GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    
    glShaderSource(VertexShaderID, VertSourceCount, VertSource, 0);
    glCompileShader(VertexShaderID);
    
    GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
    
    glShaderSource(FragmentShaderID, FragSourceCount, FragSource, 0);
    glCompileShader(FragmentShaderID);
    
    glAttachShader(ProgramID, VertexShaderID);
    glAttachShader(ProgramID, FragmentShaderID);
    glLinkProgram(ProgramID);
    
    glValidateProgram(ProgramID);
    GLint Linked = false;
    glGetProgramiv(ProgramID, GL_LINK_STATUS, &Linked);
    if(!Linked)
    {
        GLsizei Ignored;
        char VertexErrors[4096];
        char FragmentErrors[4096];
        char ProgramErrors[4096];
        glGetShaderInfoLog(VertexShaderID, sizeof(VertexErrors), &Ignored, VertexErrors);
        glGetShaderInfoLog(FragmentShaderID, sizeof(FragmentErrors), &Ignored, FragmentErrors);
        glGetProgramInfoLog(ProgramID, sizeof(ProgramErrors), &Ignored, ProgramErrors);
        
        *NoError = false;
    }
    
    glDeleteShader(VertexShaderID);
    glDeleteShader(FragmentShaderID);
    
    return(ProgramID);
    
}

static GLenum
InternalFormatToFormat(GLenum InternalFormat) {
    GLenum Result = {};
    
    switch(InternalFormat) {
        
        case(GL_R8):
        case(GL_R16):
        { Result = GL_RED; }break;
        
        case(GL_RG8):
        case(GL_RG16):
        { Result = GL_RG; }break;
        
        case(GL_RGB8):
        case(GL_SRGB8):
        case(GL_RGB10):
        case(GL_RGB16):
        case(GL_RGB16F):
        case(GL_RGB32F):
        { Result = GL_RGB; }break;
        
        case(GL_RGBA8):
        case(GL_SRGB8_ALPHA8):
        case(GL_RGBA16F):
        case(GL_RGBA32F):
        { Result = GL_RGBA; }break;
        
        case(GL_DEPTH_COMPONENT):
        case(GL_DEPTH_COMPONENT16):
        case(GL_DEPTH_COMPONENT24):
        case(GL_DEPTH_COMPONENT32):
        case(GL_DEPTH_COMPONENT32F):
        { Result = GL_DEPTH_COMPONENT; }break;
        
        case(GL_DEPTH24_STENCIL8):
        { Result = GL_DEPTH_STENCIL; }break;
        
        default: {Assert(!"Time To Add This Format To The List")}break;
    }
    return Result;
}

static GLuint
CreateFramebufferTexture(u32 Width, u32 Height, GLenum InternalFormat) {
    GLenum Format = InternalFormatToFormat(InternalFormat);
    GLuint ID;
    glCreateTextures(GL_TEXTURE_2D, 1, &ID);
    
    GLenum Filter = GL_NEAREST;
    GLenum WrapFilter = GL_REPEAT;
    
    glTextureParameteri(ID, GL_TEXTURE_WRAP_S, WrapFilter);
    glTextureParameteri(ID, GL_TEXTURE_WRAP_T, WrapFilter);
    glTextureParameteri(ID, GL_TEXTURE_MIN_FILTER, Filter);
    glTextureParameteri(ID, GL_TEXTURE_MAG_FILTER, Filter);
    glGenerateTextureMipmap(ID);
    
    glTextureStorage2D(ID, 1, InternalFormat, Width, Height);
    
    return ID;
}

static GLuint
CreateDepthTexture(u32 Width, u32 Height, u32 InternalFormat, GLenum Type) {
    
    GLuint ID;
    glCreateTextures(Type, 1, &ID);
    
    glTextureParameteri(ID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTextureParameteri(ID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTextureParameteri(ID, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTextureParameteri(ID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glGenerateTextureMipmap(ID);
    
    glTextureStorage2D(ID, 1, InternalFormat, Width, Height);
    return ID;
    
}

static GLuint
Create3DDepthTexture(u32 Width, u32 Height, u32 Depth, u32 InternalFormat, GLenum Type) {
    
    GLuint ID;
    glCreateTextures(Type, 1, &ID);
    
    glTextureParameteri(ID, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTextureParameteri(ID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTextureParameteri(ID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTextureParameteri(ID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTextureParameteri(ID, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    
    glTextureStorage3D(ID, 1, InternalFormat, Width, Height, Depth);
    return ID;
}


static void
AddFramebufferTexture(framebuffer* Framebuffer, GLenum InternalFormat) {
    GLuint Texture = CreateFramebufferTexture(Framebuffer->Width, Framebuffer->Height, InternalFormat);
    
    u32 ColorIndex = Framebuffer->ColorAttachmentCount;
    Framebuffer->ColorTarget[ColorIndex] = Texture;
    glNamedFramebufferTexture(Framebuffer->ID, GL_COLOR_ATTACHMENT0 + ColorIndex, Framebuffer->ColorTarget[ColorIndex], 0);
    Framebuffer->ColorAttachmentCount++;
}

static void
AddFramebufferDepthTexture(framebuffer* Framebuffer, GLuint ID) {
    Framebuffer->DepthTarget = ID;
    glNamedFramebufferTexture(Framebuffer->ID, GL_DEPTH_ATTACHMENT, Framebuffer->DepthTarget, 0);
}

static framebuffer
CreateFramebuffer(u32 Width, u32 Height) {
    framebuffer Result = {};
    Result.Width = Width;
    Result.Height = Height;
    glCreateFramebuffers(1, &Result.ID);
    return Result;
}

static void
EndFramebufferCreation(framebuffer* Framebuffer) {
    glNamedFramebufferDrawBuffers(Framebuffer->ID, Framebuffer->ColorAttachmentCount, OpenGLAllColorAttachments);
    GLenum FrameBufferStatus = glCheckNamedFramebufferStatus(Framebuffer->ID, GL_FRAMEBUFFER);
    Assert(FrameBufferStatus == GL_FRAMEBUFFER_COMPLETE);
}

static GLuint
CreateTexture(gltf_texture GLTFTexture) {
    u32 Width = GLTFTexture.Width; 
    u32 Height = GLTFTexture.Height;
    
    GLenum InternalFormat = GL_RGBA8;
    if(GLTFTexture.Channels == 3) {
        InternalFormat = GL_RGB8;
    }
    GLenum Format = InternalFormatToFormat(InternalFormat);
    
    GLuint ID;
    glCreateTextures(GL_TEXTURE_2D, 1, &ID);
    
    GLenum Filter = GL_LINEAR;
    GLenum WrapFilter = GL_REPEAT;
    
    glTextureParameteri(ID, GL_TEXTURE_WRAP_S, WrapFilter);
    glTextureParameteri(ID, GL_TEXTURE_WRAP_T, WrapFilter);
    glTextureParameteri(ID, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
    glTextureParameteri(ID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    u32 MipmapLevel = (u32)log2(Max((f32)Width, (f32)Height));
    
    glTextureStorage2D(ID, Max(MipmapLevel, 1), InternalFormat, Width, Height);
    if(GLTFTexture.Data) {
        glTextureSubImage2D(ID, 0, 0, 0, Width, Height, Format, GL_UNSIGNED_BYTE, GLTFTexture.Data);
    }
    glGenerateTextureMipmap(ID);
    
    return ID;
}

static GLuint
CreateCubemapTexture(u8** ImageArray, s32 Width, s32 Height, u32 InternalFormat) {
    
    GLuint ID;
    glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &ID);
    
    glTextureParameteri(ID, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTextureParameteri(ID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTextureParameteri(ID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTextureParameteri(ID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTextureParameteri(ID, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    
    glTextureStorage2D(ID, 1, InternalFormat, Width, Height);
    
    for(u32 FaceIndex = 0;
        FaceIndex < 6;
        FaceIndex++) {
        u8* ImageData = ImageArray[FaceIndex];
        Assert(ImageData);
        glTextureSubImage3D(ID, 0, 0, 0, FaceIndex, Width, Height, 1, InternalFormatToFormat(InternalFormat), GL_UNSIGNED_BYTE, ImageData);
    }
    
    return ID;
}

static void
InitializeOpenGL() {
    
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallbackARB(OpenGLDebugMessageCallback, NULL);
    
    v4 ClearColor = V4(1, 1, 1, 0);
    glClearColor(ClearColor.r, ClearColor.g, ClearColor.b, ClearColor.a);
}