#pragma once
#include "util.hpp"
#include <vector>
#include <string>

// The externly Supplied read and write Function
extern u8 Read(u16 addres);
extern void Write(u16 addres, u8 value);

// 6502 Class
class MOS6502
{
public:
	// Program Counter - The program counter is a 16 bit register which points to the next instruction to be executed.
	u16 PC;

	// Stack Pointer - The stack pointer is an 8 bit register and holds the low 8 bits of the next free location on the stack.
	u16 SP;

	// Accumulator, Index X, Index Y, Instruction Register, Cycles
	u8 A, X, Y, IR, Cycles;

	// status Register - The 8-bit status Register contains status flags.
	u8 status;

	// The CPU (RESB), input is used to initialize the microprocessor and start program execution.
	MOS6502();

	// Interrupt Request (IRQB), The Interrupt Request (IRQB) input signal is used to request that an interrupt sequence be initiated.
	void IRQB();

	// Non-Maskable Interrupt (NMIB), A negative transition on the Non-Maskable Interrupt (NMIB) input initiates an interrupt sequence after the current instruction is completed.
	void NMIB();

	// Flags in status register
	enum Flags
	{
		// Carry Flag - The carry flag is set if the last operation caused an overflow from bit 7 of the result or an underflow from bit 0.
		C = (1 << 0),

		// Zero Flag - The zero flag is set if the result of the last operation as was zero.
		Z = (1 << 1),

		// Interrupt Disable - The interrupt disable flag is set if the program has executed a 'Set Interrupt Disable' (SEI) instruction.
		I = (1 << 2),

		// Decimal Mode - While the decimal mode flag is set the processor will obey the rules of Binary Coded Decimal (BCD) arithmetic during addition and subtraction.
		D = (1 << 3),

		// Break Command - While the decimal mode flag is set the processor will obey the rules of Binary Coded Decimal (BCD) arithmetic during addition and subtraction.
		B = (1 << 4),

		// Unused
		U = (1 << 5),

		// Overflow Flag - The overflow flag is set during arithmetic operations if the result has yielded an invalid 2's complement result
		V = (1 << 6),

		// Negative Flag - The negative flag is set if the result of the last operation had bit 7 set to a one.
		N = (1 << 7)
	};

	/* Addressing Modes */

	// Implied - Implied addressing uses a single byte instruction.
	u8 IMP();

	// Immediate - Immediate addressing allows the programmer to directly specify an 8 bit constant within the instruction.
	u8 IMM();

	// Zero Page - An instruction using zero page addressing mode has only an 8 bit address operand.
	u8 ZP0();

	// Zero Page X - The address to be accessed by an instruction using indexed zero page addressing is calculated by taking the 8 bit zero page address from the instruction and adding the current value of the X register to it.
	u8 ZPX();

	// Zero Page Y - The address to be accessed by an instruction using indexed zero page addressing is calculated by taking the 8 bit zero page address from the instruction and adding the current value of the Y register to it.
	u8 ZPY();

	// Relative - Relative addressing mode is used by branch instructions (e.g. BEQ, BNE, etc.) which contain a signed 8 bit relative offset (e.g. -128 to +127) which is added to program counter if the condition is true.
	u8 REL();

	// Absolute - Instructions using absolute addressing contain a full 16 bit address to identify the target location.
	u8 ABS();

	// Absolute X - The address to be accessed by an instruction using X register indexed absolute addressing is computed by taking the 16 bit address from the instruction and added the contents of the X register.
	u8 ABX();

	// Absolute Y - The address to be accessed by an instruction using X register indexed absolute addressing is computed by taking the 16 bit address from the instruction and added the contents of the Y register.
	u8 ABY();

	// Indirect - JMP is the only 6502 instruction to support indirection. The instruction contains a 16 bit address which identifies the location of the least significant byte of another 16 bit memory address which is the real target of the instruction.
	u8 IND();

	// Indexed Indirect - Indexed indirect addressing is normally used in conjunction with a table of address held on zero page.
	u8 IZX();

	// Indirect Indexed - Indirect indirect addressing is the most common indirection mode used on the 6502. In instruction contains the zero page location of the least significant byte of 16 bit address. The Y register is dynamically added to this value to generated the actual target address for operation.
	u8 IZY();

	/* The Instruction Set */

	// ADC - Add with Carry, This instruction adds the contents of a memory location to the accumulator together with the carry bit. If overflow occurs the carry bit is set.
	u8 ADC();

	// AND - Logical AND, A logical AND is performed, bit by bit, on the accumulator contents using the contents of a byte of memory.
	u8 AND();

