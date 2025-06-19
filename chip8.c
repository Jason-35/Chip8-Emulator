#include <stdio.h>
#include <stdlib.h>
#include <time.h>
/* 
typdef struct {
    uint8_t opcode;
    uint8_t memory[MEM_SIZE];
    uint8_t V[GENERAL_REG]; // The VF register should not be used by any program, as it is used as a flag by some instructions.
    uint16_t I; // This register is generally used to store memory addresses, so only the lowest (rightmost) 12 bits are usually used.
    uint8_t sound;
    uint8_t delay;
    uint16_t PC; // program counter
    uint16_t stack[STACK_SIZE];
    uint8_t SP; // stack pointer
    uint8_t graphic[GRAPHIC_SIZE];
    uint16_t key[KEY_SIZE]
} Chip8;
*/


Chip8 initalize_chip8() {
    srand(time(NULL));
    
    Chip8 ch8;
    ch8.opcode = 0;
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
} 

