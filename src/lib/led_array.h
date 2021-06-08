/** @file
 * @defgroup led点阵相关
 * 在该部分，列号由左到右从零开始
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

void blink_bit_and_appear(int line_id, int col_id, int clock) {
  // int added_code = ~(1 << (ARRY_COL_SIZE - 1 - col_id));
  int added_code = ~(1 << col_id);

  if (clock < 400) {
    show_in_array(line_id, base_image[line_id] & added_code);
    return;  // 如果不到显示的周期，退出
  }
  if (clock == 800) {
    base_image[line_id] &= added_code;
  }
}

///@}

#endif
