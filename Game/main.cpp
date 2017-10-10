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
GLuint score=0;
GLuint misses=0;

GLuint platformEBO,platformVAO,platformVBO;
GLuint bgVBO, bgVAO, bgEBO;
GLuint shadowVBO, shadowVAO;
GLuint bgTexture, platformTexture;

GLfloat platformVertexes[]=
{    //vertex                //tex
    -1.0f, -0.6f, 0.0f,       1.0f,   0.421f,
    -0.9f, -0.8f, 0.0f,       0.845f, 1.0f,
    -0.8f, -0.6f, 0.0f,       0.665f, 0.421f,
    -0.7f, -0.8f, 0.0f,       0.51f,  1.0f,
    -0.6f, -0.6f, 0.0f,       0.335f, 0.421f,
    -0.5f, -0.8f, 0.0f,       0.135f, 1.0f,
    -0.4f, -0.6f, 0.0f,       0.0f,   0.421f,
    
    -0.92f, -0.53f, 0.0f,       0.8f, 0.0f,
    -0.48f, -0.53f, 0.0f,       0.19f, 0.0f
    
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

GLfloat shadowVertexes[]=
{    //vertex
    -0.5f,  -0.8f, 0.0f,
    -0.35f, -0.74f, 0.0f,
    -0.435f, -0.67, 0.0f
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
    
    Shader textureShader("/Users/u40/Desktop/Game/Game/textureVertexShader.txt", "/Users/u40/Desktop/Game/Game/textureFragmentShader.txt");
    Shader heartShader("/Users/u40/Desktop/Game/Game/heartVertexShader.txt", "/Users/u40/Desktop/Game/Game/textureFragmentShader.txt");
    Shader shadowShader("/Users/u40/Desktop/Game/Game/shadowVertexShader.txt", "/Users/u40/Desktop/Game/Game/fragmentShader.txt");

   
    
    initBackground();
    heartObject health[5]{{6},{7},{8},{9},{10}};
    initPlatform();
        initShadow();
    
    circleObject o1(2);
    circleObject o2(5);

    
    while ( !glfwWindowShouldClose( window ) && misses<5 )
    {
        glfwPollEvents();
        
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        textureShader.Use();
        bgDraw();
        
        GLfloat xXx=-0.925;
        for(int a=0;a<(5-misses);a++){
            health[a].drawObject(xXx,heartShader);
            xXx+=0.125;
        }
        
        textureShader.Use();
        platformDraw();
        shadowShader.Use();
        shadowDraw();
        textureShader.Use();
        o1.drawObject();
        o2.drawObject();

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
    
    for(int i=0;i<41;i+=5)
        platformVertexes[i]+=x;
    platformUpgrade(0);
}

void platformUpgrade(GLfloat a){
    for(int i=0;i<41;i+=5)
        platformVertexes[i]+=a;
    
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

void bgDraw(){
    glBindTexture(GL_TEXTURE_2D, bgTexture);
    glBindVertexArray(bgVAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void initShadow(){
    glGenVertexArrays( 1, &shadowVAO );
    glGenBuffers(1, &shadowVBO);

    glBindVertexArray( shadowVAO );

    glBindBuffer( GL_ARRAY_BUFFER, shadowVBO );
    glBufferData( GL_ARRAY_BUFFER, sizeof( shadowVertexes ), NULL, GL_STREAM_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray( 0 );

    for(int i=0;i<7;i+=3)
        shadowVertexes[i]+=x;

    shadowUpgrade(0);
}

void shadowUpgrade(GLfloat a){
    
    for(int i=0;i<7;i+=3)
        shadowVertexes[i]+=a;
    
    glBindBuffer(GL_ARRAY_BUFFER, shadowVBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(shadowVertexes),shadowVertexes);
    glBindBuffer(GL_ARRAY_BUFFER, shadowVBO);
}

void shadowDraw(){
    glBindVertexArray(shadowVAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);

}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    GLfloat shift=0.15f;
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    if(key == 262 && (action == GLFW_PRESS || action==GLFW_REPEAT))//right
    {
        if((platformVertexes[0]+shift)>0.4)
            shift=0.4-platformVertexes[0];
        platformUpgrade(shift);
        shadowUpgrade(shift);
    }
    if(key == 263 && (action == GLFW_PRESS|| action==GLFW_REPEAT))//left
    {
        if((platformVertexes[0]-shift)<-1)
            shift=platformVertexes[0]+1;
        platformUpgrade(-shift);
        shadowUpgrade(-shift);
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


