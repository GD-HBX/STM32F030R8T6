 THUMB
 AREA init_stack,DATA,NOINIT
s_size EQU 0x200
 SPACE s_size
stack_End 
 
 AREA Vector,DATA,READONLY
 DCD stack_End
 DCD Start_up
 IMPORT BUTTON_PA0_DOWN [WEAK]
 IMPORT CUOWU [WEAK]
 IMPORT DMA1_Handler [WEAK]
 IMPORT DMA2_3_Handler ;[WEAK];我草，加了[WEAK]后居然不把DMA2_3_Handler的值放到向量表里(此函数在DMA.c中有声明的)，无语
 IMPORT I2C1_Handler [WEAK]
 IMPORT SPI1_Handler [WEAK]
 IMPORT USART1_Handler [WEAK]
 DCD 0,CUOWU,0,0,0,0,0,0,0,0,0,0,0,0
 DCD 0,0,0,0,0,BUTTON_PA0_DOWN,0,0,0,DMA1_Handler,DMA2_3_Handler,0,0,0,0,0,0,0,0,0,0,0,0,I2C1_Handler,0,SPI1_Handler,0,USART1_Handler
 
 AREA RESET,CODE
Start_up PROC
 IMPORT main;自己写的，不调用库
 IMPORT __main;这是调用库的
 IMPORT RCC_INIT
 ;内存清理 R4:开始地址.R5:当前次数.R6:次数.R7:填充值
 LDR R4,SRAM_AD
 LDR R5,ZERO
 LDR R6,SRAM_SIZE
 LDR R7,ZERO
clean_SRAM
 STR R7,[R4,R5]
 ADDS R5,#4
 CMP R5,R6
 BMI clean_SRAM
 ALIGN
ZERO 		DCD 0
SRAM_AD 	DCD 0x20000000
SRAM_SIZE 	DCD 8*1024
 ;内存清理
 LDR R0,=RCC_INIT
 BLX R0
 ;LDR R0,=main;自己写的，不调用库
 LDR R0,=__main;这个是调用库的
 BX R0
 ALIGN
 ENDP
 
 AREA rewrite,code
 ;IMPORT __use_no_semihosting
 EXPORT __user_initial_stackheap ;重写这个函数，因为这个函数调用了主机函数
 ;EXPORT _sys_exit
__user_initial_stackheap 
 ;LDR R0,=ZERO
 MOVS R0,#0
 LDR R1,=stack_End
 MOVS R2,#0
 LDR R3,=stack_size
 BX LR
 ALIGN
stack_size DCD s_size
;_sys_exit
 
 
 
 
 
 END