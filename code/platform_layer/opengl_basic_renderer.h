/* date = October 15th 2020 7:40 pm */

#ifndef OPENGL_BASIC_RENDERER_H
#define OPENGL_BASIC_RENDERER_H

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
    char* Out = FormatString("%d: %s of %s severity, raised from %s: %s\n",
                             ID, _Type, _Severity, _Source, Message);
    DebugOut(Out);
}

struct shader {
    GLuint ID;
};

struct framebuffer {
    
    GLuint FBO;
    GLuint ColorTexture;
    GLuint DepthTexture;
    u32 Width;
    u32 Height;
};

struct standard_vertex_data {
    v3 P;
    v2 UV;
    v3 N;
    v4 Color;
};

struct standard_index_data {
    u16 Index;
};

struct texture {
    u32 Width;
    u32 Height;
    u32 Channels;
    u8* Data;
    GLuint Handle;
};

enum render_element_type {
    RENDER_ELEMENT_TYPE_NONE,
    RENDER_ELEMENT_TYPE_MESH,
    RENDER_ELEMENT_TYPE_CLIP_RECT,
    RENDER_ELEMENT_TYPE_SET_PROJECTION,
    RENDER_ELEMENT_TYPE_COUNT,
};

struct render_element_mesh {
    u32 VertexCount;
    u32 IndexCount;
    u32 BaseVertex;
    u32 BaseIndex;
    u32 Texture;
    m4x4 Model;
};

struct render_element_clip_rect {
    rectangle2 ClipRect;
};

struct render_element_set_projection {
    m4x4 Proj;
};

struct render_element {
    u32 Type;
    union {
        render_element_mesh Mesh;
        render_element_clip_rect ClipRect;
        render_element_set_projection SetProjection;
    };
};

struct gl_state {
    u32 RenderWidth;
    u32 RenderHeight;
    
    GLuint VAO;
    
    standard_vertex_data* FrameVertices;
    GLuint VBO;
    u32 FrameVertexCount;
    u32 MaxFrameVertexCount;
    
    standard_index_data* FrameIndices;
    GLuint IBO;
    u32 FrameIndexCount;
    u32 MaxFrameIndexCount;
    
    texture Textures[4096];
    u32 TextureCount;
    
    shader MainShader;
    
    render_element RenderElements[4096];
    u32 RenderElementCount;
    
    rectangle2 LastClip;
};

global_variable gl_state* GLState;


static shader
CompileMainShader() {
    shader Result = {};
    Result.ID = glCreateProgram();
    
    char* HeaderSource = R"(
    #version 420 core
    )";
    
    char* VertexSource = R"(
    layout(location = 0)in vec3 Position;
    layout(location = 1)in vec2 TextureCoord;
    layout(location = 2)in vec3 Normal;
    layout(location = 3)in vec4 VertexColor;

out vec2 UV;
out vec4 Color;

uniform mat4 Projection;
uniform mat4 Model;

void main() {

vec4 ModelPosition = Model * vec4(Position, 1.0f);
gl_Position = Projection*ModelPosition;

UV = TextureCoord;
Color = VertexColor;

}

    )";
    
    
    char* FragmentSource = R"(
    in vec2 UV;
    in vec4 Color;
    
 layout(binding = 0)uniform sampler2D Texture;
layout(binding = 1)uniform sampler2D Texture1;
    layout(location = 0)out vec4 OutColor;

