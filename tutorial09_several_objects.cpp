// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include "ground.hpp"
#include "Character.hpp"
#include "BBox.hpp"
#include "Bomb.hpp"
#include "Water.hpp"
#include "UBox.hpp"
#include "Elements.hpp"
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

//UBoxes
std::vector<glm::vec3> vertices_ubox;
std::vector<glm::vec2> uvs_ubox;
std::vector<glm::vec3> normals_ubox;

GLuint Texture_ubox;
GLuint TextureID_ubox;

std::vector<unsigned short> indices_ubox;
std::vector<glm::vec3> indexed_vertices_ubox;
std::vector<glm::vec2> indexed_uvs_ubox;
std::vector<glm::vec3> indexed_normals_ubox;

GLuint vertexbuffer_ubox;
GLuint uvbuffer_ubox;
GLuint normalbuffer_ubox;
GLuint elementbuffer_ubox;

//Bombs
std::vector<glm::vec3> vertices_bomb;
std::vector<glm::vec2> uvs_bomb;
std::vector<glm::vec3> normals_bomb;

GLuint Texture_bomb;
GLuint TextureID_bomb;

std::vector<unsigned short> indices_bomb;
std::vector<glm::vec3> indexed_vertices_bomb;
std::vector<glm::vec2> indexed_uvs_bomb;
std::vector<glm::vec3> indexed_normals_bomb;

GLuint vertexbuffer_bomb;
GLuint uvbuffer_bomb;
GLuint normalbuffer_bomb;
GLuint elementbuffer_bomb;

//Waters
std::vector<glm::vec3> vertices_water;
std::vector<glm::vec2> uvs_water;
std::vector<glm::vec3> normals_water;

GLuint Texture_water;
GLuint TextureID_water;

std::vector<unsigned short> indices_water;
std::vector<glm::vec3> indexed_vertices_water;
std::vector<glm::vec2> indexed_uvs_water;
std::vector<glm::vec3> indexed_normals_water;

GLuint vertexbuffer_water;
GLuint uvbuffer_water;
GLuint normalbuffer_water;
GLuint elementbuffer_water;


//camera
double cam_x = 10; //0
double cam_y = 23; // 13
double cam_z = 25; // 10

//
glm::mat4 View;
glm::mat4 Model;
glm::mat4 Projection;


//Game Objects

std::vector<Bomb *> bomb_vec;
std::vector<Water *> water_vec;
std::vector<BBox *> bbox_vec;
std::vector<UBox *> ubox_vec;

Elements map[20][20];

Ground * ground = new Ground(10, 0, 10);
Character * character = new Character(10, 1.5, 10);




void initialize_bbox(){
    
    map[14][10] = Elements::BBOX;
    bbox_vec.push_back(new BBox(14.5, 1.0, 10.5, 14, 10));
    
    map[15][11] = Elements::BBOX;
    bbox_vec.push_back(new BBox(15.5, 1.0, 11.5, 15, 11));
    
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

void initialize_ubox(){
    
    map[2][2] = Elements::UBOX;
    ubox_vec.push_back(new UBox(2.5, 1.0, 2.5, 2, 2));
    
    map[18][19] = Elements::UBOX;
    ubox_vec.push_back(new UBox(18.5, 1.0, 19.5, 18, 19));
    
    bool res = loadOBJ("Ubox.obj", vertices_ubox, uvs_ubox, normals_ubox);
    Texture_ubox = loadDDS("Ubox.DDS");
    TextureID_ubox  = glGetUniformLocation(programID, "myTextureSampler");
    
    indexVBO(vertices_ubox, uvs_ubox, normals_ubox, indices_ubox, indexed_vertices_ubox, indexed_uvs_ubox, indexed_normals_ubox);
    
    glGenBuffers(1, &vertexbuffer_ubox);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer_ubox);
    glBufferData(GL_ARRAY_BUFFER, indexed_vertices_ubox.size() * sizeof(glm::vec3), &indexed_vertices_ubox[0], GL_STATIC_DRAW);
    
    glGenBuffers(1, &uvbuffer_ubox);
    glBindBuffer(GL_ARRAY_BUFFER, uvbuffer_ubox);
    glBufferData(GL_ARRAY_BUFFER, indexed_uvs_ubox.size() * sizeof(glm::vec2), &indexed_uvs_ubox[0], GL_STATIC_DRAW);
    
    glGenBuffers(1, &normalbuffer_ubox);
    glBindBuffer(GL_ARRAY_BUFFER, normalbuffer_ubox);
    glBufferData(GL_ARRAY_BUFFER, indexed_normals_ubox.size() * sizeof(glm::vec3), &indexed_normals_ubox[0], GL_STATIC_DRAW);
    
    glGenBuffers(1, &elementbuffer_ubox);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer_ubox);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_ubox.size() * sizeof(unsigned short), &indices_ubox[0] , GL_STATIC_DRAW);
    
}



