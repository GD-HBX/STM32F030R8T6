//2014-7-6 23:21:27 恶搞
enum f_x{m2m,m2p,p2m,DMA_TEST};
void DMA_clock(void);
void DMA_chuanshu(int fangxiang,int ma,int pa,int data_size);







//中断处理函数
void DMA1_Handler(void);
void DMA2_3_Handler(void);



//中断处理函数子部分

void DMA3_TC_FLAG_HANDLER(void);
