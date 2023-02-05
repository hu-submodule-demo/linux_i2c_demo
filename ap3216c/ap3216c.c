/**
 * @file      : ap3216c.c
 * @brief     : AP3216C驱动源文件
 * @author    : huenrong (huenrong1028@outlook.com)
 * @date      : 2023-02-04 14:16:56
 *
 * @copyright : Copyright (c) 2023 huenrong
 *
 * @history   : date       author          description
 *              2023-02-04 huenrong        创建文件
 *
 */

#include <stdio.h>
#include <unistd.h>

#include "./ap3216c.h"
#include "../linux_i2c/i2c.h"

// AP3216C I2C设备地址
#define AP3216C_I2C_DEV_NAME "/dev/i2c-0"
// AP3216C I2C器件地址
#define AP3216C_I2C_SLAVE_ADDR 0x1E

// AP3316C寄存器
#define AP3216C_SYSTEM_CONFIG 0x00 // 配置寄存器
#define AP3216C_INT_STATUS 0x01    // 中断状态寄存器
#define AP3216C_INT_CLEAR 0x02     // 中断清除寄存器
#define AP3216C_IR_DATA_LOW 0x0A   // IR数据低字节
#define AP3216C_IR_DATA_HIGH 0x0B  // IR数据高字节
#define AP3216C_ALS_DATA_LOW 0x0C  // ALS数据低字节
#define AP3216C_ALS_DATA_HIGH 0x0D // ALS数据高字节
#define AP3216C_PS_DATA_LOW 0x0E   // PS数据低字节
#define AP3216C_PS_DATA_HIGH 0x0F  // PS数据高字节

// AP3216C模式
#define POWER_DOWN 0x00                // 关闭电源
#define ALS_FUNCTION_ACTIVE 0x01       // 激活ALS模式
#define PS_IR_FUNCTION_ACTIVE 0x02     // 激活PS, IR模式
#define ALS_PS_IR_FUNCTION_ACTIVE 0x03 // 激活ALS, PS, IR模式
#define SW_RESET 0x04                  // 复位
#define ALS_FUNCTION_ONCE 0x05         // ALS模式单次采集
#define PS_IR_FUNCTION_ONCE 0x06       // PS, IR模式单次采集
#define ALS_PS_IR_FUNCTION_ONCE 0x07   // ALS, PS, IR模式单次采集

/**
 * @brief  初始化AP3216C
 * @return true : 成功
 * @return false: 失败
 */
bool ap3216c_init(void)
{
    uint8_t ap3216c_config = 0;

    if (!i2c_open(AP3216C_I2C_DEV_NAME))
    {
        return false;
    }

    // 初始化ap3216c
    // 复位ap3216c
    ap3216c_config = SW_RESET;
    if (!i2c_write_data_sub(AP3216C_I2C_DEV_NAME, AP3216C_I2C_SLAVE_ADDR, AP3216C_SYSTEM_CONFIG, &ap3216c_config, 1))
    {
        return false;
    }
    // 复位以后至少延时10ms
    usleep(20 * 1000);

    // 开启ALS、PS+IR
    ap3216c_config = ALS_PS_IR_FUNCTION_ACTIVE;
    if (!i2c_write_data_sub(AP3216C_I2C_DEV_NAME, AP3216C_I2C_SLAVE_ADDR, AP3216C_SYSTEM_CONFIG, &ap3216c_config, 1))
    {
        return false;
    }

    // 配置后需要延时一段时间才生效
    usleep(350 * 1000);

    return true;
}

/**
 * @brief  读取AP3216C数据
 * @param  ir_data : 输出参数, 读取到的ir数据
 * @param  als_data: 输出参数, 读取到的als数据
 * @param  ps_data : 输出参数, 读取到的ps数据
 * @return true : 成功
 * @return false: 失败
 */
bool ap3216c_read_data(uint16_t *ir_data, uint16_t *als_data, uint16_t *ps_data)
{
    uint8_t read_data[6] = {0};

    // 循环读取所有传感器数据
    for (uint8_t i = 0; i < 6; i++)
    {
        if (!i2c_read_data_sub(&read_data[i], AP3216C_I2C_DEV_NAME, AP3216C_I2C_SLAVE_ADDR,
                               (AP3216C_IR_DATA_LOW + i), 1))
        {
            printf("ap3216c read data fail\n");

            return false;
        }
    }

    // IR_OF位为1,则数据无效
    if (read_data[0] & 0x80)
    {
        *ir_data = 0;
    }
    // 获取IR传感器的数据
    else
    {
        *ir_data = (uint16_t)((read_data[1] << 2) | (read_data[0] & 0x03));
    }

    // 获取ALS传感器的数据
    *als_data = (uint16_t)((read_data[3] << 8) | read_data[2]);

    // PS_OF位为1,则数据无效
    if (read_data[4] & 0x40)
    {
        *ps_data = 0;
    }
    // 获取PS传感器的数据
    else
    {
        *ps_data = (uint16_t)(((read_data[5] & 0x3F) << 4) | (read_data[4] & 0x0F));
    }

    return true;
}
