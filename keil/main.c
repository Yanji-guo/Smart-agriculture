#include "delay.h"
#include "sys.h"
#include "led.h"
#include "lcd_init.h"
#include "lcd.h"
#include "pic.h"
#include "uart5.h"
#include "uart2.h"
#include "usart.h"
#include "usart3.h"
#include "dht11.h"


uint8_t  huanying[] = {0xFD,0x00,0x1E,0x01,0x01,0xBB,0xB6,0xD3,0xAD,0xCA,0xB9,0xD3,0xC3,0xD6,0xC7,0xBB,0xDB,0xC5,0xA9,0xD2,0xB5,0xBB,0xF9,0xB5,0xD8,0xB9,0xDC,0xC0,0xED,0xCF,0xB5,0xCD,0xB3};
uint16_t huanying_size = sizeof(huanying) / sizeof(huanying[0]);

uint8_t  chonghai[] = {0xFD,0x00,0x1E,0x01,0x01,0xBA,0xA6,0xB3,0xE6,0xC3,0xDC,0xB6,0xC8,0xB9,0xFD,0xB4,0xF3,0xBF,0xAA,0xC6,0xF4,0xB3,0xAC,0xC9,0xF9,0xB2,0xA8,0xC7,0xFD,0xB3,0xE6,0xC6,0xF7};
uint16_t chonghai_size = sizeof(chonghai) / sizeof(chonghai[0]);

uint8_t  shujuchuan[] = {0xFD,0x00,0x1C,0x01,0x01,0xBF,0xAA,0xCA,0xBC,0xBD,0xF8,0xD0,0xD0,0xCF,0xB5,0xCD,0xB3,0xD0,0xC5,0xCF,0xA2,0xB4,0xAB,0xCA,0xE4,0xD3,0xEB,0xC9,0xCF,0xB4,0xAB};
uint16_t shujuchuan_size = sizeof(shujuchuan) / sizeof(shujuchuan[0]);
	
uint8_t  shui[] = {0xFD,0x00,0x12,0x01,0x01,0xBF,0xAA,0xC6,0xF4,0xCB,0xAE,0xB1,0xC3,0xBD,0xF8,0xD0,0xD0,0xC5,0xE7,0xB9,0xE0};
uint16_t shui_size = sizeof(shui) / sizeof(shui[0]);

uint8_t  huo[] = {0xFD,0x00,0x0E,0x01,0x01,0xBE,0xAF,0xB1,0xA8,0xB7,0xA2,0xCF,0xD6,0xBB,0xF0,0xD1,0xE6};
uint16_t huo_size = sizeof(huo) / sizeof(huo[0]);

uint8_t  cong_off[] = {0xFD,0x00,0x12,0x01,0x01,0xBA,0xA6,0xB3,0xE6,0xC3,0xDC,0xB6,0xC8,0xBB,0xD6,0xB8,0xB4,0xD5,0xFD,0xB3,0xA3};
uint16_t cong_off_size = sizeof(cong_off) / sizeof(cong_off[0]);

uint8_t  guang_zheng[] = {0xFD,0x00,0x18,0x01,0x01,0xB9,0xE2,0xD5,0xD5,0xB4,0xAB,0xB8,0xD0,0xC6,0xF7,0xCA,0xFD,0xBE,0xDD,0xBB,0xD6,0xB8,0xB4,0xD5,0xFD,0xB3,0xA3};
uint16_t guang_zheng_size = sizeof(guang_zheng) / sizeof(guang_zheng[0]);

uint8_t  shui_off[] = {0xFD,0x00,0x0A,0x01,0x01,0xCB,0xAE,0xB1,0xC3,0xB9,0xD8,0xB1,0xD5};
uint16_t shui_off_size = sizeof(shui_off) / sizeof(shui_off[0]);

uint8_t  feng[] = {0xFD,0x00,0x0E,0x01,0x01,0xBE,0xAF,0xB8,0xE6,0xB7,0xE7,0xCB,0xD9,0xB9,0xFD,0xB4,0xF3};
uint16_t feng_size = sizeof(feng) / sizeof(feng[0]);

uint8_t  xiaoce[] = {0xFD,0x00,0x0E,0x01,0x01,0xBF,0xAA,0xC6,0xF4,0xCA,0xA9,0xB7,0xCA,0xD0,0xA1,0xB3,0xB5};
uint16_t xiaoce_size = sizeof(xiaoce) / sizeof(xiaoce[0]);

