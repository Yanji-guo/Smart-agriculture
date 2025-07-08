#include <rtthread.h>

#define DBG_TAG "main"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>
#include <rtdevice.h>
#include <board.h>
#include <serial.h>                       //待添加的功能有，超声波测距（水位，bug），风速（ADC），土壤传感器（串口）。

#include "sensor_asair_aht10.h"
#define AHT10_I2C_BUS "i2c4"

uint8_t  wenxun[] = {0x01,0x03,0x00,0x00,0x00,0x04,0x44,0x09};
uint16_t wenxun_size = sizeof(wenxun) / sizeof(wenxun[0]);
uint8_t  shuju[] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
////第一二为板载温湿度，三四为光照和风速，五六七八为土壤传感器的温湿度，电导，pH，9,10,11,12,13,14,15,16为摄像头的害虫数据
uint16_t shuju_size = sizeof(shuju) / sizeof(shuju[0]);

rt_adc_device_t  dev;
rt_thread_t th1;       //adc线程
rt_thread_t uart2;       //uart2线程
rt_thread_t uart3;       //uart3线程
rt_thread_t uart4;       //uart4线程
rt_thread_t aht;       //aht10线程
rt_thread_t shui;       //水泵线程
struct serial_configure u2_configs = RT_SERIAL_CONFIG_DEFAULT2;
rt_device_t t_uart2;
rt_device_t t_uart3;
rt_device_t t_uart4;

char rec_str[13];
char rec_str2[6];
rt_uint8_t header = 0xFF;
rt_uint8_t footer = 0xFE;

int shidu,wendu,diandao,ph,wendu_b,shidu_b,red,black, white, green,huang;
int shiduzhuanhua;
int map_light_value(int sensor_value)
{
    return 100 - (sensor_value - 30) * 100 / 4070;
}
//int map_to_range(int value) {
//    if (value <= 0) return 0;
//    if (value >= 66000) return 100;
//return (value / 66000) * 100;
//}
#define ADC_MAX 2000
#define VOLTAGE_MAX 3.3
#define WIND_SPEED_MAX 30.0

float calculate_wind_speed(int adc_value) {
    float voltage = (adc_value / (float) ADC_MAX) * VOLTAGE_MAX;
    float wind_speed = (voltage / VOLTAGE_MAX) * WIND_SPEED_MAX;

    return wind_speed;
}

void read_adc1_entry(void *parameter)
{
    rt_uint32_t num = 0;
    rt_uint32_t num4 = 0;
    rt_uint32_t num7 = 0;
    rt_uint32_t huo = 0;
    while(1)
    {
        num = rt_adc_read(dev, 5);
        int light_value1 = map_light_value(num);
//        rt_kprintf("num:%d",num);
//        rt_kprintf("light_value1:%d \n",light_value1);
        shuju[2] = light_value1;
        num4 = rt_adc_read(dev, 4);
        int fan_value = calculate_wind_speed(num4);
        shuju[3] =fan_value;
//        rt_kprintf("fan_value:%d \n",fan_value);
//        num7 = rt_adc_read(dev, 7);
//        if (num7>2500)
//        {
//            huo =0;
//        }
//        if (num7<=2500)
//        {
//            huo =1;
//        }
        int pin_value = rt_pin_read(GET_PIN(A,7));
        if (pin_value == PIN_LOW)
        {
            huo = 0;
        }
        else if (pin_value == PIN_HIGH)
        {
            huo = 1;
        }
        shuju[11] = huo;
        rt_kprintf("huo:%d \n",huo);
        rt_thread_delay(1000);
    }
}

rt_err_t rx2_callback(rt_device_t dev,rt_size_t size)
{
    return RT_EOK;
}
void read_uart2_entry(void *parameter)
{
    while(1)
    {
        rt_device_read(t_uart2, 0, &rec_str2, 7);
        if (rec_str2[0] == header && rec_str2[5] == footer)
        {
            red = rec_str2[1];
            white = rec_str2[2];
            black= rec_str2[3];
            green= rec_str2[4];
    //        huang= rec_str2[5];
            shuju[8]= red;
            shuju[9]=black;
            shuju[10]= white;
        }
//        shuju[11] = green;
//        shuju[12] = huang;
        //rt_kprintf("red:%d  white: %d  black:%d  green:%d huang:%d\n",red,white,black,green,huang);
        rt_thread_delay(1000);
     }
}

rt_err_t rx3_callback(rt_device_t dev,rt_size_t size)
{
    return RT_EOK;
}
void read_uart3_entry(void *parameter)
{
    while(1)
    {
        rt_device_read(t_uart3, 0, &rec_str, 13);
        rt_device_write(t_uart3, 0, wenxun, wenxun_size);

//        int shidu,wendu,diandao,ph;
//        shidu = (rec_str[3]*256 + rec_str[4])/10;
//        wendu = (rec_str[5]*256 + rec_str[6])/10;
//        diandao = rec_str[7]*256 + rec_str[8];
//        ph = (rec_str[9]*256 + rec_str[10])/10;
//        shuju[4] = shidu;
//        shuju[5] = wendu;
//        shuju[6] = diandao/100;
//        shuju[7] = ph;

        shidu = (rec_str[3] << 16) | rec_str[4];
        wendu = (rec_str[5] << 16) | rec_str[6];
        diandao = (rec_str[7] << 16) | rec_str[8];
        ph = (rec_str[9] << 16) | rec_str[10];
        shuju[4] = shidu/60;
        shiduzhuanhua = shidu/60;
//        int shidu_s = (shidu* 100 / 66000) ;
        shuju[5] = wendu/10;
        shuju[6] = diandao/100;
        shuju[7] = ph/10;
//        rt_kprintf("shidu number is %d,wendu number is %d,diandao number is %d,ph number is %d\n",shidu,wendu,diandao,ph);
        rt_kprintf("%d %d %d %d\n",wendu,shiduzhuanhua,diandao,ph);

        rt_thread_delay(1000);
     }
}

