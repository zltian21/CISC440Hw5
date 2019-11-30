//Billboard.hpp
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "Character.hpp"
#include "Enemy.hpp"

#include <vector>
#include <algorithm>
#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/norm.hpp>
#include <glm/gtc/random.hpp>

class Billboard{
public:
    glm::vec3 position;
    Character * character;
    Enemy * enemy;
    int type;
    float percent;
    Billboard(
           double, double, double,
           int, Character *,  Enemy *
              );
    void draw();
    void update();

};
