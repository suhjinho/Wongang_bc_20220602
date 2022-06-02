#include "Global_Variable_Function.h"
//================================================================
// ADC 5664 Test Program
// 전반적으로 ADC 5664 관련 Timming들이 빨라서...avr128입장에서는 
// 그냥 cpu에서 딜레이 줄 필요없이 해도됨(한줄실행시간이면 넉넉함)
// AD5664가 24비트시스템이고...상위 2비트는 그냥아무거나 쓰면되고
//                        다음 상위 3비트는 command고
//                      그다음 상위 3비트는 채널선택이고
//                      나머지 하위 16비트가 DAC데이타임 
// input register와 dac register가 있으므로...따로따로 access했는데
//==================================================================
// cmd(command) :
// 0 : Wirte to input register n
// 1 : Update DAC register n
// 2 : Write to input register n, upgate all(software LDAC)
// 3 : Write to and updateDAC channel n
// 4 : Power Down DAC(power-up)
// 5 : Reset
// 6 : Load LDAC register
// 7 : Reserved
// add(address) : 
// DAC_A(0), DAC_B(1), DAC_C(2), DAC_D(3)
//==================================================================


//==================================================================
// cmd(command) :
// 0 : Wirte to input register n
// 1 : Update DAC register n
// 2 : Write to input register n, upgate all(software LDAC)
// 3 : Write to and updateDAC channel n
// 4 : Power Down DAC(power-up)
// 5 : Reset
// 6 : Load LDAC register
// 7 : Reserved
// add(address) : 
// DAC_A(0), DAC_B(1), DAC_C(2), DAC_D(3)
//===================================================================
// cmd define
#define AD5664_WRn				0
#define AD5664_UPn				1
#define AD5664_WRn_UPall		2
#define AD5664_WRUP_n			3
#define AD5664_PDPU				4
#define AD5664_RESET			5
#define AD5664_LDAC				6
#define AD5664_Reserved			7
// address(channel) define
#define AD5664_CH_A			0
#define AD5664_CH_B			1
#define AD5664_CH_C			2
#define AD5664_CH_D			3
#define ADC5664_CH_ADBC		7

#define ADC5664_DAC1		0
#define ADC5664_DAC2		1

unsigned int dacvalue1 = 0;
unsigned int dacvalue2 = 0;
unsigned int dacvalue3 = 0;
unsigned int dacvalue4 = 0;
unsigned int dacvalue5 = 0;
unsigned int dacvalue6 = 0;
unsigned int dacvalue7 = 0;
unsigned int dacvalue8 = 0;

extern Uint16 msec_tmr;	
// Initialize SPI for DAC7714 DAC device
unsigned int testdac16=0;

float batcur_dac=0.0;	    		
float batcur_sum=0.0;
unsigned int batcur_idx=0;    	

extern float UnbalVSet;	
void DAC_Out(WORD ch, WORD dat);
void DAC_Wirte_ADC5664(char dac1_2, char cmd, char add, unsigned int data);


