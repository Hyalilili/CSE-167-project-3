//
//  Rasterizer.cpp
//  CSE167 Spring 2015 Starter Code
//
//  Created by Gaojie Li on 10/14/15.
//  Copyright © 2015 RexWest. All rights reserved.
//

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <iostream>
#include <math.h>
#include "Rasterizer.h"
#include "Globals.h"
#include "Cube.h"
//#include <GL/gl.h>
//#include <GL/glut.h>

static int window_width = 512, window_height = 512;
static float* pixels = new float[window_width * window_height * 3];
static float* zBuffer = new float[window_width * window_height];

static float z_value;

using namespace std;

/*struct Color    // generic color
{
    float r,g,b;
};*/

// Constructor
Rasterizer::Rasterizer() {
    //Initialize the ViewPort Matrix according to the window size.
    VP_Matrix.set(window_width/2, 0, 0, 0,
                  0, window_height/2, 0, 0,
                  0, 0, 1/2, 0,
                  window_width/2, window_height/2, 1/2, 1);
}

// Destructor
Rasterizer::~Rasterizer() {
}

// Method to set the perspective projection matrix
void Rasterizer::setProjMatrix(float aspect, float fieldOfView, float near, float far) {
    // convert degree to x*pi
    fieldOfView = fieldOfView / 180 * M_PI;
    Proj_Matrix.set(1/(aspect*tan(fieldOfView/2)), 0, 0, 0,
                    0, 1/(tan(fieldOfView/2)), 0, 0,
                    0, 0, (near+far)/(near-far), -1,
                    0, 0, (2*near*far)/(near-far), 0);
}

// Method to set the Viewport Matrix according to reshaped window size
void Rasterizer::setVPMatrix(float width, float height) {
    VP_Matrix.set(width/2, 0, 0, 0,
                  0, height/2, 0, 0,
                  0, 0, 1/2, 0,
                  width/2, height/2, 1/2, 1);
}

void loadData()
{
    // put code to load data model here
}

// Clear frame buffer
void Rasterizer::clearBuffer()
{
    Color clearColor = {0.0, 0.0, 0.0};   // clear color: black
    for (int i=0; i<window_width*window_height; ++i)
    {
        pixels[i*3]   = clearColor[0];
        pixels[i*3+1] = clearColor[1];
        pixels[i*3+2] = clearColor[2];
        zBuffer[i] = 1;
    }
}

// Draw a point into the frame buffer
void Rasterizer::drawPoint(int x, int y, float r, float g, float b)
{
    int offset = y*window_width*3 + x*3;
    
    if(z_value < zBuffer[x + y * window_width]/* && z_value >= 0*/) {
        pixels[offset]   = r;
        pixels[offset+1] = g;
        pixels[offset+2] = b;
        
        if (part > 1){
            zBuffer[x + y * window_width] = z_value;
        }
    }
}

// Method to rasterizeVertex (Part 1 of project 3)
Vector4 Rasterizer::rasterizeVertex(Vector4& target) {
    target = Globals::camera.getInverseMatrix() * target;
    target = Proj_Matrix * target;
    target = target.dehomogenize();
    //target.print("before dehomogenize");
    //VP_Matrix.print("check matrix");
    target = VP_Matrix * target;
    
   // target.print("check pt");
    
    // condition to draw points
    if ((int)target[0] >= 0 && (int)target[1] >= 0
        && (int)target[0] <= window_width && (int)target[1] <= window_height) {
        // Draw white point on that pixel
        drawPoint(target[0], target[1], 1, 1, 1);
    }
    
    return target;
}

