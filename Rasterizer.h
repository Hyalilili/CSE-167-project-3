//
//  Rasterizer.hpp
//  CSE167 Spring 2015 Starter Code
//
//  Created by Gaojie Li on 10/14/15.
//  Copyright © 2015 RexWest. All rights reserved.
//

#ifndef CSE167_Rasterizer_h
#define CSE167_Rasterizer_h

#include <iostream>
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4.h"
#include "Window.h"

class Rasterizer {
protected:
    Matrix4 Proj_Matrix; // projection matrix
    Matrix4 VP_Matrix;  // ViewPort matrix
    
public:
    int drawMode;
    Vector3 p;
    float Red, Green, Blue; //define colors
    float alpha, beta, gamma; // define barycentric coordinates
    int part = 0;
    bool debug_mode = false;
    
    Rasterizer(void); //constructor
    virtual ~Rasterizer(void); //destructor
    virtual void clearBuffer(void);
    virtual void display(void);
    virtual Vector4 rasterizeVertex(Vector4&);
    virtual void rasterizeTriangle(Vector4, Vector4, Vector4);
    virtual void reshape(int, int); //reset Proj Matrix and viewport matrix after resizing window
    virtual void setProjMatrix(float, float, float, float);
    virtual void setVPMatrix(float, float);
    virtual void drawPoint(int, int, float, float, float);
    virtual bool findBarycenter(Vector3 a, Vector3 b, Vector3 c, Vector3& p);
};

#endif /* Rasterizer_h */