//==================================================================================//
// 원강선 동력차(30kw)/객차(50kw) 축전지 충전기 모듈(10kw) 프로그램	           		//
//==================================================================================//
// INPUT : 670Vdc(620~720Vdc)														//
// 출력용량 : 객차(50kW, 10kw*5module), 72Vdc ~ 83Vdc								//
// 회로방식 : 모듈(절연형 zvzcs dcdc converter										//
// 제어기 ; wongan Battery MCU(TMS320F28335)                                        //
// 프로그램명 : WGBC Control                                        //
//======================================================================================//
// ProgramVersion                                                    by PACTECH R&D LSH //
//------------------------------------------------------------------------------------------------------//
// REV      | DATE           | 내용                                                                     //
//------------------------------------------------------------------------------------------------------//
// ver0.01  | 2015년04월28일 | 4/24~25 객차/동력차 자체 온도상승시험한 프로그램							//
//------------------------------------------------------------------------------------------------------//
// ver0.01  | 2015년04월28일 | 박스폴트 처리부 수정(경고장인경우 모듈폴트처리 안하고 재기동)			//
//------------------------------------------------------------------------------------------------------//
// ver2.00  | 2015년0?월??일 | 조합시험전 니튬폴리머 배터리 특성이 그동안 시험하던 배터리와 특성틀려 다시 튜닝함			//
//------------------------------------------------------------------------------------------------------//
// ver1.00  | 2015년11월12일 | 시험시 SW2로 폴트 처리하는부분, 차량에서 리셋지령 수정     	   		    //
//                           | 1호기 완성차시험(11월16~18일)전 회사에서 프로그램 수정함                 //
//======================================================================================================//

#define	GLOBAL__VARIABLE_FUNCTION_H
#include "Global_Variable_Function.h"

#define ProgramMode_BOX0	0
#define ProgramMode_MODULE1	1
#define ProgramMode_CPU2	2
unsigned int ProgramMode_B0M1C2 = 0;	//20150404추가


//0---정상 모드
//1---mcu 테스트 모드
//2--모듈 테스트 모드

extern Uint16 msec_tmr;	


// These are defined by the linker (see FLASH.cmd)
// 이 변수는 메인 함수의 초기화 루틴에서 "ramfuncs" 이라고 정의된 코드 섹션을
// 내부 Flash에서 RAM영역으로 복사하기 위한 MemCopy 함수에서 사용됨.
extern Uint16 RamfuncsLoadStart;
extern Uint16 RamfuncsLoadEnd;
extern Uint16 RamfuncsRunStart;

Uint16 msec_1000m = 0,msec_500m = 0,msec_200m = 0,msec_100m = 0, msec_10m = 0;
Uint16 msec_1m = 0;
Uint16 msec_2m = 0;
Uint16 msec_400u = 0;

unsigned int fault_swclear = 0;
unsigned int secondcounter = 0;

//void BoardTest(unsigned char boardname);
Uint16 FanOnCmd = 0;
Uint16 DAC_Mode = 0;

Uint16 GPIO_INT0 = 0;
Uint16 GPIO_INT1 = 0;
Uint16 GPIO_INT2 = 0;
Uint16 GPIO_INT3 = 0;
Uint16 GPIO_INT4 = 0;
Uint16 GPIO_INT5 = 0;
Uint16 GPIO_INT6 = 0;
Uint16 GPIO_L_INT0 = 0;
Uint16 GPIO_L_INT1 = 0;
Uint16 GPIO_L_INT2 = 0;
Uint16 GPIO_L_INT3 = 0;
Uint16 GPIO_L_INT4 = 0;
Uint16 GPIO_L_INT5 = 0;
Uint16 GPIO_L_INT6 = 0;
void intpin_Chk();

