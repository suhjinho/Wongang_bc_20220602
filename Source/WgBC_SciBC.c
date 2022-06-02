//################################################################################
// 파일명   : SCI_B.c
// 회사명   : RealSYS
// 작성날짜 : 2008.02.21
// 설명     : F28335 교육용 키트인 DSP28335 DSK의 SCI-B 관련 구현 코드 파일
// [ 사용한 개발 환경 ]
// 1) TI CCS3.3과 Service Pack v8.0(CCS_v3.3_SR8_77.exe 파일) 
// 2) CCS3.3 Setup용 Target Driver Package(setupCCSPlatinum_v30323.exe 파일) 
// 3) C2000 Code Generation Tools v5.0 Release(C2000CodeGenerationTools5.0.0.exe 파일) 
//
// [ 참조한 Software Developer Kit(SDK) ]
// 1) TI의 SDK인 C/C++ Header Files and Peripheral Examples v1.10 사용 
//################################################################################
#include "Global_Variable_Function.h"

// 제공된 예제에서는 인터럽트 서비스 루틴을 "ramfuncs" 이라는 섹션에 할당하고
// "ramfuncs" 섹션은 빠른 응답 처리를 위해 내부 RAM에서 동작하도록 함

// SCI-B Interrupt Service Function 선언
#pragma CODE_SECTION(scib_tx_isr, "ramfuncs");
#pragma CODE_SECTION(scib_rx_isr, "ramfuncs");

interrupt void scib_tx_isr(void);
interrupt void scib_rx_isr(void);
// SCI-C 포트의 Interrupt Service Function 선언
#pragma CODE_SECTION(scic_tx_isr, "ramfuncs");
#pragma CODE_SECTION(scic_rx_isr, "ramfuncs");

interrupt void scic_tx_isr(void);
interrupt void scic_rx_isr(void);

#define	CPUCLK			150000000L							// CPU Main Clock
#define	SCIB_LSPCLK		(CPUCLK/4)							// Peripheral Low Speed Clock for SCI-B
#define	SCIB_BAUDRATE	9600L								// SCI-B Baudrate
#define	SCIB_BRR_VAL	(SCIB_LSPCLK/(8*SCIB_BAUDRATE)-1)	// SCI-B BaudRate 설정 Register 값

/* BPS 에러율 *********************************************************
*    BPS	   CPUCLK	  LSPCLK	         BRR_VAL	     BPS	error
*   4800	150000000	37500000	975.5625	976 	4797.851 	-0.045 
*   9600	150000000	37500000	487.28125	487 	9605.533 	0.058 
*  19200	150000000	37500000	243.140625	243 	19211.066 	0.058 
*  38400	150000000	37500000	121.0703125	121 	38422.131 	0.058 
*  57600	150000000	37500000	80.38020833	80 		57870.370 	0.469 
* 115200	150000000	37500000	39.69010417	40 		114329.268 	-0.756 
***********************************************************************/

#define	SCIB_BUF_SIZE	50

#define	SCIB_TX_START	{	if(ScibRegs.SCICTL2.bit.TXRDY){						\
								ScibRegs.SCICTL2.bit.TXINTENA=1;				\
								ScibRegs.SCITXBUF = scib_tx_buf[scib_tx_pos++];	\
								if(scib_tx_pos >= SCIB_BUF_SIZE) scib_tx_pos=0;	\
							}													\
							else ScibRegs.SCICTL2.bit.TXINTENA=1;				\
						}

#define	SCIB_TX_STOP	ScibRegs.SCICTL2.bit.TXINTENA=0

// SCI-B의 송신 처리를 위한 버퍼 관련 변수
BYTE scib_tx_buf[SCIB_BUF_SIZE+1];
BYTE scib_tx_pos=0, scib_tx_end=0;

                                                                                                              
// 현재 SCI-B 포트에 수신된 데이터(ex07_sci.c 파일에 변수 정의됨)
unsigned char scib_rxd; 
unsigned char scic_rxd;
//void RS485_Scib_test();

unsigned int scib_cnt = 0;
unsigned int scic_cnt = 0;

unsigned int scic_txdisr_cnt = 0;
unsigned int scic_rxdisr_cnt = 0;
unsigned int scib_txdisr_cnt = 0;
unsigned int scib_rxdisr_cnt = 0;


