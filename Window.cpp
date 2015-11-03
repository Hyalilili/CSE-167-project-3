#include <iostream>

#ifdef __APPLE__
    #include <GL/freeglut.h>
#else
    #include <GL/glut.h>
#endif

#include "Window.h"
#include "Cube.h"
#include "Matrix4.h"
#include "Globals.h"
#include "Vector3.h"
#include "Rasterizer.h"
#include "House.h"
#include "Sphere.h"
#include "Drawable.h"
#include "Light.h"

int Window::width  = 512;   //Set window width in pixels here
int Window::height = 512;   //Set window height in pixels here
float spin_speed = 0.005;
int mode = 0;   //Mode for what object to be displayed.
bool rast_mode = false;
int timebase = 0;
float fps;
int frame = 0;
//bool debug_mode = false;

Drawable* Window::obj = &Globals::cube;

// for mouse callback
bool leftButton = false;
bool rightButton = false;
bool scroll = false;
bool clicked;
Vector3 mouseLocation, newLocation;
Vector3 rotateAxis;
float rotateAngle;
bool lightControl = false;
int chosenLight = 1;
#define MOUSE_SCROLL_UP 3
#define MOUSE_SCROLL_DOWN  4


void Window::initialize(void)
{
    //Setup the light
    Vector4 lightPos(0.0, 10.0, 15.0, 1.0);
    Globals::light.position = lightPos;
    Globals::light.quadraticAttenuation = 0.02;
    
    Globals::direc_light.position = lightPos;
    // direction should be a vector from origin to light position
    Globals::direc_light.direction = ((lightPos.toVector3()).negate()).toVector4(0.0);
    
    Vector4 spotPos(0.0, 0, 20, 1);
    Globals::spot_light.position = spotPos;
    Globals::spot_light.direction = ((spotPos.toVector3()).negate()).toVector4(0.0);
    
    Globals::point_light.position = spotPos;
    Globals::point_light.direction = ((spotPos.toVector3()).negate()).toVector4(0.0);
    
    //Initialize cube matrix:
    Globals::sphere.toWorld.identity();
    
    //Setup the cube's material properties
    /*Color color(0x23ff27ff);
    Globals::cube.material.color = color;*/

    //Color ballColor(0x1e99d6ea);
    // setMaterial: ambient, diffuse, specular, emission, shine
    Globals::sphere.material.setMaterial(Color::ambientDefault(), Color::red(), Color::white(), Color::black(), 50);
    
   // Color objColor(0xedeff1ff)
    
    Globals::bear.material.setMaterial(Color(0.19125, 0.0735, 0.0225), Color::red(), Color(0.256777, 0.137622, 0.086014), Color::emissionMaterialDefault(), 0.1 * 128.0);
    Globals::dragon.material.setMaterial(Color(0.02, 0.06, 0.04), Color::yellow(), Color(0.1, 0.1, 0.1), Color::emissionMaterialDefault(), 0.01 * 128.0);
    Globals::bunny.material.setMaterial(Color(0.25, 0.25, 0.25), Color::purple(), Color(0.774597, 0.774597, 0.774597), Color::emissionMaterialDefault(), 0.6 * 128);
    
    // keep the normal unchanged
    glEnable(GL_NORMALIZE);
}

//----------------------------------------------------------------------------
// Callback method called when system is idle.
// This is called at the start of every new "frame" (qualitatively)
void Window::idleCallback()
{
    //Set up a static time delta for update calls
    Globals::updateData.dt = 1.0/60.0;// 60 fps
    
    //Rotate cube; if it spins too fast try smaller values and vice versa
    //*************************//
    Globals::cube.spin(spin_speed);
    //Call the update function on cube
    Globals::cube.update(Globals::updateData);
    
    /*
    switch (mode) {
        case 0: //cube
            //Globals::cube.spin(spin_speed);
            Globals::cube.update(Globals::updateData);
            break;
            
        case 2:
           // Globals::bunny.spin(spin_speed);
            Globals::bunny.update(Globals::updateData);
            break;
            
        case 3:
            //Globals::dragon.spin(spin_speed);
            Globals::dragon.update(Globals::updateData);
            break;
            
        case 4:
            //Globals::bear.spin(spin_speed);
            Globals::bear.update(Globals::updateData);
            break;
            
        default:
            break;
    }*/
    
    /* print time elapse
    int startTime = glutGet(GLUT_ELAPSED_TIME);
    frame++;
    
    if (startTime - timebase > 1000) {
        std::string time = "current";
        fps = frame*1000.0/(startTime-timebase);
        printf("FPS:%4.2f",
               frame*1000.0/(startTime-timebase));
        printf("\n");
        timebase = startTime;
        frame = 0;
    } */
    
    //Call the display routine to draw the cube
    displayCallback();
}

