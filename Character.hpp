//Character.hpp

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "Enums.hpp"

#include <vector>
#include <deque>
#include <string>

#include <sys/time.h>
#include <unistd.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/norm.hpp>
#include <glm/gtc/random.hpp>

using namespace std;

class Character{
public:
    glm::vec3 position;
    double length = 1;
    double velocity = 0.1;
    enum Direction direction = South;
    
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
};
