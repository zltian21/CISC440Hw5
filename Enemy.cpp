//Enemy.cpp
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "Enemy.hpp"
#include "BBox.hpp"
#include "UBox.hpp"
#include "Bomb.hpp"
#include "Elements.hpp"
#include "Character.hpp"
#include <string.h>
#include <common/shader.hpp>
#include <common/texture.hpp>

extern GLuint programID;
extern GLuint VertexArrayID;
extern GLuint MatrixID;
extern GLuint ViewMatrixID;
extern GLuint ModelMatrixID;
extern GLuint LightID;
extern glm::mat4 View;
extern glm::mat4 Model;
extern glm::mat4 Projection;

extern GLuint Texture_enemy;
extern GLuint TextureID_enemy;
extern GLuint vertexbuffer_enemy;
extern GLuint uvbuffer_enemy;
extern GLuint normalbuffer_enemy;
extern GLuint elementbuffer_enemy;
extern std::vector<unsigned short> indices_enemy;

extern Character * character;
extern std::vector<BBox *> bbox_vec;
extern std::vector<UBox *> ubox_vec;
extern std::vector<Bomb *> bomb_vec;
extern Elements map[20][20];


Enemy::Enemy(
        double pos_x, double pos_y, double pos_z,
        EDirections d,
        double rand_force_limit,
        double min_starvation_distance, double max_starvation_distance, double starvation_weight,
        int i_x, int i_z
             ){
        position = glm::vec3(pos_x, pos_y, pos_z);
        new_position = glm::vec3(pos_x, pos_y, pos_z);
        critical_position = glm::vec2(position.x, position.z);
        direction = d;
    
        hunger_weight = starvation_weight;
        
        random_force_limit = rand_force_limit;

        min_hunger_distance = min_starvation_distance;
        min_squared_hunger_distance = min_hunger_distance * min_hunger_distance;
    
        max_hunger_distance = max_starvation_distance;
        max_squared_hunger_distance = max_hunger_distance * max_hunger_distance;
    
        inv_range_squared_hunger_distance = 1.0 / (max_squared_hunger_distance - min_squared_hunger_distance);
        index_x = i_x;
        index_z = i_z;

}

void Enemy::calculate_predator_character_squared_distances(){
    glm::vec2 diff;
    diff = critical_position - character->critical_position;
    predator_character_squared_distance = glm::length2(diff);
//    cout << predator_character_squared_distance << endl;
}

bool Enemy::compute_hunger_force(){
    glm::vec2 forward;
    int count = 0;
    double F;
    double percent;
    hunger_force = glm::vec2(0, 0);
//    cout << "predator_character_squared_distance:" << predator_character_squared_distance << endl;
//    cout << "min_hunger_distance: " << min_hunger_distance << endl;
//    cout << "max_hunger_distance: " << max_hunger_distance << endl;
    if(predator_character_squared_distance >= min_hunger_distance && predator_character_squared_distance <= max_hunger_distance){
        
        percent = (predator_character_squared_distance - max_hunger_distance) * inv_range_squared_hunger_distance;
        F = 0.5 + -0.5 * cos(percent * 2.0 * M_PI);
        
        forward = (float) F * glm::normalize(character->critical_position - critical_position);
        
        hunger_force += forward;
        
        if(hunger_force.x != 0.0 && hunger_force.y != 0.0){
            
            int temp = rand()%2;
            if(temp == 0){
                hunger_force.y = 0.0;
            }else{
                hunger_force.x = 0.0;
            }
            
            //cout << hunger_force.y << endl;
        }
        count++;
        
    }
    
    if(count > 0){
        hunger_force *= (float) hunger_weight;
        flag = !flag;
        return true;
    }else{
        return false;
    }
}

void Enemy::updateVelocity(){
    compute_hunger_force();
    glm::vec2 temp = new_velocity;
    new_velocity = hunger_force;

    
    
    if(new_velocity.x == 0 && new_velocity.y == 0){
        if(flag == oldflag){
            flagcount--;
            if(flagcount <= 0){
                flagcount = 150;
                int temp = rand()%2;
                
                if(temp == 0){
                    temp = rand()%2;
                    if(temp == 0){
                        new_velocity.x +=  random_force_limit;
                    }else{
                        new_velocity.x -=  random_force_limit;
                    }
                
                }else{
                    temp = rand()%2;
                    if(temp == 0){
                        new_velocity.y +=  random_force_limit;
                    }else{
                        new_velocity.y -=  random_force_limit;
                    }
                }
            }else{
                new_velocity = temp;
            }
        }else{
            oldflag = flag;
        }
        
    }
}

void Enemy::checkBomb(){
    for(int i = 0; i < bomb_vec.size(); i++){
        bool collisionX = new_position.x + length/2 > bomb_vec[i]->critical_position.x - bomb_vec[i]->length/2 &&
        bomb_vec[i]->critical_position.x + bomb_vec[i]->length/2 > new_position.x - length/2;
        
        bool collisionZ = new_position.z + length/2 > bomb_vec[i]->critical_position.y - bomb_vec[i]->length/2 &&
        bomb_vec[i]->critical_position.y + bomb_vec[i]->length/2 > new_position.z - length/2;
        
        if(collisionX && collisionZ){
            new_position = position;
        }
    }
    
}

