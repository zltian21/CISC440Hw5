// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include "ground.hpp"
#include "Character.hpp"
#include "BBox.hpp"
#include <iostream>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>
GLFWwindow* window;

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;
using namespace std;

#include <common/shader.hpp>
#include <common/texture.hpp>
#include <common/controls.hpp>
#include <common/objloader.hpp>
#include <common/vboindexer.hpp>

//Globals
GLuint programID;
GLuint VertexArrayID;
GLuint MatrixID;
GLuint ViewMatrixID;
GLuint ModelMatrixID;
GLuint LightID;

//BBoxes
std::vector<glm::vec3> vertices_bbox;
std::vector<glm::vec2> uvs_bbox;
std::vector<glm::vec3> normals_bbox;

GLuint Texture_bbox;
GLuint TextureID_bbox;

std::vector<unsigned short> indices_bbox;
std::vector<glm::vec3> indexed_vertices_bbox;
std::vector<glm::vec2> indexed_uvs_bbox;
std::vector<glm::vec3> indexed_normals_bbox;

GLuint vertexbuffer_bbox;
GLuint uvbuffer_bbox;
GLuint normalbuffer_bbox;
GLuint elementbuffer_bbox;


//camera
double cam_x = 0; //0
double cam_y = 23; // 13
double cam_z = 15; // 10

//
glm::mat4 View;
glm::mat4 Model;
glm::mat4 Projection;


//Objects
Ground * ground = new Ground(0, 0, 0);
Character * character = new Character(0, 1.5, 0);
BBox * bbox = new BBox(4, 1, 0);


//bool CheckCollision(Character *c, BBox *bb){
//    
////    cout<<bb->critical_position.x <<endl;
//    bool collisionX = c->critical_position.x + c->length/2 >= bb->critical_position.x - bb->length/2 &&
//    bb->critical_position.x + bb->length/2 >= c->critical_position.x - c->length/2;
//    
//    bool collisionY = c->critical_position.y + c->length/2 >= bb->critical_position.y - bb->length/2 &&
//    bb->critical_position.y + bb->length/2 >= c->critical_position.y - c->length/2;
//
//    return collisionX && collisionY;
//
//}



void initialize_bbox(){
    bool res = loadOBJ("BBox.obj", vertices_bbox, uvs_bbox, normals_bbox);
    Texture_bbox = loadDDS("uvtemplate.DDS");
    TextureID_bbox  = glGetUniformLocation(programID, "myTextureSampler");
    
    indexVBO(vertices_bbox, uvs_bbox, normals_bbox, indices_bbox, indexed_vertices_bbox, indexed_uvs_bbox, indexed_normals_bbox);
    
    glGenBuffers(1, &vertexbuffer_bbox);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer_bbox);
    glBufferData(GL_ARRAY_BUFFER, indexed_vertices_bbox.size() * sizeof(glm::vec3), &indexed_vertices_bbox[0], GL_STATIC_DRAW);
    
    glGenBuffers(1, &uvbuffer_bbox);
    glBindBuffer(GL_ARRAY_BUFFER, uvbuffer_bbox);
    glBufferData(GL_ARRAY_BUFFER, indexed_uvs_bbox.size() * sizeof(glm::vec2), &indexed_uvs_bbox[0], GL_STATIC_DRAW);
    
    glGenBuffers(1, &normalbuffer_bbox);
    glBindBuffer(GL_ARRAY_BUFFER, normalbuffer_bbox);
    glBufferData(GL_ARRAY_BUFFER, indexed_normals_bbox.size() * sizeof(glm::vec3), &indexed_normals_bbox[0], GL_STATIC_DRAW);
    
    glGenBuffers(1, &elementbuffer_bbox);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer_bbox);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_bbox.size() * sizeof(unsigned short), &indices_bbox[0] , GL_STATIC_DRAW);
}

void cleanUp_bbox(){
    glDeleteBuffers(1, &vertexbuffer_bbox);
    glDeleteBuffers(1, &uvbuffer_bbox);
    glDeleteBuffers(1, &normalbuffer_bbox);
    glDeleteBuffers(1, &elementbuffer_bbox);
    glDeleteProgram(programID);
    glDeleteTextures(1, &Texture_bbox);
    glDeleteVertexArrays(1, &VertexArrayID);
}

//setUp Light
void initialize_objects(){
    ground->initialize();
    character->initialize();
    initialize_bbox();

}

void draw_objects(){
    ground->draw(Model * glm::translate(ground->position));
    bbox->draw(Model * glm::translate(bbox->position));
    
    switch(character->direction){
        case SOUTH:
            character->draw(Model * glm::translate(character->position));
            break;
        case NORTH:
            character->draw(Model * glm::translate(character->position) * glm::rotate((float) (1.00*M_PI),
                                                                                      glm::vec3(0.0f, 1.0f, 0.0f)
                                                                                      ));
            break;
        case WEST:
            character->draw(Model * glm::translate(character->position) * glm::rotate((float) (1.5*M_PI),
                                                                                      glm::vec3(0.0f, 1.0f, 0.0f)
                                                                                      ));
            break;
        case EAST:
            character->draw(Model * glm::translate(character->position) * glm::rotate((float) (0.5*M_PI),
                                                                                      glm::vec3(0.0f, 1.0f, 0.0f)
                                                                                      ));
            break;
            
    }
    
}

void clean_up(){
    ground->cleanUp();
    character->cleanUp();
    cleanUp_bbox();
    
}

void setup_light(){
    glm::vec3 lightPos = glm::vec3(10,10,10);
    glUniform3f(LightID, lightPos.x, lightPos.y, lightPos.z);
}

