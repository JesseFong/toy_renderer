
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

enum resolve_texture_names {
    TEXTURE_ALBEDO,
    TEXTURE_NORMAL,
    TEXTURE_ROUGHNESS,
    TEXTURE_POSITION,
    TEXTURE_DEPTH,
    TEXTURE_SHADOWED,
};

struct renderer_options {
    u32 FramebufferTextureToDisplay;
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
    float Radius;
    v3 Color;
    float Pad0;
};

#define MAX_POINT_LIGHTS 4
struct frame_uniforms {
    m4x4 CameraProj;
    m4x4 LightProj;
    point_light PointLight[MAX_POINT_LIGHTS];
    directional_light SunLight;
    v3 CameraP;
    u32 FramebufferTextureToDisplay;
};

#define MAX_DRAWS_PER_BATCH 2048
#define MAX_VERTICES 65536*2*2*2*2
#define MAX_INDIRECT_COMMANDS 65536
#define MAX_TEXTURE_HANDLES 1024
#define MAX_MESH_HANDLES 512
#define CASCADE_COUNT 4

struct mesh_geometry_buffers {
    GLuint VAO;
    GLuint PBuffer;
    GLuint UVBuffer;
    GLuint NBuffer;
    GLuint ColorBuffer;
    GLuint IndexBuffer;
    
    u32 MaxVertexCount;
    u32 MaxIndexCount;
    
    u32 CurrentVertexCount;
    u32 CurrentIndexCount;
};

struct gl_context {
    u32 RenderWidth;
    u32 RenderHeight;
    
    mesh_geometry_buffers MeshInfo;
    registered_mesh MeshHandles[MAX_MESH_HANDLES];
    u32 MeshHandleCount;
    
    GLuint FullscreenVAO;
    GLuint FullscreenVBO;
    
    GLuint CubemapVAO;
    GLuint CubemapVBO;
    
    GLuint DrawIDBuffer;
    
    GLuint IndirectCommandBuffer;
    draw_element_indirect_command IndirectCommands[MAX_INDIRECT_COMMANDS];
    u32 IndirectCommandsCount;
    
    GLuint TextureHandleBuffer;
    u64 TextureHandles[MAX_TEXTURE_HANDLES];
    u32 TextureHandleCount;
    
    GLuint DrawUniformBuffer;
    per_draw_uniforms DrawUniforms[MAX_DRAWS_PER_BATCH];
    
    GLuint FrameUniformBuffer;
    frame_uniforms FrameUniforms;
    
    framebuffer ShadowmapFramebuffer;
    framebuffer CubeShadowmapFramebuffer;
    framebuffer SunlightFramebuffer;
    framebuffer GBufferFramebuffer;
    framebuffer ResolveFramebuffer;
    
    shader ShadowmapShader;
    shader PointLightShadowShader;
    shader CascadeShadowmapShader;
    shader SkyboxShader;
    shader GBufferShader;
    shader ResolveShader;
};

gl_context* OpenGL = NULL;

