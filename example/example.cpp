// The example shows the Fibonacci Sequence
#include <SFML/Graphics.hpp>
#include <bits/stdc++.h>
#include "6502.hpp"
#include "Bus.hpp"

/* Meta Data */

// The Bus which connects CPU and RAM
Bus bus = Bus();

// Height And Width Of the Screen
#define HEIGHT 600
#define WIDTH 550

/* Helper Function */

// Supplying external read and write Function
u8 Read(u16 addres)
{
    return bus.read(addres);
};

void Write(u16 addres, u8 value)
{
    return bus.write(addres, value);
};

// Converte a decimal digit to hex
std::string hex(uint32_t decimalNumber, uint8_t digitCount)
{
    std::string hexDigit(digitCount, '0');
    for (int i = digitCount - 1; i >= 0; i--, decimalNumber >>= 4)
    {
        hexDigit[i] = "0123456789ABCDEF"[decimalNumber & 0xF];
    };
    return hexDigit;
};

// Draw the Ram
void DrawRam(int x, int y, uint16_t nAddr, int nRows, int nColumns, sf::Font &font, sf::RenderWindow &window)
{
    int nRamX = x, nRamY = y;
    for (int row = 0; row < nRows; row++)
    {
        std::string sOffset = "$" + hex(nAddr, 4) + ":";
        for (int col = 0; col < nColumns; col++)
        {
            sOffset += " " + hex(bus.read(nAddr), 2);
            nAddr += 1;
        }
        sf::Text text;
        text.setFont(font);
        text.setString(sOffset);
        text.setCharacterSize(10);
        text.setLetterSpacing(2);
        text.setFillColor(sf::Color::White);
        text.setPosition((float)nRamX, (float)nRamY);
        window.draw(text);
        nRamY += 10;
    }
};

// Draw the CPU
void DrawCPU(int x, int y, sf::Font &font, sf::RenderWindow &window)
{
    sf::Text text;
    text.setFont(font);
    text.setString("STATUS - ");
    text.setCharacterSize(10);
    text.setLetterSpacing(2);
    text.setFillColor(sf::Color::White);
    text.move((float)x, (float)y);
    window.draw(text);

    text.setString("N");
    text.setFillColor(bus.CPU.getFlag(bus.CPU.N) ? sf::Color::Green : sf::Color::Red);
    text.setPosition(x + 64, y);
    window.draw(text);
    text.setString("V");
    text.setFillColor(bus.CPU.getFlag(bus.CPU.V) ? sf::Color::Green : sf::Color::Red);
    text.setPosition(x + 80, y);
    window.draw(text);
    text.setString("-");
    text.setFillColor(bus.CPU.getFlag(bus.CPU.U) ? sf::Color::Green : sf::Color::Red);
    text.setPosition(x + 96, y);
    window.draw(text);
    text.setString("B");
    text.setFillColor(bus.CPU.getFlag(bus.CPU.B) ? sf::Color::Green : sf::Color::Red);
    text.setPosition(x + 112, y);
    window.draw(text);
    text.setString("D");
    text.setFillColor(bus.CPU.getFlag(bus.CPU.D) ? sf::Color::Green : sf::Color::Red);
    text.setPosition(x + 128, y);
    window.draw(text);
    text.setString("I");
    text.setFillColor(bus.CPU.getFlag(bus.CPU.I) ? sf::Color::Green : sf::Color::Red);
    text.setPosition(x + 144, y);
    window.draw(text);
    text.setString("Z");
    text.setFillColor(bus.CPU.getFlag(bus.CPU.Z) ? sf::Color::Green : sf::Color::Red);
    text.setPosition(x + 160, y);
    window.draw(text);
    text.setString("C");
    text.setFillColor(bus.CPU.getFlag(bus.CPU.C) ? sf::Color::Green : sf::Color::Red);
    text.setPosition(x + 178, y);
    window.draw(text);

    text.setFillColor(sf::Color::White);
    text.setString("PC: $" + hex(bus.CPU.PC, 4));
    text.setPosition(x, y + 10);
    window.draw(text);
    text.setString("A: $" + hex(bus.CPU.A, 2) + "   [" + std::to_string(bus.CPU.A) + "]");
    text.setPosition(x, y + 20);
    window.draw(text);
    text.setString("X: $" + hex(bus.CPU.X, 2) + "   [" + std::to_string(bus.CPU.X) + "]");
    text.setPosition(x, y + 30);
    window.draw(text);
    text.setString("Y: $" + hex(bus.CPU.Y, 2) + "   [" + std::to_string(bus.CPU.Y) + "]");
    text.setPosition(x, y + 40);
    window.draw(text);
    text.setString("SP: $" + hex(bus.CPU.SP, 4));
    text.setPosition(x, y + 50);
    window.draw(text);
};

int main(void)
{
    // Opening a window
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "6502 Demonstration");

    // Loading the Font
    sf::Font font;
    font.loadFromFile("arial.ttf");

    // loading program to memory
    std::stringstream ss;
    ss << "A9 00 85 F0 A9 01 85 F1 A2 00 A5 F1 9D 1B 0F 85 F2 65 F0 85 F1 A5 F2 85 F0 E8 E0 0A 30 EC 60";
    uint16_t nOffset = 0x8000;
    while (!ss.eof())
    {
        std::string b;
        ss >> b;
        bus.RAM[nOffset++] = (uint8_t)std::stoul(b, nullptr, 16);
    }

    // While Window is Open
    while (window.isOpen())
    {
        // Event var, to check all the window's events that were triggered
        sf::Event event;

        // Pop the event on top of the event queue, if any, and return it.
        while (window.pollEvent(event))
        {
            // The Closed Event is triggered when the user wants to close the window.
            if (event.type == sf::Event::Closed || event.key.code == sf::Keyboard::Escape)
            {
                // Than we close the window
                window.close();
            }
            else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space)
            {
                bus.CPU.tick();
            }
            else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::R)
            {
                bus.CPU = MOS6502();
            }
            else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::I)
            {
                bus.CPU.IRQB();
            }
            else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::N)
            {
                bus.CPU.NMIB();
            }
            else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter)
            {
                bus.CPU.Execute();
            };
        };

        // Clear screen
        window.clear(sf::Color(0, 0, 139));

        // Drawing Ram
        DrawRam(2, 2, 0x0000, 16, 16, font, window);
        DrawRam(2, 182, 0x8000, 16, 16, font, window);

        // Drawing CPU
        DrawCPU(350, 2, font, window);

        // Instructions
        sf::Text text;
        text.setFont(font);
        text.setString("R - RESB, I - IRQB, N - NMIB, Enter - Execute Whole Code, Space - Step Instruction");
        text.setCharacterSize(10);
        text.setLetterSpacing(2);
        text.setFillColor(sf::Color::White);
        text.move((float)2, (float)585);
        window.draw(text);
        // Update the window
        window.display();
    };

    return 0;
};