//----------------------------------------------------------------------------
// Callback method called by GLUT when graphics window is resized by the user
void Window::reshapeCallback(int w, int h)
{
    width = w;                                                       //Set the window width
    height = h;                                                      //Set the window height
    glViewport(0, 0, w, h);                                          //Set new viewport size
    glMatrixMode(GL_PROJECTION);                                     //Set the OpenGL matrix mode to Projection
    glLoadIdentity();                                                //Clear the projection matrix by loading the identity
    gluPerspective(60.0, double(width)/(double)height, 1.0, 1000.0); //Set perspective projection viewing frustum
    Globals::raster.reshape(w, h);
}

//----------------------------------------------------------------------------
// Callback method called by GLUT when window readraw is necessary or when glutPostRedisplay() was called.
void Window::displayCallback()
{
    //Clear color and depth buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    //Set the OpenGL matrix mode to ModelView
    glMatrixMode(GL_MODELVIEW);
    
    //Push a matrix save point
    //This will save a copy of the current matrix so that we can
    //make changes to it and 'pop' those changes off later.
    glPushMatrix();
    
    //Replace the current top of the matrix stack with the inverse camera matrix
    //This will convert all world coordiantes into camera coordiantes
    glLoadMatrixf(Globals::camera.getInverseMatrix().ptr());
    
    //Bind the light to slot 0.  We do this after the camera matrix is loaded so that
    //the light position will be treated as world coordiantes
    //(if we didn't the light would move with the camera, why is that?)
    //Globals::direc_light.bind(1);
    //Globals::point_light.bind(2);
    Globals::spot_light.bind(3);
    
    //Switch case to decide whether display house or cube.
    switch (mode) {
        case 0:
            //Draw the cube!
            glEnable(GL_LIGHTING);
            //Globals::direc_light.bind(1);
            if (rast_mode == false) {
                Globals::cube.draw(Globals::drawData);
            }
            else {
                Globals::cube.rast_draw(Globals::drawData);
            }
            break;
            
        case 1:
            //Draw the house
            glDisable(GL_LIGHTING);
            if (rast_mode == false) {
                Globals::house.draw(Globals::drawData);
            }
            else {
                Globals::house.rast_Draw(Globals::drawData);
            }
            break;
            
        case 2:
            //Draw the bunny
            glEnable(GL_LIGHTING);
            
            if (rast_mode == false) {
                Globals::bunny.draw(Globals::drawData);
            }
            else {
                Globals::bunny.Rast_Draw(Globals::drawData);
            }
            
            break;
            
        case 3:
            //Draw the dragon
            glEnable(GL_LIGHTING);

            if (rast_mode == false) {
                Globals::dragon.draw(Globals::drawData);
            }
            else {
                Globals::dragon.Rast_Draw(Globals::drawData);
            }
            
            break;
        
        case 4:
            //Draw the bear
            glEnable(GL_LIGHTING);
        
            if (rast_mode == false) {
                Globals::bear.draw(Globals::drawData);
            }
            else {
                Globals::bear.Rast_Draw(Globals::drawData);
            }
            break;
            
        case 5:
            //Draw the sphere
            glEnable(GL_LIGHTING);
            //Globals::direc_light.bind(1);
            Globals::sphere.draw(Globals::drawData);
            break;
            
        default:
            break;
    }
    
    //lighting thing
    // point presentation for point_light
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    Matrix4 pt;
    pt.identity();
    pt = pt.makeTranslate(Globals::point_light.position.toVector3());
    glMultMatrixf(pt.ptr());
    glPushAttrib(GL_LIGHTING_BIT);
    glDisable(GL_LIGHTING);
    
    glColor3f(0, 0, 1.0);
    glutSolidSphere(0.5f,20,16);
    
    glEnable(GL_LIGHTING);
    glPopAttrib();
    glPopMatrix();
        
    //point presentation for spot_light
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    Matrix4 sp;
    sp.identity();
    sp = sp.makeTranslate(Globals::spot_light.position.toVector3());
    glMultMatrixf(sp.ptr());
    glPushAttrib(GL_LIGHTING_BIT);
    glDisable(GL_LIGHTING);
    
    glColor3f(1.0, 1.0, 0);
    glutSolidSphere(0.5f,20,16);
    
    glEnable(GL_LIGHTING);
    glPopAttrib();
    glPopMatrix();
    
    //Pop off the changes we made to the matrix stack this frame
    glPopMatrix();
    
    //Tell OpenGL to clear any outstanding commands in its command buffer
    //This will make sure that all of our commands are fully executed before
    //we swap buffers and show the user the freshly drawn frame
    glFlush();
    
    //Swap the off-screen buffer (the one we just drew to) with the on-screen buffer
    glutSwapBuffers();
}

