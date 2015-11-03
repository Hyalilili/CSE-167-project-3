#ifndef CSE167_Light_h
#define CSE167_Light_h

#include "Matrix4.h"
#include "Vector4.h"
#include "Color.h"
#include <vector>

class Light
{
    
protected:
    
    int bindID = -1;
    int lightSource;
    
public:
    
    Vector4 position;
    Vector4 direction;
    // cone angle -> cutoff (half)
    // exponent -> how concentrate the light is
    // both for spot light only
    float coneAngle, exponent;
    
    Color ambientColor;
    Color diffuseColor;
    Color specularColor;
    
    float constantAttenuation;
    float linearAttenuation;
    float quadraticAttenuation;
    
public:
    
    Light(int);
    virtual ~Light(void);
    
    void bind(int);
    void unbind(void);
    
    virtual void initialLight();
};

#endif
