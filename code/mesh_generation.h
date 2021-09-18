/* date = February 16th 2021 2:32 pm */
#ifndef MESH_GENERATION_H
#define MESH_GENERATION_H


/*
-  *THIS FILE USES OPENGL COORDINATES*
-
 -      (+Y)
  -        |
-        |
-        |
-        +-----(+X) 
-       / 
-      /
-   (+Z)
-    
*/ 

struct generated_mesh {
    v3* P;
    v2* UV;
    v3* N;
    v3* Tangents;
    v3* BiTangents;
    u32 VertexCount;
    
    u32* Indices;
    u32 IndexCount;
};

static generated_mesh
GenerateCircle(u32 CircleSegmentCount, f32 Radius = 1.0f) {
    generated_mesh Result = {};
    
    u32 VertexCount = CircleSegmentCount + 2;
    u32 IndexCount = CircleSegmentCount * 3;
    
    v3* VertexData = (v3*)RendererAllocateMemory(sizeof(v3)*VertexCount);
    u32* IndexData = (u32*)RendererAllocateMemory(sizeof(u32)*IndexCount);
    
    f32 SegmentWidth = TAU32 / CircleSegmentCount;
    f32 Theta = 0;
    
    VertexData[0] = {};
    for(u32 SegmentIndex = 1;
        SegmentIndex < VertexCount;
        SegmentIndex++) {
        
        v3 Vertex = V3(Cos(Theta), Sin(Theta), 0) * Radius;
        VertexData[SegmentIndex] = Vertex;
        
        Theta -= SegmentWidth;
        if(SegmentIndex > 1) {
            u32 I = (SegmentIndex - 2) * 3;
            IndexData[I + 0] = 0;
            IndexData[I + 1] = SegmentIndex - 1;
            IndexData[I + 2] = SegmentIndex;
        }
    }
    
    
    Result.VertexCount = VertexCount;
    Result.P = VertexData;
    Result.Indices = IndexData;
    Result.IndexCount = IndexCount;
    
    return Result;
}

static generated_mesh 
GenerateCube() {
    generated_mesh Result = {};
    
    u32 VertexCount = 4 * 6;
    u32 IndexCount  = 3 * 2 * 6;
    
    v3* VertexData = (v3*)RendererAllocateMemory(sizeof(v3)*VertexCount);
    u32* IndexData = (u32*)RendererAllocateMemory(sizeof(u32)*IndexCount);
    
    for(u32 FaceIndex = 0;
        FaceIndex < 6;
        FaceIndex++) {
        
		const int Index0 = FaceIndex / 2;
		const int Index1 = (Index0 + 1) % 3;
		const int Index2 = (Index0 + 2) % 3;
        
        f32 Value = ((FaceIndex & 1) == 0) ? -1.0f : 1.0f;
        s32 Winding = ((FaceIndex & 1) == 0) ? 0 : 3;
        s32 V = (s32)-Value;
        
        VertexData[FaceIndex * 4 + Winding].E[Index0] = Value;
        VertexData[FaceIndex * 4 + Winding].E[Index1] = Value;
        VertexData[FaceIndex * 4 + Winding].E[Index2] = Value;
        
        VertexData[FaceIndex * 4 + (Winding + V)].E[Index0] = Value;
        VertexData[FaceIndex * 4 + (Winding + V)].E[Index1] = -Value;
        VertexData[FaceIndex * 4 + (Winding + V)].E[Index2] = Value;
        
        VertexData[FaceIndex * 4 + (Winding + V*2)].E[Index0] = Value;
        VertexData[FaceIndex * 4 + (Winding + V*2)].E[Index1] = -Value;
        VertexData[FaceIndex * 4 + (Winding + V*2)].E[Index2] = -Value;
        
        VertexData[FaceIndex * 4 + (Winding + V*3)].E[Index0] = Value;
        VertexData[FaceIndex * 4 + (Winding + V*3)].E[Index1] = Value;
        VertexData[FaceIndex * 4 + (Winding + V*3)].E[Index2] = -Value;
        
        
        IndexData[FaceIndex * 6 + 0] =  FaceIndex * 4 + 0;
        IndexData[FaceIndex * 6 + 1] =  FaceIndex * 4 + 1;
        IndexData[FaceIndex * 6 + 2] =  FaceIndex * 4 + 2;
        
        IndexData[FaceIndex * 6 + 3] =  FaceIndex * 4 + 0;
        IndexData[FaceIndex * 6 + 4] =  FaceIndex * 4 + 2;
        IndexData[FaceIndex * 6 + 5] =  FaceIndex * 4 + 3;
    }
    
    Result.VertexCount = VertexCount;
    Result.P = VertexData;
    Result.Indices = IndexData;
    Result.IndexCount = IndexCount;
    
    return Result;
}

