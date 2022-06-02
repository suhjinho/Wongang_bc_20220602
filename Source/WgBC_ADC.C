//==================================================================================//
// P17KW PROJECT(삼성SDI 충방전기)													//
//==================================================================================//
// Analog Program																	//
//==================================================================================//

// 센서카드 매핑 정리          										
//ADC  |-----DCDC---------------------------|----ACDC-----------------------//
//AD0  | N.C								| ???							//
//AD1  | GND								| ???							//
//AD2  | BAT전류1							| ???							//
//AD3  | GND								| ???							//
//AD4  | BAT전류2							| ???							//
//AD5  | GND								| ???							//
//AD6  | BAT전류3							| ???							//
//AD7  | GND								| ???							//
//AD8  | DCLINK전류1						| ???							//
//AD9  | GND								| ???							//
//AD10 | DCLINK전류2						| ???							//
//AD11 | GND								| ???							//
//AD12 | DCLINK전류3						| ???							//
//AD13 | GND								| ???							//
//AD14 | 12V출력전압						| ???							//
//AD15 | 1.5V기준전압						| ???							//
//----------------------------------------------------------------------------------//

#include "Global_Variable_Function.h"
#define ADC_usDELAY           10000L

float MInputVolLPF_test = 0.0;
extern unsigned int nvr_faulttest;

extern unsigned int EpwmCnt1;

void Adc_CurrentOffset_Caculation()
{
	if(adc_offsetCounter < 4096){
		adc0_offset = 0;
		adc1_offset = 0;
		adc2_offset = 0;
		adc3_offset = 0;
		adc4_offset = 0;
		adc5_offset = 0;
		adc6_offset = 0;
		
		adc0_sum += ADCRD0;//모듈 입력전류
		adc1_sum += ADCRD1;//모듈 충전전류
		adc2_sum += ADCRD2;//모듈 출력전류
		adc3_sum += ADCRD3;//스페어
		adc4_sum += ADCRD4;//box BAT전류1
		adc5_sum += ADCRD5;//box BAT전류2 (객차충전기에는 사용안함)
		adc6_sum += ADCRD6;//box 출력전류
		adc_offsetCounter++;
	}
	if(adc_offsetCounter == 4096)
	{    
		adc0_offset = adc0_sum/adc_offsetCounter;
		adc1_offset = adc1_sum/adc_offsetCounter;
		adc2_offset = adc2_sum/adc_offsetCounter;
		adc3_offset = adc3_sum/adc_offsetCounter;
		adc4_offset = adc4_sum/adc_offsetCounter;
		adc5_offset = adc5_sum/adc_offsetCounter;
		adc6_offset = adc6_sum/adc_offsetCounter;
		adc0_sum = 0;
		adc1_sum = 0;
		adc2_sum = 0;
		adc3_sum = 0;
		adc4_sum = 0;
		adc5_sum = 0;
		adc6_sum = 0;
   		adc_offsetCounter = 5001;
    }
}

