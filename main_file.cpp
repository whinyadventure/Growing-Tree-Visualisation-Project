#define GLM_FORCE_RADIANS

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <ctime>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <lodepng.h>
#include "constants.h"

#include "skybox.h"
#include "tree.h"

using namespace glm;
using namespace std;

// global variables
const int width = 1920;
const int height = 1080;
float aspect = 1.0;
float speed_x = 0.0;
float speed_y = 0.0;
float zoom = 15.0;
float time_counter = 0.0;

// pointers
Skybox* skybox;
Tree* tree;

// textures
GLuint box_down_tex, box_left_tex, box_right_tex, box_front_tex, box_back_tex, bark_tex, leaf_tex;

// function declaration
void error_callback(int, const char*);
void windowResize(GLFWwindow*, int, int);
void key_callback(GLFWwindow*, int, int, int, int);
void scroll_callback(GLFWwindow*, double, double);
void initOpenGLProgram(GLFWwindow*);
void loadTextures();
void deleteTextures();
void manageLighting();
void drawScene(GLFWwindow*,float,float);

int main(void){
    srand(time(NULL));

	GLFWwindow* window; //Pointer to object that represents the application window

	glfwSetErrorCallback(error_callback); //Register error processing callback procedure

	if (!glfwInit()){ //Initialize GLFW library
		fprintf(stderr, "Can't initialize GLFW.\n");
		exit(EXIT_FAILURE);
	}

	window = glfwCreateWindow(width, height, "Growing Tree Visualisation", nullptr, nullptr);

	if (!window){ //If no window is opened then close the program
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window); //Since this moment OpenGL context corresponding to the window is active and all OpenGL calls will refer to this context.
	glfwSwapInterval(1); //During vsync wait for the first refresh

	GLenum err;
	if ((err = glewInit()) != GLEW_OK){ //Initialize GLEW library
		fprintf(stderr, "Can't initialize GLEW: %s\n", glewGetErrorString(err));
		exit(EXIT_FAILURE);
	}

	initOpenGLProgram(window); //Call initialization procedure

	float angle_x = 0; //Current rotation around X axis
	float angle_y = 0; //Current rotation around Y axis
	glfwSetTime(0);

	loadTextures();

	skybox = new Skybox();

	tree = new Tree();
	tree->loadModels();

	//Main application loop
	while (!glfwWindowShouldClose(window)){ //As long as the window shouldn't be closed yet
        //Apply changes in rotation angles
	    if((angle_x + speed_x*glfwGetTime() <= 0) && (angle_x + speed_x*glfwGetTime() >= -0.9*PI))
            angle_x += speed_x*glfwGetTime();
		angle_y += speed_y*glfwGetTime();
		glfwSetTime(0);

		drawScene(window, angle_x, angle_y); //Execute drawing procedure

		time_counter += TIME_STEP;
		glfwPollEvents(); //Process callback procedures corresponding to the events that took place up to now
	}

	glfwDestroyWindow(window); // Delete OpenGL context and the window.
	glfwTerminate(); // Free GLFW resources
    deleteTextures();
	exit(EXIT_SUCCESS);
}

//Error processing callback procedure
void error_callback(int error, const char* description){
	fputs(description, stderr);
}

//Framebuffer size change event processing
void windowResize(GLFWwindow* window, int width, int height){
	glViewport(0, 0, width, height); //Generate images in this resolution
	if (height != 0)
		aspect = (float)width/(float)height; //Compute aspect ratio of width to height of the window
	else
		aspect = 1.0;
}

//Key event processing
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
	if (action == GLFW_PRESS){
		if (key == GLFW_KEY_LEFT) speed_y = PI/2;
		if (key == GLFW_KEY_RIGHT) speed_y = -PI/2;
		if (key == GLFW_KEY_UP) speed_x = -PI/2;
		if (key == GLFW_KEY_DOWN) speed_x = PI/2;
	}

	if (action == GLFW_RELEASE){
		if (key == GLFW_KEY_LEFT) speed_y = 0;
		if (key == GLFW_KEY_RIGHT) speed_y = 0;
		if (key == GLFW_KEY_UP) speed_x = 0;
		if (key == GLFW_KEY_DOWN) speed_x = 0;
	}
}

//Mouse scroll event processing
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset){
    if(yoffset == 1 && zoom > 5.0f){ // zoom in
        zoom -= 0.5f;
    }

    if(yoffset == -1 && zoom < 45.f){ // zoom out
        zoom += 0.5f;
    }
}

//Initialization code procedure
void initOpenGLProgram(GLFWwindow* window){
	glfwSetFramebufferSizeCallback(window, windowResize); //Register frame buffer size change callback procedure
	glfwSetKeyCallback(window,key_callback); //Register key event processing callback procedure
	glfwSetScrollCallback(window, scroll_callback);

	glClearColor(0, 0, 0, 1); //Set window clear color

	// enable lightning
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);

	// enable depth buffer
	glEnable(GL_DEPTH_TEST);

	//Turn on setting material color via glColor* procedures
	glEnable(GL_COLOR_MATERIAL);

	// normal vectors are normalized to unit length after transformation and before lighting
	glEnable(GL_NORMALIZE);
}

