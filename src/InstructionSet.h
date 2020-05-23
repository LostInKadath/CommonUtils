#pragma once
// Compile by using: cl /EHsc /W4 InstructionSet.cpp
// processor: x86, x64
// Uses the __cpuid intrinsic to get information about CPU extended instruction set support.

class InstructionSet
{
public:
    static std::string Vendor(void);
    static std::string Brand(void);

    static bool SSE3(void);
    static bool PCLMULQDQ(void);
    static bool MONITOR(void);
    static bool SSSE3(void);
    static bool FMA(void);
    static bool CMPXCHG16B(void);
    static bool SSE41(void);
    static bool SSE42(void);
    static bool MOVBE(void);
    static bool POPCNT(void);
    static bool AES(void);
    static bool XSAVE(void);
    static bool OSXSAVE(void);
    static bool AVX(void);
    static bool F16C(void);
    static bool RDRAND(void);

    static bool MSR(void);
    static bool CX8(void);
    static bool SEP(void);
    static bool CMOV(void);
    static bool CLFSH(void);
    static bool MMX(void);
    static bool FXSR(void);
    static bool SSE(void);
    static bool SSE2(void);

    static bool FSGSBASE(void);
    static bool BMI1(void);
    static bool HLE(void);
    static bool AVX2(void);
    static bool BMI2(void);
    static bool ERMS(void);
    static bool INVPCID(void);
    static bool RTM(void);
    static bool AVX512F(void);
    static bool RDSEED(void);
    static bool ADX(void);
    static bool AVX512PF(void);
    static bool AVX512ER(void);
    static bool AVX512CD(void);
    static bool SHA(void);

    static bool PREFETCHWT1(void);

    static bool LAHF(void);
    static bool LZCNT(void);
    static bool ABM(void);
    static bool SSE4a(void);
    static bool XOP(void);
    static bool TBM(void);

    static bool SYSCALL(void);
    static bool MMXEXT(void);
    static bool RDTSCP(void);
    static bool _3DNOWEXT(void);
    static bool _3DNOW(void);

private:
    struct InstructionSet_Internal
    {
        InstructionSet_Internal();

        int nIds_;
        int nExIds_;
        std::string vendor_;
        std::string brand_;
        bool isIntel_;
        bool isAMD_;
        std::bitset<32> f_1_ECX_;
        std::bitset<32> f_1_EDX_;
        std::bitset<32> f_7_EBX_;
        std::bitset<32> f_7_ECX_;
        std::bitset<32> f_81_ECX_;
        std::bitset<32> f_81_EDX_;
        std::vector<std::array<int, 4>> data_;
        std::vector<std::array<int, 4>> extdata_;
    };

    static const InstructionSet_Internal CPU_Rep;
};
