/** @file
 * 针对不同操作系统的头文件和代码风格调整
 * @defgroup 计数器控制器与中断
 */

#ifndef INTERRUPT_H
#define INTERRUPT_H

#include "alias.h"
#include "system_header.h"

#define T0_STATUS TR0           //!< TR0 控制 T0 的状态
#define ALLOW_INTERRUPT EA      //!< EA 寄存器控制使能所有中断
#define ALLOW_T0_INTERRUPT ET0  //!< 允许T0中断
#define ENABLE_T0 TR0           //!< 启动T0的控制寄存器
//!< 计时1ms 的计数器初值
#define TIME_1MS() \
  TH0 = 0xFC;      \
  TL0 = 0x67;
/// 计时 2 ms 的计数器初值
#define TIME_2MS() \
  TH0 = 0xF8;      \
  TL0 = 0xCD;

/**
 * 使计时器T0计时1ms，
 * 并在计时结束之后产生中断
 */
void enable_timer_t0_with_interrupt() {
  ALLOW_INTERRUPT = TRUE;  // 使能总中断
  TMOD = 0x01;             // 设置T0为模式1
  // TIME_1MS();
  ALLOW_T0_INTERRUPT = TRUE;  // 使能T0中断
  ENABLE_T0 = TRUE;           // 启动T0的控制寄存器
}
///@}

//!< @defgroup 中断号
///@{
#define T0_OVERFLOW 1  //!< 中断号1 表示 T0溢出
#define T1_OVERFLOW 3  //!< 中断号3 表示 T1溢出
///@}

#endif
