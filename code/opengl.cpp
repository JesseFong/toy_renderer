
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
    //char* Out = FormatString("%d: %s of %s severity, raised from %s: %s\n",
    //ID, _Type, _Severity, _Source, Message);
    //DebugOut(Out);
}

global_variable f32 GlobalScreenQuadVertices[] = {
    // positions        // texture Coords
    -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
    -1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
    1.0f,  -1.0f, 0.0f, 1.0f, 0.0f,
    1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
};

global_variable f32 SkyboxVertices[] = {
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

struct open_gl {
    u32 RenderWidth;
    u32 RenderHeight; 
    
    GLuint DrawIDBuffer;
    GLuint TextureBuffer;
    
    model_geometry_info ModelGeometryInfo;
    GLuint FullscreenVAO;
    GLuint FullscreenVBO;
    
#define MAX_REGISTERED_MESH_COUNT 1024
    registered_mesh RegisteredMeshArray[MAX_REGISTERED_MESH_COUNT];
    u32 RegisteredMeshCount;
    
#define MAX_TEXTURE_COUNT 1024
    u64 TextureShaderHandleArray[MAX_TEXTURE_COUNT];
    u32 RegisteredTextureCount;
    
#define MAX_DRAW_ELEMENTS_INDIRECT_COMMANDS 65536
    GLuint IndirectCommandBuffer;
    draw_element_indirect_command IndirectCommands[MAX_DRAW_ELEMENTS_INDIRECT_COMMANDS];
    u32 IndirectCommandsCount;
    
};

open_gl* OpenGL;

//
//
//
//

static draw_element_indirect_command
GenerateIndirectDrawCommand(u32 MeshID, u32 InstanceCount) {
    
    registered_mesh* Mesh = &OpenGL->RegisteredMeshArray[MeshID];
    
    draw_element_indirect_command IndirectCommand;
    IndirectCommand.Count = Mesh->IndexCount;
    IndirectCommand.InstanceCount = 1;
    IndirectCommand.FirstIndex = Mesh->BaseIndex;
    IndirectCommand.BaseVertex = Mesh->BaseVertex;
    IndirectCommand.BaseInstance = OpenGL->IndirectCommandsCount;
    
    return IndirectCommand;
}

static void
FreeShader(GLuint* Handle) {
    glDeleteProgram(*Handle);
    *Handle = 0;
}

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
        { Result = GL_RGB; }break;
        
        case(GL_RGBA8):
        case(GL_SRGB8_ALPHA8):
        case(GL_RGBA16F):
        { Result = GL_RGBA; }break;
        
        case(GL_DEPTH_COMPONENT):
        case(GL_DEPTH_COMPONENT16):
        case(GL_DEPTH_COMPONENT24):
        case(GL_DEPTH_COMPONENT32):
        { Result = GL_DEPTH_COMPONENT; }break;
        
        case(GL_DEPTH24_STENCIL8):
        { Result = GL_DEPTH_STENCIL; }break;
        
        default: {Assert(!"Time To Add This Format To The List")}break;
    }
    return Result;
}

static GLuint
CreateTexture(u32 Width, u32 Height, GLenum InternalFormat, u8* Data) {
    GLenum Format = InternalFormatToFormat(InternalFormat);
    
    GLuint ID;
    glCreateTextures(GL_TEXTURE_2D, 1, &ID);
    
    GLenum Filter = GL_LINEAR;
    GLenum WrapFilter = GL_REPEAT;
    
    glTextureParameteri(ID, GL_TEXTURE_WRAP_S, WrapFilter);
    glTextureParameteri(ID, GL_TEXTURE_WRAP_T, WrapFilter);
    glTextureParameteri(ID, GL_TEXTURE_MIN_FILTER, Filter);
    glTextureParameteri(ID, GL_TEXTURE_MAG_FILTER, Filter);
    glGenerateTextureMipmap(ID);
    
    glTextureStorage2D(ID, 1, InternalFormat, Width, Height);
    if(Data) {
        glTextureSubImage2D(ID, 0, 0, 0, Width, Height, Format, GL_UNSIGNED_BYTE, Data);
    }
    
    return ID;
}

static void
AddFramebufferTexture(framebuffer* Framebuffer, GLenum InternalFormat) {
    GLuint Texture = CreateTexture(Framebuffer->Width, Framebuffer->Height, InternalFormat, 0);
    
    u32 ColorIndex = Framebuffer->ColorAttachmentCount;
    Framebuffer->ColorTarget[ColorIndex] = Texture;
    glNamedFramebufferTexture(Framebuffer->ID, GL_COLOR_ATTACHMENT0 + ColorIndex, Framebuffer->ColorTarget[ColorIndex], 0);
    Framebuffer->ColorAttachmentCount++;
}

