/** @file
 * 存储与按键有关的函数
 * TODO ：col_id,line_id 从0 开始
 * @defgroup  与按键有关的定义和函数
 * @{
 */

#ifndef KEY_H
#define KEY_H

#include "system_header.h"

#define KEY_EVENT P2  /// P2 用于处理键盘事件

#define KEY_PRESSED 0          /// 表示按键被按下
#define KEY_KEEP_PRESSED 0x00  /// 四次buffer记录都记录到按键按下

#define KEY_RELEASED 1          /// 1 表示按键被释放
#define KEY_KEEP_RELEASED 0x0F  /// buffer中记录到四次按键释放状态

#define KEY_LINE_SIZE 4  /// 按键共四行
#define KEY_COL_SIZE 4   /// 按键共四列

/**
 * 键盘按键的缓存，记录按键之前的状态
 */
unsigned char key_buffer[KEY_LINE_SIZE][KEY_COL_SIZE] = {
    {0xFF, 0xFF, 0xFF, 0xFF},
    {0xFF, 0xFF, 0xFF, 0xFF},
    {0xFF, 0xFF, 0xFF, 0xFF},
    {0xFF, 0xFF, 0xFF, 0xFF},
};

/// 记录按键状态
static unsigned int key_status[KEY_LINE_SIZE][KEY_COL_SIZE] = {
    {KEY_RELEASED, KEY_RELEASED, KEY_RELEASED, KEY_RELEASED},
    {KEY_RELEASED, KEY_RELEASED, KEY_RELEASED, KEY_RELEASED},
    {KEY_RELEASED, KEY_RELEASED, KEY_RELEASED, KEY_RELEASED},
    {KEY_RELEASED, KEY_RELEASED, KEY_RELEASED, KEY_RELEASED},
};

// 记录按键之前的状态
static unsigned int key_pre_status[KEY_LINE_SIZE][KEY_COL_SIZE] = {
    {KEY_RELEASED, KEY_RELEASED, KEY_RELEASED, KEY_RELEASED},
    {KEY_RELEASED, KEY_RELEASED, KEY_RELEASED, KEY_RELEASED},
    {KEY_RELEASED, KEY_RELEASED, KEY_RELEASED, KEY_RELEASED},
    {KEY_RELEASED, KEY_RELEASED, KEY_RELEASED, KEY_RELEASED},
};

/**
 * 设置要检测的行
 * @param line_id 行号
 * | line_id | keyout  | P2   |
 * |:------- |:------- | ---- |
 * | 0       | keyOut1 | P2.3 |
 * | 1       | keyOut2 | P2.2 |
 * | 2       | keyOut3 | P2.1 |
 * | 3       | keyOut4 | P2.0 |
 */
void set_listener_of_line(int line_id) {
  KEY_EVENT = ~(1 << (3 - line_id));  // 低四位用来设置行线的初值
}

/**
 * 检测按键状态
 * 一下函数均需要提前调用该函数来检测
 * @param  col_id 列号
 * @return        返回按键状态
 */
unsigned int get_key_status_of_col(int col_id) {
  unsigned int bit_loc = 4 + col_id;
  unsigned int mask = 1 << bit_loc;
  // return (KEY_EVENT & mask) >> bit_loc;
  return (KEY_EVENT & mask) >> bit_loc;
}

/**
 * 更新缓存，将当前按键的状态写入buffer
 *
 * 旧的状态左移，留出最低位保存新状态
 * 用最低位表示最新状态
 * 最终结果只保留低四位
 */
void update_key_buffer(int line_id, int col_id) {
  key_buffer[line_id][col_id] =
      ((key_buffer[line_id][col_id] << 1) | get_key_status_of_col(col_id)) &
      0x0F;
}

/**
 * 检测按键是否刚刚按下
 * @param  line_id 行号
 * @param  col_id  列号
 * @return         按下为1否则为0
 */
unsigned int is_just_pressed(int line_id, int col_id) {
  // 如果没有变化，就不是刚刚按下
  if (key_pre_status[line_id][col_id] == key_status[line_id][col_id]) {
    return 0;
  }
  unsigned int old_state = key_pre_status[line_id][col_id];
  key_pre_status[line_id][col_id] = key_status[line_id][col_id];
  // 如果前一个状态没有按下，就认为现在刚刚按下这个键
  return (old_state != KEY_PRESSED);
}

/**
 * 连续四次扫描状态相同，则更新当前状态
 * @param line_id 行号
 * @param col_id  列号
 */
void update_key_status(int line_id, int col_id) {
  if (key_buffer[line_id][col_id] == KEY_KEEP_PRESSED) {
    key_status[line_id][col_id] = KEY_PRESSED;
  } else if (key_buffer[line_id][col_id] == KEY_KEEP_RELEASED) {
    key_status[line_id][col_id] = KEY_RELEASED;
  }
}

///@}

#endif
