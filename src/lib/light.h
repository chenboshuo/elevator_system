/** @file
 * @defgroup  计数器控制器与中断
 * @{
 */

#ifndef LIGHT_H
#define LIGHT_H

#include "alias.h"

#define ENABLE_LED_ARRAYS 14  //!< 01110 表示右边LED单独的led灯
/**
 * 打开light_code 代表的led灯
 * @param light_code 代表LED 灯的编码 如 1111 1110 表示最右边的灯
 */
void open_lights(int light_code) {
  DATA = CLOSE_ALL;
  ADDRESS = ENABLE_LED_ARRAYS;
  DATA = light_code;
}

///@}

#endif
