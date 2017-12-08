#include <iostream>
#include <unistd.h>
#include <cmath>
#include <stdlib.h>
#include <string.h>

//GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//#include <CoreFoundation/CoreFoundation.h>

//class
#include "Shader.cpp"
#include "circleObject.cpp"
#include "heartObject.cpp"
#include "platform.cpp"

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

//SOIL
#include "SOIL.h"

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;

void initShadow();
void shadowDraw();
void shadowUpgrade(GLfloat a);
void showScore();
void initPlatform();
void initBackground();
void initCircle();
void bgDraw();
void platformDraw();
void platformUpgrade(GLfloat a);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void upgradeScore();
GLfloat timeTracker();

GLfloat x=0.8f;
GLfloat speed=-0.01f;
GLfloat speedDelta=-0.000005f;
GLboolean isPause=true;
GLfloat step;
GLdouble oldtime=0;
GLuint misses=0;

GLuint bgVBO, bgVAO, bgEBO;
GLuint shadowVBO, shadowVAO;
GLuint bgTexture, platformTexture;
platform * pl;

GLfloat shadowVertexes[]=
{    //vertex
    -0.5f,  -0.8f, 0.0f,
    -0.34f, -0.74f, 0.0f,
    -0.435f, -0.67f, 0.0f
};

int main()
{
    
    glfwInit( );
    
    glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
    glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
    glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );
    
    glfwWindowHint( GLFW_RESIZABLE, GL_FALSE );
    
    GLFWwindow *window = glfwCreateWindow( WIDTH, HEIGHT, "Catch a ball", nullptr, nullptr );
    
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

    Shader heartShader("/Users/u40/Developer/Game/Game/heartVertexShader.txt", "/Users/u40/Developer/Game/Game/textureFragmentShader.txt");
    Shader textureShader("/Users/u40/Developer/Game/Game/textureVertexShader.txt", "/Users/u40/Developer/Game/Game/textureFragmentShader.txt");

   
    
    initBackground();
    heartObject health[5]{{6},{7},{8},{9},{10}};
    platform p;
    pl=&p;
    
    circleObject o1(2,pl);
    circleObject o2(3,pl);

    
    while ( !glfwWindowShouldClose( window ) && misses<5 )
    {
        glfwPollEvents();
        
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        textureShader.Use();
        bgDraw();
        
        GLfloat xXx=-0.9;
        for(int a=0;a<(5-misses);a++){
            health[a].drawObject(xXx,heartShader);
            xXx+=0.12;
        }
        
        pl->drawObject(heartShader);
        o1.drawObject(heartShader);
        o2.drawObject(heartShader);
        
        GLuint res;
        res = o1.collisionCheck();
        res += o2.collisionCheck();
        misses+=res;
        
//        if(!isPause){
            step=timeTracker();
            o1.moveDown(step);
            o2.moveDown(step);
//        }

        glfwSwapBuffers( window );
    };
    o1.deleteBuffers();
    o2.deleteBuffers();

    glDeleteVertexArrays( 3, &bgVAO );
    glDeleteBuffers( 3, &bgVBO );
    glDeleteBuffers( 3, &bgEBO );
    
    glfwTerminate( );
    
    return EXIT_SUCCESS;
}

void initBackground(){
    GLfloat bgRec[] = {
        // Positions         // Texture Coords
        1.0f,  1.0f, 0.0f,     0.0f, 0.0f, // Top Right
        1.0f, -1.0f, 0.0f,     0.0f, 1.0f, // Bottom Right
        -1.0f, -1.0f, 0.0f,    1.0f, 1.0f, // Bottom Left
        -1.0f,  1.0f, 0.0f,    1.0f, 0.0f  // Top Left
    };

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
    unsigned char* image = SOIL_load_image("/Users/u40/Developer/Game/Game/wallTexture.png", &width, &height, 0, SOIL_LOAD_RGB);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    SOIL_free_image_data(image);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void bgDraw(){
    glBindTexture(GL_TEXTURE_2D, bgTexture);
    glBindVertexArray(bgVAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    GLfloat shift=0.15f;
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    if(key == 262 && (action == GLFW_PRESS || action==GLFW_REPEAT))//right
    {
        pl->Move(shift);
    }
    if(key == 263 && (action == GLFW_PRESS|| action==GLFW_REPEAT))//left
    {
        pl->Move(-shift);
    }
    if(key == GLFW_KEY_SPACE && action == GLFW_PRESS)
        isPause=!isPause;
    
}

GLfloat timeTracker(){
    GLdouble time = glfwGetTime();
    if((time-oldtime)<0.01)
        return 0;
    oldtime=time;
    speed+=speedDelta;
    return speed;
}