void loadTextures(){
    vector<unsigned char> img_0, img_1, img_2, img_3, img_4, img_5, img_6;
    unsigned w_0,w_1,w_2,w_3,w_4,w_5,w_6;
    unsigned h_0,h_1,h_2,h_3,h_4,h_5,h_6;
    unsigned err_0,err_1,err_2,err_3,err_4,err_5,err_6;

    // ground
    err_0 = lodepng::decode(img_0, w_0, h_0, "ground_tex.png");
    glGenTextures(1,&box_down_tex);
    glBindTexture(GL_TEXTURE_2D, box_down_tex);
    glTexImage2D(GL_TEXTURE_2D, 0, 4, w_0, h_0, 0, GL_RGBA, GL_UNSIGNED_BYTE, (unsigned char*) img_0.data());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // left
    err_1 = lodepng::decode(img_1, w_1, h_1, "left_tex.png");
    glGenTextures(1,&box_left_tex);
    glBindTexture(GL_TEXTURE_2D, box_left_tex);
    glTexImage2D(GL_TEXTURE_2D, 0, 4, w_1, h_1, 0, GL_RGBA, GL_UNSIGNED_BYTE, (unsigned char*) img_1.data());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // right
    err_2 = lodepng::decode(img_2, w_2, h_2, "right_tex.png");
    glGenTextures(1,&box_right_tex);
    glBindTexture(GL_TEXTURE_2D, box_right_tex);
    glTexImage2D(GL_TEXTURE_2D, 0, 4, w_2, h_2, 0, GL_RGBA, GL_UNSIGNED_BYTE, (unsigned char*) img_2.data());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // front
    err_3 = lodepng::decode(img_3, w_3, h_3, "front_tex.png");
    glGenTextures(1,&box_front_tex);
    glBindTexture(GL_TEXTURE_2D, box_front_tex);
    glTexImage2D(GL_TEXTURE_2D, 0, 4, w_3, h_3, 0, GL_RGBA, GL_UNSIGNED_BYTE, (unsigned char*) img_3.data());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // back
    err_4 = lodepng::decode(img_4, w_4, h_4, "back_tex.png");
    glGenTextures(1,&box_back_tex);
    glBindTexture(GL_TEXTURE_2D, box_back_tex);
    glTexImage2D(GL_TEXTURE_2D, 0, 4, w_4, h_4, 0, GL_RGBA, GL_UNSIGNED_BYTE, (unsigned char*) img_4.data());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // bark
    err_5 = lodepng::decode(img_5, w_5, h_5, "bark_tex.png");
    glGenTextures(1,&bark_tex);
    glBindTexture(GL_TEXTURE_2D, bark_tex);
    glTexImage2D(GL_TEXTURE_2D, 0, 4, w_5, h_5, 0, GL_RGBA, GL_UNSIGNED_BYTE, (unsigned char*) img_5.data());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // leaf
    err_6 = lodepng::decode(img_6, w_6, h_6, "leaf_tex.png");
    glGenTextures(1,&leaf_tex);
    glBindTexture(GL_TEXTURE_2D, leaf_tex);
    glTexImage2D(GL_TEXTURE_2D, 0, 4, w_6, h_6, 0, GL_RGBA, GL_UNSIGNED_BYTE, (unsigned char*) img_6.data());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void deleteTextures(){
    glDeleteTextures(1,&box_down_tex);
    glDeleteTextures(1,&box_left_tex);
    glDeleteTextures(1,&box_right_tex);
    glDeleteTextures(1,&box_front_tex);
    glDeleteTextures(1,&box_back_tex);
    glDeleteTextures(1,&bark_tex);
    glDeleteTextures(1,&leaf_tex);
}

void manageLighting(){
    float light0_pos[] = {5.0f, 0.5f, 5.0f, 1.0f};
    float ambient_light0[] = {0.2, 0.2, 0.2, 1.0};
    float difuse_light0[] = {1.0, 1.0, 1.0, 1.0};
    float specular_light0[] = {1.0, 1.0, 1.0, 1.0};

    glLightfv(GL_LIGHT0, GL_POSITION, light0_pos);
    glLightfv(GL_LIGHT1, GL_AMBIENT, ambient_light0);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, difuse_light0);
    glLightfv(GL_LIGHT1, GL_SPECULAR, specular_light0);

    float light1_pos[] = {0.0f, 20.0f, 0.0f, 1.0f};
    float light1_att = 10*pow(sin(2 * PI * time_counter / 360.0f - PI / 4), 2);

    glLightfv(GL_LIGHT1, GL_POSITION, light1_pos);
    glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, light1_att);
}

// Drawing procedure
void drawScene(GLFWwindow* window, float angle_x, float angle_y){
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT); // Clear color buffer

	// Prepare to draw
	mat4 P = perspective(50.0f*PI/180.0f, aspect, 1.0f, 500.0f); // Compute projection matrix

    mat4 V = lookAt( // Compute view matrix
           vec3(0.0f, 4.0f, -5.0f -zoom),
           vec3(0.0f, 0.0f, 0.0f),
           vec3(0.0f, 1.0f, 0.0f));

    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(value_ptr(P));

    mat4 M = mat4(1.0f); // Compute model matrix
    M = rotate(M, angle_x, vec3(1.0f, 0.0f, 0.0f));
    M = rotate(M, angle_y, vec3(0.0f, 1.0f, 0.0f));

    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(value_ptr(V*M));

    // Set lighting
    manageLighting();

    skybox->drawSolid(V, M, box_down_tex, box_left_tex, box_right_tex, box_front_tex, box_back_tex);
    tree->draw(V, M, bark_tex, leaf_tex, time_counter);

	glfwSwapBuffers(window); //Swap the back and front buffers
}