void Init_Spi_Dac(void)
{
	// Initialize GPIO for SPI-A according to MCP4822 DAC
	EALLOW;
	/* Enable internal pull-up for the selected pins */
		GpioCtrlRegs.GPBPUD.bit.GPIO54 = 0;   // Enable pull-up on GPIO54 (SPISIMOA)
		GpioCtrlRegs.GPBPUD.bit.GPIO56 = 0;   // Enable pull-up on GPIO56 (SPICLKA)
	
	//GpioCtrlRegs.GPBPUD.bit.GPIO57 = 0;   // Enable pull-up on GPIO57 (SPISTEA)

	/* Set qualification for selected pins to asynch only */
		GpioCtrlRegs.GPBQSEL2.bit.GPIO54 = 3; // Asynch input GPIO54 (SPISIMOA)
		GpioCtrlRegs.GPBQSEL2.bit.GPIO56 = 3; // Asynch input GPIO56 (SPICLKA)
	//GpioCtrlRegs.GPBQSEL2.bit.GPIO57 = 3; // Asynch input GPIO57 (SPISTEA)

	/* Configure SPI-A pins using GPIO regs*/
		GpioCtrlRegs.GPBMUX2.bit.GPIO54 = 1;  // Configure GPIO54 as SPISIMOA
		GpioCtrlRegs.GPBMUX2.bit.GPIO56 = 1;  // Configure GPIO56 as SPICLKA
	//GpioCtrlRegs.GPBMUX2.bit.GPIO57 = 1;  // Configure GPIO57 as SPISTEA
	
	/* Configure /LDAC1 */
	GpioCtrlRegs.GPBMUX2.bit.GPIO55 = 0;  // Configure GPIO55 as GPIO(/LDAC)
	GpioCtrlRegs.GPBDIR.bit.GPIO55 = 1;	  // Output Mode
	GpioDataRegs.GPBSET.bit.GPIO55 = 1; 
	/* Configure /LDAC2 */
	GpioCtrlRegs.GPBMUX2.bit.GPIO53 = 0;  // Configure GPIO53 as GPIO(/LDAC)
	GpioCtrlRegs.GPBDIR.bit.GPIO53 = 1;	  // Output Mode
	GpioDataRegs.GPBSET.bit.GPIO53 = 1; 
		
	EDIS;
	
	// Disable the FIFO operation
	SpiaRegs.SPIFFTX.all = 0xA000;
	SpiaRegs.SPIFFRX.all = 0x2000;
	SpiaRegs.SPIFFCT.all = 0x0;

	SpiaRegs.SPICCR.all =0x000F;	// Reset on, rising edge, 16-bit char bits  
	SpiaRegs.SPICTL.all =0x000E;    // Enable master mode, one half-cycle delay phase,
                                    // Enable TALK, and SPI int disabled.

	// SPI clock speed가 20MHz(DAC소자가 지원하는 최대속도)가 넘지 않도록 설정함
	// For SPIBRR = 3 to 127	LSPCLK/(SPIBRR+1)  LSPCLK = 75M/2=37.5MHz
	// For SPIBRR = 0, 1 to 2	LSPCLK/4
	// Here, SPIBRR = 16;
	SpiaRegs.SPIBRR =0;			// Maximum SPI speed(LSPCLK/16 = 150MHz/32 = 4.6875MHz)									
    SpiaRegs.SPICCR.all =0x008F;	// Relinquish SPI from Reset   
    SpiaRegs.SPIPRI.bit.FREE = 1;	// Set so breakpoints don't disturb xmission	 
}


void Init_GPIO_Dac(void)
{
	// Initialize GPIO for SPI-A according to MCP4822 DAC
	EALLOW;
	/* Enable internal pull-up for the selected pins */
		GpioCtrlRegs.GPBPUD.bit.GPIO54 = 0;   // Enable pull-up on GPIO54 (SPISIMOA)
		GpioCtrlRegs.GPBPUD.bit.GPIO56 = 0;   // Enable pull-up on GPIO56 (SPICLKA)
	
	//GpioCtrlRegs.GPBPUD.bit.GPIO57 = 0;   // Enable pull-up on GPIO57 (SPISTEA)

	/* Set qualification for selected pins to asynch only */
		GpioCtrlRegs.GPBQSEL2.bit.GPIO54 = 3; // Asynch input GPIO54 (SPISIMOA)
		GpioCtrlRegs.GPBQSEL2.bit.GPIO56 = 3; // Asynch input GPIO56 (SPICLKA)
	//GpioCtrlRegs.GPBQSEL2.bit.GPIO57 = 3; // Asynch input GPIO57 (SPISTEA)

	/* Configure SPI-A pins using GPIO regs*/
		GpioCtrlRegs.GPBMUX2.bit.GPIO54 = 0;  // Configure GPIO54 as SPISIMOA
		GpioCtrlRegs.GPBMUX2.bit.GPIO56 = 0;  // Configure GPIO56 as SPICLKA
	
		GpioCtrlRegs.GPBDIR.bit.GPIO54 = 1;	  // Output Mode
		GpioCtrlRegs.GPBDIR.bit.GPIO56 = 1;	  // Output Mode
	//GpioCtrlRegs.GPBMUX2.bit.GPIO57 = 1;  // Configure GPIO57 as SPISTEA
	
	/* Configure /LDAC1 */
		GpioCtrlRegs.GPBMUX2.bit.GPIO55 = 0;  // Configure GPIO55 as GPIO(/LDAC)
		GpioCtrlRegs.GPBDIR.bit.GPIO55 = 1;	  // Output Mode
		GpioDataRegs.GPBSET.bit.GPIO55 = 1; 
	/* Configure /LDAC2 */
		GpioCtrlRegs.GPBMUX2.bit.GPIO53 = 0;  // Configure GPIO53 as GPIO(/LDAC)
		GpioCtrlRegs.GPBDIR.bit.GPIO53 = 1;	  // Output Mode
		GpioDataRegs.GPBSET.bit.GPIO53 = 1; 
	
	EDIS;
}