static void
AddFramebufferDepthTexture(framebuffer* Framebuffer, GLenum InternalFormat) {
    GLuint Texture = CreateTexture(Framebuffer->Width, Framebuffer->Height, InternalFormat, 0);
    Framebuffer->DepthTarget = Texture;
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

static u64
RegisterBindlessTexture(GLuint ID) {
    u64 Result = glGetTextureHandleARB(ID);
    glMakeTextureHandleResidentARB(Result);
    return Result;
}

static u32
RegisterTexture(gltf_texture GLTFTexture) {
    u32 Result = 0;
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
    glTextureParameteri(ID, GL_TEXTURE_MIN_FILTER, Filter);
    glTextureParameteri(ID, GL_TEXTURE_MAG_FILTER, Filter);
    glGenerateTextureMipmap(ID);
    
    glTextureStorage2D(ID, 1, InternalFormat, Width, Height);
    if(GLTFTexture.Data) {
        glTextureSubImage2D(ID, 0, 0, 0, Width, Height, Format, GL_UNSIGNED_BYTE, GLTFTexture.Data);
    }
    
    Assert(OpenGL->RegisteredTextureCount < MAX_TEXTURE_COUNT);
    OpenGL->TextureShaderHandleArray[OpenGL->RegisteredTextureCount] = RegisterBindlessTexture(ID);
    return OpenGL->RegisteredTextureCount++;
}

static u32
RegisterCubemap(u8** ImageArray, s32 Width, s32 Height, u32 InternalFormat) {
    u32 Result = {};
    
    GLuint ID;
    glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &ID);
    
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
    
    Result = (u32)ID;
    return(Result);
}

static u32
RegisterMesh(gltf_mesh* GLTFMesh) {
    model_geometry_info* Info = &OpenGL->ModelGeometryInfo;
    
    registered_mesh Mesh = {};
    Mesh.VertexCount = GLTFMesh->VertexCount;
    Mesh.IndexCount = GLTFMesh->IndexCount;
    Mesh.BaseVertex = Info->CurrentVertexCount;
    Mesh.BaseIndex = Info->CurrentIndexCount;
    
    glBindVertexArray(Info->VAO);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Info->IndexBuffer);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, Info->CurrentIndexCount*sizeof(u32), Mesh.IndexCount*sizeof(u32), (void*)GLTFMesh->Indices);
    
    glBindBuffer(GL_ARRAY_BUFFER, Info->PBuffer);
    glBufferSubData(GL_ARRAY_BUFFER, Info->CurrentVertexCount*sizeof(v3), Mesh.VertexCount*sizeof(v3), (void*)GLTFMesh->P);
    
    glBindBuffer(GL_ARRAY_BUFFER, Info->UVBuffer);
    glBufferSubData(GL_ARRAY_BUFFER, Info->CurrentVertexCount*sizeof(v2), Mesh.VertexCount*sizeof(v2), (void*)GLTFMesh->UV);
    
    glBindBuffer(GL_ARRAY_BUFFER, Info->NBuffer);
    glBufferSubData(GL_ARRAY_BUFFER, Info->CurrentVertexCount*sizeof(v3), Mesh.VertexCount*sizeof(v3), (void*)GLTFMesh->N);
    
    if(GLTFMesh->Tangents) {
        glBindBuffer(GL_ARRAY_BUFFER, Info->TangentsBuffer);
        glBufferSubData(GL_ARRAY_BUFFER, Info->CurrentVertexCount*sizeof(v4), Mesh.VertexCount*sizeof(v4), (void*)GLTFMesh->Tangents);
    }
    glBindVertexArray(0);
    
    Info->CurrentVertexCount += Mesh.VertexCount;
    Info->CurrentIndexCount += Mesh.IndexCount;
    
    OpenGL->RegisteredMeshArray[OpenGL->RegisteredMeshCount] = Mesh;
    Assert(OpenGL->RegisteredMeshCount < ArrayCount(OpenGL->RegisteredMeshArray));
    return OpenGL->RegisteredMeshCount++;
}

static u32
RegisterGeneratedMesh(generated_mesh* GeneratedMesh) {
    gltf_mesh GLTFMesh = {};
    GLTFMesh.P = GeneratedMesh->P;
    GLTFMesh.UV = GeneratedMesh->UV;
    GLTFMesh.N = GeneratedMesh->N;
    GLTFMesh.VertexCount = GeneratedMesh->VertexCount;
    GLTFMesh.Indices = GeneratedMesh->Indices;
    GLTFMesh.IndexCount = GeneratedMesh->IndexCount;
    GLTFMesh.TriangleCount = GeneratedMesh->IndexCount / 3;
    
    return RegisterMesh(&GLTFMesh);
}