void Window::keyboardCallback(unsigned char key_press, int x, int y) {
    Matrix4 change;
   
    switch (key_press) {
        // If press lower case c: change between counter clockwize and clockwize
        case 'c':
            spin_speed = -spin_speed;
            break;
            
        // If press lower case s: scale down the cube
        case 's':
            change.makeScale(0.9, 0.9, 0.9);
            if (mode == 0)
                Globals::cube.toWorld = Globals::cube.toWorld * change;
            else if (mode == 2)
                Globals::bunny.toWorld = Globals::bunny.toWorld * change;
            else if (mode == 3)
                Globals::dragon.toWorld = Globals::dragon.toWorld * change;
            else if (mode == 4)
                Globals::bear.toWorld = Globals::bear.toWorld * change;
            break;
            
        // If press upper case S: scale up the cube
        case 'S':
            change.makeScale(1.1, 1.1, 1.1);
            if (mode == 0)
                Globals::cube.toWorld = Globals::cube.toWorld * change;
            else if (mode == 2)
                Globals::bunny.toWorld = Globals::bunny.toWorld * change;
            else if (mode == 3)
                Globals::dragon.toWorld = Globals::dragon.toWorld * change;
            else if (mode == 4)
                Globals::bear.toWorld = Globals::bear.toWorld * change;
            break;
            
        // If press lower case x: move the cube left for small amount
        case 'x':
            change.makeTranslate(-1, 0, 0);
            if (mode == 0)
                Globals::cube.toWorld = change * Globals::cube.toWorld;
            else if (mode == 2)
                Globals::bunny.toWorld = change * Globals::bunny.toWorld;
            else if (mode == 3)
                Globals::dragon.toWorld = change * Globals::dragon.toWorld;
            else if (mode == 4)
                Globals::bear.toWorld = change * Globals::bear.toWorld;
            break;
        
        // If press upper case X: move the cube right for small amount
        case 'X':
            change.makeTranslate(1, 0, 0);
            if (mode == 0)
                Globals::cube.toWorld = change * Globals::cube.toWorld;
            else if (mode == 2)
                Globals::bunny.toWorld = change * Globals::bunny.toWorld;
            else if (mode == 3)
                Globals::dragon.toWorld = change * Globals::dragon.toWorld;
            else if (mode == 4)
                Globals::bear.toWorld = change * Globals::bear.toWorld;
            break;
           
        // If press lower case y: move the cube up for small amount
        case 'y':
            change.makeTranslate(0, -1, 0);
            if (mode == 0)
                Globals::cube.toWorld = change * Globals::cube.toWorld;
            else if (mode == 2)
                Globals::bunny.toWorld = change * Globals::bunny.toWorld;
            else if (mode == 3)
                Globals::dragon.toWorld = change * Globals::dragon.toWorld;
            else if (mode == 4)
                Globals::bear.toWorld = change * Globals::bear.toWorld;
            break;
            
        // If press upper case Y: move the cube down for small amount
        case 'Y':
            change.makeTranslate(0, 1, 0);
            if (mode == 0)
                Globals::cube.toWorld = change * Globals::cube.toWorld;
            else if (mode == 2)
                Globals::bunny.toWorld = change * Globals::bunny.toWorld;
            else if (mode == 3)
                Globals::dragon.toWorld = change * Globals::dragon.toWorld;
            else if (mode == 4)
                Globals::bear.toWorld = change * Globals::bear.toWorld;
            break;
            
        // If press lower case z: move the cube in for small amount
        case 'z':
            change.makeTranslate(0, 0, -1);
            if (mode == 0)
                Globals::cube.toWorld = change * Globals::cube.toWorld;
            else if (mode == 2)
                Globals::bunny.toWorld = change * Globals::bunny.toWorld;
            else if (mode == 3)
                Globals::dragon.toWorld = change * Globals::dragon.toWorld;
            else if (mode == 4)
                Globals::bear.toWorld = change * Globals::bear.toWorld;
            break;
            
        // If press upper case Z: move the cube out for small amount
        case 'Z':
            change.makeTranslate(0, 0, 1);
            if (mode == 0)
                Globals::cube.toWorld = change * Globals::cube.toWorld;
            else if (mode == 2)
                Globals::bunny.toWorld = change * Globals::bunny.toWorld;
            else if (mode == 3)
                Globals::dragon.toWorld = change * Globals::dragon.toWorld;
            else if (mode == 4)
                Globals::bear.toWorld = change * Globals::bear.toWorld;
            break;
            
        // On keypress of r: reset the cube to original state
        case 'r':
            if (mode == 0)
                Globals::cube.toWorld.identity();
            else if (mode == 2) {
                //Globals::bunny.toWorld.identity();
                Globals::bunny.reset(16.37);
            }
            else if (mode == 3){
                //Globals::dragon.toWorld.identity();
                Globals::dragon.reset(19.23);
            }
            else if (mode == 4)
                Globals::bear.reset(15.25);
            else if (mode == 5) {
                Globals::sphere.toWorld.identity();
            }
            break;
       
        // On keypress of lower case o: rotate the cube counterclockwise for a small angle
        case 'o':
            change.makeRotateZ(0.1);
            if (mode == 0)
                Globals::cube.toWorld = change * Globals::cube.toWorld;
            else if (mode == 2)
                Globals::bunny.toWorld = change * Globals::bunny.toWorld;
            else if (mode == 3)
                Globals::dragon.toWorld = change * Globals::dragon.toWorld;
            else if (mode == 4)
                Globals::bear.toWorld = change * Globals::bear.toWorld;
            break;
            
        // On keypress of upper case O: rotate the cube clockwise for a small angle
        case 'O':
            change.makeRotateZ(-0.1);
            if (mode == 0)
                Globals::cube.toWorld = change * Globals::cube.toWorld;
            else if (mode == 2)
                Globals::bunny.toWorld = change * Globals::bunny.toWorld;
            else if (mode == 3)
                Globals::dragon.toWorld = change * Globals::dragon.toWorld;
            else if (mode == 4)
                Globals::bear.toWorld = change * Globals::bear.toWorld;
            break;
            
        /* Project 3 rasterizer
        case 'e':
            if (rast_mode == false){
                rast_mode = true;
                Globals::raster.part = 0;
                Globals::raster.debug_mode = false;
            }
            else
                rast_mode = false;
            
            break;
         
        case '+':
            if (Globals::raster.part < 4 && (rast_mode == true)) {
                Globals::raster.part++;
            }
            break;
            
        case '-':
            if (Globals::raster.part > 0 && (rast_mode == true)) {
                Globals::raster.part--;
            }
            break;
            
        case 'd':
            Globals::raster.debug_mode = !Globals::raster.debug_mode;
            break;
         */
        case '1':
            lightControl = true;
            chosenLight = 1;  // directional light
            break;
            
        case '2':
            lightControl = true;
            chosenLight = 2; // point light
            break;
            
        case '3':
            lightControl = true;
            chosenLight = 3; // spot light
            break;
            
        case '0':
            lightControl = false;
            break;
            
        default:
            break;
    }
    
    /* Print position of the cube
    if (mode == 0) {
        Vector3 position = Vector3(Globals::cube.toWorld.get(3, 0),
                                   Globals::cube.toWorld.get(3, 1),
                                   Globals::cube.toWorld.get(3, 2));
        position.print("cube");
    }
    else if (mode == 2) {
        Vector3 position = Vector3(Globals::bunny.toWorld.get(3, 0),
                                   Globals::bunny.toWorld.get(3, 1),
                                   Globals::bunny.toWorld.get(3, 2));
        position.print("bunny");
    }
    else if (mode == 3) {
        Vector3 position = Vector3(Globals::dragon.toWorld.get(3, 0),
                                   Globals::dragon.toWorld.get(3, 1),
                                   Globals::dragon.toWorld.get(3, 2));
        position.print("dragon");
    }
    else if (mode == 4) {
        Vector3 position = Vector3(Globals::bear.toWorld.get(3, 0),
                                   Globals::bear.toWorld.get(3, 1),
                                   Globals::bear.toWorld.get(3, 2));
        position.print("bear");
    } */
}


