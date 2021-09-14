/* date = August 31st 2021 3:45 pm */

#ifndef GLTF_LOADER_H
#define GLTF_LOADER_H

#define CGLTF_IMPLEMENTATION
#include <cgltf.h>


#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#endif

#define LOADER_MALLOC malloc
#define LOADER_FREE free

#define LOADER_GET_CURRENT_DIRECTORY GetCurrentDirectory
static char GLTFCurrentDirectory[512];


#define S8MIN  ((s8)0x80)
#define S8MAX  ((s8)0x7f)
#define S16MIN ((s32)0x8000)
#define S16MAX ((s32)0x7fff)
#define S32MIN ((s32)0x80000000)
#define S32MAX ((s32)0x7fffffff) 
#define S64MIN ((s64)0x800000000000)
#define S64MAX ((s64)0x7fffffffffff) 

#define U8MAX  ((u8)-1)
#define U16MAX ((u16)-1)
#define U32MAX ((u32)-1)
#define U64MAX ((u64)-1)

#define F32MAX FLT_MAX
#define F32MIN -FLT_MAX

#define F64MAX DBL_MAX
#define F64MIN -DBL_MAX


enum gltf_material_type {
    MATERIAL_TYPE_ALBEDO,
    MATERIAL_TYPE_DIFFUSE,
    MATERIAL_TYPE_NORMAL,
    MATERIAL_TYPE_SPECULAR,
    MATERIAL_TYPE_ROUGHNESS,
    MATERIAL_TYPE_METALLIC,
    MATERIAL_TYPE_OCCLUSION,
    MATERIAL_TYPE_EMISSION,
    MATERIAL_TYPE_COUNT,
};

struct gltf_loader_settings {
    u32 IndicesSize;
    b32 KeepOriginalTransform;
};

static gltf_loader_settings GLOBAL_LOADER_SETTINGS = {};

struct gltf_texture {
    u32 Width;
    u32 Height;
    u32 Channels;
    u8* Data;
};

struct gltf_material_map {
    gltf_texture Texture;
    v4 Color;
    f32 Value;
};

struct gltf_material {
    gltf_material_map Materials[MATERIAL_TYPE_COUNT];
};

struct gltf_mesh {
    v3* P;
    v3* N;
    v3* Tan;
    v3* BiTan;
    v4* Tangents;
    v2* UV;
    
    u32 VertexCount;
    u32 TriangleCount;
    u32* Indices;
    u32 IndexCount;
    
    u32 MaterialIndex;
};

struct gltf_model {
    gltf_mesh* MeshArray;
    u32 MeshCount;
    
    gltf_material* MaterialArray;
    u32 MaterialCount;
};


static gltf_texture
LoadGLTFImageFromFile(char* FileName)
{
    gltf_texture Image = {};
    stbi_set_flip_vertically_on_load(false);
    Image.Data = (u8*)stbi_load(FileName, &(int)Image.Width, &(int)Image.Height, &(int)Image.Channels, 0);
    
#if 0
    if(Image.Data) {
        if(Image.Channels == 4 && PREMULTIPLY_ALPHA) {
            u32 *Pixels = (u32*)Image.Data;
            u32 *Dest = (u32*)Image.Data;
            for(u32 Y = 0;
                Y < Image.Height;
                Y++) {
                
                for(u32 X = 0;
                    X < Image.Width;
                    X++) {
                    
                    u32 Pixel = *Pixels++;
                    
                    v4 Texel = RGBAUnpack4x8(Pixel);
                    Texel = SRGB255ToLinear1(Texel);
                    Texel.rgb *= Texel.a;
                    Texel = Linear1ToSRGB255(Texel);
                    
                    *Dest++ = (((u32)(Texel.a + 0.5f) << 24) |
                               ((u32)(Texel.b + 0.5f) << 16) |
                               ((u32)(Texel.g + 0.5f) << 8) |
                               ((u32)(Texel.r + 0.5f) << 0));
                }
            }
        }
    }
#endif
    return(Image);
}
//
//

inline v3
Reject(v3 A, v3 B) {
    return (A - B * (Inner(A, B) / Inner(B, B)));
}

