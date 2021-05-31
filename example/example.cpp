#include "6502.hpp"
#include "Bus.hpp"
#include <iostream>

using namespace std;

// giving the read and write function to CPU
Bus bus;

u8 Read(u16 addres)
{
    return bus.read(addres);
};

void Write(u16 addres, u8 value)
{
    return bus.write(addres, value);
};

int main(void)
{
    bus.RAM[0xFFFC] = 0x00;
    bus.RAM[0xFFFD] = 0x80;
    bus.RAM[0x8000] = 0xA9;
    bus.RAM[0x8001] = 50;

    MOS6502 CPU = MOS6502();

    CPU.Execute();

    printf("Program Counter, Stack Pointer, Instruction Register, Cycles - \n");
    printf("\tStack Pointer is holding the address 0x%X\n", CPU.SP);
    printf("\tProgram Counter is holding the address 0x%X\n", CPU.PC);
    printf("\tInstruction Register is holding the Operation Code 0x%X\n", CPU.IR);
    printf("\tNumber of Cycles Taken By the instructions are %d\n", CPU.Cycles);

    printf("Resgisters - \n");
    printf("\tThe value hold By Accumulator is %d\n", CPU.A);
    printf("\tThe value hold By Index X is %d\n", CPU.X);
    printf("\tThe value hold By Index Y is %d\n", CPU.Y);

    printf("Status Flags - \n");
    printf("\tThe Carry flag is set to %d\n", CPU.getFlag(CPU.C));
    printf("\tThe Zero flag is set to %d\n", CPU.getFlag(CPU.Z));
    printf("\tThe Overflow flag is set to %d\n", CPU.getFlag(CPU.V));
    printf("\tThe Negative flag is set to %d\n", CPU.getFlag(CPU.N));
    printf("\tThe Unused flag is set to %d\n", CPU.getFlag(CPU.U));
    printf("\tInterrupt Disable is set to %d\n", CPU.getFlag(CPU.I));
    printf("\tDecimal Mode is set to %d\n", CPU.getFlag(CPU.D));
    printf("\tBreak Command is set to %d\n", CPU.getFlag(CPU.B));

    return 0;
};