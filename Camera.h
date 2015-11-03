#ifndef CSE167_Camera_h
#define CSE167_Camera_h

#include "Matrix4.h"
#include "Vector3.h"

class Camera
{
private:
    
protected:
    
    Matrix4 c;   //Camera Matrix
    Matrix4 ci;  //Inverse of Camera Matrix
    
public:
    
    Vector3 e;   //Center of Projection
    Vector3 d;   //Look At Point
    Vector3 up;  //Up Vector ^
    
    Camera(void);
   // Camera(Vector3 e, Vector3 d, Vector3 up);
    ~Camera(void);
    
    void update(void);
    
    Matrix4& getMatrix(void);
    Matrix4& getInverseMatrix(void);
    
    void set(Vector3&, Vector3&, Vector3&);
    
    void reset_Cam();
};

#endif
