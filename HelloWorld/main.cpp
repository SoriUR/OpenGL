#include <iostream>
#include <unistd.h>
#include <cmath>
#include <stdlib.h>

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

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;

GLfloat timeTracker();

void initPlatform();
void initCircle();

void platformUpgrade();

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

GLfloat x=0.0f;
GLfloat speed=-0.01f;
GLfloat speedDelta=-0.0000005f;

GLdouble oldtime=0;

GLfloat platformVertexes[27];

GLuint platformEBO,platformVAO,platformVBO;

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
    
    Shader ourShader("/Users/u40/Desktop/HelloWorld/HelloWorld/vertexShader.txt", "/Users/u40/Desktop/HelloWorld/HelloWorld/fragmentShader.txt");
    
    circleObject o1(2, 1.1f);
    circleObject o2(3, 1.8f);
    
    initPlatform();
    
    while ( !glfwWindowShouldClose( window ) )
    {
        glfwPollEvents( );
        
        glClearColor( 0.2f, 0.3f, 0.3f, 1.0f );
        glClear( GL_COLOR_BUFFER_BIT );
        
        ourShader.Use();
        
        glBindVertexArray( platformVAO );
        glDrawElements(GL_TRIANGLES, sizeof(platformIndexes), GL_UNSIGNED_INT, 0);
        glBindVertexArray( 0 );
        
        o1.drawObject();
        o1.collisionCheck(platformVertexes);
        
        o2.drawObject();
        o2.collisionCheck(platformVertexes);
        
        GLfloat step = timeTracker();
        o1.setY(o1.getY()+step);
        o2.setY(o2.getY()+step);
        
        glfwSwapBuffers( window );
    };
    
    glDeleteVertexArrays( 1, &platformVAO );
    glDeleteBuffers( 1, &platformVBO );
    glDeleteBuffers( 2, &platformEBO );
    
    glfwTerminate( );
    
    return EXIT_SUCCESS;
}

void initPlatform(){
    glGenBuffers(1, &platformVBO);
    glGenBuffers(2, &platformEBO);
    glGenVertexArrays( 1, &platformVAO );
    
    glBindVertexArray( platformVAO );
    
    glBindBuffer( GL_ARRAY_BUFFER, platformVBO );
    glBufferData( GL_ARRAY_BUFFER, sizeof( platformVertexes ), NULL, GL_STREAM_DRAW);
    
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, platformEBO );
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof( platformIndexes ), platformIndexes, GL_STREAM_DRAW );
    
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof( GLfloat ), ( GLvoid * ) 0 );
    glEnableVertexAttribArray( 0 );
    
    glBindBuffer( GL_ARRAY_BUFFER, 0 );
    
    glBindVertexArray( 0 );
    
    platformUpgrade();
}

GLfloat timeTracker(){
    GLdouble time = glfwGetTime();
    if((time-oldtime)<0.01)
        return 0;
    oldtime=time;
    speed+=speedDelta;
    return speed;
    
}

void platformUpgrade(){
    GLfloat a = -1.0f, b = -1.0f, c=0.1f;
    for(int i=0;i<21;i+=3){
        platformVertexes[i]=a+x;
        a+=0.1f;
        platformVertexes[i+1]=b+c;
        c*=-1;
        platformVertexes[i+2]=0.0f;
    }
    
    platformVertexes[21]=-0.95f+x;
    platformVertexes[22]=-0.87f;
    platformVertexes[23]=0.0f;
    
    platformVertexes[24]=-0.45f+x;
    platformVertexes[25]=-0.87f;
    platformVertexes[26]=0.0f;
    
    glBindBuffer(GL_ARRAY_BUFFER, platformVBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(platformVertexes),platformVertexes);      //replace data in VBO with new data
    glBindBuffer(GL_ARRAY_BUFFER, platformVBO);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    if(key == 262 && x<1.35f && (action == GLFW_PRESS || action==GLFW_REPEAT))//right
    {
        x+=0.05f;
        platformUpgrade();
    }
    if(key == 263 && x>0.05 && (action == GLFW_PRESS|| action==GLFW_REPEAT))//left
    {
        x-=0.05f;
        platformUpgrade();
    }
}
