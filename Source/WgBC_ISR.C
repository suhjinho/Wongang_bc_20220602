#include "Global_Variable_Function.h"

Uint16  xint1_cnt = 0;

Uint16  adcisr_seq1_cnt = 0;
Uint16  adcisr_seq2_cnt = 0;
Uint16  adcisr_cnt = 0;

Uint16  xint2_cnt = 0;
Uint16  timer0_cnt = 0;
Uint16  timer1_cnt = 0;

Uint16  EpwmCnt = 0;
Uint16	SystemReset = 0;

float T1READ_TMP1 = 0.;
float T1READ_TMP2 = 0.;
float T1READ_TMP3 = 0.;

float T1READ_TMP4 = 0.;
float T1READ_TMP5 = 0.;
float T1READ_TMP6 = 0.;

float T0READ_TMP1 = 0.;
float T0READ_TMP2 = 0.;
float T0READ_TMP3 = 0.;


Uint16 msec_tmrold = 0;
Uint16 msec_tmr = 0;
unsigned int EpwmCnt1 = 0;
Uint16 PWM_FLAG_USER = 0;

void Average_Cal(float inval, unsigned int N, float *Output);
void Control_Routine();
//========================================================================//
// 15khz pwm주기를 제어주기로 함(epwm1)
// 15khz === 66.67usec
// 수행시간은 약 32usec정도(프로그램에 따라...변동)
//========================================================================//
interrupt void epwm1_timer_isr(void)
{
	EpwmCnt++;
	
	EpwmCnt1++;
	if(EpwmCnt1 > 200) EpwmCnt1 = 0;
	Control_Routine();			
	siv_test();

	EPwm1Regs.ETCLR.bit.INT = 1;
   	// Acknowledge this interrupt to receive more interrupts from group 3
   	PieCtrlRegs.PIEACK.all = PIEACK_GROUP3;
}
//======================================
//**********************************************************************************//
// epwm5 Timer Interrupt															//
// Epwm5 ac/dc 인터럽트 내에서 모든 제어루틴 처리									//
//**********************************************************************************//
// 1. ac/dc epwm5 스위칭주파수 5khz													//
// 2. dc/dc(siv) epwm1,2,4 스위칭주파수 5khz											//
// Tperiod = 200usec																//
//**********************************************************************************//
interrupt void epwm5_timer_isr(void)
{
	//EpwmCnt++;
//MCU_D33_TOGGLE();	//D33();
  //	SensorBoard_AnalogRead();	// 20100513.LSH, 차세대용, APS, BACH공용,전압 전류 온도읽기 	
	//Control_Routine();

 	EPwm5Regs.ETCLR.bit.INT = 1;   	// Clear INT flag for this timer
   	PieCtrlRegs.PIEACK.all = PIEACK_GROUP3;	// Acknowledge this interrupt to receive more interrupts from group 3
}
//**********************************************************************************//
// Timer0 interrupt Service routine													//
// Tperiod = 200usec	,실행시간 약 38usec											//
//**********************************************************************************//
interrupt void cpu_timer0_isr(void)
{
	CpuTimer0.InterruptCount++;
	msec_tmr++;		// 메인 함수에서 일정 주기 task발생을 위한 카운터
	BCM_HartBeat++;
	if(!SystemReset)
	{	
   		ServiceDog();// Reset the watchdog counter
	}
	//DAC_Out_test(0);
	
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP1; // Acknowledge this interrupt to receive more interrupts from group 1
}

//==================================================================//
// 전원주파수 위상검출												//
// 전원주파수카운트값 = Timer1주기 - 현재 Timer1값					// 
// 읽은후 Timer1값을 초기화시킴										//
//==================================================================//
//Uint16 FaultDispCount=0;
// 하드웨어 폴트 발생시에 외부 인터럽트 걸림
interrupt void xint1_isr(void){
	//MCU_LD4_OFF();
	xint1_cnt++;

//	LineFreqCounter = ReadCpuTimer1Period() - ReadCpuTimer1Counter();
	
	T1READ_TMP1=ReadCpuTimer1Period();
	T1READ_TMP2=ReadCpuTimer1Counter();	
	T1READ_TMP3 = T1READ_TMP1-T1READ_TMP2;
		T1READ_TMP4 = 150000/T1READ_TMP3;
//	LineFreq = SYS_CLK/LineFreqCounter;

	ReloadCpuTimer1();									//초기화시킴
	timer1_cnt = 0;
	//LineFreq = msec_tmrold;
	//msec_tmrold = 0;


	// Acknowledge this interrupt to receive more interrupts from group 12	
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}