	// ASL - Arithmetic Shift Left, This operation shifts all the bits of the accumulator or memory contents one bit left. Bit 0 is set to 0 and bit 7 is placed in the carry flag. The effect of this operation is to multiply the memory contents by 2 (ignoring 2's complement considerations), setting the carry if the result will not fit in 8 bits.
	u8 ASL();

	// BCC - Branch if Carry Clear, If the carry flag is clear then add the relative displacement to the program counter to cause a branch to a new location.
	u8 BCC();

	// BCS - Branch if Carry Set, If the carry flag is set then add the relative displacement to the program counter to cause a branch to a new location.
	u8 BCS();

	// BEQ - Branch if Equal, If the zero flag is set then add the relative displacement to the program counter to cause a branch to a new location.
	u8 BEQ();

	// BIT - Bit Test, This instructions is used to test if one or more bits are set in a target memory location. The mask pattern in A is ANDed with the value in memory to set or clear the zero flag, but the result is not kept. Bits 7 and 6 of the value from memory are copied into the N and V flags.
	u8 BIT();

	// BMI - Branch if Minus, If the negative flag is set then add the relative displacement to the program counter to cause a branch to a new location.
	u8 BMI();

	// BNE - Branch if Not Equal, If the zero flag is clear then add the relative displacement to the program counter to cause a branch to a new location.
	u8 BNE();

	// BNE - Branch if Not Equal, If the negative flag is clear then add the relative displacement to the program counter to cause a branch to a new location.
	u8 BPL();

	// BRK - Force Interrupt, The BRK instruction forces the generation of an interrupt request. The program counter and processor status are pushed on the stack then the IRQ interrupt vector at $FFFE/F is loaded into the PC and the break flag in the status set to one.
	u8 BRK();

	// BVC - Branch if Overflow Clear, If the overflow flag is clear then add the relative displacement to the program counter to cause a branch to a new location.
	u8 BVC();

	// BVS - Branch if Overflow Set, If the overflow flag is set then add the relative displacement to the program counter to cause a branch to a new location.
	u8 BVS();

	// CLC - Clear Carry Flag, Set the carry flag to zero.
	u8 CLC();

	// CLD - Clear Decimal Mode, Sets the decimal mode flag to zero.
	u8 CLD();

	// CLI - Clear Interrupt Disable, Sets the Interrupt Disable flag to zero.
	u8 CLI();

	// CLV - Clear Overflow Flag, Sets the Overflow flag to zero.
	u8 CLV();

	// CMP - Compare, This instruction compares the contents of the accumulator with another memory held value and sets the zero and carry flags as appropriate.
	u8 CMP();

	// CPX - Compare X Register, This instruction compares the contents of the X register with another memory held value and sets the zero and carry flags as appropriate.
	u8 CPX();

	// CPY - Compare Y Register, This instruction compares the contents of the Y register with another memory held value and sets the zero and carry flags as appropriate.
	u8 CPY();

	// DEC - Decrement Memory, Subtracts one from the value held at a specified memory location setting the zero and negative flags as appropriate.
	u8 DEC();

	// DEX - Decrement X Register, Subtracts one from the X register setting the zero and negative flags as appropriate.
	u8 DEX();

	// DEY - Decrement Y Register, Subtracts one from the Y register setting the zero and negative flags as appropriate.
	u8 DEY();

	// EOR - Exclusive OR, An exclusive OR is performed, bit by bit, on the accumulator contents using the contents of a byte of memory.
	u8 EOR();

	// INC - Increment Memory, Adds one to the value held at a specified memory location setting the zero and negative flags as appropriate.
	u8 INC();

	// INX - Increment X Register, Adds one to the X register setting the zero and negative flags as appropriate.
	u8 INX();

	// INY - Increment Y Register, Adds one to the Y register setting the zero and negative flags as appropriate.
	u8 INY();

	// JMP - Jump, Sets the program counter to the address specified by the operand.
	u8 JMP();

	// JSR - Jump to Subroutine, The JSR instruction pushes the address (minus one) of the return point on to the stack and then sets the program counter to the target memory address.
	u8 JSR();

	// LDA - Load Accumulator, Loads a byte of memory into the accumulator setting the zero and negative flags as appropriate.
	u8 LDA();

	// LDX - Load X Register, Loads a byte of memory into the X register setting the zero and negative flags as appropriate.
	u8 LDX();

	// LDY - Load Y Register, Loads a byte of memory into the X register setting the zero and negative flags as appropriate.
	u8 LDY();

