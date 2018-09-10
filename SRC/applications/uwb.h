#ifndef __uwb_h
#define __uwb_h
#include "stm32f4xx.h"

typedef struct _xyz
{
	int16_t x;
	int16_t y;
	int16_t z;
	u8 tag_num;
	u8 flag;
	u8 cnt;
}uwb_location_t;
extern uwb_location_t uwb_location;
void uwb_Init (void);
void protocol_uwb_location(u8 com_data);
#endif
