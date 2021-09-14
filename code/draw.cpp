
// TODO(Jesse):
/*
- PBR 
- ANTI ALIASING
- HDR
- BLOOM
- MOTION BLUR
- ROBUST SHADER SYSTEM
- DEBUG VIEWS
- LOD STUFF?
- MIP MAP TEXTURES
- SHADOWMAP / CASCADED? / POINT LIGHT SHADOWS
- DIRECTIONAL LIGHTS / SPOTLIGHTS
- EMISSIVE GEOMETRY
- GI SURFELS

*/

enum shader_flags {
    SHADER_FLAG_NONE = 0,
    SHADER_FLAG_USE_COLOR = 1 << 0,
    SHADER_FLAG_USE_ROUGHNESS = 1 << 1,
    SHADER_FLAG_USE_METALLIC = 1 << 2,
}; 


struct per_draw_uniforms {
    u32 AlbedoID;
    u32 NormalID;
    u32 RoughnessID;
    u32 OcclusionID;
    
    f32 RoughnessFactor;
    f32 MetallicFactor;
    u32 ShaderFlags;
    f32 Pad;
    
    v4 Color;
    m4x4 ModelProj;
};

struct directional_light {
    v3 Direction;
    float Pad;
    v3 Color;
    float Pad1;
};

struct point_light {
    
    v3 Position;
    float Pad;
    
    v3 Color;
    f32 Constant;
    
    f32 Linear;
    f32 Quadratic;
    
    f32 Pad1;
    f32 Pad2;
};

struct frame_uniforms {
    m4x4 CameraProj;
    m4x4 LightProj;
    point_light PointLight[4];
    directional_light SunLight;
    v3 CameraP;
    u32 FramebufferTextureToDisplay;
};

struct shadowmap_shader {
    u32 ID;
    u32 DrawUniformsID;
    u32 FrameUniformsID;
    
    platform_file_info VertInfo;
    platform_file_info FragInfo;
    b32 NoErrors;
};

struct resolve_shader {
    u32 ID;
    
    u32 GAlbedoID;
    u32 GNormalID;
    u32 GRoughnessID;
    u32 GPositionID;
    u32 GDepthID;
    u32 GShadowedID;
    
    u32 FrameUniformsID;
    
    platform_file_info VertInfo;
    platform_file_info FragInfo;
    b32 NoErrors;
};


struct gbuffer_shader  {
    u32 ID;
    
    u32 TextureArrayID;
    u32 FrameUniformsID;
    u32 DrawUniformsID;
    u32 ShadowmapID;
    
    u32 LightProjID;
    
#define MAX_BINDINGS 32
    GLuint Bindings[MAX_BINDINGS];
    platform_file_info VertInfo;
    platform_file_info FragInfo;
    b32 NoErrors;
};

global_variable gbuffer_shader GBufferShader;
global_variable GLuint DrawUniformBuffer;
global_variable per_draw_uniforms* DrawUniformArray;
global_variable framebuffer GBufferFramebuffer;

global_variable GLuint FrameUniformBuffer;
global_variable frame_uniforms FrameUniforms;

global_variable resolve_shader ResolveShader;
global_variable framebuffer ResolveFramebuffer;
global_variable GLuint FullScreenVAO;
global_variable GLuint FullScreenVBO;

global_variable shadowmap_shader ShadowmapShader;
global_variable framebuffer ShadowmapFramebuffer;



static void
BindAllUniforms() {
    
    
}

static m4x4_inv
GenerateDirectionLightProj(directional_light Light) {
    
    
    m4x4_inv Result = {};
    
#if 1
    m4x4_inv LightProj = OrthographicProjection(V2(-20.0f, 20.0f), V2(-20.0f, 20.0f), V2(0.1f, 30.0f));
    m4x4_inv LightCameraProj = LookAt(Light.Direction, V3(0, 0, 0), V3(0, 1, 0));
    
#else
    f32 NearClip = 0.1f;
    f32 FarClip = 20.0f;
    //
    f32 AspectRatio = (f32)OpenGL->RenderWidth / (f32)OpenGL->RenderHeight;
    m4x4_inv LightProj = PerspectiveProjection(AspectRatio, 45.0, 0.1f, 30.0f);
    
    f32 Length = 20.0f;
    m4x4_inv LightCameraProj = LookAt(Light.Direction * Length, Light.Direction, V3(0, 1, 0));
#endif
    Result.Forward = LightProj.Forward * LightCameraProj.Forward; 
    Result.Inverse = LightCameraProj.Inverse * LightProj.Inverse;
    
    
    
    return Result;
}

