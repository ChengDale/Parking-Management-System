#include <linux/input.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/ioctl.h>

extern int x,y;

/*
	@get_xy获取手指触摸的坐标值
	//
*/

void get_xy()
{

	struct input_absinfo absI;
	//1.打开触摸屏
	int fd=open("/dev/input/event0",O_RDONLY);
	if(-1 == fd)
	{
		perror("open fail");
		return ;
	}
	//2.获取用户输入
	struct input_event ev;
	int flag1=0;
	int flag2=0;
	int flag=0;

	//得到x轴的abs信息
	ioctl(fd,EVIOCGABS(ABS_X),&absI);
	printf("x abs min = %d\n",absI.minimum);
	printf("x abs max = %d\n",absI.maximum);

	//得到y轴的abs信息
	ioctl(fd,EVIOCGABS(ABS_Y),&absI);
	printf("y abs min = %d\n",absI.minimum);
	printf("y abs max = %d\n",absI.maximum);

	//得到按压轴的abs信息
	ioctl(fd,EVIOCGABS(ABS_PRESSURE),&absI);
	printf("PRESSURE abs min = %d\n",absI.minimum);
	printf("PRESSURE abs max = %d\n",absI.maximum);
	
	while(1)
	{
		int res=read(fd,&ev,sizeof(ev));
		if(res!=sizeof(ev))
		{
			continue;
		}
		if(ev.type==EV_ABS&&ev.code==ABS_X)//获取x轴
		{
			x=ev.value;
			flag=1;
			/*if(x>=200&&x<=300){
				flag1 = 1;//表示获取到了x轴
			}
			if(x>=500&&x<=600){
				flag1 = 2;
			}
			if(x>=700&&x<=800){
				flag1 = 3;
			}**/
		}
		if(ev.type==EV_ABS&&ev.code==ABS_Y)//获取y轴
		{
			y=ev.value;
			/*if(y>=360&&y<=440){
				flag2 = 1;
			}
			if(y>=200&&y<=280){
				flag2 = 2;
			}**/
			//只有x , y都获取了 那么才会退出这个死循环
			if(flag)
			{
				break;
			}
		}
		
		/*if(1== flag1 && flag2 == 1){
			return 1;
		}else if(2== flag1 && flag2 == 1){
			return 2;
		}else if(3==flag1 && flag2 == 2){
			return 3;
		}**/
	}
	printf("x=%d,y=%d\n",x,y);
	close(fd);
}
