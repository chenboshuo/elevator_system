/** @file
 * @defgroup  电梯对应的数据结构
 * @{
 */

#ifndef ELEVATOR_H
#define ELEVATOR_H

#include "direction.h"
#include "requests.h"
#include "system_header.h"

#define LEFT_ELEVATOR 0   //!< 0 代表左边机位
#define RIGHT_ELEVATOR 1  //!< 1 代表右边机位

/// 电梯实体对应数据结构 注意current_floor 0 代表一楼
struct Elevator {
  unsigned char current_floor;
  unsigned char id;
  int direction;  // 记录电梯运行方向

  unsigned char direction_lamp_loc;
  // unsigned char level_lamp_loc;  // 显示楼层的数码管的位置
  // } left_elevator, right_elevator;
} left_elevator;

/**
 * 初始化左电梯，默认在一楼
 */
void init_left_evevator() {
  left_elevator.current_floor = FIRST_FLOOR;
  // left_elevator.targets = NO_TARGET;  // 每一位表示一个楼层的请求
  left_elevator.direction = NO_DIRECTION;  // 记录电梯运行方向
  left_elevator.id = LEFT_ELEVATOR;
  left_elevator.direction_lamp_loc = LEFT_SYMBOL;
  // left_elevator.level_lamp_loc = 5;
}

// /**
//  * 初始化右边电梯，默认在三楼
//  */
// void init_right_evevator() {
//   right_elevator.current_floor = THIRD_FLOOR;
//   // right_elevator.targets = NO_TARGET;  // 每一位表示一个楼层的请求
//   right_elevator.direction = NO_DIRECTION;  // 记录电梯运行方向
//   right_elevator.id = RIGHT_ELEVATOR;
//
//   right_elevator.direction_lamp_loc = RIGHT_SYMBOL;
//   // right_elevator.level_lamp_loc = 0;
// }

///@}

#endif