static m4x4_inv
SetProjection(camera* Camera) {
    f32 AspectWidthOverHeight = (f32)OpenGL->RenderWidth/(f32)OpenGL->RenderHeight;
    
    m4x4_inv ProjInv = PerspectiveProjection(AspectWidthOverHeight, Camera->FOV, Camera->NearClip, Camera->FarClip);
    m4x4_inv CameraXForm = CameraTransform(Camera->XAxis, Camera->YAxis, Camera->ZAxis, Camera->P);
    
    m4x4_inv Proj;
    Proj.Forward = ProjInv.Forward * CameraXForm.Forward;
    Proj.Inverse = CameraXForm.Inverse * ProjInv.Inverse;
    
    return Proj;
}

static camera_update_params
UpdateCamera(camera* Camera) {
    mouse_input* MouseInput = &GLOBALPlatformState->Input.Mouse;
    camera_update_params Result = {};
    
    v3 CameraddP = V3(0, 0, 0);
    if(IsDown(KEY_W)) {
        CameraddP = -Normalize(Camera->ZAxis);
    }
    if(IsDown(KEY_A)) {
        CameraddP = -Normalize(Camera->XAxis);
    }
    if(IsDown(KEY_S)) {
        CameraddP = Normalize(Camera->ZAxis);
    }
    if(IsDown(KEY_D)) {
        CameraddP = Normalize(Camera->XAxis);
    }
    
    Result.dMouseP = MouseInput->dP;
    if(Length(MouseInput->LastP) == 0 || !(IsDown(KEY_SPACE))) {
        Result.dMouseP = V2(0, 0);
    }
    Result.ddP = CameraddP;
    if(Length(Result.dMouseP) != 0) {
        int breakpoint = 0;
    }
    UpdateCamera(Camera, Result.dMouseP, Result.ddP);
    return(Result);
}

static shadowmap_shader 
LoadShadowmapShader(app_memory* Memory) {
    shadowmap_shader Shader;
    
    platform_file_info HeaderInfo = PlatformOpenFile("gl_shaders/header.h");
    char* HeaderSource = (char*)ArenaPushSizePlusNull(&Memory->TempArena, HeaderInfo.FileSize);
    PlatformReadFile(&HeaderInfo, HeaderInfo.FileSize, (void*)HeaderSource);
    PlatformCloseFile(&HeaderInfo);
    
    platform_file_info FrameUniformsInfo = PlatformOpenFile("gl_shaders/shader_frame_uniforms.h");
    char* FrameUniformsSource = (char*)ArenaPushSizePlusNull(&Memory->TempArena, FrameUniformsInfo.FileSize);
    PlatformReadFile(&FrameUniformsInfo, FrameUniformsInfo.FileSize, (void*)FrameUniformsSource);
    PlatformCloseFile(&FrameUniformsInfo);
    
    platform_file_info DrawUniformsInfo = PlatformOpenFile("gl_shaders/shader_draw_uniforms.h");
    char* DrawUniformsSource = (char*)ArenaPushSizePlusNull(&Memory->TempArena, DrawUniformsInfo.FileSize);
    PlatformReadFile(&DrawUniformsInfo, DrawUniformsInfo.FileSize, (void*)DrawUniformsSource);
    PlatformCloseFile(&DrawUniformsInfo);
    
    platform_file_info VertInfo = PlatformOpenFile("gl_shaders/shadowmap.vert");
    char* VertSource = (char*)ArenaPushSizePlusNull(&Memory->TempArena, VertInfo.FileSize);
    PlatformReadFile(&VertInfo, VertInfo.FileSize, (void*)VertSource);
    PlatformCloseFile(&VertInfo);
    
    platform_file_info FragInfo = PlatformOpenFile("gl_shaders/shadowmap.frag"); 
    char* FragSource = (char*)ArenaPushSizePlusNull(&Memory->TempArena, FragInfo.FileSize);
    PlatformReadFile(&FragInfo, FragInfo.FileSize, (void*)FragSource);
    PlatformCloseFile(&FragInfo);
    
    char* VertSourceArray[4] = {
        HeaderSource,
        FrameUniformsSource,
        DrawUniformsSource,
        VertSource,
    };
    
    char* FragSourceArray[4] = {
        HeaderSource,
        FrameUniformsSource,
        DrawUniformsSource,
        FragSource,
    };
    
    Shader.VertInfo = VertInfo;
    Shader.FragInfo = FragInfo;
    Shader.NoErrors = true;
    Shader.ID = CompileShader(ArrayCount(VertSourceArray), &(GLchar*)VertSourceArray[0], ArrayCount(FragSourceArray), &(GLchar*)FragSourceArray[0], &Shader.NoErrors); 
    Shader.FrameUniformsID = glGetUniformBlockIndex(Shader.ID, "FrameUniforms");
    Shader.DrawUniformsID = glGetProgramResourceIndex(Shader.ID, GL_SHADER_STORAGE_BLOCK, "DrawUniforms");
    
    return Shader;
}

