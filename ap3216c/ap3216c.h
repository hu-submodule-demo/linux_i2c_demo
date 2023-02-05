/**
 * @file      : ap3216c.h
 * @brief     : AP3216C驱动头文件
 * @author    : huenrong (huenrong1028@outlook.com)
 * @date      : 2023-02-04 14:16:45
 *
 * @copyright : Copyright (c) 2023 huenrong
 *
 * @history   : date       author          description
 *              2023-02-04 huenrong        创建文件
 *
 */

#ifndef __AP3216C_H
#define __AP3216C_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include <stdbool.h>

/**
 * @brief  初始化AP3216C
 * @return true : 成功
 * @return false: 失败
 */
bool ap3216c_init(void);

/**
 * @brief  读取AP3216C数据
 * @param  ir_data : 输出参数, 读取到的ir数据
 * @param  als_data: 输出参数, 读取到的als数据
 * @param  ps_data : 输出参数, 读取到的ps数据
 * @return true : 成功
 * @return false: 失败
 */
bool ap3216c_read_data(uint16_t *ir_data, uint16_t *als_data, uint16_t *ps_data);

#ifdef __cplusplus
}
#endif

#endif // __AP3216C_H
