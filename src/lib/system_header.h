/** @file
 * 针对不同操作系统的头文件和代码风格调整
 * @defgroup 对不同操作系统的调整
 */

#ifndef SYSTEM_HEADER
#define SYSTEM_HEADER

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
#define __bit bit

///@}
#endif
#endif
