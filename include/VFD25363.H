#ifndef __VFD25363_H__
#define __VFD25363_H__

void init_vfd(void);
void light_set(unsigned char light);  //设置亮度,亮度值为0-255
void char_str(unsigned char h,l,hl,unsigned char*zmk); 
//显示字符串函数
//h=行坐标0-15范围,
//l为列坐标0-31(宽度为8的字体),0-42(宽度为6的字体)
//hl为字体形式,3为16x8字体(支持外挂字库中文),2位12x8字体,1为12x6字体(支持外挂字库中文),0为8x6字体,

#endif