//TODO: Keyboard callbacks!
void Window::functionKeyboardCallback(int key_press, int x, int y) {
    switch (key_press) {
        
        case GLUT_KEY_F1: // sphere
            mode = 5;
            Globals::camera.reset_Cam();
            //Globals::raster.drawMode = 0;
            obj = &Globals::sphere;
            break;
        /*
        case GLUT_KEY_F2: // house 1
            mode = 1;
            
            Globals::camera.e.set(0, 24.14, 24.14);
            Globals::camera.d.set(0, 0, 0);
            Globals::camera.up.set(0, 1, 0);
            
            Globals::camera.update();
            //Globals::raster.drawMode = 1;
            //rast_mode = false;
            break;
            
        case GLUT_KEY_F3: // house 2
            mode = 1;
            
            Globals::camera.e.set(-28.33, 11.66, 23.33);
            Globals::camera.d.set(-5, 0, 0);
            Globals::camera.up.set(0, 1, 0.5);
            
            Globals::camera.update();
            //Globals::raster.drawMode = 2;
           // rast_mode = false;
            break;*/
            
        case GLUT_KEY_F2: //bunny
            mode = 2;
            //Globals::raster.drawMode = 3;
            Globals::camera.reset_Cam();
            //rast_mode = false;
            obj = &Globals::bunny;
            
            if (Globals::bunny.initial == true) {
                Globals::bunny.reset(16.37);
                Globals::bunny.initial = false;
            }
            
            break;
            
        case GLUT_KEY_F3:  // dragon
            mode = 3;
            //Globals::raster.drawMode = 4;
            Globals::camera.reset_Cam();
            //rast_mode = false;
            obj = &Globals::dragon;
            
            if (Globals::dragon.initial == true) {
                Globals::dragon.reset(19.23);
                Globals::dragon.initial = false;
            }
           
            break;
            
        case GLUT_KEY_F4:  // bear
            mode = 4;
            //Globals::raster.drawMode = 5;
            Globals::camera.reset_Cam();
            //rast_mode = false;
            obj = &Globals::bear;
            
            if (Globals::bear.initial == true) {
                Globals::bear.reset(15.25);
                Globals::bear.initial = false;
            }
    
            break;
            
        case GLUT_KEY_F5: //cube
            mode = 0;
            Globals::camera.reset_Cam();
            obj = &Globals::cube;
            break;
            
        default:
            break;
    }
}
//TODO: Function Key callbacks!

