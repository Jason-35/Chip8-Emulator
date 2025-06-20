#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "chip8.h"
#include <stdint.h>

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
    uint8_t opcode = ch8 -> opcode;
    uint8_t x;
    uint8_t kk;
    uint8_t y;
    switch (opcode >> 12) {
        // there is only two instructions for 0xxx
        case 0x0000:
            if (opcode == 0x00E0) {
                for (int i = 0; i < GRAPHIC_SIZE; i++) {
                    ch8 -> graphic[i] = 0;
                }
            } else {
                // opcode 0x00EE
                ch8 -> SP -= 1;
                ch8 -> PC = ch8 -> stack[ch8 -> SP];
            }
            break;
        case 0x0001:
            ch8 -> PC = opcode & 0x0FFF;
            break;
        case 0x0002:
            ch8 -> stack[ch8 -> SP] = ch8 -> PC;
            ch8 -> SP += 1;
            ch8 -> PC = opcode & 0x0FFF;
            break;
        case 0x0003:
            x = (opcode & 0x0F00) >> 8;
            kk = opcode & 0x00FF;
            if (ch8 -> V[x] == kk) {
                ch8 -> PC += 2;
            }
            ch8 -> PC += 2;
            break;
        case 0x0004:
            x = (opcode & 0x0F00) >> 8;
            kk = opcode & 0x00FF;
            if (ch8 -> V[x] != kk) {
                ch8 -> PC += 2;
            }
            ch8 -> PC += 2;
            break;
        case 0x0005:
            x = (opcode & 0x0F00) >> 8;
            y = opcode & 0x00F0 >> 4;
            if (ch8 -> V[x] == ch8 -> V[y]) {
                ch8 -> PC += 2;
            }
            ch8 -> PC += 2;
            break;
        case 0x0006:
            x = (opcode & 0x0F00) >> 8;
            uint8_t kk = opcode & 0x00FF;
            ch8 -> V[x] = kk;
            ch8 -> PC += 2;
            break;
        case 0x0007:
            break;
        case 0x0008:
            // want the 4th instruction so mask the first 12 bits
            uint8_t instr_4 = opcode & 0x000F;
            switch (instr_4) {
                case 0x0000:
                    break;
                case 0x0001:
                    break;
                case 0x0002:
                    break;
                case 0x0003:
                    break;
                case 0x0004:
                    break;
                case 0x0005:
                    break;
                case 0x0006:
                    break;
                case 0x0007:
                    break;
                case 0x000E:
                    break;
            };
            break;
        case 0x0009:
            break;
        case 0x000A:
            break;
        case 0x000B:
            break;
        case 0x000C:
            break;
        case 0x000D:
            break;
        case 0x000E:
            if ((opcode & 0x000F) == 0x000E) {
                ;
            } else {
                // 0xExA1
                ;
            }
            break;
        case 0x000F:
            switch (opcode & 0x00FF) {
                case 0x0007:
                    break;
                case 0x000A:
                    break;
                case 0x0015:
                    break;
                case 0x0018:
                    break;
                case 0x001E:
                    break;
                case 0x0029:
                    break;
                case 0x0033:
                    break;
                case 0x0055:
                    break;
                case 0x0065:
                    break;
            };
            break; 
    };
    // execute opcode
    // update timer

}



