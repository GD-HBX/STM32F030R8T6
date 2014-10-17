#include "my_lib/register.h"
#include "my_lib/DMA.h"
#include "ADC.h"
#include "TIMx.h"
#include "EXTI.h"
#include "NVIC.h"
#include "RTC.h"
#include "ASCII.h"
#include "my_lib/I2C.h"
extern ui result;
extern void delay(int);

uc a,b,c,d,e,f=0xff,ss[] = "2014-04-25-20-32-00";
ui TR,DR;

void TEST(uc ts){
	uc _long;
	I2C_data p ;//= {i2c_tx,0x50,sizeof(data),data,yes_auto};
	_long = sizeof(ts);
	p._dir = i2c_tx;
	p.addr = _24c02naddress;
	p.d_size = sizeof(ts);
	p.p = &ts;
	p._autoend = yes_auto;
	I2C1_MASTER_TRANSFER(&p);
}
