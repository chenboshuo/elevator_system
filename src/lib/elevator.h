/** @file
 * @defgroup  电梯对应的数据结构与函数
 * @{
 */

#ifndef ELEVATOR_H
#define ELEVATOR_H

#include "direction.h"
#include "system_header.h"

#define NO_TARGET 0x00  /// NO_TARGET 表明没有楼层要访问

/// 电梯实体对应数据结构
struct Evevator {
  char current_level;
  char targets;    // 每一位表示一个楼层的请求
  char direction;  // 记录电梯运行方向

  char direction_lamp_loc;
  char level_lamp_loc;  // 显示楼层的数码管的位置
} left_elevator, right_elevator;

void init_left_evevator() {
  left_elevator.current_level = 1;
  left_elevator.targets = NO_TARGET;  // 每一位表示一个楼层的请求
  left_elevator.direction = 0;        // 记录电梯运行方向

  left_elevator.direction_lamp_loc = LEFT_SYMBOL;
  left_elevator.level_lamp_loc = 5;
}

void init_right_evevator() {
  right_elevator.current_level = 1;
  right_elevator.targets = NO_TARGET;  // 每一位表示一个楼层的请求
  right_elevator.direction = 0;        // 记录电梯运行方向

  right_elevator.direction_lamp_loc = RIGHT_SYMBOL;
  right_elevator.level_lamp_loc = 0;
}

///@}

#endif