uint8_t banzaiwendu[] = {0xFD,0x00,0x0E,0x01,0x01,0xB0,0xE5,0xD4,0xD8,0xCE,0xC2,0xB6,0xC8,0xD2,0xEC,0xB3,0xA3};
uint16_t banzaiwendu_size = sizeof(banzaiwendu) / sizeof(banzaiwendu[0]);

uint8_t banzaishidu[] = {0xFD,0x00,0x0E,0x01,0x01,0xB0,0xE5,0xD4,0xD8,0xCA,0xAA,0xB6,0xC8,0xD2,0xEC,0xB3,0xA3};
uint16_t banzaishidu_size = sizeof(banzaishidu) / sizeof(banzaishidu[0]);

uint8_t banzaiwenshidu[] = {0xFD,0x00,0x10,0x01,0x01,0xB0,0xE5,0xD4,0xD8,0xCE,0xC2,0xCA,0xAA,0xB6,0xC8,0xD2,0xEC,0xB3,0xA3};
uint16_t banzaiwenshidu_size = sizeof(banzaiwenshidu) / sizeof(banzaiwenshidu[0]);
	
uint8_t wei_stop[] = {0x00};
uint16_t wei_stop_size = sizeof(wei_stop) / sizeof(wei_stop[0]);

uint8_t wei_on[] = {0x01};
uint16_t wei_on_size = sizeof(wei_on) / sizeof(wei_on[0]);


volatile unsigned int wendu = 0;
volatile unsigned int shidu = 0;
volatile unsigned int GZ = 0;
volatile unsigned int HZ = 0;
volatile unsigned int FS = 0;
	
volatile unsigned int bettle=0, moth=0, yellow=0, spider =0,black=0,white=0,green=0,huang=0,wenduchuan=0,shiduchuan=0,diandao=0,ph=0,diandao_z=0,ph_z=0,wd_z=0,light_z=0,shidu_z=0;
u8 wd=0, sd=0;
int shengzhang=0;
uint8_t shuju_rxBuffer[17];
uint8_t shuju5_rxBuffer[6];
char butters[600];
char stop[2];
char xunji[2];
char J = 'J';
//USART3_SendDatas(&J, 1);
// 发送字符 'A'
char A = 'A';
//USART3_SendDatas(&A, 1);

int flog=0;
int huo_z=0;
int huo_flog=0;

int USART1_SendDatas (char *pBuffer, int size)  
{  
    int i = 0;  
    for (i = 0; i < size; i++)  
    {
        while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET); 
        USART_SendData(USART1, (uint8_t) *pBuffer++); 
    }  
    return size;  
}

	int USART2_SendChar(int ch)
{
    
    while(!(USART2->SR & USART_SR_TC));
    USART2->DR = (uint8_t)ch;
    return ch;
}

void USART2_SendString(const char* str)
{
    while(*str)
    {
        USART2_SendChar(*str++);
    }
}


