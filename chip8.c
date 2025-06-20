#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "chip8.h"


unsigned char chip8_fontset[80] =
{ 
  0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
  0x20, 0x60, 0x20, 0x20, 0x70, // 1
  0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
  0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
  0x90, 0x90, 0xF0, 0x10, 0x10, // 4
  0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
  0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
  0xF0, 0x10, 0x20, 0x40, 0x40, // 7
  0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
  0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
  0xF0, 0x90, 0xF0, 0x90, 0x90, // A
  0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
  0xF0, 0x80, 0x80, 0x80, 0xF0, // C
  0xE0, 0x90, 0x90, 0x90, 0xE0, // D
  0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
  0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

Chip8 initalize_chip8() {
    srand(time(NULL));
    
    Chip8 ch8;
    ch8.opcode = 1;
    ch8.I = 0;
    ch8.sound = 0;
    ch8.delay = 0;
    ch8.PC = 0x200;
    ch8.SP = 0;
    
    for (int i = 0; i < 16; i++) {
        ch8.V[i] = 0;
        ch8.stack[i] = 0;
        ch8.key[i] = 0;
    }

    for (int i = 0; i < MEM_SIZE; i++) {
        ch8.memory[i] = 0;
    }

    for (int i = 0; i < GRAPHIC_SIZE; i++) {
        ch8.graphic[i] = 0;
    }

    for (int i = 0; i < 80; i++) {
        ch8.memory[i] = chip8_fontset[i];
    }

    return ch8;     
}



// void for now change if needed.
void emulateCycle(Chip8 *ch8) {
    // fetch opcode
    ch8->opcode = ch8->memory[ch8->PC] << 8 | ch8->memory[ch8->PC + 1];
    ch8->PC += 2;

    // decode opcode
    // opcode >> 12 gets the first 4 bits out of the 16 bit
    // 0x1 is actually 0x0001.
    switch(ch8->opcode >> 12) {
        case 0x0;
            break;
        case 0x1;
            break;
        case 0x2;
            break;
        case 0x3;
            break;
        case 0x4;
            break;
        case 0x5;
            break;
        case 0x6;
            break;
        case 0x7;
            break;
        case 0x8;
            break;
        case 0x9;
            break;
        case 0xA;
            break;
        case 0xB;
            break;
        case 0xC;
            break;
        case 0xD;
            break;
        case 0xE;
            break;
        case 0xF;
            break; 
    }
    // execute opcode
    // update timer

}



