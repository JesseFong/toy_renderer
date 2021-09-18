/* date = September 14th 2021 9:40 pm */

#ifndef OPENGL_H
#define OPENGL_H


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

struct opengl_texture {
    GLuint Handle;
    u64 ResidentHandle;
    b32 IsResident;
};

struct opengl_cubemap_texture {
    
    
};

#endif //OPENGL_H
