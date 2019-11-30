////Billboard.cpp
#include "Billboard.hpp"
//#include "Character.hpp"
//#include "Enemy.hpp"
#include <string.h>
#include <common/shader.hpp>
#include <common/texture.hpp>
#include <iostream>

#include <common/objloader.hpp>
#include <common/vboindexer.hpp>

extern GLuint programID_billboard;
extern GLuint CameraRight_worldspace_ID;
extern GLuint CameraUp_worldspace_ID;
extern GLuint ViewProjMatrixID;
extern GLuint BillboardPosID;
extern GLuint BillboardSizeID;
extern GLuint LifeLevelID;
extern GLuint TextureID_billboard;
extern GLuint Texture_billboard;
extern const GLfloat g_vertex_buffer_data[];
extern GLuint billboard_vertex_buffer;
extern glm::mat4 ViewProjectionMatrix;
extern glm::mat4 View;
extern glm::mat4 Model;
extern glm::mat4 Projection;

extern GLuint programID_billboard_E;
extern GLuint CameraRight_worldspace_ID_E;
extern GLuint CameraUp_worldspace_ID_E;
extern GLuint ViewProjMatrixID_E;
extern GLuint BillboardPosID_E;
extern GLuint BillboardSizeID_E;
extern GLuint LifeLevelID_E;
extern GLuint TextureID_billboard_E;

using namespace std;

Billboard::Billboard(
            double pos_x, double pos_y, double pos_z,
            int t, Character * ch, Enemy * en
                     ){
    position = glm::vec3(pos_x, pos_y, pos_z);
    type = t;
    character = ch;
    enemy = en;
    
    
    
}

void Billboard::update(){
    
    if(character == NULL){
        percent = (float)enemy->health / (float) 100 * 0.97;
        position.x = enemy->position.x;
        position.y = enemy->position.y;
        position.z = enemy->position.z;
        
    }else if(enemy == NULL){
        percent = (float) character->health / (float) 350 * 0.97;
        position.x = character->position.x;
        position.y = character->position.y;
        position.z = character->position.z;
    }
}

void Billboard::draw(){
    ViewProjectionMatrix = Projection * View;
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    float LifeLevel = percent;
    
    if(type == 0){
        
        glUseProgram(programID_billboard);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, Texture_billboard);
        glUniform1i(TextureID_billboard, 0);
        //glDisable(GL_BLEND);
        
        glUniform3f(CameraRight_worldspace_ID, View[0][0], View[1][0], View[2][0]);
        glUniform3f(CameraUp_worldspace_ID   , View[0][1], View[1][1], View[2][1]);
        glUniform3f(BillboardPosID, position.x, position.y+1.5, position.z); // The billboard will be just above the cube
        glUniform2f(BillboardSizeID, 1.5f, 0.225f); // and 1m*12cm, because it matches its 256*32 resolution =)
//        float LifeLevel = percent;
        glUniform1f(LifeLevelID, LifeLevel);
        glUniformMatrix4fv(ViewProjMatrixID, 1, GL_FALSE, &ViewProjectionMatrix[0][0]);
    }else{
        glUseProgram(programID_billboard_E);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, Texture_billboard);
        glUniform3f(CameraRight_worldspace_ID_E, View[0][0], View[1][0], View[2][0]);
        glUniform3f(CameraUp_worldspace_ID_E   , View[0][1], View[1][1], View[2][1]);
        glUniform3f(BillboardPosID_E, position.x, position.y+1.5, position.z);
        glUniform2f(BillboardSizeID_E, 1.5f, 0.225f);
        glUniform1f(LifeLevelID_E, LifeLevel);
        glUniformMatrix4fv(ViewProjMatrixID_E, 1, GL_FALSE, &ViewProjectionMatrix[0][0]);
    }
    glDisable(GL_BLEND);

    
    
    
    // 1rst attribute buffer : vertices
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, billboard_vertex_buffer);
    glVertexAttribPointer(
        0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
        3,                  // size
        GL_FLOAT,           // type
        GL_FALSE,           // normalized?
        0,                  // stride
        (void*)0            // array buffer offset
    );
    
    // Draw the billboard !
            // This draws a triangle_strip which looks like a quad.
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    
            glDisableVertexAttribArray(0);
}
