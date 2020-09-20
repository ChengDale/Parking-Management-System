#include "lcd.h"
#include "bmp.h"
#include "ts.h"
#include "touch.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <linux/input.h>
#include <termios.h>
#include <stdlib.h>
#include <time.h>

int x=-1,y=-1;

int init_serial(char *com,int speed)
{
	int fd = open(com,O_RDWR);
	if(-1 == fd)
	{
		perror("open serial error");
		exit(-1);//直接结束程序
	}
	struct termios t;
	t.c_cflag |= (CREAD | CLOCAL);//本地模式，可接受数据
	t.c_cflag &= ~CSTOPB;//停止位1
	t.c_cflag &= ~CSIZE;//清空数据位
	t.c_cflag |= CS8;//设置8位数据位
	t.c_cflag &= ~PARENB;//不要奇偶校检
	t.c_cflag &= ~CRTSCTS;//不要流控
	switch(speed)//设置波特率
	{
		case 9600:
			cfsetspeed(&t, 9600);
			break;
		case 19200:
			cfsetspeed(&t, 19200);
			break;
		case 115200:
			cfsetspeed(&t, 115200);
			break;
	}	
	tcflush(fd,TCIFLUSH);//清空输入缓存区内容
	tcsetattr(fd,TCSANOW,&t);//设置串口属性	
	return fd;	
}

int get_dis_num(){
	int fd=init_serial("/dev/ttySAC1",9600);
	//while(1)
	//{
		//计算距离
		unsigned char s[2];
		int dis_t=-1;
		char b=0x55;
		write(fd,&b,1);
		sleep(1);
		if(read(fd,&s,2)){
			int dis=s[0]*256+s[1];
			dis_t = dis;
		    printf("distance=%dcm\n",dis/10);
		}

	//}
	close(fd);
	return dis_t/10;
	
}


void draw_Carnums(int cou_numbers){
	int m=0;
	int count_numbers = 16 - cou_numbers;
	for(m=0; m<17; m++){
		draw_word(60+m*30,240,24,19,0xffee, text[m]);
	}
	int div=0;
	int ind=0;
	div = count_numbers / 10;
	ind = count_numbers % 10;
	//LCD_Draw_Matrix(555,220,50,50,0xffee);  //w 16 h 19
	draw_word(60+17*30,240,16,19,0xff00, textnums[div]);
	draw_word(60+18*30,240,16,19,0xff00, textnums[ind]);
	draw_word(60+19*30,240,24,19,0xffee, text[17]);
	
}

void draw_proname(){
	int e, r, t,u;
	srand(time(NULL));
	if(rand()%4==0){
		display_bmp(0, 0,"car1.bmp");
	}else if(rand()%4==1){
		display_bmp(0, 0,"car2.bmp");
	}else if(rand()%4==2){
		display_bmp(0, 0,"car3.bmp");
	}else if(rand()%4==3){
		display_bmp(0, 0,"car4.bmp");
	}else{
		for(t=0;t<480;t++){
			for(u=0;u<800;u++){
				LCD_Draw_Point(u,t,0xff00);
			}
		}
	}
	for(e=0;e<7;e++){
		draw_word(220+50*e,60,48,48,0xff4500,pro_show[e]);
	}
	for(r=0;r<9;r++){
		draw_word(520+20*r,130,24,19,0x080808,names_show[r]);
	}
}

int main(void)
{
	int get_nums=0;
	int temp_nums=0;
	int get_xyloc = 0;
	int disnums = 0;
	int car_pos[16] = {0};
	int count_nums = 0;
	//1.初始化屏幕
	LCD_init();
	//2.操作屏幕
	int i,j,k;
	
	
f1:	draw_proname();
	display_bmp(200, 360, "1.bmp");
	display_bmp(500, 360, "2.bmp");
	get_xy();
	//printf("%d-------%d\n",x,y);
	if(x>=200&&x<=300 && y>=360 && y<=440){
		display_bmp(0, 0,"pos_1.bmp");
		display_bmp(700, 200, "3.bmp");
		
f2:		for(k=0; k<16; k++){
			if(car_pos[k] == 1 && k>=0&&k<=7){
				display_bmp(k*100, 0,"car_1.bmp");
				count_nums++;
			}else if(car_pos[k] == 1 && k>=8&&k<=15){
				display_bmp((k%8)*100, 330,"car_1.bmp");
				count_nums++;
			}

		}		
		draw_Carnums(count_nums);
		count_nums = 0;
		
		get_xy();
		if(x>=700&&x<=800 && y>=200&&y<=280){
			goto f1;
		}else if(y>=0 && y<=150){
			for(i=0; i<8; i++){
			    if(x >= i*100 && x <= (i+1)*100){
					while(1){
                        disnums = get_dis_num();
						if(disnums <= 6){
							display_bmp(i*100, 0,"car_1.bmp");
							disnums = 0;
							car_pos[i] = 1;
							display_bmp(0, 0,"pos_1.bmp");
		                    display_bmp(700, 200, "3.bmp");
							goto f2;
						}else{
							car_pos[i] = 0;
							disnums = 0;
							display_bmp(0, 0,"pos_1.bmp");
		                    display_bmp(700, 200, "3.bmp");
							goto f2;
						}
					}
			    }
		    }
		}else if(y>=330 && y<=480){
			for(j=0; j<8; j++){
			    if(x >= j*100 && x <= (j+1)*100){
				   while(1){
                        disnums = get_dis_num();
						if(disnums <= 6){
							display_bmp(j*100, 330,"car_1.bmp");
							disnums = 0;
							car_pos[j+8] = 1;
							display_bmp(0, 0,"pos_1.bmp");
		                    display_bmp(700, 200, "3.bmp");
							goto f2;
						}else{
							car_pos[j+8] = 0;
							disnums = 0;
							display_bmp(0, 0,"pos_1.bmp");
		                    display_bmp(700, 200, "3.bmp");
							goto f2;
						}
					}
			    }
		    }
		}else{
			goto f2;
		}
	}else if(x>=500&&x<=600 && y>=360&&y<=440){
f3:		display_bmp(0, 0,"pos_1.bmp");
		//for(i=0;i<2;i++){
			for(j=0;j<8;j++){
				if(j%2 == 1){
					display_bmp(j*100, 0,"car_1.bmp");
					display_bmp(j*100, 330,"car_1.bmp");
				}else{
					display_bmp(j*100, 0,"car_2.bmp");
					display_bmp(j*100, 330,"car_2.bmp");
				}
			}
		//}
		display_bmp(700, 200, "3.bmp");
		get_xy();
		if(x>=700&&x<=800 && y>=200&&y<=280){
			goto f1;
		}else{
			goto f3;
		}
	}else{
		goto f1;
	}
	
	//3.关闭屏幕
	lcd_uninit();
	return 0;
}
