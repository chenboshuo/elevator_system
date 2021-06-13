/** @file
 * @defgroup 方向控制模块
 * 该模块模拟电梯运行方向
 * @{
 */

#ifndef DIRECTION_H
#define DIRECTION_H

// #include "elevator.h"
#include "led_array.h"

#define ARROW_LINE_SIZE 5  /// 表示方向信息的行数是5

#define LEFT_SYMBOL 0   /// 0 表明将方向标识放到左半部分
#define RIGHT_SYMBOL 4  /// 4 表明将方向标识放到右半部分

#define SELECT_LEFT_PART 0x0F   /// 选择ARROW_SYMBOL 的左半部分
#define SELECT_RIGHT_PART 0xF0  /// 选择ARROW_SYMBOL 的左半部分

#define UP 1            /// 上行记为 1
#define NO_DIRECTION 0  /// 静止记为 0
#define DOWN -1         /// 下行记为 -1

/// 上下箭头
static unsigned char code ARROW_SYMBOL[] = {
    0x5b,  // ..#..#.#
    0xb5,  // .#.#..#.
    0xff,  // ........
    0x5b,  // ..#..#.#
    0xb5   // .#.#..#.
};

/**
 * 向上移动的方向标识
 * @param begin_bit_loc 位置 LEFT_SYMBOL 或 RIGHT_SYMBOL
 */
void move_up(unsigned char begin_bit_loc) {
  unsigned char i;
  unsigned char clean_mask;
  if (begin_bit_loc == LEFT_SYMBOL) {
    clean_mask = 0x0F;  // 将图片放在左边需要清除左边部分(低位)
  } else {
    clean_mask = 0xF0;
  }

  ALLOW_INTERRUPT = FALSE;  // 关键步骤，禁止中断
  for (i = 0; i < ARROW_LINE_SIZE; ++i) {
    base_image[i + 3] |= clean_mask;
    base_image[i + 3] &=
        ((ARROW_SYMBOL[i] &
          SELECT_LEFT_PART)  // 选择ARROW_SYMBOL上箭头部分(低位)
         << begin_bit_loc)   // 移动到对应位置
        | ~clean_mask        // 去掉另外半部分
        ;
  }
  ALLOW_INTERRUPT = TRUE;  // 恢复中断
}

/**
 * 向下移动的方向标识
 * @param begin_bit_loc 位置 LEFT_SYMBOL 或 RIGHT_SYMBOL
 */
void move_dowm(unsigned char begin_bit_loc) {
  unsigned char i;
  unsigned char clean_mask;
  if (begin_bit_loc == LEFT_SYMBOL) {
    clean_mask = 0x0F;  // 将图片放在左边需要清除左边部分(低位)
  } else {
    clean_mask = 0xF0;
  }

  ALLOW_INTERRUPT = FALSE;  // 关键步骤，禁止中断
  for (i = 0; i < ARROW_LINE_SIZE; ++i) {
    base_image[i + 3] |= clean_mask;
    base_image[i + 3] &=
        ((((ARROW_SYMBOL[i] &
            SELECT_RIGHT_PART)  // 选择ARROW_SYMBOL上箭头部分(低位)
           >> 4)                // 右移到低位
          << begin_bit_loc)     // 移动到对应位置
         | ~clean_mask);        // 去掉另外半部分
  }
  ALLOW_INTERRUPT = TRUE;  // 恢复中断
}

#endif
