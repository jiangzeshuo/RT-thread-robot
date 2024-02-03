/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-05-06     jzs       the first version
 */
#include <rtthread.h>
#include "gear_con.h"
#include "colorcfg.h"
#include <math.h>
#define THREAD_PRIORITY         10   //越大优先级越高
#define THREAD_STACK_SIZE       4096
#define THREAD_TIMESLICE        5
ALIGN(RT_ALIGN_SIZE)

 int prepare_statu(void)
{
    rt_uint32_t period, pulse;
    period = 20000000;
    pulse = 1500000;
        for (int i = 1; i <= 8; i++)
        {
            pwm_device[i] = (struct rt_device_pwm *)rt_device_find(PWM_DEVICE[i].name);
            if (pwm_device[i] == RT_NULL)
            {
                rt_kprintf("%d pwm sample run failed! can't find %s device!\n",i, PWM_DEVICE[i].name);
                return RT_ERROR;
            }
            //rt_kprintf("pwm  run successful!  find %s device!\n", PWM_DEVICE[i].name);
            rt_pwm_set(pwm_device[i], PWM_DEVICE[i].channel, period,pulse);
            rt_pwm_enable(pwm_device[i], PWM_DEVICE[i].channel);
        }
        for (int i = 1;  i <=8 ; i++)
        {
            int temp = 500000+(int)GearProg00[0][i-1]/180.0*2000000;
            rt_pwm_set(pwm_device[i], PWM_DEVICE[i].channel, period,temp);
//          rt_pwm_set(pwm_device[i], PWM_DEVICE[i].channel, period,pulse);
        }
}
INIT_APP_EXPORT(prepare_statu);

static rt_thread_t tid1 = RT_NULL;

int force_analysis(rt_uint16_t *target,rt_uint16_t *obstacle,double *force)
{
    Point point[2];
    int temp;
     point[0].x = (target[1]+target[0])/2;
     point[0].y = (target[3]+target[2])/2;
     point[0].length = target[1]-target[0];
     point[1].x = (obstacle[1]+obstacle[0])/2;
     point[1].y = (obstacle[3]+obstacle[2])/2;
     point[1].length = obstacle[1]-obstacle[0];
     for(int i=0 ;i<2;i++)
     {
         if(point[i].x>160)
         {
             temp=point[i].x-160;
             point[i].dir = -1;
         }
         else
         {
             temp = point[i].x;
             point[i].dir = 1;
         }
         if(temp==0)
         {
             point[i].angle = 0;
         }
          else {
              point[i].angle = atan(temp*1.0/point[i].y)*180.0/3.1416;
        }
         if(point[i].length<=20)
         {
             if(i==0)
             {
                 point[i].des = 80;
             }
             else {
                 point[i].des = 0;
            }
         }
         else if(point[i].length<=90&&point[i].length>=20)
         {
             point[i].des = (90-point[i].length)+40;
         }
         else if (point[i].length>90&&point[i].length<=120)
         {
             point[i].des = 40- ((point[i].length-90)/3);
         }
         else if(point[i].length>120)
         {
           point[i].des = 20;
         }
     }
     point[0].force =GRAV_CON[0]*point[0].des;
     if(point[1].des>60||point[1].des==0)
     {
         point[1].force = 0;
     }
     else
     {
         point[1].force =pow((1.0/(point[1].des*0.01) - 1.0/0.6),1)*pow(1.0/(point[1].des*0.01),1)*20;
     }
     rt_kprintf("target = %d obstacle = %d",(int)point[0].force,(int)point[1].force);

     //force[0]上下移动，force【1】是左右右下角是0 0
     force[0] = point[0].force*cos(point[0].angle*3.1416/180.0)+point[1].force*cos(point[1].angle*3.1416/180.0);
     force[1] = point[0].force*sin(point[0].angle*3.1416/180.0)*point[0].dir+point[1].force*sin(point[1].angle*3.1416/180.0)*point[1].dir*-1;
     return 1;

}
int err;
int i_2=0;
static void gear_control(void *parameter)
{
    rt_uint16_t des[9];
    rt_uint32_t count = 0;
    double force[2]={0};
    while (1)
    {
//        /* 线程 1 采用低优先级运行，一直打印计数值 */
//        if (rt_mq_recv(&mq, &des, sizeof(des), RT_WAITING_FOREVER) == RT_EOK)
//        {
//            if(des[8] ==1)
//            {
//                i_2 = 0;
//                force_analysis(des,des+4,force);
//                if(force[1]>30)
//                {
//                    rt_kprintf("  moveright  ");
//                    cmdcon(GearProg05, 11);
//                }
//                else if(force[1]<-30)
//                {
//                    rt_kprintf("  moveleft  ");
//                    cmdcon(GearProg04,11);
//                }
//                if(force[1]<60&&force[1]>-60)
//                {
//                    if(force[0]>5)
//                    {
//                        rt_kprintf("  forward   ");
//                        cmdcon(GearProg02,11);
//                    }
//                    else if(force[0]<-5)
//                    {
//                        rt_kprintf("  backward   ");
//                        cmdcon(GearProg03,11);
//                    }
//                }
//                char B='b';
//                err = rt_mq_send(&mc, &B, sizeof(B));
//                if (err != RT_EOK)
//                {
//                  rt_kprintf("rt_mq_send ERR\n");
//                }
//                rt_kprintf("force0 = %d  force1 = %d\n",(int)force[0],(int)force[1]);
//            }
//            else if (des[8]==2) {
//                if(i_2==0)
//                {
//                   i_2++;
//                   cmdcon(GearProg06,8);
//                }
//                else
//                {
//                    cmdcon(GearProg07,8);
//                    cmdcon(GearProg07,8);
//                   i_2 = 0;
//                }
//            }
//        }


        cmdcon(GearProg02,11);
        rt_thread_mdelay(2000);
        cmdcon(GearProg04,11);
        rt_thread_mdelay(2000);
        cmdcon(GearProg05,11);
        rt_thread_mdelay(2000);
        cmdcon(GearProg06,8);
        rt_thread_mdelay(2000);
        cmdcon(GearProg07,8);
        rt_thread_mdelay(2000);
        cmdcon(GearProg09,4);
        rt_thread_mdelay(2000);
        cmdcon(GearProg10,11);
        rt_thread_mdelay(2000);
        cmdcon(GearProg13,10);
        rt_thread_mdelay(20000);
    }
}

