/** @file
 * @defgroup  一些小功能函数
 * @{
 */

#ifndef UTILITY_H
#define UTILITY_H

#include "alias.h"
#include "system_header.h"
/** 大约计时1s
 * 忽略访问内存时间
 * 采用T1定时 50ms
 * 然后循环 20次，计时大约1s
 */
void keep_one_second() {
  char count = 0;  // 记录T0中断次数
  TMOD = 0x10;  // 设置T1为模式1 TMOD 高四位控制T0 GATE=0 T/C = 0 (M1 M0) = 1
                // 工作方式0
  // 为T0赋初值0xB800, 根据主频计算计时20ms的初值(样例程序给出)
  // TH1 = 0xB8;
  // TL1 = 0x00;

  // 为T0赋初值0xFC67, 根据主频计算计时50ms的初值
  TH1 = 0xFC;
  TL1 = 0x67;
  TR1 = ENABLE;  // 启动T1

  while (count < 20) {
    if (TF1 == OVERFLOW) {  // T1溢出
      TF1 = NOT_OVERFLOW;   // T1溢出后，清零中断标志
      T1 = 0x4c00;          // 0x4c00
      // 以下是计时20ms 的初值，t20_two = 0xB800
      // t20 = 2^16 - t20_two
      // t50 = t20/2 * 5
      // t50_two = 2^16 - t50 = 19456
      // TH0 = 0xB8;  // 重新赋初值
      // TL0 = 0x00;
      ++count;  //计数值自加1
    }
  }
}

/**
 * 睡眠一段时间(30000 个for循环)来保持当前状态
 */
void sleep() {
  int i = 30000;
  while (i--) {
    ;
  }
}

/** 闪烁数码管
 * @param t 闪烁次数
 */
void blink(int t) {
  unsigned int pre, i;
  for (i = 0; i < t; ++i) {
    sleep();
    pre = DATA;  // backup
    DATA = CLOSE_ALL;
    sleep();
    DATA = pre;
  }
}
///@}

#endif
