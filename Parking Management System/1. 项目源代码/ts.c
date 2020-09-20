/*
	@draw_word:显示一个字符 
	@x,y 从哪里开始显示字符 左上角
	@w h 字符显示多大
	@color 字符显示的颜色 单色
*/
#include "lcd.h"

void draw_word(int x,int y,int w,int h,int color, char s[])
{
	//1.遍历整个字符数组的元素
	int i,j;
	int x0,y0;
	int size=w/8;
	for(i=0;i<w*h/8;i++)//比如 第一个是0x01
	{
		//2.根据每个字符数组的元素的每一位来显示像素点呢？
		for(j=7;j>=0;j--)
		{
			//判断对应位是否为1
			//1000 0001 判断最高位是否为1？该怎么判断呢？
			//我们又要接着判断第6为是否为1？
			//假设是为1,那么是不是我们就需要显示该像素点呢？
			if((s[i]>>j)&1)
			{
				x0=x+7-j+8*(i%size);
				y0=y+i/size;
				//这个像素点的位置是什么呢？
				LCD_Draw_Point(x0,y0,0xff0000);
			}
		}
	}
}
