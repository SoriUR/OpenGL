
#ifndef heartObject_h
#define heartObject_h

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


class heartObject
{
    GLuint heartVAO, heartVBO, heartEBO;
    
    GLuint k;
    GLfloat xCord,y1,y2,y3,y4,y5;
    
    GLuint texture;
    GLfloat heartVertexes[50] =
    {
        //vert                  //tex
        0.0f, -1.0f, 0.0f,      0.5f, 1.025f,
        1.0f, 0.0f, 0.0f,       1.0f, 0.45f,
        1.0f, 0.5f, 0.0f,       1.0f, 0.25f,
        0.6f, 0.8f, 0.0f,       0.8f, 0.1f,
        0.25f, 0.8f, 0.0f,      0.63f, 0.1f,
        0.0f, 0.5f, 0.0f,       0.5f, 0.25f,
        -0.25f, 0.8f, 0.0f,     0.37f, 0.1f,
        -0.6f, 0.8f, 0.0f,      0.2f, 0.1f,
        -1.0f, 0.5f, 0.0f,      0.0f, 0.25f,
        -1.0f, 0.0f, 0.0f,      0.0f, 0.45f
    };
    
    GLuint heartIndecies[24] =
    {
        1,0,9,
        1,5,9,
        1,2,3,
        1,3,5,
        3,4,5,
        5,6,7,
        5,7,9,
        7,8,9
    };
public:
    
    heartObject(GLuint k){
        glGenBuffers(k, &heartVBO);
        glGenBuffers(k, &heartEBO);
        glGenVertexArrays( k, &heartVAO );
        
        glBindVertexArray( heartVAO );
        
        glBindBuffer( GL_ARRAY_BUFFER, heartVBO );
        glBufferData( GL_ARRAY_BUFFER, sizeof( heartVertexes ), heartVertexes, GL_STATIC_DRAW);
        
        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, heartEBO );
        glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof( heartIndecies ), heartIndecies, GL_STATIC_DRAW);
        
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);
        
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
        glEnableVertexAttribArray(1);
        
        glBindBuffer( GL_ARRAY_BUFFER, 0 );
        
        glBindVertexArray( 0 );
        glGenTextures(k, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        int width, height;
        unsigned char* image = SOIL_load_image("/Users/u40/Developer/Game/Game/heartTexture.jpg", &width, &height, 0, SOIL_LOAD_RGB);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
        glGenerateMipmap(GL_TEXTURE_2D);
        SOIL_free_image_data(image);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    
    
    void drawObject(GLfloat a,Shader heartShader){
        heartShader.Use();
        glm::mat4 transform;
        transform = glm::translate(transform, glm::vec3(a, 0.9, 0.0f));
        transform = glm::scale(transform, glm::vec3(0.05, 0.05, 0.05));
        GLint transformLoc = glGetUniformLocation(heartShader.Program, "transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
        
        glBindTexture(GL_TEXTURE_2D, texture);
        glBindVertexArray( heartVAO );
        glDrawElements(GL_TRIANGLES, 24, GL_UNSIGNED_INT, 0);
        glBindVertexArray( 0 );
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    
    void deleteBuffers(){
        glDeleteVertexArrays( k, &heartVAO );
        glDeleteBuffers( k, &heartVBO );
        glDeleteBuffers( k, &heartEBO );
    }
    
};

#endif /* circleObject_h */


