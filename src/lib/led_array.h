/** @file
 * @defgroup led点阵相关
 * 在该部分，列号由左到右从零开始
 * 对于八位二进制数，最左边为最低位
 * @{
 */

#ifndef LED_ARRAY_H
#define LED_ARRAY_H

#include "alias.h"

#define ARRY_ROW_SIZE 8  //!< 该单片机共8行
#define ARRY_COL_SIZE 8  //!< 该单片机共8列

#define ARRAY_LINE_VALID_BITS 0x07  /// 范围0-7

/// 用来存储单片机的一些状态
unsigned char base_image[] = {
    CLOSE_ALL, CLOSE_ALL, CLOSE_ALL, CLOSE_ALL,
    CLOSE_ALL, CLOSE_ALL, CLOSE_ALL, CLOSE_ALL,
};

/** 将mask所代表的内容放到row_id
 * 注意地址最左边为mask 最低位
 * @param row_id 行号
 * @param code   对应二进制信息
 */
void show_in_array(int row_id, int image_code) {
  DATA = CLOSE_ALL;
  ADDRESS = row_id;
  DATA = image_code;
}

/**
 * 闪烁一次之后固定常亮
 * @param line_id 行号
 * @param col_id  列号
 * @param clock   时钟，400的时候闪烁，要求>=0
 * 800之后将该元素加入base_map
 */
void blink_bit_and_appear(int line_id, int col_id, int clock) {
  unsigned char added_code = ~(1 << col_id);
  if ((added_code | base_image[line_id]) != 0xFF) {
    // added_code 要点亮的位为 0, base_image 对应数位如果是 0,
    // 说明函数调用之前该位置已经点亮，函数不做任何操作
    return;
  }

  if (clock < 400) {
    show_in_array(line_id, base_image[line_id] & added_code);
    return;  // 如果不到显示的周期，退出
  }
  if (clock == 800) {
    base_image[line_id] &= added_code;
  }
}

/**
 * 闪烁一次并消失
 * @param line_id 行号
 * @param col_id  列号
 * @param clock   时钟，在函数中400-800期间显示，要求>=0
 */
void blink_bit_and_disappear(int line_id, int col_id, int clock) {
  int deleted_code = (1 << col_id);
  if ((base_image[line_id] & deleted_code) != 0) {
    // 要熄灭的位为 1, 若 base_map 的对应位置为 1,
    // 说明灯已经熄灭，不做任何操作
    return;
  }
  base_image[line_id] |= deleted_code;

  if (clock > 400 && clock < 800) {
    show_in_array(line_id, base_image[line_id] & (~deleted_code));
  }
}

///@}

#endif