void Window::mouseCallback(int button, int state, int x, int y) {
    Matrix4 change;
    
    switch (button) {
        case GLUT_LEFT_BUTTON:
            if (state == GLUT_DOWN) {
                leftButton = true;
                rightButton = false;
                scroll = false;
                clicked = true;
              //  std::cout << "x= " << x << " y=" << y << std::endl;
                mouseLocation = trackBallMapping(x, y);
              //  mouseLocation.print("left");
            }
            break;
            
        case GLUT_RIGHT_BUTTON:
            if (state == GLUT_DOWN) {
                leftButton = false;
                rightButton = true;
                scroll = false;
                clicked = true;
              //  std::cout << "x= " << x << " y=" << y << std::endl;
                mouseLocation = trackBallMapping(x, y);
             //   mouseLocation.print("right");
            }
            break;
            
        case MOUSE_SCROLL_DOWN:
            if (lightControl == false) {
                scroll = true;
                change.makeScale(1.05, 1.05, 1.05);
                obj -> toWorld = (obj -> toWorld) * change;
            }
            else {
                if (chosenLight == 2) {
                    Vector4 pointLoc = Globals::point_light.position;
                    pointLoc = pointLoc + (((pointLoc.toVector3()).normalize())*1.5).toVector4(0.0);
                    
                    Globals::point_light.position = pointLoc;
                    Globals::point_light.direction = ((pointLoc.toVector3().negate()).toVector4(0));
                }
                else if (chosenLight == 3) {
                    Vector4 spotLoc = Globals::spot_light.position;
                    spotLoc = spotLoc + (((spotLoc.toVector3()).normalize())*1.2).toVector4(0.0);
                    
                    Globals::spot_light.position = spotLoc;
                    Globals::spot_light.direction = ((spotLoc.toVector3().negate()).toVector4(0));
                }
            }
            break;
            
        case MOUSE_SCROLL_UP:
            if (lightControl == false) {
                scroll = true;
                change.makeScale(0.95, 0.95, 0.95);
                obj -> toWorld = (obj -> toWorld) * change;
            }
            else {
                if (chosenLight == 2) {
                    Vector4 pointLoc = Globals::point_light.position;
                    pointLoc = pointLoc + (((pointLoc.toVector3()).normalize())*0.5).toVector4(0.0);
                    
                    Globals::point_light.position = pointLoc;
                    Globals::point_light.direction = ((pointLoc.toVector3().negate()).toVector4(0));
                }
                else if (chosenLight == 3) {
                    Vector4 spotLoc = Globals::spot_light.position;
                    spotLoc = spotLoc + (((spotLoc.toVector3()).normalize())*0.8).toVector4(0.0);
                    
                    Globals::spot_light.position = spotLoc;
                    Globals::spot_light.direction = ((spotLoc.toVector3().negate()).toVector4(0));
                }
            }
            break;
            
        default:
            break;
    }
    
}
//TODO: Mouse callbacks!

