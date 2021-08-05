#include "util.hpp"
#include "6502.hpp"

// The Reset (RESB)
MOS6502::MOS6502()
{
    /* Initializing the Stack Pointer 0x0100, Initializing Program Counter 16 BIT addres stored at 0xFFFC and 0xFFFD*/
    this->PC = (u16)this->read(0xFFFC) | ((u16)this->read(0xFFFD) << 8);
    this->SP = 0x0100;

    /* Reseting the Primary Registers */
    this->A = 0;
    this->X = 0;
    this->Y = 0;
    this->IR = 0;
    this->Cycles = 7;

    /* Reseting the Convenience var */
    this->absoluteAddres = 0x0;
    this->relativeAddres = 0x0;
    this->FetchedByte = 0x0;

    /* Reseting the status Flags */
    this->status = U | 0x0;

    lookUpTable =
        {
            {"BRK", &MOS6502::BRK, &MOS6502::IMM, 7},
            {"ORA", &MOS6502::ORA, &MOS6502::IZX, 6},
            {"???", &MOS6502::XXX, &MOS6502::IMP, 2},
            {"???", &MOS6502::XXX, &MOS6502::IMP, 8},
            {"???", &MOS6502::NOP, &MOS6502::IMP, 3},
            {"ORA", &MOS6502::ORA, &MOS6502::ZP0, 3},
            {"ASL", &MOS6502::ASL, &MOS6502::ZP0, 5},
            {"???", &MOS6502::XXX, &MOS6502::IMP, 5},
            {"PHP", &MOS6502::PHP, &MOS6502::IMP, 3},
            {"ORA", &MOS6502::ORA, &MOS6502::IMM, 2},
            {"ASL", &MOS6502::ASL, &MOS6502::IMP, 2},
            {"???", &MOS6502::XXX, &MOS6502::IMP, 2},
            {"???", &MOS6502::NOP, &MOS6502::IMP, 4},
            {"ORA", &MOS6502::ORA, &MOS6502::ABS, 4},
            {"ASL", &MOS6502::ASL, &MOS6502::ABS, 6},
            {"???", &MOS6502::XXX, &MOS6502::IMP, 6},
            {"BPL", &MOS6502::BPL, &MOS6502::REL, 2},
            {"ORA", &MOS6502::ORA, &MOS6502::IZY, 5},
            {"???", &MOS6502::XXX, &MOS6502::IMP, 2},
            {"???", &MOS6502::XXX, &MOS6502::IMP, 8},
            {"???", &MOS6502::NOP, &MOS6502::IMP, 4},
            {"ORA", &MOS6502::ORA, &MOS6502::ZPX, 4},
            {"ASL", &MOS6502::ASL, &MOS6502::ZPX, 6},
            {"???", &MOS6502::XXX, &MOS6502::IMP, 6},
            {"CLC", &MOS6502::CLC, &MOS6502::IMP, 2},
            {"ORA", &MOS6502::ORA, &MOS6502::ABY, 4},
            {"???", &MOS6502::NOP, &MOS6502::IMP, 2},
            {"???", &MOS6502::XXX, &MOS6502::IMP, 7},
            {"???", &MOS6502::NOP, &MOS6502::IMP, 4},
            {"ORA", &MOS6502::ORA, &MOS6502::ABX, 4},
            {"ASL", &MOS6502::ASL, &MOS6502::ABX, 7},
            {"???", &MOS6502::XXX, &MOS6502::IMP, 7},
            {"JSR", &MOS6502::JSR, &MOS6502::ABS, 6},
            {"AND", &MOS6502::AND, &MOS6502::IZX, 6},
            {"???", &MOS6502::XXX, &MOS6502::IMP, 2},
            {"???", &MOS6502::XXX, &MOS6502::IMP, 8},
            {"BIT", &MOS6502::BIT, &MOS6502::ZP0, 3},
            {"AND", &MOS6502::AND, &MOS6502::ZP0, 3},
            {"ROL", &MOS6502::ROL, &MOS6502::ZP0, 5},
            {"???", &MOS6502::XXX, &MOS6502::IMP, 5},
            {"PLP", &MOS6502::PLP, &MOS6502::IMP, 4},
            {"AND", &MOS6502::AND, &MOS6502::IMM, 2},
            {"ROL", &MOS6502::ROL, &MOS6502::IMP, 2},
            {"???", &MOS6502::XXX, &MOS6502::IMP, 2},
            {"BIT", &MOS6502::BIT, &MOS6502::ABS, 4},
            {"AND", &MOS6502::AND, &MOS6502::ABS, 4},
            {"ROL", &MOS6502::ROL, &MOS6502::ABS, 6},
            {"???", &MOS6502::XXX, &MOS6502::IMP, 6},
            {"BMI", &MOS6502::BMI, &MOS6502::REL, 2},
            {"AND", &MOS6502::AND, &MOS6502::IZY, 5},
            {"???", &MOS6502::XXX, &MOS6502::IMP, 2},
            {"???", &MOS6502::XXX, &MOS6502::IMP, 8},
            {"???", &MOS6502::NOP, &MOS6502::IMP, 4},
            {"AND", &MOS6502::AND, &MOS6502::ZPX, 4},
            {"ROL", &MOS6502::ROL, &MOS6502::ZPX, 6},
            {"???", &MOS6502::XXX, &MOS6502::IMP, 6},
            {"SEC", &MOS6502::SEC, &MOS6502::IMP, 2},
            {"AND", &MOS6502::AND, &MOS6502::ABY, 4},
            {"???", &MOS6502::NOP, &MOS6502::IMP, 2},
            {"???", &MOS6502::XXX, &MOS6502::IMP, 7},
            {"???", &MOS6502::NOP, &MOS6502::IMP, 4},
            {"AND", &MOS6502::AND, &MOS6502::ABX, 4},
            {"ROL", &MOS6502::ROL, &MOS6502::ABX, 7},
            {"???", &MOS6502::XXX, &MOS6502::IMP, 7},
            {"RTI", &MOS6502::RTI, &MOS6502::IMP, 6},
            {"EOR", &MOS6502::EOR, &MOS6502::IZX, 6},
            {"???", &MOS6502::XXX, &MOS6502::IMP, 2},
            {"???", &MOS6502::XXX, &MOS6502::IMP, 8},
            {"???", &MOS6502::NOP, &MOS6502::IMP, 3},
            {"EOR", &MOS6502::EOR, &MOS6502::ZP0, 3},
            {"LSR", &MOS6502::LSR, &MOS6502::ZP0, 5},
            {"???", &MOS6502::XXX, &MOS6502::IMP, 5},
            {"PHA", &MOS6502::PHA, &MOS6502::IMP, 3},
            {"EOR", &MOS6502::EOR, &MOS6502::IMM, 2},
            {"LSR", &MOS6502::LSR, &MOS6502::IMP, 2},
            {"???", &MOS6502::XXX, &MOS6502::IMP, 2},
            {"JMP", &MOS6502::JMP, &MOS6502::ABS, 3},
            {"EOR", &MOS6502::EOR, &MOS6502::ABS, 4},
            {"LSR", &MOS6502::LSR, &MOS6502::ABS, 6},
            {"???", &MOS6502::XXX, &MOS6502::IMP, 6},
            {"BVC", &MOS6502::BVC, &MOS6502::REL, 2},
            {"EOR", &MOS6502::EOR, &MOS6502::IZY, 5},
            {"???", &MOS6502::XXX, &MOS6502::IMP, 2},
            {"???", &MOS6502::XXX, &MOS6502::IMP, 8},
            {"???", &MOS6502::NOP, &MOS6502::IMP, 4},
            {"EOR", &MOS6502::EOR, &MOS6502::ZPX, 4},
            {"LSR", &MOS6502::LSR, &MOS6502::ZPX, 6},
            {"???", &MOS6502::XXX, &MOS6502::IMP, 6},
            {"CLI", &MOS6502::CLI, &MOS6502::IMP, 2},
            {"EOR", &MOS6502::EOR, &MOS6502::ABY, 4},
            {"???", &MOS6502::NOP, &MOS6502::IMP, 2},
            {"???", &MOS6502::XXX, &MOS6502::IMP, 7},
            {"???", &MOS6502::NOP, &MOS6502::IMP, 4},
            {"EOR", &MOS6502::EOR, &MOS6502::ABX, 4},
            {"LSR", &MOS6502::LSR, &MOS6502::ABX, 7},
            {"???", &MOS6502::XXX, &MOS6502::IMP, 7},
            {"RTS", &MOS6502::RTS, &MOS6502::IMP, 6},
            {"ADC", &MOS6502::ADC, &MOS6502::IZX, 6},
            {"???", &MOS6502::XXX, &MOS6502::IMP, 2},
            {"???", &MOS6502::XXX, &MOS6502::IMP, 8},
            {"???", &MOS6502::NOP, &MOS6502::IMP, 3},
            {"ADC", &MOS6502::ADC, &MOS6502::ZP0, 3},
            {"ROR", &MOS6502::ROR, &MOS6502::ZP0, 5},
            {"???", &MOS6502::XXX, &MOS6502::IMP, 5},
            {"PLA", &MOS6502::PLA, &MOS6502::IMP, 4},
            {"ADC", &MOS6502::ADC, &MOS6502::IMM, 2},
            {"ROR", &MOS6502::ROR, &MOS6502::IMP, 2},
            {"???", &MOS6502::XXX, &MOS6502::IMP, 2},
            {"JMP", &MOS6502::JMP, &MOS6502::IND, 5},
            {"ADC", &MOS6502::ADC, &MOS6502::ABS, 4},
            {"ROR", &MOS6502::ROR, &MOS6502::ABS, 6},
            {"???", &MOS6502::XXX, &MOS6502::IMP, 6},
            {"BVS", &MOS6502::BVS, &MOS6502::REL, 2},
            {"ADC", &MOS6502::ADC, &MOS6502::IZY, 5},
            {"???", &MOS6502::XXX, &MOS6502::IMP, 2},
            {"???", &MOS6502::XXX, &MOS6502::IMP, 8},
            {"???", &MOS6502::NOP, &MOS6502::IMP, 4},
            {"ADC", &MOS6502::ADC, &MOS6502::ZPX, 4},
            {"ROR", &MOS6502::ROR, &MOS6502::ZPX, 6},
            {"???", &MOS6502::XXX, &MOS6502::IMP, 6},
            {"SEI", &MOS6502::SEI, &MOS6502::IMP, 2},
            {"ADC", &MOS6502::ADC, &MOS6502::ABY, 4},
            {"???", &MOS6502::NOP, &MOS6502::IMP, 2},
            {"???", &MOS6502::XXX, &MOS6502::IMP, 7},
            {"???", &MOS6502::NOP, &MOS6502::IMP, 4},
            {"ADC", &MOS6502::ADC, &MOS6502::ABX, 4},
            {"ROR", &MOS6502::ROR, &MOS6502::ABX, 7},
            {"???", &MOS6502::XXX, &MOS6502::IMP, 7},
            {"???", &MOS6502::NOP, &MOS6502::IMP, 2},
            {"STA", &MOS6502::STA, &MOS6502::IZX, 6},
            {"???", &MOS6502::NOP, &MOS6502::IMP, 2},
            {"???", &MOS6502::XXX, &MOS6502::IMP, 6},
            {"STY", &MOS6502::STY, &MOS6502::ZP0, 3},
            {"STA", &MOS6502::STA, &MOS6502::ZP0, 3},
            {"STX", &MOS6502::STX, &MOS6502::ZP0, 3},
            {"???", &MOS6502::XXX, &MOS6502::IMP, 3},
            {"DEY", &MOS6502::DEY, &MOS6502::IMP, 2},
            {"???", &MOS6502::NOP, &MOS6502::IMP, 2},
            {"TXA", &MOS6502::TXA, &MOS6502::IMP, 2},
            {"???", &MOS6502::XXX, &MOS6502::IMP, 2},
            {"STY", &MOS6502::STY, &MOS6502::ABS, 4},
            {"STA", &MOS6502::STA, &MOS6502::ABS, 4},
            {"STX", &MOS6502::STX, &MOS6502::ABS, 4},
            {"???", &MOS6502::XXX, &MOS6502::IMP, 4},
            {"BCC", &MOS6502::BCC, &MOS6502::REL, 2},
            {"STA", &MOS6502::STA, &MOS6502::IZY, 6},
            {"???", &MOS6502::XXX, &MOS6502::IMP, 2},
            {"???", &MOS6502::XXX, &MOS6502::IMP, 6},
            {"STY", &MOS6502::STY, &MOS6502::ZPX, 4},
            {"STA", &MOS6502::STA, &MOS6502::ZPX, 4},
            {"STX", &MOS6502::STX, &MOS6502::ZPY, 4},
            {"???", &MOS6502::XXX, &MOS6502::IMP, 4},
            {"TYA", &MOS6502::TYA, &MOS6502::IMP, 2},
            {"STA", &MOS6502::STA, &MOS6502::ABY, 5},
            {"TXS", &MOS6502::TXS, &MOS6502::IMP, 2},
            {"???", &MOS6502::XXX, &MOS6502::IMP, 5},
            {"???", &MOS6502::NOP, &MOS6502::IMP, 5},
            {"STA", &MOS6502::STA, &MOS6502::ABX, 5},
            {"???", &MOS6502::XXX, &MOS6502::IMP, 5},
            {"???", &MOS6502::XXX, &MOS6502::IMP, 5},
            {"LDY", &MOS6502::LDY, &MOS6502::IMM, 2},
            {"LDA", &MOS6502::LDA, &MOS6502::IZX, 6},
            {"LDX", &MOS6502::LDX, &MOS6502::IMM, 2},
            {"???", &MOS6502::XXX, &MOS6502::IMP, 6},
            {"LDY", &MOS6502::LDY, &MOS6502::ZP0, 3},
            {"LDA", &MOS6502::LDA, &MOS6502::ZP0, 3},
            {"LDX", &MOS6502::LDX, &MOS6502::ZP0, 3},
            {"???", &MOS6502::XXX, &MOS6502::IMP, 3},
            {"TAY", &MOS6502::TAY, &MOS6502::IMP, 2},
            {"LDA", &MOS6502::LDA, &MOS6502::IMM, 2},
            {"TAX", &MOS6502::TAX, &MOS6502::IMP, 2},
            {"???", &MOS6502::XXX, &MOS6502::IMP, 2},
            {"LDY", &MOS6502::LDY, &MOS6502::ABS, 4},
            {"LDA", &MOS6502::LDA, &MOS6502::ABS, 4},
            {"LDX", &MOS6502::LDX, &MOS6502::ABS, 4},
            {"???", &MOS6502::XXX, &MOS6502::IMP, 4},
            {"BCS", &MOS6502::BCS, &MOS6502::REL, 2},
            {"LDA", &MOS6502::LDA, &MOS6502::IZY, 5},
            {"???", &MOS6502::XXX, &MOS6502::IMP, 2},
            {"???", &MOS6502::XXX, &MOS6502::IMP, 5},
            {"LDY", &MOS6502::LDY, &MOS6502::ZPX, 4},
            {"LDA", &MOS6502::LDA, &MOS6502::ZPX, 4},
            {"LDX", &MOS6502::LDX, &MOS6502::ZPY, 4},
            {"???", &MOS6502::XXX, &MOS6502::IMP, 4},
            {"CLV", &MOS6502::CLV, &MOS6502::IMP, 2},
            {"LDA", &MOS6502::LDA, &MOS6502::ABY, 4},
            {"TSX", &MOS6502::TSX, &MOS6502::IMP, 2},
            {"???", &MOS6502::XXX, &MOS6502::IMP, 4},
            {"LDY", &MOS6502::LDY, &MOS6502::ABX, 4},
            {"LDA", &MOS6502::LDA, &MOS6502::ABX, 4},
            {"LDX", &MOS6502::LDX, &MOS6502::ABY, 4},
            {"???", &MOS6502::XXX, &MOS6502::IMP, 4},
            {"CPY", &MOS6502::CPY, &MOS6502::IMM, 2},
            {"CMP", &MOS6502::CMP, &MOS6502::IZX, 6},
            {"???", &MOS6502::NOP, &MOS6502::IMP, 2},
            {"???", &MOS6502::XXX, &MOS6502::IMP, 8},
            {"CPY", &MOS6502::CPY, &MOS6502::ZP0, 3},
            {"CMP", &MOS6502::CMP, &MOS6502::ZP0, 3},
            {"DEC", &MOS6502::DEC, &MOS6502::ZP0, 5},
            {"???", &MOS6502::XXX, &MOS6502::IMP, 5},
            {"INY", &MOS6502::INY, &MOS6502::IMP, 2},
            {"CMP", &MOS6502::CMP, &MOS6502::IMM, 2},
            {"DEX", &MOS6502::DEX, &MOS6502::IMP, 2},
            {"???", &MOS6502::XXX, &MOS6502::IMP, 2},
            {"CPY", &MOS6502::CPY, &MOS6502::ABS, 4},
            {"CMP", &MOS6502::CMP, &MOS6502::ABS, 4},
            {"DEC", &MOS6502::DEC, &MOS6502::ABS, 6},
            {"???", &MOS6502::XXX, &MOS6502::IMP, 6},
            {"BNE", &MOS6502::BNE, &MOS6502::REL, 2},
            {"CMP", &MOS6502::CMP, &MOS6502::IZY, 5},
            {"???", &MOS6502::XXX, &MOS6502::IMP, 2},
            {"???", &MOS6502::XXX, &MOS6502::IMP, 8},
            {"???", &MOS6502::NOP, &MOS6502::IMP, 4},
            {"CMP", &MOS6502::CMP, &MOS6502::ZPX, 4},
            {"DEC", &MOS6502::DEC, &MOS6502::ZPX, 6},
            {"???", &MOS6502::XXX, &MOS6502::IMP, 6},
            {"CLD", &MOS6502::CLD, &MOS6502::IMP, 2},
            {"CMP", &MOS6502::CMP, &MOS6502::ABY, 4},
            {"NOP", &MOS6502::NOP, &MOS6502::IMP, 2},
            {"???", &MOS6502::XXX, &MOS6502::IMP, 7},
            {"???", &MOS6502::NOP, &MOS6502::IMP, 4},
            {"CMP", &MOS6502::CMP, &MOS6502::ABX, 4},
            {"DEC", &MOS6502::DEC, &MOS6502::ABX, 7},
            {"???", &MOS6502::XXX, &MOS6502::IMP, 7},
            {"CPX", &MOS6502::CPX, &MOS6502::IMM, 2},
            {"SBC", &MOS6502::SBC, &MOS6502::IZX, 6},
            {"???", &MOS6502::NOP, &MOS6502::IMP, 2},
            {"???", &MOS6502::XXX, &MOS6502::IMP, 8},
            {"CPX", &MOS6502::CPX, &MOS6502::ZP0, 3},
            {"SBC", &MOS6502::SBC, &MOS6502::ZP0, 3},
            {"INC", &MOS6502::INC, &MOS6502::ZP0, 5},
            {"???", &MOS6502::XXX, &MOS6502::IMP, 5},
            {"INX", &MOS6502::INX, &MOS6502::IMP, 2},
            {"SBC", &MOS6502::SBC, &MOS6502::IMM, 2},
            {"NOP", &MOS6502::NOP, &MOS6502::IMP, 2},
            {"???", &MOS6502::SBC, &MOS6502::IMP, 2},
            {"CPX", &MOS6502::CPX, &MOS6502::ABS, 4},
            {"SBC", &MOS6502::SBC, &MOS6502::ABS, 4},
            {"INC", &MOS6502::INC, &MOS6502::ABS, 6},
            {"???", &MOS6502::XXX, &MOS6502::IMP, 6},
            {"BEQ", &MOS6502::BEQ, &MOS6502::REL, 2},
            {"SBC", &MOS6502::SBC, &MOS6502::IZY, 5},
            {"???", &MOS6502::XXX, &MOS6502::IMP, 2},
            {"???", &MOS6502::XXX, &MOS6502::IMP, 8},
            {"???", &MOS6502::NOP, &MOS6502::IMP, 4},
            {"SBC", &MOS6502::SBC, &MOS6502::ZPX, 4},
            {"INC", &MOS6502::INC, &MOS6502::ZPX, 6},
            {"???", &MOS6502::XXX, &MOS6502::IMP, 6},
            {"SED", &MOS6502::SED, &MOS6502::IMP, 2},
            {"SBC", &MOS6502::SBC, &MOS6502::ABY, 4},
            {"NOP", &MOS6502::NOP, &MOS6502::IMP, 2},
            {"???", &MOS6502::XXX, &MOS6502::IMP, 7},
            {"???", &MOS6502::NOP, &MOS6502::IMP, 4},
            {"SBC", &MOS6502::SBC, &MOS6502::ABX, 4},
            {"INC", &MOS6502::INC, &MOS6502::ABX, 7},
            {"???", &MOS6502::XXX, &MOS6502::IMP, 7},
        };
};

