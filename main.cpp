#include <stdio.h>
#include <stdlib.h>

using byte = unsigned char;
using word = unsigned short;

using u32 = unsigned int;

struct Mem
{
    static constexpr u32 MAX_MEM = 1024 * 64;
    byte data[MAX_MEM];

    void init()
    {
        for (u32 i = 0; i < MAX_MEM; i++){
            data[i] = 0;
        }
    }

    // read byte
    byte operator[](u32 addr) const
    {
        return data[addr];
    }

    // write byte
    byte& operator[](u32 addr)
    {
        return data[addr];
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

    byte fetchByte(u32& cycles, Mem& memory)
    {
        byte data = memory[PC];
        PC++;
        cycles--;

        return data;
    }

    static constexpr byte
        INS_LDA_IM = 0xA9; // ? load Accumulator Immediate mode

    void execute(u32 cycles, Mem& memory)
    {
        while (cycles > 0)
        {
            byte ins = fetchByte(cycles, memory);

            switch (ins)
            {
                case INS_LDA_IM:
                {
                    byte value = fetchByte(cycles, memory);
                    
                    A = value;
                    Z = (A == 0);
                    N = (A & 0b100000000) > 0;

                    break;
                }
                default:
                {
                    printf("instruction not handled '%d'", ins);
                    break;
                }
            }
        }
    }

};

int main()
{
    Mem mem;
    CPU cpu;
    cpu.reset(mem);
    mem[0xFFFC] = CPU::INS_LDA_IM;
    mem[0xFFFD] = 0x69;
    cpu.execute(2, mem);
    return 0;
}