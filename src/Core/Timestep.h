#pragma once

namespace Botanica
{
    class Timestep
    {
    public:
        Timestep(double time = 0.0f)
            : m_TimeSec(time)
        {
        }

        inline double GetSeconds() const { return m_TimeSec; }
        inline double GetMiliseconds() const { return m_TimeSec / 1000.0f; }

    private:
        double m_TimeSec;
    };
}