static resolve_shader
LoadResolveShader(app_memory* Memory) {
    resolve_shader Shader;
    
    platform_file_info HeaderInfo = PlatformOpenFile("gl_shaders/header.h");
    char* HeaderSource = (char*)ArenaPushSizePlusNull(&Memory->TempArena, HeaderInfo.FileSize);
    PlatformReadFile(&HeaderInfo, HeaderInfo.FileSize, (void*)HeaderSource);
    PlatformCloseFile(&HeaderInfo);
    
    platform_file_info FrameUniformsInfo = PlatformOpenFile("gl_shaders/shader_frame_uniforms.h");
    char* FrameUniformsSource = (char*)ArenaPushSizePlusNull(&Memory->TempArena, FrameUniformsInfo.FileSize);
    PlatformReadFile(&FrameUniformsInfo, FrameUniformsInfo.FileSize, (void*)FrameUniformsSource);
    PlatformCloseFile(&FrameUniformsInfo);
    
    platform_file_info VertInfo = PlatformOpenFile("gl_shaders/resolve.vert");
    char* VertSource = (char*)ArenaPushSizePlusNull(&Memory->TempArena, VertInfo.FileSize);
    PlatformReadFile(&VertInfo, VertInfo.FileSize, (void*)VertSource);
    PlatformCloseFile(&VertInfo);
    
    platform_file_info FragInfo = PlatformOpenFile("gl_shaders/resolve.frag"); 
    char* FragSource = (char*)ArenaPushSizePlusNull(&Memory->TempArena, FragInfo.FileSize);
    PlatformReadFile(&FragInfo, FragInfo.FileSize, (void*)FragSource);
    PlatformCloseFile(&FragInfo);
    
    char* VertSourceArray[2] = {
        HeaderSource,
        VertSource,
    };
    
    char* FragSourceArray[3] = {
        HeaderSource,
        FrameUniformsSource,
        FragSource,
    };
    
    Shader.VertInfo = VertInfo;
    Shader.FragInfo = FragInfo;
    Shader.NoErrors = true;
    Shader.ID = CompileShader(ArrayCount(VertSourceArray), &(GLchar*)VertSourceArray[0], ArrayCount(FragSourceArray), &(GLchar*)FragSourceArray[0], &Shader.NoErrors); 
    Shader.GAlbedoID = GL_TEXTURE0;
    Shader.GNormalID = GL_TEXTURE1;
    Shader.GRoughnessID = GL_TEXTURE2;
    Shader.GPositionID = GL_TEXTURE3;
    Shader.GDepthID = GL_TEXTURE4;
    Shader.GShadowedID = GL_TEXTURE5;
    Shader.FrameUniformsID = glGetUniformBlockIndex(Shader.ID, "FrameUniforms");
    
    return Shader;
    
}

