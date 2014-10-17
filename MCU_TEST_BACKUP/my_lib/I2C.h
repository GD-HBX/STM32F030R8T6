//2014-7-6 23:21:27 恶搞
#define _24c02naddress 0x50

#define RCC_APB1ENR_I2C1EN (0x00000001 << 21)
#define RCC_CFGR3_I2C1SW	 (0x00000001 <<  4)
#define I2C_CR1_PE				 (0x00000001 <<  0)

enum DIR{i2c_tx,i2c_rx};
enum AUTOEND_MODE{no_auto,yes_auto};

typedef struct{
	enum DIR _dir;
	enum AUTOEND_MODE _autoend;
	uc addr;
	uc d_size;
	uc *p;
} I2C_data;

void I2C1_CLOCK_ENB(void);
void I2C1_INIT(void);
void I2C1_handler(void);
void I2C1_ENB(void);
void IRQ_ENB(void);
void I2C1_DIS(void);
void I2C1_SEND_RECEIVE_DATA_WITH_DMA(enum DIR _dir,uc d_size,uc *p);
void I2C1_MASTER_TRANSFER(I2C_data *start);
void I2C1_SLAVE_TRANSFER(enum DIR _dir,uc addr,uc d_size,uc *p);
void DMA_FOR_I2C1_SETTING(void);

//两个关键的全局变量_start_data和_restart_data，都是I2C_data数据结构
/*
|--底层函数
  |-主机发送
	|-void I2C1_MASTER_TRANSFER(I2C_data *start);
  |-从机发送
	|-void I2C1_SLAVE_TRANSFER(enum DIR _dir,uc addr,uc d_size,uc *p);
  |
*/
/*主要使用到的函数*/
void I2C1_INIT_ALL(void);//初始化要用到的函数
void I2C1_SOFTWARE_RESET(void);

/*单片机外部器件通信部分函数(高层函数)*/
uc e2prom_byte_write(uc addr,uc d_size,uc *p);      //如果I2C总线正在忙的话返回
uc e2prom_byte_read(uc addr,uc d_size[2],uc *p[2]); //如果I2C总线正在忙的话返回，由于e2prombyte读用到写和读，所以参数中数组
