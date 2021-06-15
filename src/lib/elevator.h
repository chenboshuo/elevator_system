/** @file
 * @defgroup  电梯对应的数据结构与函数
 * @{
 */

#ifndef ELEVATOR_H
#define ELEVATOR_H

#include "direction.h"
#include "requests.h"
#include "system_header.h"

// #define NO_TARGET 0x00  /// NO_TARGET 表明没有楼层要访问

#define LEFT_ELEVATOR 0   //!< 0 代表左边机位
#define RIGHT_ELEVATOR 1  //!< 1 代表右边机位

/// 符号函数，计算(a-b)的符号
#define sign(a, b) ((a - b == 0) ? 0 : ((a - b) > 0 ? 1 : -1))  // 计算a-b的符号

/// 将电梯运行方向(1,-1)换算成请求方向(0,1)
#define get_calling_direction(elevator_direction) \
  ((elevator_direction == UP) ? UP_CALL : DOWN_CALL)

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

/**
 * 关闭电梯外的呼叫请求，并关闭对应位置指示灯
 * @param elevator 电梯
 */
void close_calls(struct Elevator* elevator) {
  unsigned char calling_direction =
      (elevator->direction == UP) ? UP_CALL : DOWN_CALL;

  has_called[calling_direction][elevator->current_floor] = FALSE;
  switch (elevator->current_floor) {
    case FIRST_FLOOR:
      bit_disappear(0, 2);
      key_clocks[2][0] = 0;
      break;
    case SECOND_FLOOR:
      bit_disappear(calling_direction, 3);
      key_clocks[2 + calling_direction][1] = FALSE;
      break;
    case THIRD_FLOOR:
      bit_disappear(1, 4);
      key_clocks[2][2] = FALSE;
      break;
  }
}

/**
 * 处理电梯到达之后的程序
 * @param elevator 电梯
 */
void arrive(struct Elevator* elevator) {
  // 到达新楼层
  elevator->current_floor = elevator->current_floor + elevator->direction;

  // 关闭方向标识静止
  clear_direction_lamp(elevator->direction_lamp_loc);

  // 清除电梯内请求,关闭对应位置的灯
  // ALLOW_INTERRUPT = FALSE;
  has_requested[elevator->id][elevator->current_floor] = FALSE;
  bit_disappear(2 - elevator->current_floor, elevator->id * 7);
  key_clocks[elevator->id][elevator->current_floor] =
      0;  // 清空按键计时，防止闪烁

  // 关闭电梯外呼叫请求
  close_calls(elevator);
}

/**
 * 显示电梯运行方向
 * @param elevator 电梯
 */
void show_direction(struct Elevator* elevator) {
  if (elevator->direction == DOWN) {
    move_dowm(elevator->direction_lamp_loc);
  } else if (elevator->direction == UP) {
    move_up(elevator->direction_lamp_loc);
  } else {
    clear_direction_lamp(elevator->direction_lamp_loc);
  }
}

/**
 * 获得电梯运行方向
 * @param elevator 电梯
 */
void get_direction(struct Elevator* elevator) {
  unsigned char target_floor;
  // 没有运行方向则选择运行方向
  if (elevator->direction == NO_DIRECTION) {
    // 若电梯内还有请求
    if (has_any_requests(elevator->id)) {
      // 寻找目标楼层
      for (target_floor = FIRST_FLOOR;
           !has_requested[elevator->id][target_floor]; ++target_floor) {
        ;
      }
      // 找到之后计算方向
      elevator->direction = sign(target_floor, elevator->current_floor);
    }
  } else {  // 方向不为空
    // 向运行方向寻找电梯内外请求
    for (target_floor = elevator->current_floor;
         target_floor != 0xFF && target_floor < 3 &&
         !has_requested[elevator->id][target_floor] &&
         !has_called[get_calling_direction(elevator->direction)][target_floor];
         target_floor += elevator->direction) {
      ;
    }

    // 若目标楼层出范围，说明没有请求，我们消除运行方向
    if (target_floor == 0xFF || target_floor == 3) {
      elevator->direction = NO_DIRECTION;
    }
  }

  // 显示运行方向
  show_direction(elevator);
}

///@}

#endif
