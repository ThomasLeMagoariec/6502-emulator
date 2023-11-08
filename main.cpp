#include <stdio.h>
#include <stdlib.h>

using byte = unsigned char;
using word = unsigned short;

using u32 = unsigned int;

struct Mem
{
    static constexpr u32 MAX_MEM = 1024 * 64;
    byte Data[MAX_MEM];

    void init()
    {
        for (u32 i = 0; i < MAX_MEM; i++){
            Data[i] = 0;
        }
    }
};

struct CPU
{
    
    word PC; // program counter
    word SP; // stack pointer

    byte A, X, Y; // registers

    byte C : 1; // status flags
    byte Z : 1; 
    byte I : 1;
    byte D : 1;
    byte B : 1;
    byte V : 1;
    byte N : 1;

    void reset(Mem& memory)
    {
        PC = 0xFFFC;
        SP = 0x0100;
        C = Z = I = D = B = V = N = 0;
        A = X = Y = 0;

        memory.init();
    }

};

int main()
{
    Mem mem;
    CPU cpu;
    cpu.reset(mem);
    return 0;
}