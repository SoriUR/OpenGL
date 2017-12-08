
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
#include "Shader.cpp"
#include "platform.cpp"

class circleObject
{
    GLuint circleVAO, circleVBO;
    GLuint k;
    glm::vec3 moveVertex;
    GLuint texture;
    GLfloat circleVertexes[610];
    GLuint fragmentCount=120;
    platform* p;
public:
    
    circleObject(GLuint k,platform* p){
        this->p=p;
        this->k=k;
       
        circleVertexes[0]=0;
        circleVertexes[1]=0;
        circleVertexes[2]=0.0f;
        
        circleVertexes[3]=0.5;
        circleVertexes[4]=0.5;
        
        int a=0;
        for (int i=5; i < (fragmentCount+2)*5; i+=5)
        {
            double angle = 2 * 3.14 * a++ / fragmentCount;
            circleVertexes[i]=round(cos(angle)*100)/100;
            circleVertexes[i+1]=round(sin(angle)*100)/100;
            circleVertexes[i+2]=0.0f;
            
            circleVertexes[i+3]=0.5+0.5f*round(cos(angle)*100)/100;
            circleVertexes[i+4]=0.5+0.5f*round(sin(angle)*100)/100;
        }
        
        glGenBuffers(k, &circleVBO);
        glGenVertexArrays( k, &circleVAO );
        
        glBindVertexArray( circleVAO );
        
        glBindBuffer( GL_ARRAY_BUFFER, circleVBO );
        glBufferData( GL_ARRAY_BUFFER, sizeof( circleVertexes ), circleVertexes, GL_STATIC_DRAW);
        
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);
        
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
        glEnableVertexAttribArray(1);
        
        glBindBuffer( GL_ARRAY_BUFFER, 0 );
        
        glBindVertexArray( 0 );
        
        glGenTextures(k, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        int width, height;
        unsigned char* image = SOIL_load_image("/Users/u40/Developer/Game/Game/ballTexture.jpg", &width, &height, 0, SOIL_LOAD_RGB);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
        glGenerateMipmap(GL_TEXTURE_2D);
        SOIL_free_image_data(image);
        glBindTexture(GL_TEXTURE_2D, 0);
        
        reGenerate(k);
    }
    
    
    void drawObject(Shader shader){
        shader.Use();
        glm::mat4 transform;
        transform = glm::translate(transform,moveVertex);
        transform = glm::scale(transform, glm::vec3(0.05, 0.05, 0.05));
        GLint transformLoc = glGetUniformLocation(shader.Program, "transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
        
        glBindTexture(GL_TEXTURE_2D, texture);
        glBindVertexArray( circleVAO );
        glDrawArrays(GL_TRIANGLE_FAN, 0, sizeof(circleVertexes));
        glBindVertexArray( 0 );
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    
    GLfloat getRand(){
        return ((rand() % 17)-8)*0.1;
    }
    
    void deleteBuffers(){
        glDeleteVertexArrays( k, &circleVAO );
        glDeleteBuffers( k, &circleVBO );
    }
    
    void moveDown(GLfloat step){
        GLfloat y = moveVertex.y;
        moveVertex = glm::vec3(moveVertex.x, step+y, 0.0f);
    }
    
    void reGenerate(GLfloat y){
        moveVertex = glm::vec3(getRand(), y, 0.0f);
    }
    
    GLuint collisionCheck(){
        if(moveVertex.y<-1.1){
            reGenerate(3);
            return 1;
        }
        glm::vec3 pV = p->getVec();
        GLfloat scale = p->getScale();
        GLfloat x=moveVertex.x;
        GLfloat y=moveVertex.y;
        GLfloat right = (pV.x+scale);
        GLfloat left = (pV.x-scale);
        GLfloat top = (pV.y+scale*0.5);
        GLfloat bot = (pV.y);
        if(((right>x)&&(left<x))&&((top>y)&&(bot<y)))
            reGenerate(1.5);
        return 0;
    }
};

#endif /* circleObject_h */

