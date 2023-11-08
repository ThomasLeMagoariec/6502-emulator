#include <stdio.h>
#include <stdlib.h>


struct CPU
{
    using byte = unsigned char;
    using word = unsigned short;

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
};

int main()
{
    return 0;
}