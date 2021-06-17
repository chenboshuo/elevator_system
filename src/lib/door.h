/** @file
 * 开关门控制系统的定义
 * @defgroup 开关门控制系统
 * 控制开关门的动作模拟
 * @{
 */

#ifndef DOOR_H
#define DOOR_H
// #include "direction.h"
#include "light.h"

#define LEFT_DOOR 4   //!< 左边的们从led灯的第4位开始
#define RIGHT_DOOR 0  //!< 右边的们从led灯的第0位开始

#define OPEN_MOMENT_1 1600  //!< 时钟为1600 进入开门第1阶段
#define OPEN_MOMENT_2 1700  //!< 时钟为1700 进入开门第2阶段
#define OPEN_MOMENT_3 1800  //!< 时钟为1800 进入开门第3阶段
#define OPEN_MOMENT_4 1900  //!< 时钟为1900 进入开门第4阶段

#define READY_TO_CLOSE_MOMENT 4500  // !< 作为

#define CLOSE_MOMENT_1 5000  //!< 时钟2500 进入关门第1阶段
#define CLOSE_MOMENT_2 5100  //!< 时钟2500 进入关门第2阶段
#define CLOSE_MOMENT_3 5200  //!< 时钟2500 进入关门第3阶段
#define CLOSE_MOMENT_4 5300  //!< 时钟2500 进入关门第4阶段

#define READY_MOMENT 5500  //!< 时钟2900 时关闭所有时钟准备运行

/// 获得选定的部分
#define get_selected_mask(loc) ((loc == RIGHT_DOOR) ? 0x0F : 0xF0)

unsigned char door_status = CLOSE_ALL;  //!< 记录们的状态

void open_door(unsigned char right_bit_loc) {
  unsigned char mask = get_selected_mask(right_bit_loc);
  unsigned char selected_part = (door_status & mask) >> right_bit_loc;
  door_status =
      (((selected_part << 1) & 0x0F) << right_bit_loc) | (door_status & ~mask);
}

void close_door(unsigned char right_bit_loc) {
  unsigned char mask = get_selected_mask(right_bit_loc);
  unsigned char selected_part = (door_status & mask) >> right_bit_loc;
  door_status = ((((selected_part | 0xF0)  // 高位补1
                   >> 1) &
                  0x0F)             // 只保留低4位
                 << right_bit_loc)  // 送到对应位置
                | (door_status & ~mask);
}

#endif
