#ifndef colorcfg_h
#define colorcfg_h

#include "color.h"


/*配置色块查询的范围  图像在LCD的坐标*/
#define IMG_X 0                       //图片x坐标120
#define IMG_Y 0               //图片y坐标240
#define IMG_W 320               //图片宽度240
#define IMG_H 480                   //图片高度320
#define ALLOW_FAIL_PER       2     //容错率越大 颜色匹配越高，也越难识别 取值>1

#define COLOR_RANG           30    //设定颜色的偏移范围 越大越容易识别 太大容易误识别
#define COLOR_NUM            7     //设定追踪颜色的数目
#define OBJECT_NUM          4     //设定追踪物体数量
extern rt_uint8_t global_page;                                  //当前颜色的
extern rt_uint8_t color_list[COLOR_NUM][7+7];       //颜色字符
extern SEARCH_AREA_t area[OBJECT_NUM+1];                            //定义搜索区域
extern RESULT_t result[OBJECT_NUM];         //定义搜索结果
extern TARGET_CONDITION_t condition[COLOR_NUM];//定义目标参数
extern Quadrant     Quadrant_control;
/* 消息队列控制块 */
struct rt_messagequeue mq;
struct rt_messagequeue mc;
void Feature_Recognize_Start_Phoenix();

#define LCD_READPOINT( usX, usY )  LCD_ReadPoint(usX,usY)//定义读点函数

#endif