void initialize_bomb(){
    bool res = loadOBJ("Bomb.obj", vertices_bomb, uvs_bomb, normals_bomb);
    Texture_bomb = loadDDS("bomb.DDS");
    TextureID_bomb  = glGetUniformLocation(programID, "myTextureSampler");
    
    indexVBO(vertices_bomb, uvs_bomb, normals_bomb, indices_bomb, indexed_vertices_bomb, indexed_uvs_bomb, indexed_normals_bomb);
    
    glGenBuffers(1, &vertexbuffer_bomb);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer_bomb);
    glBufferData(GL_ARRAY_BUFFER, indexed_vertices_bomb.size() * sizeof(glm::vec3), &indexed_vertices_bomb[0], GL_STATIC_DRAW);
    
    glGenBuffers(1, &uvbuffer_bomb);
    glBindBuffer(GL_ARRAY_BUFFER, uvbuffer_bomb);
    glBufferData(GL_ARRAY_BUFFER, indexed_uvs_bomb.size() * sizeof(glm::vec2), &indexed_uvs_bomb[0], GL_STATIC_DRAW);
    
    glGenBuffers(1, &normalbuffer_bomb);
    glBindBuffer(GL_ARRAY_BUFFER, normalbuffer_bomb);
    glBufferData(GL_ARRAY_BUFFER, indexed_normals_bomb.size() * sizeof(glm::vec3), &indexed_normals_bomb[0], GL_STATIC_DRAW);
    
    glGenBuffers(1, &elementbuffer_bomb);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer_bomb);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_bomb.size() * sizeof(unsigned short), &indices_bomb[0] , GL_STATIC_DRAW);
}

void initialize_water(){
    bool res = loadOBJ("water.obj", vertices_water, uvs_water, normals_water);
    Texture_water = loadDDS("water.DDS");
    TextureID_water  = glGetUniformLocation(programID, "myTextureSampler");
    
    indexVBO(vertices_water, uvs_water, normals_water, indices_water, indexed_vertices_water, indexed_uvs_water, indexed_normals_water);
    
    glGenBuffers(1, &vertexbuffer_water);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer_water);
    glBufferData(GL_ARRAY_BUFFER, indexed_vertices_water.size() * sizeof(glm::vec3), &indexed_vertices_water[0], GL_STATIC_DRAW);
    
    glGenBuffers(1, &uvbuffer_water);
    glBindBuffer(GL_ARRAY_BUFFER, uvbuffer_water);
    glBufferData(GL_ARRAY_BUFFER, indexed_uvs_water.size() * sizeof(glm::vec2), &indexed_uvs_water[0], GL_STATIC_DRAW);
    
    glGenBuffers(1, &normalbuffer_water);
    glBindBuffer(GL_ARRAY_BUFFER, normalbuffer_water);
    glBufferData(GL_ARRAY_BUFFER, indexed_normals_water.size() * sizeof(glm::vec3), &indexed_normals_water[0], GL_STATIC_DRAW);
    
    glGenBuffers(1, &elementbuffer_water);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer_water);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_water.size() * sizeof(unsigned short), &indices_water[0] , GL_STATIC_DRAW);
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

void cleanUp_ubox(){
    glDeleteBuffers(1, &vertexbuffer_ubox);
    glDeleteBuffers(1, &uvbuffer_ubox);
    glDeleteBuffers(1, &normalbuffer_ubox);
    glDeleteBuffers(1, &elementbuffer_ubox);
    glDeleteProgram(programID);
    glDeleteTextures(1, &Texture_ubox);
    glDeleteVertexArrays(1, &VertexArrayID);
}

void cleanUp_bomb(){
    glDeleteBuffers(1, &vertexbuffer_bomb);
    glDeleteBuffers(1, &uvbuffer_bomb);
    glDeleteBuffers(1, &normalbuffer_bomb);
    glDeleteBuffers(1, &elementbuffer_bomb);
    glDeleteProgram(programID);
    glDeleteTextures(1, &Texture_bomb);
    glDeleteVertexArrays(1, &VertexArrayID);
}

void cleanUp_water(){
    glDeleteBuffers(1, &vertexbuffer_water);
    glDeleteBuffers(1, &uvbuffer_water);
    glDeleteBuffers(1, &normalbuffer_water);
    glDeleteBuffers(1, &elementbuffer_water);
    glDeleteProgram(programID);
    glDeleteTextures(1, &Texture_water);
    glDeleteVertexArrays(1, &VertexArrayID);
}


void initialize_objects(){
    ground->initialize();
    character->initialize();
    initialize_bbox();
    initialize_ubox();
    initialize_bomb();
    initialize_water();

}

