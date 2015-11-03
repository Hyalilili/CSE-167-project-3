#include "Camera.h"

Camera::Camera()
{
    c.identity();
    e.set(0.0, 0.0, 20.0);
    d.set(0.0, 0.0, 0.0);
    up.set(0.0, 1.0, 0.0);
    
    //Pre-define a camera matrix (and its inverse) that are shifted 'e' from the origin
    //This is used as a default camera position for Project 1
    c.set(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, e[0], e[1], e[2], 1);
    ci.set(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, -e[0], -e[1], -e[2], 1);
}
/*
Camera::Camera(Vector3 e, Vector3 d, Vector3 up) {
    this -> e = e;
    this -> d = d;
    this -> up = up;
    
    // Find Zc vector
    Vector3 z = e.subtract(d);
    z.scale(1/z.magnitude());
    
    // Find Xc vector
    Vector3 x = up.cross(z);
    x.scale(1/x.magnitude());
    
    // Find Yc vector by Xc and Zc
    Vector3 y = z.cross(x);
    
    // Set the camera matrix
    C.set(x[0], x[1], x[2], 0,
          y[0], y[1], y[2], 0,
          z[0], z[1], z[2], 0,
          e[0], e[1], e[2], 1);
    
    // c inverse = R(-1) x T(-1), R(-1) = RT
    Ci.set(x[0], y[0], z[0], 0,
           x[1], y[1], z[1], 0,
           x[2], y[2], z[2], 0,
           -e[0], -e[1], -e[2], 1);
}
*/
Camera::~Camera()
{
    //Delete and dynamically allocated memory/objects here
}

Matrix4& Camera::getMatrix()
{
    return c;
}

Matrix4& Camera::getInverseMatrix()
{
    return ci;
}

void Camera::update()
{
    //Update the Camera Matrix using d, e, and up
    //Solve for the z, x, and y axes of the camera matrix
    //Use these axes and the e vector to create a camera matrix c
    //Use c to solve for an inverse camera matrix ci
    
    // Find Zc vector
    Vector3 z = e.subtract(d);
    z = z.scale(1/z.magnitude());
    
    // Find Xc vector
    Vector3 x = up.cross(z);
    x = x.scale(1/x.magnitude());
    
    // Find Yc vector by Xc and Zc
    Vector3 y = z.cross(x);
    
    // Set the camera matrix
    c.set(x.m[0], x.m[1], x.m[2], 0,
          y.m[0], y.m[1], y.m[2], 0,
          z.m[0], z.m[1], z.m[2], 0,
          e.m[0], e.m[1], e.m[2], 1);

    // c inverse = R(-1) x T(-1), R(-1) = RT
    Matrix4 T;
    T = T.makeTranslate(-e.m[0], -e.m[1], -e.m[2]);
    Matrix4 R;
    R.set(x.m[0], y.m[0], z.m[0], 0,
          x.m[1], y.m[1], z.m[1], 0,
          x.m[2], y.m[2], z.m[2], 0,
          0, 0, 0, 1);
    ci = R.multiply(T);
}

void Camera::set(Vector3& e, Vector3& d, Vector3& up)
{
    this->e = e;
    this->d = d;
    this->up = up;
    
    update();
}

void Camera::reset_Cam() {
    this -> e.set(0, 0, 20);
    this -> d.set(0, 0, 0);
    this -> up.set(0, 1, 0);
    
    update();
}