void main() {

OutColor = mix(Color, texture(Texture1, UV), 0.5);
}
    
    )";
    
    GLchar* VertexShaderCode[] = {
        HeaderSource,
        VertexSource,
    };
    
    GLchar* FragmentShaderCode[] = {
        HeaderSource,
        FragmentSource,
    };
    
    GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(VertexShaderID, ArrayCount(VertexShaderCode), VertexShaderCode, 0);
    glCompileShader(VertexShaderID);
    
    GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(FragmentShaderID, ArrayCount(FragmentShaderCode), FragmentShaderCode, 0);
    glCompileShader(FragmentShaderID);
    
    glAttachShader(Result.ID, VertexShaderID);
    glAttachShader(Result.ID, FragmentShaderID);
    glLinkProgram(Result.ID);
    glValidateProgram(Result.ID);
    
    GLint Linked = false;
    glGetProgramiv(Result.ID, GL_LINK_STATUS, &Linked);
    
    if(!Linked) {
        GLsizei Ignored;
        char VertexErrors[4096];
        char FragmentErrors[4096];
        char ProgramErrors[4096];
        glGetShaderInfoLog(VertexShaderID, sizeof(VertexErrors), &Ignored, VertexErrors);
        glGetShaderInfoLog(FragmentShaderID, sizeof(FragmentErrors), &Ignored, FragmentErrors);
        glGetProgramInfoLog(Result.ID, sizeof(ProgramErrors), &Ignored, ProgramErrors);
        Assert(!"Shader validation failed");
    }
    
    glDeleteShader(VertexShaderID);
    glDeleteShader(FragmentShaderID);
    
    return Result;
}