void Window::mouseMotionCallback(int x, int y) {
    newLocation = trackBallMapping(x, y);
   // std::cout << "newloc= " << newLocation[0] << newLocation[1]
     //           << " mouseLoc= " << mouseLocation[0] << mouseLocation[1] << std::endl;
    Vector3 moveDistance = newLocation - mouseLocation;
    Matrix4 change;
    
    if (lightControl == false) {
        if (leftButton == true) {
          //  std::cout << "rotate left button" << std::endl;
            rotateAxis = mouseLocation.cross(newLocation);
            rotateAngle = asinf(rotateAxis.magnitude() / (mouseLocation.magnitude() * newLocation.magnitude()));
            
            rotateAxis = rotateAxis.normalize();
            
            change.makeRotateArbitrary(rotateAxis, rotateAngle);
            
            obj -> toWorld = change * (obj -> toWorld);
        }
        else if (rightButton == true) {
            //std::cout << "resize right button" << std::endl;
            change.makeTranslate(10 * (newLocation[0] - mouseLocation[0]), 10*(newLocation[1] - mouseLocation[1]), 0);
            obj -> toWorld = change * (obj -> toWorld);
        }
    }
    else {
        // Adjust directional light
        if (chosenLight == 1) {
            rotateAxis = mouseLocation.cross(newLocation);
            rotateAxis = rotateAxis.normalize();
            
            rotateAngle = asinf(rotateAxis.magnitude() / (mouseLocation.magnitude() * newLocation.magnitude()));
        
            change.makeRotateArbitrary(rotateAxis,rotateAngle);
            
            Globals::direc_light.position = change * Globals::direc_light.position;
            Globals::direc_light.direction = ((Globals::direc_light.position.toVector3().negate()).toVector4(0));
        }
        
        // Adjust point light
        else if (chosenLight == 2) {
            if (leftButton == true) {
                rotateAxis = mouseLocation.cross(newLocation);
                rotateAxis = rotateAxis.normalize();
                
                rotateAngle = asinf(rotateAxis.magnitude() / (mouseLocation.magnitude() * newLocation.magnitude()));
                
                change.makeRotateArbitrary(rotateAxis,rotateAngle);
                
                Globals::point_light.position = change * Globals::point_light.position;
                Globals::point_light.direction = ((Globals::point_light.position.toVector3().negate()).toVector4(0));
            }
            /*
            if (scroll == true) {
                float gap = newLocation[1] - mouseLocation[1];
                Vector4 pointLoc = Globals::point_light.position;
                
                pointLoc = pointLoc + (((pointLoc.toVector3()).normalize())*(gap / 10)).toVector4(0.0);
                
                Globals::point_light.position = pointLoc;
                Globals::point_light.direction = ((pointLoc.toVector3().negate()).toVector4(0));
            }*/
        }
        
        // Adjust spot light
        else if (chosenLight == 3) {
            if (leftButton == true) {
                rotateAxis = mouseLocation.cross(newLocation);
                rotateAxis = rotateAxis.normalize();
                
                rotateAngle = asinf(rotateAxis.magnitude() / (mouseLocation.magnitude() * newLocation.magnitude()));
                
                change.makeRotateArbitrary(rotateAxis,rotateAngle);
                
                Globals::spot_light.position = change * Globals::spot_light.position;
                Globals::spot_light.direction = ((Globals::spot_light.position.toVector3().negate()).toVector4(0));
            }
            
            /*
            if (scroll == true) {
                float move = newLocation[1] - mouseLocation[1];
                
                Vector4 spotLoc = Globals::spot_light.position;
                spotLoc = spotLoc + (((spotLoc.toVector3()).normalize())*(move / 10)).toVector4(0.0)  ;

                Globals::spot_light.position = spotLoc;
                Globals::spot_light.direction = ((spotLoc.toVector3().negate()).toVector4(0));
            }*/
            
            if (rightButton == true) {
                float yShift = newLocation[1] - mouseLocation[1];
                float xShift = newLocation[0] - mouseLocation[0];
                
                Globals::spot_light.coneAngle = Globals::spot_light.coneAngle * exp(yShift / 50);
                Globals::spot_light.exponent = Globals::spot_light.exponent * exp(-xShift / 50);
                
            }
        }
    }
    mouseLocation = newLocation;
}
//TODO: Mouse Motion callbacks!

Vector3 Window::trackBallMapping(int x, int y) {
    Vector3 pos;
    float d;
    
    pos[0] = (2.0 * x - width) / height;
    pos[1] = (height - 2.0 * y) / height;
    pos[2] = 0;
    
    d = pos.magnitude();
    d = (d < 1.0) ? d : 1.0;
    pos[2] = sqrtf(1.001 - d * d);
    pos.normalize();

    return pos;
}


void Window::mouseWheelCallback(int wheel, int direction, int x, int y) {
    Matrix4 change;
    
    std::cout << "wheel change" << std::endl;
    
    if (direction > 0) {
        // zoom out
        change.makeScale(1.1, 1.1, 1.1);
        
        obj -> toWorld = (obj -> toWorld) * change;
    }
    else {
        // zoom in
        change.makeScale(0.9, 0.9, 0.9);
        
        obj -> toWorld = (obj -> toWorld) * change;
    }
}

