static void
InitializeStandardGeometry(u32 MaxVertexCount) {
    
    u32 MaxIndexCount = MaxVertexCount;
    
    model_geometry_info* Info = &OpenGL->ModelGeometryInfo;
    
    Info->MaxVertexCount = MaxVertexCount;
    Info->MaxIndexCount = MaxIndexCount;
    
    Info->CurrentIndexCount = 0;
    Info->CurrentVertexCount = 0;
    
    glCreateVertexArrays(1, &Info->VAO);
    glBindVertexArray(Info->VAO);
    
    glCreateBuffers(1, &Info->IndexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Info->IndexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, Info->MaxIndexCount*sizeof(u32), 0, GL_STREAM_DRAW);
    
    glCreateBuffers(1, &Info->PBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, Info->PBuffer);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glBufferData(GL_ARRAY_BUFFER, Info->MaxVertexCount*sizeof(v3), 0, GL_STREAM_DRAW);
    
    glCreateBuffers(1, &Info->UVBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, Info->UVBuffer);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glBufferData(GL_ARRAY_BUFFER, Info->MaxVertexCount*sizeof(v2), 0, GL_STREAM_DRAW);
    
    glCreateBuffers(1, &Info->NBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, Info->NBuffer);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glBufferData(GL_ARRAY_BUFFER, Info->MaxVertexCount*sizeof(v3), 0, GL_STREAM_DRAW);
    
    glCreateBuffers(1, &Info->TangentsBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, Info->TangentsBuffer);
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glBufferData(GL_ARRAY_BUFFER, Info->MaxVertexCount*sizeof(v4), 0, GL_STREAM_DRAW);
    
    glBindBuffer(GL_ARRAY_BUFFER, OpenGL->DrawIDBuffer);
    glEnableVertexAttribArray(4);
    glVertexAttribIPointer(4, 1, GL_UNSIGNED_INT, 0, 0);
    glVertexAttribDivisor(4, 1);
    
    glBindVertexArray(0);
}

static void
InitializeFullScreenGeometry() {
    glCreateVertexArrays(1, &OpenGL->FullscreenVAO);
    glBindVertexArray(OpenGL->FullscreenVAO);
    
    glCreateBuffers(1, &OpenGL->FullscreenVBO);
    glBindBuffer(GL_ARRAY_BUFFER, OpenGL->FullscreenVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GlobalScreenQuadVertices), &GlobalScreenQuadVertices, GL_STATIC_DRAW);
    
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(f32), 0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(f32), (void*)(sizeof(f32)*3));
    
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

static void
InitializeOpenGL() {
    
    v2u RenderSize = PlatformGetWindowDimension();
    OpenGL->RenderWidth = RenderSize.x;
    OpenGL->RenderHeight = RenderSize.y;
    
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallbackARB(OpenGLDebugMessageCallback, NULL);
    
#define MAX_DRAWS_PER_BATCH 2048
    u32 DrawIDs[MAX_DRAWS_PER_BATCH] = {};
    
    for(u32 I = 0;
        I < MAX_DRAWS_PER_BATCH;
        I++) {
        DrawIDs[I] = I + 0;
    }
    
    glCreateBuffers(1, &OpenGL->DrawIDBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, OpenGL->DrawIDBuffer);
    glNamedBufferData(OpenGL->DrawIDBuffer, sizeof(u32)*MAX_DRAWS_PER_BATCH, DrawIDs, GL_STATIC_DRAW);
    
    glCreateBuffers(1, &OpenGL->TextureBuffer);
    glNamedBufferData(OpenGL->TextureBuffer, sizeof(OpenGL->TextureShaderHandleArray), NULL, GL_STATIC_DRAW);
    
    glGenBuffers(1, &OpenGL->IndirectCommandBuffer);
    glBindBuffer(GL_DRAW_INDIRECT_BUFFER, OpenGL->IndirectCommandBuffer);
    
    
#define MAX_VERTEX_COUNT 65536*2*2*2*2
    
    InitializeStandardGeometry(MAX_VERTEX_COUNT);
    InitializeFullScreenGeometry();
    
    u8 WhiteBitmapData[4];
    WhiteBitmapData[0] = 255;
    WhiteBitmapData[1] = 255;
    WhiteBitmapData[2] = 255;
    WhiteBitmapData[3] = 255; 
    gltf_texture WhiteTexture = {};
    WhiteTexture.Width = 1;
    WhiteTexture.Height = 1;
    WhiteTexture.Channels = 4;
    WhiteTexture.Data = WhiteBitmapData;
    
    RegisterTexture(WhiteTexture);
    
    
    v4 ClearColor = V4(1, 1, 0, 1);
    glClearColor(ClearColor.r, ClearColor.g, ClearColor.b, ClearColor.a);
}