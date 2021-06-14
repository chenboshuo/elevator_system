/** @file
 * 主函数，完成电梯模块
 */
#include "lib/elevator.h"
#include "lib/interrupt.h"
#include "lib/refresh.h"

unsigned char left_arrive_floor = FALSE;
unsigned char right_arrive_floor = FALSE;

void main() {
  unsigned char move_clock;
  // 初始化电梯
  init_left_evevator();
  init_right_evevator();

  // 计时一秒
  enable_timer_t0_with_interrupt();
  // TIME_1MS();
  TIME_half_1MS();

  while (TRUE) {
    // get direction
    // get_direction(&left_elevator);
    get_direction(&right_elevator);

    // 移动
    move_clock = 255;
    while (move_clock--) {
      ;
    }

    // 到达处理
    // arrive(&left_elevator);
    // left_arrive_floor = TRUE;

    // arrive(&right_elevator);
    // right_arrive_floor = TRUE;

    // debug
    // ++left_elevator.current_level;
    // left_elevator.current_level %= 3;
  }
}

void reload_and_update() interrupt T0_OVERFLOW {
  // 刷新模块的显示
  refresh_module();

  // 刷新键盘响应
  refresh_key_line();

  // 左边电梯到达
  if (left_arrive_floor) {
    ;
  }

  // 右边电梯到达
  if (right_arrive_floor) {
    ;
  }

  // 更新计数器初值
  // TIME_1MS();
  TIME_half_1MS();
}