Uint16 FaultClear_flag = 0;
//==================================================================================//
// Main()		    																//
//==================================================================================//
void main(void)
{

	//ProgramVersion = 101;// REV01.01 ---> 1,// REV01.23 ---> 123, // REV12.34 ---> 1234
	ProgramVersion = 2;// rev0.2 ---> 2015년4월28일
	ProgramVersion = 200;// rev0.2 ---> 2015년4월28일
	ProgramVersion = 100;// rev1.00 ---> 2015년11월12일-->완성차시험전 1호기 버전-->모니터프로그램 아나로그 SPARE1에 표현
	
	// 주변장치 초기화
	PeripheralInit();			// DSP 초기화
	DioOutBuf1 = 0x02;
	CS_WR_DOUT1() = DioOutBuf1;// ALL DOUT OFF

	System_Define();			// 장비셋업,시스템 정격
	Init_I2C_eeprom();			// eeprom 초기하
	
	PWM_OFF();			// PWM 출력 디스에이블
	TimerInterruptEnable(); 	// 타이머 인터럽트 인에이블
	
	VariableInit();				// 모든 변수 0으로 초기화
	FaultClear();				// 센서보드 하드웨어 폴트, DSP IDU폴트 클리어

	DOutputInit();				// IO보드 출력, LED SENSOR보드 LED 모두 끄기	

	delay_ms(500);				// 슬레이브 보드 하드웨어 안정화 시간
	FaultClear();				// 센서보드 하드웨어 폴트, DSP IDU폴트 클리어
	
	Check_DI();
	msec_1000m = msec_500m=msec_200m = msec_10m = msec_1m = msec_tmr;	// 일정 주기 task를 만들기 위해서 변수 초기화		

	EPwmInterruptEnable();		// PWM 인터럽트
	Nvram_Init();
	
	FaultClear();				// 센서보드 하드웨어 폴트, DSP IDU폴트 클리어
	ReloadCpuTimer1();  
	MidiumFault_3min3N_INIT();// 3분3회 중고장 처리변수 초기화
	
	//===20150404추가
//#define ProgramMode_BOX0	0
//#define ProgramMode_MODULE1	1
//#define ProgramMode_CPU2	2
//var Uint16 ProgramMode_B0M1C2;	//20150404추가
	if(ProgramMode_B0M1C2 == ProgramMode_CPU2)
	{
		while(1)
		{
			BoardTest(1);
		}
	}
	siv_test_init();
	//=======main loop===========//	
	while(TRUE)
	{
		//== 1.0 sec ====
		if( 5000 <= msec_dif(msec_tmr, msec_1000m))
		{		// 1000mse = 200usec*5000
			msec_1000m = msec_tmr;		
			secondcounter++;	
		}
		//== 0.5sec ====
		if( 2500 <= msec_dif(msec_tmr, msec_500m))
		{			// 500mse = 200usec*2500
			msec_500m = msec_tmr;
			Nvram_TDpage_clear();
		}
		//== 0.1sec ====
		if( 500 <= msec_dif(msec_tmr, msec_100m))
		{			// 100ms = 200usec*500; 
			msec_100m = msec_tmr;
//			MCU_D33_TOGGLE();	//D33				//20220325 전자피시험 대비 비활성화

		}
		//== 10msec ====
		if( 50 <= msec_dif(msec_tmr, msec_10m))
		{			// 10ms = 200usec*50; 
			msec_10m = msec_tmr;
			Sequence();
			if(fault_swclear == 1)
			{
				fault_swclear = 0;
				FaultClear();
			}
		}
		//== 400usec ====	
		if( 2 <= msec_dif(msec_tmr, msec_400u))
		{				// 1ms = 200usec*2; 
			msec_400u = msec_tmr;
		}	
		//== 1msec ====	
		if( 5 <= msec_dif(msec_tmr, msec_1m))
		{				// 1ms = 200usec*5; 
			msec_1m = msec_tmr;
			RTC_Read();
			RTC_Write();
			NvramSDTDWrite();
			DAC_Out_test(DAC_Mode);
		}
		//== 2msec ====	
		if( 10 <= msec_dif(msec_tmr, msec_2m))
		{				// 2ms = 200usec*10; 
			msec_2m = msec_tmr;
			Can_Routine();
		}

		if(FaultClear_flag == 1)
		{
			FaultClear();
			FaultClear_flag = 0;
		}
		
 		
	}   
}

