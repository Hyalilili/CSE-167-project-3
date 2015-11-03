#include "Light.h"
#include "Window.h"
#include <iostream>

#ifdef __APPLE__
    #include <GLUT/glut.h>
#else
    #include <GL/glut.h>
#endif

#define DIRECTIONAL 1
#define POINT 2
#define SPOT 3

Light::Light(int mode)
{
    ambientColor = Color::ambientDefault();
    diffuseColor = Color::diffuseDefault();
    specularColor = Color::specularDefault();
    
    constantAttenuation = 1.0;
    linearAttenuation = 0.0;
    quadraticAttenuation = 0.05;
    
    lightSource = mode;
    initialLight();
}

Light::~Light()
{
    //Delete any dynamically allocated memory/objects here
}

void Light::bind(int id)
{
    if(id < 0 || id > 7) {
        std::cout << "ERROR: The light bind ID " << id << " is not valid!" << std::endl;
        return;
    }
  
    //Set the bindID
    bindID = id;
    
    //Configure the light at the bindID
    glEnable(GL_LIGHT0 + bindID);
    
    //Configure the color of the light
    glLightfv(GL_LIGHT0 + bindID, GL_AMBIENT, ambientColor.ptr());
    glLightfv(GL_LIGHT0 + bindID, GL_DIFFUSE, diffuseColor.ptr());
    glLightfv(GL_LIGHT0 + bindID, GL_SPECULAR, specularColor.ptr());
    
    //Configure the attenuation properties of the light
    //Add support for Constant Attenuation
    //Add support for Linear Attenuation
    glLightf(GL_LIGHT0 + bindID, GL_QUADRATIC_ATTENUATION, quadraticAttenuation);
    
    //Position the light
    glLightfv(GL_LIGHT0 + bindID, GL_POSITION, position.ptr());
    
    //Setup spotlight direction, angle, and exponent here
    if (lightSource == SPOT) {
        glLightf(GL_LIGHT0 + bindID, GL_SPOT_CUTOFF, coneAngle);
        glLightfv(GL_LIGHT0 + bindID, GL_SPOT_DIRECTION, direction.ptr());
        glLightf(GL_LIGHT0 + bindID, GL_SPOT_EXPONENT, exponent);
    }
}

void Light::unbind(void)
{
    glDisable(GL_LIGHT0 + bindID);
    bindID = -1;
}

void Light::initialLight() {
    switch (lightSource) {
        // regular lighting condition
        case 0:
            coneAngle = 180.0;
            exponent = 2.0;
            break;
            
        case DIRECTIONAL:
            position[3] = 0;
            ambientColor = Color::white();
            diffuseColor = Color::white();
            specularColor = Color::white();
            constantAttenuation = 1;
            linearAttenuation = 0;
            quadraticAttenuation = 0;
            break;
            
        case POINT:
            coneAngle = 180.0;
            exponent = 2.0;
            break;
            
        case SPOT:
            coneAngle = 30.0;
            exponent = 100.0;
            break;
    }
}