// The Interrupt Request (IRQB)
void MOS6502::IRQB()
{
    // if the Interrupt Disable is not set
    if (!getFlag(I))
    {
        // Pusing the Program Counter on the stack
        write(0x100 + this->SP, (this->PC >> 8) & 0xFF);
        write(0x100 + (this->SP - 1), this->PC & 0xFF);

        // Pushing the Flag on the stack
        setFlag(B, 0);
        setFlag(U, 1);
        setFlag(I, 1);
        write(0x0100 + (this->SP - 2), this->status);

        // Initializing Program Counter 16 BIT addres stored at 0xFFFE and 0xFFFF
        this->PC = (u16)this->read(0xFFFE) | ((u16)this->read(0xFFFF) << 8);

        // Decrementing the Stack Pointer
        this->SP -= 3;
    };
};

// The Non-Maskable Interrupt (NMIB)
void MOS6502::NMIB()
{
    // Pusing the Program Counter on the stack
    write(0x100 + this->SP, (this->PC >> 8) & 0xFF);
    write(0x100 + (this->SP - 1), this->PC & 0xFF);

    // Pushing the Flag on the stack
    setFlag(B, 0);
    setFlag(U, 1);
    setFlag(I, 1);
    write(0x0100 + (this->SP - 2), this->status);

    // Initializing Program Counter 16 BIT addres stored at 0xFFFE and 0xFFFF
    this->PC = (u16)this->read(0xFFFE) | ((u16)this->read(0xFFFF) << 8);

    // Decrementing the Stack Pointer
    this->SP -= 3;
};

// Fetch
u8 MOS6502::Fetch(void)
{
    if (!(lookUpTable[this->IR].addresMode == &MOS6502::IMP))
    {
        FetchedByte = read(absoluteAddres);
    };
    return FetchedByte;
};

// Execute - Execute the instruction
void MOS6502::Execute()
{
    while (true)
    {
        this->IR = this->read(this->PC);
        if (this->IR == 0)
            break;
        this->PC += 1;

        this->Cycles += this->lookUpTable[this->IR].cycles;
        u8 addresMode = (this->*lookUpTable[this->IR].addresMode)();
        u8 Operation = (this->*lookUpTable[this->IR].Operation)();

        this->Cycles += (addresMode & Operation);
        setFlag(U, 1);
    };
};

// get Flag
u8 MOS6502::getFlag(Flags flag)
{
    return ((this->status & flag) > 0) ? 1 : 0;
};

// set Flag
void MOS6502::setFlag(Flags flag, bool state)
{
    if (state)
    {
        this->status |= flag;
    }
    else
    {
        this->status &= ~flag;
    }
};

void MOS6502::tick()
{
    this->IR = this->read(this->PC);
    if (this->IR == 0)
        return;
    this->PC += 1;

    this->Cycles += this->lookUpTable[this->IR].cycles;
    u8 addresMode = (this->*lookUpTable[this->IR].addresMode)();
    u8 Operation = (this->*lookUpTable[this->IR].Operation)();

    this->Cycles += (addresMode & Operation);
    setFlag(U, 1);
};

