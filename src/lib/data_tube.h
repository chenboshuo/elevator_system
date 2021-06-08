/** @file
 * @defgroup 七段数码管相关函数和变量
 * @{
 */

#ifndef DATA_TUBE_H
#define DATA_TUBE_H

#include "alias.h"

/**
 * DIGITS_LED[i] 0<=i<16 表示i的数码管的表示
 * 数码管共阳极，故1表示打开
 * code 说明指定数组存在 flash ROM 中，
 * 没有 code 会存到片内 RAM 中
 */
unsigned char code DIGITS_LED[] = {
    //         dp g f e; d c b a
    0xC0,  // 0: 1 1 0 0; 0 0 0 0
    0xF9,  // 1: 1 1 1 1; 1 0 0 1
    0xA4,  // 2: 1 0 1 0; 1 1 0 0
    0xB0,  // 3: 1 0 1 1; 0 0 0 0
    0x99,  // 4: 1 0 0 1; 1 0 0 1
    0x92,  // 5: 1 0 0 1; 0 0 1 0
    0x82,  // 6: 1 0 0 0; 0 0 1 0
    0xF8,  // 7: 1 1 1 1; 1 0 0 0
    0x80,  // 8: 1 0 0 0; 0 0 0 0
    0x90,  // 9: 1 0 0 1; 0 0 0 0
    0x88,  // A: 1 0 0 0; 1 0 0 0
    0x83,  // B: 1 0 0 0; 0 0 1 1
    0xC6,  // C: 1 1 0 0; 0 1 1 0
    0xA1,  // D: 1 0 1 0; 0 0 0 1
    0x86,  // E: 1 0 0 0; 0 1 1 0
    0x8E   // F: 1 0 0 0; 1 1 1 0
};

/**
 * 将code代表的灯点亮
 * @param digit_tube_id 管子的id 0-5 表示 LEDS0-LEDS5
 *                      也可以直接输入寄存器P1 的取值
 * @param code 按七段码的8bit数 ai = 0 表示第i位打开
 */
void open_data_tube(unsigned int digit_tube_id, unsigned int data_code) {
  // 8 指 ADDR3 = 1
  ADDRESS = digit_tube_id | 0x08;
  DATA = CLOSE_ALL;  // 刷新之前关闭所有数码管防止残影
  DATA = data_code;
}

///@}

#endif