void USART2_IRQHandler(void) {
		static uint8_t rxindex = 0;
    if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) { 
        uint8_t Res = USART_ReceiveData(USART2); 
        USART_ClearITPendingBit(USART2, USART_IT_RXNE);
				if (rxindex < 12) {
            shuju_rxBuffer[rxindex++] = Res;
        }
				if (rxindex == 12) {
					wendu = shuju_rxBuffer[0];
					shidu = shuju_rxBuffer[1];
					GZ = shuju_rxBuffer[2];
					FS = shuju_rxBuffer[3];
					shiduchuan=shuju_rxBuffer[4];
					wenduchuan =shuju_rxBuffer[5];
					diandao = shuju_rxBuffer[6];
					ph= shuju_rxBuffer[7];
//					red =shuju_rxBuffer[8];
//					white= shuju_rxBuffer[9];
//					black =shuju_rxBuffer[10];
					huo_z=shuju_rxBuffer[11];

//					HZ = shuju_rxBuffer[8]+shuju_rxBuffer[9]+shuju_rxBuffer[10]+shuju_rxBuffer[11];
//					HZ = red+black+white;

					DHT11_Read_Data(&wd,&sd);

//					if(diandao!=0)
//					{
//						if(diandao<100&diandao>0)
//						{
//							diandao_z=diandao*2/50;
//						}
//					}
//					if(ph!=0)
//					{
//						if((ph>4&ph<=5)|(ph<10&ph>9))
//						{
//							ph_z = 2;
//						}
//						if((ph>5&ph<=6)|(ph<9&ph>8))
//						{
//							ph_z = 3;
//						}
//						if((ph>6&ph<=7)|(ph<8&ph>7))
//						{
//							ph_z = 5;
//						}
//						else
//						{
//							ph_z=0;
//						}
//					
//					}
//					if(wd!=0)
//					{
//						if(wd>0&wd<=40)
//						{
//							wd_z=3*wd/10-2;
//						}
//						if(wd>40&wd<=69)
//						{
//							wd_z=-3*wd/10+22;
//						}
//					}
//					if(GZ!=0)
//					{
//						if(GZ>0&GZ<=85)
//						{
//							light_z=2*GZ/10-5;
//						}
//						if(GZ>85&GZ<=100)
//						{
//							light_z=-2*GZ/10+29;
//						}
//					}
//					if(GZ!=0)
//					{
//						if(GZ>0&GZ<=85)
//						{
//							light_z=(3*GZ/10)-6;
//						}
//						if(GZ>85&GZ<=100)
//						{
//							light_z=-(3*GZ/10)+45;
//						}
//					}
//					if(shiduchuan!=0)
//					{
//						if(shiduchuan>0&shiduchuan<=90)
//						{
//							shidu_z=2*shiduchuan/10-5;
//						}
//						if(shiduchuan>85&shiduchuan<=100)
//						{
//							shidu_z=-2*shiduchuan/10+29;
//						}
//					}
//					shengzhang=diandao_z+ph_z+wd_z+light_z+shidu_z;
//					if (shengzhang<=0)
//					{
//						shengzhang=0;
//					}
					shuju_rxBuffer[14]=wd;
					shuju_rxBuffer[15]=sd;
//					shuju_rxBuffer[14]=shengzhang;
					
//					sprintf(butters, "{\"sensorDatas\":[{\"flag\":\"WD\",\"value\":%d},{\"flag\":\"SD\",\"value\":%d},{\"flag\":\"EC\",\"value\":%d},{\"flag\":\"PH\",\"value\":%d},{\"flag\":\"GM\",\"value\":%d},{\"flag\":\"MO\",\"value\":%d},{\"flag\":\"ZZ\",\"value\":%d},{\"flag\":\"RE\",\"value\":%d},{\"flag\":\"BC\",\"value\":%d},{\"flag\":\"WDB\",\"value\":%d},{\"flag\":\"SDB\",\"value\":%d},{\"flag\":\"WDH\",\"value\":%d},{\"flag\":\"SDH\",\"value\":%d},{\"flag\":\"SZ\",\"value\":%d},{\"flag\":\"fs\",\"value\":%d},{\"flag\":\"HU\",\"value\":%d}]}\n",wenduchuan,shiduchuan,diandao,ph,GZ,moth,black,red,white,wendu,shidu,wd,sd,shengzhang,FS,huo_z);					
//					sprintf(butters, "{\"sensorDatas\":[{\"flag\":\"WD\",\"value\":%d},{\"flag\":\"SD\",\"value\":%d},{\"flag\":\"EC\",\"value\":%d},{\"flag\":\"PH\",\"value\":%d},{\"flag\":\"GM\",\"value\":%d},{\"flag\":\"MO\",\"value\":%d},{\"flag\":\"ZZ\",\"value\":%d},{\"flag\":\"RE\",\"value\":%d},{\"flag\":\"BC\",\"value\":%d},{\"flag\":\"WDB\",\"value\":%d},{\"flag\":\"SDB\",\"value\":%d},{\"flag\":\"WDH\",\"value\":%d},{\"flag\":\"SDH\",\"value\":%d},{\"flag\":\"fs\",\"value\":%d},{\"flag\":\"HU\",\"value\":%d}]}\n",wenduchuan,shiduchuan,diandao,ph,GZ,moth,black,red,white,wendu,shidu,wd,sd,FS,huo_z);					
					sprintf(butters, "S,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,T",wenduchuan,shiduchuan,diandao,ph,GZ,wendu,shidu,wd,sd,FS,huo_z);					

//					USART2_SendDatas(butters,sizeof(butters));
					USART2_SendString(butters);
					rxindex = 0;
        }
    }
}


