
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

GLuint DrawUniformBuffer;
per_draw_uniforms* DrawUniformArray;
framebuffer GBufferFramebuffer;

GLuint FrameUniformBuffer;
frame_uniforms FrameUniforms;

framebuffer ResolveFramebuffer;

framebuffer ShadowmapFramebuffer;

shader GBufferShader = {};
shader ResolveShader = {};
shader ShadowmapShader = {};
shader SkyboxShader = {};


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
    
    Memory->Scene.SkyboxID = RegisterCubemap(DataArray, SkyboxImages[0].Width, SkyboxImages[0].Height, GL_RGB8);
    
    stbi_set_flip_vertically_on_load(false);
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
    
    
    generated_mesh SphereMesh = GenerateSphere(1);
    Memory->Scene.GeneratedSphereID = RegisterGeneratedMesh(&SphereMesh);
    generated_mesh CubeMesh = GenerateTessellatedCube(1);
    Memory->Scene.GeneratedCubeID = RegisterGeneratedMesh(&CubeMesh);
    
    ShadowmapFramebuffer = CreateFramebuffer(OpenGL->RenderWidth, OpenGL->RenderHeight);
    AddFramebufferDepthTexture(&ShadowmapFramebuffer, GL_DEPTH_COMPONENT16);
    EndFramebufferCreation(&ShadowmapFramebuffer);
    
    
    Scene->IsInitialized = true;
}


