#include "my_lib/register.h"
#include "ASCII.h"

ui ASCII2CHAR(uc *p){//因为编译器的每个字符都是32位的
	if((uc)*p < '0' || (uc)*p > '9')return 0xFF;
	switch(*p - '0'){
		case 0:return 0;
		case 1:return 1;
		case 2:return 2;
		case 3:return 3;
		case 4:return 4;
		case 5:return 5;
		case 6:return 6;
		case 7:return 7;
		case 8:return 8;
		case 9:return 9;
		default :return 0xFF;
	}
}


ui CHAR2ASCII(uc *p,ui temp){
	if(p != 0){
		if(*p >= 0xA)return 0xFF;
		switch(*p){
			case 0:return '0';
			case 1:return '1';
			case 2:return '2';
			case 3:return '3';
			case 4:return '4';
			case 5:return '5';
			case 6:return '6';
			case 7:return '7';
			case 8:return '8';
			case 9:return '9';
			default :return 0xFF;
		}
	}else{
		if(temp >= 0xA)return 0xFF;
		switch(temp){
			case 0:return '0';
			case 1:return '1';
			case 2:return '2';
			case 3:return '3';
			case 4:return '4';
			case 5:return '5';
			case 6:return '6';
			case 7:return '7';
			case 8:return '8';
			case 9:return '9';
			default :return 0xFF;
		}
	}
		
}

