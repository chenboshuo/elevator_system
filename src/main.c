/** @file
 * 主函数，完成电梯模块
 */
// #include "lib/elevator.h"
#include "lib/interrupt.h"
// #include "lib/light.h"  // debug
#include "lib/refresh.h"

__bit left_arrive_floor = FALSE;
__bit right_arrive_floor = FALSE;

void main() {
  // unsigned int move_clock;
  // 初始化电梯
  init_left_evevator();
  // init_right_evevator();

  // 计时一秒
  enable_timer_t0_with_interrupt();
  // TIME_1MS();
  TIME_half_1MS();

  while (TRUE) {
    // get direction
    //   get_direction(&left_elevator);
    //   get_direction(&right_elevator);
    //
    //   // 移动
    //   move_clock = 0xFFFF;
    //   while (move_clock--) {
    //     ;
    //   }
    //
    //   // 到达处理
    //   arrive(&left_elevator);
    //   left_arrive_floor = TRUE;
    //   // open_lights(0xEF);
    //
    //   arrive(&right_elevator);
    //   right_arrive_floor = TRUE;
    ;
  }
}

void reload_and_update() interrupt T0_OVERFLOW {
  // 刷新模块的显示
  refresh_module();

  // 刷新键盘响应
  refresh_key_line();

  // 刷新坐电梯状态
  update_elevator_status(&left_elevator);
  // refresh_right_elevator();

  // 更新计数器初值
  // TIME_1MS();
  TIME_half_1MS();
  // ++ms_count;
}
