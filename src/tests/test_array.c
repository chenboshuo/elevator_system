/** @file
 * 测试数码点阵的功能
 */
#include "../lib/alias.h"
#include "../lib/interrupt.h"
// #include "../lib/key.h"
#include "../lib/direction.h"
#include "../lib/led_array.h"

void main() {
  // 计时一秒
  enable_timer_t0_with_interrupt();
  TIME_1MS();

  while (TRUE) {
    move_dowm(RIGHT_SYMBOL);
  }
}

void flash_and_update() interrupt T0_OVERFLOW {
  static int line_id = 0;
  static int ms_count = 0;
  static unsigned char blink_clock = 0;  // 用于闪烁的周期

  if (ms_count == 200) {
    ++blink_clock;
    update_key_clocks();  // 刷新每个按键时钟
    ms_count = 0;
  }

  // 更新计数器初值
  TIME_1MS();

  // 处理闪烁
  if (blink_clock == 0) {
    key_clocks[0][0] = BLINK_PROCESSING_CLOCK;
    key_clocks[1][0] = BLINK_PROCESSING_CLOCK;
  }
  blink_bit_and_appear(0, 0, key_clocks[0][0]);
  blink_bit_and_appear(1, 0, key_clocks[1][0]);

  if (blink_clock == 30) {
    key_clocks[0][0] = BLINK_PROCESSING_CLOCK;
  }
  if (blink_clock >= 30) {
    blink_bit_and_disappear(0, 0, key_clocks[0][0]);
  }

  if (blink_clock == 20) {
    move_up(LEFT_SYMBOL);
  } else if (blink_clock == 40) {
    move_up(RIGHT_SYMBOL);
  } else if (blink_clock == 50) {
    move_dowm(LEFT_SYMBOL);
  } else if (blink_clock == 60) {
    move_dowm(RIGHT_SYMBOL);
  }

  // 显示底图
  show_in_array(line_id, base_image[line_id]);

  // 处理下一行
  ++line_id;                         // 下一次刷新下一行
  line_id &= ARRAY_LINE_VALID_BITS;  // 数码管共7行

  // 计时器+1
  ++ms_count;
}