//
//
static void
GenerateTangents(gltf_mesh* Mesh) {
    
    Mesh->Tan = (v3*)LOADER_MALLOC(Mesh->VertexCount*sizeof(v3));
    Mesh->BiTan = (v3*)LOADER_MALLOC(Mesh->VertexCount*sizeof(v3));
    
    for(u32 i = 0;
        i < Mesh->VertexCount;
        i++) {
        Mesh->Tan[i] = V3(0, 0, 0);
        Mesh->BiTan[i] = V3(0, 0, 0);
    }
    
    for(u32 i = 0;
        i < Mesh->TriangleCount;
        i++) {
        
        u32 TriangleIndex = i*3;
        
        u32 I0 = Mesh->Indices[TriangleIndex+0];
        u32 I1 = Mesh->Indices[TriangleIndex+1];
        u32 I2 = Mesh->Indices[TriangleIndex+2];
        
        v3 P0 = Mesh->P[I0];
        v3 P1 = Mesh->P[I1];
        v3 P2 = Mesh->P[I2];
        
        v2 UV0 = Mesh->UV[I0];
        v2 UV1 = Mesh->UV[I1];
        v2 UV2 = Mesh->UV[I2];
        
        v3 E1 = P1 - P0;
        v3 E2 = P2 - P0;
        
        f32 X1 = UV1.x - UV0.x;
        f32 X2 = UV1.x - UV0.x;
        f32 Y1 = UV1.y - UV0.y;
        f32 Y2 = UV2.y - UV0.y;
        
        f32 R = 1.0f / (X1 * Y2 - X2 * Y1);
        v3 Tan = (E1 * Y2 - E2 * Y1)*R;
        v3 BiTan = (E1 * X1 - E1 * X2)*R;
        
        Mesh->Tan[I0] += Tan;
        Mesh->Tan[I1] += Tan;
        Mesh->Tan[I2] += Tan;
        Mesh->BiTan[I0] += BiTan;
        Mesh->BiTan[I1] += BiTan;
        Mesh->BiTan[I2] += BiTan;
        
    }
    
    Mesh->Tangents = (v4*)LOADER_MALLOC(Mesh->VertexCount*sizeof(v4));
    for(u32 i = 0;
        i < Mesh->VertexCount;
        i++) {
        
        v3 N = Mesh->N[i];
        v3 T = Mesh->Tan[i];
        v3 B = Mesh->BiTan[i];
        
        Mesh->Tangents[i].xyz = Normalize(Reject(T, N));
		Mesh->Tangents[i].w = (Inner(Cross(T, B), N) > 0.0F) ? 1.0F : -1.0F;
    }
}


//
static bool ReadGLTFValue(cgltf_accessor *acc, unsigned int index, void *variable)
{
    unsigned int typeElements = 0;
    
    switch (acc->type)
    {
        case cgltf_type_scalar: typeElements = 1; break;
        case cgltf_type_vec2: typeElements = 2; break;
        case cgltf_type_vec3: typeElements = 3; break;
        case cgltf_type_vec4:
        case cgltf_type_mat2: typeElements = 4; break;
        case cgltf_type_mat3: typeElements = 9; break;
        case cgltf_type_mat4: typeElements = 16; break;
        case cgltf_type_invalid: typeElements = 0; break;
        default: break;
    }
    
    unsigned int typeSize = 0;
    
    switch (acc->component_type)
    {
        case cgltf_component_type_r_8u:
        case cgltf_component_type_r_8: typeSize = 1; break;
        case cgltf_component_type_r_16u:
        case cgltf_component_type_r_16: typeSize = 2; break;
        case cgltf_component_type_r_32f:
        case cgltf_component_type_r_32u: typeSize = 4; break;
        case cgltf_component_type_invalid: typeSize = 0; break;
        default: break;
    }
    
    unsigned int singleElementSize = typeSize*typeElements;
    
    if (acc->count == 2)
    {
        if (index > 1) return false;
        
        memcpy(variable, index == 0 ? acc->min : acc->max, singleElementSize);
        return true;
    }
    
    memset(variable, 0, singleElementSize);
    
    if (acc->buffer_view == NULL || acc->buffer_view->buffer == NULL || acc->buffer_view->buffer->data == NULL) return false;
    
    if (!acc->buffer_view->stride)
    {
        void *readPosition = ((char *)acc->buffer_view->buffer->data) + (index*singleElementSize) + acc->buffer_view->offset + acc->offset;
        memcpy(variable, readPosition, singleElementSize);
    }
    else
    {
        void *readPosition = ((char *)acc->buffer_view->buffer->data) + (index*acc->buffer_view->stride) + acc->buffer_view->offset + acc->offset;
        memcpy(variable, readPosition, singleElementSize);
    }
    
    return true;
}