static gbuffer_shader
LoadGBufferShader(app_memory* Memory) {
    
    gbuffer_shader Shader = {};
    
    platform_file_info HeaderInfo = PlatformOpenFile("gl_shaders/header.h");
    char* HeaderSource = (char*)ArenaPushSizePlusNull(&Memory->TempArena, HeaderInfo.FileSize);
    PlatformReadFile(&HeaderInfo, HeaderInfo.FileSize, (void*)HeaderSource);
    PlatformCloseFile(&HeaderInfo);
    
    platform_file_info FrameUniformsInfo = PlatformOpenFile("gl_shaders/shader_frame_uniforms.h");
    char* FrameUniformsSource = (char*)ArenaPushSizePlusNull(&Memory->TempArena, FrameUniformsInfo.FileSize);
    PlatformReadFile(&FrameUniformsInfo, FrameUniformsInfo.FileSize, (void*)FrameUniformsSource);
    PlatformCloseFile(&FrameUniformsInfo);
    
    platform_file_info DrawUniformsInfo = PlatformOpenFile("gl_shaders/shader_draw_uniforms.h");
    char* DrawUniformsSource = (char*)ArenaPushSizePlusNull(&Memory->TempArena, DrawUniformsInfo.FileSize);
    PlatformReadFile(&DrawUniformsInfo, DrawUniformsInfo.FileSize, (void*)DrawUniformsSource);
    PlatformCloseFile(&DrawUniformsInfo);
    
    platform_file_info VertInfo = PlatformOpenFile("gl_shaders/shader.vert");
    char* VertSource = (char*)ArenaPushSizePlusNull(&Memory->TempArena, VertInfo.FileSize);
    PlatformReadFile(&VertInfo, VertInfo.FileSize, (void*)VertSource);
    PlatformCloseFile(&VertInfo);
    
    platform_file_info FragInfo = PlatformOpenFile("gl_shaders/shader.frag"); 
    char* FragSource = (char*)ArenaPushSizePlusNull(&Memory->TempArena, FragInfo.FileSize);
    PlatformReadFile(&FragInfo, FragInfo.FileSize, (void*)FragSource);
    PlatformCloseFile(&FragInfo);
    
    char* VertSourceArray[4] = {
        HeaderSource,
        FrameUniformsSource,
        DrawUniformsSource,
        VertSource,
    };
    
    char* FragSourceArray[4] = {
        HeaderSource,
        FrameUniformsSource,
        DrawUniformsSource,
        FragSource,
    };
    
    Shader.VertInfo = VertInfo;
    Shader.FragInfo = FragInfo;
    Shader.NoErrors = true;
    Shader.ID = CompileShader(ArrayCount(VertSourceArray), &(GLchar*)VertSourceArray[0], ArrayCount(FragSourceArray), &(GLchar*)FragSourceArray[0], &Shader.NoErrors);
    Shader.TextureArrayID = glGetUniformBlockIndex(Shader.ID, "TextureArray");
    Shader.FrameUniformsID = glGetUniformBlockIndex(Shader.ID, "FrameUniforms");
    Shader.DrawUniformsID = glGetProgramResourceIndex(Shader.ID, GL_SHADER_STORAGE_BLOCK, "DrawUniforms");
    Shader.LightProjID = glGetUniformLocation(Shader.ID, "LightProj");
    Shader.ShadowmapID = GL_TEXTURE0;
    
    return Shader;
}

static b32
ShouldShadersReload() {
    b32 Result = false;
    
    platform_file_info TestVertInfo = PlatformOpenFile("gl_shaders/shader.vert");
    platform_file_info TestFragInfo = PlatformOpenFile("gl_shaders/shader.frag");
    
    u64 VertFileTime = TestVertInfo.FileTime;
    u64 FragFileTime = TestFragInfo.FileTime;
    
    if((VertFileTime != GBufferShader.VertInfo.FileTime) || (FragFileTime != GBufferShader.FragInfo.FileTime)) {
        Result = true;
    }
    PlatformCloseFile(&TestVertInfo);
    PlatformCloseFile(&TestFragInfo);
    
    TestVertInfo = PlatformOpenFile("gl_shaders/resolve.vert");
    TestFragInfo = PlatformOpenFile("gl_shaders/resolve.frag");
    
    VertFileTime = TestVertInfo.FileTime;
    FragFileTime = TestFragInfo.FileTime;
    
    if((VertFileTime != ResolveShader.VertInfo.FileTime) || (FragFileTime != ResolveShader.FragInfo.FileTime)) {
        Result = true;
    }
    PlatformCloseFile(&TestVertInfo);
    PlatformCloseFile(&TestFragInfo);
    
    
    TestVertInfo = PlatformOpenFile("gl_shaders/shadowmap.vert");
    TestFragInfo = PlatformOpenFile("gl_shaders/shadowmap.frag");
    
    VertFileTime = TestVertInfo.FileTime;
    FragFileTime = TestFragInfo.FileTime;
    
    if((VertFileTime != ResolveShader.VertInfo.FileTime) || (FragFileTime != ResolveShader.FragInfo.FileTime)) {
        Result = true;
    }
    PlatformCloseFile(&TestVertInfo);
    PlatformCloseFile(&TestFragInfo);
    
    return Result;
}

