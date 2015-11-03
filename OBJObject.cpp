
#ifdef __APPLE__
    #include <GLUT/glut.h>
#else
    #include <GL/glut.h>
#endif

#include "Window.h"
#include "math.h"
#include <sstream>
#include <fstream>
#include "Globals.h"
#include <algorithm>
using namespace std;
#define deleteVector(__type__, __vect__) do {\
                                   std::vector<__type__>::iterator iter; \
                                   std::vector<__type__>::iterator end; \
                                   iter = __vect__->begin();\
                                   end = __vect__->end();\
                                   while(iter != end) delete (*(iter++));\
                                   delete __vect__;\
                               } while(false)

int obj_index;
/*float minX = 10000;
float maxX = 0;
float minY = 10000;
float maxY = 0;
float centerX, centerY;*/
Matrix4 centerChange;

OBJObject::OBJObject(std::string filename) : Drawable()
{
    this->vertices = new std::vector<Vector3*>();
    this->normals = new std::vector<Vector3*>();
    this->faces = new std::vector<Face*>();
    this->colors = new std::vector<Vector3*>();
    
    parse(filename);
}

OBJObject::~OBJObject()
{
    //Delete any dynamically allocated memory/objects here
    
    deleteVector(Vector3*, vertices);
    deleteVector(Vector3*, normals);
    deleteVector(Vector3*, colors);
    deleteVector(Face*, faces);
}

void OBJObject::spin(float radians)
{
    Matrix4 rotation;
    rotation.makeRotateY(radians);
    
    toWorld = toWorld * rotation;
}

void OBJObject::draw(DrawData& data)
{
    material.apply();
    
    glMatrixMode(GL_MODELVIEW);
    
    glPushMatrix();
    glMultMatrixf(toWorld.ptr());
    
    glBegin(GL_TRIANGLES);
    
    
    //Loop through the faces
    //For each face:
    //  Look up the vertices, normals (if they exist), and texcoords (if they exist)
    //  Draw them as triplets:
    
    //      glNorm(normals->at(face.normalIndices[0]))
    //      glVert(vertices->at(face.vertexIndices[0]))
    //      Etc.
    //
    for (int i = 0; i < faces -> size(); i++) {
        Face face = *faces -> at(i);
        
       // glNormal3fv(normals -> at((*faces)[i] -> normalIndices[0]) -> ptr());
    //    glVertex3fv(vertices -> at((*faces)[i] -> vertexIndices[0]) -> ptr());
    
        for (int j = 0; j < 3; j++) {
            Vector3 vector_normal = *normals -> at(face.normalIndices[j] - 1);
            vector_normal = vector_normal.normalize();
            glNormal3f(vector_normal.m[0], vector_normal.m[1], vector_normal.m[2]);
            
            Vector3 vector_vertex = *vertices -> at(face.vertexIndices[j] - 1);
            glVertex3f(vector_vertex.m[0],vector_vertex.m[1],vector_vertex.m[2]);
            
            if (hasColor) {
                Vector3 vector_color = *colors -> at(face.vertexIndices[j] - 1);
                glColor3f(vector_color.m[0], vector_color.m[1], vector_color.m[2]);
               // glColor3fv(colors -> at((*faces)[i] -> colorIndices[0]) -> ptr());
            }
            else {
                glColor3f(0.752941, 0.752941, 0.752941);
            }
        }
    }
    
    glEnd();
    
    glPopMatrix();
}

// Draw method with rasterization
void OBJObject::Rast_Draw(DrawData& data) {
    Globals::raster.clearBuffer();
    // declare 3 vertices of each drawing triangle
    Vector4 vertex1, vertex2, vertex3;
    Vector3 vertex;
    //srand(0);
    
    // loop through all faces
    for (int i = 0; i < faces -> size(); i++) {
        Face face = *faces -> at(i);
        
        vect1_norm = *normals -> at(face.normalIndices[0] - 1);
        vect1_norm = vect1_norm.normalize();
        vect2_norm = *normals -> at(face.normalIndices[1] - 1);
        vect2_norm = vect2_norm.normalize();
        vect3_norm = *normals -> at(face.normalIndices[2] - 1);
        vect3_norm = vect3_norm.normalize();
        
        vertex = *(vertices -> at(face.vertexIndices[0] - 1));
        vertex1.set(vertex[0], vertex[1], vertex[2], 1);
        vertex1 = toWorld * vertex1;
        
        vertex = *(vertices -> at(face.vertexIndices[1] - 1));
        vertex2.set(vertex[0], vertex[1], vertex[2], 1);
        vertex2 = toWorld * vertex2;
        
        vertex = *(vertices -> at(face.vertexIndices[2] - 1));
        vertex3.set(vertex[0], vertex[1], vertex[2], 1);
        vertex3 = toWorld * vertex3;
        
        if (Globals::raster.part == 0) {
            Globals::raster.rasterizeVertex(vertex1);
            Globals::raster.rasterizeVertex(vertex2);
            Globals::raster.rasterizeVertex(vertex3);
        }
        else {
            Globals::raster.rasterizeTriangle(vertex1, vertex2, vertex3);
        }
    }
    
    Globals::raster.display();
}


