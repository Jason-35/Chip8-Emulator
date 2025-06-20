#ifndef CHIP8_H
#define CHIP8_H

#include <stdint.h>
#define MEM_SIZE 4096
#define GENERAL_REG 16
#define STACK_SIZE 16
#define GRAPHIC_SIZE (64 * 32)
#define KEY_SIZE 16

typedef struct {
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
    uint16_t key[KEY_SIZE];
} Chip8;

Chip8 initalize_chip8();
void emulateCycle(Chip8 *ch8);

#endif
