//Character.cpp
#include <iostream>
#include "Character.hpp"
#include "ground.hpp"
#include "BBox.hpp"
#include "UBox.hpp"
#include "Bomb.hpp"
#include "Water.hpp"
#include "Enemy.hpp"
#include <string.h>
#include <common/shader.hpp>
#include <common/texture.hpp>


#include <common/objloader.hpp>
#include <common/vboindexer.hpp>

extern GLuint programID;
extern GLuint VertexArrayID;
extern GLuint MatrixID;
extern GLuint ViewMatrixID;
extern GLuint ModelMatrixID;
extern GLuint LightID;
extern glm::mat4 View;
extern glm::mat4 Model;
extern glm::mat4 Projection;
extern Ground ground;

extern std::vector<BBox *> bbox_vec;
extern std::vector<UBox *> ubox_vec;
extern std::vector<Bomb *> bomb_vec;
extern std::vector<Enemy *> enemy_vec;
extern std::vector<Water *> water_vec;

Character::Character(
    double pos_x, double pos_y, double pos_z
                     ){
    position = glm::vec3(pos_x, pos_y, pos_z);
    new_position = glm::vec3(pos_x, pos_y, pos_z);
    critical_position = glm::vec2(position.x, position.z);
    
}

void Character::initialize(){
    res = loadOBJ("character.obj", vertices, uvs, normals);
    Texture = loadDDS("character.DDS");
    TextureID  = glGetUniformLocation(programID, "myTextureSampler");
    
    indexVBO(vertices, uvs, normals, indices, indexed_vertices, indexed_uvs, indexed_normals);
    
    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, indexed_vertices.size() * sizeof(glm::vec3), &indexed_vertices[0], GL_STATIC_DRAW);
    
    glGenBuffers(1, &uvbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
    glBufferData(GL_ARRAY_BUFFER, indexed_uvs.size() * sizeof(glm::vec2), &indexed_uvs[0], GL_STATIC_DRAW);

    glGenBuffers(1, &normalbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
    glBufferData(GL_ARRAY_BUFFER, indexed_normals.size() * sizeof(glm::vec3), &indexed_normals[0], GL_STATIC_DRAW);
    
    glGenBuffers(1, &elementbuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), &indices[0] , GL_STATIC_DRAW);
}

void Character::draw(glm::mat4 Model){
    glUseProgram(programID);
    glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &View[0][0]);
    
    glm::mat4 MVP = Projection * View * Model;
    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
    glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &Model[0][0]);
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, Texture);
    
    glUniform1i(TextureID, 0);
    
    // 1rst attribute buffer : vertices
            glEnableVertexAttribArray(0);
            glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
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
            glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
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
            glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
            glVertexAttribPointer(
                2,                                // attribute
                3,                                // size
                GL_FLOAT,                         // type
                GL_FALSE,                         // normalized?
                0,                                // stride
                (void*)0                          // array buffer offset
            );
    
    // Index buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
    
    // Draw the triangles !
    glDrawElements(
        GL_TRIANGLES,      // mode
        indices.size(),    // count
        GL_UNSIGNED_SHORT,   // type
        (void*)0           // element array buffer offset
    );
    
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
}

//bool Character::CheckCollision(BBox *bb){
    
//    cout<<bb->critical_position.x <<endl;
//    bool collisionX = critical_position.x + length/2 >= bb->critical_position.x - bb->length/2 &&
//    bb->critical_position.x + bb->length/2 >= critical_position.x - length/2;
//
//    bool collisionY = critical_position.y + length/2 >= bb->critical_position.y - bb->length/2 &&
//    bb->critical_position.y + bb->length/2 >= critical_position.y - length/2;

//    return collisionX && collisionY;

//}


void Character::checkEnemy(){
    for(int i = 0; i < enemy_vec.size(); i++){
        bool collisionX = new_position.x + length/2 > enemy_vec[i]->critical_position.x - enemy_vec[i]->length/2 &&
        enemy_vec[i]->critical_position.x + enemy_vec[i]->length/2 > new_position.x - length/2;

        bool collisionZ = new_position.z + length/2 > enemy_vec[i]->critical_position.y - enemy_vec[i]->length/2 &&
        enemy_vec[i]->critical_position.y + enemy_vec[i]->length/2 > new_position.z - length/2;

        if(collisionX && collisionZ){
            health--;
//            cout << health << endl;
        }
    }
}

void Character::checkWater(){
    
    for(int i = 0; i < water_vec.size(); i++){
        
        for(int j = 0; j < 13; j++){
            if((int)critical_position.x == water_vec[i]->range[j][0] && (int)critical_position.y == water_vec[i]->range[j][1]){
                health--;
                cout << health << endl;
            }
        }
    }
}


void Character::checkBomb(){
    for(int i = 0; i < bomb_vec.size(); i++){
        if(bomb_vec[i]->on_flag){
            if((position.x - 0.5 >= bomb_vec[i]->critical_position.x + 0.5 || position.x + 0.5 <= bomb_vec[i]->critical_position.x - 0.5) || (position.z - 0.5 >= bomb_vec[i]->critical_position.y + 0.5 || position.z + 0.5 <= bomb_vec[i]->critical_position.y - 0.5)){
                bomb_vec[i]->on_flag = false;
            }
        }else{
            bool collisionX = new_position.x + length/2 > bomb_vec[i]->critical_position.x - bomb_vec[i]->length/2 &&
            bomb_vec[i]->critical_position.x + bomb_vec[i]->length/2 > new_position.x - length/2;
            
            bool collisionZ = new_position.z + length/2 > bomb_vec[i]->critical_position.y - bomb_vec[i]->length/2 &&
            bomb_vec[i]->critical_position.y + bomb_vec[i]->length/2 > new_position.z - length/2;
            
            if(collisionX && collisionZ){
                new_position = position;
            }
        }
    }
}

void Character::checkUBox(){
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

void Character::checkBBox(){
    
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
void Character::update(){
    
      new_position = glm::vec3(position.x + velocity.x, position.y, position.z + velocity.y);
    
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
    checkEnemy();
    checkWater();
    if(health <= 0){
        cout << "GAME OVER" << endl;
    }

    
    position = new_position;
    
    critical_position = glm::vec2(position.x, position.z);


}


void Character::cleanUp(){
    glDeleteBuffers(1, &vertexbuffer);
    glDeleteBuffers(1, &uvbuffer);
    glDeleteBuffers(1, &normalbuffer);
    glDeleteBuffers(1, &elementbuffer);
    glDeleteProgram(programID);
    glDeleteTextures(1, &Texture);
    glDeleteVertexArrays(1, &VertexArrayID);
}
