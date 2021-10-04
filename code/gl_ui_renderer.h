/* date = September 24th 2021 7:32 pm */

#ifndef GL_UI_RENDERER_H
#define GL_UI_RENDERER_H

static GLuint GLOBALUIVAO;
static GLuint GLOBALUIVBO;

static GLuint GLOBALUIShader;
static GLuint GLOBALLocationTex;
static GLuint GLOBALLocationM;
static GLuint GLOBALLocationP;
static GLuint GLOBALLocationUV;
static GLuint GLOBALLocationColor;

static GLuint GLOBALWhiteTextureID;
static GLuint GLOBALFontTextureID;

static asset_font_atlas* GLOBALUIFontAtlas;

struct ui_vertex {
    v3 P;
    v2 UV;
    v4 Color;
};

static rectangle2
RenderUITextFromAtlas(f32 X, f32 Y, f32 ZValue, char* Text, f32 PixelSize, v4 Color = V4(1, 1, 1, 1), b32 SizeText = 0) {
    
    X = Floor(X);
    Y = Floor(Y);
    
    asset_font_atlas* Atlas = GLOBALUIFontAtlas;
    
    rectangle2 TotalRect = InvertedInfinityRectangle2();
    f32 Scale = PixelSize/(f32)Atlas->OriginalSize;
    
    glBindTexture(GL_TEXTURE_2D, GLOBALFontTextureID);
    while(*Text) {
        
        font_char_info* Info = &Atlas->CharInfos[*Text - Atlas->UnicodeStart];
        
        v2 PixelMinP = V2(X + Info->XOffsetMin*Scale, Y + Info->YOffsetMin*Scale);
        v2 PixelMaxP = V2(X + Info->XOffsetMax*Scale, Y + Info->YOffsetMax*Scale);
        
        rectangle2 QuadRect = RectMinMax(PixelMinP, PixelMaxP);
        TotalRect = Union(TotalRect, QuadRect);
        
        if(!SizeText) {
            v3 P0 = V3(PixelMinP.x, PixelMinP.y, ZValue);
            v3 P1 = V3(PixelMinP.x, PixelMaxP.y, ZValue);
            v3 P2 = V3(PixelMaxP.x, PixelMaxP.y, ZValue);
            v3 P3 = V3(PixelMaxP.x, PixelMinP.y, ZValue);
            
            ui_vertex Vertex[6];
            Vertex[0].P = P0;
            Vertex[1].P = P1;
            Vertex[2].P = P3;
            
            Vertex[3].P = P1;
            Vertex[4].P = P2;
            Vertex[5].P = P3; 
            
            v2 UVMin = Info->UVMin;
            v2 UVMax = Info->UVMax;
            
            Vertex[0].UV = UVMin;
            Vertex[1].UV = V2(UVMin.x, UVMax.y);
            Vertex[2].UV = V2(UVMax.x, UVMin.y);
            
            Vertex[3].UV = V2(UVMin.x, UVMax.y);
            Vertex[4].UV = UVMax;
            Vertex[5].UV = V2(UVMax.x, UVMin.y);
            
            Vertex[0].Color = Color;
            Vertex[1].Color = Color;
            Vertex[2].Color = Color;
            Vertex[3].Color = Color;
            Vertex[4].Color = Color;
            Vertex[5].Color = Color;
            
            
            glBufferData(GL_ARRAY_BUFFER, sizeof(ui_vertex)*6, (GLvoid*)&Vertex, GL_STREAM_DRAW);
            glDrawArrays(GL_TRIANGLES, 0, 6);
        }
        
        X += Info->XAdvance*Scale;
        Text++;
    }
    glBindTexture(GL_TEXTURE_2D, 0);
    return TotalRect;
}

rectangle2
SizeText(char* Text, float X, float Y, float Size) {
    return RenderUITextFromAtlas(X, Y, 0, Text, Size, V4(1, 1, 1, 1), 1);
}

