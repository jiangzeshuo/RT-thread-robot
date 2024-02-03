#ifndef __COLOR_H
#define __COLOR_H
#include <rtthread.h>



#define minOf3Values( v1, v2, v3 )          ( (v1<v2) ? ( (v1<v3) ? (v1) : (v3) ) \
                                : ( (v2<v3) ? (v2) : (v3) ) )

#define maxOf3Values( v1, v2, v3 )          ( (v1>v2) ? ( (v1>v3) ? (v1) : (v3) ) \
                                : ( (v2>v3) ? (v2) : (v3) ) )
                                
typedef struct                          //判定为目标的条件
{
    rt_uint16_t H_MIN;             //目标最小色度
    rt_uint16_t H_MAX;             //目标最大色度

    rt_uint8_t S_MIN;              //目标最小饱和度
    rt_uint8_t S_MAX;              //目标最大饱和度

    rt_uint8_t L_MIN;              //目标最小亮度
    rt_uint8_t L_MAX;              //目标最大亮度

    rt_uint16_t WIDTH_MIN;     //目标最小宽度
    rt_uint16_t HEIGHT_MIN;    //目标最小高度
    
    rt_uint16_t WIDTH_MAX;     //目标最大宽度
    rt_uint16_t HEIGHT_MAX;    //目标最大高度
}TARGET_CONDITION_t;

typedef struct                  //搜寻区域
{
    rt_uint16_t X_Start;
    rt_uint16_t X_End;
    rt_uint16_t Y_Start;
    rt_uint16_t Y_End;
}SEARCH_AREA_t;


typedef struct              //结果
{
    rt_uint16_t x;         //目标x坐标
    rt_uint16_t y;         //目标y坐标
    rt_uint16_t w;         //目标宽度
    rt_uint16_t h;         //目标高度
    rt_uint16_t Xmin;
    rt_uint16_t Ymin;
    rt_uint16_t Xmax;
    rt_uint16_t Ymax;
    rt_uint16_t object;
}RESULT_t;

typedef struct                      //HLS颜色
{
    rt_uint8_t Hue;                    //色度    ,[0,240]
    rt_uint8_t Lightness;      //亮度    ,[0,240]
    rt_uint8_t Saturation;     //饱和度,[0,240]
}COLOR_HLS_t;

typedef struct                      //RGB
{
    rt_uint8_t Red;                    // [0,255]
    rt_uint8_t Green;        // [0,255]
    rt_uint8_t Blue;         // [0,255]
}COLOR_RGB_t;

typedef struct  {

    rt_uint16_t    Up_x;
    rt_uint16_t    Up_y;
    rt_uint16_t    Dowm_x;
    rt_uint16_t    Dowm_y;
    rt_uint16_t    Left_x;
    rt_uint16_t    Left_y;
    rt_uint16_t    Right_x;
    rt_uint16_t    Right_y;
}Quadrant;


//读取某点的颜色
void ReadColor( rt_uint16_t usX, rt_uint16_t usY, COLOR_RGB_t* color_rgb );

//RGB转换为HLS
void RGB2HSL( const COLOR_RGB_t* color_rgb, COLOR_HLS_t* color_hls );

 //  颜色匹配
int ColorMatch(const COLOR_HLS_t* color_hls, const TARGET_CONDITION_t* condition );

//  寻找腐蚀中心
int SearchCenter(rt_uint16_t* x, rt_uint16_t* y, const TARGET_CONDITION_t* condition,  SEARCH_AREA_t* area );

// 从腐蚀中心向外腐蚀，得到新的腐蚀中心
int Corrode(rt_uint16_t oldX, rt_uint16_t oldY, const TARGET_CONDITION_t* condition, RESULT_t* result );

// 得到匹配色块的信息
int Trace(const TARGET_CONDITION_t* condition, RESULT_t* result_final, SEARCH_AREA_t* area,rt_uint8_t control_num);

int Draw_Circle(rt_uint16_t x0,rt_uint16_t y0,rt_uint8_t r, RESULT_t* result);

void Quandrant_init(RESULT_t* result_final);

int  consend_color(const TARGET_CONDITION_t* condition,RESULT_t* result_final,Quadrant* Quadrant_control );

int Draw(rt_uint16_t x0,rt_uint16_t y0,rt_uint8_t r);
void draw_cross(rt_uint16_t x,rt_uint16_t y);
#endif