void scib_putc(char d){
	scib_tx_buf[scib_tx_end++] = d;
	if(scib_tx_end >= SCIB_BUF_SIZE) scib_tx_end = 0;
}

void scib_puts(char* p){
char rd;
	while(rd = *p++){             
		scib_tx_buf[scib_tx_end++] = rd;
		if(scib_tx_end >= SCIB_BUF_SIZE) scib_tx_end = 0;
	}
}

interrupt void scib_tx_isr(void){
	scib_txdisr_cnt++;
//	if(scib_tx_pos != scib_tx_end){
//		ScibRegs.SCITXBUF = scib_tx_buf[scib_tx_pos++];
//		if(scib_tx_pos >= SCIB_BUF_SIZE) scib_tx_pos = 0;
//	}
//	else{                              
//		SCIB_TX_STOP;
//	}

	// Acknowledge this interrupt to recieve more interrupts from group 9
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP9;
}

interrupt void scib_rx_isr(void){
	scib_rxd = ScibRegs.SCIRXBUF.all;
	scib_rxdisr_cnt++;
		
//	scib_puts("SCI-B: rxd = ");
//	scib_putc(scib_rxd);
//	scib_puts("\r\n");
//	SCIB_TX_START;
	
	// Acknowledge this interrupt to recieve more interrupts from group 9
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP9;
}
interrupt void scic_tx_isr(void){
	scic_txdisr_cnt++;
/*
	if(scic_tx_pos != scib_tx_end){
		ScicRegs.SCITXBUF = scic_tx_buf[scic_tx_pos++];
		if(scic_tx_pos >= SCIB_BUF_SIZE) scic_tx_pos = 0;
	}
	else{                              
		SCIB_TX_STOP;
	}
*/
	// Acknowledge this interrupt to recieve more interrupts from group 9
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP8;
}