//===DCDC 센싱부====
void Sensor_Readout()
{
	ADCRD0 = (AdcRegs.ADCRESULT0>>4) - adc0_offset; //모듈 입력전류
	ADCRD1 = (AdcRegs.ADCRESULT1>>4) - adc1_offset; //모듈 충전전류
	ADCRD2 = (AdcRegs.ADCRESULT2>>4) - adc2_offset; //모듈 출력전류
	ADCRD3 = (AdcRegs.ADCRESULT3>>4) - adc3_offset; //스페어
	
	ADCRD4 = (AdcRegs.ADCRESULT4>>4) - adc4_offset; //box BAT전류1
	ADCRD5 = (AdcRegs.ADCRESULT5>>4) - adc5_offset; //box BAT전류2 (객차충전기에는 사용안함)
	ADCRD6 = (AdcRegs.ADCRESULT6>>4) - adc6_offset; //box 출력전류
	ADCRD7 = (AdcRegs.ADCRESULT7>>4) - adc7_offset; //모듈 제어전압
	
	ADCRD8 = (AdcRegs.ADCRESULT8>>4) - adc8_offset; //모듈출력전압1(OBD전단)
	ADCRD9 = (AdcRegs.ADCRESULT9>>4) - adc9_offset; //모듈출력전압2(OBD후단) ---> 배터리전압 및 BOX출력전압임
	ADCRD10 = (AdcRegs.ADCRESULT10>>4) - adc10_offset; //모듈입력전압
	ADCRD11 = (AdcRegs.ADCRESULT11>>4) - adc11_offset; //모듈 DCLINK전압	
/*
	MInputVol = Scale_MInputVol*ADCRD10;	//모듈입력전압 --> ADCRD10
	MDcLinkVol = Scale_MDcLinkVol*ADCRD11;	//모듈 DCLINK전압(RATED 670Vdc) --> ADCRD11
	MBatVol = Scale_MOutVol*ADCRD9;			//모듈출력전압1(OBD후단) 정격 720Vdc(72~83) --> ADCRD9
	MOutVol = Scale_MOutVol1*ADCRD8;		//모듈출력전압1(OBD전단) 정격 720Vdc(72~83) --> ADCRD8
	MCtrlVol = Scale_MCtrlVol*ADCRD7;		//모듈 제어전압 정격 72Vdc(72~83) --> ADCRD7
	MInputCur = Scale_MInputCur*ADCRD0;		//모듈 입력전류 --> ADCRD0
	BOutCur = Scale_BOutCur*ADCRD6;			//box 출력전류 --> ADCRD6
	MBatCur = Scale_MBatCur*ADCRD1;			//모듈 충전전류 --> ADCRD1
	MOutCur = Scale_MOutCur*ADCRD2;			//모듈 출력전류 --> ADCRD2
	BBatCur1 = Scale_BBatCur1*ADCRD4;		//box BAT전류1 --> ADCRD4
	BBatCur2 = Scale_BBatCur2*ADCRD5;		//box BAT전류2 --> ADCRD5 (객차 BC는 사용안함)
*/
	siv_test_Vrefin1 = siv_test_Vrefin1_Scale*ADCRD0/4097;		//ref1--> ADCRD0
	siv_test_Vrefin2 = siv_test_Vrefin2_Scale*ADCRD1/4097;		//ref1 --> ADCRD3
	
	MInputCur = Scale_MInputCur*ADCRD0;		//모듈 입력전류 --> ADCRD0
	MBatCur = Scale_MBatCur*ADCRD1;			//모듈 충전전류 --> ADCRD1
	MOutCur = Scale_MOutCur*ADCRD2;			//모듈 출력전류 --> ADCRD2
	BBatCur1 = Scale_BBatCur1*ADCRD4;		//box BAT전류1 --> ADCRD4
	BBatCur2 = Scale_BBatCur2*ADCRD5;		//box BAT전류2 --> ADCRD5 (객차 BC는 사용안함)
	BOutCur = Scale_BOutCur*ADCRD6;			//box 출력전류 --> ADCRD6
	MCtrlVol = Scale_MCtrlVol*ADCRD7;		//모듈 제어전압 정격 72Vdc(72~83) --> ADCRD7
	MOutVol = Scale_MOutVol*ADCRD8;			//모듈출력전압1(OBD전단) 정격 720Vdc(72~83) --> ADCRD8
	MBatVol = Scale_MOutVol*ADCRD9;			//모듈출력전압1(OBD후단) 정격 720Vdc(72~83) --> ADCRD9
	MInputVol = Scale_MInputVol*ADCRD10;	//모듈입력전압 --> ADCRD10
	MDcLinkVol = Scale_MDcLinkVol*ADCRD11;	//모듈 DCLINK전압(RATED 670Vdc) --> ADCRD11
	
//---BBatCur 객차 구할때는 BBatCur2를 0으로하고 계산
//	BBatCur1 = BBatCur1-0.2;		//box BAT전류1 --> ADCRD4
//	BBatCur2 = BBatCur2-0.2;		//box BAT전류2 --> ADCRD5 (객차 BC는 사용안함)
//	if(BBatCur1<0.0) BBatCur1 = 0.0;
//	if(BBatCur2<0.0) BBatCur2 = 0.0;
//---필터링
	MDcLinkVolLPF = MDcLinkVol;
	MBatVolLPF = MBatVol;
	MOutVolLPF = MOutVol;
	MCtrlVolLPF = MCtrlVol;

	MInputCurLPF = MInputCur;
	BOutCurLPF = BOutCur;
	MBatCurLPF = MBatCur;
	MOutCurLPF = MOutCur;

//==전압
	MInputVolLPF += 0.01*(MInputVol - MInputVolLPF);
	MDcLinkVolLPF += 0.01*(MDcLinkVol - MDcLinkVolLPF);
  //MBatVolLPF += 0.1*(MBatVol - MBatVolLPF);
  //MOutVolLPF += 0.01*(MOutVol - MOutVolLPF);
	MOutVolLPF += 0.1*(MOutVol - MOutVolLPF);
	MCtrlVolLPF += 0.1*(MCtrlVol - MCtrlVolLPF);

//==전류
	MInputCurLPF += 0.005*(MInputCur - MInputCurLPF);//= MInputCur;
	BBatCur1LPF += 0.01*(BBatCur1 - BBatCur1LPF);//= BBatCur1;
	BBatCur2LPF += 0.01*(BBatCur2 - BBatCur2LPF);//= BBatCur2;
  //BOutCurLPF += 0.01*(BOutCur - BOutCurLPF);//= BBatCur1;
	BBatCurLPF = BBatCur1LPF + BBatCur2LPF;		//box BAT전류 --> 동력차 BC =BAT1+BAT2, 객차 BC = BAT1
	BLoadCurLPF = (BOutCurLPF - BBatCurLPF);	//box 부하전류 -->  출력전류-충전전류(BAT전류)
	MLoadCurLPF = (MOutCurLPF - MBatCurLPF);	//모듈 부하전류 --> 출력전류-충전전류(BAT전류)	
}