/* ADDRESSING MODES */

// Impiled
u8 MOS6502::IMP()
{
    this->FetchedByte = this->A;
    return 0;
};

// Immediate
u8 MOS6502::IMM()
{
    this->absoluteAddres = this->PC++;
    return 0;
};

// Zero Page
u8 MOS6502::ZP0()
{
    this->absoluteAddres = this->read(this->PC);
    this->PC += 1;
    this->absoluteAddres &= 0xFF;

    return 0;
};

// Zero Page X
u8 MOS6502::ZPX()
{
    this->absoluteAddres = this->read(this->PC + this->X);
    this->PC += 1;
    this->absoluteAddres &= 0xFF;

    return 0;
};

// Zero Page Y
u8 MOS6502::ZPY()
{
    this->absoluteAddres = this->read(this->PC + this->Y);
    this->PC += 1;
    this->absoluteAddres &= 0xFF;

    return 0;
};

// Relative
u8 MOS6502::REL()
{
    this->relativeAddres = this->read(this->PC);
    this->PC += 1;
    if (this->relativeAddres & 0x80)
    {
        this->relativeAddres |= 0xFF00;
    };

    return 0;
};

// Absolute
u8 MOS6502::ABS()
{
    u8 lowByte = this->read(this->PC);
    u8 hightByte = this->read(this->PC + 1);
    this->absoluteAddres = (hightByte << 8) | lowByte;
    this->PC += 2;

    return 0;
};

// Absolute X
u8 MOS6502::ABX()
{
    u8 lowByte = this->read(this->PC);
    u8 hightByte = this->read(this->PC + 1);
    this->absoluteAddres = ((hightByte << 8) | lowByte) + this->X;

    this->PC += 2;

    if ((this->absoluteAddres & 0xFF00) != (hightByte << 8))
    {
        return 1;
    };

    return 0;
};

// Absolute Y
u8 MOS6502::ABY()
{
    u8 lowByte = this->read(this->PC);
    u8 hightByte = this->read(this->PC + 1);
    this->absoluteAddres = ((hightByte << 8) | lowByte) + this->Y;

    this->PC += 2;

    if ((this->absoluteAddres & 0xFF00) != (hightByte << 8))
    {
        return 1;
    };

    return 0;
};

// Indirect
u8 MOS6502::IND()
{
    u16 ptrLowByte = this->read(this->PC);
    u16 ptrHighByte = this->read(this->PC + 1);
    u16 ptr = (ptrHighByte << 8) | ptrLowByte;

    if (ptrLowByte == 0x00FF)
    {
        this->absoluteAddres = (this->read(ptr & 0xFF00) << 8) | this->read(ptr);
    }
    else
    {
        this->absoluteAddres = (this->read(ptr + 1) << 8) | this->read(ptr);
    };

    return 0;
};

// Indirect Indexed
u8 MOS6502::IZX()
{
    u16 tmp = this->read(this->PC);
    this->PC += 1;

    u16 lowByte = this->read((u16)(tmp + (u16)this->X) & 0x00FF);
    u16 highByte = this->read((u16)(tmp + (u16)this->X + 1) & 0x00FF);

    this->absoluteAddres = (highByte << 8) | lowByte;

    return 0;
};

// Indexed Indirect
u8 MOS6502::IZY()
{
    u16 t = this->read(this->PC);
    this->PC += 1;

    u16 lowByte = this->read(t & 0x00FF);
    u16 highByte = this->read((t + 1) & 0x00FF);

    absoluteAddres = (highByte << 8) | lowByte;
    absoluteAddres += this->Y;

    if ((absoluteAddres & 0xFF00) != (highByte << 8))
    {
        return 1;
    };

    return 0;
};

/* Instructions */

// ADC
u8 MOS6502::ADC()
{
    Fetch();

    u16 result = this->A + this->FetchedByte + getFlag(C);

    setFlag(C, result > 255);
    setFlag(Z, (result & 0xFF) == 0);
    setFlag(V, (~(this->A) ^ this->FetchedByte) & (this->A ^ result) & 0x80);
    setFlag(N, result & 0x80);

    this->A = result & 0xFF;

    return 1;
};

// AND
u8 MOS6502::AND()
{
    Fetch();

    this->A = this->A & FetchedByte;

    setFlag(Z, this->A == 0);
    setFlag(N, this->A & 0x80);

    return 1;
};

// ASL
u8 MOS6502::ASL()
{
    Fetch();

    u16 result = FetchedByte << 1;
    setFlag(C, (result & 0xFF00) > 0);
    setFlag(Z, (result & 0x00FF) == 0);
    setFlag(N, result & 0x80);

    if (lookUpTable[this->IR].addresMode == &MOS6502::IMP)
    {
        this->A = result & 0x00FF;
    }
    else
    {
        write(absoluteAddres, result & 0x00FF);
    };

    return 0;
};

// BCC
u8 MOS6502::BCC()
{
    if (getFlag(C) == 0)
    {
        this->Cycles++;
        this->absoluteAddres = this->PC + this->relativeAddres;

        if ((absoluteAddres & 0xFF00) != (this->PC & 0xFF00))
            this->Cycles++;

        this->PC = absoluteAddres;
    };

    return 0;
};

// BCS
u8 MOS6502::BCS()
{
    if (getFlag(C) == 1)
    {
        this->Cycles++;
        this->absoluteAddres = this->PC + this->relativeAddres;

        if ((absoluteAddres & 0xFF00) != (this->PC & 0xFF00))
            this->Cycles++;

        this->PC = absoluteAddres;
    };

    return 0;
};

// BEQ
u8 MOS6502::BEQ()
{
    if (getFlag(Z) == 1)
    {
        this->Cycles++;
        this->absoluteAddres = this->PC + this->relativeAddres;

        if ((absoluteAddres & 0xFF00) != (this->PC & 0xFF00))
            this->Cycles++;

        this->PC = absoluteAddres;
    };

    return 0;
};

// BIT
u8 MOS6502::BIT()
{
    Fetch();

    u16 result = this->A & FetchedByte;

    setFlag(Z, (result & 0x00FF) == 0x00);
    setFlag(N, FetchedByte & (1 << 7));
    setFlag(V, FetchedByte & (1 << 6));

    return 0;
};

// BMI
u8 MOS6502::BMI()
{
    if (getFlag(N) == 1)
    {
        this->Cycles++;
        this->absoluteAddres = this->PC + this->relativeAddres;

        if ((absoluteAddres & 0xFF00) != (this->PC & 0xFF00))
            this->Cycles++;

        this->PC = absoluteAddres;
    };

    return 0;
};

// BNE
u8 MOS6502::BNE()
{
    if (getFlag(Z) == 0)
    {
        this->Cycles++;
        this->absoluteAddres = this->PC + this->relativeAddres;

        if ((absoluteAddres & 0xFF00) != (this->PC & 0xFF00))
            this->Cycles++;

        this->PC = absoluteAddres;
    };

    return 0;
};

// BNE
u8 MOS6502::BPL()
{
    if (getFlag(N) == 0)
    {
        this->Cycles++;
        this->absoluteAddres = this->PC + this->relativeAddres;

        if ((absoluteAddres & 0xFF00) != (this->PC & 0xFF00))
            this->Cycles++;

        this->PC = absoluteAddres;
    };

    return 0;
};

