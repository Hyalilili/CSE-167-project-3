#ifndef CSE167_Globals_h
#define CSE167_Globals_h

#include "Camera.h"
//#include "Light.h"
#include "DrawData.h"
#include "UpdateData.h"
#include "OBJObject.h"
//#include "Rasterizer.h"

class Rasterizer;
class Cube;
class House;
class Sphere;
class Light;

class Globals
{
    
public:
    
    static Camera camera;
    static Cube cube;
    static Light light;
    static Light direc_light;
    static Light point_light;
    static Light spot_light;
    static DrawData drawData;
    static UpdateData updateData;
    static House house;
    static OBJObject bunny;
    static OBJObject dragon;
    static OBJObject bear;
    static Rasterizer raster;
    static Sphere sphere;
    //Feel free to add more member variables as needed
    
};

#endif