//================================================================//
//==시스템 셋업 및 정격관련 정의
//================================================================//
void System_Define()
{
	
	Setting_Chk();	// 충전기 종류(객차,동력차) 및 모듈 ID 설정
		
	//모듈입력전압 --> ADCRD10
	MInputVolRATED = 670.0;		//시스템정격(670[V])
	MInputVolMAX = 720.0;		//시스템정격(720[V])
	MInputVolMIN = 620.0;		//시스템정격(620[V])
	MInputVolSWOVF = 900.0;		//시험절차서 기준
	MInputVolHWOVF = 1005.0;	//
//	MInputVolUVF = 620.0;
	MInputVolUVF = 600.0;

	//모듈 DCLINK전압(RATED 670Vdc) --> ADCRD11
	MDclinkVolRATED = 670.0;
	MDclinkVolMAX = 720.0;	
	MDclinkVolMIN = 620.0;
	MDclinkVolSWOVF = 900.0;
	MDclinkVolHWOVF = 1005.0;	//
	//MDclinkVolUVF = 620.0;
//	MDclinkVolUVF = 610.0; // 620V인데  시험중 폴트걸려 610V로 수정
	MDclinkVolUVF = 600.0; // 620V인데  시험중 폴트걸려 610V로 수정
//모듈출력전압(OBD후단) 정격 720Vdc(72~83) --> ADCRD9
	//MBatVolRATED = 73.0;
	//MBatVolRATED = 83.0;
	MBatVolRATED = 84.0;
	//MBatVolRATED = 80.0;//2015년8월11일 임시로 84v에서 80v로, 더션테크 배터리시험중
	MBatVolMAX = 83.0;
	MBatVolMIN = 72.0;
	//MBatVolSWOVF = 90.0;	//상세설계서 시험기준 90V
	//MBatVolSWOVF = 105.0;	//20150307임시로튜닝중
	MBatVolSWOVF = 90.0;	//20150307임시로튜닝중
	MBatVolHWOVF = 103.25;	//
//	MBatVolUVF = 40.0;		//상세설계서 시험기준 50V
	MBatVolUVF = 50.0;		//상세설계서 시험기준 50V

//모듈출력전압1(OBD전단) 정격 720Vdc(72~83) --> ADCRD8
	//MOutVolRATED = 74.0;
	MOutVolRATED = 84.0;
	MOutVolMAX = 83.0;
	MOutVolMIN = 72.0;
	//MOutVolSWOVF = 90.0;	//상세설계서 시험기준 90V
	//MOutVolSWOVF = 105.0;	//20150408임시로튜닝중
	MOutVolSWOVF = 90.0;	//20150307임시로튜닝중
	MOutVolHWOVF = 103.25;	//
//	MOutVolUVF = 40.0;		//시험절차서 기준
	MOutVolUVF = 50.0;		//시험절차서 기준

//모듈 제어전압 정격 72Vdc(72~83) --> ADCRD7
	MCtrlVolRATED = 72.0;
	MCtrlVolMAX = 83.0;
	MCtrlVolMIN = 72.0;
//	MCtrlVolSWOVF = 95.0;	//시험절차서 기준//92v
	//MCtrlVolSWOVF = 90.0;	//시험절차서 기준//92v
	MCtrlVolSWOVF = 92.0;	//시험절차서 기준//92v--20150522
	MCtrlVolHWOVF = 93.8;	//
	MCtrlVolUVF = 48.0;		//시험절차서 기준	
//모듈 입력전류 --> ADCRD0
	MInputCurRATED = 20.0;		//확인필요????
	MInputCurMAX = 20.0;		//확인필요????
	MInputCurSWOCF = 30.00;		// 20150326,25A-->30A(시험절차서),--->20150407 보드수정하면서 정상으로 복귀(30A)
	MInputCurHWOCF = 30.00;		// 
//box 출력전류 --> ADCRD6
	if(WGBC_TC0PC1 == WGBC_TC0) 
	{	// 객차용 축전지 충전기
		BOutCurRATED = 695.0;		//객차 상세설계서 사양, 충전전전류는 60A, 과충전전류 66A(시험기준사양)
		BOutCurMAX = 835.0;			//객차 과부하 120%
		BOutCurSWOCF = 900.0;		//객차 상세설계서/시험기준 사양
		BOutCurHWOCF = 928.0;		//
		//모듈 충전전류 --> ADCRD1
		MBatCurRATED = 12.0;	//객차 상세설계서, 60[A]/5모듈 = 12[A]/M
		MBatCurMAX = 14.4;		//정격*120%로, 60[A]/5모듈*120% = 12[A]/M*1.2
		MBatCurSWOCF = 95.0;	//상세설계서 시험기준, 박스충전 과전류보다 작아서 이상함
		MBatCurHWOCF = 120.0;	
	}
	else
	{
		BOutCurRATED = 417.0;		//동력차 상세설계서 사양, 충전전전류는 68A, 배터리1개당 과충전 75A(시험기준사?)
		BOutCurMAX = 500.0;			//동력차 과부하 120%
		BOutCurSWOCF = 550.0;		//동력차 상세설계서/시험기준 사양
		BOutCurHWOCF = 593.0;		//
		//모듈 충전전류 --> ADCRD1
		MBatCurRATED = 22.6;	//동력차 상세설계서, 68[A]/3모듈 = 22.6[A]/M
		MBatCurMAX = 27.2;		//정격*120%로, 68[A]/3모듈*120% = 22.6[A]/M*1.2
		MBatCurSWOCF = 95.0;	//상세설계서 시험기준, 박스충전 과전류보다 작아서 이상함
		MBatCurHWOCF = 120.0;	
	}
	
//모듈 출력전류 --> ADCRD2
	MOutCurRATED = 138.9;	// 10kW/72V = 138.9[A]
	MOutCurMAX = 166.7;		// 10kW/72V = 138.9[A] * 1.2
	MOutCurSWOCF = 180.0;	// 상세설계서 시험기준 180A
	MOutCurHWOCF = 190.0;	// 

//box BAT전류1 --> ADCRD4
//box BAT전류2 --> ADCRD5 (객차 BC는 사용안함)
	if(WGBC_TC0PC1 == WGBC_TC0) 
	{	// 객차용 축전지 충전기
		//box BAT전류1 --> ADCRD4
		BBatCur1RATED = 60.0;	// 객차 상세설계서 사양 60A
		BBatCur1MAX = 72.0;		// 객차 과부하120%	
		BBatCur1SWOCF = 75.0;	// 객차,동력차 상세설계서 시험기준 75A
//		BBatCur1SWOCF = 95.0;	// 20151116_창원 객차 테스트 (BOX충전과전류 발생)
		//BBatCur1SWOCF = 100.0;	// 20150326--부하급변시험중 폴트나서 우선 제한 올림
		BBatCur1HWOCF = 102.3;	// 
		//box BAT전류2 --> ADCRD5 (객차 BC는 사용안함)
		BBatCur2RATED = 0.0;	//
		BBatCur2MAX = 0.0;		//
		BBatCur2SWOCF = 75.0;	// 객차는 사용안함 75A
		BBatCur2HWOCF = 102.3;	// 객차는 사용안함 
		//box BAT전류 --> 객차 BC = BAT1 + BAT2(0)
		BBatCurRATED = BBatCur1RATED + BBatCur2RATED;	// 
		BBatCurMAX = BBatCur1MAX + BBatCur2MAX;			//
		BBatCurSWOCF = BBatCur1SWOCF + BBatCur2SWOCF;	//
	}
	else
	{						// 동력차용 축전지 충전기
		//box BAT전류1 --> ADCRD4
		BBatCur1RATED = 34.0;	// 동력차 상세설계서 사양 68A
		BBatCur1MAX = 81.6;		// 동력차 과부하120%
		BBatCur1SWOCF = 75.0;	// 객차,동력차 상세설계서 시험기준 75A
		BBatCur1HWOCF = 102.3;	// 
		//box BAT전류2 --> ADCRD5 (객차 BC는 사용안함) --- 배터리 2개라도 충전전류/폴트베렐등은 최대치로
		BBatCur2RATED = 34.0;	// 동력차 상세설계서 사양 68A
		BBatCur2MAX = 81.6;		// 동력차 과부하120%
		BBatCur2SWOCF = 75.0;	// 동력차 상세설계서 시험기준 75A
		BBatCur2HWOCF = 102.3;	// 
		//box BAT전류 --> 동력차BC = BAT1 + BAT2
		//BBatCurRATED = 45.0;//BBatCur1RATED + BBatCur2RATED;	// 
		BBatCurRATED = BBatCur1RATED + BBatCur2RATED;	// 
		BBatCurMAX = BBatCur1MAX + BBatCur2MAX;			//
		BBatCurSWOCF = BBatCur1SWOCF + BBatCur2SWOCF;	//
	}
}
//==================================================================================//
// 함수명 : Setting_Chk()		            										//
// DATE(수정)  : 2015. 01. 16														//
//==================================================================================//
void Setting_Chk()
{
	Uint16 Temp0_GPIO = 0;
	Uint16 Temp1_GPIO = 0;
	Uint16 Temp2_GPIO = 0;
	Uint16 Temp3_GPIO = 0;		
	

	Temp0_GPIO = GpioDataRegs.GPADAT.bit.GPIO12;//SWHEX0--> HEX SWITCH BIT0
	Temp1_GPIO = GpioDataRegs.GPADAT.bit.GPIO13;//SWHEX1--> HEX SWITCH BIT1
	Temp2_GPIO = GpioDataRegs.GPADAT.bit.GPIO14;//SWHEX2--> HEX SWITCH BIT2
	Temp3_GPIO = GpioDataRegs.GPADAT.bit.GPIO15;//SWHEX3--> HEX SWITCH BIT3
	MCUBD_HEXSW_IN = !Temp3_GPIO*8 + !Temp2_GPIO*4 +!Temp1_GPIO*2 +!Temp0_GPIO*1;
	
	Temp0_GPIO = GpioDataRegs.GPADAT.bit.GPIO22;//SWDIP0
	Temp1_GPIO = GpioDataRegs.GPADAT.bit.GPIO23;//SWDIP1
	Temp2_GPIO = GpioDataRegs.GPADAT.bit.GPIO24;//SWDIP2
	Temp3_GPIO = GpioDataRegs.GPADAT.bit.GPIO25;//SWDIP3

	MCUBD_DIPSW_IN = (!Temp3_GPIO*8 + !Temp2_GPIO*4 +!Temp1_GPIO*2 +!Temp0_GPIO*1)&0xff;
	MCUBD_MODESW_IN = GpioDataRegs.GPADAT.bit.GPIO26;//SWSEL---> MODE SWITCH로 사용할까?(TEST MODE / NORMAL MODE)
	MCUBD_PUSHBUTTONSW_IN = !GpioDataRegs.GPADAT.bit.GPIO27;//SWPB - PUSH BUTTON SW	

//---모듈 모드 (테스트모드/NORMAL 모드)
	MODEAck = GpioDataRegs.GPADAT.bit.GPIO26;//SWSEL---> MODE SWITCH로 사(TEST MODE(0) / NORMAL MODE(1))

	if(MCUBD_HEXSW_IN == 0)
	{//객차 BC 모듈1
		WGBC_TC0PC1 = WGBC_TC0;
		WGBC_M_ID = WGBC_M_ID1;
		MCU_LD4_ON();	// TCBC(ON), PCBC(OFF)
	}
	else if(MCUBD_HEXSW_IN == 1)
	{//객차 BC 모듈2
		WGBC_TC0PC1 = WGBC_TC0;
		WGBC_M_ID = WGBC_M_ID2;
		MCU_LD4_ON();	// TCBC(ON), PCBC(OFF)
	}
	else if(MCUBD_HEXSW_IN == 2)
	{//객차 BC 모듈3
		WGBC_TC0PC1 = WGBC_TC0;
		WGBC_M_ID = WGBC_M_ID3;
		MCU_LD4_ON();	// TCBC(ON), PCBC(OFF)
	}
	else if(MCUBD_HEXSW_IN == 3)
	{//객차 BC 모듈4
		WGBC_TC0PC1 = WGBC_TC0;
		WGBC_M_ID = WGBC_M_ID4;
		MCU_LD4_ON();	// TCBC(ON), PCBC(OFF)
	}
	else if(MCUBD_HEXSW_IN == 4)
	{//객차 BC 모듈5
		WGBC_TC0PC1 = WGBC_TC0;
		WGBC_M_ID = WGBC_M_ID5;
		MCU_LD4_ON();	// TCBC(ON), PCBC(OFF)
	}
	
	else if(MCUBD_HEXSW_IN == 5)
	{//동력차 BC 모듈1
		WGBC_TC0PC1 = WGBC_PC1;
		WGBC_M_ID = WGBC_M_ID1;
		MCU_LD4_OFF();	// TCBC(ON), PCBC(OFF)
	}
	else if(MCUBD_HEXSW_IN == 6)
	{//동력차 BC 모듈2
		WGBC_TC0PC1 = WGBC_PC1;
		WGBC_M_ID = WGBC_M_ID2;
		MCU_LD4_OFF();	// TCBC(ON), PCBC(OFF)
	}
	else if(MCUBD_HEXSW_IN == 7)
	{//동력차 BC 모듈3
		WGBC_TC0PC1 = WGBC_PC1;
		WGBC_M_ID = WGBC_M_ID3;
		MCU_LD4_OFF();	// TCBC(ON), PCBC(OFF)
	}

	else if(MCUBD_HEXSW_IN == 10)
	{//모듈 테스트용
		WGBC_TC0PC1 = WGBC_PC1;
		WGBC_M_ID = WGBC_M_ID1;
		MCU_LD4_OFF();	// TCBC(ON), PCBC(OFF)
		ProgramMode_B0M1C2 = ProgramMode_MODULE1;//20150404추가
	}
	else if(MCUBD_HEXSW_IN == 11)
	{//MCU BOARD 검사용
		WGBC_TC0PC1 = WGBC_PC1;
		WGBC_M_ID = WGBC_M_ID1;
		MCU_LD4_OFF();	// TCBC(ON), PCBC(OFF)
		ProgramMode_B0M1C2 = ProgramMode_CPU2;//20150404추가
	}
	else
	{
		WGBC_TC0PC1 = WGBC_TC0;
		WGBC_M_ID = WGBC_M_ID1;
		MCU_LD4_ON();	// TCBC(ON), PCBC(OFF)
		ProgramMode_B0M1C2 = ProgramMode_BOX0;//20150404추가
	}
}