// BRK
u8 MOS6502::BRK()
{
    this->PC += 1;

    setFlag(I, 1);
    write(0x0100 + this->SP, (this->PC >> 8) & 0x00FF);
    ;
    write(0x0100 + (this->SP - 1), this->PC & 0x00FF);

    setFlag(B, 1);
    write(0x0100 + (this->SP - 2), this->status);
    setFlag(B, 0);

    this->SP -= 3;

    this->PC = read(0xFFFE) | (read(0xFFFF) << 8);

    return 0;
};

// BVC
u8 MOS6502::BVC()
{
    if (getFlag(V) == 0)
    {
        this->Cycles++;
        this->absoluteAddres = this->PC + this->relativeAddres;

        if ((absoluteAddres & 0xFF00) != (this->PC & 0xFF00))
            this->Cycles++;

        this->PC = absoluteAddres;
    };

    return 0;
};

// BVS
u8 MOS6502::BVS()
{
    if (getFlag(V) == 1)
    {
        this->Cycles++;
        this->absoluteAddres = this->PC + this->relativeAddres;

        if ((absoluteAddres & 0xFF00) != (this->PC & 0xFF00))
            this->Cycles++;

        this->PC = absoluteAddres;
    };

    return 0;
};

// CLC
u8 MOS6502::CLC()
{
    setFlag(C, 0);
    return 0;
};

// CLD
u8 MOS6502::CLD()
{
    setFlag(D, 0);
    return 0;
};

// CLI
u8 MOS6502::CLI()
{
    setFlag(I, 0);

    return 0;
};

// CLV
u8 MOS6502::CLV()
{
    setFlag(V, 0);

    return 0;
};

// CMP
u8 MOS6502::CMP()
{
    Fetch();
    u16 result = this->A - FetchedByte;
    setFlag(C, this->A >= FetchedByte);
    setFlag(Z, (result & 0x00FF) == 0x0000);
    setFlag(N, result & 0x0080);

    return 1;
};

// CPX
u8 MOS6502::CPX()
{
    Fetch();
    u16 result = this->X - FetchedByte;
    setFlag(C, this->X >= FetchedByte);
    setFlag(Z, (result & 0x00FF) == 0x0000);
    setFlag(N, result & 0x0080);

    return 1;
};

// CPY
u8 MOS6502::CPY()
{
    Fetch();
    u16 result = this->Y - FetchedByte;
    setFlag(C, this->Y >= FetchedByte);
    setFlag(Z, (result & 0x00FF) == 0x0000);
    setFlag(N, result & 0x0080);

    return 1;
};

// DEC
u8 MOS6502::DEC()
{
    Fetch();
    u16 result = FetchedByte - 1;
    write(absoluteAddres, result & 0xFF);

    setFlag(Z, (result & 0xFF) == 0x0);
    setFlag(N, result & 0x0080);

    return 0;
}

// DEX
u8 MOS6502::DEX()
{
    this->X--;
    setFlag(Z, this->X == 0x00);
    setFlag(N, this->X & 0x80);

    return 0;
};

// DEY
u8 MOS6502::DEY()
{
    this->Y--;
    setFlag(Z, this->Y == 0x00);
    setFlag(N, this->Y & 0x80);

    return 0;
};

// EOR
u8 MOS6502::EOR()
{
    Fetch();
    this->A = this->A ^ FetchedByte;
    setFlag(Z, this->A == 0x00);
    setFlag(N, this->A & 0x80);

    return 1;
};

// INC
u8 MOS6502::INC()
{
    Fetch();
    u16 result = FetchedByte + 1;
    write(absoluteAddres, result & 0xFF);

    setFlag(Z, (result & 0xFF) == 0x0);
    setFlag(N, result & 0x0080);

    return 0;
};

// INX
u8 MOS6502::INX()
{
    this->X++;
    setFlag(Z, this->X == 0x00);
    setFlag(N, this->X & 0x80);

    return 0;
};

// INY
u8 MOS6502::INY()
{
    this->Y++;
    setFlag(Z, this->Y == 0x00);
    setFlag(N, this->Y & 0x80);

    return 0;
};

// JMP
u8 MOS6502::JMP()
{
    this->PC = absoluteAddres;

    return 0;
};

// JSR
u8 MOS6502::JSR()
{
    this->PC--;

    write(0x0100 + this->SP, (this->PC >> 8) & 0x00FF);
    write(0x0100 + (this->SP - 1), this->PC & 0x00FF);

    this->PC = absoluteAddres;

    return 0;
};

// LDA
u8 MOS6502::LDA()
{
    Fetch();

    this->A = FetchedByte;
    setFlag(Z, this->A == 0);
    setFlag(N, this->A & 0x80);

    return 0;
};

// LDX
u8 MOS6502::LDX()
{
    Fetch();

    this->X = FetchedByte;
    setFlag(Z, this->X == 0);
    setFlag(N, this->X & 0x80);

    return 0;
};

// LDY
u8 MOS6502::LDY()
{
    Fetch();

    this->Y = FetchedByte;
    setFlag(Z, this->Y == 0);
    setFlag(N, this->Y & 0x80);

    return 0;
};