void Enemy::checkUBox(){
    for(int i = 0; i < ubox_vec.size(); i++){
        
        bool collisionX = new_position.x + length/2 > ubox_vec[i]->critical_position.x - ubox_vec[i]->length/2 &&
        ubox_vec[i]->critical_position.x + ubox_vec[i]->length/2 > new_position.x - length/2;

        bool collisionZ = new_position.z + length/2 > ubox_vec[i]->critical_position.y - ubox_vec[i]->length/2 &&
        ubox_vec[i]->critical_position.y + ubox_vec[i]->length/2 > new_position.z - length/2;

        if(collisionX && collisionZ){
            new_position = position;
        }
    }
}
void Enemy::checkBBox(){
    for(int i = 0; i < bbox_vec.size(); i++){
        
        bool collisionX = new_position.x + length/2 > bbox_vec[i]->critical_position.x - bbox_vec[i]->length/2 &&
        bbox_vec[i]->critical_position.x + bbox_vec[i]->length/2 > new_position.x - length/2;

        bool collisionZ = new_position.z + length/2 > bbox_vec[i]->critical_position.y - bbox_vec[i]->length/2 &&
        bbox_vec[i]->critical_position.y + bbox_vec[i]->length/2 > new_position.z - length/2;

        if(collisionX && collisionZ){
            new_position = position;
        }
    }
}

void Enemy::update(){
    calculate_predator_character_squared_distances();
    updateVelocity();
    if(new_velocity.y == 0 && new_velocity.x == 0){
        direction = S;
    }else if(new_velocity.x == 0){
        if(new_velocity.y > 0){
            direction = S;
        }else{
            direction = N;
        }
    }else if(new_velocity.y == 0){
        if(new_velocity.x > 0){
            direction = E;
        }else{
            direction = W;
        }
    }
    new_position = glm::vec3(position.x + new_velocity.x, position.y, position.z + new_velocity.y);
    if((new_position.x - length/2) <= 0.0){
            new_position.x = 0.0 + length/2;
        }
        if((new_position.x + length/2) >= 20.0){
            new_position.x = 20.0 - length/2;
        }
    
        if((new_position.z - length/2) <= 0.0){
            new_position.z = 0.0 + length/2;
        }
        if((new_position.z + length/2) >= 20.0){
            new_position.z = 20.0 - length/2;
        }
    checkBBox();
    checkUBox();
    checkBomb();
    
    position = new_position;
    int tempX = (int) position.x;
    int tempZ = (int) position.z;
    map[index_x][index_z] = Elements::EMPTY;
    index_x = tempX;
    index_z = tempZ;
    map[index_x][index_z] = Elements::ENEMY;
    
    
    critical_position = glm::vec2(position.x, position.z);
    

}

void Enemy::draw(glm::mat4 Model){
    glUseProgram(programID);

    glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &View[0][0]);
    
    glm::mat4 MVP = Projection * View * Model;
    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
    glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &Model[0][0]);
    
    glActiveTexture(GL_TEXTURE0);
    //glBindTexture(GL_TEXTURE_2D, Texture);
    glBindTexture(GL_TEXTURE_2D, Texture_enemy);
    
    //glUniform1i(TextureID, 0);
    glUniform1i(TextureID_enemy, 0);
    
    // 1rst attribute buffer : vertices
            glEnableVertexAttribArray(0);
            //glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer_enemy);
            glVertexAttribPointer(
                0,                  // attribute
                3,                  // size
                GL_FLOAT,           // type
                GL_FALSE,           // normalized?
                0,                  // stride
                (void*)0            // array buffer offset
            );
    
            // 2nd attribute buffer : UVs
            glEnableVertexAttribArray(1);
            //glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, uvbuffer_enemy);
            glVertexAttribPointer(
                1,                                // attribute
                2,                                // size
                GL_FLOAT,                         // type
                GL_FALSE,                         // normalized?
                0,                                // stride
                (void*)0                          // array buffer offset
            );
    
            // 3rd attribute buffer : normals
            glEnableVertexAttribArray(2);
            //glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, normalbuffer_enemy);
            glVertexAttribPointer(
                2,                                // attribute
                3,                                // size
                GL_FLOAT,                         // type
                GL_FALSE,                         // normalized?
                0,                                // stride
                (void*)0                          // array buffer offset
            );
    
    // Index buffer
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer_enemy);
    
    // Draw the triangles !
    glDrawElements(
        GL_TRIANGLES,      // mode
        //indices.size(),    // count
    indices_enemy.size(),
        GL_UNSIGNED_SHORT,   // type
        (void*)0           // element array buffer offset
    );
    
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
}
