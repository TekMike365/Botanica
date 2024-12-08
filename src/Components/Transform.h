#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Transform
{
public:
    Transform()
        : m_Position(0.0f), m_Rotation(glm::vec3(0.0f)), m_Scale(1.0f)
    {
        UpdateMatrix();
    }
    ~Transform() = default;

    inline void Translate(glm::vec3 position)
    {
        m_Position += position;
        m_Matrix = glm::translate(m_Matrix, position);
    }

    inline void Scale(glm::vec3 scale)
    {
        m_Scale += scale;
        m_Matrix = glm::scale(m_Matrix, scale);
    }

    inline void Rotate(glm::quat rotation)
    {
        m_Rotation = rotation * m_Rotation;
        UpdateMatrix();
    }

    inline void Rotate(glm::vec3 euler)
    {
        Rotate(glm::quat(euler));
    }

    inline const glm::vec3 &GetPosition() const { return m_Position; }
    inline void SetPosition(glm::vec3 position)
    {
        m_Position = position;
        UpdateMatrix();
    }

    inline const glm::vec3 &GetScale() const { return m_Scale; }
    inline void SetScale(glm::vec3 scale)
    {
        m_Scale = scale;
        UpdateMatrix();
    }

    inline const glm::quat &GetRotation() const { return m_Rotation; }
    inline glm::vec3 GetEulerRotation() const { return glm::eulerAngles(m_Rotation); }
    inline void SetRotation(glm::quat rotation)
    {
        m_Rotation = rotation;
        UpdateMatrix();
    }
    inline void SetRotation(glm::vec3 euler)
    {
        SetRotation(glm::quat(euler));
    }

    inline glm::vec3 GetForwardVector() const { return m_Rotation * glm::vec3(0.0f, 0.0f, -1.0f); }
    inline glm::vec3 GetUpVector() const { return m_Rotation * glm::vec3(0.0f, 1.0f, 0.0f); }
    inline glm::vec3 GetRightVector() const { return m_Rotation * glm::vec3(1.0f, 0.0f, 0.0f); }

    inline const glm::mat4 &GetMatrtix() const { return m_Matrix; }
    inline glm::mat4 GetInverseMatrix() const { return glm::inverse(m_Matrix); }

private:
    inline void UpdateMatrix()
    {
        m_Matrix = glm::translate(glm::mat4(1.0f), m_Position) *
                    glm::mat4(m_Rotation) *
                    glm::scale(glm::mat4(1.0f), m_Scale);
    }

private:
    glm::mat4 m_Matrix;

    glm::vec3 m_Position;
    glm::vec3 m_Scale;
    glm::quat m_Rotation;
};