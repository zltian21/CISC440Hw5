//Bomb.hpp
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

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

class Bomb{
public:
    glm::vec3 position;
    glm::vec2 critical_position;
    double length = 1.0;
    int index_x;
    int index_z;
    int remain_time = 200;
    bool on_flag = true;
    
    Bomb(
         double, double, double, //position of Bomb
         int, int               //index x, z of the bomb
         );
    
    void draw(glm::mat4);
    bool timesUp();
    void update();
};
