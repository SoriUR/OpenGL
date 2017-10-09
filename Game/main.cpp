#include <iostream>
#include <unistd.h>
#include <cmath>
#include <stdlib.h>
#include <string.h>

//GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//shader class
#include "Shader.cpp"
#include "circleObject.cpp"

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

//SOIL
#include "SOIL.h"

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;


void initPlatform();
void initBackground();
void initCircle();
void bgDraw();
void platformDraw();
void platformUpgrade();
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void upgradeScore();
GLfloat timeTracker();

GLfloat x=0.0f;
GLfloat speed=-0.01f;
GLfloat speedDelta=-0.000005f;
GLboolean isPause=true;
GLfloat step;
GLdouble oldtime=0;
GLuint score=0;
GLuint misses=0;

GLuint platformEBO,platformVAO,platformVBO;
GLuint bgVBO, bgVAO, bgEBO;
GLuint bgTexture, platformTexture;

GLfloat platformVertexes[45]=
{    //vertex                //tex
    0.0f, 0.0f, 0.0f,       1.0f,   0.421f,
    0.0f, 0.0f, 0.0f,       0.845f, 1.0f,
    0.0f, 0.0f, 0.0f,       0.665f, 0.421f,
    0.0f, 0.0f, 0.0f,       0.51f,  1.0f,
    0.0f, 0.0f, 0.0f,       0.335f, 0.421f,
    0.0f, 0.0f, 0.0f,       0.135f, 1.0f,
    0.0f, 0.0f, 0.0f,       0.0f,   0.421f,
    
    0.0f, 0.0f, 0.0f,       0.8f, 0.0f,
    0.0f, 0.0f, 0.0f,       0.19f, 0.0f
    
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

int main()
{
    
    glfwInit( );
    
    glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
    glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
    glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );
    
    glfwWindowHint( GLFW_RESIZABLE, GL_FALSE );
    
    GLFWwindow *window = glfwCreateWindow( WIDTH, HEIGHT, "LearnOpenGL", nullptr, nullptr );
    
    int screenWidth, screenHeight;
    glfwGetFramebufferSize( window, &screenWidth, &screenHeight );
    
    if ( nullptr == window )
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate( );
        
        return EXIT_FAILURE;
    }
    
    glfwMakeContextCurrent( window );
    
    glfwSetKeyCallback(window, key_callback);
    
    glewExperimental = GL_TRUE;
    
    if ( GLEW_OK != glewInit( ) )
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return EXIT_FAILURE;
    }
    
    glViewport( 0, 0, screenWidth, screenHeight );
    
    Shader textureShader("/Users/u40/Desktop/Game/Game/bgVertexShader.txt", "/Users/u40/Desktop/Game/Game/bgFragmentShader.txt");
    
    circleObject o1(1, 1.1f);
    circleObject o2(4, 1.9f);
    
    initBackground();
    initPlatform();
    
    while ( !glfwWindowShouldClose( window ) && misses<5 )
    {
        glfwPollEvents();
        
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        textureShader.Use();
        
        bgDraw();
        platformDraw();
        o1.drawObject();
        o2.drawObject();
        
//        upgradeScore();

        GLint k;
        k = o1.collisionCheck(platformVertexes);
        switch (k) {
            case 1:
                score++;
                break;
            case -1:
                misses++;
                break;
            default:
                break;
        }
        
        k = o2.collisionCheck(platformVertexes);
        switch (k) {
            case 1:
                score++;
                break;
            case -1:
                misses++;
                break;
            default:
                break;
        }

        
        if(!isPause){
            step=timeTracker();
            o1.setY(o1.getY()+step);
            o2.setY(o2.getY()+step);
        }
        //
        glfwSwapBuffers( window );
    };
    
    o1.deleteBuffers();
    o2.deleteBuffers();
    glDeleteVertexArrays( 1, &platformVAO );
    glDeleteBuffers( 1, &platformVBO );
    glDeleteBuffers( 1, &platformEBO );
    glDeleteVertexArrays( 3, &bgVAO );
    glDeleteBuffers( 3, &bgVBO );
    glDeleteBuffers( 3, &bgEBO );
    
    glfwTerminate( );
    
    return EXIT_SUCCESS;
}

