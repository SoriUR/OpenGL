//
//  platform.cpp
//  Game
//
//  Created by Юрий Сорокин on 13/10/2017.
//  Copyright © 2017 ITMO University. All rights reserved.
//

#ifndef platform_h
#define platform_h

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
#include "Shader.cpp"

class platform
{
    GLuint platformEBO,platformVAO,platformVBO;
    GLuint size=24;
    glm::vec3 moveVertex = glm::vec3(0, -0.55, 0.0f);
    GLuint texture;
    GLfloat scaleSize = 0.22;
public:
    
    platform(){
        
        GLfloat platformVertexes[]=
        {    //vertex                //tex
            -1.0f, 0.2f, 0.0f,       1.0f,   0.421f,
           -0.67, -0.8f, 0.0f,       0.845f, 1.0f,
            -0.34, 0.2f, 0.0f,       0.665f, 0.421f,
            0.0f, -0.8f, 0.0f,       0.51f,  1.0f,
            0.34f, 0.2f, 0.0f,       0.335f, 0.421f,
            0.67f, -0.8f, 0.0f,       0.135f, 1.0f,
            1.0f, 0.2f, 0.0f,       0.0f,   0.421f,
            
            -0.7f, 0.7f, 0.0f,       0.8f, 0.0f,
            0.7f, 0.7f, 0.0f,       0.19f, 0.0f
            
        };
        
        
        GLuint platformIndexes[]=
        {
            0,1,2,
            1,2,3,
            2,3,4,
            3,4,5,
            4,5,6,
            //
            0,7,2,
            7,2,8,
            2,8,6
            
        };
        
        glGenVertexArrays( 1, &platformVAO );
        glGenBuffers(1, &platformVBO);
        glGenBuffers(1, &platformEBO);
        
        glBindVertexArray( platformVAO );
        
        glBindBuffer( GL_ARRAY_BUFFER, platformVBO );
        glBufferData( GL_ARRAY_BUFFER, sizeof( platformVertexes ), platformVertexes, GL_STREAM_DRAW);
        
        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, platformEBO );
        glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof( platformIndexes ), platformIndexes, GL_STREAM_DRAW );
        
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);
        
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
        glEnableVertexAttribArray(1);
        
        glBindVertexArray( 0 );
        
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        int width, height;
        unsigned char* image = SOIL_load_image("/Users/u40/Developer/Game/Game/boxTexture.jpg", &width, &height, 0, SOIL_LOAD_RGB);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
        glGenerateMipmap(GL_TEXTURE_2D);
        SOIL_free_image_data(image);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    
    
    void drawObject(Shader shader){
        shader.Use();
        glm::mat4 transform;
        transform = glm::translate(transform,moveVertex);
        transform = glm::scale(transform, glm::vec3(scaleSize, scaleSize, 0.0));
        GLint transformLoc = glGetUniformLocation(shader.Program, "transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
        
        glBindTexture(GL_TEXTURE_2D,texture);
        glBindVertexArray( platformVAO );
        glDrawElements(GL_TRIANGLES, size, GL_UNSIGNED_INT, 0);
        glBindVertexArray( 0 );
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    
    void deleteBuffers(){
        glDeleteVertexArrays( 1, &platformVAO );
        glDeleteBuffers( 1, &platformVBO );
        glDeleteBuffers( 1, &platformEBO );
    }
    
    void Move(GLfloat step){
        GLfloat a = moveVertex.x+step;
        GLfloat border = -1+scaleSize;
        if(a<border)
            a=border;
        if(a>-border)
            a=-border;
        moveVertex = glm::vec3(a, moveVertex.y, 0.0f);
    }
    
    glm::vec3 getVec(){
        return moveVertex;
    }
    
    GLfloat getScale(){
        return scaleSize;
    }
};

#endif

