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
    float m_fov, m_aspectRatio, m_nearField, m_farField;
    glm::vec3 m_camPos, m_camDir, m_camRight;
    glm::vec3 m_up = glm::vec3(0.f, 1.f, 0.f);
    
    glm::vec4 m_viewMatrix, m_projMatrix;
    
    
    
public:
    Camera(float fov = 45.f, float aspectRatio = 4.f/3.f, float nearField = 0.1f, float farField = 100.f,
           glm::vec3 camPos = glm::vec3(0.f,0.f,0.f), glm::vec3 camDir = glm::vec3(0.f, 0.f, 1.f));
    
    void setPosition(glm::vec3 position) {m_camPos = position;};
//    void setDirection(glm::vec3 direction) {m_camDir = direction;};
    
    void turnYaw(float yawIncrement)
    {
        m_yaw += yawIncrement;
        m_camRight = -glm::normalize(glm::cross(m_up, m_camDir));
    };
    void turnPitch(float pitchIncrement)
    {
        m_pitch += pitchIncrement;
        if(m_pitch > 89) m_pitch = 89;
        if(m_pitch < -89) m_pitch = -89;
    };
    
    void moveLeft(float posIncrement) {m_camPos -= m_camRight*posIncrement;};
    void moveRight(float posIncrement) {m_camPos += m_camRight*posIncrement;};
    void moveUp(float posIncrement) {m_camPos.y += posIncrement;};
    void moveDown(float posIncrement) {m_camPos.y -= posIncrement;};
    void moveForward(float posIncrement) {m_camPos += posIncrement*m_camDir;};
    void moveBackward(float posIncrement) {m_camPos -= posIncrement*m_camDir;};
    
    void lookAt(glm::vec3 target) {m_camDir = target - m_camPos;};
    
    
    glm::mat4 getViewMatrix();
    glm::mat4 getProjMatrix();
    
};




#endif /* Camera_h */