rt_err_t rx4_callback(rt_device_t dev,rt_size_t size)
{
    return RT_EOK;
}
void read_uart4_entry(void *parameter)
{
    while(1)
    {
//        rt_device_read(t_uart4, 0, &rec_str, 13);
        rt_device_write(t_uart4, 0, shuju, shuju_size);
        rt_thread_delay(1300);
     }
}


void aht_entry(void *parameter)
{
    struct rt_sensor_data aht10_temp,aht10_humi;
    rt_device_t aht10,aht10humi;
    aht10 = rt_device_find("temp_aht10");
    aht10humi = rt_device_find("humi_aht10");
    rt_device_open(aht10,RT_DEVICE_FLAG_RDWR);
    rt_device_open(aht10humi,RT_DEVICE_FLAG_RDWR);

    while(1)
    {
        rt_device_read(aht10, 0, &aht10_temp, 1);
        rt_device_read(aht10humi, 0, &aht10_humi, 1);
        wendu_b = aht10_temp.data.temp/10;
        shidu_b = aht10_humi.data.humi/10;
        shuju[0] = wendu_b;
        shuju[1] = shidu_b;
//        rt_kprintf("temp:%d  humi:%d\n",wendu_b,shidu_b);
        rt_thread_delay(1000);
    }

}

int rt_hw_aht10_port(void)
{
    struct rt_sensor_config cfg;
    cfg.intf.dev_name = AHT10_I2C_BUS;
    cfg.intf.user_data = (void *)AHT10_I2C_ADDR;
    rt_hw_aht10_init("aht10", &cfg);

    return RT_EOK;
}
INIT_ENV_EXPORT(rt_hw_aht10_port);

//void shui_entry(void *parameter)
//{
////    if (shiduzhuanhua<50)
////    {
////        rt_pin_write(GET_PIN(D,11), PIN_HIGH);
////    }
////    if (shiduzhuanhua>200)
////    {
////        rt_pin_write(GET_PIN(D,11), PIN_LOW);
////    }
//}


int main(void)
{
    rt_err_t ret= 0,ret2=0;
    rt_pin_mode(GET_PIN(B,0),PIN_MODE_OUTPUT);
    rt_pin_mode(GET_PIN(A,6),PIN_MODE_OUTPUT);
    rt_pin_mode(GET_PIN(D,11),PIN_MODE_OUTPUT);
    rt_pin_mode(GET_PIN(A,7),PIN_MODE_INPUT);
//    rt_pin_write(GET_PIN(A,7), PIN_LOW);
    rt_pin_write(GET_PIN(B,0), PIN_HIGH);
    rt_thread_mdelay(500);
    rt_pin_write(GET_PIN(B,0), 0);
    dev = (rt_adc_device_t)rt_device_find("adc1");
    ret = rt_adc_enable(dev,5);
    rt_adc_enable(dev,4);

    if(ret <0)
    {
        LOG_E("adc1,filed\n");
    }
    th1 = rt_thread_create("th1", read_adc1_entry, NULL, 512, 10, 5);
    if(th1 == NULL)
    {
        LOG_E("field\n");
    }
    rt_thread_startup(th1);


    /////////////
    t_uart2 = rt_device_find("uart2");
    rt_device_open(t_uart2, RT_DEVICE_OFLAG_RDWR|RT_DEVICE_FLAG_INT_RX);
    rt_device_control(t_uart2, RT_DEVICE_CTRL_CONFIG, (void *)&u2_configs);

    rt_device_set_rx_indicate(t_uart2, rx2_callback);
    uart2 = rt_thread_create("uart2", read_uart2_entry, RT_NULL, 512, 15, 5);
    if (uart2 !=RT_NULL)
    {
        rt_thread_startup(uart2);
    }

    t_uart3 = rt_device_find("uart3");
    rt_device_open(t_uart3, RT_DEVICE_OFLAG_RDWR|RT_DEVICE_FLAG_INT_RX);
    rt_device_control(t_uart3, RT_DEVICE_CTRL_CONFIG, (void *)&u2_configs);

    rt_device_set_rx_indicate(t_uart3, rx3_callback);
    uart3 = rt_thread_create("uart3", read_uart3_entry, RT_NULL, 512, 15, 5);
    if (uart3 !=RT_NULL)
    {
        rt_thread_startup(uart3);
    }

    t_uart4 = rt_device_find("uart4");
    rt_device_open(t_uart4, RT_DEVICE_OFLAG_RDWR|RT_DEVICE_FLAG_INT_RX);
    rt_device_control(t_uart4, RT_DEVICE_CTRL_CONFIG, (void *)&u2_configs);

    rt_device_set_rx_indicate(t_uart4, rx4_callback);
    uart4 = rt_thread_create("uart4", read_uart4_entry, RT_NULL, 512, 15, 5);
    if (uart4 !=RT_NULL)
    {
        rt_thread_startup(uart4);
    }


    ////////////
    aht = rt_thread_create("aht", aht_entry, NULL, 1024, 10, 10);
    rt_thread_startup(aht);
//    shui = rt_thread_create("shui", shui_entry, NULL, 1024, 10, 10);
//    rt_thread_startup(shui);

    return RT_EOK;
}

