//Water.hpp
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <iostream>

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

class Water{
public:
    glm::vec3 position;
    glm::vec2 critical_position;
    double length = 7.0;
    double width = 1.0;
    int index_x;
    int index_z;
    //int index; // 2 waters a pair
    int remain_time = 18; //30
    int range[13][2];
    
    
    Water(
          double, double, double,  //position of water
          int, int               //index x, z in the map
                               //index 0 or 1 in a pair of waters
          );
    void draw(glm::mat4);
    bool timesUp();
    void update();
    void getMatrix();
};
