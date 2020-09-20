#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "lcd.h"

/*
	@x0 y0:从哪里开始显示图片 图片的左上角的坐标
	@pathname:显示的图片的路径名
*/
void display_bmp(int x0,int y0,const char* pathname)
{
	//1打开bmp图片
	int bmp_fd=open(pathname,O_RDONLY);//以只读来打开bmp图片
	if(-1==bmp_fd)
	{
		printf("open %s fail\n",pathname);
		perror("open fail");
		return ;
	}
	//2.操作
	//判断是否为真正的bmp图片
	char buf1[2];
	read(bmp_fd,buf1,2);
	if(buf1[0]!=0x42||buf1[1]!=0x4d)//如果说不是BM
	{
		printf("NOT BMP!\n");
		close(bmp_fd);
		return ;
	}
	//获取图片的宽 高 色深
	int width,height;
	short depth;

	unsigned char buf[4];
	//定位光标
	lseek(bmp_fd,0x12,SEEK_SET);
	read(bmp_fd,buf,4);
	width=(buf[3]<<24)|
		  (buf[2]<<16)|
		  (buf[1]<<8)|
		  (buf[0]);
	lseek(bmp_fd,0x16,SEEK_SET);
	read(bmp_fd,buf,4);
	height=(buf[3]<<24)|
		  (buf[2]<<16)|
		  (buf[1]<<8)|
		  (buf[0]);
	//打印图片的宽和高
	printf("this Bmp is %d*%d\n",width,height);
	//获取色深
	lseek(bmp_fd,0x1c,SEEK_SET);
	read(bmp_fd,buf,2);
	depth=(buf[1]<<8)|
		  (buf[0]);
	printf("this Bmp depth is %d\n",depth);

	//我们仅支持24和32
	if(!(depth==24||depth==32))
	{
		printf("Not Support!\n");
		close(bmp_fd);
		return ;
	}
	//显示图片
	int line_valid_bytes=abs(width)*depth/8;//一行有效字节数
	int line_bytes;//一行的总字节数=有效字节数+赖子
	int laizi=0;
	if(line_valid_bytes%4)
	{
		laizi=4-line_valid_bytes%4;
	}
	line_bytes=line_valid_bytes+laizi;
	
	int total_bytes=line_bytes*abs(height);//整个像素数组的大小

	unsigned char *piexls=malloc(total_bytes);
	//malloc动态申请一段内存 返回这段内存的首地址 

	//读取像素数组
	lseek(bmp_fd,54,SEEK_SET);
	read(bmp_fd,piexls,total_bytes);//把像素数组读入piexls所指向的内存中
	//读取每一个像素点的颜色值并且显示
	unsigned char a,r,g,b;//定义单个像素 ARGB 占4个字节
	int color;
	int i = 0;
	int x,y;
	for(y=0;y<abs(height);y++)//遍历每一行
	{
		//显示一行
		for(x=0;x<abs(width);x++)
		{
			//b g r a
			b = piexls[i++];
			g = piexls[i++];
			r = piexls[i++];
			if(depth == 32)
			{
				a = piexls[i++];
			}
			else
			{
				a = 0;
			}
			int color=(a<<24)|(r<<16)|(g<<8)|b;
			LCD_Draw_Point(width>0?x0+x:x0+abs(width)-1-x,
						   height>0?y0+abs(height)-1-y:y0+y,
							color);
		}
		//每一行的末尾，可能有赖子 跳过赖子
		i+=laizi;
	}
	free(piexls);//释放动态内存
	close(bmp_fd);
	
}
