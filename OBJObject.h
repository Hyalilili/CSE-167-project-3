#ifndef CSE167_OBJObject_h
#define CSE167_OBJObject_h

#include <iostream>
#include <vector>
#include <string>
#include "Vector3.h"
#include "Drawable.h"
#include "Rasterizer.h"

struct Face
{
    int vertexIndices[3];
    int normalIndices[3];
    //Add more members as necessary
    int colorIndices[3];
};

class OBJObject : public Drawable
{
    
protected:
    
    //Storage vectors
    std::vector<Vector3*>* vertices;
    std::vector<Vector3*>* normals;
    //Color
    std::vector<Vector3*>* colors;
    
    std::vector<Face*>* faces;
    
    //Helper functions
    std::vector<std::string>& split(const std::string&, char, std::vector<std::string>&);
    std::vector<std::string> split(const std::string&, char);
    
    //Parse
    void parse(std::string&);
    
    bool hasColor;
    
public:
    
    OBJObject(std::string);
    virtual ~OBJObject(void);
    
    float minX = 10000;
    float maxX = 0;
    float minY = 10000;
    float maxY = 0;
    float minZ = 10000;
    float maxZ = 0;
    float centerX, centerY, centerZ;
    bool initial = true;
    
    virtual void draw(DrawData&);
    virtual void Rast_Draw(DrawData&);
    Vector3 vect1_norm, vect2_norm, vect3_norm;
    virtual void update(UpdateData&);
    virtual void spin(float);
    virtual void center();
    virtual void scale(float index);
    virtual void reset(float index);
};

#endif
