//Enemy.hpp

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

enum EDirections{
    N,
    S,
    W,
    E
};

class Enemy{
public:
    glm::vec3 position;
    glm::vec2 critical_position;
    glm::vec3 new_position;
    double length = 1.0;
    glm::vec2 velocity = glm::vec2(0, 0);
    glm::vec2 new_velocity;
    glm::vec2 hunger_force;
    EDirections direction;
    int index_x;
    int index_z;
    int health = 100;

    double random_force_limit;
    double hunger_weight;
    double min_hunger_distance, min_squared_hunger_distance;
    double max_hunger_distance, max_squared_hunger_distance;
    double inv_range_squared_hunger_distance;
    double predator_character_squared_distance;

    
    Enemy(
        double, double, double,   //position of enemy
        EDirections,               //Direction of enemy
        double,                     //random uniform acceleration limit
        double, double, double,    // min, max hunger distance, wight
        int, int                    //index of Enemy x,z in map
    
          );
    
     void draw(glm::mat4);
     void update();
     void calculate_predator_character_squared_distances();
     bool compute_hunger_force();
     void updateVelocity();
     void checkBBox();
     void checkUBox();
     void checkBomb();
};

void calculate_predators_character_squared_distances();
