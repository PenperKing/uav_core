#include "include.h"
#include "uwb.h"
#include "Drv_usart.h"

u8 UWB_data[8] = {0};
uwb_location_t uwb_location = {0,0,0,0,0,0};

// 3 is tag
void uwb_Init (void)
{
	 Uart5_Init(500000);			
}
//uwb_protoaol:
/*
* AA AA 32 LEN TAG X Y Z CHECKSUM
*/
void protocol_uwb_location(u8 com_data)
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
			if (com_data == 0x32)
				state++;
			else 
				state = 0;
		}break;
		case 3:
		{
				static u8 byte_cnts = 0;
				UWB_data[byte_cnts++] = com_data;
				if (byte_cnts >= 8)
				{
					state++;
					byte_cnts = 0;
				}
		}break;
		case 4:
		{
			u8 check_sum = (u8)(0xAA + 0xAA + 0x32);
			for (int i = 0; i < 8; i++)
			{
				check_sum += UWB_data[i];
			}
			if (check_sum == com_data && UWB_data[0] == 0x07)
			{
				uwb_location.flag = 1;
				uwb_location.cnt++;
				uwb_location.tag_num = UWB_data[1];
				uwb_location.x = (int16_t)(*(UWB_data+2)<<8)|*(UWB_data+3);
				uwb_location.y = (int16_t)(*(UWB_data+4)<<8)|*(UWB_data+5);;
				uwb_location.z = (int16_t)(*(UWB_data+6)<<8)|*(UWB_data+7);;
			}
			state = 0;
		}break;
		default:
		{
			state = 0;
		}break;
	}
}