interrupt void scic_rx_isr(void){
	scic_rxdisr_cnt++;
	scic_rxd = ScicRegs.SCIRXBUF.all;

//	scib_puts("SCI-C: rxd = ");
//	scib_putc(scic_rxd);
//	scib_puts("\r\n");
//	SCIB_TX_START;
	// Acknowledge this interrupt to recieve more interrupts from group 9
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP8;
}
// SCI-B 초기화 함수
void scib_init(){
	ScibRegs.SCIFFTX.all = 0x8000;			// FIFO reset
 	ScibRegs.SCIFFCT.all = 0x4000;			// Clear ABD(Auto baud bit)
 	
 	ScibRegs.SCICCR.all = 0x0007;  			// 1 stop bit,  No loopback 
                                   			// No parity,8 char bits,
                                   			// async mode, idle-line protocol
	ScibRegs.SCICTL1.all = 0x0003; 			// enable TX, RX, internal SCICLK, 
                                   			// Disable RX ERR, SLEEP, TXWAKE

	ScibRegs.SCICTL2.bit.RXBKINTENA = 1;	// RX/BK INT ENA=1,
				//ScibRegs.SCICTL2.bit.TXINTENA = 1;		// TX INT ENA=1,

    ScibRegs.SCIHBAUD = SCIB_BRR_VAL >> 8;
    ScibRegs.SCILBAUD = SCIB_BRR_VAL & 0xff;

	ScibRegs.SCICTL1.all = 0x0023;			// Relinquish SCI from Reset  
    
	// Initialize SCI-B RX interrupt
    EALLOW;
	PieVectTable.SCIRXINTB = &scib_rx_isr;
    			PieVectTable.SCITXINTB = &scib_tx_isr;
 //485tx 9->22
 //485rx 11->23  
    /* Enable internal pull-up for the selected pins */
	GpioCtrlRegs.GPAPUD.bit.GPIO23 = 0; // Enable pull-up for GPIO11 (SCIRXDB)
	GpioCtrlRegs.GPAPUD.bit.GPIO22 = 0;  // Enable pull-up for GPIO9  (SCITXDB)

	/* Set qualification for selected pins to asynch only */
	GpioCtrlRegs.GPAQSEL2.bit.GPIO23 = 3;  // Asynch input GPIO11 (SCIRXDB)

	/* Configure SCI-B pins using GPIO regs*/
	//GpioCtrlRegs.GPAMUX2.bit.GPIO23 = 2;   // Configure GPIO11 for SCIRXDB operation
	//GpioCtrlRegs.GPAMUX2.bit.GPIO22 = 2;    // Configure GPIO9 for SCITXDB operation
	GpioCtrlRegs.GPAMUX2.bit.GPIO23 = 3;   // Configure GPIO11 for SCIRXDB operation
	GpioCtrlRegs.GPAMUX2.bit.GPIO22 = 3;    // Configure GPIO9 for SCITXDB operation
	EDIS;

    // Enable CPU INT9 for SCI-B
	IER |= M_INT9;
	
    // Enable SCI-B RX INT in the PIE: Group 9 interrupt 3
	PieCtrlRegs.PIEIER9.bit.INTx3 = 1;
    // Enable SCI-B TX INT in the PIE: Group 9 interrupt 3
	PieCtrlRegs.PIEIER9.bit.INTx4 = 1;

    // Enable SCI-B TX INT in the PIE: Group 9 interrupt 4
				//PieCtrlRegs.PIEIER9.bit.INTx4 = 1;
}
// SCI-C 초기화 함수
void scic_init(){
	ScicRegs.SCIFFTX.all = 0x8000;			// FIFO reset
 	ScicRegs.SCIFFCT.all = 0x4000;			// Clear ABD(Auto baud bit)
 	
 	ScicRegs.SCICCR.all = 0x0007;  			// 1 stop bit,  No loopback 
                                   			// No parity,8 char bits,
                                   			// async mode, idle-line protocol
	ScicRegs.SCICTL1.all = 0x0003; 			// enable TX, RX, internal SCICLK, 
                                   			// Disable RX ERR, SLEEP, TXWAKE

	ScicRegs.SCICTL2.bit.RXBKINTENA = 1;	// RX/BK INT ENA=1,
				//ScicRegs.SCICTL2.bit.TXINTENA = 1;		// TX INT ENA=1,

    ScicRegs.SCIHBAUD = SCIB_BRR_VAL >> 8;
    ScicRegs.SCILBAUD = SCIB_BRR_VAL & 0xff;

	ScicRegs.SCICTL1.all = 0x0023;			// Relinquish SCI from Reset  
    
	// Initialize SCI-B RX interrupt
    EALLOW;
	PieVectTable.SCIRXINTC = &scic_rx_isr;
    			PieVectTable.SCITXINTB = &scib_tx_isr;
 //485tx 9->22-->63
 //485rx 11->23-->62  
    /* Enable internal pull-up for the selected pins */
	GpioCtrlRegs.GPBPUD.bit.GPIO62 = 0; // Enable pull-up for GPIO11 (SCIRXDB)
	GpioCtrlRegs.GPBPUD.bit.GPIO63 = 0;  // Enable pull-up for GPIO9  (SCITXDB)

	/* Set qualification for selected pins to asynch only */
	GpioCtrlRegs.GPBQSEL2.bit.GPIO62 = 3;  // Asynch input GPIO11 (SCIRXDB)

	/* Configure SCI-B pins using GPIO regs*/
	GpioCtrlRegs.GPBMUX2.bit.GPIO62 = 1;   // Configure GPIO11 for SCIRXDB operation
	GpioCtrlRegs.GPBMUX2.bit.GPIO63 = 1;    // Configure GPIO9 for SCITXDB operation
	EDIS;

    // Enable CPU INT9 for SCI-B
	IER |= M_INT8;
	
    // Enable SCI-C RX INT in the PIE: Group 8 interrupt 5
	PieCtrlRegs.PIEIER8.bit.INTx5 = 1;

    // Enable SCI-C TX INT in the PIE: Group 8 interrupt 6
	PieCtrlRegs.PIEIER8.bit.INTx6 = 1;
}
void RS485_Scib_test()
{
	scib_cnt++;
	//SCIB_TX_START;
	//ScibRegs.SCICTL2.bit.TXINTENA=1;
	//ScibRegs.SCITXBUF = 17;

//	while(!ScibRegs.SCICTL2.bit.TXRDY);
   // ScibRegs.SCITXBUF=tx_data;



}
void transmit_RS485(unsigned char tx_data)
{
	while(!ScibRegs.SCICTL2.bit.TXRDY);
    ScibRegs.SCITXBUF=tx_data;
    	scib_cnt++;
}
void transmit_RS232(unsigned char tx_data)
{
	if(ScicRegs.SCICTL2.bit.TXRDY)
	{
   		ScicRegs.SCITXBUF=tx_data;
    	scic_cnt++;
	}
}
