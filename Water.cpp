//Water.cpp
#include "Water.hpp"
#include "Elements.hpp"
#include "Character.hpp"
#include "Bomb.hpp"
#include "BBox.hpp"
#include "Enemy.hpp"
#include <string.h>
#include <common/shader.hpp>
#include <common/texture.hpp>
#include <iostream>

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

extern GLuint Texture_water;
extern GLuint TextureID_water;
extern GLuint vertexbuffer_water;
extern GLuint uvbuffer_water;
extern GLuint normalbuffer_water;
extern GLuint elementbuffer_water;
extern std::vector<unsigned short> indices_water;

//Game Objects
extern std::vector<Bomb *> bomb_vec;
extern Elements map[20][20];
extern Character * character;

extern std::vector<BBox *> bbox_vec;
extern std::vector<Enemy *> enemy_vec;


Water::Water(
    double pos_x, double pos_y, double pos_z,
    int i_x, int i_z
             ){
    position = glm::vec3(pos_x, pos_y, pos_z);
    critical_position = glm::vec2(position.x, position.z);
    index_x = i_x;
    index_z = i_z;
    //int range[13][2];
    for(int i = 0; i < 3; i++){
        range[i][0] = (index_x-3)+i;
        range[i][1] = index_z;
        
        range[4+i][0] = (index_x + 1) + i;
        range[4+i][1] = index_z;
        
        range[7+i][0] = index_x;
        range[7+i][1] = (index_z-3)+i;
        
        range[10+i][0] = index_x;
        range[10+i][1] = (index_z+1)+i;
    }
    range[3][0] = index_x;
    range[3][1] = index_z;
    
    //index = i;
}

//map[range[i][0]][range[i][1]]
void Water::update(){
    for(int i = 0; i < 13; i++){
        if(map[range[i][0]][range[i][1]] != Elements::EMPTY){
            if(map[range[i][0]][range[i][1]] == Elements::BOMB){
                
                for(std::vector<Bomb *>::iterator it = bomb_vec.begin(); it != bomb_vec.end();++it){

//                    cout << (*it)->index_x << endl;
                    if((*it)->index_x == range[i][0] && (*it)->index_z == range[i][1]){
//                        cout << (*it)->index_x << endl;
//                        cout << range[i][0] << " " << range[i][1] << endl;
                        (*it)->remain_time = 0;
                    }
                }
            }else if(map[range[i][0]][range[i][1]] == Elements::BBOX){
                
                for(std::vector<BBox *>::iterator itr = bbox_vec.begin(); itr != bbox_vec.end();++itr){
//                    
                    if((*itr)->index_x == range[i][0] && (*itr)->index_z == range[i][1]){
                        map[range[i][0]][range[i][1]] = Elements::EMPTY;
//                        cout << "Hello" << endl;
                        BBox * temp = *itr;
                        delete(temp);
                        itr = bbox_vec.erase(itr);
                        break;
                    }
                }
            }else if(map[range[i][0]][range[i][1]] == Elements::ENEMY){
                for(std::vector<Enemy *>::iterator itr = enemy_vec.begin(); itr != enemy_vec.end();++itr){
                    if((*itr)->index_x == range[i][0] && (*itr)->index_z == range[i][1]){
                        (*itr)->health --;
                        cout << (*itr)->health << endl;
                        if((*itr)->health <= 0){
                            map[range[i][0]][range[i][1]] = Elements::EMPTY;
                            Enemy * temp = *itr;
                            delete(temp);
                            itr = enemy_vec.erase(itr);
                        }
                        break;
                        
                    }
                }
            }
            
//            else if(map[range[i][0]][range[i][1]] == Elements::BBOX){
//                for(std::vector<BBox *>::iterator it = bbox_vec.begin(); it != bbox_vec.end();++it){
//                    if((*it)->index_x == range[i][0] && (*it)->index_z == range[i][1]){
//                        map[range[i][0]][range[i][1]] = Elements::EMPTY;
//                        BBox * temp = *it;
//                        //delete(temp);
//                        it = bbox_vec.erase(it);
//                        cout<< "HEllo" << endl;
//                    }
//                }
//            }
        }
    }
}

bool Water::timesUp(){
    remain_time--;
//    std::cout << remain_time << std::endl;
    if(remain_time <= 0){
        return true;
    }
    return false;
}

void Water::draw(glm::mat4 Model){
    glUseProgram(programID);

    glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &View[0][0]);
    
    glm::mat4 MVP = Projection * View * Model;
    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
    glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &Model[0][0]);
    
    glActiveTexture(GL_TEXTURE0);
    //glBindTexture(GL_TEXTURE_2D, Texture);
    glBindTexture(GL_TEXTURE_2D, Texture_water);
    
    //glUniform1i(TextureID, 0);
    glUniform1i(TextureID_water, 0);
    
    // 1rst attribute buffer : vertices
            glEnableVertexAttribArray(0);
            //glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer_water);
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
    glBindBuffer(GL_ARRAY_BUFFER, uvbuffer_water);
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
    glBindBuffer(GL_ARRAY_BUFFER, normalbuffer_water);
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
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer_water);
    
    // Draw the triangles !
    glDrawElements(
        GL_TRIANGLES,      // mode
        //indices.size(),    // count
        indices_water.size(),
        GL_UNSIGNED_SHORT,   // type
        (void*)0           // element array buffer offset
    );
    
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
}