static generated_mesh
GenerateTessellatedCube(u32 Divisions) {
    Assert(Divisions > 0);
    
    generated_mesh Result = {};
    
    u32 DivisionsSq = (Divisions + 1) * (Divisions + 1);
    
    v3* Vertices = (v3*)RendererAllocateMemory(sizeof(v3)*DivisionsSq);
    
    for(u32 Y = 0; 
        Y < Divisions + 1;
        Y++) {
        for(u32 X = 0; 
            X < Divisions + 1;
            X++) {
            
            f32 XBilateral = ((f32)X / (f32)Divisions  - 0.5f) * 2.0f;
            f32 YBilateral = ((f32)Y / (f32)Divisions  - 0.5f) * 2.0f;
            
            Vertices[Y * (Divisions + 1) + X] = V3(XBilateral, YBilateral, 1.0f);
        }
    }
    
    u32 QuadsPerFace = Divisions*Divisions;
    u32* FaceIDs = (u32*)RendererAllocateMemory(sizeof(u32)*6*QuadsPerFace);
    u32 FaceIndex = 0;
    for(u32 Y = 0;
        Y < Divisions;
        Y++) {
        for(u32 X = 0;
            X < Divisions;
            X++) {
            u32 Y0 = Y;
            u32 Y1 = Y + 1;
            u32 X0 = X;
            u32 X1 = X + 1;
            
            FaceIDs[FaceIndex * 6 + 1] = Y0 * (Divisions + 1) + X0;
            FaceIDs[FaceIndex * 6 + 0] = Y1 * (Divisions + 1) + X0;
            FaceIDs[FaceIndex * 6 + 2] = Y1 * (Divisions + 1) + X1;
            
            FaceIDs[FaceIndex * 6 + 4] = Y0 * (Divisions + 1) + X0;
            FaceIDs[FaceIndex * 6 + 3] = Y1 * (Divisions + 1) + X1;
            FaceIDs[FaceIndex * 6 + 5] = Y0 * (Divisions + 1) + X1;
            FaceIndex++;
        }
    }
    
    m4x4 Orientations[6];
    
    v3 PX = V3( 1.0f,  0.0f,  0.0f);
    v3 NX = V3(-1.0f,  0.0f,  0.0f);
    v3 PY = V3( 0.0f,  1.0f,  0.0f);
    v3 NY = V3( 0.0f, -1.0f,  0.0f);
    v3 PZ = V3( 0.0f,  0.0f,  1.0f);
    v3 NZ = V3( 0.0f,  0.0f, -1.0f);
    
    Orientations[0] = Rows3x3(PZ, PX, PY);
    Orientations[1] = Rows3x3(NZ, NX, PY);
    Orientations[2] = Rows3x3(PX, PZ, NY);
    Orientations[3] = Rows3x3(PX, NZ, PY);
    Orientations[4] = Rows3x3(PX, PY, PZ);
    Orientations[5] = Rows3x3(NX, PY, NZ);
    
    
    u32 FinalVertexCount = 4*6*QuadsPerFace;
    u32 FinalIndexCount = 3*2*6*QuadsPerFace;
    
    Result.UV = (v2*)RendererAllocateMemory(sizeof(v2)*FinalVertexCount);
    Result.P = (v3*)RendererAllocateMemory(sizeof(v3)*FinalVertexCount);
    Result.N = (v3*)RendererAllocateMemory(sizeof(v3)*FinalVertexCount);
    Result.Indices = (u32*)RendererAllocateMemory(sizeof(u32)*FinalIndexCount);
    Result.VertexCount = FinalVertexCount;
    Result.IndexCount = FinalIndexCount;
    
    v2 UVArray[3] = {};
    UVArray[0] = V2(0.1, 0.1);
    UVArray[1] = V2(0.1, 0.9);
    UVArray[2] = V2(0.9, 0.9);
    u32 UVIndex = 0;
    
    for(u32 SideIndex = 0;
        SideIndex < 6;
        SideIndex++) {
        
        m4x4 Mat = Orientations[SideIndex];
        
        for(u32 VertIndex = 0; 
            VertIndex < DivisionsSq;
            VertIndex++) {
            
            v3 FinalP = Mat * Vertices[VertIndex];
            
            Result.P[SideIndex * DivisionsSq + VertIndex] = FinalP;
            Result.UV[SideIndex * DivisionsSq + VertIndex] = UVArray[UVIndex++];
            if(UVIndex == 2) {
                UVIndex = 0;
            }
            
            Result.N[SideIndex * DivisionsSq + VertIndex] = Normalize(FinalP);
        }
        
        for(u32 IndexIndex = 0;
            IndexIndex < 3*2*QuadsPerFace;
            IndexIndex++) {
            
            u32 Offset = 3*2*QuadsPerFace*SideIndex;
            Result.Indices[IndexIndex + Offset] = FaceIDs[IndexIndex] + DivisionsSq * SideIndex;
            
        }
        
    }
    
    return Result;
}

static generated_mesh
GenerateSphere(f32 r) {
    
    generated_mesh Result = {};
    if(r < 0.0f) {
        r = 0.0f;
    }
    
    f32 Min = 10;
    f32 Max = 30;
    
    f32 t = Clamp01(r / 100.0f);
    f32 Subdivisions = Lerp(Min, t, Max);
    
    Result = GenerateTessellatedCube((u32)Subdivisions);
    
    for(u32 VertIndex = 0;
        VertIndex < Result.VertexCount;
        VertIndex++) {
        
        v3 P = Result.P[VertIndex];
        Result.P[VertIndex] = Normalize(P);
        
    }
    
    return Result;
}

#endif //MESH_GENERATION_H
