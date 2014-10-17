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
 IMPORT DMA2_3_Handler ;[WEAK];�Ҳݣ�����[WEAK]���Ȼ����DMA2_3_Handler��ֵ�ŵ���������(�˺�����DMA.c����������)������
 IMPORT I2C1_Handler [WEAK]
 IMPORT SPI1_Handler [WEAK]
 IMPORT USART1_Handler [WEAK]
 DCD 0,CUOWU,0,0,0,0,0,0,0,0,0,0,0,0
 DCD 0,0,0,0,0,BUTTON_PA0_DOWN,0,0,0,DMA1_Handler,DMA2_3_Handler,0,0,0,0,0,0,0,0,0,0,0,0,I2C1_Handler,0,SPI1_Handler,0,USART1_Handler
 
 AREA RESET,CODE
Start_up PROC
 IMPORT main;�Լ�д�ģ������ÿ�
 IMPORT __main;���ǵ��ÿ��
 IMPORT RCC_INIT
 ;�ڴ����� R4:��ʼ��ַ.R5:��ǰ����.R6:����.R7:���ֵ
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
 ;�ڴ�����
 LDR R0,=RCC_INIT
 BLX R0
 ;LDR R0,=main;�Լ�д�ģ������ÿ�
 LDR R0,=__main;����ǵ��ÿ��
 BX R0
 ALIGN
 ENDP
 
 AREA rewrite,code
 ;IMPORT __use_no_semihosting
 EXPORT __user_initial_stackheap ;��д�����������Ϊ���������������������
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