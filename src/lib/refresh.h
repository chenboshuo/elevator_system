/** @file
 * @defgroup  刷新显示的相关函数
 * @{
 */

#ifndef REFRESH_H
#define REFRESH_H

#include "alias.h"
#include "data_tube.h"
#include "key.h"
#include "led_array.h"

#define FLASH_UNIT_SIZE 11  /// 要刷新的单元数目

/**
 * 更新显示的模块
 * 将所有要显示的单元统一刷新
 */
void refresh_module() {
  static int unit_turn = 0;  /// 选择模块刷新
  switch (unit_turn) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:  // 刷新底图
      show_in_array(unit_turn, base_image[unit_turn]);
      break;
    case 9:
      open_data_tube(0, DIGITS_LED[1]);
      break;
    case 10:
      open_data_tube(5, DIGITS_LED[1]);
      break;
  }
  ++unit_turn;
  unit_turn %= FLASH_UNIT_SIZE;
}

/**
 * 刷新键盘监听器和键盘事件响应
 */
void refresh_key_line() {
  static unsigned char key_line = 0;
  unsigned char key_col;

  static unsigned char ms_count = 0;
  static unsigned char blink_clock = 0;

  if (ms_count == 100) {
    ++blink_clock;
    ms_count = 0;
  }

  // 将一行按键移动到缓冲区
  for (key_col = 0; key_col < KEY_COL_SIZE; ++key_col) {
    update_key_buffer(key_line, key_col);  // 将按键状态移动到缓冲区
    update_key_status(key_line, key_col);  // 更新按键状态
  }

  // 刷新键盘并响应
  switch (key_line) {
    case 0:  // 第一行按键作为左边电梯内目标楼层的标识
      for (key_col = 0; key_col < 3; ++key_col) {
        if (key_clocks[key_line][key_col] == 0 &&
            is_just_pressed(key_line, key_col)) {
          key_clocks[key_line][key_col] = blink_clock;
        }
        if (key_clocks[key_line][key_col] != 0) {
          blink_bit_and_appear(2 - key_col, 0,
                               blink_clock - key_clocks[key_line][key_col]);
        }
      }
      break;
    case 1:  // 第二行按键作为左边电梯内目标楼层的标识
      for (key_col = 0; key_col < 3; ++key_col) {
        if (key_clocks[key_line][key_col] == 0 &&
            is_just_pressed(key_line, key_col)) {
          key_clocks[key_line][key_col] = blink_clock;
        }
        if (key_clocks[key_line][key_col] != 0) {
          blink_bit_and_appear(2 - key_col, 7,
                               blink_clock - key_clocks[key_line][key_col]);
        }
      }
      break;
    case 2:  // 第三行表明 1上，2上，3下
      for (key_col = 0; key_col < 3; ++key_col) {
        if (key_clocks[key_line][key_col] ==
                0 &&  // 在按键按下之前保证没有闪烁过程
            is_just_pressed(key_line, key_col)) {
          key_clocks[key_line][key_col] = blink_clock;
        }
      }
      if (key_clocks[key_line][0] != 0) {
        blink_bit_and_appear(0, 2, blink_clock - key_clocks[key_line][0]);
      }
      if (key_clocks[key_line][1] != 0) {
        blink_bit_and_appear(0, 3, blink_clock - key_clocks[key_line][1]);
      }
      if (key_clocks[key_line][2] != 0) {
        blink_bit_and_appear(1, 4, blink_clock - key_clocks[key_line][2]);
      }
      break;
    case 3:  // 第四行表明 2下
      if (key_clocks[key_line][0] == 0 && is_just_pressed(key_line, 0)) {
        key_clocks[key_line][0] = blink_clock;
      }
      if (key_clocks[key_line][0] != 0) {
        blink_bit_and_appear(1, 3, blink_clock - key_clocks[key_line][0]);
      }
  }

  // 更新linster
  ++key_line;  // 下一次刷新下一行
  key_line &= 0x03;
  set_listener_of_line(key_line);

  // 更新闪烁寄存器
  ++ms_count;
}

///@}

#endif