void UART5_IRQHandler(void) {
    static uint8_t rxindex5 = 0;
    static uint8_t frame_started = 0;     // 帧开始标志
    static uint8_t data_index = 0;        // 当前数据索引(0-4)
    static char temp_num[4];              // 临时数字字符串缓存
    static uint8_t temp_index = 0;        // 临时缓存索引
    static int data_values[5];            // 存储转换后的int数值
    
    if (USART_GetITStatus(UART5, USART_IT_RXNE) != RESET) { 
        uint8_t Res5 = USART_ReceiveData(UART5); 
        USART_ClearITPendingBit(UART5, USART_IT_RXNE);
        
        // 检测帧头 'S'
        if (Res5 == 'S' && !frame_started) {
            frame_started = 1;
            data_index = 0;
            temp_index = 0;
            memset(temp_num, 0, sizeof(temp_num));
            return;
        }
        
        // 如果帧已开始，处理接收数据
        if (frame_started) {
            // 检测帧尾 'T'
            if (Res5 == 'T') {
                // 处理最后一个数字
                if (temp_index > 0 && data_index < 5) {
                    temp_num[temp_index] = '\0';
                    data_values[data_index] = atoi(temp_num);
                    data_index++;
                }
                
                // 确保接收到5个有效数据
                if (data_index == 5) {
                    bettle = data_values[0];
                    white = data_values[1];
                    yellow = data_values[2];
                    moth = data_values[3];
                    spider = data_values[4];
                    HZ = bettle + white + yellow + moth + spider;
                }
                
                // 重置所有状态
                frame_started = 0;
                data_index = 0;
                temp_index = 0;
                memset(temp_num, 0, sizeof(temp_num));
            }
                       // 遇到逗号分隔符
            else if (Res5 == ',') {
                // 转换当前累积的数字字符串
                if (temp_index > 0 && data_index < 5) {
                    temp_num[temp_index] = '\0';
                    data_values[data_index] = atoi(temp_num);
                    data_index++;
                    temp_index = 0;
                    memset(temp_num, 0, sizeof(temp_num));
                }
            }
            // 接收数字字符
            else if (Res5 >= '0' && Res5 <= '9') {
                if (temp_index < 3) {  // 防止溢出，假设数字最大3位
                    temp_num[temp_index++] = Res5;
                }
            }
            // 遇到无效字符，重置状态
            else {
                frame_started = 0;
                data_index = 0;
                temp_index = 0;
                memset(temp_num, 0, sizeof(temp_num));
            }
        }
    }
}
						
						
						
						
//void UART5_IRQHandler(void) {
//		static uint8_t rxindex5 = 0;
//    if (USART_GetITStatus(UART5, USART_IT_RXNE) != RESET) { 
//        uint8_t Res5 = USART_ReceiveData(UART5); 
//        USART_ClearITPendingBit(UART5, USART_IT_RXNE);
//				if (rxindex5 < 5) {
//            shuju5_rxBuffer[rxindex5++] = Res5;
//        }
//				if (rxindex5 == 5) {
//					bettle = shuju5_rxBuffer[0];
//					white = shuju5_rxBuffer[1];
//					yellow = shuju5_rxBuffer[2];	
//					moth = shuju5_rxBuffer[3];	
//					spider = shuju5_rxBuffer[4];	
//					rxindex5 = 0;
//					HZ = bettle+white+yellow+moth+spider;

//        }
//    }
//}



