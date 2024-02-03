/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-09-02     RT-Thread    first version
 */

#include <rtthread.h>
#include <rtdevice.h>
#include "drv_common.h"
#include "lcd_port.h"
#include "colorcfg.h"
#define LED_PIN GET_PIN(I, 8)
struct rt_event ov2640_event;
extern rt_uint8_t model_input_data_rgb[];
extern rt_uint8_t model_input_data_gray[];


int main(void)
{
    rt_pin_mode(LED_PIN, PIN_MODE_OUTPUT);
    while(1)
    {
        rt_thread_mdelay(100);
    }
    return RT_EOK;
}

#include "stm32h7xx.h"
static int vtor_config(void)
{
   /* Vector Table Relocation in Internal QSPI_FLASH */
   SCB->VTOR = QSPI_BASE;
   return 0;
}
INIT_BOARD_EXPORT(vtor_config);




