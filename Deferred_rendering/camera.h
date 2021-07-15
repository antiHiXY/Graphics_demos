#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

// possible options for camera movement
enum class Camera_Movement
{
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.05f;
const float ZOOM = 45.0f;

class Camera
{
public:
    glm::vec3 m_Position;
    glm::vec3 m_Front;
    glm::vec3 m_Up;
    glm::vec3 m_Right;
    glm::vec3 m_WorldUp;
    // Euler angles
    float m_Yaw;
    float m_Pitch;
    // camera options
    float m_MovementSpeed;
    float m_MouseSensitivity;
    float m_Zoom;

    // constructor with vectors
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH) 
        : m_Position(position), m_Front(glm::vec3(0.0f, 0.0f, -1.0f)), m_WorldUp(up), m_Yaw(yaw)
        , m_Pitch(pitch), m_MovementSpeed(SPEED), m_MouseSensitivity(SENSITIVITY), m_Zoom(ZOOM)
    {
        UpdateCameraVectors();
    }
    // constructor with scalar values
    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch)
        : m_Position(glm::vec3(posX, posY, posZ)), m_Front(glm::vec3(0.0f, 0.0f, -1.0f)), m_WorldUp(glm::vec3(upX, upY, upZ))
        , m_Yaw(yaw), m_Pitch(pitch), m_MovementSpeed(SPEED), m_MouseSensitivity(SENSITIVITY), m_Zoom(ZOOM)
    {
        UpdateCameraVectors();
    }
    // view matrix calculated with Euler angles and lookAt matrix
    glm::mat4 GetViewMatrix()
    {
        return glm::lookAt(m_Position, m_Position + m_Front, m_Up);
    }
    // processes input received from input system
    void ProcessKeyboard(Camera_Movement direction, float deltaTime)
    {
        float velocity = m_MovementSpeed * deltaTime;
        if (direction == Camera_Movement::FORWARD)
            m_Position += m_Front * velocity;
        if (direction == Camera_Movement::BACKWARD)
            m_Position -= m_Front * velocity;
        if (direction == Camera_Movement::LEFT)
            m_Position -= m_Right * velocity;
        if (direction == Camera_Movement::RIGHT)
            m_Position += m_Right * velocity;
    }
    // processes input received from a mouse input system
    void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true)
    {
        xoffset *= m_MouseSensitivity;
        yoffset *= m_MouseSensitivity;

        m_Yaw += xoffset;
        m_Pitch += yoffset;

        if (constrainPitch)
        {
            if (m_Pitch > 89.0f)
                m_Pitch = 89.0f;
            if (m_Pitch < -89.0f)
                m_Pitch = -89.0f;
        }
        UpdateCameraVectors();
    }
    // processes input received from a mouse scroll-wheel
    void ProcessMouseScroll(float yoffset)
    {
        m_Zoom -= (float)yoffset;
        if (m_Zoom < 1.0f)
            m_Zoom = 1.0f;
        if (m_Zoom > 45.0f)
            m_Zoom = 45.0f;
    }
private:
    // calculates front vector from updated Camera's Euler angles
    void UpdateCameraVectors()
    {
        // new Front vector
        glm::vec3 front;
        front.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
        front.y = sin(glm::radians(m_Pitch));
        front.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
        m_Front = glm::normalize(front);
        // re-calculate Right and Up vector
        m_Right = glm::normalize(glm::cross(m_Front, m_WorldUp));
        m_Up = glm::normalize(glm::cross(m_Right, m_Front));
    }
};
