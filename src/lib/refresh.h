/** @file
 * @defgroup  刷新显示的相关函数
 * @{
 */

#ifndef REFRESH_H
#define REFRESH_H

#include "alias.h"
#include "dispatch_system.h"
#include "door.h"
#include "key.h"
#include "led_array.h"
#include "requests.h"
#include "seven-segment_display.h"

#define FLASH_UNIT_SIZE 11  //!< 要刷新的单元数目

#define ELEVATOR_START_MOMENT 0           //!< 电梯启动的时间戳
#define ELEVATOR_BEGIN_WAITING_TIME 1500  //!< 电梯进行楼层等待的时间段

#define LEFT_CLOCK_SIZE 6000         //!< 定义左边电梯时钟大小
static unsigned int left_clock = 0;  //!< 左电梯的时钟

/**
 * 更新显示的模块
 * 将所有要显示的单元统一刷新
 */
void refresh_module() {
  static unsigned char unit_turn = 0;  /// 选择模块刷新
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
      open_data_tube(5, DIGITS_LED[left_elevator.current_floor + 1]);
      break;
    case 10:
      open_lights(door_status);
      // open_data_tube(0, DIGITS_LED[right_elevator.current_floor + 1]);
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
  static unsigned char ms_count = 0;
  unsigned char key_col;

  static unsigned char blink_clock = 0;

  if (ms_count == 150) {
    ++blink_clock;
    update_key_clocks();
    ms_count = 0;
  }

  // 将一行按键移动到缓冲区
  for (key_col = 0; key_col < KEY_COL_SIZE; ++key_col) {
    // TODO debug
    update_key_buffer(key_line, key_col);  // 将按键状态移动到缓冲区
    update_key_status(key_line, key_col);  // 更新按键状态
  }

  // 刷新键盘并响应
  switch (key_line) {
    case 1:  // 第二行按键作为左边电梯内目标楼层的标识
      if (is_just_pressed(1, 3)) {  // 一行三列作为开门键
        if (left_clock > ELEVATOR_BEGIN_WAITING_TIME &&
            left_clock < READY_MOMENT) {  // 电梯处于等待运行状态,响应打开
          bit_appear(2, 1);
          left_clock = OPEN_MOMENT_1;
        }
      } else if (is_just_released(1, 3)) {
        bit_disappear(2, 1);
      }

    // 注意以上是额外判断，没有break，第二行前半部分也是楼层响应
    case 0:  // 第一行按键作为左边电梯内目标楼层的标识
      for (key_col = 0; key_col < 3; ++key_col) {
        if (is_just_pressed(key_line, key_col) &&
            !has_requested[key_line][2 - key_col]) {
          key_clocks[key_line][key_col] = BLINK_PROCESSING_CLOCK;
          has_requested[key_line][key_col] = TRUE;  // 将请求发送到对应的电梯
        }
        blink_bit_and_appear(2 - key_col, key_line * 7,
                             key_clocks[key_line][key_col]);
      }
      break;

    case 2:  // 第三行表明 1上，2上，3下
      // 一层楼发送上行请求
      if (!has_called[UP_CALL][FIRST_FLOOR] && is_just_pressed(2, 0)) {
        key_clocks[2][0] = BLINK_PROCESSING_CLOCK;
        has_called[UP_CALL][FIRST_FLOOR] = TRUE;
      }
      blink_bit_and_appear(0, 2, key_clocks[2][0]);

      // 二层发送上行请求
      if (!has_called[UP_CALL][SECOND_FLOOR] && is_just_pressed(2, 1)) {
        key_clocks[2][1] = BLINK_PROCESSING_CLOCK;
        has_called[UP_CALL][SECOND_FLOOR] = TRUE;
      }
      blink_bit_and_appear(0, 3, key_clocks[2][1]);

      // 三层发送下行请求
      if (!has_called[DOWN_CALL][THIRD_FLOOR] && is_just_pressed(2, 2)) {
        key_clocks[2][2] = BLINK_PROCESSING_CLOCK;
        has_called[DOWN_CALL][THIRD_FLOOR] = TRUE;
      }
      blink_bit_and_appear(1, 4, key_clocks[2][2]);
      break;

    case 3:  // 第四行表明 2下
      // 按键按下，若无请求，二层发送下行请求
      if (!has_called[DOWN_CALL][SECOND_FLOOR] && is_just_pressed(3, 0)) {
        key_clocks[3][0] = BLINK_PROCESSING_CLOCK;
        has_called[DOWN_CALL][SECOND_FLOOR] = TRUE;
      }
      blink_bit_and_appear(1, 3, key_clocks[3][0]);

      // 响应关门请求，三行三列表示关门
      if (is_just_pressed(3, 3)) {
        bit_appear(2, 2);
        if (left_clock > OPEN_MOMENT_4 && left_clock <= READY_TO_CLOSE_MOMENT) {
          left_clock = READY_TO_CLOSE_MOMENT;
        }
      } else if (is_just_released(3, 3)) {
        bit_disappear(2, 2);
      }
  }

  // 更新linster
  ++key_line;  // 下一次刷新下一行
  key_line &= 0x03;
  set_listener_of_line(key_line);

  // 更新闪烁寄存器
  ++ms_count;
}

void refresh_left_elevator() {
  static __bit need_responce = FALSE;
  if (left_clock == ELEVATOR_START_MOMENT) {  // 启动电梯
    get_direction(&left_elevator);
    show_direction(&left_elevator);                        // 显示运行方向
  } else if (left_clock == ELEVATOR_BEGIN_WAITING_TIME) {  // 到达楼层
    arrive(&left_elevator);
  } else if (left_clock > ELEVATOR_BEGIN_WAITING_TIME) {  // 在楼层等待
    need_responce |= close_calls(&left_elevator);
    need_responce |= close_requests(&left_elevator);
    if (need_responce) {
      switch (left_clock) {
        case OPEN_MOMENT_1:
        case OPEN_MOMENT_2:
        case OPEN_MOMENT_3:
        case OPEN_MOMENT_4:
          open_door(4);
          break;
        case CLOSE_MOMENT_1:
        case CLOSE_MOMENT_2:
        case CLOSE_MOMENT_3:
        case CLOSE_MOMENT_4:
          close_door(4);
          break;
        case READY_MOMENT:
          need_responce = FALSE;
      }
    } else {
      left_clock = ELEVATOR_START_MOMENT - 1;
    }
  }
  ++left_clock;
  left_clock %= LEFT_CLOCK_SIZE;
}

// void refresh_right_elevator() {
//   static unsigned int right_clock = 0;
//   switch (right_clock) {
//     case 0:
//       get_direction(&right_elevator);
//       break;
//     case 0x0FFF:
//       arrive(&right_elevator);
//   }
//   ++right_clock;
// }

///@}

#endif
