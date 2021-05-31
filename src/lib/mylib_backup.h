/** @file
 * 定义一些实用函数
 * 给寄存器一些别名
 * 方便代码的理解
 */

//!< @defgroup 对不同操作系统的调整
///@{
#define USE_LINUX 1  //!< Windows 环境注释掉这一句

#ifdef USE_LINUX
#include <8052.h>
#define code __code  //!< 定义code 为 __code 来兼容windows风格
#define interrupt \
  __interrupt  //!< 定义interrupt 为 __interrupt 来兼容windows风格

#else

#include <reg52.h>
#define __code code
#define __interrupt interrupt
#endif
///@}

//!< @defgroup 寄存器别名
///@{
/** P0 寄存器一般传递数据，为了阅读方便，
 * 改名为 DATA
 */
#define DATA P0
/**
 * P1 储存地址信息，低五位分别对应 ENLD,ADDR3,ADDR2,ADDR1,ADDR0
 */
#define ADDRESS P1
///@}

//!< @defgroup 一些数字的特殊含义
///@{
#define OVERFLOW 1      //!< TF 标志位1 表示溢出
#define NOT_OVERFLOW 0  //!< TF 标志位0 表示不溢出
#define OPEN 0          //!< 一般的led灯0表示打开
#define CLOSE 1         //!< 一般的led灯1表示关闭
#define CLOSE_ALL 0xff  //!< 所有位为 1 表示全部关闭
#define TRUE 1          //!< 定义布尔值TRUE=1
#define FALSE 0         //!< 定义布尔值FALSE=0
#define ENABLE 1        //!< 1有时表示状态位的使能状态
///@}

//!< @defgroup  led灯地址
///@{
#define ENABLE_LED_ARRAYS 14  //!< 01110 表示右边LED单独的led灯
///@}

//!< @defgroup  计数器控制器与中断
///@{
#define T0_STATUS TR0           //!< TR0 控制 T0 的状态
#define ALLOW_INTERRUPT EA      //!< EA 寄存器控制使能所有中断
#define ALLOW_T0_INTERRUPT ET0  //!< 允许T0中断
#define ENABLE_T0 TR0           //!< 启动T0的控制寄存器
//!< 计时1ms 的计数器初值
#define TIME_1MS() \
  TH0 = 0xFC;      \
  TL0 = 0x67;

/**
 * 使计时器T0计时1ms，
 * 并在计时结束之后产生中断
 */
void enable_timer_t0_with_interrupt() {
  ALLOW_INTERRUPT = TRUE;  // 使能总中断
  TMOD = 0x01;             // 设置T0为模式1
  TIME_1MS();
  ALLOW_T0_INTERRUPT = TRUE;  // 使能T0中断
  ENABLE_T0 = TRUE;           // 启动T0的控制寄存器
}
///@}

//!< @defgroup 中断号
///@{
#define T0_OVERFLOW 1  //!< 中断号1 表示 T0溢出
#define T1_OVERFLOW 3  //!< 中断号3 表示 T1溢出
///@}

//!< @defgroup 一些小功能函数
///@{
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

//!< @defgroup 七段数码管相关函数和变量
///@{
/**
 * DIGITS_LED[i] 0<=i<16 表示i的数码管的表示
 * 数码管共阳极，故1表示打开
 * code 说明指定数组存在 flash ROM 中，
 * 没有 code 会存到片内 RAM 中
 */
unsigned char code DIGITS_LED[] = {
    //         dp g f e; d c b a
    0xC0,  // 0: 1 1 0 0; 0 0 0 0
    0xF9,  // 1: 1 1 1 1; 1 0 0 1
    0xA4,  // 2: 1 0 1 0; 1 1 0 0
    0xB0,  // 3: 1 0 1 1; 0 0 0 0
    0x99,  // 4: 1 0 0 1; 1 0 0 1
    0x92,  // 5: 1 0 0 1; 0 0 1 0
    0x82,  // 6: 1 0 0 0; 0 0 1 0
    0xF8,  // 7: 1 1 1 1; 1 0 0 0
    0x80,  // 8: 1 0 0 0; 0 0 0 0
    0x90,  // 9: 1 0 0 1; 0 0 0 0
    0x88,  // A: 1 0 0 0; 1 0 0 0
    0x83,  // B: 1 0 0 0; 0 0 1 1
    0xC6,  // C: 1 1 0 0; 0 1 1 0
    0xA1,  // D: 1 0 1 0; 0 0 0 1
    0x86,  // E: 1 0 0 0; 0 1 1 0
    0x8E   // F: 1 0 0 0; 1 1 1 0
};

/** 使某一数码管显示某字符
 * @param digit_tube_id 管子的id 0-5 表示 LEDS0-LEDS5
 *                      也可以直接输入寄存器P1 的取值
 * @param c             显示的字符串
 */
void show_char(unsigned int digit_tube_id, char c) {
  DATA = CLOSE_ALL;  // 刷新之前关闭所有数码管防止残影
  // 8 指 ADDR3 = 1
  ADDRESS = digit_tube_id | 0x08;
  switch (c) {  // dp g f e; d c b a
    case 'H':
      DATA = 0x89;  //  1 0 0 0; 1 0 0 1
      break;
    case 'E':
      DATA = 0x86;  //  1 0 0 0; 0 1 1 0
      break;
    case 'L':
      DATA = 0xc7;  //  1 1 0 0; 0 1 1 1
      break;
    case 'l':
      DATA = 0xcf;  //  1 1 0 0; 1 1 1 1
      break;
    case 'O':
    case 'o':
      DATA = 0xC0;  //   1 1 0 0; 0 0 0 0
      break;
    case '!':
      DATA = 0x79;  // 0 1 1 1; 1 0 0 1
      break;
    default:
      DATA = DIGITS_LED[c - '0'];
      break;
  }
}
///@}

//!< @defgroup led点阵相关
///@{

#define ROW_SIZE 8  //!< 该单片机共8行
/** 将mask所代表的内容放到row_id
 * 注意地址最左边为mask 最低位
 * @param row_id 行号
 * @param mask   对应二进制信息
 */
void show_in_array(int row_id, int mask) {
  DATA = CLOSE_ALL;
  ADDRESS = row_id;
  DATA = mask;
}

///@}