static void
DrawUI(ui_state* UI) {
    
    GLint last_program; glGetIntegerv(GL_CURRENT_PROGRAM, &last_program);
    GLint last_texture; glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture);
    GLint last_active_texture; glGetIntegerv(GL_ACTIVE_TEXTURE, &last_active_texture);
    GLint last_array_buffer; glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &last_array_buffer);
    GLint last_element_array_buffer; glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &last_element_array_buffer);
    GLint last_vertex_array; glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &last_vertex_array);
    GLint last_blend_src; glGetIntegerv(GL_BLEND_SRC, &last_blend_src);
    GLint last_blend_dst; glGetIntegerv(GL_BLEND_DST, &last_blend_dst);
    GLint last_blend_equation_rgb; glGetIntegerv(GL_BLEND_EQUATION_RGB, &last_blend_equation_rgb);
    GLint last_blend_equation_alpha; glGetIntegerv(GL_BLEND_EQUATION_ALPHA, &last_blend_equation_alpha);
    GLint last_viewport[4]; glGetIntegerv(GL_VIEWPORT, last_viewport);
    GLboolean last_enable_blend = glIsEnabled(GL_BLEND);
    GLboolean last_enable_cull_face = glIsEnabled(GL_CULL_FACE);
    GLboolean last_enable_depth_test = glIsEnabled(GL_DEPTH_TEST);
    GLboolean last_enable_scissor_test = glIsEnabled(GL_SCISSOR_TEST);
    
    glEnable(GL_BLEND);
    glBlendEquation(GL_FUNC_ADD);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_SCISSOR_TEST);
    
    glBindVertexArray(GLOBALUIVAO);
    glBindBuffer(GL_ARRAY_BUFFER, GLOBALUIVBO);
    glUseProgram(GLOBALUIShader);
    
    
    glActiveTexture(GL_TEXTURE0);
    
    f32 DrawWidth = UI->DrawRegionDimInPixels.x;
    f32 DrawHeight = UI->DrawRegionDimInPixels.y;
    
    // Setup viewport, orthographic projection matrix
    glViewport(0, 0, (GLsizei)UI->DrawRegionDimInPixels.x, (GLsizei)UI->DrawRegionDimInPixels.y);
    glScissor(0, 0, (GLsizei)UI->DrawRegionDimInPixels.x, (GLsizei)UI->DrawRegionDimInPixels.y);
    
    m4x4 Proj = OrthographicProjection(V2(0, DrawWidth), V2(0, DrawHeight), V2(-1000, 1000)).Forward;
    glUniform1i(GLOBALLocationTex, 0);
    glUniformMatrix4fv(GLOBALLocationM, 1, GL_TRUE, (GLfloat*)&Proj);
    
    ui_render_array* RenderList = UIEndFrame(UI);
    
    for(u32 RenderIndex = 0;
        RenderIndex < RenderList->RenderElementCount;
        RenderIndex++) {
        
        ui_render_element El = RenderList->RenderElements[RenderIndex];
        
        switch(El.Type) {
            
            case(UI_RENDER_QUAD): {
                v3 P0 = V3(El.Quad.P0, El.ZValue);
                v3 P1 = V3(El.Quad.P1, El.ZValue);
                v3 P2 = V3(El.Quad.P2, El.ZValue);
                v3 P3 = V3(El.Quad.P3, El.ZValue);
                
                ui_vertex Vertex[6];
                Vertex[0].P = P2;
                Vertex[1].P = P3;
                Vertex[2].P = P1;
                
                Vertex[3].P = P3;
                Vertex[4].P = P0;
                Vertex[5].P = P1; 
                
                Vertex[0].UV = V2(0, 0);
                Vertex[1].UV = V2(0, 1);
                Vertex[2].UV = V2(1, 0);
                
                Vertex[3].UV = V2(0, 1);
                Vertex[4].UV = V2(1, 1);
                Vertex[5].UV = V2(1, 0);
                
                Vertex[0].Color = El.Color;
                Vertex[1].Color = El.Color;
                Vertex[2].Color = El.Color;
                Vertex[3].Color = El.Color;
                Vertex[4].Color = El.Color;
                Vertex[5].Color = El.Color;
                
                
                glBindTexture(GL_TEXTURE_2D, GLOBALWhiteTextureID);
                glBufferData(GL_ARRAY_BUFFER, sizeof(ui_vertex)*6, (GLvoid*)&Vertex, GL_STREAM_DRAW);
                glDrawArrays(GL_TRIANGLES, 0, 6);
            } break;
            
            case(UI_RENDER_TEXT): {
                RenderUITextFromAtlas(El.Text.At.x, El.Text.At.y, El.ZValue, El.Text.Text, El.Text.Size, El.Color);
            } break;
            
        }
        
    }
    
    
    // Restore modified GL state
    glUseProgram(last_program);
    glActiveTexture(last_active_texture);
    glBindTexture(GL_TEXTURE_2D, last_texture);
    glBindVertexArray(last_vertex_array);
    glBindBuffer(GL_ARRAY_BUFFER, last_array_buffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, last_element_array_buffer);
    glBlendEquationSeparate(last_blend_equation_rgb, last_blend_equation_alpha);
    glBlendFunc(last_blend_src, last_blend_dst);
    if (last_enable_blend) glEnable(GL_BLEND); else glDisable(GL_BLEND);
    if (last_enable_cull_face) glEnable(GL_CULL_FACE); else glDisable(GL_CULL_FACE);
    if (last_enable_depth_test) glEnable(GL_DEPTH_TEST); else glDisable(GL_DEPTH_TEST);
    if (last_enable_scissor_test) glEnable(GL_SCISSOR_TEST); else glDisable(GL_SCISSOR_TEST);
    glViewport(last_viewport[0], last_viewport[1], (GLsizei)last_viewport[2], (GLsizei)last_viewport[3]);
    
}

