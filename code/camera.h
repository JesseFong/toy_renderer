/* date = August 21st 2020 3:30 pm */

#ifndef CAMERA_H
#define CAMERA_H


struct camera {
    v3 P;
    v3 dP;
    v3 XAxis;
    v3 YAxis;
    v3 ZAxis;
    v3 Up;
    f32 Pitch;
    f32 Orbit;
    f32 FOV;
    m4x4 Rotation;
    f32 NearClip;
    f32 FarClip;
};

struct camera_update_params {
    v2 dMouseP;
    v3 ddP;
};

static camera
InitializeCamera(v3 P, v3 ZAxis, v3 Up, f32 FOV, f32 Near, f32 Far) {
    
    camera Camera = {};
    
    Camera.P = P;
    Camera.dP = {};
    Camera.ZAxis = ZAxis;
    Camera.XAxis = NOZ(Cross(ZAxis, Up));
    Camera.YAxis = NOZ(Cross(Camera.XAxis, ZAxis));
    Camera.Up = Up;
    Camera.FOV = FOV;
    Camera.NearClip = Near;
    Camera.FarClip = Far;
    
    quaternion PitchRotation = RotToQuat(0, V3(1, 0, 0));
    quaternion OrbitRotation = RotToQuat(0, V3(0, 1, 0)); 
    
    Camera.Rotation = QuatToRotM4x4(PitchRotation) * QuatToRotM4x4(OrbitRotation);
    Camera.Pitch = 0.0f;
    Camera.Orbit = -90.0f;
    
    return(Camera);
    
}

#define ROTATION_SPEED 0.5f
#define MOVEMENT_SPEED 5.0f
#define CAMERA_DRAG 0.8f
#define CAMERA_DT 1.0/60.0f
#define CAMERA_MAX_VELOCITY 20.0f

static void
UpdateCamera(camera* Camera, v2 dMouseP, v3 ddP) {
    ddP *= MOVEMENT_SPEED;
    ddP += -CAMERA_DRAG*Camera->dP;
    
    v3 MovementDelta = (0.5f*ddP*Square(CAMERA_DT) + Camera->dP*CAMERA_DT);
    
    v3 OldVelocity = Camera->dP;
    Camera->dP = ddP*CAMERA_DT + Camera->dP;
    if(LengthSq(Camera->dP) > CAMERA_MAX_VELOCITY) {
        Camera->dP = OldVelocity;
    }
    Camera->P += MovementDelta;
    
    
    Camera->Orbit += ROTATION_SPEED*-dMouseP.x;
    Camera->Pitch += ROTATION_SPEED*-dMouseP.y;
    
    if(Camera->Pitch > 89.0f) Camera->Pitch = 89.0f;
    if(Camera->Pitch < -89.0f) Camera->Pitch = -89.0f;
    
    f32 NewX = Cos(Radians(Camera->Orbit)) * Cos(Radians(Camera->Pitch));
    f32 NewY = Sin(Radians(Camera->Pitch));
    f32 NewZ = Sin(Radians(Camera->Orbit)) * Cos(Radians(Camera->Pitch));
    
    Camera->ZAxis = NOZ(V3(NewX, NewY, NewZ));
    Camera->XAxis = NOZ(Cross(Camera->ZAxis, Camera->Up));
    Camera->YAxis = NOZ(Cross(Camera->XAxis, Camera->ZAxis));
}

#endif //CAMERA_H