void Rasterizer::rasterizeTriangle(Vector4 vertex1, Vector4 vertex2, Vector4 vertex3) {
    vertex1 = Globals::camera.getInverseMatrix() * vertex1;
    vertex1 = Proj_Matrix * vertex1;
    vertex1 = VP_Matrix * vertex1;
    vertex1 = vertex1.dehomogenize();
    
    vertex2 = Globals::camera.getInverseMatrix() * vertex2;
    vertex2 = Proj_Matrix * vertex2;
    vertex2 = VP_Matrix * vertex2;
    vertex2 = vertex2.dehomogenize();

    vertex3 = Globals::camera.getInverseMatrix() * vertex3;
    vertex3 = Proj_Matrix * vertex3;
    vertex3 = VP_Matrix * vertex3;
    vertex3 = vertex3.dehomogenize();
    
    Vector3 v1 = vertex1.toVector3();
    Vector3 v2 = vertex2.toVector3();
    Vector3 v3 = vertex3.toVector3();
    
    //cout << "v1" << v1[0] << "v2" << v2[0] << "v3" << v3[0] << endl;
    int min_X = min(min(v1[0], v2[0]), v3[0]);
    int min_Y = min(min(v1[1], v2[1]), v3[1]);
    int max_X = max(max(v1[0], v2[0]), v3[0]);
    int max_Y = max(max(v1[1], v2[1]), v3[1]);
    
    min_X = max(min_X, 0);
    //cout << "minX=" << min_X << endl;
    min_Y = max(min_Y, 0);
    max_X = min(max_X, window_width - 1);
    //cout << "maxX=" << max_X << endl;
    max_Y = min(max_Y, window_height - 1);
    
    //******************************UNDONE *************************************//
   // Vector3 p;
    // part = 1 or 2 rasterize triangle / z-buffer
    if (part < 3) {
       /* int seed = rand();
        srand(seed);*/
        
        Red = (float)(rand()) / (float)(RAND_MAX);
        Green = (float)(rand()) / (float)(RAND_MAX);
        Blue = (float)(rand()) / (float)(RAND_MAX);
        
        for (int i = min_X; i <= max_X; i++) {
            for (int j = min_Y; j <= max_Y; j++) {
                if ((i == min_X || i == max_X) && debug_mode == true) {
                    drawPoint(i, j, 255, 0, 0);
                }
                
                if ((j == min_Y || j == max_Y) && debug_mode == true) {
                    drawPoint(i, j, 255, 0, 0);
                }

                Vector3 p(i, j, 0);
                
                /********************DEBUG*********************/
                //cout << "find is" << findBarycenter(v1, v2, v3, p) << "part=" << part << endl;
                
                if (findBarycenter(v1, v2, v3, p)) {
                    drawPoint(i, j, Red, Green, Blue);
                }
            }
        }
    }
    else {
        for (int i = min_X; i <= max_X; i++) {
            for (int j = min_Y; j <= max_Y; j++) {
                // debug bounded box
                if ((i == min_X || i == max_X) && debug_mode == true) {
                    drawPoint(i, j, 255, 0, 0);
                }
                
                if ((j == min_Y || j == max_Y) && debug_mode == true) {
                    drawPoint(i, j, 255, 0, 0);
                }
                
                p.set(i, j, 0);
                
                if (findBarycenter(v1, v2, v3, p)) {
                    if (drawMode == 3) {
                        Vector3 color(Globals::bunny.vect1_norm * alpha +
                                      Globals::bunny.vect2_norm * beta +
                                      Globals::bunny.vect3_norm * gamma);
                        
                        color = color.add(Vector3(1, 1, 1));
                        color = color.multiply(0.5);
                        
                        Red = color[0];
                        Green = color[1];
                        Blue = color[2];
                        
                        drawPoint(i, j, Red, Green, Blue);
                    }
                    
                    else if (drawMode == 4) {
                        Vector3 color(Globals::dragon.vect1_norm * alpha +
                                      Globals::dragon.vect2_norm * beta +
                                      Globals::dragon.vect3_norm * gamma);
                        
                        color = color.add(Vector3(1, 1, 1));
                        color = color.multiply(0.5);
                        
                        Red = color[0];
                        Green = color[1];
                        Blue = color[2];
                        
                        drawPoint(i, j, Red, Green, Blue);
                    }
                    
                    else if (drawMode == 5) {
                        Vector3 color(Globals::bear.vect1_norm * alpha +
                                      Globals::bear.vect2_norm * beta +
                                      Globals::bear.vect3_norm * gamma);
                        
                        color = color.add(Vector3(1, 1, 1));
                        color = color.multiply(0.5);
                        
                        Red = color[0];
                        Green = color[1];
                        Blue = color[2];
                        
                        drawPoint(i, j, Red, Green, Blue);
                    }
                    else if (drawMode == 0) {
                        Vector3 rand1(0, 1, 2);
                        Vector3 rand2(3, 3, 4);
                        Vector3 rand3(2, 4, 7);
                        Vector3 color(rand1 * alpha +
                                      rand2 * beta +
                                      rand3 * gamma);
                        
                        color = color.add(Vector3(1, 1, 1));
                        color = color.multiply(0.5);
                        
                        Red = color[0];
                        Green = color[1];
                        Blue = color[2];
                        
                        drawPoint(i, j, Red, Green, Blue);
                    }
                    else if (drawMode == 1) {
                        drawPoint(i, j,Red,Green,Blue);
                    }
                    
                    else if (drawMode == 2) {
                        drawPoint(i, j, Red, Green, Blue);
                    }
                }
            }
        }
    }
    // Add code to rasterize a triangle
}