// DAC 채널1/2을 출력하는 함수
void DAC_Out(WORD ch, WORD dat){
	WORD d;
	
	switch(ch)
	{
		case 0	:	d = (0x0000 | (dat & 0xfff));	// DAC_A
					SpiaRegs.SPIDAT = d;	
					while(SpiaRegs.SPISTS.bit.INT_FLAG == 0);
				    d = SpiaRegs.SPIRXBUF;			// clear SPI INT Flag
				    break;
		case 1	:	d = (0x4000 | (dat & 0xfff));	// DAC_B
					SpiaRegs.SPIDAT = d;	
					while(SpiaRegs.SPISTS.bit.INT_FLAG == 0);
				    d = SpiaRegs.SPIRXBUF;			// clear SPI INT Flag
					break;
		case 2	:	d = (0x8000 | (dat & 0xfff));	// DAC_C
					SpiaRegs.SPIDAT = d;	
					while(SpiaRegs.SPISTS.bit.INT_FLAG == 0);
				    d = SpiaRegs.SPIRXBUF;			// clear SPI INT Flag
					break;
		case 3	:	d = (0xc000 | (dat & 0xfff));	// DAC_D
					SpiaRegs.SPIDAT = d;	
					while(SpiaRegs.SPISTS.bit.INT_FLAG == 0);
				    d = SpiaRegs.SPIRXBUF;			// clear SPI INT Flag
					break;
		default : 	break;

	}
	

//while(!SpiaRegs.SPISTS.bit.INT_FLAG);
	
//Set /LDAC signal timing according to MCP4822 DAC device datasheet
//DELAY_US(0.04L);
//GpioDataRegs.GPBCLEAR.bit.GPIO58 = 1;
//DELAY_US(0.1L);
//GpioDataRegs.GPBSET.bit.GPIO58 = 1;	
}