//==================================================================================//
// 함수명 : int핀 체크()		            										//
// DATE(수정)  : 2015. 04. 22														//
//==================================================================================//
void intpin_Chk()
{
	
	// WGMCU-INT 입력(DIR-->0)
	GpioCtrlRegs.GPBMUX2.bit.GPIO52 = 0;	//-INT0
	GpioCtrlRegs.GPBMUX2.bit.GPIO51 = 0;	//-INT1
	GpioCtrlRegs.GPBMUX2.bit.GPIO50 = 0;	//-INT2---pcb에서 gnd로 연결됨 또는 open되어 있음
	GpioCtrlRegs.GPBMUX2.bit.GPIO49 = 0;	//-INT3
	GpioCtrlRegs.GPBMUX2.bit.GPIO48 = 0;	//-INT4
	GpioCtrlRegs.GPBMUX2.bit.GPIO58 = 0;	//-INT5
	GpioCtrlRegs.GPBMUX2.bit.GPIO57 = 0;	//-INT6
		

	GPIO_INT0 = GpioDataRegs.GPBDAT.bit.GPIO52;//-INT0
	GPIO_INT1 = GpioDataRegs.GPBDAT.bit.GPIO51;//SWHEX1--> HEX SWITCH BIT1
	GPIO_INT2 = GpioDataRegs.GPBDAT.bit.GPIO50;//SWHEX2--> HEX SWITCH BIT2
	GPIO_INT3 = GpioDataRegs.GPBDAT.bit.GPIO49;//SWHEX3--> HEX SWITCH BIT3
	
	GPIO_INT4 = GpioDataRegs.GPBDAT.bit.GPIO48;//SWDIP0
	GPIO_INT5 = GpioDataRegs.GPBDAT.bit.GPIO58;//SWDIP1
	GPIO_INT6 = GpioDataRegs.GPBDAT.bit.GPIO57;//SWDIP2

	if(!GPIO_INT0)		GPIO_L_INT0 = 1;//hwf0
	if(!GPIO_INT1)		GPIO_L_INT1 = 1;//hwf1
	//if(!GPIO_INT2)		GPIO_L_INT2 = GPIO_INT2;//hwf2
	if(!GPIO_INT3)		GPIO_L_INT3 = 1;//hwf2
	if(!GPIO_INT4)		GPIO_L_INT4 = GPIO_INT4;//--hwf1,2,3
	if(GPIO_INT5)		GPIO_L_INT5 = GPIO_INT5;//gdf,hwfs --->f면 h, latch=0
	if(GPIO_INT6)		GPIO_L_INT6 = GPIO_INT6;//pwm


}

