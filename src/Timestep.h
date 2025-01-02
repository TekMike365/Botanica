#pragma once

class Timestep
{
public:
    Timestep(double time = 0.0f)
        : m_TimeSec(time)
    {
    }

    inline float GetSeconds() const { return (float)m_TimeSec; }
    inline float GetMiliseconds() const { return (float)(m_TimeSec * 1000.0f); }

private:
    double m_TimeSec;
};
