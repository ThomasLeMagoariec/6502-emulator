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

    void writeWord(word value, u32 addr, u32& cycles)
    {
        data[addr] = value & 0xFF;
        data[addr + 1] = (value >> 8);
        
        cycles -= 2;
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

    byte fetchWord(u32& cycles, Mem& memory)
    {

        // ! ONLY WORKS ON LITTLE ENDIANT ARCHS
        //   (for now)
        word data = memory[PC];
        PC++;

        data |= (memory[PC] << 8);
        PC++;

        cycles -= 2;

        return data;

    }

    byte readByte(u32& cycles, byte addr, Mem& memory)
    {
        byte data = memory[addr];
        cycles--;
        return data;
    }

    static constexpr byte
        INS_LDA_IM = 0xA9,  // ? load accumulator immediate mode
        INS_LDA_ZP = 0xA5,  // ? load accumulator zero page
        INS_LDA_ZPX = 0xB5, // ? load accumulator zero page + X
        INS_JSR = 0x20; // ? jump to subroutine

    void LDASetStatus()
    {
        Z = (A == 0);
        N = (A & 0b100000000) > 0;
    }

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
                    LDASetStatus();

                    break;
                }
                case INS_LDA_ZP:
                {
                    byte zp_addr = fetchByte(cycles, memory);
                    A = readByte(cycles, zp_addr, memory);

                    LDASetStatus();

                    break;
                }
                case INS_LDA_ZPX:
                {
                    byte zp_addr = fetchByte(cycles, memory);
                    zp_addr += X;
                    cycles--;
                    A = readByte(cycles, zp_addr, memory);

                    LDASetStatus();
                    break;
                }
                case INS_JSR:
                {
                    word subAddr = fetchWord(cycles, memory);
                    memory.writeWord(PC - 1, SP, cycles);
                    PC = subAddr;
                    cycles--;
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
    mem[0xFFFC] = CPU::INS_JSR;
    mem[0xFFFD] = 0x42;
    mem[0xFFFE] = 0x42;
    mem[0x4242] = CPU::INS_LDA_IM;
    mem[0x4243] = 0x84;

    cpu.execute(9, mem);

    printf("A = %d\n", cpu.A);
    printf("X = %d\n", cpu.X);
    printf("Y = %d\n", cpu.Y);

    return 0;
}