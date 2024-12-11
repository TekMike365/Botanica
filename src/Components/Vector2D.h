#pragma once

#include <cstdint>
#include <vector>
#include <glm/glm.hpp>

template <typename T>
class Vector2D
{
public:
    Vector2D() = default;
    Vector2D(glm::uvec3 size, const T &allocator)
        : m_Size(size), m_Data(GetSize(), allocator) {}
    ~Vector2D() = default;

    inline uint32_t GetSize() const { return m_Size.x * m_Size.y * m_Size.z; }
    inline std::vector<T> &GetData() const { return m_Data; }

    inline T &operator[](glm::uvec3 pos) { return m_Data[pos.x + pos.y * m_Size.x + pos.z * m_Size.x * m_Size.y]; }
    inline const T &operator[](glm::uvec3 pos) const { return m_Data[pos.x + pos.y * m_Size.x + pos.z * m_Size.x * m_Size.y]; }

private:
    glm::uvec3 m_Size;
    std::vector<T> m_Data;
};