void DAC_Out_test(unsigned int mode)
{
	unsigned int d1,d2,d3,d4,d5,d6,d7,d8;	
	mode = 3;
	
	switch(mode)
	{
		case 0 ://MCU TEST
			d1 = msec_tmr;
			d2 = msec_tmr;
			d3 = msec_tmr;
			d4 = msec_tmr;
			d5 = msec_tmr;
			d6 = msec_tmr;
			d7 = msec_tmr;
			d8 = msec_tmr;
		break;
		case 1 ://전압전류
			d1 = (unsigned int)(MInputVolLPF/1000*65535);//--->1000[v]일때 10[v]출력
			d2 = (unsigned int)(MDcLinkVolLPF/1000*65535);//--->1000[v]일때 10[v]출력
			d3 = (unsigned int)(MBatVolLPF/100*65535);//--->100[V]일때 10[v]출력
			d4 = (unsigned int)(MInputCurLPF/100*65535);//--->100[A]일때 10[v]출력
			d5 = (unsigned int)(MOutCurLPF/200*65535);//--->200[A]일때 10[v]출력
			d6 = (unsigned int)(MBatCurLPF/100*65535);//--->100[A]일때 10[v]출력
			d7 = (unsigned int)(BOutCurLPF/1000*65535);//--->1000[A]일때 10[v]출력
			d8 = (unsigned int)(BBatCur1LPF/100*65535);//--->100[A]일때 10[v]출력
		break;
		case 2 ://전압전류
			d1 = (unsigned int)(MInputVolLPF/1000*65535);//--->1000[v]일때 10[v]출력
			d2 = (unsigned int)(MDcLinkVolLPF/1000*65535);//--->1000[v]일때 10[v]출력
			d3 = (unsigned int)(MBatVolLPF/100*65535);//--->100[V]일때 10[v]출력
			d4 = (unsigned int)(MInputCurLPF/100*65535);//--->100[A]일때 10[v]출력
			d5 = (unsigned int)(MOutCurLPF/200*65535);//--->200[A]일때 10[v]출력
			d6 = (unsigned int)(MBatCurLPF/100*65535);//--->100[A]일때 10[v]출력
			d7 = (unsigned int)(BOutCurLPF/1000*65535);//--->1000[A]일때 10[v]출력
			d8 = (unsigned int)(BBatCur1LPF/100*65535);//--->100[A]일때 10[v]출력
			
		break;
		case 3 ://10kW 모듈 시퀸스 시험--->정상기동, 정지
			d1 = (unsigned int)(MInputVolLPF/1000*65535);//--->1000[v]일때 10[v]출력
			d2 = (unsigned int)(BCSTARTAck*65535);//--->
			d3 = (unsigned int)(CHKAck*65535);//--->
			d4 = (unsigned int)(MDcLinkVolLPF/1000*65535);//--->100[A]일때 10[v]출력
			d5 = (unsigned int)(LKAck*65535);//--->
			d6 = (unsigned int)(PWM_FLAG*65535);//--->
			d7 = (unsigned int)(MOutVolLPF/100*65535);//--->100[v]일때 10[v]출력
			d8 = (unsigned int)(MOUTV_OKAck*65535);//--->100[A]일때 10[v]출력
		break;
		case 31 ://시퀸스--->정상기동, 정지
			//d1 = (unsigned int)(MInputVolLPF/1000*65535);//--->1000[v]일때 10[v]출력
			//d2 = (unsigned int)(BCSTARTAck*65535);//--->
			d1 = (unsigned int)(CHKAck*65535);//--->
			d2 = (unsigned int)(MDcLinkVolLPF/1000*65535);//--->100[A]일때 10[v]출력
			d3 = (unsigned int)(LKAck*65535);//--->
			d4 = (unsigned int)(PWM_FLAG*65535);//--->
			d5 = (unsigned int)(MOutVolLPF/100*65535);//--->100[v]일때 10[v]출력
			d6 = (unsigned int)(BCSTATUSAck*65535);//--->100[A]일때 10[v]출력
		break;
		case 4 ://시퀸스--->자동재기동
			//d1 = (unsigned int)(MInputVolLPF/1000*65535);//--->1000[v]일때 10[v]출력
			d1 = (unsigned int)(BCSTARTAck*65535);//--->
			d2 = (unsigned int)(CHKAck*65535);//--->
			d3 = (unsigned int)(MDcLinkVolLPF/1000*65535);//--->100[A]일때 10[v]출력
			d4 = (unsigned int)(LKAck*65535);//--->
			d5 = (unsigned int)(PWM_FLAG*65535);//--->
			d6 = (unsigned int)(MOutVolLPF/100*65535);//--->100[v]일때 10[v]출력
			d7 = (unsigned int)(BCSTATUSAck*65535);//--->
			d8 = (unsigned int)(FaultAck*65535);//--->
		break;
		case 41 ://시퀸스--->자동재기동
			//d1 = (unsigned int)(MInputVolLPF/1000*65535);//--->1000[v]일때 10[v]출력
			//d1 = (unsigned int)(BCSTARTAck*65535);//--->
			d1 = (unsigned int)(CHKAck*65535);//--->
			d2 = (unsigned int)(MDcLinkVolLPF/1000*65535);//--->100[A]일때 10[v]출력
			d3 = (unsigned int)(LKAck*65535);//--->
			//d5 = (unsigned int)(PWM_FLAG*65535);//--->
			d4 = (unsigned int)(MOutVolLPF/100*65535);//--->100[v]일때 10[v]출력
			//d7 = (unsigned int)(BCSTATUSAck*65535);//--->
			d5 = (unsigned int)(FaultAck*65535);//--->
		break;
		case 5 ://시퀸스--->3분3회
			//d1 = (unsigned int)(MInputVolLPF/1000*65535);//--->1000[v]일때 10[v]출력
			//d2 = (unsigned int)(BCSTARTAck*65535);//--->
			d1 = (unsigned int)(CHKAck*65535);//--->
			d2 = (unsigned int)(MDcLinkVolLPF/1000*65535);//--->100[A]일때 10[v]출력
			d3 = (unsigned int)(LKAck*65535);//--->
			d4 = (unsigned int)(PWM_FLAG*65535);//--->
			d5 = (unsigned int)(MOutVolLPF/100*65535);//--->100[v]일때 10[v]출력
			d6 = (unsigned int)(BCSTATUSAck*65535);//--->
			d7 = (unsigned int)(FaultAck*65535);//--->
			d8 = (unsigned int)(MajorFault*65535);//--->
		break;
		case 51 ://시퀸스--->3분3회
			//d1 = (unsigned int)(MInputVolLPF/1000*65535);//--->1000[v]일때 10[v]출력
			//d2 = (unsigned int)(BCSTARTAck*65535);//--->
			d1 = (unsigned int)(CHKAck*65535);//--->
			//d2 = (unsigned int)(MDcLinkVolLPF/1000*65535);//--->100[A]일때 10[v]출력
			d2 = (unsigned int)(LKAck*65535);//--->
			//d4 = (unsigned int)(PWM_FLAG*65535);//--->
			d3 = (unsigned int)(MOutVolLPF/100*65535);//--->100[v]일때 10[v]출력
			//d6 = (unsigned int)(BCSTATUSAck*65535);//--->
			d4 = (unsigned int)(FaultAck*65535);//--->
			d5 = (unsigned int)(MajorFault*65535);//--->
		break;

		case 6 ://충전시cc모드 병렬운전 게인튜닝
			//d1 = (unsigned int)(MInputVolLPF/1000*65535);//--->1000[v]일때 10[v]출력
			//d2 = (unsigned int)(BCSTARTAck*65535);//--->
	/*
			d1 = dacvalue1;//--->
			d2 = dacvalue2;//--->100[A]일때 10[v]출력
			d3 = dacvalue3;//--->100[A]일때 10[v]출력
			d4 = dacvalue4;//--->100[A]일때 10[v]출력
			d5 = dacvalue5;//--->100[A]일때 10[v]출력
			d6 = dacvalue6;//--->100[A]일때 10[v]출력
			d7 = dacvalue7;//--->100[A]일때 10[v]출력
			d8 = dacvalue8;//--->100[A]일때 10[v]출력
*/
			//d1 = (unsigned int)(BBatCurPI)/30*65535);//--->30일때 10[v]출력
			if(BBatCurPI<0){
				d1 = (unsigned int)(-BBatCurPI/30*65535);//--->30일때 10[v]출력
			}
			else{
				d1 = 0;
				
			}
			d2 = (unsigned int)((fabs)(BBatCurP)/30*65535);//--->100[v]일때 10[v]출력
			d3 = (unsigned int)((fabs)(BBatCurI)/30*65535);//--->100[v]일때 10[v]출력
			d4 = (unsigned int)((fabs)(BBatCurLPF)/100*65535);//--->100[A]일때 10[v]출력
			d5 = (unsigned int)((fabs)(BBatCurErr)/100*65535);//--->100[A]일때 10[v]출력
			d6 = (unsigned int)(OutVolRef/100*65535);//--->100[v]일때 10[v]출력

			//d7 = (unsigned int)((fabs)(UnbalVSet)/100*65535);//--->100[v]일때 10[v]출력
			//d7 = (unsigned int)((fabs)(UnbalVSet)/30*65535);//--->100[v]일때 10[v]출력
			if(UnbalVSet<0){
				d7 = (unsigned int)(-UnbalVSet/5*65535);//--->30일때 10[v]출력
			}
			else{
				d7 = 0;
			}

			d8 = (unsigned int)(DutyRatio/1.0*65535);//--->1.0(100%)일때 10[v]출력
			//d7 = (unsigned int)(BOutCurLPF/1000*65535);//--->1000[A]일때 10[v]출력
			//d8 = (unsigned int)((fabs)(BBatCurI)/30*65535);//--->100[v]일때 10[v]출력

		break;
		case 30 ://CCU1, CCU2 튜닝-위상등
		  	d1 = (unsigned int)((sineU1/1.0*65535/2)+(0.5*65535));  //--->1000[v]일때 10[v]출력---//	입력전압(가선전압, ZeroCrossing Board에서 받는거)
		  	d2 = (unsigned int)((sineV1/1.0*65535/2)+(0.5*65535));  //--->1000[v]일때 10[v]출력---//	입력전압(가선전압, ZeroCrossing Board에서 받는거)
		  	d3 = (unsigned int)((sineW1/1.0*65535/2)+(0.5*65535));  //--->1000[v]일때 10[v]출력---//	입력전압(가선전압, ZeroCrossing Board에서 받는거)
			d4 = 0;
		  	d5 = (unsigned int)((sineU2/1.0*65535/2)+(0.5*65535));  //--->1000[v]일때 10[v]출력---//	입력전압(가선전압, ZeroCrossing Board에서 받는거)
		  	d6 = (unsigned int)((sineV2/1.0*65535/2)+(0.5*65535));  //--->1000[v]일때 10[v]출력---//	입력전압(가선전압, ZeroCrossing Board에서 받는거)
		  	d7 = (unsigned int)((sineW2/1.0*65535/2)+(0.5*65535));  //--->1000[v]일때 10[v]출력---//	입력전압(가선전압, ZeroCrossing Board에서 받는거)
		
			d8 = 0;
		break;
	
		default:
			d1 = msec_tmr;
			d2 = msec_tmr;
			d3 = msec_tmr;
			d4 = msec_tmr;
			d5 = msec_tmr;
			d6 = msec_tmr;
			d7 = msec_tmr;
			d8 = msec_tmr;
			
		break;
	}
	

	//DAC_Wirte_ADC5664(0, char cmd, char add, unsigned int data)
	DAC_Wirte_ADC5664(ADC5664_DAC1, AD5664_WRUP_n, AD5664_CH_A, d1);	// 출력전압검출값 모니터링 외부출력-세팅전압보다 높게
	DAC_Wirte_ADC5664(ADC5664_DAC1, AD5664_WRUP_n, AD5664_CH_B, d2);	// 출력전압검출값 모니터링 외부출력-세팅전압보다 높게
	DAC_Wirte_ADC5664(ADC5664_DAC1, AD5664_WRUP_n, AD5664_CH_C, d3);	// 출력전압검출값 모니터링 외부출력-세팅전압보다 높게
	DAC_Wirte_ADC5664(ADC5664_DAC1, AD5664_WRUP_n, AD5664_CH_D, d4);	// 출력전압검출값 모니터링 외부출력-세팅전압보다 높게

	DAC_Wirte_ADC5664(ADC5664_DAC2, AD5664_WRUP_n, AD5664_CH_A, d5);	// 출력전압검출값 모니터링 외부출력-세팅전압보다 높게
	DAC_Wirte_ADC5664(ADC5664_DAC2, AD5664_WRUP_n, AD5664_CH_B, d6);	// 출력전압검출값 모니터링 외부출력-세팅전압보다 높게
	DAC_Wirte_ADC5664(ADC5664_DAC2, AD5664_WRUP_n, AD5664_CH_C, d7);	// 출력전압검출값 모니터링 외부출력-세팅전압보다 높게
	DAC_Wirte_ADC5664(ADC5664_DAC2, AD5664_WRUP_n, AD5664_CH_D, d8);	// 출력전압검출값 모니터링 외부출력-세팅전압보다 높게
	//DAC_Wirte_ADC5664(ADC5664_DAC1, AD5664_WRUP_n, AD5664_CH_C, 30000);	// 출력전압검출값 모니터링 외부출력-세팅전압보다 높게
	//DAC_Wirte_ADC5664(ADC5664_DAC1, AD5664_WRUP_n, AD5664_CH_D, 40000);	// 출력전압검출값 모니터링 외부출력-세팅전압보다 높게
	
	if(FaultAck){//9
		//ELED_TP1_HI();
		ELED_TP4_HI();
	}
	else{
		//ELED_TP1_LOW();		
		ELED_TP4_LOW();		
	}

	if(MajorFault){//10
		//ELED_TP2_HI();
		ELED_TP3_HI();
	}
	else{
		//ELED_TP2_LOW();		
		ELED_TP3_LOW();		
	}	
}

