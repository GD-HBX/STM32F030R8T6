//恶搞 2014-9-15 05:33:02
#include "register.h"
#include "USART1.h"
#include "log.h"
#define TAG "LOG: "
#define CRLF "\r\n"

uc turnoff = 0;///

void _log(uc* p,us d_size){
	USART1_TxDAT temp;
	if(turnoff)return;///
	temp.tx_p = TAG;
	temp.tx_size = sizeof(TAG) - 1;
	USART1_SEND(&temp);
	temp.tx_p = p;
	temp.tx_size = d_size;
	USART1_SEND(&temp);
	temp.tx_p = CRLF;
	temp.tx_size = sizeof(CRLF);
	USART1_SEND(&temp);
}