	// LSR - Logical Shift Right, Each of the bits in A or M is shift one place to the right. The bit that was in bit 0 is shifted into the carry flag. Bit 7 is set to zero.
	u8 LSR();

	// NOP - No Operation, The NOP instruction causes no changes to the processor other than the normal incrementing of the program counter to the next instruction.
	u8 NOP();

	// ORA - Logical Inclusive OR, An inclusive OR is performed, bit by bit, on the accumulator contents using the contents of a byte of memory.
	u8 ORA();

	// PHA - Push Accumulator, Pushes a copy of the accumulator on to the stack.
	u8 PHA();

	// PHP - Push Processor status, Pushes a copy of the status flags on to the stack.
	u8 PHP();

	// PLA - Pull Accumulator, Pulls an 8 bit value from the stack and into the accumulator. The zero and negative flags are set as appropriate.
	u8 PLA();

	// PLP - Pull Processor status, Pulls an 8 bit value from the stack and into the processor flags. The flags will take on new states as determined by the value pulled.
	u8 PLP();

	// ROL - Rotate Left, Move each of the bits in either A or M one place to the left. Bit 0 is filled with the current value of the carry flag whilst the old bit 7 becomes the new carry flag value.
	u8 ROL();

	// ROR - Rotate Right, Move each of the bits in either A or M one place to the right. Bit 7 is filled with the current value of the carry flag whilst the old bit 0 becomes the new carry flag value.
	u8 ROR();

	// RTI - Return from Interrupt, The RTI instruction is used at the end of an interrupt processing routine. It pulls the processor flags from the stack followed by the program counter.
	u8 RTI();

	// RTS - Return from Subroutine, The RTS instruction is used at the end of a subroutine to return to the calling routine. It pulls the program counter (minus one) from the stack.
	u8 RTS();

	// SBC - Subtract with Carry, This instruction subtracts the contents of a memory location to the accumulator together with the not of the carry bit. If overflow occurs the carry bit is clear, this enables multiple byte subtraction to be performed.
	u8 SBC();

	// SEC - Set Carry Flag, Set the carry flag to one.
	u8 SEC();

	// SED - Set Decimal Flag, Set the decimal to one.
	u8 SED();

	// SEI - Set Interrupt Disable, Set the nterrupt Disable to one.
	u8 SEI();

	// STA - Store Accumulator, Stores the contents of the accumulator into memory.
	u8 STA();

	// STX - Store X Register, Stores the contents of the X register into memory.
	u8 STX();

	// STY - Store Y Register, Stores the contents of the Y register into memory.
	u8 STY();

	// TAX - Transfer Accumulator to X, Copies the current contents of the accumulator into the X register and sets the zero and negative flags as appropriate.
	u8 TAX();

	// TAY - Transfer Accumulator to Y, Copies the current contents of the accumulator into the Y register and sets the zero and negative flags as appropriate.
	u8 TAY();

	// TSX - Transfer Stack Pointer to X, Copies the current contents of the stack register into the X register and sets the zero and negative flags as appropriate.
	u8 TSX();

	// TXA - Transfer X to Accumulator, Copies the current contents of the X register into the accumulator and sets the zero and negative flags as appropriate.
	u8 TXA();

	// TXS - Transfer X to Stack Pointer, Copies the current contents of the X register into the stack register.
	u8 TXS();

	// TYA - Transfer Y to Accumulator, Copies the current contents of the Y register into the accumulator and sets the zero and negative flags as appropriate.
	u8 TYA();

	// Invalid Instruction
	u8 XXX();

	// Instruction Struct
	struct InstructionMetaData
	{
		std::string name;
		u8 (MOS6502::*Operation)(void);
		u8 (MOS6502::*addresMode)(void);
		u8 cycles = 0;
	};

	// Instruction Table
	std::vector<InstructionMetaData> lookUpTable;

	/* Instruction Cycle */

	// Fetch - Fetch instruction from memory.
	u8 Fetch(void);

	// Execute - Execute the instruction.
	void Execute(void);

	/* Memory Instruction Function */

	// read - Read a byte from the memory at a given location
	u8 (*read)(u16) = &Read;

	// write - write a byte of memory at a given location
	void (*write)(u16, u8) = &Write;

	/* Convenience functions and var */

	u16 absoluteAddres, relativeAddres;
	u8 FetchedByte;

	// getFlag - gets a flag
	u8 getFlag(Flags flag);

	// setFlag - sets a flag
	void setFlag(Flags flag, bool state);

	void tick();
};
