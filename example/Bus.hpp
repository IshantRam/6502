#pragma once
#include "util.hpp"

// Memory Limit of RAM or MAX Size of RAM
#define MEMORY_LIMIT 64 * 1024

class Bus
{
public:
    // Fake RAM for this example
    u8 RAM[MEMORY_LIMIT];

    // constructor - initialize the RAM with all 0 Byte
    Bus();

    // write - write a byte to memory
    void write(u16 addres, u8 data);

    // read - read a byte of memory
    u8 read(u16 addres);
};

// ..
Bus::Bus()
{
    for (auto &i : this->RAM)
    {
        i = 0;
    };
};

// ..
void Bus::write(u16 addres, u8 data)
{
    if (addres >= 0xFFFF)
    {
        return;
    }
    this->RAM[addres] = data;
};

// ..
u8 Bus::read(u16 addres)
{
    if (addres >= 0xFFFF)
    {
        return 0;
    }
    return this->RAM[addres];
};