void InitADC_Var()
{
	ADCRD0 = 0;
	ADCRD1 = 0;
	ADCRD2 = 0;
	ADCRD3 = 0;
	ADCRD4 = 0;
	ADCRD5 = 0;
	ADCRD6 = 0;
	ADCRD7 = 0;
	ADCRD8 = 0;
	ADCRD9 = 0;
	ADCRD10 = 0;
	ADCRD11 = 0;
	ADCRD12 = 0;
	ADCRD13 = 0;
	ADCRD14 = 0;
	ADCRD15 = 0;

	adc0_offset = 0;
	adc1_offset = 0;
	adc2_offset = 0;
	adc3_offset = 0;
	adc4_offset = 0;
	adc5_offset = 0;
	adc6_offset = 0;
	adc7_offset = 0;
	adc8_offset = 0;
	adc9_offset = 0; 
	//adc8_offset = -2; // 0-->-6 MODULE1,0-->-2 MODULE2 옵셋조정 
	//adc9_offset = -3; // 0-->-3 MODULE1,2 옵셋뗍� 
	adc10_offset = 0;
	adc11_offset = 0;
	adc_offsetCounter = 0;
		
	Scale_MInputVol = 0.24487363;	//모듈입력전압 --> ADCRD10
	Scale_MDcLinkVol = 0.24487363;	//모듈 DCLINK전압(RATED 670Vdc) --> ADCRD11
	Scale_MOutVol = 0.02529;		//모듈출력전압1(OBD후단) 정격 720Vdc(72~83) --> ADCRD9--2015년1월28일--스케일조정

	Scale_MCtrlVol = 0.0230894;		//모듈 제어전압 정격 72Vdc(72~83) --> ADCRD7

	//Scale_MInputCur = 0.00762893;	//모듈 입력전류 --> ADCRD0
	//Scale_MInputCur = 0.0073249;	//모듈 입력전류 --> 20150325 240오옴→250오옴으로 변경 시험절차서 기준 맞추기 위함
	Scale_MInputCur = 0.0075263;	//모듈 입력전류 --> 20150407 →249.5오옴(499/2)으로 변경 시험절차서 기준 맞추기 위함
	
	Scale_MBatCur = 0.03051758;		//모듈 충전전류 --> ADCRD1
	//Scale_MOutCur = 0.0475678000;		//모듈 출력전류 --> ADCRD2 --->--2015년3월9일16:10--스케일조정0.0492000  --> 0.046788
	Scale_MOutCur = 0.04882813;		//모듈 출력전류 --> ADCRD2 --->--2015년3월10일15:35--스케일조정0.046788  --> 0.04882813
	if(WGBC_TC0PC1 == WGBC_TC0) {	// 객차용 축전지 충전기
		//Scale_BOutCur = 0.22605613;		//객차 box 출력전류 --> ADCRD6		
		Scale_BOutCur = 0.244010468;		//객차 box 출력전류 --> ADCRD6		
		Scale_BBatCur1 = 0.02441406;	//box BAT전류1 --> ADCRD4
		Scale_BBatCur2 = 0.00;			//box BAT전류2 --> ADCRD5 (객차 BC는 사용안함)
	}
	else
	{
		//Scale_BOutCur = 0.1568;		//미계산, 동력차 box 출력전류 --> ADCRD6 -->2015년2월5일--스케일조정0.124 --> 0.1213 0.050600
		Scale_BOutCur = 0.1580;		//미계산, 동력차 box 출력전류 --> ADCRD6 -->2015년2월5일--스케일조정0.124 --> 0.1213 0.050600
		Scale_BBatCur1 = 0.02441406;	//box BAT전류1 --> ADCRD4
		Scale_BBatCur2 = 0.02441406;	//box BAT전류2 --> ADCRD5 (객차 BC는 사용안함)
	}

	MInputVolLPF = 0.0;
	MDcLinkVolLPF = 0.0;
	MBatVolLPF = 0.0;
	MOutVolLPF = 0.0;
	MCtrlVolLPF = 0.0;

	MInputCurLPF = 0.0;
	BOutCurLPF = 0.0;
	MBatCurLPF = 0.0;
	MOutCurLPF = 0.0;
	BBatCur1LPF = 0.0;
	BBatCur2LPF = 0.0;
	BLoadCurLPF = 0.0;
	MLoadCurLPF = 0.0;
}

