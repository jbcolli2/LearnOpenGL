//
//  Camera.cpp
//  LearnOpenGL
//
//  Created by Jeb Collins on 8/13/21.
//



#include "Camera.hpp"




Camera::Camera(float fov, float aspectRatio, float nearField, float farField, glm::vec3 camPos, float pitch, float yaw) :
    m_camPos(camPos), m_fov(fov), m_aspectRatio(aspectRatio), m_nearField(nearField), m_farField(farField)
{
    m_camRight = glm::vec3(1.f, 0.f, 0.f);
    setDirection(pitch, yaw);
    
}


glm::mat4 Camera::getViewMatrix()
{
    setDirection(m_pitch, m_yaw);
    
    return glm::lookAt(m_camPos, m_camPos + m_camDir, m_up);
}



glm::mat4 Camera::getProjMatrix()
{
    return glm::perspective(glm::radians(m_fov), m_aspectRatio, m_nearField, m_farField);
}