void show()
{
	LCD_Fill(0,0,LCD_W,LCD_H,BLACK);
	LCD_ShowString(1,0,"tem1:",WHITE,BLACK,16,0);
	LCD_ShowString(1,20,"hum1:",WHITE,BLACK,16,0);
	LCD_ShowString(69,0,"tem2:",WHITE,BLACK,16,0);
	LCD_ShowString(69,20,"hum2:",WHITE,BLACK,16,0);
	LCD_ShowString(1,40,"lig:",WHITE,BLACK,16,0);
	LCD_ShowString(69,40,"fan:",WHITE,BLACK,16,0);
//	LCD_ShowString(1,60,"red:",WHITE,BLACK,16,0);
//	LCD_ShowString(69,60,"bla:",WHITE,BLACK,16,0);
//	LCD_ShowString(1,80,"whi:",WHITE,BLACK,16,0);
//	LCD_ShowString(69,80,"gre:",WHITE,BLACK,16,0);
//	LCD_ShowString(1,100,"hua:",WHITE,BLACK,16,0);
//	LCD_ShowString(1,100,"tem3:",WHITE,BLACK,16,0);
//	LCD_ShowString(69,100,"hum3:",WHITE,BLACK,16,0);
//	LCD_ShowString(1,120,"dia:",WHITE,BLACK,16,0);
//	LCD_ShowString(69,120,"phz:",WHITE,BLACK,16,0);
	
	
	LCD_ShowString(1,60,"bet:",WHITE,BLACK,16,0);
	LCD_ShowString(69,60,"whi:",WHITE,BLACK,16,0);
	LCD_ShowString(1,80,"yel:",WHITE,BLACK,16,0);
	LCD_ShowString(69,80,"mot:",WHITE,BLACK,16,0);
	LCD_ShowString(1,100,"spi:",WHITE,BLACK,16,0);
	LCD_ShowString(69,100,"wor:",WHITE,BLACK,16,0);	
	LCD_ShowString(1,120,"tem3:",WHITE,BLACK,16,0);
	LCD_ShowString(69,120,"hum3:",WHITE,BLACK,16,0);
	LCD_ShowString(1,140,"dia:",WHITE,BLACK,16,0);
	LCD_ShowString(69,140,"phz:",WHITE,BLACK,16,0);
	
//	LCD_ShowIntNum(40,0,00,2,WHITE,BLACK,16);
//	LCD_ShowIntNum(40,20,00,2,WHITE,BLACK,16);
//	LCD_ShowIntNum(109,0,00,2,WHITE,BLACK,16);
//	LCD_ShowIntNum(109,20,00,2,WHITE,BLACK,16);
//	LCD_ShowIntNum(109,40,00,2,WHITE,BLACK,16);
//	LCD_ShowIntNum(109,60,00,2,WHITE,BLACK,16);
////	LCD_ShowIntNum(109,80,00,2,WHITE,BLACK,16);
//	LCD_ShowIntNum(40,40,00,2,WHITE,BLACK,16);///
//	LCD_ShowIntNum(40,60,00,2,WHITE,BLACK,16);
//	LCD_ShowIntNum(40,80,00,2,WHITE,BLACK,16);
////	LCD_ShowIntNum(40,100,00,2,WHITE,BLACK,16);
//	LCD_ShowIntNum(109,100,00,2,WHITE,BLACK,16);
//	LCD_ShowIntNum(40,120,00,2,WHITE,BLACK,16);
//	LCD_ShowIntNum(109,120,00,2,WHITE,BLACK,16);
//	LCD_ShowIntNum(40,140,00,2,WHITE,BLACK,16);
//	LCD_ShowIntNum(109,140,00,2,WHITE,BLACK,16);
}
int main(void)
{ 
	
	delay_init(168);
	uart_init(9600);
	uart5_init(9600);
	uart2_init(9600);
	uart3_init(9600);
	LED_Init();//LED初始化
	LCD_Init();//LCD初始化
	shui_Init();
	DHT11_Init();
	LED0=0;
	show();
	USART5_SendDatas(huanying,huanying_size);
	delay_ms(4200);	
	USART5_SendDatas(shujuchuan,shujuchuan_size);
	delay_ms(3000);	
	
	while(1) 
	{

		if(huo_z==1)
		{
				USART5_SendDatas(huo,huo_size);
			  LCD_ShowPicture(0, 0 , 124, 155, gImage_huozai);
				delay_ms(2500);	          
			  huo_flog=1;
		}
			if(huo_z==0)
		{
			if(huo_flog==1)
				{
					show();
				}
			huo_flog = 0;
			if(shiduchuan<10&sd<=46)
			{
					GPIO_SetBits(GPIOF,GPIO_Pin_7);

			}
			if(shiduchuan>=50|sd>49)
			{
					GPIO_ResetBits(GPIOF,GPIO_Pin_7);
			}	
			if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_3)==1&flog==0)
			{
				 USART5_SendDatas(shui,shui_size);
					flog = 1;
				delay_ms(2300);
			}
				if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_3)==0&flog==1)
			{
				 USART5_SendDatas(shui_off,shui_off_size);
					flog = 0;
				delay_ms(1800);
			}
			
