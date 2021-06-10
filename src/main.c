/** @file
 * 主函数，完成电梯模块
 */
#include "lib/data_tube.h"
#include "lib/interrupt.h"
#include "lib/refresh.h"
#include "lib/system_header.h"

void main() {
  // 初始化电梯
  // init_left_evevator();
  // init_right_evevator();

  // 计时一秒
  enable_timer_t0_with_interrupt();
  TIME_1MS();

  while (TRUE) {
    ;
  }
}

void reload_and_update() interrupt T0_OVERFLOW {
  // 更新计数器初值
  TIME_1MS();

  // 刷新模块的显示
  refresh_module();

  // 刷新键盘响应
  refresh_key_line();
}
