/**
 * @file      : main.c
 * @brief     : 程序入口文件
 * @author    : huenrong (huenrong1028@outlook.com)
 * @date      : 2023-02-04 14:15:42
 *
 * @copyright : Copyright (c) 2023 huenrong
 *
 * @history   : date       author          description
 *              2023-02-04 huenrong        创建文件
 *
 */

#include <stdio.h>
#include <unistd.h>

#include "./ap3216c/ap3216c.h"

/**
 * @brief  : 程序入口
 * @param  : argc: 输入参数, 参数个数
 * @param  : argv: 输入参数, 参数列表
 * @return : 成功: 0
 *           失败: 其它
 */
int main(int argc, char *argv[])
{
    if (!ap3216c_init())
    {
        printf("init ap3216c fail\n");

        return -1;
    }

    while (true)
    {
        uint16_t ir_data = 0;
        uint16_t als_data = 0;
        uint16_t ps_data = 0;

        if (!ap3216c_read_data(&ir_data, &als_data, &ps_data))
        {
            printf("read ap3216c data fail\n");

            return false;
        }

        printf("current ir: %d, current als: %d, current ps: %d\n", ir_data, als_data, ps_data);

        sleep(1);
    }

    return 0;
}