//===================================================//
void InitADC()
{
	//Clearify the ADC speed.
	DELAY_US(ADC_usDELAY);	
	AdcRegs.ADCTRL3.all = 0x00E0;  			// Power up bandgap/reference/ADC circuits
	DELAY_US(ADC_usDELAY);			    	// Delay before powering up rest of ADC   
	AdcRegs.ADCTRL3.bit.ADCPWDN = 1;	   	// Power up rest of ADC
	DELAY_US(ADC_usDELAY);	

//*****PRESCALER TO BE DETERMINED*************
/*
   EALLOW;
   SysCtrlRegs.HISPCP.bit.HSPCLK = 0x3; //HSPCLK  = SYSCLOCKOUT/6  = 40ns
   EDIS;

   AdcRegs.ADCTRL1.bit.CPS = 1; // 40ns * 2 = 80ns
   AdcRegs.ADCTRL3.bit.ADCCLKPS = 1;   //80ns * 2 = 160ns (ADC sampling rate) =>6.25 MHz 
   AdcRegs.ADCTRL1.bit.ACQ_PS = 0x1;   //aquisition window time (ACQ_PS+1)*160ns
*/    

//************MODIFIED**************
	// Set internal reference selection
	// F28335의 내부 ADC Reference를 사용하지 않고 외부 ADC Reference를 쓰는 경우에는
	// 아래의 값을 사용하는 Referecne Voltage에 따라 설정하십시요. 
	//AdcRegs.ADCREFSEL.bit.REF_SEL = 0x0; // 내부 ADC Referecne 사용할 때
	AdcRegs.ADCREFSEL.bit.REF_SEL = 0x01; // 외부 ADC Referecne(2.048 V) 사용할 때
	//AdcRegs.ADCREFSEL.bit.REF_SEL = 0x02; // 외부 ADC Referecne(1.500 V) 사용할 때
	//AdcRegs.ADCREFSEL.bit.REF_SEL = 0x03; // 외부 ADC Referecne(1.024 V) 사용할 때


	AdcRegs.ADCTRL3.bit.ADCCLKPS 	= 16;   // Set up ADCTRL3 register 0000 1000 -> HSPCLK/(ADCCTRL[7]+1)(by kim)
	AdcRegs.ADCTRL1.bit.CPS 		= 1; 	// 40ns * 2 = 80ns
	AdcRegs.ADCTRL1.bit.ACQ_PS 		= 0x1;  // aquisition window time (ACQ_PS+1)*160ns

	AdcRegs.ADCTRL3.bit.SMODE_SEL = 0x0; 	// 샘플링모드(0-->시퀸스(순차적) 샘플링, 1--> 동시샘플링) 
	AdcRegs.ADCTRL1.bit.SEQ_CASC  = 0x0;	// 시퀸스선택
	                                        //(0-->Dual-sequencer mode. SEQ1 and SEQ2 operate as two 8-state sequencers.)
	                                        //(1-->Casecaded mode, SEQ1 and SEQ2 operate as a single 16-state sequencers.)

	//ADCTRL1
	AdcRegs.ADCTRL1.bit.CONT_RUN  = 0;		// 변환모드설정(Start-stop mode), 0-->SOC신호로 변환, 1--> 연속변환모드	
	AdcRegs.ADCTRL1.bit.SEQ_OVRD  = 0;	    // Override disabled	

	//ADCTRL2 (SEQ1 initializaton)
	AdcRegs.ADCTRL2.bit.INT_ENA_SEQ1 	= 1;	//Interrupt from SEQ1 enabled
	AdcRegs.ADCTRL2.bit.INT_MOD_SEQ1 	= 0;    //INT_SEQ1 is set at the end of every SEQ1 sequence
	AdcRegs.ADCTRL2.bit.EPWM_SOCA_SEQ1	= 1;	//Allows SEQ1 to be started by ePWM1 SOCA trigger.

	//ADCTRL2 (SEQ2 initialization)
	AdcRegs.ADCTRL2.bit.INT_ENA_SEQ2 	= 1;	//Interrupt from SEQ2 enabled
	AdcRegs.ADCTRL2.bit.INT_MOD_SEQ2 	= 0;    //INT_SEQ2 is set at the end of every SEQ2 sequence
	AdcRegs.ADCTRL2.bit.EPWM_SOCB_SEQ2	= 1;	//Allows SEQ2 to be started by ePWM2 SOCB trigger.

	//Reset SEQ1 and SEQ2
	AdcRegs.ADCTRL2.bit.RST_SEQ1 		= 1;	//Immidiately reset SEQ1
	AdcRegs.ADCTRL2.bit.RST_SEQ2 		= 1;	//Immidiately reset SEQ2

	//Reset interrupt flags
	AdcRegs.ADCST.bit.INT_SEQ1_CLR 		= 1;       // Clear INT SEQ1 bit
	AdcRegs.ADCST.bit.INT_SEQ2_CLR 		= 1;       // Clear INT SEQ2 bit

	//AdcRegs.ADCMAXCONV.all = 0x22; 			    // 3 double conv's each sequencer (6 total)
	AdcRegs.ADCMAXCONV.all = 0x77; 			    // 3 double conv's each sequencer (6 total)
	//AdcRegs.ADCMAXCONV.all = 0x0f; 			    // 3 double conv's each sequencer (6 total)

	//Sequence for SOC1
	//AdcRegs.ADCCHSELSEQ1.bit.CONV00 = 0x0;  	// Setup conv from ADCINA0
	//AdcRegs.ADCCHSELSEQ1.bit.CONV01 = 0x2;  	// Setup conv from ADCINA1
	//AdcRegs.ADCCHSELSEQ1.bit.CONV02 = 0x7;  	// Setup conv from ADCINA2
	//Sequence for SOC1
/*
	AdcRegs.ADCCHSELSEQ1.bit.CONV00 = 0x6;  	// Setup conv from ADCINA0
	AdcRegs.ADCCHSELSEQ1.bit.CONV01 = 0x8;  	// Setup conv from ADCINA1
	AdcRegs.ADCCHSELSEQ1.bit.CONV02 = 0xA;  	// Setup conv from ADCINA2
	AdcRegs.ADCCHSELSEQ1.bit.CONV03 = 0x4;  	// Setup conv from ADCINA2

	AdcRegs.ADCCHSELSEQ3.bit.CONV04 = 0x2;  	// Setup conv from ADCINA2
	AdcRegs.ADCCHSELSEQ2.bit.CONV05 = 0x0;  	// Setup conv from ADCINA2

	//Sequence for SOC2
	AdcRegs.ADCCHSELSEQ3.bit.CONV08 = 0xC;  	// Setup conv from ADCINB4	<-- ADC6(ACDC(WV),DCDC(NONE))
	AdcRegs.ADCCHSELSEQ3.bit.CONV09 = 0xE;  	// Setup conv from ADCINB6  <-- ADC7(ACDC(DCV),DCDC(12V OUTV))
	AdcRegs.ADCCHSELSEQ3.bit.CONV10 = 0xF;  	// Setup conv from ADCINB7  <-- ADC_MP(1.5V REF)
 */

 	AdcRegs.ADCCHSELSEQ1.all = 0x3210;  	// Setup conv from ADCINB7  <-- ADC_MP(1.5V REF)
 	AdcRegs.ADCCHSELSEQ2.all = 0x7654;  	// Setup conv from ADCINB7  <-- ADC_MP(1.5V REF)
 	AdcRegs.ADCCHSELSEQ3.all = 0xBA98;  	// Setup conv from ADCINB7  <-- ADC_MP(1.5V REF)
 	AdcRegs.ADCCHSELSEQ4.all = 0xFEDC;  	// Setup conv from ADCINB7  <-- ADC_MP(1.5V REF)
 
	return;
}
