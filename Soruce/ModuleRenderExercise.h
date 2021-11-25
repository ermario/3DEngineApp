#pragma once
#include "Module.h"
#include "Geometry/Frustum.h"


class ModuleRenderExercise : public Module
{
public:
    ModuleRenderExercise();
    ~ModuleRenderExercise();
    
    bool Init();
    bool CleanUp();
    update_status Update();
    


private:
    unsigned vertex_buffer_object = 0;
};