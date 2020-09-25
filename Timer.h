#pragma once

#include <algorithm>
#include <chrono>

#include <Windows.h>

class Timer
{
public:
    Timer() = default;

    void StartTimer() 
    {
        m_startRealTime = ChronoClockNow();
        m_startCpuTime = ThreadCPUUsage();
    }

    void StopTimer() 
    {
        m_realTimeUsed += ChronoClockNow() - m_startRealTime;
        // Floating point error can result in the subtraction producing a negative
        // time. Guard against that.
        m_cpuTimeUsed += std::max<double>(ThreadCPUUsage() - m_startCpuTime, 0);
    }

    double RealTimeUsed() const
    {
        return m_realTimeUsed;
    }

    double CpuTimeUsed() const
    {
        return m_cpuTimeUsed;
    }

private:
    double MakeTime(FILETIME const& kernelTime, FILETIME const& userTime)
    {
        ULARGE_INTEGER kernel;
        ULARGE_INTEGER user;
        kernel.HighPart = kernelTime.dwHighDateTime;
        kernel.LowPart = kernelTime.dwLowDateTime;
        user.HighPart = userTime.dwHighDateTime;
        user.LowPart = userTime.dwLowDateTime;
        return (static_cast<double>(kernel.QuadPart) + static_cast<double>(user.QuadPart)) * 100;
    }

    double ThreadCPUUsage() 
    {
        HANDLE thisThread = GetCurrentThread();
        FILETIME creationTime;
        FILETIME exitTime;
        FILETIME kernelTime;
        FILETIME userTime;
        GetThreadTimes(thisThread, &creationTime, &exitTime, &kernelTime, &userTime);
        return MakeTime(kernelTime, userTime);
    }

    double ChronoClockNow() 
    {
        using FpSeconds = std::chrono::duration<double, std::chrono::nanoseconds::period>;
        return FpSeconds(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
    }

private:
    double m_startRealTime = 0;
    double m_startCpuTime = 0;

    double m_realTimeUsed = 0;
    double m_cpuTimeUsed = 0;
};