static void
ReloadShaders(app_memory* Memory) {
    
    gbuffer_shader TempGBufferShader = LoadGBufferShader(Memory);
    if(TempGBufferShader.NoErrors) {
        FreeShader(&GBufferShader.ID);
        GBufferShader = TempGBufferShader;
    }
    
    resolve_shader TempResolveShader = LoadResolveShader(Memory);
    if(TempResolveShader.NoErrors) {
        FreeShader(&ResolveShader.ID);
        ResolveShader = TempResolveShader;
    } 
    
    
    shadowmap_shader TempShadowmapShader = LoadShadowmapShader(Memory);
    if(TempShadowmapShader.NoErrors) {
        FreeShader(&ShadowmapShader.ID);
        ShadowmapShader = TempShadowmapShader;
    } 
    
}


static void
LoadModel(app_memory* Memory) {
    Memory->Scene.LoadedModel = LoadGLTF();
}

static void
InitializeScene(app_memory* Memory) {
    
    v3 CameraP = V3(0, 2, 0);
    v3 ZAxis = V3(0, 0, 1);
    v3 Up = V3(0, 1, 0);
    f32 FOV = 45.0f;
    
    scene* Scene = &Memory->Scene;
    
    Scene->Camera = InitializeCamera(CameraP, ZAxis, Up, FOV, 0.1f, 1000.0f);
    
    u32 MeshCount = Scene->LoadedModel.MeshCount;
    
    
    Scene->RenderMeshIDArray = (u32*)ArenaPush(&Memory->MainArena, MeshCount*sizeof(u32));
    Scene->RenderMeshIDCount = MeshCount;
    
    Scene->RenderMeshTextureArray = (u32**)ArenaPush(&Memory->MainArena, MeshCount*sizeof(u32*));
    
    for(u32 i = 0;
        i < MeshCount;
        i++) {
        Scene->RenderMeshTextureArray[i] = (u32*)ArenaPush(&Memory->MainArena, sizeof(u32)*MATERIAL_TYPE_COUNT);
    }
    
    for(u32 MeshIndex = 0;
        MeshIndex < MeshCount;
        MeshIndex++) {
        gltf_mesh* Mesh = &Scene->LoadedModel.MeshArray[MeshIndex];
        Scene->RenderMeshIDArray[MeshIndex] = RegisterMesh(Mesh);
        
        for(u32 TextureIndex = MATERIAL_TYPE_ALBEDO;
            TextureIndex < MATERIAL_TYPE_COUNT;
            TextureIndex++) {
            
            gltf_texture Texture = Scene->LoadedModel.MaterialArray[Mesh->MaterialIndex].Materials[TextureIndex].Texture;
            
            if(Texture.Data) {
                Scene->RenderMeshTextureArray[MeshIndex][TextureIndex] = RegisterTexture(Texture); 
            }
        }
    }
    
    glNamedBufferSubData(OpenGL->TextureBuffer, 0, sizeof(u64)*OpenGL->RegisteredTextureCount, OpenGL->TextureShaderHandleArray);
    
    
    glCreateBuffers(1, &DrawUniformBuffer);
    glNamedBufferData(DrawUniformBuffer, sizeof(per_draw_uniforms)*MAX_DRAWS_PER_BATCH, NULL, GL_DYNAMIC_DRAW);
    DrawUniformArray = (per_draw_uniforms*)ArenaPush(&Memory->MainArena, sizeof(per_draw_uniforms)*MAX_DRAWS_PER_BATCH);
    
    
    glCreateBuffers(1, &FrameUniformBuffer);
    glNamedBufferData(FrameUniformBuffer, sizeof(frame_uniforms), NULL, GL_DYNAMIC_DRAW);
    
    GBufferFramebuffer = CreateFramebuffer(OpenGL->RenderWidth, OpenGL->RenderHeight);
    AddFramebufferTexture(&GBufferFramebuffer, GL_RGBA8);
    AddFramebufferTexture(&GBufferFramebuffer, GL_RGBA16F);
    AddFramebufferTexture(&GBufferFramebuffer, GL_RGBA8);
    AddFramebufferTexture(&GBufferFramebuffer, GL_RGBA16F);
    AddFramebufferTexture(&GBufferFramebuffer, GL_RGBA16F);
    AddFramebufferDepthTexture(&GBufferFramebuffer, GL_DEPTH_COMPONENT16);
    EndFramebufferCreation(&GBufferFramebuffer);
    
    
    ResolveFramebuffer = CreateFramebuffer(OpenGL->RenderWidth, OpenGL->RenderHeight);
    AddFramebufferTexture(&ResolveFramebuffer, GL_RGBA8);
    EndFramebufferCreation(&ResolveFramebuffer);
    
    
    Memory->Scene.GeneratedMeshArray[Memory->Scene.GeneratedMeshCount++] = GenerateSphere(1);
    Memory->Scene.GeneratedMeshID = RegisterGeneratedMesh(&Memory->Scene.GeneratedMeshArray[0]);
    
    ShadowmapFramebuffer = CreateFramebuffer(OpenGL->RenderWidth, OpenGL->RenderHeight);
    AddFramebufferDepthTexture(&ShadowmapFramebuffer, GL_DEPTH_COMPONENT16);
    EndFramebufferCreation(&ShadowmapFramebuffer);
    
    
    Scene->IsInitialized = true;
}

