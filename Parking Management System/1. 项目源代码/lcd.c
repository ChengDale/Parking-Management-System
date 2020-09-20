#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#define DEBUG 1
#if DEBUG
#define print printf("%s %s %d\n",__FILE__,__FUNCTION__,__LINE__)
#else
#define print
#endif 

int *plcd=NULL;//指向帧缓冲区域的首地址
int fb_fd = -1;//帧缓冲设备的文件描述符
/*
	初始化lcd屏幕
*/
void LCD_init(void)
{
	//1.打开屏幕
	int fd=open("/dev/fb0",O_RDWR);
	if(-1 == fd)
	{
		perror("open fail:");
		return ;
	}
	fb_fd=fd;
	//2.映射
	plcd=mmap(NULL,800*480*4,PROT_WRITE,MAP_SHARED,fd,0);
	if(plcd==MAP_FAILED)
	{
		perror("mmap fail:");
		return ;
	}
}
/*
	
*/
void lcd_uninit(void)
{
	munmap(plcd,800*480*4);
	plcd = NULL;//不让plcd成为野指针
	close(fb_fd);
}
/*
	屏幕画点
*/
void LCD_Draw_Point(int x,int y,int color)
{
	if(plcd == NULL)//避免憨憨
	{
		printf("ni ge hanhan,YOU DONOT initlcd\n");
		print;
		return ;
	}
	if(x>=0&&x<800&&y>=0&&y<480)
	{
		*(plcd+800*y+x)=color;
	}
}

/*画一个圆*/
void LCD_Draw_Circle(int x0,int y0,int r,int color)
{
	
}


/*
	画一个矩形
	@x0 y0:矩形的左上角的坐标
	@w h:矩形的宽和高
	@color:矩形的颜色
*/
void LCD_Draw_Matrix(int x0,int y0,int w,int h,int color)
{
	int n,p;
	for(n=0; n<h; n++){
		for(p=0; p<w; p++){
			LCD_Draw_Point(x0,y0, color);
		}
	}
}

