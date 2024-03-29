//Character.hpp

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <vector>
#include <deque>
#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/norm.hpp>
#include <glm/gtc/random.hpp>

using namespace std;
enum Directions{
    NORTH,
    SOUTH,
    WEST,
    EAST
};
class Character{
public:
    glm::vec3 position;
    glm::vec3 new_position;
    glm::vec2 critical_position;
    double length = 0.9;
    glm::vec2 velocity = glm::vec2(0, 0);
    Directions direction = SOUTH;
    int health = 350;
    int MAX_BOMB = 5;
    int current_bomb = 0;
    
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> uvs;
    std::vector<glm::vec3> normals;
    
    std::vector<unsigned short> indices;
    std::vector<glm::vec3> indexed_vertices;
    std::vector<glm::vec2> indexed_uvs;
    std::vector<glm::vec3> indexed_normals;
    
    GLuint vertexbuffer;
    GLuint uvbuffer;
    GLuint normalbuffer;
    GLuint elementbuffer;

    GLuint Texture;
    GLuint TextureID;
    
    bool res;
    
    Character(
    double, double, double  //position of character
              );
    
    void draw(glm::mat4);
    void initialize();
    void update();
    void cleanUp();
    void checkBBox();
    void checkUBox();
    void checkBomb();
    void checkWater();
    void checkEnemy();
    
};