void initPlatform(){
    glGenVertexArrays( 1, &platformVAO );
    glGenBuffers(1, &platformVBO);
    glGenBuffers(1, &platformEBO);
    
    glBindVertexArray( platformVAO );
    
    glBindBuffer( GL_ARRAY_BUFFER, platformVBO );
    glBufferData( GL_ARRAY_BUFFER, sizeof( platformVertexes ), NULL, GL_STREAM_DRAW);
    
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, platformEBO );
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof( platformIndexes ), platformIndexes, GL_STREAM_DRAW );
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    
    glBindVertexArray( 0 );
    
    glGenTextures(1, &platformTexture);
    glBindTexture(GL_TEXTURE_2D, platformTexture);
    int width, height;
    unsigned char* image = SOIL_load_image("/Users/u40/Desktop/Game/Game/boxTexture.jpg", &width, &height, 0, SOIL_LOAD_RGB);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    SOIL_free_image_data(image);
    glBindTexture(GL_TEXTURE_2D, 0);
    
    platformUpgrade();
}

void platformUpgrade(){
    GLfloat a = -1.0f, b = -0.7f, c=0.1f;
    for(int i=0;i<(7*5);i+=5){
        platformVertexes[i]=a+x;
        a+=0.1f;
        platformVertexes[i+1]=b+c;
        c*=-1;
    }
    
    platformVertexes[35]=-0.92f+x;
    platformVertexes[36]=-0.53f;
    
    platformVertexes[40]=-0.48f+x;
    platformVertexes[41]=-0.53f;
    
    glBindBuffer(GL_ARRAY_BUFFER, platformVBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(platformVertexes),platformVertexes);
    glBindBuffer(GL_ARRAY_BUFFER, platformVBO);
}

void platformDraw(){
    glBindTexture(GL_TEXTURE_2D, platformTexture);
    glBindVertexArray( platformVAO );
    glDrawElements(GL_TRIANGLES, sizeof(platformIndexes), GL_UNSIGNED_INT, 0);
    glBindVertexArray( 0 );
    glBindTexture(GL_TEXTURE_2D, 0);
}

void initBackground(){
    GLfloat bgRec[] = {
        // Positions         // Texture Coords
        1.0f,  1.0f, 0.0f,     0.0f, 0.0f, // Top Right
        1.0f, -1.0f, 0.0f,     0.0f, 1.0f, // Bottom Right
        -1.0f, -1.0f, 0.0f,    1.0f, 1.0f, // Bottom Left
        -1.0f,  1.0f, 0.0f,    1.0f, 0.0f  // Top Left
    };
    //
    // 0.7, 0.8
    GLuint bgIndices[] = {
        0, 1, 3,
        1, 2, 3
    };
    glGenVertexArrays(3, &bgVAO);
    glGenBuffers(3, &bgVBO);
    glGenBuffers(3, &bgEBO);
    
    glBindVertexArray(bgVAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, bgVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(bgRec), bgRec, GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bgEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(bgIndices), bgIndices, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    
    glBindVertexArray(0);
    
    glGenTextures(3, &bgTexture);
    glBindTexture(GL_TEXTURE_2D, bgTexture);
    int width, height;
    unsigned char* image = SOIL_load_image("/Users/u40/Desktop/Game/Game/wallTexture.png", &width, &height, 0, SOIL_LOAD_RGB);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    SOIL_free_image_data(image);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    GLfloat shift=0.15f;
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    if(key == 262 && x<1.4f && (action == GLFW_PRESS || action==GLFW_REPEAT))//right
    {
        x+=shift;
        platformUpgrade();
    }
    if(key == 263 && x-shift>=0 && (action == GLFW_PRESS|| action==GLFW_REPEAT))//left
    {
        x-=shift;
        platformUpgrade();
    }
    if(key == GLFW_KEY_SPACE && action == GLFW_PRESS)
        isPause=!isPause;
    
}

void bgDraw(){
    glBindTexture(GL_TEXTURE_2D, bgTexture);
    glBindVertexArray(bgVAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

GLfloat timeTracker(){
    GLdouble time = glfwGetTime();
    if((time-oldtime)<0.01)
        return 0;
    oldtime=time;
    speed+=speedDelta;
return speed;
}

void upgradeScore(){
    
}

