//Bomb.cpp
#include "Bomb.hpp"
#include "Elements.hpp"
#include "Character.hpp"
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

extern GLuint Texture_bomb;
extern GLuint TextureID_bomb;
extern GLuint vertexbuffer_bomb;
extern GLuint uvbuffer_bomb;
extern GLuint normalbuffer_bomb;
extern GLuint elementbuffer_bomb;
extern std::vector<unsigned short> indices_bomb;

//extern Character * character;


using namespace std;

Bomb::Bomb(
    double pos_x, double pos_y, double pos_z,
    int i_x, int i_z
           ){
    position = glm::vec3(pos_x, pos_y, pos_z);
    critical_position = glm::vec2(position.x, position.z);
    index_x = i_x;
    index_z = i_z;
}

//void Bomb::update(){
//    if(on_flag){
//        if((character->critical_position.x > critical_position.x + 0.5 || character->critical_position.x < critical_position.x - 0.5) || (character->critical_position.y > critical_position.y + 0.5 || character->critical_position.y < critical_position.y - 0.5)){
//            on_flag = false;
//        }
//    }
//}


bool Bomb::timesUp(){
    remain_time--;
//    std::cout << remain_time << std::endl;
    if(remain_time <= 0){
        return true;
    }
    return false;
}

void Bomb::draw(glm::mat4 Model){
    glUseProgram(programID);

    glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &View[0][0]);
    
    glm::mat4 MVP = Projection * View * Model;
    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
    glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &Model[0][0]);
    
    glActiveTexture(GL_TEXTURE0);
    //glBindTexture(GL_TEXTURE_2D, Texture);
    glBindTexture(GL_TEXTURE_2D, Texture_bomb);
    
    //glUniform1i(TextureID, 0);
    glUniform1i(TextureID_bomb, 0);
    
    // 1rst attribute buffer : vertices
            glEnableVertexAttribArray(0);
            //glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer_bomb);
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
    glBindBuffer(GL_ARRAY_BUFFER, uvbuffer_bomb);
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
    glBindBuffer(GL_ARRAY_BUFFER, normalbuffer_bomb);
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
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer_bomb);
    
    // Draw the triangles !
    glDrawElements(
        GL_TRIANGLES,      // mode
        //indices.size(),    // count
        indices_bomb.size(),
        GL_UNSIGNED_SHORT,   // type
        (void*)0           // element array buffer offset
    );
    
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
}
