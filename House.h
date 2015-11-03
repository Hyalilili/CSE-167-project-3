//
//  House.hpp
//  CSE167 Spring 2015 Starter Code
//
//  Created by Gaojie Li on 10/7/15.
//  Copyright © 2015 RexWest. All rights reserved.
//

#ifndef House_h
#define House_h

#include "Drawable.h"
#include "Vector3.h"
#include "Globals.h"

class House:public Drawable {
    
public:
    int nVerts;
    Vector3 a, b, c;
   // Vector3 n1, n2, n3;
    
    House();
    // Destructor
    virtual ~House(void);
    // Draw method
    virtual void draw(DrawData&);
    virtual void rast_Draw(DrawData&);
};

#endif /* House_h */
