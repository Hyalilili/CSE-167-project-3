#include "Globals.h"
#include "Rasterizer.h"
#include "Cube.h"
#include "House.h"
#include "Sphere.h"
#include "Light.h"

Camera Globals::camera;

Cube Globals::cube(10.0);

Light Globals::light = Light(0);

Light Globals::direc_light = Light(1);
Light Globals::point_light = Light(2);
Light Globals::spot_light = Light(3);

House Globals::house;

OBJObject Globals::bunny = OBJObject("bunny.obj");
OBJObject Globals::dragon = OBJObject("dragon.obj");
OBJObject Globals::bear = OBJObject("bear.obj");

Rasterizer Globals::raster = Rasterizer();

Sphere Globals::sphere = Sphere(10, 30, 30);

DrawData Globals::drawData;
UpdateData Globals::updateData;