static const unsigned char base64Table[] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 62, 0, 0, 0, 63, 52, 53,
    54, 55, 56, 57, 58, 59, 60, 61, 0, 0,
    0, 0, 0, 0, 0, 0, 1, 2, 3, 4,
    5, 6, 7, 8, 9, 10, 11, 12, 13, 14,
    15, 16, 17, 18, 19, 20, 21, 22, 23, 24,
    25, 0, 0, 0, 0, 0, 0, 26, 27, 28,
    29, 30, 31, 32, 33, 34, 35, 36, 37, 38,
    39, 40, 41, 42, 43, 44, 45, 46, 47, 48,
    49, 50, 51
};

static int GetSizeBase64(char *input)
{
    int size = 0;
    
    for (int i = 0; input[4*i] != 0; i++)
    {
        if (input[4*i + 3] == '=')
        {
            if (input[4*i + 2] == '=') size += 1;
            else size += 2;
        }
        else size += 3;
    }
    
    return size;
}

//Stolen From Raylib
//https://github.com/raysan5/raylib/blob/master/src/models.c
static unsigned char*
DecodeBase64(char *input, int *size)
{
    *size = GetSizeBase64(input);
    
    unsigned char *buf = (unsigned char *)LOADER_MALLOC(*size);
    for (int i = 0; i < *size/3; i++)
    {
        unsigned char a = base64Table[(int)input[4*i]];
        unsigned char b = base64Table[(int)input[4*i + 1]];
        unsigned char c = base64Table[(int)input[4*i + 2]];
        unsigned char d = base64Table[(int)input[4*i + 3]];
        
        buf[3*i] = (a << 2) | (b >> 4);
        buf[3*i + 1] = (b << 4) | (c >> 2);
        buf[3*i + 2] = (c << 6) | d;
    }
    
    if (*size%3 == 1)
    {
        int n = *size/3;
        unsigned char a = base64Table[(int)input[4*n]];
        unsigned char b = base64Table[(int)input[4*n + 1]];
        buf[*size - 1] = (a << 2) | (b >> 4);
    }
    else if (*size%3 == 2)
    {
        int n = *size/3;
        unsigned char a = base64Table[(int)input[4*n]];
        unsigned char b = base64Table[(int)input[4*n + 1]];
        unsigned char c = base64Table[(int)input[4*n + 2]];
        buf[*size - 2] = (a << 2) | (b >> 4);
        buf[*size - 1] = (b << 4) | (c >> 2);
    }
    return buf;
}

static void*
WriteGLTFFloatsToBuffer(cgltf_accessor* Accessor, cgltf_component_type Type) {
    u32 Count = (u32)Accessor->count;
    u32 TypeSize = 0;
    u32 TypeElements = 0;
    
    TypeSize = (u32)cgltf_component_size(Type);
    TypeElements = (u32)cgltf_num_components(Accessor->type);
    u32 Size = (u32)cgltf_calc_size(Accessor->type, Type);
    
    if(Accessor->component_type != Type) {
        Assert(!"Handle This");
    }
    
    cgltf_float* Result = (cgltf_float*)LOADER_MALLOC(Count*Size);
    u32 TotalFloatsRead = (u32)cgltf_accessor_unpack_floats(Accessor, Result, Count*TypeElements);
    return (void*)Result;
}