static void
InitializeMeshInfo() {
    
    u32 MaxVertexCount = MAX_VERTICES;
    u32 MaxIndexCount = MaxVertexCount;
    
    mesh_geometry_buffers* Info = &OpenGL->MeshInfo;
    
    Info->MaxVertexCount = MaxVertexCount;
    Info->MaxIndexCount = MaxIndexCount;
    
    Info->CurrentIndexCount = 0;
    Info->CurrentVertexCount = 0;
    
    glCreateVertexArrays(1, &Info->VAO);
    
    glCreateBuffers(1, &Info->IndexBuffer);
    glNamedBufferStorage(Info->IndexBuffer, sizeof(u32)*MaxIndexCount, NULL, GL_DYNAMIC_STORAGE_BIT);
    glVertexArrayElementBuffer(Info->VAO, Info->IndexBuffer);
    
    glCreateBuffers(1, &Info->PBuffer);
    glNamedBufferStorage(Info->PBuffer, sizeof(v3)*MaxVertexCount, NULL, GL_DYNAMIC_STORAGE_BIT);
    glVertexArrayVertexBuffer(Info->VAO, 0, Info->PBuffer, 0, sizeof(f32)*3);
    glEnableVertexArrayAttrib(Info->VAO, 0);
    glVertexArrayAttribFormat(Info->VAO, 0, 3, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribBinding(Info->VAO, 0, 0);
    
    glCreateBuffers(1, &Info->UVBuffer);
    glNamedBufferStorage(Info->UVBuffer, sizeof(v2)*MaxVertexCount, NULL, GL_DYNAMIC_STORAGE_BIT);
    glVertexArrayVertexBuffer(Info->VAO, 1, Info->UVBuffer, 0, sizeof(f32)*2);
    glEnableVertexArrayAttrib(Info->VAO, 1);
    glVertexArrayAttribFormat(Info->VAO, 1, 2, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribBinding(Info->VAO, 1, 1);
    
    glCreateBuffers(1, &Info->NBuffer);
    glNamedBufferStorage(Info->NBuffer, sizeof(v3)*MaxVertexCount, NULL, GL_DYNAMIC_STORAGE_BIT);
    glVertexArrayVertexBuffer(Info->VAO, 2, Info->NBuffer, 0, sizeof(f32)*3);
    glEnableVertexArrayAttrib(Info->VAO, 2);
    glVertexArrayAttribFormat(Info->VAO, 2, 3, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribBinding(Info->VAO, 2, 2);
    
    u32 DrawIDs[MAX_DRAWS_PER_BATCH] = {};
    for(u32 I = 0;
        I < MAX_DRAWS_PER_BATCH;
        I++) {
        DrawIDs[I] = I;
    }
    glCreateBuffers(1, &OpenGL->DrawIDBuffer);
    glNamedBufferData(OpenGL->DrawIDBuffer, sizeof(u32)*MAX_DRAWS_PER_BATCH, DrawIDs, GL_STATIC_DRAW);
    
    glVertexArrayVertexBuffer(Info->VAO, 3, OpenGL->DrawIDBuffer, 0, sizeof(u32));
    glEnableVertexArrayAttrib(Info->VAO, 3);
    glVertexArrayAttribIFormat(Info->VAO, 3, 1, GL_UNSIGNED_INT, 0);
    glVertexArrayAttribBinding(Info->VAO, 3, 3);
    glVertexArrayBindingDivisor(Info->VAO, 3, 1);
    
}

static void
InitializeFullScreenInfo() {
    
    glCreateVertexArrays(1, &OpenGL->FullscreenVAO);
    
    glCreateBuffers(1, &OpenGL->FullscreenVBO);
    glNamedBufferData(OpenGL->FullscreenVBO, sizeof(GLOBALScreenQuadVertices), &GLOBALScreenQuadVertices, GL_STATIC_DRAW);
    
    glVertexArrayVertexBuffer(OpenGL->FullscreenVAO, 0, OpenGL->FullscreenVBO, 0, sizeof(f32)*5);
    
    glEnableVertexArrayAttrib(OpenGL->FullscreenVAO, 0);
    glVertexArrayAttribFormat(OpenGL->FullscreenVAO, 0, 3, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribBinding(OpenGL->FullscreenVAO, 0, 0);
    
    glEnableVertexArrayAttrib(OpenGL->FullscreenVAO, 1);
    glVertexArrayAttribFormat(OpenGL->FullscreenVAO, 1, 2, GL_FLOAT, GL_FALSE, sizeof(f32)*3);
    glVertexArrayAttribBinding(OpenGL->FullscreenVAO, 1, 0);
}

static void
InitializeCubemapInfo() {
    glCreateVertexArrays(1, &OpenGL->CubemapVAO);
    
    glCreateBuffers(1, &OpenGL->CubemapVBO);
    glNamedBufferData(OpenGL->CubemapVBO, sizeof(GLOBALCubemapVertices), &GLOBALCubemapVertices, GL_STATIC_DRAW);
    
    glVertexArrayVertexBuffer(OpenGL->CubemapVAO, 0, OpenGL->CubemapVBO, 0, sizeof(f32)*3);
    glEnableVertexArrayAttrib(OpenGL->CubemapVAO, 0);
    glVertexArrayAttribFormat(OpenGL->CubemapVAO, 0, 3, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribBinding(OpenGL->CubemapVAO, 0, 0);
}

static u32
RegisterMesh(gltf_mesh* GLTFMesh) {
    mesh_geometry_buffers* Info = &OpenGL->MeshInfo;
    
    registered_mesh Mesh = {};
    Mesh.VertexCount = GLTFMesh->VertexCount;
    Mesh.IndexCount = GLTFMesh->IndexCount;
    Mesh.BaseVertex = Info->CurrentVertexCount;
    Mesh.BaseIndex = Info->CurrentIndexCount;
    
    glNamedBufferSubData(Info->IndexBuffer, Info->CurrentIndexCount*sizeof(u32), Mesh.IndexCount*sizeof(u32), (void*)GLTFMesh->Indices);
    glNamedBufferSubData(Info->PBuffer, Info->CurrentVertexCount*sizeof(v3), Mesh.VertexCount*sizeof(v3), (void*)GLTFMesh->P);
    glNamedBufferSubData(Info->UVBuffer, Info->CurrentVertexCount*sizeof(v2), Mesh.VertexCount*sizeof(v2), (void*)GLTFMesh->UV);
    glNamedBufferSubData(Info->NBuffer, Info->CurrentVertexCount*sizeof(v3), Mesh.VertexCount*sizeof(v3), (void*)GLTFMesh->N);
    
    Info->CurrentVertexCount += Mesh.VertexCount;
    Info->CurrentIndexCount += Mesh.IndexCount;
    
    Assert(OpenGL->MeshHandleCount < ArrayCount(OpenGL->MeshHandles));
    OpenGL->MeshHandles[OpenGL->MeshHandleCount] = Mesh;
    return OpenGL->MeshHandleCount++;
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

static u32
RegisterTexture(GLuint ID) {
    u64 BindlessHandle = glGetTextureHandleARB(ID);
    glMakeTextureHandleResidentARB(BindlessHandle);
    
    Assert(OpenGL->TextureHandleCount < ArrayCount(OpenGL->TextureHandles));
    OpenGL->TextureHandles[OpenGL->TextureHandleCount] = BindlessHandle;
    return OpenGL->TextureHandleCount++;
}

static draw_element_indirect_command
GenerateIndirectDrawCommand(u32 MeshID, u32 InstanceCount) {
    registered_mesh* Mesh = &OpenGL->MeshHandles[MeshID];
    
    draw_element_indirect_command IndirectCommand;
    IndirectCommand.Count = Mesh->IndexCount;
    IndirectCommand.InstanceCount = 1;
    IndirectCommand.FirstIndex = Mesh->BaseIndex;
    IndirectCommand.BaseVertex = Mesh->BaseVertex;
    IndirectCommand.BaseInstance = OpenGL->IndirectCommandsCount;
    
    return IndirectCommand;
}

static void
RegisterDrawCommand(u32 MeshID, u32 InstanceCount) {
    draw_element_indirect_command Command = GenerateIndirectDrawCommand(MeshID, InstanceCount);
    OpenGL->IndirectCommands[OpenGL->IndirectCommandsCount] = Command;
    OpenGL->IndirectCommandsCount++;
}

static void
InitializeOpenGLScene(app_memory* Memory, u32 WindowWidth, u32 WindowHeight) {
    
    OpenGL = ArenaPushStruct(&Memory->MainArena, gl_context);
    
    v2u RenderSize = PlatformGetWindowDimension();
    OpenGL->RenderWidth = RenderSize.x;
    OpenGL->RenderHeight = RenderSize.y;
    
    InitializeMeshInfo();
    InitializeFullScreenInfo();
    InitializeCubemapInfo();
    
    glCreateBuffers(1, &OpenGL->IndirectCommandBuffer);
    glNamedBufferStorage(OpenGL->IndirectCommandBuffer, sizeof(OpenGL->IndirectCommands), NULL, GL_DYNAMIC_STORAGE_BIT);
    
    glCreateBuffers(1, &OpenGL->TextureHandleBuffer);
    glNamedBufferStorage(OpenGL->TextureHandleBuffer, sizeof(OpenGL->TextureHandles), NULL, GL_DYNAMIC_STORAGE_BIT);
    
    
    glCreateBuffers(1, &OpenGL->DrawUniformBuffer);
    glNamedBufferData(OpenGL->DrawUniformBuffer, sizeof(per_draw_uniforms)*MAX_DRAWS_PER_BATCH, NULL, GL_DYNAMIC_DRAW);
    
    glCreateBuffers(1, &OpenGL->FrameUniformBuffer);
    glNamedBufferData(OpenGL->FrameUniformBuffer, sizeof(frame_uniforms), NULL, GL_DYNAMIC_DRAW);
    
    OpenGL->GBufferFramebuffer = CreateFramebuffer(OpenGL->RenderWidth, OpenGL->RenderHeight);
    
    AddFramebufferTexture(&OpenGL->GBufferFramebuffer, GL_RGBA32F);
    AddFramebufferTexture(&OpenGL->GBufferFramebuffer, GL_RGBA32F);
    AddFramebufferTexture(&OpenGL->GBufferFramebuffer, GL_RGBA32F);
    AddFramebufferTexture(&OpenGL->GBufferFramebuffer, GL_RGBA32F);
    AddFramebufferTexture(&OpenGL->GBufferFramebuffer, GL_RGBA32F);
    GLuint GBufferDepth = CreateDepthTexture(OpenGL->RenderWidth, OpenGL->RenderHeight, GL_DEPTH_COMPONENT16, GL_TEXTURE_2D);
    AddFramebufferDepthTexture(&OpenGL->GBufferFramebuffer, GBufferDepth);
    EndFramebufferCreation(&OpenGL->GBufferFramebuffer);
    
    OpenGL->ResolveFramebuffer = CreateFramebuffer(OpenGL->RenderWidth, OpenGL->RenderHeight);
    AddFramebufferTexture(&OpenGL->ResolveFramebuffer, GL_RGBA8);
    EndFramebufferCreation(&OpenGL->ResolveFramebuffer);
    
    generated_mesh SphereMesh = GenerateSphere(1);
    Memory->Scene.GeneratedSphereID = RegisterGeneratedMesh(&SphereMesh);
    generated_mesh CubeMesh = GenerateTessellatedCube(1);
    Memory->Scene.GeneratedCubeID = RegisterGeneratedMesh(&CubeMesh);
    
    OpenGL->ShadowmapFramebuffer = CreateFramebuffer(OpenGL->RenderWidth, OpenGL->RenderHeight);
    GLuint ShadowmapDepth = CreateDepthTexture(OpenGL->RenderWidth, OpenGL->RenderHeight, GL_DEPTH_COMPONENT32F, GL_TEXTURE_2D);
    AddFramebufferDepthTexture(&OpenGL->ShadowmapFramebuffer, ShadowmapDepth);
    EndFramebufferCreation(&OpenGL->ShadowmapFramebuffer);
    
    
    OpenGL->CubeShadowmapFramebuffer = CreateFramebuffer(1024, 1024);
    GLuint CubeDepth = Create3DDepthTexture(1024, 1024, MAX_POINT_LIGHTS*6, GL_DEPTH_COMPONENT32F, GL_TEXTURE_CUBE_MAP_ARRAY);
    AddFramebufferDepthTexture(&OpenGL->CubeShadowmapFramebuffer, CubeDepth);
    EndFramebufferCreation(&OpenGL->CubeShadowmapFramebuffer);
    
    OpenGL->SunlightFramebuffer = CreateFramebuffer(2048, 2048);
    GLuint SunDepth = Create3DDepthTexture(2048, 2048, CASCADE_COUNT, GL_DEPTH_COMPONENT32F, GL_TEXTURE_2D_ARRAY);
    AddFramebufferDepthTexture(&OpenGL->SunlightFramebuffer, SunDepth);
    EndFramebufferCreation(&OpenGL->SunlightFramebuffer);
    
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
    GLuint WhiteTextureID = CreateTexture(WhiteTexture);
    
    u32 WhiteTextureIndex = RegisterTexture(WhiteTextureID);
    Assert(WhiteTextureIndex == 0);
    
    u8 DefaultNormalBitmapData[4];
    DefaultNormalBitmapData[0] = 128;
    DefaultNormalBitmapData[1] = 128;
    DefaultNormalBitmapData[2] = 255;
    DefaultNormalBitmapData[3] = 255; 
    gltf_texture DefaultNormalTexture = {};
    DefaultNormalTexture.Width = 1;
    DefaultNormalTexture.Height = 1;
    DefaultNormalTexture.Channels = 4;
    DefaultNormalTexture.Data = DefaultNormalBitmapData;
    GLuint DefaultNormalTextureID = CreateTexture(DefaultNormalTexture);
    
    u32 DefaultNormalTextureIndex = RegisterTexture(DefaultNormalTextureID);
    Assert(DefaultNormalTextureIndex == 1);
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


static void
GeneratePointLightProj(point_light Light,  m4x4* ProjArray) {
    
    v3 P = Light.Position;
    float FarPlane = Light.Radius;
    
    m4x4 LightProj = PerspectiveProjection(1, Radians(90.0), 0.1, FarPlane).Forward;
    
    ProjArray[0] = LightProj * LookAt(P, P + V3(1, 0, 0), V3(0, -1, 0)).Forward;
    ProjArray[1] = LightProj * LookAt(P, P + V3(-1, 0, 0), V3(0, -1, 0)).Forward;
    ProjArray[2] = LightProj * LookAt(P, P + V3(0, 1, 0), V3(0, 0, 1)).Forward;
    ProjArray[3] = LightProj * LookAt(P, P + V3(0, -1, 0), V3(0, 0, -1)).Forward;
    ProjArray[4] = LightProj * LookAt(P, P + V3(0, 0, 1), V3(0, -1, 0)).Forward;
    ProjArray[5] = LightProj * LookAt(P, P + V3(0, 0, -1), V3(0, -1, 0)).Forward;
    
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

static void
GetFrustrumCornersWorldSpace(m4x4 InvProj, v4* CornersOut) {
    
    u32 CornerIndex = 0;
    for(u32 X = 0;X < 2;X++) {
        for(u32 Y = 0;Y < 2;Y++) {
            for(u32 Z = 0; Z < 2; Z++) {
                
                v4 Point = InvProj * V4(2.0f*X - 1.0f, 2.0f*Y - 1.0f, 2.0f*Z - 1.0f, 1.0f);
                
                CornersOut[CornerIndex++] = Point / Point.w; 
            }
        }
    }
}

static m4x4
GetCascadeLightSpaceMatrix(directional_light Light, rectangle3 BoundingBox, camera* Camera, f32 NearPlane, f32 FarPlane) {
    
    f32 AspectWidthOverHeight = (f32)OpenGL->RenderWidth/(f32)OpenGL->RenderHeight;
    
    m4x4_inv ProjInv = PerspectiveProjection(AspectWidthOverHeight, Camera->FOV, NearPlane, FarPlane);
    m4x4_inv CameraXForm = CameraTransform(Camera->XAxis, Camera->YAxis, Camera->ZAxis, Camera->P);
    
    m4x4_inv Proj;
    Proj.Forward = ProjInv.Forward * CameraXForm.Forward;
    Proj.Inverse = CameraXForm.Inverse * ProjInv.Inverse;
    
    v4 Corners[8] = {};
    GetFrustrumCornersWorldSpace(Proj.Inverse, Corners); 
    
    
    v3 Center = {};
    for(u32 I = 0;
        I < 8;
        I++) {
        
        Center += Corners[I].xyz;
    }
    Center /= 8;
    
    m4x4_inv LightViewMatrix = LookAt(Center + Light.Direction, Center, V3(0, 1, 0));
    
    f32 MinX = F32MAX;
    f32 MaxX = F32MIN;
    f32 MinY = F32MAX;
    f32 MaxY = F32MIN;
    f32 MinZ = F32MAX;
    f32 MaxZ = F32MIN;
    
    for(u32 I = 0; I < 8; I++) {
        
        v4 Test = LightViewMatrix.Forward * Corners[I];
        MinX = Min(MinX, Test.x);
        MaxX = Max(MaxX, Test.x);
        MinY = Min(MinY, Test.y);
        MaxY = Max(MaxY, Test.y);
        MinZ = Min(MinZ, Test.z);
        MaxZ = Max(MaxZ, Test.z);
    }
    
    f32 ZMuliplier = 2.0f;
    if(MinZ < 0.0f) {
        MinZ *= ZMuliplier;
    } else {
        MinZ /= ZMuliplier;
    }
    if(MaxZ < 0.0f) {
        MaxZ /= ZMuliplier;
    } else {
        MaxZ *= ZMuliplier;
    }
    
    m4x4_inv LightOrthoMatrix = OrthographicProjection(V2(MinX, MaxX), V2(BoundingBox.Min.y, BoundingBox.Max.y), V2(MinZ, MaxZ));
    
    f32 ScaleX = 2.0f / (MaxX - MinX);
    f32 ScaleY = 2.0f / (MaxY - MinY);
    f32 OffsetX = -0.5f * (MinX + MaxX) * ScaleX;
    f32 OffsetY = -0.5f * (MinY + MaxY) * ScaleY;
    
    m4x4 CropMatrix = M4x4Identity();
    CropMatrix.E[0][0] = ScaleX;
    //CropMatrix.E[1][1] = ScaleY;
    CropMatrix.E[0][3] = OffsetX;
    //CropMatrix.E[1][3] = OffsetY;
    
    m4x4 Result = LightOrthoMatrix.Forward * LightViewMatrix.Forward;
    return Result;
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

static void
LoadModel(app_memory* Memory) {
    Memory->Scene.LoadedModel = LoadGLTF();
}

static void
LoadSkybox(app_memory* Memory) {
    gltf_texture SkyboxImages[6] = {};
    
    stbi_set_flip_vertically_on_load(true);
    
    SkyboxImages[0] = LoadGLTFImageFromFile("skybox/right.jpg");
    SkyboxImages[1] = LoadGLTFImageFromFile("skybox/left.jpg");
    SkyboxImages[2] = LoadGLTFImageFromFile("skybox/top.jpg");
    SkyboxImages[3] = LoadGLTFImageFromFile("skybox/bottom.jpg");
    SkyboxImages[4] = LoadGLTFImageFromFile("skybox/front.jpg");
    SkyboxImages[5] = LoadGLTFImageFromFile("skybox/back.jpg");
    
    u8* DataArray[6] = {};
    DataArray[0] = SkyboxImages[0].Data;
    DataArray[1] = SkyboxImages[1].Data;
    DataArray[2] = SkyboxImages[2].Data;
    DataArray[3] = SkyboxImages[3].Data;
    DataArray[4] = SkyboxImages[4].Data;
    DataArray[5] = SkyboxImages[5].Data;
    
    Memory->Scene.SkyboxID = CreateCubemapTexture(DataArray, SkyboxImages[0].Width, SkyboxImages[0].Height, GL_RGB8);
    
    stbi_set_flip_vertically_on_load(false);
}

static per_draw_uniforms
BuildDrawUniforms(v3 P, v3 S, f32 Roughness, f32 Metalness, v4 Color) {
    
    per_draw_uniforms DrawUniform = {};
    DrawUniform.RoughnessFactor = Roughness;
    DrawUniform.MetallicFactor = Metalness;
    DrawUniform.ShaderFlags |= SHADER_FLAG_USE_COLOR;
    DrawUniform.ShaderFlags |= SHADER_FLAG_USE_ROUGHNESS;
    DrawUniform.ShaderFlags |= SHADER_FLAG_USE_METALLIC;
    DrawUniform.Color = Color;
    
    m4x4 M = M4x4Identity();
    M = Translate(M, P);
    M = Scale(M, S);
    DrawUniform.ModelProj = M;
    
    return DrawUniform;
}

static void
DrawCube(app_memory* Memory, v3 P, v3 S, f32 Roughness, f32 Metalness, v4 Color) {
    
    per_draw_uniforms DrawUniform = BuildDrawUniforms(P, S, Roughness, Metalness, Color);
    OpenGL->DrawUniforms[OpenGL->IndirectCommandsCount] = DrawUniform;
    
    u32 MeshID = Memory->Scene.GeneratedCubeID;
    RegisterDrawCommand(MeshID, 1);
}


static void
DrawSphere(app_memory* Memory, v3 P, v3 Scale, f32 Roughness, f32 Metalness, v4 Color) {
    
    per_draw_uniforms DrawUniform = BuildDrawUniforms(P, Scale, Roughness, Metalness, Color);
    DrawUniform.NormalID = 1;
    OpenGL->DrawUniforms[OpenGL->IndirectCommandsCount] = DrawUniform;
    
    u32 MeshID = Memory->Scene.GeneratedSphereID;
    RegisterDrawCommand(MeshID, 1);
}


static void
InitializeScene(app_memory* Memory) {
    
    v3 CameraP = V3(0, 2, 0);
    v3 ZAxis = V3(0, 0, 1);
    v3 Up = V3(0, 1, 0);
    f32 FOV = 45.0f;
    
    scene* Scene = &Memory->Scene;
    
    Scene->Camera = InitializeCamera(CameraP, ZAxis, Up, FOV, 0.1f, 50.0f);
    
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
                GLuint TextureID = CreateTexture(Texture);
                Scene->RenderMeshTextureArray[MeshIndex][TextureIndex] = RegisterTexture(TextureID); 
            }
        }
    }
    
    OpenGL->IndirectCommandsCount = 0;
    
#if 0
    u32 SphereCount = 5;
    for(u32 Y = 0;
        Y < SphereCount;
        Y++) {
        for(u32 X = 0;
            X < SphereCount;
            X++) {
            
            f32 tRough = (f32)X / (f32)(SphereCount);
            f32 tMetal = (f32)Y / (f32)(SphereCount);
            v4 Color = V4(1, 0, 0, 1);
            v3 P = V3(0, Y, X);
            v3 S = V3(0.5, 0.5, 0.5);
            
            DrawSphere(Memory, P, S, tRough, tMetal, Color);
        }
    }
#endif
    
#if 0
    {
        
        DrawCube(Memory, V3(0, 0, 0), V3(5, 0.1, 5), 1, 1, V4(1, 1, 1, 1));
        DrawCube(Memory, V3(5, 1.5, 0), V3(0.1, 3, 5), 1, 1, V4(1, 1, 1, 1));
        DrawCube(Memory, V3(-5, 1.5, 0), V3(0.1, 3, 5), 1, 1, V4(1, 1, 1, 1));
        DrawCube(Memory, V3(0, 1.5, 5), V3(5, 3, 0.1), 1, 1, V4(1, 1, 1, 1));
        
        DrawCube(Memory, V3(2, 2, 0), V3(0.5, 0.5, 0.5), 0.9, 0.9, V4(0.8, 0.8, 0.2, 1));
        DrawCube(Memory, V3(-2, 2, -2), V3(0.5, 0.5, 0.5), 0.9, 0.9, V4(0.8, 0.8, 0.2, 1));
        DrawSphere(Memory, V3(-2, 1.5, 0), V3(0.5, 0.5 ,0.5), 0.5, 0.5, V4(1, 0, 0, 1));
    }
#endif
    
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
        
        OpenGL->DrawUniforms[OpenGL->IndirectCommandsCount] = DrawUniform;
        
        u32 MeshID = Memory->Scene.RenderMeshIDArray[MeshIndex];
        RegisterDrawCommand(MeshID, 1);
    }
#endif
    
    
    u32 TotalDrawCount = OpenGL->IndirectCommandsCount;
    glNamedBufferSubData(OpenGL->TextureHandleBuffer, 0, sizeof(u64)*OpenGL->TextureHandleCount, OpenGL->TextureHandles);
    glNamedBufferSubData(OpenGL->IndirectCommandBuffer, 0, sizeof(draw_element_indirect_command)*TotalDrawCount, OpenGL->IndirectCommands);
    glBindBuffer(GL_DRAW_INDIRECT_BUFFER, OpenGL->IndirectCommandBuffer);
    
    glFrontFace(GL_CW);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_SCISSOR_TEST);
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBlendEquation(GL_FUNC_ADD);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    
    Scene->IsInitialized = true;
}

static void
UpdateAndRender(app_memory* Memory) {
    
    if(!Memory->Scene.IsInitialized) {
        LoadModel(Memory);
        LoadSkybox(Memory);
        InitializeScene(Memory);
        
        OpenGL->GBufferShader = LoadShader(Memory, "gl_shaders/gbuffer.vert", "gl_shaders/gbuffer.frag", 0);
        OpenGL->ResolveShader = LoadShader(Memory, "gl_shaders/resolve.vert", "gl_shaders/resolve.frag", 0);
        OpenGL->ShadowmapShader = LoadShader(Memory, "gl_shaders/shadowmap.vert", "gl_shaders/shadowmap.frag", 0);
        OpenGL->SkyboxShader = LoadShader(Memory, "gl_shaders/skybox.vert", "gl_shaders/skybox.frag", 0);
        OpenGL->PointLightShadowShader = LoadShader(Memory, "gl_shaders/point_light_shadow.vert", "gl_shaders/point_light_shadow.frag", "gl_shaders/point_light_shadow.geom");
        OpenGL->CascadeShadowmapShader = LoadShader(Memory, "gl_shaders/cascade_shadowmap.vert", "gl_shaders/cascade_shadowmap.frag", "gl_shaders/cascade_shadowmap.geom");
    }
    
    MaybeReloadShader(Memory, &OpenGL->GBufferShader, "gl_shaders/gbuffer.vert", "gl_shaders/gbuffer.frag", 0);
    MaybeReloadShader(Memory, &OpenGL->ResolveShader, "gl_shaders/resolve.vert", "gl_shaders/resolve.frag", 0);
    MaybeReloadShader(Memory, &OpenGL->ShadowmapShader, "gl_shaders/shadowmap.vert", "gl_shaders/shadowmap.frag", 0);
    MaybeReloadShader(Memory, &OpenGL->SkyboxShader, "gl_shaders/skybox.vert", "gl_shaders/skybox.frag", 0);
    MaybeReloadShader(Memory, &OpenGL->PointLightShadowShader, "gl_shaders/point_light_shadow.vert", "gl_shaders/point_light_shadow.frag", "gl_shaders/point_light_shadow.geom");
    MaybeReloadShader(Memory, &OpenGL->CascadeShadowmapShader, "gl_shaders/cascade_shadowmap.vert", "gl_shaders/cascade_shadowmap.frag", "gl_shaders/cascade_shadowmap.geom");
    
    UpdateCamera(&Memory->Scene.Camera);
    
    point_light Lights[4] = {};
    Lights[0].Position = V3(0, 2, 0);
    Lights[0].Radius = 5.0f;
    Lights[0].Color = V3(0.9, 0.1, 0.1);
    
#if 1
    Lights[1].Position = V3(2, 2, 0);
    Lights[1].Radius = 5.0f;
    Lights[1].Color = V3(0.1, 0.9, 0.1);
    
    
    Lights[2].Position = V3(-2, 2, 0);
    Lights[2].Radius = 5.0f;
    Lights[2].Color = V3(0.1, 0.1, 0.9);
    
    
    Lights[3].Position = V3(0, 1, 0);
    Lights[3].Radius = 5.0f;
    Lights[3].Color = V3(0.9, 0.9, 0.9);
#endif
    
    
    directional_light SunLight = {};
    SunLight.Direction = Normalize(V3(0.3, 1, 0.1));
    SunLight.Color = V3(0.8, 0.8, 0.2);
    
    m4x4_inv SunLightProj = GenerateDirectionLightProj(SunLight);
    
#if 0
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
        
        
        OpenGL->DrawUniforms[OpenGL->IndirectCommandsCount] = DrawUniform;
        u32 MeshID = Memory->Scene.GeneratedSphereID;
        OpenGL->IndirectCommands[OpenGL->IndirectCommandsCount] = GenerateIndirectDrawCommand(MeshID, 1);
        OpenGL->IndirectCommandsCount++;
        
    }
#endif
    
    
    m4x4_inv Proj = SetProjection(&Memory->Scene.Camera);
    
    OpenGL->FrameUniforms.CameraProj = Proj.Forward;
    OpenGL->FrameUniforms.LightProj = SunLightProj.Forward;
    OpenGL->FrameUniforms.PointLight[0] = Lights[0];
    OpenGL->FrameUniforms.PointLight[1] = Lights[1];
    OpenGL->FrameUniforms.PointLight[2] = Lights[2];
    OpenGL->FrameUniforms.PointLight[3] = Lights[3];
    
    OpenGL->FrameUniforms.SunLight = SunLight;
    OpenGL->FrameUniforms.FramebufferTextureToDisplay = 0;
    OpenGL->FrameUniforms.CameraP = Memory->Scene.Camera.P;
    
    u32 TotalDrawCount = OpenGL->IndirectCommandsCount;
    glNamedBufferSubData(OpenGL->DrawUniformBuffer, 0, sizeof(per_draw_uniforms)*TotalDrawCount, OpenGL->DrawUniforms);
    glNamedBufferSubData(OpenGL->FrameUniformBuffer, 0, sizeof(frame_uniforms), &OpenGL->FrameUniforms);
    
    
    {
        
        glBindVertexArray(OpenGL->MeshInfo.VAO);
        GLuint Shader = OpenGL->PointLightShadowShader.ID;
        glUseProgram(Shader);
        
        m4x4 CubeShadowProj[6*MAX_POINT_LIGHTS] = {};
        v3 LightPArray[MAX_POINT_LIGHTS] = {};
        f32 LightFarPlaneArray[MAX_POINT_LIGHTS] = {};
        
        for(u32 I = 0;
            I < MAX_POINT_LIGHTS;
            I++) {
            
            point_light Light = OpenGL->FrameUniforms.PointLight[I];
            LightPArray[I] = Light.Position;
            LightFarPlaneArray[I] = Light.Radius;
            GeneratePointLightProj(Light, &CubeShadowProj[I*6]);
        }
        
        
        glBindFramebuffer(GL_FRAMEBUFFER, OpenGL->CubeShadowmapFramebuffer.ID);
        glViewport(0, 0, OpenGL->CubeShadowmapFramebuffer.Width, OpenGL->CubeShadowmapFramebuffer.Height);
        glScissor(0, 0, OpenGL->CubeShadowmapFramebuffer.Width, OpenGL->CubeShadowmapFramebuffer.Height);
        glClear(GL_DEPTH_BUFFER_BIT);
        
        glUniformMatrix4fv(glGetUniformLocation(Shader, "ShadowMatrices"), 6*MAX_POINT_LIGHTS, GL_TRUE, (GLfloat*)&CubeShadowProj);
        glUniform3fv(glGetUniformLocation(Shader, "LightP"), MAX_POINT_LIGHTS, (GLfloat*)&LightPArray);
        glUniform1fv(glGetUniformLocation(Shader, "FarPlane"), MAX_POINT_LIGHTS, (GLfloat*)&LightFarPlaneArray);
        
        //glNamedBufferSubData(DrawUniformBuffer, 0, sizeof(per_draw_uniforms)*TotalDrawCount, DrawUniformArray);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, glGetProgramResourceIndex(Shader, GL_SHADER_STORAGE_BLOCK, "DrawUniforms"), OpenGL->DrawUniformBuffer);
        
        
        glMultiDrawElementsIndirect(GL_TRIANGLES, GL_UNSIGNED_INT, NULL, TotalDrawCount, 0);
    }
    
    
    camera* Camera = &Memory->Scene.Camera;
    m4x4 SunLightCascadeProj[CASCADE_COUNT] = {};
    float ShadowCascades[] = {Camera->FarClip /15.0f, Camera->FarClip /5.0f, Camera->FarClip /2.0f};
    {
        
        //glCullFace(GL_FRONT);
        for(u32 I = 0;
            I < CASCADE_COUNT;
            I++) {
            
            if(I == 0) {
                SunLightCascadeProj[I] = GetCascadeLightSpaceMatrix(SunLight, Memory->Scene.LoadedModel.BoundingBox, Camera, Camera->NearClip, ShadowCascades[I]);
            } else if(I < CASCADE_COUNT - 1) {
                SunLightCascadeProj[I] = GetCascadeLightSpaceMatrix(SunLight,Memory->Scene.LoadedModel.BoundingBox, Camera, ShadowCascades[I-1], ShadowCascades[I]);
            } else {
                SunLightCascadeProj[I] = GetCascadeLightSpaceMatrix(SunLight,Memory->Scene.LoadedModel.BoundingBox, Camera, ShadowCascades[I-1], Camera->FarClip);
            }
        }
        
        GLuint Shader = OpenGL->CascadeShadowmapShader.ID;
        glUseProgram(Shader);
        
        framebuffer Framebuffer = OpenGL->SunlightFramebuffer;
        glBindFramebuffer(GL_FRAMEBUFFER, Framebuffer.ID);
        glViewport(0, 0, Framebuffer.Width, Framebuffer.Height);
        glScissor(0, 0, Framebuffer.Width, Framebuffer.Height);
        glClear(GL_DEPTH_BUFFER_BIT);
        
        
        glUniformMatrix4fv(glGetUniformLocation(Shader, "LightSpaceMatrices"), CASCADE_COUNT, GL_TRUE, (GLfloat*)&SunLightCascadeProj);
        glBindVertexArray(OpenGL->MeshInfo.VAO);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, glGetProgramResourceIndex(Shader, GL_SHADER_STORAGE_BLOCK, "DrawUniforms"), OpenGL->DrawUniformBuffer);
        
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, glGetProgramResourceIndex(Shader, GL_SHADER_STORAGE_BLOCK, "DrawUniforms"), OpenGL->DrawUniformBuffer);
        
        glMultiDrawElementsIndirect(GL_TRIANGLES, GL_UNSIGNED_INT, NULL, TotalDrawCount, 0);
        
        //glCullFace(GL_BACK);
    }
    
    
    {
        
        GLuint Shader = OpenGL->ShadowmapShader.ID;
        glUseProgram(Shader);
        
        glBindFramebuffer(GL_FRAMEBUFFER, OpenGL->ShadowmapFramebuffer.ID);
        glViewport(0, 0, OpenGL->RenderWidth, OpenGL->RenderHeight);
        glScissor(0, 0, OpenGL->RenderWidth, OpenGL->RenderHeight);
        glClear(GL_DEPTH_BUFFER_BIT);
        
        glBindVertexArray(OpenGL->MeshInfo.VAO);
        glBindBufferBase(GL_UNIFORM_BUFFER, glGetUniformBlockIndex(Shader, "FrameUniforms"), OpenGL->FrameUniformBuffer);
        
        //glNamedBufferSubData(DrawUniformBuffer, 0, sizeof(per_draw_uniforms)*TotalDrawCount, DrawUniformArray);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, glGetProgramResourceIndex(Shader, GL_SHADER_STORAGE_BLOCK, "DrawUniforms"), OpenGL->DrawUniformBuffer);
        
        glMultiDrawElementsIndirect(GL_TRIANGLES, GL_UNSIGNED_INT, NULL, TotalDrawCount, 0);
    }
    
    {
        GLuint Shader = OpenGL->GBufferShader.ID;
        
        glUseProgram(Shader);
        
        glBindFramebuffer(GL_FRAMEBUFFER, OpenGL->GBufferFramebuffer.ID);
        glViewport(0, 0, OpenGL->RenderWidth, OpenGL->RenderHeight);
        glScissor(0, 0, OpenGL->RenderWidth, OpenGL->RenderHeight);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        
        glBindVertexArray(OpenGL->MeshInfo.VAO);
        
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, OpenGL->ShadowmapFramebuffer.DepthTarget);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_CUBE_MAP_ARRAY, OpenGL->CubeShadowmapFramebuffer.DepthTarget);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D_ARRAY, OpenGL->SunlightFramebuffer.DepthTarget);
        
        glUniformMatrix4fv(glGetUniformLocation(Shader, "CascadeProjs"), 4, GL_TRUE, (GLfloat*)&SunLightCascadeProj);
        glUniform1fv(glGetUniformLocation(Shader, "CascadeDepths"), 4, ShadowCascades);
        
        glBindBufferBase(GL_UNIFORM_BUFFER, glGetUniformBlockIndex(Shader, "TextureArray"), OpenGL->TextureHandleBuffer);
        glBindBuffer(GL_DRAW_INDIRECT_BUFFER, OpenGL->IndirectCommandBuffer);
        
        glBindBufferBase(GL_UNIFORM_BUFFER, glGetUniformBlockIndex(Shader, "FrameUniforms"), OpenGL->FrameUniformBuffer);
        
        
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, glGetProgramResourceIndex(Shader, GL_SHADER_STORAGE_BLOCK, "DrawUniforms"), OpenGL->DrawUniformBuffer);
        
        glMultiDrawElementsIndirect(GL_TRIANGLES, GL_UNSIGNED_INT, NULL, TotalDrawCount, 0);
    }
    
    
    {
        
        glBindFramebuffer(GL_FRAMEBUFFER, OpenGL->ResolveFramebuffer.ID);
        glViewport(0, 0, OpenGL->RenderWidth, OpenGL->RenderHeight);
        glScissor(0, 0, OpenGL->RenderWidth, OpenGL->RenderHeight);
        
        GLuint Shader = OpenGL->SkyboxShader.ID;
        glDisable(GL_CULL_FACE);
        glUseProgram(Shader);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        glBindVertexArray(OpenGL->CubemapVAO);
        
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, Memory->Scene.SkyboxID);
        
        f32 AspectWidthOverHeight = (f32)OpenGL->RenderWidth/(f32)OpenGL->RenderHeight;
        
        camera* Camera = &Memory->Scene.Camera;
        m4x4_inv SkyboxProj = PerspectiveProjection(AspectWidthOverHeight, Camera->FOV, Camera->NearClip, Camera->FarClip);
        m4x4_inv SkyboxXForm = CameraTransform(Camera->XAxis, Camera->YAxis, Camera->ZAxis, V3(0, 0, 0));
        
        m4x4 FinalSkyboxProj = SkyboxProj.Forward * SkyboxXForm.Forward;
        
        glUniformMatrix4fv(glGetUniformLocation(Shader, "Proj"), 1, GL_TRUE, (GLfloat*)&FinalSkyboxProj);
        
        //glBindBuffer(GL_ARRAY_BUFFER, OpenGL->CubemapVBO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glEnable(GL_CULL_FACE);
        
    }
    
    
    {
        
        GLuint Shader = OpenGL->ResolveShader.ID;
        
        glUseProgram(Shader);
        glBindVertexArray(OpenGL->FullscreenVAO);
        glBindFramebuffer(GL_FRAMEBUFFER, OpenGL->ResolveFramebuffer.ID);
        
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, OpenGL->GBufferFramebuffer.ColorTarget[0]);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, OpenGL->GBufferFramebuffer.ColorTarget[1]);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, OpenGL->GBufferFramebuffer.ColorTarget[2]);
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, OpenGL->GBufferFramebuffer.ColorTarget[3]);
        glActiveTexture(GL_TEXTURE4);
        glBindTexture(GL_TEXTURE_2D, OpenGL->GBufferFramebuffer.ColorTarget[4]);
        //glActiveTexture(GL_TEXTURE5);
        //glBindTexture(GL_TEXTURE_2D, OpenGL->GBufferFramebuffer.DepthTarget);
        
        glBindBufferBase(GL_UNIFORM_BUFFER, glGetUniformBlockIndex(Shader, "FrameUniforms"), OpenGL->FrameUniformBuffer);
        
        //glBindBuffer(GL_ARRAY_BUFFER, OpenGL->FullscreenVBO);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        
        glBlitNamedFramebuffer(OpenGL->ResolveFramebuffer.ID, 0, 0, 0, OpenGL->ResolveFramebuffer.Width, OpenGL->ResolveFramebuffer.Height,
                               0, 0, OpenGL->RenderWidth, OpenGL->RenderHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR);
    }
}