static void
UpdateAndRender(app_memory* Memory) {
    
    if(!Memory->Scene.IsInitialized) {
        FreeShader(&GBufferShader.ID);
        ReloadShaders(Memory);
        LoadModel(Memory);
        InitializeScene(Memory);
    }
    
    if(ShouldShadersReload()) {
        ReloadShaders(Memory);
    }
    
    UpdateCamera(&Memory->Scene.Camera);
    OpenGL->IndirectCommandsCount = 0;
    
    point_light Lights[4] = {};
    Lights[0].Position = V3(4, 1, 3);
    Lights[0].Color = V3(0.9, 0.9, 0.9);
    Lights[0].Constant = 1.0;
    Lights[0].Linear = 0.7;
    Lights[0].Quadratic = 1.8;
    
    Lights[1].Position = V3(4, 3, 3);
    Lights[1].Color = V3(0.9, 0.9, 0.9);
    Lights[1].Constant = 1.0;
    Lights[1].Linear = 0.7;
    Lights[1].Quadratic = 1.8;
    
    Lights[2].Position = V3(0, 1, 3);
    Lights[2].Color = V3(0.9, 0.9, 0.9);
    Lights[2].Constant = 1.0;
    Lights[2].Linear = 0.7;
    Lights[2].Quadratic = 1.8;
    
    Lights[3].Position = V3(0, 3, 3);
    Lights[3].Color = V3(0.9, 0.9, 0.9);
    Lights[3].Constant = 1.0;
    Lights[3].Linear = 0.7;
    Lights[3].Quadratic = 1.8;
    
    directional_light SunLight = {};
    SunLight.Direction = Normalize(V3(0.3, 1, 0));
    SunLight.Color = V3(0.8, 0.8, 0.2);
    
    m4x4_inv SunLightProj = GenerateDirectionLightProj(SunLight);
    
    for(u32 LightIndex = 0;
        LightIndex < 4;
        LightIndex++) {
        per_draw_uniforms DrawUniform = {};
        DrawUniform.RoughnessFactor = 0;
        DrawUniform.MetallicFactor = 1;
        DrawUniform.ShaderFlags |= SHADER_FLAG_USE_COLOR;
        DrawUniform.Color = V4(1, 1, 1, 1);
        
        m4x4 M = M4x4Identity();
        M = Translate(M, Lights[LightIndex].Position);
        M = Scale(M, V3(0.1, 0.1, 0.1));
        DrawUniform.ModelProj = M;
        
        
        DrawUniformArray[OpenGL->IndirectCommandsCount] = DrawUniform;
        
        u32 MeshID = Memory->Scene.GeneratedMeshID;
        OpenGL->IndirectCommands[OpenGL->IndirectCommandsCount] = GenerateIndirectDrawCommand(MeshID, 1);
        OpenGL->IndirectCommandsCount++;
        
    }
    
    
    u32 SphereCount = 5;
    for(u32 Y = 0;
        Y < SphereCount;
        Y++) {
        for(u32 X = 0;
            X < SphereCount;
            X++) {
            
            per_draw_uniforms DrawUniform = {};
            
            f32 tRough = (f32)X / (f32)(SphereCount);
            f32 tMetal = (f32)Y / (f32)(SphereCount);
            DrawUniform.RoughnessFactor = tRough;
            DrawUniform.MetallicFactor = tMetal;
            DrawUniform.ShaderFlags |= SHADER_FLAG_USE_COLOR;
            DrawUniform.ShaderFlags |= SHADER_FLAG_USE_ROUGHNESS;
            DrawUniform.ShaderFlags |= SHADER_FLAG_USE_METALLIC;
            DrawUniform.Color = V4(1, 0, 0, 1);
            
            m4x4 M = M4x4Identity();
            M = Translate(M , V3(0, Y, X));
            M = Scale(M, V3(0.5, 0.5, 0.5));
            DrawUniform.ModelProj = M;
            
            
            DrawUniformArray[OpenGL->IndirectCommandsCount] = DrawUniform;
            
            u32 MeshID = Memory->Scene.GeneratedMeshID;
            OpenGL->IndirectCommands[OpenGL->IndirectCommandsCount] = GenerateIndirectDrawCommand(MeshID, 1);
            OpenGL->IndirectCommandsCount++;
            
        }
    }
    
#if 1
    for(u32 MeshIndex = 0;
        MeshIndex < Memory->Scene.RenderMeshIDCount;
        MeshIndex++) {
        
        per_draw_uniforms DrawUniform = {};
        DrawUniform.AlbedoID = Memory->Scene.RenderMeshTextureArray[MeshIndex][MATERIAL_TYPE_ALBEDO];
        DrawUniform.NormalID = Memory->Scene.RenderMeshTextureArray[MeshIndex][MATERIAL_TYPE_NORMAL];
        DrawUniform.RoughnessID = Memory->Scene.RenderMeshTextureArray[MeshIndex][MATERIAL_TYPE_ROUGHNESS];
        DrawUniform.OcclusionID = Memory->Scene.RenderMeshTextureArray[MeshIndex][MATERIAL_TYPE_OCCLUSION];
        
        DrawUniform.RoughnessFactor = 0;
        DrawUniform.MetallicFactor = 0;
        DrawUniform.ModelProj = M4x4Identity();
        
        DrawUniformArray[OpenGL->IndirectCommandsCount] = DrawUniform;
        
        u32 MeshID = Memory->Scene.RenderMeshIDArray[MeshIndex];
        OpenGL->IndirectCommands[OpenGL->IndirectCommandsCount] = GenerateIndirectDrawCommand(MeshID, 1);
        OpenGL->IndirectCommandsCount++;
    }
#endif
    
    m4x4_inv Proj = SetProjection(&Memory->Scene.Camera);
    
    FrameUniforms.CameraProj = Proj.Forward;
    FrameUniforms.LightProj = SunLightProj.Forward;
    FrameUniforms.PointLight[0] = Lights[0];
    FrameUniforms.PointLight[1] = Lights[1];
    FrameUniforms.PointLight[2] = Lights[2];
    FrameUniforms.PointLight[3] = Lights[3];
    
    FrameUniforms.SunLight = SunLight;
    
    FrameUniforms.FramebufferTextureToDisplay = 0;
    FrameUniforms.CameraP = Memory->Scene.Camera.P;
    
    glNamedBufferSubData(FrameUniformBuffer, 0, sizeof(frame_uniforms), &FrameUniforms); 
    
    glFrontFace(GL_CW);
    
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_SCISSOR_TEST);
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    glBlendEquation(GL_FUNC_ADD);
    
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    
    
    {
        glUseProgram(ShadowmapShader.ID);
        
        glBindFramebuffer(GL_FRAMEBUFFER, ShadowmapFramebuffer.ID);
        glViewport(0, 0, OpenGL->RenderWidth, OpenGL->RenderHeight);
        glScissor(0, 0, OpenGL->RenderWidth, OpenGL->RenderHeight);
        glClear(GL_DEPTH_BUFFER_BIT);
        
        glBindVertexArray(OpenGL->ModelGeometryInfo.VAO);
        
        glBindBufferBase(GL_UNIFORM_BUFFER, ShadowmapShader.FrameUniformsID, FrameUniformBuffer);
        
        u32 TotalDrawCount = OpenGL->IndirectCommandsCount;
        //glNamedBufferSubData(DrawUniformBuffer, 0, sizeof(per_draw_uniforms)*TotalDrawCount, DrawUniformArray);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, ShadowmapShader.DrawUniformsID, DrawUniformBuffer);
        
        glBufferData(GL_DRAW_INDIRECT_BUFFER, TotalDrawCount*sizeof(draw_element_indirect_command), OpenGL->IndirectCommands, GL_DYNAMIC_DRAW);
        glMultiDrawElementsIndirect(GL_TRIANGLES, GL_UNSIGNED_INT, NULL, TotalDrawCount, 0);
    }
    
    {
        glUseProgram(GBufferShader.ID);
        
        glBindFramebuffer(GL_FRAMEBUFFER, GBufferFramebuffer.ID);
        glViewport(0, 0, OpenGL->RenderWidth, OpenGL->RenderHeight);
        glScissor(0, 0, OpenGL->RenderWidth, OpenGL->RenderHeight);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        
        glBindVertexArray(OpenGL->ModelGeometryInfo.VAO);
        
        glActiveTexture(GBufferShader.ShadowmapID);
        glBindTexture(GL_TEXTURE_2D, ShadowmapFramebuffer.DepthTarget);
        
        glBindBufferBase(GL_UNIFORM_BUFFER, GBufferShader.TextureArrayID, OpenGL->TextureBuffer);
        glBindBuffer(GL_DRAW_INDIRECT_BUFFER, OpenGL->IndirectCommandBuffer);
        
        glBindBufferBase(GL_UNIFORM_BUFFER, GBufferShader.FrameUniformsID, FrameUniformBuffer);
        
        u32 TotalDrawCount = OpenGL->IndirectCommandsCount;
        
        glNamedBufferSubData(DrawUniformBuffer, 0, sizeof(per_draw_uniforms)*TotalDrawCount, DrawUniformArray);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, GBufferShader.DrawUniformsID, DrawUniformBuffer);
        
        glBufferData(GL_DRAW_INDIRECT_BUFFER, TotalDrawCount*sizeof(draw_element_indirect_command), OpenGL->IndirectCommands, GL_DYNAMIC_DRAW);
        glMultiDrawElementsIndirect(GL_TRIANGLES, GL_UNSIGNED_INT, NULL, TotalDrawCount, 0);
    }
    
    
    {
        glUseProgram(ResolveShader.ID);
        glBindVertexArray(OpenGL->FullscreenVAO);
        glBindFramebuffer(GL_FRAMEBUFFER, ResolveFramebuffer.ID);
        
        glActiveTexture(ResolveShader.GAlbedoID);
        glBindTexture(GL_TEXTURE_2D, GBufferFramebuffer.ColorTarget[0]);
        glActiveTexture(ResolveShader.GNormalID);
        glBindTexture(GL_TEXTURE_2D, GBufferFramebuffer.ColorTarget[1]);
        glActiveTexture(ResolveShader.GRoughnessID);
        glBindTexture(GL_TEXTURE_2D, GBufferFramebuffer.ColorTarget[2]);
        glActiveTexture(ResolveShader.GPositionID);
        glBindTexture(GL_TEXTURE_2D, GBufferFramebuffer.ColorTarget[4]);
        glActiveTexture(ResolveShader.GDepthID);
        glBindTexture(GL_TEXTURE_2D, GBufferFramebuffer.DepthTarget);
        glActiveTexture(ResolveShader.GShadowedID);
        glBindTexture(GL_TEXTURE_2D, GBufferFramebuffer.ColorTarget[3]);
        
        glBindBufferBase(GL_UNIFORM_BUFFER, ResolveShader.FrameUniformsID, FrameUniformBuffer);
        
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        
        glBlitNamedFramebuffer(ResolveFramebuffer.ID, 0, 0, 0, ResolveFramebuffer.Width, ResolveFramebuffer.Height,
                               0, 0, OpenGL->RenderWidth, OpenGL->RenderHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR);
    }
}