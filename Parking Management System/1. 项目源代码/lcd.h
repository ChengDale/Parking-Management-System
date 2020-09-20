#ifndef _LCD_H_
#define _LCD_H_
//声明函数
void LCD_init(void);
void lcd_uninit(void);
void LCD_Draw_Point(int x,int y,int color);
void LCD_Draw_Matrix(int x0,int y0,int w,int h,int color);

#endif