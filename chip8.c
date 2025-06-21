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
            x = (opcode & 0x0F00) >> 8;
            kk = opcode & 0x00FF;
            ch8 -> V[x] += kk;
            ch8 -> PC += 2;
            break;
        case 0x0008:
            // want the 4th instruction so mask the first 12 bits
            uint8_t instr_4 = opcode & 0x000F;
            x = (opcode & 0x0F00) >> 8;
            y = opcode & 0x00F0 >> 4;
            switch (instr_4) {
                case 0x0000:
                    ch8 -> V[x] = ch8 -> V[y];
                    break;
                case 0x0001:
                    ch8 -> V[x] |= ch8 -> V[y]; 
                    break;
                case 0x0002:
                    ch8 -> V[x] &= ch8 -> V[y];
                    break;
                case 0x0003:
                    ch8 -> V[x] ^= ch8 -> V[y];
                    break;
                case 0x0004:
                    uint16_t add_sum = ch8 -> V[x] + ch8 -> V[y];
                    if (add_sum > 255) {
                        ch8 -> V[0x000F] = 1;
                    } else {
                        ch8 -> V[0x000F] = 0;
                    }
                    ch8 -> V[x] += ch8 -> V[y];
                    break;
                case 0x0005:
                    if (ch8 -> V[x] > ch8 -> V[y]) {
                        ch8 -> V[0x000F] = 1;
                    } else {
                        ch8 -> V[0x000F] = 0;
                    }
                    ch8 -> V[x] -= ch8 -> V[y];
                    break;
                case 0x0006:
                    ch8 -> V[0x000F] = ch8 -> V[x] & 1;
                    ch8 -> V[x] >>= 1;
                    break;
                case 0x0007:
                    if (ch8 -> V[x] < ch8 -> V[y]) {
                        ch8 -> V[0x000F] = 1;
                    } else {
                        ch8 -> V[0x000F] = 0;
                    }
                    ch8 -> V[x] = ch8 -> V[y] - ch8 -> V[x];
                    break;
                case 0x000E:
                    ch8 -> V[0x000F] = ch8 -> V[x] >> 7;
                    ch8 -> V[x] <<= 1;
                    break;
            };
            ch8 -> PC += 2;
            break;
        case 0x0009:
            if (ch8 -> V[x] != ch8 -> V[y]) {
                ch8 -> PC += 2;
            }
            ch8 -> PC +=2;
            break;
        case 0x000A:
            ch8 -> I = opcode & 0x0FFF;
            ch8 -> PC += 2;
            break;
        case 0x000B:
            ch8 -> PC = ch8 -> V[0] + (opcode & 0x0FFF);
            break;
        case 0x000C:
            x = (opcode & 0x0F00) >> 8;
            kk = opcode & 0x00FF;
            ch8 -> V[x] = (rand() % 256) & kk;
            ch8 -> PC += 2;
            break;
        case 0x000D:
            ch8 -> V[0x000F] = 0;
            x = (opcode & 0x0F00) >> 8;
            y = (opcode & 0x00F0) >> 4;
            uint8_t n = opcode & 0x000F;
            uint8_t Vx = ch8 -> V[x];
            uint8_t Vy = ch8 -> V[y];
            
            for (int y1; y1 < n; y1++) {
                uint8_t mask = 0x80;
                uint8_t pixel = ch8 -> memory[ch8 -> I + y1];
                for (int x1; x1 < 8; x1++) {

                    uint8_t tx = (x1 + Vx) % 64;                
                    uint8_t ty = (y1 + Vy) % 32;

                    uint16_t index = tx + ty * 64;

                    // pixel & mask >> x1 is used so that the left most significant bit is shifted 1 bit at a time until the end
                    // this checks if the graphic at index is 1 and if the pixel shift is greater than 0 because it can only be 0 or greater than 0 which indicate draw
                    mask >> x1;
                    if ((pixel & mask) && ch8 -> graphic[index]) {
                        ch8 -> V[0x000F] = 1;
                    }
                    // regardless of collision we draw
                    ch8 -> graphic[index] ^= (pixel & mask) ? 1 : 0; 
                }
            }
            ch8 -> PC += 2;
            break;
        case 0x000E:
            x = (opcode & 0x0F00) >> 8;
            if ((opcode & 0x000F) == 0x000E) {
                if (ch8 -> key[ch8 -> V[x]] == 1) {
                    ch8 -> PC += 2;
                }
            } else {
                // 0xExA1
                if (ch8 -> key[ch8 -> V[x]] != 1) {
                    ch8 -> PC += 2;
                }
                
            }
            ch8 -> PC += 2;
            break;
        case 0x000F:
            x = (opcode & 0x0F00) >> 8;
            switch (opcode & 0x00FF) {
                case 0x0007:
                    ch8 -> V[x] = ch8 -> delay;
                    break;
                case 0x000A:
                    uint8_t key_pressed = 0;                
                    for (int i = 0; i < KEY_SIZE; i++) {
                            if (ch8 -> key[i] != 0) {
                                ch8 -> V[x] = i;
                                break;
                            }
                    }
                    if (!key_pressed)
                        return;
                    break;
                case 0x0015:
                    ch8 -> delay = ch8 -> V[x];
                    break;
                case 0x0018:
                    ch8 -> sound = ch8 -> V[x];
                    break;
                case 0x001E:
                    ch8 -> I += ch8 -> V[x];
                    break;
                case 0x0029:
                    ch8 -> I = 5 * ch8 -> V[x];
                    break;
                case 0x0033:
                    uint8_t num = ch8 -> V[x];
                    ch8 -> memory[ch8 -> I] = num / 100;
                    ch8 -> memory[ch8 -> I + 1] = (num / 10) % 10;
                    ch8 -> memory[ch8 -> I + 2] = num % 10;
                    break;
                case 0x0055:
                    for (int i = 0; i <= x; i++) {
                        ch8 -> memory[ch8 -> I + i] = ch8 -> V[i];
                    }
                    break;
                case 0x0065:
                    for (int i = 0; i <= x; i++) {
                        ch8 -> V[i] = ch8 -> memory[ch8 -> I + i];
                    }
                    break;
            };
            ch8 -> PC += 2;
            break; 
    };
    // execute opcode
    // update timer

}