static void*
WriteGLTFUintsToBuffer(cgltf_accessor* Accessor, cgltf_component_type Type) {
    u32 Count = (u32)Accessor->count;
    u32 TypeSize = 0;
    u32 TypeElements = 0;
    
    TypeSize = (u32)cgltf_component_size(Type);
    TypeElements = (u32)cgltf_num_components(Accessor->type);
    u32 Size = (u32)cgltf_calc_size(Accessor->type, Type);
    
    void* Result = LOADER_MALLOC(Count*Size);;
    
    if(Accessor->component_type == cgltf_component_type_r_32f) {
        Assert(!"Handle This");
    }
    
    switch(Type) {
        
        case cgltf_component_type_r_32u: {
            u32* TypedDestArray = (u32*)Result;
            for (u32 i = 0; i < Count*TypeElements; i++) TypedDestArray[i] = (u32)cgltf_accessor_read_index(Accessor, i);
        } break;
    }
    return Result;
}

static gltf_texture
LoadGLTFImage(cgltf_image* Image) {
    
    gltf_texture Result = {};
    if(Image->uri) {
        
        if ((strlen(Image->uri) > 5) &&
            (Image->uri[0] == 'd') &&
            (Image->uri[1] == 'a') &&
            (Image->uri[2] == 't') &&
            (Image->uri[3] == 'a') &&
            (Image->uri[4] == ':')) {
            
            int I = 0;
            while ((Image->uri[I] != ',') && (Image->uri[I] != 0)) I++;
            
            if(Image->uri[I] == 0); //TODO(Jesse): Logging 
            else {
                int Size = 0;
                u8* Filename = (u8*)DecodeBase64(Image->uri + I + 1, &Size);
                
                int Width, Height, Channels;
                Result.Data = stbi_load_from_memory(Filename, Size, &Width, &Height, &Channels, 0);
                Result.Width = Width;
                Result.Height = Height;
                Result.Channels = Channels;
            }
            
        } else {
            Result = LoadGLTFImageFromFile(FormatString("%s/%s", GLTFCurrentDirectory, Image->uri));
        }
    }
    return Result;
}

