/** @file
 * @defgroup  电梯调度相关的函数
 * @{
 */

#ifndef DISPATCH_SYSTEM_H
#define DISPATCH_SYSTEM_H

#include "direction.h"
#include "door.h"
#include "elevator.h"
#include "requests.h"
#include "system_header.h"

/// 符号函数，计算(a-b)的符号
#define sign(a, b) ((a - b == 0) ? 0 : ((a - b) > 0 ? 1 : -1))  // 计算a-b的符号

#define UNDERFLOW 0xFF  //!< 楼层(unsigned char 类型)出范围，向下溢出时为0xFF

/**
 * 关闭电梯外的呼叫请求，并关闭对应位置指示灯
 * @param elevator 电梯
 * @return 是否关闭了呼叫请求
 */
__bit close_calls(struct Elevator* elevator) {
  unsigned char calling_direction =
      (elevator->direction == DOWN)
          ? DOWN_CALL
          : UP_CALL;  // 一般一楼无方向，我们也认为向上
  __bit has_closed_calls =
      has_called[calling_direction][elevator->current_floor];

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
  return has_closed_calls;
}

/**
 * 关闭电梯内目标楼层请求
 * @param elevator 电梯
 * @return 是否关闭了请求
 */
__bit close_requests(struct Elevator* elevator) {
  __bit has_closed_request =
      has_requested[elevator->id][elevator->current_floor];
  has_requested[elevator->id][elevator->current_floor] = FALSE;
  bit_disappear(2 - elevator->current_floor, elevator->id * 7);
  key_clocks[elevator->id][elevator->current_floor] =
      0;  // 清空按键计时，防止闪烁

  return has_closed_request;
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
  unsigned char calling_direction = get_calling_direction(elevator->direction);
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
      // elevator->direction = sign(target_floor, elevator->current_floor);
    } else if (has_any_calls()) {
      // 找同方向请求

      // 寻找目标楼层
      for (target_floor = FIRST_FLOOR; !has_called[UP_CALL][target_floor] &&
                                       !has_called[DOWN_CALL][target_floor];
           ++target_floor) {
        ;
      }

      // 若目标就在当前楼层，关闭请求
      if (target_floor == elevator->current_floor) {
        elevator->direction = UP;
        close_calls(elevator);
        elevator->direction = DOWN;
        close_calls(elevator);
        elevator->direction = NO_DIRECTION;
      }
    } else {  // 无任何请求，归位
      switch (elevator->id) {
        case LEFT_ELEVATOR:
          target_floor = FIRST_FLOOR;
          break;
        case RIGHT_ELEVATOR:
          target_floor = THIRD_FLOOR;
          break;
      }
    }
    elevator->direction = sign(target_floor, elevator->current_floor);

  } else {  // 方向不为空
    // 向运行方向寻找电梯内外请求
    for (target_floor = elevator->current_floor + elevator->direction;
         target_floor != UNDERFLOW && target_floor < 3 &&
         !has_requested[elevator->id][target_floor] &&
         !has_called[UP_CALL][target_floor] &&
         !has_called[DOWN_CALL][target_floor];
         target_floor += elevator->direction) {
      ;
    }
    // if (target_floor == 0xFF) {
    //   open_lights(0xEE);
    // }

    // 若目标楼层出范围，说明没有请求，我们消除运行方向
    if (target_floor == UNDERFLOW || target_floor == 3) {
      elevator->direction = NO_DIRECTION;
      // elevator->clock = ELEVATOR_BEGIN_WAITING_TIME;
    }
  }
}

void update_elevator_status(struct Elevator* elevator) {
  static __bit need_responce = FALSE;
  if (elevator->clock == ELEVATOR_START_MOMENT) {  // 启动电梯
    get_direction(elevator);
    show_direction(elevator);  // 显示运行方向
  } else if (elevator->clock == ELEVATOR_BEGIN_WAITING_TIME) {  // 到达楼层
    arrive(elevator);
  } else if (elevator->clock > ELEVATOR_BEGIN_WAITING_TIME) {  // 在楼层等待
    need_responce |= close_calls(elevator);
    need_responce |= close_requests(elevator);
    if (need_responce) {
      switch (elevator->clock) {
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
      elevator->clock = ELEVATOR_START_MOMENT - 1;
    }
  }
  ++(elevator->clock);
  elevator->clock %= CLOCK_SIZE;
}

///@}

#endif
