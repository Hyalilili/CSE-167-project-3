#ifndef CSE167_Cube_h
#define CSE167_Cube_h

#include "Drawable.h"
#include "Globals.h"

class Cube : public Drawable
{
    
public:
    
    float size;
    Vector3 n1, n2;
    
    Cube(float);
    virtual ~Cube(void);
    
    virtual void draw(DrawData&);
    void rast_draw (DrawData&);
    virtual void update(UpdateData&);
    
    void spin(float);
    
};

#endif