static void
LoadGLTFMaterials(cgltf_data* Data, gltf_model* OutModel) {
    for(u32 MaterialIndex = 0;
        MaterialIndex < Data->materials_count;
        MaterialIndex++) {
        
        cgltf_material* Material = &Data->materials[MaterialIndex];
        gltf_material* OutMaterial = &OutModel->MaterialArray[MaterialIndex];
        
        *OutMaterial = {};
        
        if(Material->has_pbr_metallic_roughness) {
            
            cgltf_pbr_metallic_roughness* In = &Material->pbr_metallic_roughness;
            
            f32 TintR = In->base_color_factor[0];
            f32 TintG = In->base_color_factor[1];
            f32 TintB = In->base_color_factor[2];
            f32 TintA = In->base_color_factor[3];
            
            v4 BaseColor = V4(TintR, TintG, TintB, TintA);
            
            OutMaterial->Materials[MATERIAL_TYPE_ALBEDO].Color = BaseColor;
            
            if(In->base_color_texture.texture) {
                OutMaterial->Materials[MATERIAL_TYPE_ALBEDO].Texture = LoadGLTFImage(In->base_color_texture.texture->image);
            }
            
            BaseColor = V4(1, 1, 1, 1);
            
            if(In->metallic_roughness_texture.texture) {
                OutMaterial->Materials[MATERIAL_TYPE_ROUGHNESS].Texture = LoadGLTFImage(In->metallic_roughness_texture.texture->image);
                OutMaterial->Materials[MATERIAL_TYPE_ROUGHNESS].Value = In->roughness_factor;
                OutMaterial->Materials[MATERIAL_TYPE_METALLIC].Value = In->metallic_factor;
            }
        }
        
        if(Material->has_pbr_specular_glossiness) {
            
            cgltf_pbr_specular_glossiness* In = &Material->pbr_specular_glossiness;
            
            f32 TintR = In->diffuse_factor[0];
            f32 TintG = In->diffuse_factor[1];
            f32 TintB = In->diffuse_factor[2];
            f32 TintA = In->diffuse_factor[3];
            
            v4 BaseColor = V4(TintR, TintG, TintB, TintA);
            
            OutMaterial->Materials[MATERIAL_TYPE_DIFFUSE].Color = BaseColor;
            
            if(In->diffuse_texture.texture) {
                OutMaterial->Materials[MATERIAL_TYPE_DIFFUSE].Texture = LoadGLTFImage(In->diffuse_texture.texture->image);
            }
            
            BaseColor = V4(1, 1, 1, 1);
            
            if(In->specular_glossiness_texture.texture) {
                OutMaterial->Materials[MATERIAL_TYPE_SPECULAR].Texture = LoadGLTFImage(In->specular_glossiness_texture.texture->image);
                
                v4 SpecularGloss = {};
                SpecularGloss.r = In->specular_factor[0];
                SpecularGloss.g = In->specular_factor[1];
                SpecularGloss.b = In->specular_factor[2];
                SpecularGloss.r = In->glossiness_factor;
                
                OutMaterial->Materials[MATERIAL_TYPE_SPECULAR].Color = SpecularGloss;
            }
        }
        
        if(Material->normal_texture.texture) {
            OutMaterial->Materials[MATERIAL_TYPE_NORMAL].Texture = LoadGLTFImage(Material->normal_texture.texture->image);
        }
        
        if(Material->occlusion_texture.texture) {
            OutMaterial->Materials[MATERIAL_TYPE_OCCLUSION].Texture = LoadGLTFImage(Material->occlusion_texture.texture->image);
        }
        
        if(Material->emissive_texture.texture) {
            OutMaterial->Materials[MATERIAL_TYPE_EMISSION].Texture = LoadGLTFImage(Material->emissive_texture.texture->image);
            
            v4 EmissiveColor = {};
            EmissiveColor.r = Material->emissive_factor[0];
            EmissiveColor.g = Material->emissive_factor[1];
            EmissiveColor.b = Material->emissive_factor[2];
            EmissiveColor.a = 1;
            OutMaterial->Materials[MATERIAL_TYPE_EMISSION].Color = EmissiveColor;
        }
    }
}

static void
LoadGLTFMesh(cgltf_data* Data, gltf_model* OutModel, cgltf_mesh* Mesh, f32* NodeMatrixTransform, u32* PrimitiveIndex, char* Filename) {
    
    for(u32 P = 0; P < Mesh->primitives_count; P++) {
        
        gltf_mesh* WritingMesh = &OutModel->MeshArray[*PrimitiveIndex];
        
        for(u32 J = 0; J < Mesh->primitives[P].attributes_count; J++) {
            
            u32 AttributeType = Mesh->primitives[P].attributes[J].type;
            
            if(AttributeType == cgltf_attribute_type_position) {
                cgltf_accessor* Accessor = Mesh->primitives[P].attributes[J].data;
                WritingMesh->VertexCount = (u32)Accessor->count;
                
                //TODO(Jesse): Transform vertices by the node matrix
                
                WritingMesh->P = (v3*)WriteGLTFFloatsToBuffer(Accessor, cgltf_component_type_r_32f);
                
                for(u32 I = 0;
                    I < WritingMesh->VertexCount;
                    I++) {
                    WritingMesh->P[I] = *((m4x4*)NodeMatrixTransform)*WritingMesh->P[I];
                }
                
            } else if(AttributeType == cgltf_attribute_type_normal) {
                cgltf_accessor* Accessor = Mesh->primitives[P].attributes[J].data;
                u32 BufferSize = (u32)(Accessor->count*3*sizeof(f32));
                
                WritingMesh->N = (v3*)WriteGLTFFloatsToBuffer(Accessor, cgltf_component_type_r_32f);
                
            } else if(AttributeType == cgltf_attribute_type_texcoord) {
                cgltf_accessor* Accessor = Mesh->primitives[P].attributes[J].data;
                WritingMesh->UV = (v2*)WriteGLTFFloatsToBuffer(Accessor, cgltf_component_type_r_32f);
            }
            
        }
        
        cgltf_accessor* Accessor = Mesh->primitives[P].indices;
        if(Accessor) {
            WritingMesh->TriangleCount = (u32)(Accessor->count/3);
            WritingMesh->IndexCount = (u32)Accessor->count;
            WritingMesh->Indices = (u32*)WriteGLTFUintsToBuffer(Accessor, cgltf_component_type_r_32u);
            //TODO(Jesse): Check if the component is u8 or u32 as well and maybe truncate?
        } else {
            //Unindexed;
            WritingMesh->TriangleCount = WritingMesh->VertexCount/3;
            WritingMesh->Indices = (u32*)LOADER_MALLOC(sizeof(u32)*WritingMesh->VertexCount);
            
            for(u32 i = 0;
                i < WritingMesh->VertexCount;
                i++) {
                WritingMesh->Indices[i] = i;
            }
        }
        
        GenerateTangents(WritingMesh);
        
        if(Mesh->primitives[P].material) {
            
            WritingMesh->MaterialIndex = (u32)(Mesh->primitives[P].material - Data->materials);
        }
        
        
        (*PrimitiveIndex)++;
    }
}


