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

//SOIL
#include "SOIL.h"


class circleObject
{
    GLuint circleVAO, circleVBO;
    
    GLfloat xshift, yshift;
    GLuint k;
    GLuint texture;
    GLfloat circleVertexes[605];
    GLuint fragmentCount=120;
public:
    
    circleObject(GLuint k,GLfloat startY){
        xshift=getRand();
        yshift=startY;
        
        this->k=k;
        
        glGenBuffers(k, &circleVBO);
        glGenVertexArrays( k, &circleVAO );
        
        glBindVertexArray( circleVAO );
        
        glBindBuffer( GL_ARRAY_BUFFER, circleVBO );
        glBufferData( GL_ARRAY_BUFFER, sizeof( circleVertexes ), NULL, GL_STREAM_DRAW);
        
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);
        
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
        glEnableVertexAttribArray(1);
        
        glBindBuffer( GL_ARRAY_BUFFER, 0 );
        
        glBindVertexArray( 0 );
        
        glGenTextures(k, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        int width, height;
        unsigned char* image = SOIL_load_image("/Users/u40/Desktop/Game/Game/ballTexture.jpg", &width, &height, 0, SOIL_LOAD_RGB);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
        glGenerateMipmap(GL_TEXTURE_2D);
        SOIL_free_image_data(image);
        glBindTexture(GL_TEXTURE_2D, 0);

        circleUpgrade();
        
    }
    
    void circleUpgrade(){
        int a=0;
        for (int i=0; i < (fragmentCount+1)*5; i+=5)
        {
            double angle = 2 * 3.14 * a++ / fragmentCount;
            circleVertexes[i]=xshift+0.06f*round(cos(angle)*100)/100;
            circleVertexes[i+1]=0.5+yshift+0.06f*round(sin(angle)*100)/100;
            circleVertexes[i+2]=0.0f;
            
            circleVertexes[i+3]=0.5+0.5f*round(cos(angle)*100)/100;
            circleVertexes[i+4]=0.5+0.5f*round(sin(angle)*100)/100;
        }
        
        glBindBuffer(GL_ARRAY_BUFFER, circleVBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(circleVertexes),circleVertexes);
        glBindBuffer(GL_ARRAY_BUFFER, circleVBO);
    }
    
    
    void drawObject(){
        glBindTexture(GL_TEXTURE_2D, texture);
        glBindVertexArray( circleVAO );
        glDrawArrays(GL_TRIANGLE_FAN, 0, sizeof(circleVertexes));
        glBindVertexArray( 0 );
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    
    GLfloat getRand(){
        return ((rand() % 17)-8)*0.1;
    }
    
    void collisionCheck(GLfloat platformVertexes[]){
        GLuint k=fragmentCount/4*9;
        if(((circleVertexes[k+1]<(platformVertexes[36])) && (circleVertexes[k+1]>platformVertexes[1])) //y
           && (circleVertexes[k]>platformVertexes[0]) && (circleVertexes[k]<platformVertexes[30])) //x
        {
            xshift=getRand();
            yshift=1.1f;
//            glDeleteVertexArrays( k, &circleVAO );
//            glDeleteBuffers( k, &circleVBO );
//            return true;
        }
//        return false;
    }
    
    GLfloat getY(){
        return yshift;
    }
    
    void setY(GLfloat newY)
    {
        yshift=newY;
        circleUpgrade();
    }
    
    
};

#endif /* circleObject_h */

