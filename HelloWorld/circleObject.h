//
//  circleObject.h
//  HelloWorld
//
//  Created by Юрий Сорокин on 05/10/2017.
//  Copyright © 2017 ITMO University. All rights reserved.
//

#ifndef circleObject_h
#define circleObject_h

#include <iostream>
#include <unistd.h>
#include <cmath>
#include <stdlib.h>

//GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

class circleObject
{
    GLuint circleVAO, circleVBO;
    
    GLfloat xshift=0.0f;
    GLuint k;
    
public:
    GLboolean caught=true;
    GLfloat yshift=1.1f;
    GLfloat circleVertexes[363];
    GLuint fragmentCount=120;

    
    circleObject(GLuint k){
        xshift=getRand();
        this->k=k;
        glGenBuffers(k, &circleVBO);
        glGenVertexArrays( k, &circleVAO );
    }
    
    void updateVertexes(){
        int a=0;
        for (int i=0; i < fragmentCount*3+3; i+=3)
        {
            double angle = 2 * 3.14 * a++ / fragmentCount;
            circleVertexes[i]=xshift+0.1f*round(cos(angle)*100)/100;
            circleVertexes[i+1]=yshift+0.1f*round(sin(angle)*100)/100;
            circleVertexes[i+2]=0.0f;
        }
        glBindVertexArray( circleVAO );
        
        glBindBuffer( GL_ARRAY_BUFFER, circleVBO );
        glBufferData( GL_ARRAY_BUFFER, sizeof( circleVertexes ), circleVertexes, GL_STREAM_DRAW);
        
        glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof( GLfloat ), ( GLvoid * ) 0 );
        glEnableVertexAttribArray( 0 );
        
        glBindBuffer( GL_ARRAY_BUFFER, 0 );
        
        glBindVertexArray( 0 );
    }
    
    
    void drawObject(){
        updateVertexes();
        glBindVertexArray( circleVAO );
        glDrawArrays(GL_TRIANGLE_FAN, 0, sizeof(circleVertexes));
        glBindVertexArray( 0 );
    }
    
    void collision(){
//        glDeleteVertexArrays( k, &circleVAO );
//        glDeleteBuffers( k, &circleVBO );
//        for(int i=0;i<fragmentCount*3+2;i++){
//            circleVertexes[i]=0;
//        }
        xshift=getRand();
        yshift=1.1f;
    }
    
    GLfloat getRand(){
        return ((rand() % 17)-8)*0.1;
    }
    
};

#endif /* circleObject_h */
