/** @file
 * @defgroup led点阵相关
 * @{
 */

#ifndef LED_ARRAY_H
#define LED_ARRAY_H

#include "alias.h"

#define ROW_SIZE 8  //!< 该单片机共8行
/** 将mask所代表的内容放到row_id
 * 注意地址最左边为mask 最低位
 * @param row_id 行号
 * @param mask   对应二进制信息
 */
void show_in_array(int row_id, int mask) {
  DATA = CLOSE_ALL;
  ADDRESS = row_id;
  DATA = mask;
}

///@}

#endif
