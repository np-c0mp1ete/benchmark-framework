#include "Timer.h"

#include <iostream>

namespace
{
    template<typename TestFunc>
    void RunTest(const char* testName, TestFunc func)
    {
        Timer timer;
        timer.StartTimer();
        const long testsCount = 10000000;
        for (long i = 0; i < testsCount; ++i)
        {
            func();
        }
        timer.StopTimer();

        std::cout << testName << ": " << std::round(timer.CpuTimeUsed() / testsCount) << "ns per string\n";
    }

    template<typename T>
    void DontOptimize(T const* ptr)
    {
        volatile T c = *ptr;
        (void)c;
    }
}

int main()
{
    RunTest("Test", []()
    {
        DontOptimize("There are only two kinds of languages: the ones people complain about and the ones nobody uses."));
    });

    return 0;
}
