#include <iostream>
#include <unistd.h>
#include <cmath>
#include <stdlib.h>

//GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//shader class
#include "Shader.h"

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;

GLfloat changeY(GLfloat);

GLuint initPlatform();
GLuint initCircle();

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

void collisionCheck();

GLfloat x=0.0f;

GLdouble oldtime=0;

GLfloat xshift=0.0f;
GLfloat yshift=0.0f;

GLfloat circleVertexes[363];
GLuint fragmentCount=120;
GLboolean caught=true;

GLfloat platformVertexes[27];


GLuint platformEBO,platformVAO,platformVBO;
GLuint circleVAO, circleVBO;

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
    
    Shader ourShader("/Users/u40/Desktop/HelloWorld/HelloWorld/vertexShader.txt", "/Users/u40/Desktop/HelloWorld/HelloWorld/fragmentShader.txt");
    
    while ( !glfwWindowShouldClose( window ) )
    {
        
        
        
        glfwPollEvents( );
        
        
        
        glClearColor( 0.2f, 0.3f, 0.3f, 1.0f );
        glClear( GL_COLOR_BUFFER_BIT );
        
        ourShader.Use();
        
        GLuint platformSize = initPlatform();
        glBindVertexArray( platformVAO );
        glDrawElements(GL_TRIANGLES, platformSize, GL_UNSIGNED_INT, 0);
        glBindVertexArray( 0 );
        
        if(!caught){
            GLuint circleSize=initCircle();
            glBindVertexArray( circleVAO );
            glDrawArrays(GL_TRIANGLE_FAN, 0, circleSize);
            glBindVertexArray( 0 );
        }
        else
        {
            xshift=((rand() % 17)-8)*0.1;
            yshift=0;
            caught=false;
        }
        collisionCheck();
        
        yshift = changeY(yshift);
        
        glfwSwapBuffers( window );
    };
    
    glDeleteVertexArrays( 1, &platformVAO );
    glDeleteBuffers( 1, &platformVBO );
    glDeleteBuffers( 2, &platformEBO );
    
    glfwTerminate( );
    
    return EXIT_SUCCESS;
}

GLuint initCircle(){
    int a=0;
    for (int i=0; i < fragmentCount*3+3; i+=3)
        //       18<xshift<0
    {
        double angle = 2 * 3.14 * a++ / fragmentCount;
        circleVertexes[i]=xshift+0.1f*round(cos(angle)*100)/100;
        circleVertexes[i+1]=1.1f+yshift+0.1f*round(sin(angle)*100)/100;
        circleVertexes[i+2]=0.0f;
    }
    
    
    glGenBuffers(2, &circleVBO);
    glGenVertexArrays( 2, &circleVAO );
    
    glBindVertexArray( circleVAO );
    
    glBindBuffer( GL_ARRAY_BUFFER, circleVBO );
    glBufferData( GL_ARRAY_BUFFER, sizeof( circleVertexes ), circleVertexes, GL_STREAM_DRAW);
    
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof( GLfloat ), ( GLvoid * ) 0 );
    glEnableVertexAttribArray( 0 );
    
    glBindBuffer( GL_ARRAY_BUFFER, 0 );
    
    glBindVertexArray( 0 );
    
    return sizeof(circleVertexes);
}

GLuint initPlatform(){
    GLfloat a = -1.0f, b = -1.0f, c=0.1f;
    for(int i=0;i<21;i+=3){
        platformVertexes[i]=a+x;
        a+=0.1f;
        platformVertexes[i+1]=b+c;
        c*=-1;
        platformVertexes[i+2]=0.0f;
    }
    
    platformVertexes[21]=-0.9f+x;
    platformVertexes[22]=-0.87f;
    platformVertexes[23]=0.0f;
    
    platformVertexes[24]=-0.5f+x;
    platformVertexes[25]=-0.87f;
    platformVertexes[26]=0.0f;
    
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
    glGenBuffers(1, &platformVBO);
    glGenBuffers(2, &platformEBO);
    glGenVertexArrays( 1, &platformVAO );
    
    glBindVertexArray( platformVAO );
    
    glBindBuffer( GL_ARRAY_BUFFER, platformVBO );
    glBufferData( GL_ARRAY_BUFFER, sizeof( platformVertexes ), platformVertexes, GL_STREAM_DRAW);
    
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, platformEBO );
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof( platformIndexes ), platformIndexes, GL_STREAM_DRAW );
    
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof( GLfloat ), ( GLvoid * ) 0 );
    glEnableVertexAttribArray( 0 );
    
    glBindBuffer( GL_ARRAY_BUFFER, 0 );
    
    glBindVertexArray( 0 );
    
    return sizeof(platformIndexes);
}

GLfloat changeY(GLfloat y){
    GLdouble time = glfwGetTime();
    if((time-oldtime)>0.100){
        y-=0.06f;
        oldtime=time;
    }
    return y;
}

void collisionCheck(){
    GLuint k=fragmentCount/4*9;
    if((circleVertexes[k+1]<-0.9f) && (circleVertexes[k]>platformVertexes[3]) && (circleVertexes[k]<platformVertexes[15]))
    {
        glDeleteVertexArrays( 2, &circleVAO );
        glDeleteBuffers( 2, &circleVBO );
        for(int i=0;i<fragmentCount*3+2;i++){
            circleVertexes[i]=0;
        }
        caught=true;
    }
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    if(key == 262 && x<1.35f && (action == GLFW_PRESS || action==GLFW_REPEAT))//right
        x+=0.05f;
    if(key == 263 && x>0.0f && (action == GLFW_PRESS|| action==GLFW_REPEAT))//left
        x-=0.05f;
}
