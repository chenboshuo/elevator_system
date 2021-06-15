/** @file
 * 处理请求的函数
 * @defgroup  处理起始请求和目标楼层请求
 * @{
 */

#ifndef REQUESTS_H
#define REQUESTS_H

#include "system_header.h"

#define UP_CALL 0    //!< 0 代表上行请求
#define DOWN_CALL 1  //!< 1 代表下行请求

#define FIRST_FLOOR 0   //!< 一楼信息放在索引 0 的位置
#define SECOND_FLOOR 1  //!< 2楼放在索引1 的位置
#define THIRD_FLOOR 2   //!< 3楼放在索引2 的位置

/// 判断是否有电梯内请求
#define has_any_requests(elevator_id)                                \
  (has_requested[elevator_id][0] || has_requested[elevator_id][1] || \
   has_requested[elevator_id][2])

/// 判断是否有该方向的楼层请求
#define has_any_calls()                                                     \
  (has_called[UP_CALL][FIRST_FLOOR] || has_called[UP_CALL][SECOND_FLOOR] || \
   has_called[DOWN_CALL][SECOND_FLOOR] || has_called[DOWN_CALL][THIRD_FLOOR])

/// 电梯内的请求
unsigned char has_requested[2][3] = {{FALSE, FALSE, FALSE},
                                     {FALSE, FALSE, FALSE}};

/// 楼层请求
unsigned char has_called[2][3] = {{FALSE, FALSE, FALSE}, {FALSE, FALSE, FALSE}};

///@}

#endif