static void
InitUIRenderer() {
    
    Assert(GLOBALUIFontAtlas);
    
    GLint Viewport[4];
    glGetIntegerv(GL_VIEWPORT, Viewport);
    
    GLint LastTextureID = 0;
    glGetIntegerv(GL_TEXTURE_BINDING_2D, &LastTextureID);
    
    glGenTextures(1, &GLOBALFontTextureID);
    glBindTexture(GL_TEXTURE_2D, GLOBALFontTextureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, GLOBALUIFontAtlas->Bitmap.Width, GLOBALUIFontAtlas->Bitmap.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, GLOBALUIFontAtlas->Bitmap.Data);
    
    
    u8 WhiteBitmapData[4];
    WhiteBitmapData[0] = 255;
    WhiteBitmapData[1] = 255;
    WhiteBitmapData[2] = 255;
    WhiteBitmapData[3] = 255; 
    
    glGenTextures(1, &GLOBALWhiteTextureID);
    glBindTexture(GL_TEXTURE_2D, GLOBALWhiteTextureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, WhiteBitmapData);
    
    glBindTexture(GL_TEXTURE_2D, LastTextureID);
    
    
    const GLchar* VertexSource =
        "#version 330\n"
        "uniform mat4 ProjMtx;\n"
        "in vec3 Position;\n"
        "in vec2 UV;\n"
        "in vec4 Color;\n"
        "out vec2 Frag_UV;\n"
        "out vec4 Frag_Color;\n"
        "void main()\n"
        "{\n"
        "	Frag_UV = UV;\n"
        "	Frag_Color = Color;\n"
        "	gl_Position = ProjMtx * vec4(Position.xyz,1);\n"
        "}\n";
    
    const GLchar* FragmentSource =
        "#version 330\n"
        "uniform sampler2D Texture;\n"
        "in vec2 Frag_UV;\n"
        "in vec4 Frag_Color;\n"
        "out vec4 Out_Color;\n"
        "void main()\n"
        "{\n"
        "	Out_Color = Frag_Color * texture( Texture, Frag_UV.st);\n"
        "}\n";
    
    GLOBALUIShader = glCreateProgram();
    
    GLuint VertHandle = glCreateShader(GL_VERTEX_SHADER);
    GLuint FragHandle = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(VertHandle, 1, &VertexSource, 0);
    glShaderSource(FragHandle, 1, &FragmentSource, 0);
    glCompileShader(VertHandle);
    glCompileShader(FragHandle);
    glAttachShader(GLOBALUIShader, VertHandle);
    glAttachShader(GLOBALUIShader, FragHandle);
    glLinkProgram(GLOBALUIShader);
    
    GLOBALLocationTex = glGetUniformLocation(GLOBALUIShader, "Texture");
    GLOBALLocationM = glGetUniformLocation(GLOBALUIShader, "ProjMtx");
    GLOBALLocationP = glGetAttribLocation(GLOBALUIShader, "Position");
    GLOBALLocationUV = glGetAttribLocation(GLOBALUIShader, "UV");
    GLOBALLocationColor = glGetAttribLocation(GLOBALUIShader, "Color");
    
    
    GLint LastTexture;
    GLint LastVBO;
    GLint LastVAO;
    
    glGetIntegerv(GL_TEXTURE_BINDING_2D, &LastTexture);
    glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &LastVBO);
    glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &LastVAO);
    
    
    glGenVertexArrays(1, &GLOBALUIVAO);
    glBindVertexArray(GLOBALUIVAO);
    
    glGenBuffers(1, &GLOBALUIVBO);
    glBindBuffer(GL_ARRAY_BUFFER, GLOBALUIVBO);
    glEnableVertexAttribArray(GLOBALLocationP);
    glEnableVertexAttribArray(GLOBALLocationUV);
    glEnableVertexAttribArray(GLOBALLocationColor);
    
    
#define OFFSETOF(TYPE, ELEMENT) ((size_t)&(((TYPE *)0)->ELEMENT))
    glVertexAttribPointer(GLOBALLocationP, 3, GL_FLOAT, GL_FALSE, sizeof(ui_vertex), (GLvoid*)OFFSETOF(ui_vertex, P));
    glVertexAttribPointer(GLOBALLocationUV, 2, GL_FLOAT, GL_FALSE, sizeof(ui_vertex), (GLvoid*)OFFSETOF(ui_vertex, UV));
    glVertexAttribPointer(GLOBALLocationColor, 4, GL_FLOAT, GL_TRUE, sizeof(ui_vertex), (GLvoid*)OFFSETOF(ui_vertex, Color));
#undef OFFSETOF
    
    glBindTexture(GL_TEXTURE_2D, LastTexture);
    glBindBuffer(GL_ARRAY_BUFFER, LastVBO);
    glBindVertexArray(LastVAO);
}


#endif //GL_UI_RENDERER_H