void setup_camera(){
    View = glm::lookAt(glm::vec3(cam_x,cam_y,cam_z), // Camera is at (4,3,3), in World Space
                       glm::vec3(0,0,0), // and looks at the origin
                       glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
                       );
}

void update(){
//    if(CheckCollision(character, bbox)){
//        character->velocity.x = character->velocity.x * -1;
//        character->velocity.y = character->velocity.y * -1;
//    }
    character->update();
    
//    CheckCollision(character, bbox);
    //std::cout<< character->critical_position.x << "  " << character->critical_position.y <<  std::endl;
    
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
    
    //CAMERA CONTROL
    if (key == GLFW_KEY_0 && (action == GLFW_PRESS || action == GLFW_REPEAT)){
        cam_x = 0;
        cam_y = 23;
        cam_z = 15;
    }else if(key == GLFW_KEY_L && (action == GLFW_PRESS || action == GLFW_REPEAT)){
        cam_x = 0;
        cam_y = 0;
        cam_z = 30;
    }else if(key == GLFW_KEY_O && (action == GLFW_PRESS || action == GLFW_REPEAT)){
        cam_x = 0;
        cam_y = 27;
        cam_z = 0.1;
    }else if(key == GLFW_KEY_P && (action == GLFW_PRESS || action == GLFW_REPEAT)){
        cam_x = 30;
        cam_y = 0;
        cam_z = 0;
    }
    
    else if(key == GLFW_KEY_W && (action == GLFW_PRESS || action == GLFW_REPEAT)){
        cam_z -= 0.3;
    }else if(key == GLFW_KEY_S && (action == GLFW_PRESS || action == GLFW_REPEAT)){
        cam_z += 0.3;
    }else if(key == GLFW_KEY_A && (action == GLFW_PRESS || action == GLFW_REPEAT)){
        cam_x -= 0.3;
    }else if(key == GLFW_KEY_D && (action == GLFW_PRESS || action == GLFW_REPEAT)){
        cam_x += 0.3;
    }else if(key == GLFW_KEY_R && (action == GLFW_PRESS || action == GLFW_REPEAT)){
        cam_y += 0.3;
    }else if(key == GLFW_KEY_F && (action == GLFW_PRESS || action == GLFW_REPEAT)){
        cam_y -= 0.3;
    }
    
    //CHARACTER CONTROL
    if(key == GLFW_KEY_UP && (action == GLFW_PRESS || action == GLFW_REPEAT)){
        character->velocity.y = -0.1;
        character->direction = NORTH;
        
    }else if(key == GLFW_KEY_DOWN && (action == GLFW_PRESS || action == GLFW_REPEAT)){
        character->velocity.y = 0.1;
        character->direction = SOUTH;
        
    }else if(key == GLFW_KEY_LEFT && (action == GLFW_PRESS || action == GLFW_REPEAT)){
        character->velocity.x = -0.1;
        character->direction = WEST;
        
    }else if(key == GLFW_KEY_RIGHT && (action == GLFW_PRESS || action == GLFW_REPEAT)){
        character->velocity.x = 0.1;
        character->direction = EAST;
    }

    
    else{
        character->velocity = glm::vec2(0, 0);
    }
    
    
}

int main( void )
{
	// Initialise GLFW
	if( !glfwInit() )
	{
		fprintf( stderr, "Failed to initialize GLFW\n" );
		getchar();
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	window = glfwCreateWindow( 1024, 768, "Tutorial 09 - Rendering several models", NULL, NULL);
	if( window == NULL ){
		fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
		getchar();
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
    
	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}
    
    //Initialize all Objects
    initialize_objects();
    
	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    // Hide the mouse and enable unlimited mouvement
   // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    
    // Set the mouse at the center of the screen
    //glfwPollEvents();
    //glfwSetCursorPos(window, 1024/2, 768/2);

	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS); 

	// Cull triangles which normal is not towards the camera
	glEnable(GL_CULL_FACE);

	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// Create and compile our GLSL program from the shaders
	programID = LoadShaders( "StandardShading.vertexshader", "StandardShading.fragmentshader" );

	// Get a handle for our "MVP" uniform
	MatrixID = glGetUniformLocation(programID, "MVP");
	ViewMatrixID = glGetUniformLocation(programID, "V");
	ModelMatrixID = glGetUniformLocation(programID, "M");

    Projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f); //////
    
    View       = glm::lookAt(
         glm::vec3(cam_x,cam_y,cam_z), // Camera is at (4,3,3), in World Space
         glm::vec3(0,0,0), // and looks at the origin
         glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
    );
    
    Model = glm::mat4(1.0f);
    
    glfwSetKeyCallback(window, key_callback);
    
    glUseProgram(programID);
    LightID = glGetUniformLocation(programID, "LightPosition_worldspace");
    // Read ground.obj file
    
    
	// Load it into a VBO

	// For speed computation
//	double lastTime = glfwGetTime();
//	int nbFrames = 0;

	do{

		// Measure speed
//		double currentTime = glfwGetTime();
//		nbFrames++;
//		if ( currentTime - lastTime >= 1.0 ){ // If last prinf() was more than 1sec ago
//			// printf and reset
//			printf("%f ms/frame\n", 1000.0/double(nbFrames));
//			nbFrames = 0;
//			lastTime += 1.0;
//		}

		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        setup_camera();
        setup_light();
        update();
        draw_objects();

//		// Compute the MVP matrix from keyboard and mouse input
//		computeMatricesFromInputs();
//		glm::mat4 ProjectionMatrix = getProjectionMatrix();
//		glm::mat4 ViewMatrix = getViewMatrix();
		
		
		////// Start of the rendering of the first object //////
		
		


		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		   glfwWindowShouldClose(window) == 0 );

	// Cleanup VBO and shader ++++++++++++++++++++
    clean_up();

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}

