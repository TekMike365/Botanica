#pragma once

#include <cstdint>
#include <vector>
#include <glm/glm.hpp>

template <typename T>
class Vector3D
{
public:
    Vector3D() = default;
    Vector3D(glm::uvec3 size, const T &allocator)
        : m_Size(size), m_Data(GetElementCount(), allocator) {}
    ~Vector3D() = default;

    inline glm::uvec3 GetSize() const { return m_Size; }
    inline uint32_t GetElementCount() const { return m_Size.x * m_Size.y * m_Size.z; }
    inline std::vector<T> &GetData() { return m_Data; }

    inline uint32_t GetIndex(glm::uvec3 pos) const { return pos.x + m_Size.x * (pos.y + pos.z * m_Size.y); }

    inline T &operator[](glm::uvec3 pos)
    {
        DataChanged = true;
        return m_Data[GetIndex(pos)];
    }
    inline const T &operator[](glm::uvec3 pos) const { return m_Data[GetIndex(pos)]; }

public:
    bool DataChanged = false;

private:
    glm::uvec3 m_Size;
    std::vector<T> m_Data;
};