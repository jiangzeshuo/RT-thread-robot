/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-04-17     jzs       the first version
 */
#include <rtthread.h>
#include <math.h>
#include <rtdevice.h>
#include "color.h"
#include "colorcfg.h"
#include "drv_spi_ili9488.h"
#include "drv_dcmi.h"
#include "drv_common.h"
#include "lcd_port.h"
#define THREAD_PRIORITY         12   //越大优先级越高
#define THREAD_STACK_SIZE       4096
#define THREAD_TIMESLICE        5
rt_uint16_t des[9]={0};

int u=0, sum[8],t=0;
COLOR_RGB_t rgb;
COLOR_HLS_t hls;
struct rt_event ov2640_event;
static void camera_control(void *paramter)
{
    rt_event_init(&ov2640_event, "ov2640", RT_IPC_FLAG_FIFO);
    struct drv_lcd_device *lcd;
    lcd = (struct drv_lcd_device *)rt_device_find("lcd");
    struct rt_device_rect_info rect_info = {0, 0, 320, 240};
    while(1)
    {
        rt_event_recv(&ov2640_event,
                            1,
                            RT_EVENT_FLAG_AND | RT_EVENT_FLAG_CLEAR,
                            RT_WAITING_FOREVER,
                            RT_NULL);
        lcd->parent.control(&lcd->parent, RTGRAPHIC_CTRL_RECT_UPDATE, &rect_info);//show 320*240 RGB image
        Feature_Recognize_Start_Phoenix();
        DCMI_Start();
        rt_thread_mdelay(10);
    }
}
static rt_thread_t tid1 = RT_NULL;
int thread_camera(void)
{
    /* 创建线程 1，名称是 thread1，入口是 thread1_entry*/
    tid1 = rt_thread_create("camera",
                            camera_control, RT_NULL,
                            THREAD_STACK_SIZE,
                            THREAD_PRIORITY, THREAD_TIMESLICE);
    /* 如果获得线程控制块，启动这个线程 */
    if (tid1 != RT_NULL)
        rt_thread_startup(tid1);
    return ;

}
INIT_APP_EXPORT(thread_camera);

int len_x,len_y,pre_u=0,count=0;
void Feature_Recognize_Start_Phoenix()
{
    lcd_draw_line(160,0,160,100);

    rt_uint8_t i_2=0,count_color=0,distinguish=0;
    static rt_uint8_t count_spot=0;
    int err;
    if(Trace(&condition[1], &result[i_2], &area[i_2],i_2))            //执行颜色识别  画个矩形、画个十字准星。// trace（跟踪）识别颜色函数 condition是识别颜色条件的一个结构体数组里面存储着HLS的threshold，result是用来储存识别到的物体坐标与大小
    {
        des[4] = result[i_2].Xmin;
        des[5] = result[i_2].Xmax;
        des[6] = result[i_2].Ymin;
        des[7] = result[i_2].Ymax;
        sum[4]+=des[4];
        sum[5]+=des[5];
        sum[6]+=des[6];
        sum[7]+=des[7];

        if(t%10==0)
        {
            for(int i= 4;i<8;i++)
            {
                des[i] = sum[i]/10;
                sum[i] = 0;
            }
             if(t>10000)
                 t=0;
        }
           t++;
        lcd_draw_rectangle_blue( result[i_2].x-result[i_2].w/2, result[i_2].y-result[i_2].h/2, result[i_2].x-result[i_2].w/2+result[i_2].w,  result[i_2].y-result[i_2].h/2+result[i_2].h);

    }
    if(Trace(&condition[0], &result[i_2], &area[i_2],i_2))            //执行颜色识别  画个矩形、画个十字准星。// trace（跟踪）识别颜色函数 condition是识别颜色条件的一个结构体数组里面存储着HLS的threshold，result是用来储存识别到的物体坐标与大小
    {
        des[0] = result[i_2].Xmin;
        des[1] = result[i_2].Xmax;
        des[2] = result[i_2].Ymin;
        des[3] = result[i_2].Ymax;
        lcd_draw_line(result[i_2].x,result[i_2].y,result[i_2].x,0);
        sum[0]+=des[0];
        sum[1]+=des[1];
        sum[2]+=des[2];
        sum[3]+=des[3];
        u++;
        if(u%10==0)
        {
            for(int i= 0;i<4;i++)
            {
                des[i] = sum[i]/10;
                sum[i] = 0;
            }
             if(u>10000)
                 u=0;
             des[8] = 1;
//            err = rt_mq_send(&mq, &des, sizeof(des));
//            if (err != RT_EOK)
//            {
//              rt_kprintf("rt_mq_send ERR\n");
//            }
//            char B = '0';
//            if (rt_mq_recv(&mc, &B, sizeof(B), RT_WAITING_FOREVER) == RT_EOK)
//            {
//
//            }
       }
        lcd_draw_rectangle( result[i_2].x-result[i_2].w/2, result[i_2].y-result[i_2].h/2, result[i_2].x-result[i_2].w/2+result[i_2].w,  result[i_2].y-result[i_2].h/2+result[i_2].h);
    }
    if(pre_u!=u)
    {
        pre_u = u;
        count=0;
    }
    else {
        count++;
        if(count==50)
        {
            rt_kprintf("send urgent date!!!\n");
            des[8] = 2;
            err = rt_mq_urgent(&mq, &des, sizeof(des));
            if (err != RT_EOK)
            {
              rt_kprintf("rt_mq_send ERR\n");
            }
            count = 0;
        }
    }



}



/* 消息队列中用到的放置消息的内存池 */
 char msg_pool[4096];

int mesgq_sample(void)
{
    rt_err_t result;

    /* 初始化消息队列 */
    result = rt_mq_init(&mq,
                        "mqt",
                        &msg_pool[0],             /* 内存池指向 msg_pool */
                        18,                          /* 每个消息的大小是 1 字节 */
                        sizeof(msg_pool),        /* 内存池的大小是 msg_pool 的大小 */
                        RT_IPC_FLAG_PRIO);       /* 如果有多个线程等待，优先级大小的方法分配消息 */
    if (result != RT_EOK)
    {
        rt_kprintf("init message queue failed.\n");
        return -1;
    }
}
INIT_APP_EXPORT(mesgq_sample);

/* 消息队列中用到的放置消息的内存池 */
 char msg_pool2[4096];

int mesgq_con(void)
{
    rt_err_t result;

    /* 初始化消息队列 */
    result = rt_mq_init(&mc,
                        "mct",
                        &msg_pool2[0],             /* 内存池指向 msg_pool */
                        1,                          /* 每个消息的大小是 1 字节 */
                        sizeof(msg_pool2),        /* 内存池的大小是 msg_pool 的大小 */
                        RT_IPC_FLAG_PRIO);       /* 如果有多个线程等待，优先级大小的方法分配消息 */
    if (result != RT_EOK)
    {
        rt_kprintf("init message queue failed.\n");
        return -1;
    }
}
INIT_APP_EXPORT(mesgq_con);