//==========================================================//
// 변수 초기화
//==========================================================//
void VariableInit()
{
	//ProgramMode_B0M1C2 = 0;	//20150404추가
	
	msec_500m = msec_200m = msec_10m = msec_1m = msec_tmr = 0;	// 일정 주기 task를 만들기 위해서 변수 초기화

 	DioOutBuf1 = 0x02;
	MCUBD_PUSHBUTTONSW_IN = 0;
	//=====sequence 관련변수 초기화============//
	Seq_Status = 0;	// Init = SeqStatus_INIT_STATE0(0)
	SEQ_Step = 0;	// Init = SEQ_DO_NOTHING_STATE(0)
	SeqTimer = 0;
	SeqTimerSet = 0;
	SeqTimer_flag = 0;

	//=====FAULT 관련 변수============//
	HighFaultMJF = 0;

	FGDF = 0;
	FHWF = 0;
	FSWF = 0;
 	FSQF = 0;	
    FETF = 0;				//
    FBXF = 0;				//
 
	FLGDF = FGDF;
	FLHWF = FHWF;
	FLSWF = FSWF;
	FLSQF = FSQF;
	FLETF = FETF;
	FLBXF = FBXF;  	
	
	
	FaultRestartTimer = 0;
	FaultRestartTimerSet = 500;		// fault후 재기동 시간 10msec*500 = 5sec
	FaultRestartTimer_flag = FLAGSET;
	
	FaultCheckCounter = 0;
	
	
	MajorFault = 0;

	FaultCounter = 0;
	FaultCounter_old = 0;

	OverLoadAck = 0;	// 과부하명 1, 과부하 1분이상 지속시 폴트처리
	OverLoad_Counter = 0;
	THRFAck_Counter = 0; // 히트싱크 90도이상 1분
	FANF1Ack_Counter = 0;// 팬폴트1 10초
	FANF2Ack_Counter = 0;// 팬폴트2 10초
		
	PSF_Counter = 0;	// PSF 딜레이 카운터 초기화(10msec*20 = 200msec)
	//=====Acknowledge 관련 변수초기화============//

	Mode_CV0CC1 = Mode_CV0;// CVCC 모드(#define Mode_CV0	0	#define Mode_CC1	1)
	MODEAck = NORMALMODE1; // TESTMODE0, NORMALMODE1
	MInputVolAck = 0;
 	FaultAck = 0;
 	FaultAckOld = 0;
	ModuleFault_NUM = 0;		
	Pall_OK_ModuleN = 0;
	RESETAck = 0;
	BATOC1Ack = 0;
	BATOC2Ack = 0;
	BCSTARTAck = 0;
	MOUTV_OKAck = 0;
	M1FAck = 0;
	M2FAck = 0;
	M3FAck = 0;
	M4FAck = 0;
	M5FAck = 0;
	MFAck_MY = 0;
	ModuleFault2ea_flag = 0; 	
	BCModeAck = 0;
	CHKAck = 0;
	LKAck = 0;
	BC72VAck = 0;
	TEMP85SWAck = 0;
	FANF1Ack = 0;
	FANF2Ack = 0;
	FUSEAck = 0;
	FanOnOffCmd_flag = 0;//fan off 	
	PWM_FLAG = 0;
	

	//---전압제어기만 할때 게인
	V_Kp = 0.005;// 0.05; 20150128_gain 수정
	V_Ki = 1.00;//0.03;

	I_Kp = 0.001;
	I_Ki = 0.025;

	I_Kp = 0.01;//-----
	I_Ki = 0.25;



	//I_Kp = 0.03;
	//I_Ki = 0.40;
	//---전류제어기추가하면서 변경-->20150202_23_58
	//--->1KW기본부하에서 5KW급변-->10KW부하급변은 폴트남
	V_Kp = 3.000;// 0.05; 
	V_Ki = 150.00;//0.03;
	I_Kp = 0.001;
	I_Ki = 1.000;
	//---20150314튜닝 동차3모듈
	V_Kp = 3.0;//1.200;// 0.05; 
	V_Ki = 150.00;//0.03;
	I_Kp = 0.004;
	I_Ki = 0.100;	
	//---20150408튜닝 동차3모듈
	V_Kp = 3.0;//1.200;// 0.05; 
	V_Ki = 150.00;//0.03;
	I_Kp = 0.0004;
	I_Ki = 4.000;	
//----윤대리

	//---전압+전류 제어기 게인
	V_Kp = 60.0;// 0.05; 20150128_gain 수정
	V_Ki = 350.00;//0.03;

	I_Kp = .1;//0303
	I_Ki = 20.0;//0303	
	V_ref = 0.0;
	V_Duty_Uint = 0.0;
	OutPutCur_Ka = 1/I_Kp;
	OutPutVol_Ka = 1/V_Kp;
//---20150409튜닝 동차3모듈
	V_Kp = 60.0;
	V_Ki = 350.00;
	I_Kp = 0.3;
	I_Ki = 30.;		
//---20150818튜닝 객차튜닝--리튬폴리머배터리
	V_Kp = 60.0;//60-->30-->60.
	V_Ki = 350.00;
	I_Kp = 0.1;//0.3-->0.1
	I_Ki = 30.;		
	V_Duty_Uint = 0.0;
	OutPutCur_Ka = 1/I_Kp;
	OutPutVol_Ka = 1/V_Kp;	
//-----------
//	OutPutCur_Ka = 0.0;
	OutVol_KI_damp = 1.1;

//	DutyLimit = 0.92;//0.85;//우선 80%듀티
	DutyLimit = 0.8;//0.85;//우선 80%듀티
	DutyLimit = 0.9;//0.85;//우선 80%듀티
	
	
//==모듈 충전전류 제한관련=================================//
	MBatCur_Kp = 0.001;
	MBatCur_Ki = 100.0;
	MBatCurRef = MBatCurRATED;	// 모듈 충전전류지령 
//==박스 충전전류 제한관련=================================//
	BBatCur_Kp = 1.0;//0.5;
	BBatCur_Ki = 0.2;

	//BBatCur_Kp = 0.05;//1.0;
	//BBatCur_Ki = 0.2;
	//BBatCur_Kp = 0.1;//0.05 -->0.1-->0.5-->0.1-->0.05;//20150811 더션테크 배터리로 시험
	//BBatCur_Ki = 2.0;//0.2-->2.0-->5.0->2.0-->1.0;//20150811 더션테크 배터리로 시험
	BBatCur_Kp = 0.005;//0.05 -->0.1-->0.5-->0.1-->0.05;//20150811 더션테크 배터리로 시험
	BBatCur_Ki = 3.0;//0.2-->2.0-->5.0->2.0-->1.0;//20150811 더션테크 배터리로 시험

	BBatCurRef = BBatCurRATED;//BBatCurRATED;
	
//=====Softstart 전압 ref 증분값===========================		
	//DCDC_SoftStart_dv = 0.002;//0.005-->약 1.1초
	DCDC_SoftStart_dv = 0.001;//0.002 ---> 0.001, 2015년8월11일 더션테크 배터리로 시험
//	DCDC_SoftStart_dv = 0.001;//0.005-->약 1.1초

	//========CAN 관련변수

	cantxidx = 0;
	canrxidx = 0;


	InitADC_Var();	
	adc0_sum = 0;
	adc1_sum = 0;
	adc2_sum = 0;
	adc3_sum = 0;
	adc4_sum = 0;
	adc5_sum = 0;
	adc6_sum = 0;


	InitControlVar();
	
	BCM_HartBeat = 0;
	BCBox_Status = 0;
	
	BCM_Status1 = 0;
	BCM_Status2 = 0;
	BCM_Status3 = 0;
	BCM_Status4 = 0;

	BCM_TDF1 = 0;
	BCM_TDF2 = 0;
	BCM_TDF3 = 0;
	BCM_TDF4 = 0;

	BCM_SDF1 = 0;
	BCM_SDF2 = 0;
	BCM_SDF3 = 0;
	BCM_SDF4 = 0;  

}    

