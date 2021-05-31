/** @file
 * 关于寄存器别名的定义
 * @defgroup 寄存器别名
 * @{
 */

#ifndef ALIAS_H
#define ALIAS_H
#include "system_header.h"
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

#endif