void OBJObject::update(UpdateData& data)
{
    //
}

void OBJObject::parse(std::string& filename)
{
    std::ifstream infile(filename);
    std::string line;
    std::vector<std::string> tokens;
    std::vector<std::string> face_token;
    std::string token;
    hasColor = false;
    
    int lineNum = 0;
    
    
    std::cout << "Starting parse..." << std::endl;
    
    //While all your lines are belong to us
    while (std::getline(infile, line))
    {
        //Progress
        if(++lineNum % 10000 == 0)
            std::cout << "At line " << lineNum << std::endl;
        
        //Split a line into tokens by delimiting it on spaces
        //"Er Mah Gerd" becomes ["Er", "Mah", "Gerd"]
        tokens.clear();
        tokens = split(line, ' ', tokens);
        
        //If first token is a v then it gots to be a vertex
        if(tokens.at(0).compare("v") == 0)
        {
            //Parse the vertex line
            float x = std::stof(tokens.at(1));
            float y = std::stof(tokens.at(2));
            float z = std::stof(tokens.at(3));
            
            // find x minimum and maximum
            if (x > maxX) {
                maxX = x;
            }
            else if (x < minX) {
                minX = x;
            }
            
            // find y minimum and maximum
            if (y > maxY) {
                maxY = y;
            }
            else if (y < minY) {
                minY = y;
            }
            
            if (z > maxZ) {
                maxZ = z;
            }
            else if (z < minZ) {
                minZ = z;
            }
            
            vertices->push_back(new Vector3(x, y, z));
            
            // Parse the color index
            if (tokens.size() > 4) {
                hasColor = true;
                
                float c1 = std::stof(tokens.at(4));
                float c2 = std::stof(tokens.at(5));
                float c3 = std::stof(tokens.at(6));
                
                colors -> push_back(new Vector3(c1, c2, c3));
            }
        }
        else if(tokens.at(0).compare("vn") == 0)
        {
            //Parse the normal line
            float x = std::stof(tokens.at(1));
            float y = std::stof(tokens.at(2));
            float z = std::stof(tokens.at(3));
            
            normals->push_back(new Vector3(x, y, z));
        }
        else if(tokens.at(0).compare("f") == 0)
        {
            Face* face = new Face;
            
            //Parse the face line
            //1st vertex
            face_token = split(tokens.at(1), '/');
            face -> vertexIndices[0] = std::stoi(face_token.at(0));
            face -> normalIndices[0] = std::stoi(face_token.at(2));
            
            //2nd vertex
            face_token = split(tokens.at(2), '/');
            face -> vertexIndices[1] = std::stoi(face_token.at(0));
            face -> normalIndices[1] = std::stoi(face_token.at(2));
            
            //3rd vertex
            face_token = split(tokens.at(3), '/');
            face -> vertexIndices[2] = std::stoi(face_token.at(0));
            face -> normalIndices[2] = std::stoi(face_token.at(2));
            
            faces -> push_back(face);
        }
        else if(tokens.at(0).compare("How does I are C++?!?!!") == 0)
        {
            //Parse as appropriate
            //There are more line types than just the above listed
            //See the Wavefront Object format specification for details
        }
        
    }
    
    std::cout << "Done parsing." << std::endl;
}

// Center the object to the Window's center
void OBJObject::center() {
    //std::cout << "maxX= " << maxX << " maxY= " << maxY << std::endl;
    //std::cout << "minX= " << minX << " minY= " << minY << std::endl;
    
    centerX = (maxX + minX) / 2;
    centerY = (maxY + minY) / 2;
    centerZ = (maxZ + minZ) / 2;
    
    //std::cout << "centerX = " << centerX << " centerY = " << centerY << std::endl;
    
    centerChange.makeTranslate(-centerX, -centerY, -centerZ);
    
    this -> toWorld.identity();
    this -> toWorld = centerChange * this -> toWorld;// * centerChange;
}

void OBJObject::scale(float index) {
    Matrix4 change;
    Matrix4 size;
    float sizeScale;
    
    float xSize = maxX - minX;
    float ySize = maxY - minY;
    float zSize = maxZ - minZ;
    
    sizeScale = max(max(xSize, ySize), zSize) * 0.5f;

    size.makeScale(1/(sizeScale));
    change.makeScale(index*tan(30*3.1415/180));
    
    this -> toWorld = size * this -> toWorld;
    this -> toWorld = change * this -> toWorld;
}

void OBJObject::reset(float index) {
    center();
    scale(index);
}

//Split functions from the interwebs
//http://stackoverflow.com/questions/236129/split-a-string-in-c
std::vector<std::string>& OBJObject::split(const std::string &s, char delim, std::vector<std::string> &elems)
{
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim))
    {
        elems.push_back(item);
    }
    return elems;
}

std::vector<std::string> OBJObject::split(const std::string &s, char delim)
{
    std::vector<std::string> elems;
    split(s, delim, elems);
    return elems;
}
