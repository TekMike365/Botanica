#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Botanica
{
    class Transform
    {
    public:
        Transform()
            : m_Position(0.0f), m_Rotation(0.0f), m_Scale(1.0f)
        {
            UpdateMatrix();
        }
        ~Transform() {}

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

        inline void Rotate(glm::vec3 rotation)
        {
            m_Rotation += rotation;
            m_Matrix = glm::rotate(glm::mat4(1.0f), rotation.y, glm::vec3(0, 1, 0)) *
                             glm::rotate(glm::mat4(1.0f), rotation.x, glm::vec3(1, 0, 0)) *
                             glm::rotate(glm::mat4(1.0f), rotation.z, glm::vec3(0, 0, 1)) *
                             m_Matrix;
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

        inline const glm::vec3 &GetRotation() const { return m_Rotation; }
        inline void SetRotation(glm::vec3 rotation)
        {
            m_Rotation = rotation;
            UpdateMatrix();
        }

        inline const glm::mat4 &GetMatrtix() const { return m_Matrix; }
        inline glm::mat4 GetInverseMatrix() const { return glm::inverse(m_Matrix); }

    private:
        inline void UpdateMatrix()
        {
            m_Matrix = glm::translate(glm::mat4(1.0f), m_Position) *
                             glm::rotate(glm::mat4(1.0f), m_Rotation.y, glm::vec3(0, 1, 0)) *
                             glm::rotate(glm::mat4(1.0f), m_Rotation.x, glm::vec3(1, 0, 0)) *
                             glm::rotate(glm::mat4(1.0f), m_Rotation.z, glm::vec3(0, 0, 1)) *
                             glm::scale(glm::mat4(1.0f), m_Scale);
        }

    private:
        glm::mat4 m_Matrix;

        glm::vec3 m_Position;
        glm::vec3 m_Scale;
        glm::vec3 m_Rotation;
    };
}
