// AntiRE.h --- Win32 anti-reverse engineering techniques

#pragma once

#include <windows.h>
#include <intrin.h> // For __cpuid

////////////////////////////////////////////////////////////////////////////////

#if 0
    #define ANTIRE_DEBUG(status) MessageBoxA(NULL, status, "AntiRE", MB_ICONERROR)
#else
    #define ANTIRE_DEBUG(status)
#endif

// Detect debugger
inline void AntiRE_DetectDebugger(void)
{
    if (IsDebuggerPresent())
    {
        ANTIRE_DEBUG("Debugger detected using IsDebuggerPresent");
        ExitProcess(0);
    }
}

// Check debug registers
inline void AntiRE_CheckDebugRegs(void)
{
    CONTEXT context;
    ZeroMemory(&context, sizeof(context));
    context.ContextFlags = CONTEXT_DEBUG_REGISTERS;
    if (!GetThreadContext(GetCurrentThread(), &context))
        return;

#if defined(_M_AMD64) || defined(__x86_64__) // For AMD64
    if (context.Dr0 || context.Dr1 || context.Dr2 || context.Dr3)
    {
        ANTIRE_DEBUG("Debugger detected using debug registers (x64)");
        ExitProcess(0);
    }
#elif defined(_M_IX86) || defined(__i386__) // For x86
    if (context.Dr0 || context.Dr1 || context.Dr2 || context.Dr3)
    {
        ANTIRE_DEBUG("Debugger detected using debug registers (x86)");
        ExitProcess(0);
    }
#elif defined(_M_ARM64) || defined(__arm64__) || defined(__aarch64__) // For ARM64
    if (context.Bvr[0] || context.Bvr[1] || context.Bvr[2] || context.Bvr[3])
    {
        ANTIRE_DEBUG("Debugger detected using debug registers (ARM64)");
        ExitProcess(0);
    }
#elif defined(_M_ARM) || defined(__arm__) // For ARM
    if (context.Bvr[0] || context.Bvr[1] || context.Bvr[2] || context.Bvr[3])
    {
        ANTIRE_DEBUG("Debugger detected using debug registers (ARM)");
        ExitProcess(0);
    }
#else
#error Your processor is not supported yet.
#endif
}

// Detect virtual machine (WARNING: License compliant?)
inline void AntiRE_DetectVirtualMachine(void)
{
#if defined(_M_AMD64) || defined(__x86_64__) || defined(_M_IX86) || defined(__i386__)
    // For x86/x64
    int cpuInfo[4] = {0};

    // CPUID instruction for more information
    __cpuid(cpuInfo, 0x80000008);
    if (cpuInfo[2] & 0x10000) { // Virtualization indicator
        ANTIRE_DEBUG("Virtual machine detected using CPUID");
        ExitProcess(0);
    }
#elif defined(_M_ARM64) || defined(__arm64__) || defined(__aarch64__) || defined(_M_ARM) || defined(__arm__)
    // For ARM/ARM64
    char brand[49];
    __asm__ __volatile__ (
        "mrs x0, midr_el1\n\t"
        "str x0, %0"
        : "=m" (brand)
        :
        : "x0"
    );
    if (strstr(brand, "QEMU") != NULL)  // Check QEMU emulation
    {
        ANTIRE_DEBUG("Virtual machine detected (QEMU)");
        ExitProcess(0);
    }
#endif
}

// Detect time attack (WARNING: License compliant?)
inline void AntiRE_DetectTimeAttack(void)
{
    LARGE_INTEGER freq, start, end;
    QueryPerformanceFrequency(&freq);

    const int min_count = 50;
    const int max_count = 150;
    int count = rand() % (max_count - min_count + 1) + min_count;

    double elapsedTotal = 0;
    for (int i = 0; i < count; ++i)
    {
        int wait = rand() % 10 + 5; // 5ms ~ 14ms
        QueryPerformanceCounter(&start);
        Sleep(wait);
        QueryPerformanceCounter(&end);
        elapsedTotal += (end.QuadPart - start.QuadPart) * 1000.0 / freq.QuadPart;
    }

    double average = (double)elapsedTotal / count;
    // Set threshold based on experimentation and profiling
    const double threshold = 100;
    if (average > threshold)
    {
        ANTIRE_DEBUG("Time attack detected");
        ExitProcess(0);
    }
}

////////////////////////////////////////////////////////////////////////////////