void draw_objects(){
    //Ground
    ground->draw(Model * glm::translate(ground->position));
    
    //BBOX
    for(int i = 0; i < bbox_vec.size(); i++){
        bbox_vec[i]->draw(Model * glm::translate(bbox_vec[i]->position));
    }
    
    //UBOX
    for(int i = 0; i < ubox_vec.size(); i++){
        ubox_vec[i]->draw(Model * glm::translate(ubox_vec[i]->position));
    }
    
    //water->draw(Model* glm::translate(water->position));
    for(int i = 0; i < bomb_vec.size(); i++){
        bomb_vec[i]->draw(Model * glm::translate(bomb_vec[i]->position));
    }
    
    for(int i = 0; i < water_vec.size(); i++){
        water_vec[i]->draw(Model * glm::translate(water_vec[i]->position));
        water_vec[i]->draw(Model * glm::translate(water_vec[i]->position) * glm::rotate((float) (0.50*M_PI),
        glm::vec3(0.0f, 1.0f, 0.0f)));
    }
    
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

void update(){
    character->update();
    
    for(std::vector<Bomb *>::iterator it = bomb_vec.begin(); it != bomb_vec.end(); ){
        if((*it)->timesUp()){
            map[(*it)->index_x][(*it)->index_z] = Elements::WATERPR;
            Bomb * temp = *it;  //
            delete(temp);       //Not Sure
            
            it = bomb_vec.erase(it);
        }else{
            ++it;
        }
    }
    
    for(std::vector<Water *>::iterator it = water_vec.begin(); it != water_vec.end();){
        if((*it)->timesUp()){
            map[(*it)->index_x][(*it)->index_z] = Elements::EMPTY;
            Water *temp = *it;
            delete(temp);
            it = water_vec.erase(it);
        }else{
            (*it)->update();
            
            ++it;
        }
    }
    
    
    
    //Elements map[20][20];
    //std::vector<std::vector<Water *> > water_vec;
    //bomb_vec.push_back(new Bomb((double)temp_x + 0.5, 1, (double)temp_z + 0.5, temp_x, temp_z));
   
    for(int i = 0; i < 20; i++){
        for(int j = 0; j < 20; j++){
            if(map[i][j] == Elements::WATERPR){
                map[i][j] = Elements::WATERDO;
                water_vec.push_back(new Water((double)i + 0.5, 1.0, (double)j + 0.5, i, j));
            }
        }
    }
//    for(int i = 0; i < bomb_vec.size(); i++){
//        // cout << "Hello " << endl;
//        if(bomb_vec[i]->timesUp() == true){
//            map[bomb_vec[i]->index_x][bomb_vec[i]->index_z] = Elements::EMPTY;
//            bomb_vec.erase(bomb_vec.begin()+1);
//
//        }
//    }
    
}

void clean_up(){
    ground->cleanUp();
    character->cleanUp();
    cleanUp_bbox();
    cleanUp_ubox();
    cleanUp_bomb();
    cleanUp_water();
    
}

void setup_light(){
    glm::vec3 lightPos = glm::vec3(10,30,10);
    glUniform3f(LightID, lightPos.x, lightPos.y, lightPos.z);
}

void setup_camera(){
    View = glm::lookAt(glm::vec3(cam_x,cam_y,cam_z), // Camera is at (4,3,3), in World Space
                       glm::vec3(10, 0, 10), // and looks at the origin
                       glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
                       );
}



void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
    
    //CAMERA CONTROL
    if (key == GLFW_KEY_0 && (action == GLFW_PRESS || action == GLFW_REPEAT)){
        cam_x = 10;
        cam_y = 23;
        cam_z = 25;
    }else if(key == GLFW_KEY_L && (action == GLFW_PRESS || action == GLFW_REPEAT)){
        cam_x = 10;
        cam_y = 0;
        cam_z = 40;
    }else if(key == GLFW_KEY_O && (action == GLFW_PRESS || action == GLFW_REPEAT)){
        cam_x = 10;
        cam_y = 27;
        cam_z = 10.1;
    }else if(key == GLFW_KEY_P && (action == GLFW_PRESS || action == GLFW_REPEAT)){
        cam_x = 40;
        cam_y = 0;
        cam_z = 10;
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
    }else if(key == GLFW_KEY_SPACE && (action == GLFW_PRESS || action == GLFW_REPEAT)){
        int temp_x = (int) character->critical_position.x;
        int temp_z = (int) character->critical_position.y;
        
        if(map[temp_x][temp_z] == Elements::EMPTY){
            map[temp_x][temp_z] = Elements::BOMB;
            bomb_vec.push_back(new Bomb((double)temp_x + 0.5, 1, (double)temp_z + 0.5, temp_x, temp_z));
        }
        
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
    
    for(int i = 0; i < 20; i++){
        for(int j = 0; j < 20; j++){
            map[i][j] = Elements::EMPTY;
        }
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
    
    //
    
    
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

