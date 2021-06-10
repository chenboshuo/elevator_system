/** @file
 * 主函数，完成电梯模块
 */
#include "lib/alias.h"
#include "lib/data_tube.h"
#include "lib/elevator.h"
#include "lib/interrupt.h"
#include "lib/key.h"
#include "lib/light.h"
// #include "lib/requests.h"
#include "lib/system_header.h"

#define FLASH_MODULE_SIZE 11  /// 要刷新的魔块数目

void main() {
  // 初始化电梯
  // init_left_evevator();
  // init_right_evevator();

  // 计时一秒
  enable_timer_t0_with_interrupt();
  TIME_1MS();

  while (TRUE) {
    ;
  }
}

void flash_and_update() interrupt T0_OVERFLOW {
  static unsigned char key_line = 0;  ///
  static int module_choice = 0;       /// 选择模块刷新
  static unsigned char ms_count = 0;
  static unsigned char blink_clock = 0;
  static char array_line = 0;
  char key_col;

  if (ms_count == 100) {
    ++blink_clock;
    ms_count = 0;
  }

  // 更新计数器初值
  TIME_1MS();

  // 将一行按键移动到缓冲区
  for (key_col = 0; key_col < KEY_COL_SIZE; ++key_col) {
    update_key_buffer(key_line, key_col);  // 将按键状态移动到缓冲区
    update_key_status(key_line, key_col);  // 更新按键状态
  }

  // 刷新键盘
  // response_the_key(key_line, blink_clock);
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

  // show_in_array(array_line, base_image[array_line]);
  // ++array_line;
  // array_line %= ARRY_ROW_SIZE;

  switch (module_choice) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:  // 刷新底图
      show_in_array(module_choice, base_image[module_choice]);
      break;
    case 9:
      open_data_tube(0, DIGITS_LED[1]);
      break;
    case 10:
      open_data_tube(5, DIGITS_LED[1]);
      break;
  }
  ++module_choice;
  module_choice %= FLASH_MODULE_SIZE;

  // 更新linster
  ++key_line;  // 下一次刷新下一行
  key_line &= 0x03;
  set_listener_of_line(key_line);

  // 更新闪烁寄存器
  ++ms_count;
}
