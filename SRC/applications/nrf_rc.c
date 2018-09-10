#include "include.h"
#include "nrf_rc.h"
#include "Drv_usart.h"
#include "Drv_pwm_in.h"
#include "Ano_RC.h"
//0xAA, 0XAA, 0X69, len,u16_ch1, u16_ch2,u16_ch3,u16_ch4,u16_ch5...u16_ch8, check_sum
#define DATA_LEN 17
u8 nrf_data[DATA_LEN] = {0};
//eg:AA AA 69 11 01 33 02 33 03 33 04 33 05 33 06 33 07 33 08 33 8A
void protocol_nrf_rc_data(u8 com_data)
{
	static u8 state = 0;
	switch (state)
	{
		case 0: 
		{
			if (com_data == 0xAA)
				state++;
		}break;
		case 1:
		{
			if (com_data == 0xAA)
				state++;
			else
				state = 0;
		}break;
		case 2:
		{
			if (com_data == 0x69)
				state++;
			else 
				state = 0;
		}break;
		case 3:
		{
			static u8 byte_cnts = 0;
			ch_watch_dog_feed(byte_cnts);//通道检测喂狗
			nrf_data[byte_cnts++] = com_data;
			if (byte_cnts >= DATA_LEN)
			{
				state++;
				byte_cnts = 0;
			}
		}break;
		case 4:
		{
			u8 check_sum = (u8)(0xAA + 0xAA + 0x69);
			static u8 s_get_cnt = 0;
			for (int i = 0; i < DATA_LEN; i++)
			{
				check_sum += nrf_data[i];
			}
			if (check_sum == com_data && nrf_data[0] == DATA_LEN)
			{
				s_get_cnt++;//jlink 调试标志
				for (int i = 0; i < 8; i++)
					Rc_Pwm_In[i] = (u16)(*(nrf_data+2*i+1)<<8)|*(nrf_data+2*i+2);
			}
			state = 0;
		}break;
		default:
		{
			state = 0;
		}break;
	}
}

