//UBox.hpp
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
class UBox{
public:
    glm::vec3 position;
    glm::vec2 critical_position;
    double length = 0.9;
    int index_x;
    int index_z;
    
    UBox(
         double, double, double, //position of UBox
         int, int                //Index x, z in the map
         );
    
    void draw(glm::mat4);
};