// LSR
u8 MOS6502::LSR()
{
    Fetch();

    setFlag(C, FetchedByte & 0x0001);
    u16 result = FetchedByte >> 1;
    setFlag(Z, (result & 0x00FF) == 0x0000);
    setFlag(N, result & 0x0080);

    if (lookUpTable[IR].addresMode == &MOS6502::IMP)
    {
        this->A = result & 0x00FF;
    }
    else
    {
        write(absoluteAddres, result & 0x00FF);
    };
    return 0;
};

// NOP
u8 MOS6502::NOP()
{
    switch (IR)
    {
    case 0x1C:
    case 0x3C:
    case 0x5C:
    case 0x7C:
    case 0xDC:
    case 0xFC:
        return 1;
        break;
    }
    return 0;
};

// ORA
u8 MOS6502::ORA()
{
    Fetch();
    this->A = this->A | FetchedByte;

    setFlag(Z, this->A == 0x00);
    setFlag(N, this->A & 0x80);

    return 1;
};

// PHA
u8 MOS6502::PHA()
{
    write(0x0100 + this->SP, this->A);
    this->SP--;

    return 0;
};

// PHP
u8 MOS6502::PHP()
{
    write(0x0100 + this->SP, this->status | B | U);

    setFlag(B, 0);
    setFlag(U, 0);
    this->SP--;

    return 0;
};

// PLA
u8 MOS6502::PLA()
{
    this->SP++;
    this->A = read(0x0100 + this->SP);

    setFlag(Z, this->A == 0x00);
    setFlag(N, this->A & 0x80);

    return 0;
};

// PLP
u8 MOS6502::PLP()
{
    this->SP++;
    this->status = read(0x0100 + this->SP);

    setFlag(U, 1);

    return 0;
};

// ROL
u8 MOS6502::ROL()
{
    Fetch();
    u16 tmp = (FetchedByte << 1) | getFlag(C);
    setFlag(C, tmp & 0xFF00);
    setFlag(Z, (tmp & 0x00FF) == 0x0000);
    setFlag(N, tmp & 0x0080);
    if (lookUpTable[IR].addresMode == &MOS6502::IMP)
    {
        this->A = tmp & 0x00FF;
    }
    else
    {
        write(absoluteAddres, tmp & 0x00FF);
    };
    return 0;
};

// ROR
u8 MOS6502::ROR()
{
    Fetch();
    u16 temp = (getFlag(C) << 7) | (FetchedByte >> 1);
    setFlag(C, FetchedByte & 0x01);
    setFlag(Z, (temp & 0x00FF) == 0x00);
    setFlag(N, temp & 0x0080);
    if (lookUpTable[IR].addresMode == &MOS6502::IMP)
    {
        this->A = temp & 0x00FF;
    }
    else
    {
        write(absoluteAddres, temp & 0x00FF);
    };
    return 0;
};

// RTI
u8 MOS6502::RTI()
{
    this->SP++;
    this->status = read(0x0100 + this->SP);
    this->status &= ~B;
    this->status &= ~U;

    this->SP++;
    this->PC = read(0x0100 + this->SP);
    this->SP++;
    this->PC |= read(0x0100 + this->SP) << 8;

    return 0;
};

// RTS
u8 MOS6502::RTS()
{
    this->SP++;
    this->PC = read(0x0100 + this->SP);
    this->SP++;
    this->PC = read(0x0100 + this->SP) << 8;

    this->PC++;

    return 0;
};

// SBC
u8 MOS6502::SBC()
{
    return 0x0;
};

// SEC
u8 MOS6502::SEC()
{
    setFlag(C, true);
    return 0;
};

// SED
u8 MOS6502::SED()
{
    setFlag(D, true);
    return 0;
};

// SEI
u8 MOS6502::SEI()
{
    setFlag(I, true);
    return 0;
};

// STA
u8 MOS6502::STA()
{
    write(absoluteAddres, this->A);
    return 0;
};

// STX
u8 MOS6502::STX()
{
    write(absoluteAddres, this->X);
    return 0;
};

// STY
u8 MOS6502::STY()
{
    write(absoluteAddres, this->Y);
    return 0;
};

// TAX
u8 MOS6502::TAX()
{
    this->X = this->A;
    setFlag(Z, this->X == 0x00);
    setFlag(N, this->X & 0x80);
    return 0;
};

// TAY
u8 MOS6502::TAY()
{
    this->Y = this->A;
    setFlag(Z, this->Y == 0x00);
    setFlag(N, this->Y & 0x80);
    return 0;
};

// TSX
u8 MOS6502::TSX()
{
    this->X = this->SP;
    setFlag(Z, this->A == 0x00);
    setFlag(N, this->A & 0x80);
    return 0;
};

// TXA
u8 MOS6502::TXA()
{
    this->A = this->X;
    setFlag(Z, this->A == 0x00);
    setFlag(N, this->A & 0x80);
    return 0;
};

// TXS
u8 MOS6502::TXS()
{
    this->SP = this->X;

    return 0;
};

// TYA
u8 MOS6502::TYA()
{
    this->A = this->Y;
    setFlag(Z, this->A == 0x00);
    setFlag(N, this->A & 0x80);
    return 0;
};

// Invalid Instruction
u8 MOS6502::XXX()
{
    return 0;
};