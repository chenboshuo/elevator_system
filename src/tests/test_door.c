/** @file
 * 扫描所有按键，检测是否按下
 * 参考
 * https://github.com/chenboshuo/learn_c51/blob/main/key_scan_all.c
 */
#include "../lib/door.h"
#include "../lib/interrupt.h"

#define DOOR_CLOCK_PERIOD 10000  //!<开门测试的周期

void main() {
  // 计时一秒
  enable_timer_t0_with_interrupt();
  TIME_1MS();

  while (TRUE) {
    ;
  }
}

void flash_and_update() interrupt T0_OVERFLOW {
  static int door_clock = 0;  // 开关门的计时周期
  switch (door_clock) {
    case 0:
    case 1000:
    case 2000:
    case 3000:
    case 4000:  // 测试过度关门
      open_door(0);
      break;
    case 5000:
    case 6000:
    case 7000:
    case 8000:
      close_door(0);
      break;
    case 1500:
    case 2500:
    case 3500:
    case 4500:
    case 5500:  // 测试过度关门
      open_door(4);
      break;
    case 6500:
    case 7500:
    case 8500:
    case 9500:
      close_door(4);
      break;
  }
  open_lights(door_status);

  ++door_clock;
  door_clock %= DOOR_CLOCK_PERIOD;

  // 更新计数器初值
  TIME_1MS();
}