void thread_gear(void)
{
    /* 创建线程 1，名称是 thread1，入口是 thread1_entry*/
    tid1 = rt_thread_create("thread1",
                            gear_control, RT_NULL,
                            THREAD_STACK_SIZE,
                            THREAD_PRIORITY, THREAD_TIMESLICE);

    /* 如果获得线程控制块，启动这个线程 */
    if (tid1 != RT_NULL)
        rt_thread_startup(tid1);

}
MSH_CMD_EXPORT(thread_gear, thread_gear);
int map_util(int val ,int from_low,int from_high,int to_low,int to_high)
{
    float temp= (float)val/(from_high-from_low);
    return to_low+temp*(to_high-to_low);
}
int servoPos[9];
int cmdcon( int GearProg[][ControlNum],int step)
{
    int gear_angle[9];
    int pulse_num[9];
    for(int i=0 ;i<step ;i++)
    {
       for (int s = 1; s <= 8; s++)
       {
           if(i==0)
           {
               servoPos[s] = GearProg[0][s-1];
           }
           else
           {
               servoPos[s] =gear_angle[s];
           }
       }
       for (int p = 1; p <= 8; p++) { // Loop for servo
         if (i == 0)
         {
             gear_angle[p] = GearProg[i][p-1];
         }
         else
         {
             gear_angle[p] = gear_angle[p] + GearProg[i][p-1];
         }
       }
       for (int j = 0; j <5; j++)
       { // Loop for time section
         for (int k = 1; k <= 8; k++)
         { // Loop for servo
             int temp =  map_util(j,0,5,servoPos[k],gear_angle[k]);
             pulse_num[k] = 500000+(int)temp/180.0*2000000;
             if(k==0)
             {
                 rt_kprintf("gear_angle= %d \n",pulse_num[k]);
             }
             rt_pwm_set(pwm_device[k], PWM_DEVICE[k].channel, 20000000, pulse_num[k]);
         }
         rt_thread_mdelay(GearProg[i][8]/5);
       }
    }
}

//50000    0
//100000   45
//150000   90
//200000   135
//250000   180
#define PWM_DEV_NAME "pwm4"
#define PWM_DEV_CHANNEL 3
static int pwm_sample(int argc, char *argv[])
{
    rt_uint32_t period, pulse, dir;
    period = 20000000;
    dir = 1;
    pulse = 500000;
    pwm_dev = (struct rt_device_pwm *)rt_device_find(PWM_DEV_NAME);
    if (pwm_dev == RT_NULL)
    {
        rt_kprintf("pwm sample run failed! can't find %s device!\n", PWM_DEV_NAME);
        return RT_ERROR;
    }
    rt_pwm_set(pwm_dev, PWM_DEV_CHANNEL, period, pulse);
    rt_pwm_enable(pwm_dev, PWM_DEV_CHANNEL);
    while (1)
    {
        rt_thread_mdelay(3000);
        if (dir)
        {
            pulse += 500000;
        }
        else
        {
            pulse -= 500000;
        }
        if (pulse >= 2500000)
        {
            dir = 0;
        }
        if (500000 == pulse)
        {
            dir = 1;
        }
        rt_kprintf("%d\n",pulse);
        rt_pwm_set(pwm_dev, PWM_DEV_CHANNEL, period, pulse);
    }
}
MSH_CMD_EXPORT(pwm_sample, pwm sample);
