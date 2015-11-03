#include "Cube.h"

#ifdef __APPLE__
    #include <GLUT/glut.h>
#else
    #include <GL/glut.h>
#endif

#include "Window.h"
#include "math.h"

Cube::Cube(float size) : Drawable()
{
    this->size = size;
}

Cube::~Cube()
{
    //Delete any dynamically allocated memory/objects here
}


void Cube::draw(DrawData& data)
{
    float halfSize = size/2.0;
    
    //Apply the material properties
    //From here forward anything drawn will be drawn with this material
    material.apply();
    
    //Set the OpenGL Matrix mode to ModelView (used when drawing geometry)
    glMatrixMode(GL_MODELVIEW);
    
    //Push a save state onto the matrix stack, and multiply in the toWorld matrix
    glPushMatrix();
    glMultMatrixf(toWorld.ptr());
    
    //Make cube!
    //Note: The glBegin, and glEnd should always be as close to the glNormal/glVertex calls as possible
    //These are special calls that 'freeze' many internal states of OpenGL.
    //Once the glBegin state is active many of the calls made to OpenGL (like glMultMatrixf) will be IGNORED!
    //As a good habit, only call glBegin just before you need to draw, and call end just after you finish
    glBegin(GL_QUADS);
    
    // Draw front face:
    glNormal3f(0.0, 0.0, 1.0);
    glVertex3f(-halfSize,  halfSize,  halfSize);
    glVertex3f( halfSize,  halfSize,  halfSize);
    glVertex3f( halfSize, -halfSize,  halfSize);
    glVertex3f(-halfSize, -halfSize,  halfSize);
    
    // Draw left side:
    glNormal3f(-1.0, 0.0, 0.0);
    glVertex3f(-halfSize,  halfSize,  halfSize);
    glVertex3f(-halfSize,  halfSize, -halfSize);
    glVertex3f(-halfSize, -halfSize, -halfSize);
    glVertex3f(-halfSize, -halfSize,  halfSize);
    
    // Draw right side:
    glNormal3f(1.0, 0.0, 0.0);
    glVertex3f( halfSize,  halfSize,  halfSize);
    glVertex3f( halfSize,  halfSize, -halfSize);
    glVertex3f( halfSize, -halfSize, -halfSize);
    glVertex3f( halfSize, -halfSize,  halfSize);
    
    // Draw back face:
    glNormal3f(0.0, 0.0, -1.0);
    glVertex3f(-halfSize,  halfSize, -halfSize);
    glVertex3f( halfSize,  halfSize, -halfSize);
    glVertex3f( halfSize, -halfSize, -halfSize);
    glVertex3f(-halfSize, -halfSize, -halfSize);
    
    // Draw top side:
    glNormal3f(0.0, 1.0, 0.0);
    glVertex3f(-halfSize,  halfSize,  halfSize);
    glVertex3f( halfSize,  halfSize,  halfSize);
    glVertex3f( halfSize,  halfSize, -halfSize);
    glVertex3f(-halfSize,  halfSize, -halfSize);
    
    // Draw bottom side:
    glNormal3f(0.0, -1.0, 0.0);
    glVertex3f(-halfSize, -halfSize, -halfSize);
    glVertex3f( halfSize, -halfSize, -halfSize);
    glVertex3f( halfSize, -halfSize,  halfSize);
    glVertex3f(-halfSize, -halfSize,  halfSize);
    
    glEnd();
    
    //The above glBegin, glEnd, glNormal and glVertex calls can be replaced with a glut convenience function
    //glutSolidCube(size);
    
    //Pop the save state off the matrix stack
    //This will undo the multiply we did earlier
    glPopMatrix();
}

void Cube::rast_draw (DrawData& data)
{
    
    Globals::raster.clearBuffer();
    float halfSize = size/2.0;
    Vector4 f1(-halfSize,  halfSize,  halfSize);
    Vector4 f2( halfSize,  halfSize,  halfSize);
    Vector4 f3( halfSize, -halfSize,  halfSize);
    Vector4 f4(-halfSize, -halfSize,  halfSize);
    Vector4 l1(-halfSize,  halfSize, -halfSize);
    Vector4 l2(-halfSize, -halfSize, -halfSize);
    Vector4 r1( halfSize,  halfSize, -halfSize);
    Vector4 r2( halfSize, -halfSize, -halfSize);
    
    f1 = toWorld*f1;
    f2 = toWorld*f2;
    f3 = toWorld*f3;
    f4 = toWorld*f4;
    l1 = toWorld*l1;
    l2 = toWorld*l2;
    r1 = toWorld*r1;
    r2 = toWorld*r2;
    
    
    if(Globals::raster.part == 0){
        Globals::raster.rasterizeVertex(f1);
        Globals::raster.rasterizeVertex(f2);
        Globals::raster.rasterizeVertex(f3);
        Globals::raster.rasterizeVertex(f4);
        Globals::raster.rasterizeVertex(l1);
        Globals::raster.rasterizeVertex(l2);
        Globals::raster.rasterizeVertex(r1);
        Globals::raster.rasterizeVertex(r2);
    }
    else{
        
        // front face
        n1.set(0,0,1);
        
        Globals::raster.rasterizeTriangle(f1, f2 ,f3);
        Globals::raster.rasterizeTriangle(f3 ,f4, f1);
        
        //left side
        n1.set(-1,0,0);
        
        Globals::raster.rasterizeTriangle(f1, l1 ,l2);
        Globals::raster.rasterizeTriangle(l2 ,f4, f1);
        
        n1.set(1,0,0);
        
        Globals::raster.rasterizeTriangle(f2, r1 ,r2);
        Globals::raster.rasterizeTriangle(r2 ,f3, f2);
        
        n1.set(0,0,-1);
        
        Globals::raster.rasterizeTriangle(l1, r1 ,r2);
        Globals::raster.rasterizeTriangle(r2 ,l2, l1);
        
        n1.set(0,1,0);
        
        Globals::raster.rasterizeTriangle(f1, f2 ,r1);
        Globals::raster.rasterizeTriangle(r1 ,l1 ,f1);
        
        n1.set(0,-1,0);
        
        Globals::raster.rasterizeTriangle(l2, r2 ,f3);
        Globals::raster.rasterizeTriangle(f3 ,f4 ,l2);
    }
    
    
    
    Globals::raster.display();
}

void Cube::update(UpdateData& data)
{
    //
}

void Cube::spin(float radians)
{
    Matrix4 rotation;
    rotation.makeRotateY(radians);
    
    toWorld = toWorld * rotation;
}