//			if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_4)==1)      //小车的
//			{
//				USART5_SendDatas(xiaoce,xiaoce_size);
////				sprintf(xunji, "J");
//				USART3_SendDatas(&J, 1);				delay_ms(2300);
//			  sprintf(stop, "A");
//			  USART3_SendDatas(&A, 1);           //写反了
//			}
	//		DHT11_Read_Data(&wd,&sd);
			LCD_ShowIntNum(109,0,wd,2,WHITE,BLACK,16);
			LCD_ShowIntNum(109,20,sd,2,WHITE,BLACK,16);
			LCD_ShowIntNum(40,60,bettle,2,WHITE,BLACK,16);
			LCD_ShowIntNum(109,60,white,2,WHITE,BLACK,16);
			LCD_ShowIntNum(40,80,yellow,2,WHITE,BLACK,16);			
			LCD_ShowIntNum(109,80,moth,2,WHITE,BLACK,16);
			LCD_ShowIntNum(40,100,spider,2,WHITE,BLACK,16);
			
//			LCD_ShowIntNum(40,60,red,2,WHITE,BLACK,16);
//			LCD_ShowIntNum(109,60,black,2,WHITE,BLACK,16);
//			LCD_ShowIntNum(40,80,white,2,WHITE,BLACK,16);
//			LCD_ShowIntNum(109,80,green,2,WHITE,BLACK,16);
//			LCD_ShowIntNum(40,100,huang,2,WHITE,BLACK,16);
			LCD_ShowIntNum(40,120,wenduchuan,2,WHITE,BLACK,16);
			LCD_ShowIntNum(109,120,shiduchuan,2,WHITE,BLACK,16);
			LCD_ShowIntNum(40,140,diandao,2,WHITE,BLACK,16);
			LCD_ShowIntNum(109,140,ph,2,WHITE,BLACK,16);
			LCD_ShowIntNum(40,40,GZ,2,WHITE,BLACK,16);
			if(wendu!=0 & shidu!=0)
			{
				if(wendu<=60&shidu<=95)
				{
					LCD_ShowIntNum(40,0,wendu,2,WHITE,BLACK,16);
					LCD_ShowIntNum(40,20,shidu,2,WHITE,BLACK,16);
				}
				if(wendu>60&shidu<=95&wd<50)      ////对比的是板载和环境的温度值，看板载的温度是否在合理范围内
				{
					LCD_ShowIntNum(40,20,shidu,2,WHITE,BLACK,16);
					LCD_ShowIntNum(40,0,wendu,2,RED,BLACK,16);
					USART5_SendDatas(banzaiwendu,banzaiwendu_size);
					delay_ms(2000);
				}
				if(wendu<=60&shidu>95)          ////对比的是板载和环境的湿度值，看板载的湿度是否在合理范围内
				{
					LCD_ShowIntNum(40,0,wendu,2,WHITE,BLACK,16);
					LCD_ShowIntNum(40,20,shidu,2,RED,BLACK,16);
//					USART5_SendDatas(banzaishidu,banzaishidu_size);
					delay_ms(2000);
				}
				if(wendu>60&shidu>95&sd<69)
				{
					LCD_ShowIntNum(40,0,wendu,2,RED,BLACK,16);
					LCD_ShowIntNum(40,20,shidu,2,RED,BLACK,16);
					USART5_SendDatas(banzaiwenshidu,banzaiwenshidu_size);
					delay_ms(2000);
				}
			}
			if(HZ!=0)
			{
				if(HZ<=12)
				{
					USART1_SendDatas(wei_stop,wei_on_size);

//					GPIO_ResetBits(GPIOF,GPIO_Pin_11);
					LCD_ShowIntNum(109,100,HZ,2,WHITE,BLACK,16);
				}
				if(HZ>12)
				{
					USART1_SendDatas(wei_on,wei_on_size);

					LCD_ShowIntNum(109,100,HZ,2,RED,BLACK,16);
//					GPIO_SetBits(GPIOF,GPIO_Pin_11);
					USART5_SendDatas(chonghai,chonghai_size);
					delay_ms(4500);
				}
			}
			if(FS!=0)
			{
				if(FS<=9)
				{
	//				GPIO_ResetBits(GPIOF,GPIO_Pin_11);
					LCD_ShowIntNum(109,40,FS,2,WHITE,BLACK,16);
				}
				if(FS>9)
				{
					LCD_ShowIntNum(109,40,FS,2,RED,BLACK,16);
	//				GPIO_SetBits(GPIOF,GPIO_Pin_11);
					USART5_SendDatas(feng,feng_size);
					delay_ms(2500);
				}
			}
		}
	}
}

