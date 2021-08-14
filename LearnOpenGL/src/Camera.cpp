//
//  Camera.cpp
//  LearnOpenGL
//
//  Created by Jeb Collins on 8/13/21.
//

#include <stdio.h>

#include "Camera.hpp"




Camera::Camera(glm::vec3 camPos, glm::vec3 camDir) : m_camPos(camPos), m_camDir(camDir)
{
    m_pitch = glm::acos(m_camDir.y)-M_PI_2;
    if(m_camDir.x == 0)
    {
        if(m_camDir.z > 0)
        {
            m_yaw = 0.f;
        }
        else
        {
            m_yaw = M_PI;
        }
    }
    else
    {
        m_yaw = glm::atan(m_camDir.z/m_camDir.x);
    }
    
}


glm::mat4 Camera::getViewMatrix()
{
    float radius = m_camPos.length();
    m_camDir.x = glm::sin(m_yaw)*glm::sin(-m_pitch+M_PI_2);
    m_camDir.y = glm::cos(-m_pitch+M_PI_2);
    m_camDir.z = -glm::cos(m_yaw)*glm::sin(-m_pitch+M_PI_2);
//    m_camDir = glm::normalize(m_camDir);
    
    return glm::lookAt(m_camPos, m_camPos + radius*m_camDir, m_up);
}