static void
GetGLTFPrimitiveCount(cgltf_node* Node, u32* OutCount) {
    if(Node->mesh) {
        *OutCount += (u32)Node->mesh->primitives_count;
    }
    
    for(u32 ChildIndex = 0;
        ChildIndex < Node->children_count;
        ChildIndex++) {
        
        GetGLTFPrimitiveCount(Node->children[ChildIndex], OutCount);
    }
}

static void
LoadGLTFNode(cgltf_data* Data, gltf_model* OutModel, cgltf_node* Node, u32* PrimitiveIndex, char* Filename) {
    
    cgltf_float WorldMatrix[16] = {};
    cgltf_node_transform_world(Node, WorldMatrix);
    
    if(Node->mesh) {
        LoadGLTFMesh(Data, OutModel, Node->mesh, (f32*)WorldMatrix, PrimitiveIndex, Filename);
    }
    
    for(u32 ChildIndex = 0;
        ChildIndex < Node->children_count;
        ChildIndex++) {
        
        LoadGLTFNode(Data, OutModel, Node->children[ChildIndex], PrimitiveIndex, Filename);
    }
}


static gltf_model
LoadGLTF(gltf_loader_settings* Settings = NULL) {
    
    if(Settings) {
        GLOBAL_LOADER_SETTINGS = *Settings;
    }
    
    //LOADER_GET_CURRENT_DIRECTORY(512, &GLTFCurrentDirectory[0]);
    
    char* SponzaDirectory = "GLTFSamples/Sponza/glTF";
    
    strcpy(GLTFCurrentDirectory, SponzaDirectory);
    
    gltf_model Result = {};
    char* Filename = "GLTFSamples/Sponza/glTF/Sponza.gltf";
    cgltf_options options = {0};
    cgltf_data* data = NULL;
    cgltf_result result = cgltf_parse_file(&options, Filename, &data);
    
    if (result == cgltf_result_success)
    {
        
        result = cgltf_load_buffers(&options, data, Filename);
        
        u32 PrimitivesCount = 0;
        for(u32 i = 0; i < data->scene->nodes_count; i++) {
            
            GetGLTFPrimitiveCount(data->scene->nodes[i], &PrimitivesCount);
        }
        
        Result.MeshCount = PrimitivesCount;
        Result.MeshArray = (gltf_mesh*)LOADER_MALLOC(Result.MeshCount*sizeof(gltf_mesh));
        
        Result.MaterialCount = (u32)data->materials_count;
        Result.MaterialArray = (gltf_material*)LOADER_MALLOC(Result.MaterialCount*sizeof(gltf_material));
        LoadGLTFMaterials(data, &Result);
        
        u32 PrimitiveIndex = 0;
        for(u32 i = 0; i < data->scene->nodes_count; i++) {
            LoadGLTFNode(data, &Result, data->scene->nodes[i], &PrimitiveIndex, Filename);
        }
        
        /* TODO make awesome stuff */
        cgltf_free(data);
    }
    return Result;
}
#endif //GLTF_LOADER_H