void DAC_Wirte_ADC5664(char dac1_2, char cmd, char add, unsigned int data)
{
	char cmd_add;//GPIO56
	char i = 24;

	cmd_add = (cmd<<3)|add;

	GpioDataRegs.GPBSET.bit.GPIO55 = 1;	// /SYNC_DAC1	
	GpioDataRegs.GPBSET.bit.GPIO55 = 1;	// /SYNC_DAC1	

    GpioDataRegs.GPBSET.bit.GPIO53 = 1;	// /SYNC_DAC2    
    GpioDataRegs.GPBSET.bit.GPIO53 = 1;	// /SYNC_DAC2  

	if(dac1_2 == 0) {
    	GpioDataRegs.GPBCLEAR.bit.GPIO55 = 1;	// /SYNC_DAC1	
    	GpioDataRegs.GPBCLEAR.bit.GPIO55 = 1;	// /SYNC_DAC1	
	}
	if(dac1_2 == 1) {
	    GpioDataRegs.GPBCLEAR.bit.GPIO53 = 1;	// /SYNC_DAC2    
	    GpioDataRegs.GPBCLEAR.bit.GPIO53 = 1;	// /SYNC_DAC2        
	}

	while(i)
	{
		GpioDataRegs.GPBSET.bit.GPIO56 = 1;	// SCLK를 'H'로 만들고
		GpioDataRegs.GPBSET.bit.GPIO56 = 1;	// SCLK를 'H'로 만들고

		//====================
		if(i>16)
		{
			if(cmd_add & (1<<(i-1-16)))	{	GpioDataRegs.GPBSET.bit.GPIO54 = 1;	}
			else{	GpioDataRegs.GPBCLEAR.bit.GPIO54 = 1;		}
		}
		else	// data 
		{ 
			if(data & (1<<(i-1)))	{	GpioDataRegs.GPBSET.bit.GPIO54 = 1;		}
			else{	GpioDataRegs.GPBCLEAR.bit.GPIO54 = 1;	}
		}
		i--;		

		GpioDataRegs.GPBCLEAR.bit.GPIO56 = 1;	// SCLK를 'L'로 해줌, 다음루프에서 'H'되기전에 써준 데이타 샘플링함
		GpioDataRegs.GPBCLEAR.bit.GPIO56 = 1;	// SCLK를 'L'로 해줌, 다음루프에서 'H'되기전에 써준 데이타 샘플링함

	}
	// 24비트 다 전송했으면... 'H'로 해줌
	GpioDataRegs.GPBSET.bit.GPIO55 = 1;	// /SYNC_DAC1	
	GpioDataRegs.GPBSET.bit.GPIO55 = 1;	// /SYNC_DAC1	

    GpioDataRegs.GPBSET.bit.GPIO53 = 1;	// /SYNC_DAC2    
    GpioDataRegs.GPBSET.bit.GPIO53 = 1;	// /SYNC_DAC2  


}

