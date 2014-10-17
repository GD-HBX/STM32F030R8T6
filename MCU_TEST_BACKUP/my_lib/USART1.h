//��� 2014-9-15 05:32:38
#define USART1_CLOCK 8000000
enum _STATUS{success_to_set,the_UE_is_set,Invalid_parameter,_idle,_busy,_lock_error};//���������Ƿ�ɹ�
enum _BAUD{_1200bps,_2400bps,_9600bps,_19200bps,_38400bps,_57600bps,_115200bps,_230400bps,_460800bps,_921600bps};//������
enum _DATA_LENGTH{_7_Data_bits,_8_Data_bits,_9_Data_bits,};//���ݳ���
enum _STOP_BIT{_1_stop_bit,_1_point_5_stop_bit,_2_stop_bit,};//���ٸ�stopλ
enum _OVERSAMPING{_over16,_over8};//������
enum _Parity{nonuse_parity,Even_parity,Odd_parity};//��ż
enum _ENB{_set_UE,_SET_TE,_SET_RE,_clr_UE,_clr_TE,_clr_RE};//ʹ��λ
enum _Direction{_reception,_send};//���䷽��
enum _RX_STATE{_ready,_receiving,_done};
enum _LOCK{_locked,_unlock,success_to_lock,no_lock_before};
typedef struct{
	us tx_size;												//Ҫ���͵ĵ����ݵĳ���
	uc* tx_p;													//Ҫ���͵����ݵ���ָ��
} USART1_TxDAT;

typedef struct{
	us rx_size;												//ָ���������ݵĳ���
	uc* rx_p;													//���ջ�����
	volatile enum _RX_STATE rx_state;	//�ṩ�����ճ���������ʾ�����Ƿ����
} USART1_RxDAT;

//�ײ㺯��
enum _STATUS _baud_rate_set(enum _OVERSAMPING oversamp,enum _BAUD baud);//��
enum _STATUS _frame_format_select(enum _DATA_LENGTH len,enum _Parity parity,enum _STOP_BIT stop);//��
void _usart_enb_or_dis(enum _ENB ed);//��
void _usart_clock_set(void);//��
void _DMA_set(enum _Direction dir,us d_size,uc* p);//��
void _USART1_DMA_IRQ_ENB(void);//��
void _GPIO_setting_for_USART1(void);//��

//��Ҫ�õ��ĵĺ�����
void USART1_INIT_ALL(void);//��
// enum _STATUS USART1_SEND(USART1_TxDAT* p);//�� /*����ֵ_busy��success_to_set*/
void USART1_SEND(USART1_TxDAT* p);
enum _STATUS USART1_RECEIVE(USART1_RxDAT* p);//��
enum _LOCK USART1_RX_UNLOCK(USART1_RxDAT* p);

//�жϴ�������
void USART1_Handler(void);//��







