#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "LCD12864.h"

//ALIENTEK 探索者STM32F407开发板 实验1
//跑马灯实验 -库函数版本
//技术支持：www.openedv.com
//淘宝店铺：http://eboard.taobao.com  
//广州市星翼电子科技有限公司  
//作者：正点原子 @ALIENTEK

int main(void)
{ 
	double i=0;
	long long int j=0;
	delay_init(168);		  //初始化延时函数
	LCD12864_PORT_init();
	LCD_init();
	LCD_TClr();
	LCD_TLine(0,0,"Happy birthday");
	LCD_TLine(1,2,"to you!");
	LCD_TLine(2,0,"My dear country.");
	while(1)
	{             
		LCD_TData(3,0,i);
		for(j=0;j<100000;j++)
		{
			j=j;
		}
		i++;
		if(i>=10000) i=0;
	}
}