//============================================
//====500usec 
//============================================
interrupt void cpu_timer1_isr(void){
	
	CpuTimer1.InterruptCount++;
	timer1_cnt++;
	ReloadCpuTimer1();
	
	EDIS;
}

//=================================================================//
interrupt void xint2_isr(void){
	xint2_cnt++;



	// Acknowledge this interrupt to receive more interrupts from group 12	
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}


// 이 아래 인터럽트는 사용하지 않음
interrupt void epwm1_tzint_isr(void)
{
//	tzint_cnt++;
	// Acknowledge this interrupt to receive more interrupts from group 2
   	PieCtrlRegs.PIEACK.all = PIEACK_GROUP2;
}


interrupt void adc_isr(void)
{
	adcisr_cnt++;
  	
  	//TestPulseSet2();
  	
	//TestPulseClear2();
	 // Reinitialize for next ADC sequence
  	//AdcRegs.ADCTRL2.bit.RST_SEQ1 = 1;         // Reset SEQ1
	// Re-start ADC conversion
	AdcRegs.ADCTRL2.bit.SOC_SEQ1 = 1;
  	AdcRegs.ADCST.bit.INT_SEQ1_CLR = 1;       // Clear INT SEQ1 bit
  	PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;   // Acknowledge interrupt to PIE
} 
#pragma CODE_SECTION(seq1adc_isr, "ramfuncs"); //Run from RAM
interrupt void seq1adc_isr(void)
{
	adcisr_seq1_cnt++;

//	if(ADC_calibrationFalg){	
		//Sensor_Readout_offset();
//	}
//	else{
		Sensor_Readout();	// 수행시간 : 약 ??usec
//	}

//---------------------------------------------------------------------//
	AdcRegs.ADCTRL2.bit.RST_SEQ1 	= 1;		//Immidiately reset SEQ1
	AdcRegs.ADCST.bit.INT_SEQ1_CLR = 1;       // Clear INT SEQ1 bit

//	AdcRegs.ADCTRL2.bit.RST_SEQ2 	= 1;		//Immidiately reset SEQ1
//	AdcRegs.ADCST.bit.INT_SEQ2_CLR = 1;       // Clear INT SEQ1 bit
	
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;

	return;
}


//============================================================//
// ADC Interrupt Service routine(SEQ2)                        //
// 수행주기 : 54kHZ(18.53usec)                                //
// 수행시간 : 약 5.6usec                                      //
//============================================================//
#pragma CODE_SECTION(seq2adc_isr, "ramfuncs"); //Run from RAM
interrupt void seq2adc_isr(void)
{
//DSP_TMPOUT2_ON();
	adcisr_seq2_cnt++;
Sensor_Readout();

	AdcRegs.ADCTRL2.bit.RST_SEQ2 	= 1;		//Immidiately reset SEQ2
	AdcRegs.ADCST.bit.INT_SEQ2_CLR = 1;       // Clear INT SEQ2 bit
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
//DSP_TMPOUT2_OFF();
	return;
}

void Average_Cal(float inval, unsigned int N, float *Output)
{
	*Output += (inval-*Output);
	*Output += inval;	
}
//==========================================================//
// APS/BACH 제어루틴                                        //
// EPWM5 Int에서 수행(200usec)                              //
//==========================================================//
void Control_Routine()
{
	Check_DI();			//Digital 입력신호 처리	
	
	//if(FaultCheckCounter > 500){	// 초기에 폴트검지하는것 방지..
	if(FaultCheckCounter > 50000){	// 초기에 폴트검지하는것 방지..
		FaultProcess_int();
	}
	else{
		FaultCheckCounter++;
		FaultClear();// 센서보드 하드웨어 폴트, DSP IDU폴트 클리어
	}

	if(BCSTARTAck == NO){	// STOP 신호가 입력되면 PWM 바로 차단하기 위해
		PWM_FLAG = 0;
	}

	//====정상동작하는 모듈수량 계산
	if(WGBC_TC0PC1 == WGBC_TC0){	// 객차용 축전지 충전기
		Pall_OK_ModuleN = 5-ModuleFault_NUM;//5-비정상모듈수량;
	}
	else{	// 동력차용 축전지 충전기
		Pall_OK_ModuleN = 3-ModuleFault_NUM;//5-비정상모듈수량;
	}
	
	if(PWM_FLAG == YES){
		SoftStart();
		//WGBC_DCDC_Controller_20150314();
		WGBC_DCDC_Controller_20150522();
		PWM_ON(); // dcdc(PWM 1,2) ON
	}
	else{
		PWM_FLAG = NO;
		PWM_OFF();
		I_ref = 0;
		V_ref = 0;
	}

}

