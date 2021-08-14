//
//  Camera.hpp
//  LearnOpenGL
//
//  Created by Jeb Collins on 8/13/21.
//

#ifndef Camera_h
#define Camera_h

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>




class Camera
{
    float m_pitch, m_yaw;
    glm::vec3 m_camPos, m_camDir;
    glm::vec3 m_up = glm::vec3(0.f, 1.f, 0.f);
    
    glm::vec4 m_viewMatrix;
    
    
    
public:
    Camera(glm::vec3 camPos = glm::vec3(0.f,0.f,0.f), glm::vec3 camDir = glm::vec3(0.f, 0.f, 1.f));
    
    void setPosition(glm::vec3 position) {m_camPos = position;};
    void setDirection(glm::vec3 direction) {m_camDir = direction;};
    
    void turnLeft(float leftIncrement) {m_yaw -= leftIncrement;};
    void turnRight(float rightIncrement) {m_yaw += rightIncrement;};
    void turnUp(float upIncrement) {m_pitch += upIncrement;};
    void turnDown(float downIncrement) {m_pitch -= downIncrement;};
    
    void moveLeft(float posIncrement) {m_camPos.x -= posIncrement;};
    void moveRight(float posIncrement) {m_camPos.x += posIncrement;};
    void moveUp(float posIncrement) {m_camPos.y += posIncrement;};
    void moveDown(float posIncrement) {m_camPos.y -= posIncrement;};
    void moveForward(float posIncrement) {m_camPos.z += posIncrement;};
    void moveBackward(float posIncrement) {m_camPos.z -= posIncrement;};
    
    void lookAt(glm::vec3 target) {m_camDir = target - m_camPos;};
    
    
    glm::mat4 getViewMatrix();
    
};




#endif /* Camera_h */