/*
void rasterize()
{
    // Put your main rasterization loop here
    // It should go over the data model and call rasterizeTriangle for every triangle in it
    
    // example: draw diagonal line:
    for (int i=0; i<min(window_width,window_height); ++i)
    {
        drawPoint(i, i, 1.0, 0.0, 0.0);
    }
}*/

// Called whenever the window size changes
void Rasterizer::reshape(int new_width, int new_height)
{
    window_width  = new_width;
    window_height = new_height;
    delete[] pixels;
    pixels = new float[window_width * window_height * 3];
    
    zBuffer = new float[window_width * window_height];
    
    setProjMatrix((float)new_width/(float)new_height, 60, 1, 1000);
    setVPMatrix((float)new_width, (float)new_height);
}

void keyboard(unsigned char key, int, int)
{
    cerr << "Key pressed: " << key << endl;
}

void Rasterizer::display()
{
    //clearBuffer();
   // rasterize();
    srand(0);
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // glDrawPixels writes a block of pixels to the framebuffer
    glDrawPixels(window_width, window_height, GL_RGB, GL_FLOAT, pixels);
    
    //glutSwapBuffers();
    glutGet(GLUT_ELAPSED_TIME);
}

bool Rasterizer::findBarycenter(Vector3 a, Vector3 b, Vector3 c, Vector3 &p) {
    Vector3 temp0 = b - a;
    Vector3 temp1 = c - a;
    Vector3 temp2 = c - b;
    Vector3 temp3 = p - b;
    Vector3 temp4 = a - c;
    Vector3 temp5 = p - c;
    Vector3 temp6 = p - a;
    
    Vector3 n = temp0.cross(temp1);
    Vector3 nA = temp2.cross(temp3);
    Vector3 nB = temp4.cross(temp5);
    //Vector3 normalC = temp0.cross(temp6);
    
    float Mag_n = n.magnitude() * n.magnitude();
    float nnA = n.dot(nA);
    float nnB = n.dot(nB);
    
    alpha = nnA / Mag_n;
    beta = nnB / Mag_n;
    gamma = 1.0f - alpha - beta;
    
    z_value = alpha * a[2] + beta * b[2] + gamma * c[2];
    
    //cout << alpha << " " << beta << " " << gamma << endl;
    
   /* if (z_value > 1 || z_value < 0) {
        cout << "z= " << z_value << endl;
    }*/
    
    if (alpha >= 0 && alpha <= 1
        && beta >= 0 && beta <= 1
        && gamma >= 0 && gamma <= 1) {
        return true;
    }
    else
        return false;
}

/*
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(window_width, window_height);
    glutCreateWindow("CSE 167 Project 3");
    
    loadData();
    
    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glutMainLoop();
}*/