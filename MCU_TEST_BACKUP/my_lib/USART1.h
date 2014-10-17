//恶搞 2014-9-15 05:32:38
#define USART1_CLOCK 8000000
enum _STATUS{success_to_set,the_UE_is_set,Invalid_parameter,_idle,_busy,_lock_error};//反馈设置是否成功
enum _BAUD{_1200bps,_2400bps,_9600bps,_19200bps,_38400bps,_57600bps,_115200bps,_230400bps,_460800bps,_921600bps};//波特率
enum _DATA_LENGTH{_7_Data_bits,_8_Data_bits,_9_Data_bits,};//数据长度
enum _STOP_BIT{_1_stop_bit,_1_point_5_stop_bit,_2_stop_bit,};//多少个stop位
enum _OVERSAMPING{_over16,_over8};//过采样
enum _Parity{nonuse_parity,Even_parity,Odd_parity};//奇偶
enum _ENB{_set_UE,_SET_TE,_SET_RE,_clr_UE,_clr_TE,_clr_RE};//使能位
enum _Direction{_reception,_send};//传输方向
enum _RX_STATE{_ready,_receiving,_done};
enum _LOCK{_locked,_unlock,success_to_lock,no_lock_before};
typedef struct{
	us tx_size;												//要发送的的数据的长度
	uc* tx_p;													//要发送的数据的首指针
} USART1_TxDAT;

typedef struct{
	us rx_size;												//指定接收数据的长度
	uc* rx_p;													//接收缓冲区
	volatile enum _RX_STATE rx_state;	//提供给接收程序用来提示接收是否完毕
} USART1_RxDAT;

//底层函数
enum _STATUS _baud_rate_set(enum _OVERSAMPING oversamp,enum _BAUD baud);//√
enum _STATUS _frame_format_select(enum _DATA_LENGTH len,enum _Parity parity,enum _STOP_BIT stop);//√
void _usart_enb_or_dis(enum _ENB ed);//√
void _usart_clock_set(void);//√
void _DMA_set(enum _Direction dir,us d_size,uc* p);//√
void _USART1_DMA_IRQ_ENB(void);//√
void _GPIO_setting_for_USART1(void);//√

//主要用到的的函数：
void USART1_INIT_ALL(void);//√
// enum _STATUS USART1_SEND(USART1_TxDAT* p);//√ /*返回值_busy或success_to_set*/
void USART1_SEND(USART1_TxDAT* p);
enum _STATUS USART1_RECEIVE(USART1_RxDAT* p);//√
enum _LOCK USART1_RX_UNLOCK(USART1_RxDAT* p);

//中断处理函数：
void USART1_Handler(void);//√