static void
UpdateAndRender(app_memory* Memory) {
    
    if(!Memory->Scene.IsInitialized) {
        LoadModel(Memory);
        LoadSkybox(Memory);
        InitializeScene(Memory);
        
        GBufferShader = LoadShader(Memory, "gl_shaders/shader.vert", "gl_shaders/shader.frag", 0);
        ResolveShader = LoadShader(Memory, "gl_shaders/resolve.vert", "gl_shaders/resolve.frag", 0);
        ShadowmapShader = LoadShader(Memory, "gl_shaders/shadowmap.vert", "gl_shaders/shadowmap.frag", 0);
        SkyboxShader = LoadShader(Memory, "gl_shaders/skybox.vert", "gl_shaders/skybox.frag", 0);
        
    }
    
    MaybeReloadShader(Memory, &GBufferShader, "gl_shaders/shader.vert", "gl_shaders/shader.frag", 0);
    MaybeReloadShader(Memory, &ResolveShader, "gl_shaders/resolve.vert", "gl_shaders/resolve.frag", 0);
    MaybeReloadShader(Memory, &ShadowmapShader, "gl_shaders/shadowmap.vert", "gl_shaders/shadowmap.frag", 0);
    MaybeReloadShader(Memory, &SkyboxShader, "gl_shaders/skybox.vert", "gl_shaders/skybox.frag", 0);
    
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
        
        u32 MeshID = Memory->Scene.GeneratedSphereID;
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
            
            u32 MeshID = Memory->Scene.GeneratedSphereID;
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
        GLuint Shader = ShadowmapShader.ID;
        glUseProgram(Shader);
        
        glBindFramebuffer(GL_FRAMEBUFFER, ShadowmapFramebuffer.ID);
        glViewport(0, 0, OpenGL->RenderWidth, OpenGL->RenderHeight);
        glScissor(0, 0, OpenGL->RenderWidth, OpenGL->RenderHeight);
        glClear(GL_DEPTH_BUFFER_BIT);
        
        glBindVertexArray(OpenGL->ModelGeometryInfo.VAO);
        
        glBindBufferBase(GL_UNIFORM_BUFFER, glGetUniformBlockIndex(Shader, "FrameUniforms"), FrameUniformBuffer);
        
        u32 TotalDrawCount = OpenGL->IndirectCommandsCount;
        //glNamedBufferSubData(DrawUniformBuffer, 0, sizeof(per_draw_uniforms)*TotalDrawCount, DrawUniformArray);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, glGetProgramResourceIndex(Shader, GL_SHADER_STORAGE_BLOCK, "DrawUniforms"), DrawUniformBuffer);
        
        glBufferData(GL_DRAW_INDIRECT_BUFFER, TotalDrawCount*sizeof(draw_element_indirect_command), OpenGL->IndirectCommands, GL_DYNAMIC_DRAW);
        glMultiDrawElementsIndirect(GL_TRIANGLES, GL_UNSIGNED_INT, NULL, TotalDrawCount, 0);
    }
    
    {
        GLuint Shader = GBufferShader.ID;
        
        glUseProgram(Shader);
        
        glBindFramebuffer(GL_FRAMEBUFFER, GBufferFramebuffer.ID);
        glViewport(0, 0, OpenGL->RenderWidth, OpenGL->RenderHeight);
        glScissor(0, 0, OpenGL->RenderWidth, OpenGL->RenderHeight);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        
        glBindVertexArray(OpenGL->ModelGeometryInfo.VAO);
        
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, ShadowmapFramebuffer.DepthTarget);
        
        glBindBufferBase(GL_UNIFORM_BUFFER, glGetUniformBlockIndex(Shader, "TextureArray"), OpenGL->TextureBuffer);
        glBindBuffer(GL_DRAW_INDIRECT_BUFFER, OpenGL->IndirectCommandBuffer);
        
        glBindBufferBase(GL_UNIFORM_BUFFER, glGetUniformBlockIndex(Shader, "FrameUniforms"), FrameUniformBuffer);
        
        u32 TotalDrawCount = OpenGL->IndirectCommandsCount;
        
        glNamedBufferSubData(DrawUniformBuffer, 0, sizeof(per_draw_uniforms)*TotalDrawCount, DrawUniformArray);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, glGetProgramResourceIndex(Shader, GL_SHADER_STORAGE_BLOCK, "DrawUniforms"), DrawUniformBuffer);
        
        glBufferData(GL_DRAW_INDIRECT_BUFFER, TotalDrawCount*sizeof(draw_element_indirect_command), OpenGL->IndirectCommands, GL_DYNAMIC_DRAW);
        glMultiDrawElementsIndirect(GL_TRIANGLES, GL_UNSIGNED_INT, NULL, TotalDrawCount, 0);
    }
    
    
    {
        
        
        glBindFramebuffer(GL_FRAMEBUFFER, ResolveFramebuffer.ID);
        glViewport(0, 0, OpenGL->RenderWidth, OpenGL->RenderHeight);
        glScissor(0, 0, OpenGL->RenderWidth, OpenGL->RenderHeight);
        
        GLuint Shader = SkyboxShader.ID;
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
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glEnable(GL_CULL_FACE);
        
    }
    
    
    {
        GLuint Shader = ResolveShader.ID;
        
        glUseProgram(Shader);
        glBindVertexArray(OpenGL->FullscreenVAO);
        glBindFramebuffer(GL_FRAMEBUFFER, ResolveFramebuffer.ID);
        
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, GBufferFramebuffer.ColorTarget[0]);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, GBufferFramebuffer.ColorTarget[1]);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, GBufferFramebuffer.ColorTarget[2]);
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, GBufferFramebuffer.ColorTarget[4]);
        glActiveTexture(GL_TEXTURE4);
        glBindTexture(GL_TEXTURE_2D, GBufferFramebuffer.DepthTarget);
        glActiveTexture(GL_TEXTURE5);
        glBindTexture(GL_TEXTURE_2D, GBufferFramebuffer.ColorTarget[3]);
        
        glBindBufferBase(GL_UNIFORM_BUFFER, glGetUniformBlockIndex(Shader, "FrameUniforms"), FrameUniformBuffer);
        
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        
        glBlitNamedFramebuffer(ResolveFramebuffer.ID, 0, 0, 0, ResolveFramebuffer.Width, ResolveFramebuffer.Height,
                               0, 0, OpenGL->RenderWidth, OpenGL->RenderHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR);
    }
}