//==================================================================================//
// 함수명 : Setting_Chk()		            										//
// DATE(수정)  : 2015. 01. 16														//
//==================================================================================//
void Setting_Chk_old()
{
	Uint16 Temp0_GPIO = 0;
	Uint16 Temp1_GPIO = 0;
	Uint16 Temp2_GPIO = 0;
	Uint16 Temp3_GPIO = 0;		
	

	Temp0_GPIO = GpioDataRegs.GPADAT.bit.GPIO12;//SWHEX0--> HEX SWITCH BIT0
	Temp1_GPIO = GpioDataRegs.GPADAT.bit.GPIO13;//SWHEX1--> HEX SWITCH BIT1
	Temp2_GPIO = GpioDataRegs.GPADAT.bit.GPIO14;//SWHEX2--> HEX SWITCH BIT2
	Temp3_GPIO = GpioDataRegs.GPADAT.bit.GPIO15;//SWHEX3--> HEX SWITCH BIT3
	MCUBD_HEXSW_IN = !Temp3_GPIO*8 + !Temp2_GPIO*4 +!Temp1_GPIO*2 +!Temp0_GPIO*1;
	
	Temp0_GPIO = GpioDataRegs.GPADAT.bit.GPIO22;//SWDIP0
	Temp1_GPIO = GpioDataRegs.GPADAT.bit.GPIO23;//SWDIP1
	Temp2_GPIO = GpioDataRegs.GPADAT.bit.GPIO24;//SWDIP2
	Temp3_GPIO = GpioDataRegs.GPADAT.bit.GPIO25;//SWDIP3

	MCUBD_DIPSW_IN = (!Temp3_GPIO*8 + !Temp2_GPIO*4 +!Temp1_GPIO*2 +!Temp0_GPIO*1)&0xff;
	MCUBD_MODESW_IN = GpioDataRegs.GPADAT.bit.GPIO26;//SWSEL---> MODE SWITCH로 사용할까?(TEST MODE / NORMAL MODE)
	MCUBD_PUSHBUTTONSW_IN = !GpioDataRegs.GPADAT.bit.GPIO27;//SWPB - PUSH BUTTON SW	

//---모듈 모드 (테스트모드/NORMAL 모드)
	MODEAck = GpioDataRegs.GPADAT.bit.GPIO26;//SWSEL---> MODE SWITCH로 사(TEST MODE(0) / NORMAL MODE(1))

	if((MCUBD_HEXSW_IN == 0)|(MCUBD_HEXSW_IN == 5)){//객차 BC 모듈1
		WGBC_TC0PC1 = WGBC_TC0;
		WGBC_M_ID = WGBC_M_ID1;
		MCU_LD4_ON();	// TCBC(ON), PCBC(OFF)
	}
	else if((MCUBD_HEXSW_IN == 1)|(MCUBD_HEXSW_IN == 6)){//객차 BC 모듈2
		WGBC_TC0PC1 = WGBC_TC0;
		WGBC_M_ID = WGBC_M_ID2;
		MCU_LD4_ON();	// TCBC(ON), PCBC(OFF)
	}
	else if((MCUBD_HEXSW_IN == 2)|(MCUBD_HEXSW_IN == 7)){//객차 BC 모듈3
		WGBC_TC0PC1 = WGBC_TC0;
		WGBC_M_ID = WGBC_M_ID3;
		MCU_LD4_ON();	// TCBC(ON), PCBC(OFF)
	}
	else if((MCUBD_HEXSW_IN == 3)|(MCUBD_HEXSW_IN == 8)){//객차 BC 모듈4
		WGBC_TC0PC1 = WGBC_TC0;
		WGBC_M_ID = WGBC_M_ID4;
		MCU_LD4_ON();	// TCBC(ON), PCBC(OFF)
	}
	else if((MCUBD_HEXSW_IN == 4)|(MCUBD_HEXSW_IN == 9)){//객차 BC 모듈5
		WGBC_TC0PC1 = WGBC_TC0;
		WGBC_M_ID = WGBC_M_ID5;
		MCU_LD4_ON();	// TCBC(ON), PCBC(OFF)
	}
	
	else if((MCUBD_HEXSW_IN == 10)|(MCUBD_HEXSW_IN == 13)){//동력차 BC 모듈1
		WGBC_TC0PC1 = WGBC_PC1;
		WGBC_M_ID = WGBC_M_ID1;
		MCU_LD4_OFF();	// TCBC(ON), PCBC(OFF)
	}
	else if((MCUBD_HEXSW_IN == 11)|(MCUBD_HEXSW_IN == 14)){//동력차 BC 모듈2
		WGBC_TC0PC1 = WGBC_PC1;
		WGBC_M_ID = WGBC_M_ID2;
		MCU_LD4_OFF();	// TCBC(ON), PCBC(OFF)
	}
	else if((MCUBD_HEXSW_IN == 12)|(MCUBD_HEXSW_IN == 15)){//동력차 BC 모듈3
		WGBC_TC0PC1 = WGBC_PC1;
		WGBC_M_ID = WGBC_M_ID3;
		MCU_LD4_OFF();	// TCBC(ON), PCBC(OFF)
	}
	else{
		WGBC_TC0PC1 = WGBC_TC0;
		WGBC_M_ID = WGBC_M_ID1;
		MCU_LD4_ON();	// TCBC(ON), PCBC(OFF)
	}
}