static u32
UploadTextureToOpenGL(u8* Data, s32 Width, s32 Height, s32 Format, u32 TextureIndex) {
    u32 Result = {};
    texture Texture = {};
    Texture.Data = Data;
    Texture.Width = Width;
    Texture.Height = Height;
    Texture.Channels = (Format == GL_RGBA ? 4 : 1);
    
    Assert(GLState->TextureCount < ArrayCount(GLState->Textures));
    glCreateTextures(GL_TEXTURE_2D, 1, &Texture.Handle);
    glActiveTexture(GL_TEXTURE0 + TextureIndex);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, Texture.Handle);
    
    glTextureParameteri(Texture.Handle, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(Texture.Handle, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTextureParameteri(Texture.Handle, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(Texture.Handle, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    glTextureStorage2D(Texture.Handle, 1, (Format == GL_RGBA ? GL_RGBA8 : GL_R8), Width, Height);
    glTextureSubImage2D(Texture.Handle, 0, 0, 0, Width, Height, Format, GL_UNSIGNED_BYTE, Data);
    
    glGenerateTextureMipmap(Texture.Handle);
    
    Result = GLState->TextureCount;
    GLState->Textures[GLState->TextureCount++] = Texture;
    
    return Result;
    
}

static void
SetOpenGLAttributeData(u32 Index, u32 Size, u32 Offset) {
    glEnableVertexAttribArray(Index);
    glVertexAttribPointer(Index, Size, GL_FLOAT, GL_FALSE, sizeof(standard_vertex_data), (void*)(sizeof(f32)*Offset));
}

static void
InitalizeRenderer(u32 RenderWidth, u32 RenderHeight, u32 MaxFrameVertexCount, u32 MaxFrameIndexCount) {
    
    GLState = (gl_state*)PlatformAllocateMemory(sizeof(gl_state));
    
    GLState->FrameVertices = (standard_vertex_data*)PlatformAllocateMemory(sizeof(standard_vertex_data)*MaxFrameVertexCount);
    GLState->FrameIndices = (standard_index_data*)PlatformAllocateMemory(sizeof(standard_index_data)*MaxFrameVertexCount);
    
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallbackARB(OpenGLDebugMessageCallback, 0);
    
    
    GLState->RenderWidth = RenderWidth;
    GLState->RenderHeight = RenderHeight;
    GLState->MaxFrameVertexCount = MaxFrameVertexCount;
    GLState->MaxFrameIndexCount = MaxFrameIndexCount;
    
    glGenVertexArrays(1, &GLState->VAO);
    glBindVertexArray(GLState->VAO);
    
    glGenBuffers(1, &GLState->IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GLState->IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, GLState->MaxFrameIndexCount*sizeof(standard_index_data), 0, GL_STREAM_DRAW);
    
    glGenBuffers(1, &GLState->VBO);
    glBindBuffer(GL_ARRAY_BUFFER, GLState->VBO);
    glBufferData(GL_ARRAY_BUFFER, GLState->MaxFrameVertexCount*sizeof(standard_vertex_data), 0, GL_STREAM_DRAW);
    
    SetOpenGLAttributeData(0, 3, 0);
    SetOpenGLAttributeData(1, 2, 3);
    SetOpenGLAttributeData(2, 3, 5);
    SetOpenGLAttributeData(3, 4, 8);
    
    GLState->MainShader = CompileMainShader();
    
    u8 Data[4] = {};
    Data[0] = 255;
    Data[1] = 255;
    Data[2] = 255;
    Data[3] = 255;
    
    u32 WhiteTextureIndex = UploadTextureToOpenGL(Data, 1, 1, GL_RGBA, 0);
    Assert(WhiteTextureIndex == 0);
    
    glBindVertexArray(0);
    wglSwapIntervalEXT(0);
}

static void
RendererBeginFrame() {
    GLState->FrameVertexCount = 0;
    GLState->FrameIndexCount = 0;
    GLState->RenderElementCount = 0;
    
    GLState->LastClip = RectMinMax(V2(0, 0), V2(GLState->RenderWidth, GLState->RenderHeight));
}

static void
RendererAddElement(render_element Element) {
    Assert(GLState->RenderElementCount < ArrayCount(GLState->RenderElements) - 1);
    GLState->RenderElements[GLState->RenderElementCount] = Element;
    GLState->RenderElementCount++;
}

static rectangle2
PushClipRect(v2 Min, v2 Max) {
    rectangle2 Result = {};
    render_element Element = {};
    Element.Type = RENDER_ELEMENT_TYPE_CLIP_RECT;
    Element.ClipRect.ClipRect = RectMinMax(Min, Max);
    RendererAddElement(Element);
    
    Result = GLState->LastClip;
    GLState->LastClip = Element.ClipRect.ClipRect;
    
    return Result;
}

static void
PushProjectionMatrix(m4x4 Proj) {
    render_element Element = {};
    Element.Type = RENDER_ELEMENT_TYPE_SET_PROJECTION;
    Element.SetProjection.Proj = Proj;
    RendererAddElement(Element);
}


static void
AddTransientMesh(standard_vertex_data* Data, u32 VertexCount, standard_index_data* IndexData, u32 IndexCount, u32 Texture) {
    
    RendererCopyMemory(IndexData, GLState->FrameIndices + GLState->FrameIndexCount, IndexCount*sizeof(standard_index_data));
    RendererCopyMemory(Data, GLState->FrameVertices + GLState->FrameVertexCount, VertexCount*sizeof(standard_vertex_data));
    
    render_element Element = {};
    Element.Type = RENDER_ELEMENT_TYPE_MESH;
    Element.Mesh.VertexCount = VertexCount;
    Element.Mesh.IndexCount = IndexCount;
    Element.Mesh.BaseVertex = GLState->FrameVertexCount;
    Element.Mesh.BaseIndex = GLState->FrameIndexCount;
    Element.Mesh.Texture = Texture;
    Element.Mesh.Model = Identity();
    
    RendererAddElement(Element);
    
    GLState->FrameIndexCount += IndexCount;
    GLState->FrameVertexCount += VertexCount;
}

static u32
DrawMesh(standard_vertex_data* Data, u32 VertexCount, standard_index_data* IndexData, u32 IndexCount) {
    
}

static void
DrawQuad(v3 P0, v3 P1, v3 P2, v3 P3, v4 Color = V4(1, 1, 1, 1), u32 Texture = 0) {
    
    standard_vertex_data Vertices[4] = {};
    Vertices[0].P = P0;
    Vertices[1].P = P1;
    Vertices[2].P = P2;
    Vertices[3].P = P3;
    
    Vertices[0].UV = V2(0.0f, 0.0f);
    Vertices[1].UV = V2(0.0f, 1.0f);
    Vertices[2].UV = V2(1.0f, 1.0f);
    Vertices[3].UV = V2(1.0f, 0.0f);
    
    Vertices[0].Color = Color;
    Vertices[1].Color = Color;
    Vertices[2].Color = Color;
    Vertices[3].Color = Color;
    
    standard_index_data IndexArray[6] = {};
    IndexArray[0].Index = 0;
    IndexArray[1].Index = 1;
    IndexArray[2].Index = 2;
    IndexArray[3].Index = 0;
    IndexArray[4].Index = 2;
    IndexArray[5].Index = 3;
    
    AddTransientMesh(Vertices, 4, IndexArray, 6, 0);
}

static void
DrawQuad(v3 P, v2 Radius, v4 Color = V4(1, 1, 1, 1), u32 Texture = 0) {
    v3 P0 = P + V3(-Radius.x, -Radius.y, 0);
    v3 P1 = P + V3(-Radius.x,  Radius.y, 0);
    v3 P2 = P + V3( Radius.x,  Radius.y, 0);
    v3 P3 = P + V3( Radius.x, -Radius.y, 0);
    
    DrawQuad(P0, P1, P2, P3, Color, Texture);
}


static void
RendererEndFrame() {
    
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_SCISSOR_TEST);
    glEnable(GL_BLEND);
    glBlendFuncSeparate(GL_ONE, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    
    glBindVertexArray(GLState->VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, GLState->VBO);
    glBufferData(GL_ARRAY_BUFFER, GLState->FrameVertexCount*sizeof(standard_vertex_data), GLState->FrameVertices, GL_STREAM_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GLState->IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, GLState->FrameIndexCount*sizeof(standard_index_data), GLState->FrameIndices, GL_STREAM_DRAW);
    
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClearColor(1, 0, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glViewport(0, 0, GLState->RenderWidth, GLState->RenderHeight);
    glScissor(0, 0, GLState->RenderWidth, GLState->RenderHeight);
    
    
    glUseProgram(GLState->MainShader.ID);
    
    for(u32 RenderElementIndex = 0;
        RenderElementIndex < GLState->RenderElementCount;
        RenderElementIndex++) {
        
        render_element* Element = &GLState->RenderElements[RenderElementIndex];
        
        switch(Element->Type) {
            
            case RENDER_ELEMENT_TYPE_MESH: {
                render_element_mesh* Mesh = &Element->Mesh;
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, GLState->Textures[Mesh->Texture].Handle);
                
                m4x4 Model = Identity();
                GLint ModelLocation = glGetUniformLocation(GLState->MainShader.ID, "Model");
                glUniformMatrix4fv(ModelLocation, 1, GL_TRUE, (GLfloat*)&Model);
                
                u32 BaseVertex = Mesh->BaseVertex;
                u32 BaseIndex = Mesh->BaseIndex;
                u32 DrawCount = Mesh->IndexCount;
                
                glDrawElementsBaseVertex(GL_TRIANGLES, DrawCount, GL_UNSIGNED_SHORT, (void*)(sizeof(standard_index_data)*BaseIndex), BaseVertex);
                
            } break;
            
            case RENDER_ELEMENT_TYPE_CLIP_RECT: {
                render_element_clip_rect* ClipRect = &Element->ClipRect;
                rectangle2 Rect = ClipRect->ClipRect;
                glScissor(Rect.Min.x, Rect.Min.y, Rect.Max.x, Rect.Max.y);
            } break;
            
            case RENDER_ELEMENT_TYPE_SET_PROJECTION: {
                render_element_set_projection* SetProjection = &Element->SetProjection;
                
                m4x4 Proj = Identity();
                Proj = SetProjection->Proj;
                GLint ProjLocation = glGetUniformLocation(GLState->MainShader.ID, "Projection");
                glUniformMatrix4fv(ProjLocation, 1, GL_TRUE, (GLfloat*)&Proj);
                
            } break;
            
            default: {
                Assert(!"Unknown Render Element Type");
            } break;
        }
    }
}

#endif //OPENGL_BASIC_RENDERER_H
