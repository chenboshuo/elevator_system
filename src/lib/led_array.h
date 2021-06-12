/** @file
 * @defgroup led点阵相关
 * 在该部分，列号由左到右从零开始
 * 同时，所有clock的计时单位为100ms
 * 对于八位二进制数，最左边为最低位
 * @{
 */

#ifndef LED_ARRAY_H
#define LED_ARRAY_H

#include "alias.h"

#define ARRY_ROW_SIZE 8  //!< 该单片机共8行
#define ARRY_COL_SIZE 8  //!< 该单片机共8列

#define ARRAY_LINE_VALID_BITS 0x07  /// 范围0-7

#define BLINK_PROCESSING_CLOCK 0xFF  /// 用于处理闪烁的时钟

/// 用来存储单片机的一些状态
unsigned char base_image[] = {
    CLOSE_ALL, CLOSE_ALL, CLOSE_ALL, CLOSE_ALL,
    CLOSE_ALL, CLOSE_ALL, CLOSE_ALL, CLOSE_ALL,
};

/// 用于按键闪烁的时钟(注意最后一列没有时钟)，每一个1表示一个时间单位
unsigned char key_clocks[4][3] = {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}};

/** 将mask所代表的内容放到row_id
 * 注意地址最左边为mask 最低位
 * @param row_id 行号
 * @param code   对应二进制信息
 */
void show_in_array(unsigned char row_id, unsigned char image_code) {
  DATA = CLOSE_ALL;
  ADDRESS = row_id;
  DATA = image_code;
}

/**
 * 闪烁一次之后固定常亮
 * @param line_id 行号
 * @param col_id  列号
 * @param clock   时钟，需要在其他函数做处理，访问函数一次右移一位
 */
void blink_bit_and_appear(int line_id, int col_id, unsigned char clock) {
  unsigned char added_code = ~(1 << col_id);
  // if ((added_code | base_image[line_id]) != 0xFF) {
  //   // added_code 要点亮的位为 0, base_image 对应数位如果是 0,
  //   // 说明函数调用之前该位置已经点亮，函数不做任何操作
  //   return;
  // }
  switch (clock) {
    case 0xFF:
    case 1:
      base_image[line_id] &= added_code;
      break;
    case 0x0F:
      base_image[line_id] |= (~added_code);
  }
}

/**
 * 闪烁一次并消失
 * @param line_id 行号
 * @param col_id  列号
 * @param clock   时钟，需要在其他函数做处理，访问函数一次右移一位
 */
void blink_bit_and_disappear(int line_id, int col_id, unsigned char clock) {
  int deleted_code = (1 << col_id);
  switch (clock) {
    case 0xFF:
    case 1:
      base_image[line_id] |= deleted_code;
      break;
    case 0x0F:
      base_image[line_id] &= (~deleted_code);
  }
}

/**
 * 更新按键时钟，
 * 更新时将数值右移一位
 */
void update_key_clocks() {
  unsigned char i, j;
  for (i = 0; i < 4; ++i) {
    for (j = 0; j < 3; ++j) {
      key_clocks[i][j] >>= 1;
    }
  }
}

///@}

#endif
