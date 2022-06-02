//==================================================================================//
// 원강선 동력차(30kw)/객차(50kw) 축전지 충전기 모듈(10kw) 프로그램	           		//
//==================================================================================//
// main header file														//
// DATE : 2014. 11. 21            최초작성				  by PACTECH R&D LSH		//
//==================================================================================//


//==================================================================================//
#ifndef __NTHS_MAIN__   
#define __NTHS_MAIN__   

//***************************************************************//

//===============================================================//

//============================================
// 변수선언
//============================================
var Uint16 	MainEpldData;
var Uint16 	Sen1EpldData;

//var Uint16	boostConPwmModulation;

var Uint16	dcdcPwmModulation;

//==================================================================================//
// DATE : 2014. 11. 21            						  by PACTECH R&D LSH		//
//==================================================================================//
//==시스템 정격관련===

var Uint16 ProgramVersion;// 

var float	InputVolRATED;			// 모듈입력전압 정격
var float	InputCurRATED;			// 모듈입력전류 정격
var float	DclinkVolRATED;			// 모듈dclink전압 정격
var float	Bat1OutCurRATED;		// 모듈충전전류1 정격
var float	Bat2OutCurRATED;		// 모듈충전전류2 정격

var float	BoxOutCurRATED;			// box전체 출력전류
var float	BoxBat1OutCurRATED;		// box전체 충전(bat)전류1
var float	BoxBat2OutCurRATED;		// box전체 충전(bat)전류2

var float	OutPowerRATED;
var float	OutVolRATED;
var float	OutCurRATED;







//--------------------------------------------------------------------
var float 	InputVol;			//모듈입력전압
var float 	InputVolLPF;		
var float	InputVolSWOVF;
var float	InputVolHWOVF;
var float	InputVolMAX;
var float	InputVolMIN;
var float	InputVolUVF;

var float 	DcLinkVol;			//모듈 DCV,	DC-LINK 전압, RATED 670Vdc
var float 	DcLinkVolLPF;
var float	DclinkVolHWOVF;	
var float	DclinkVolSWOVF;	
var float	DclinkVolMAX;
var float	DclinkVolMIN;
var float	DclinkVolLKON;
var float	DclinkVolUVF;
	//--- 출력전압 정격 720Vdc(72~83)
var float 	OutVol;				
var float 	OutVolLPF;			
var float	OutVolSWOVF;
var float	OutVolHWOVF;
var float	OutVolMAX;
var float	OutVolMIN;
var float	OutVolUVF;

	//--- 모듈입력전류 (???)
var float 	InputCur;		//입력전류
var float 	InputCurLPF;			//
var float	InputCurSWOCF;			//
var float	InputCurHWOCF;			//
var float	InputCurMAX;
var float	InputCurMIN;
	//--- 출력전류 정격 800Adc
var float 	OutCur;		//DCOC	DC출력전류
var float 	OutCurLPF;		//DCOC	DC출력전류
var float	OutCurHWOCF;
var float 	OutCurSWOCF;
var float	OutCurMAX;
var float	OutCurMIN;
	//--- 모듈충전전류1 (???)
var float 	Bat1OutCur;				//
var float 	Bat1OutCurLPF;			//
var float 	Bat1OutCurHWOCF;
var float 	Bat1OutCurSWOCF;
	//--- 모듈충전전류2 (???)
var float 	Bat2OutCur;				//
var float 	Bat2OutCurLPF;			//
var float 	Bat2OutCurHWOCF;
var float 	Bat2OutCurSWOCF;

//---------------------------------------------
// 시퀸스 관련변수
// Nths_Seq.c
//---------------------------------------------
var Uint16	SeqTimer;
var Uint16	SeqTimerSet;
var Uint16	SeqTimer_flag;
var Uint16	FaultRestartTimer;
var Uint16	FaultRestartTimerSet;
var Uint16	FaultRestartTimer_flag;


var Uint16	DCDC_SoftStart_flag;
var Uint16	DCDC_SoftStart_counter;
var float	DCDC_SoftStart_dv;




#define FLAGSET		1
#define FLAGCLR		0

var Uint16	RunOKNG; // Init = APS_RUN_NG(0)
#define RUN_OK	1
#define RUN_NG	0

var Uint16	Sys_Status;	// Init = SYS_INIT_STATE(0)
#define SYS_INIT_STATE			0
#define SYS_CHK_STATE			1
#define SYS_Restart_STATE		2
#define SYS_MajorFault_STATE	3

var Uint16	SEQ_Step;// Init = SEQ_DO_NOTHING_STATE(0)
#define SEQ_DO_NOTHING_STATE	0
#define SEQ_START				1
#define SEQ_CHK_ON				2
#define SEQ_CHKa_ON_Delay		3
#define SEQ_CHKa_CHK			3

#define SEQ_LK_ON				4
#define SEQ_LKa_ON_Delay		5
#define SEQ_LKa_CHK				5
#define SEQ_CHK_OFF				6

#define SEQ_DCDC_PWM_ON			7
#define SEQ_OUTV_CHK			8

#define SEQ_Normal_State		10
#define SEQ_ALLOFF				11
#define SEQ_RESET				12

var Uint16	InputVolAck;		// 입력전압검지



//---------------------------------------------
// 폴트 관련 변수
//---------------------------------------------
var Uint16 LowFaultAck;		// 1. 경고장 
var Uint16 MidFaultAck;		// 2. 경중고장(3분이내 동일고장 3회시 중고장 처리되는 고장)
var Uint16 MidFaultAckold;		// 2. 경중고장(3분이내 동일고장 3회시 중고장 처리되는 고장)
var Uint16 HighFaultAck;	// 3. 중고장 
//==MidFaultAck BIT 정의====
#define MidGDF		0
#define MidINOVF	1
#define MidINOCF	2
#define MidOVOVF	3

#define MidOCOCF	4
#define MidSEQF		5	
#define MidFCOVF	6
#define MidFCOCF	8

#define MidFanf		9
#define MidPSF		7
//	#define MidFanf		9==>팬폴트 임시로 중간고장, 원래는 중고장임

/*
sbit(MidFaultAck, MidGDF);		// 중간레벨(3분3회) - GDF
sbit(MidFaultAck, MidINOVF);	// 중간레벨(3분3회) - 입력과전압
sbit(MidFaultAck, MidINOCF);	// 중간레벨(3분3회) - 입력과전류
sbit(MidFaultAck, MidOVOVF);	// 중간레벨(3분3회) - 출력 과전압
sbit(MidFaultAck, MidOCOCF);	// 중간레벨(3분3회) - 출력 과전류 폴트
sbit(MidFaultAck, MidSEQF);	// 중간레벨(3분3회) - 시퀸스폴트
sbit(MidFaultAck, MidFCOVF);	// 중간레벨(3분3회) - FCV(DCLINK VOLTAGE) 과전압 폴트
sbit(MidFaultAck, MidFCOCF);	// 중간레벨(3분3회) - FCI(DCLINK CURRENT) 과전류 폴트
sbit(MidFaultAck, MidPSF);		// 중간레벨(3분3회) - 제어전원 저전압 폴트	
*/		
var Uint16 MidFaultMJF;		// 2. 경중고장(3분이내 동일고장 3회시 중고장 처리되는 고장) 
var Uint16 HighFaultMJF;	// 3. 중고장 
//====MidFaultAck관련 폴트변수
// GDF관련 변수
var Uint16 MidFCounter0;	//GDF1,2,3,4,5 발생횟수 
var Uint16 MidFTimer01;		//GDFs 첫번째 경과시간  
var Uint16 MidFTimer02;		//GDFs 두번째 경과시간  

var Uint16 MidFCounter1;	//입력과전압 폴트 발생횟수
var Uint16 MidFTimer11;		//입력과전압 폴트 첫번째 경과시간 
var Uint16 MidFTimer12;		//입력과전압 폴트 두번째 경과시간  

var Uint16 MidFCounter2;	//입력과전류 폴트 발생횟수
var Uint16 MidFTimer21;		//입력과전류 폴트 첫번째 경과시간  
var Uint16 MidFTimer22;		//입력과전류 폴트 두번째 경과시간  

var Uint16 MidFCounter3;	//출력과전압 폴트 발생횟수
var Uint16 MidFTimer31;		//출력과전압 폴트 첫번째 경과시간 
var Uint16 MidFTimer32;		//출력과전압 폴트 두번째 경과시간  

var Uint16 MidFCounter4;	//출력과전류 폴트 발생횟수
var Uint16 MidFTimer41;		//출력과전류 폴트 첫번째 경과시간 
var Uint16 MidFTimer42;		//출력과전류 폴트 두번째 경과시간  

var Uint16 MidFCounter5;	//과부하 폴트 발생횟수
var Uint16 MidFTimer51;		//과부하 폴트 첫번째 경과시간 
var Uint16 MidFTimer52;		//과부하 폴트 두번째 경과시간    

var Uint16 MidFCounter6;	//FCV(DCLINK VOLTAGE) 과전압 폴트 발생횟수
var Uint16 MidFTimer61;		//FCV(DCLINK VOLTAGE) 과전압 폴트 첫번째 경과시간 
var Uint16 MidFTimer62;		//FCV(DCLINK VOLTAGE) 과전압 폴트 두번째 경과시간  

var Uint16 MidFCounter7;	//FCI(DCLINK CURRENT) 과전류 폴트 발생횟수
var Uint16 MidFTimer71;		//FCI(DCLINK CURRENT) 과전류 폴트 첫번째 경과시간 
var Uint16 MidFTimer72;		//FCI(DCLINK CURRENT) 과전류 폴트 두번째 경과시간 

var Uint16 MidFCounter8;	//제어전원 저전압 폴트 발생횟수
var Uint16 MidFTimer81;		//제어전원 저전압 폴트 첫번째 경과시간 
var Uint16 MidFTimer82;		//제어전원 저전압 폴트 두번째 경과시간  

var Uint16 MidFCounter9;	//제어전원 저전압 폴트 발생횟수
var Uint16 MidFTimer91;		//제어전원 저전압 폴트 첫번째 경과시간 
var Uint16 MidFTimer92;		//제어전원 저전압 폴트 두번째 경과시간  
//=======================================================//


var Uint16	FaultCounter;
var Uint16	FaultCheckCounter;
var Uint16	FaultCounter_old;
var Uint16	MajorFault;
//---------------------------------------------
// GDU 폴트 관련 bit 정의
//---------------------------------------------
//--APS/BACH --
var Uint16	FaultGDFBuf;
#define GDF1		0	// APS(siv-U), BACH(dcdc-Leading Leg)
#define GDF2		1	// APS(siv-V), BACH(dcdc-Lagging Leg)
#define GDF3		2	// APS(siv-W), BACH(none)
#define GDF4		3	// APS,BACH (acdc - Leading Leg)
#define GDF5		4	// APS,BACH (acdc - Lagging Leg)
#define GDF6		5	// SPARE
#define GDF7		6	// SPARE
#define GDF8		7	// SPARE

//---------------------------------------------
// HW 폴트 관련 bit 정의
//---------------------------------------------
var Uint16	FaultHWBuf;


//--20140613---회로도--EPLD수정
//#define BACH_HWF_DCLINKOVF	3	//DCCT4F = DCLINK OVERVOLTAGE H/W 폴트추가함
//#define BACH_HWF_ACICOCF	4	//ACCT1F = ACIC, AC LINE 입력전류, 주회로(ACCT)
//#define BACH_HWF_DCLINKOCF	5	//ACCT2F = DCIC,DCLINK 과전류,  주회로(DCCT1)
//#define BACH_HWF_OCOCF		6	//ACCT3F = 출력 과전류,  주회로(DCCT2)
//#define BACH_HWF_BATOCF		7	//ACCT4F = BAT충전과전류,  주회로(DCCT3)

#define HWF_BIT0				0	//DCCT1F = 사용안함
#define HWF_IVOVF			1	//DCCT2F = 입력과전압, 주회로(PT1)
#define HWF_FCOVF		2	//DCCT3F = DCLINK과전압, 주회로(PT2)
#define HWF_OVOVF		3	//DCCT4F = 출력과전압, 주회로(PT3)
#define HWF_ICOCF		4	//ACCT1F = 입력과전류,  주회로(CT1)
#define HWF_IGBTOCF		5	//ACCT2F = IGBT출력과전류,  주회로(CT2)
#define HWF_OCOCF		6	//ACCT3F = 출력과전류,  주회로(CT3)
#define HWF_BIT7		7	//ACCT4F = 사용안함

//---------------------------------------------
// S/W 폴트 관련 변수 및 bit 정의
//---------------------------------------------
var Uint16 	FaultSWBuf;    //--low byte(SEN1_LED_CS1(), (U22 )), high byte(SEN1_LED_CS2(), (U23 ))

//--BACH --
#define 	SWF_IC_OCF			0	//입력 과전류 폴트
#define 	SWF_IGBT_OCF		1	//IGBT출력 과전류 폴트
#define 	SWF_OC_OCF			2	//출력전류 폴트/출력과전류
#define 	SWF_BIT3			3	//
#define 	SWF_BIT4			4	//SPARE
#define 	SWF_BIT5			5	//SPARE
#define 	SWF_BIT6			6	//SPARE
#define 	SWF_OVLDF			7	//과부하 폴트, 정격120% 1분이상 지속시

#define 	SWF_IV_OVF			8	//입력과전압
#define 	SWF_IV_LVF			9	//입력저전압
#define 	SWF_FCV_OVF			10	//DCLINK과전압
#define 	SWF_FCV_LVF			11	//DCLINK저전압
#define 	SWF_OV_OVF			12	//출력과전압
#define 	SWF_OV_LVF			13	//출력저전압
#define 	SWF_BIT14			14	//SPARE
#define 	SWF_BIT15			15	//SPARE

var Uint16	FaultSeqBuf;// Init = 0
#define SEQERR_CHKa		0
#define SEQERR_LKa		1
#define SEQERR_SIVKa	2
#define SEQERR_DCUV		3

#define SEQERR_OCVUV	4
#define SEQERR_BIT5		5
#define SEQERR_BIT6		6
#define SEQERR_BIT7		7

var Uint16	FaultEtcBuf;// Init = 0
#define ETCERR_MJF		0
#define ETCERR_MTF		1
#define ETCERR_OPF		2
#define ETCERR_ERF		3

#define ETCERR_FUSEF	4
#define ETCERR_PSF		5
#define ETCERR_FANF		6
#define ETCERR_HSKTHRF	7

#define ETCERR_BATTHRF	8
#define ETCERR_PCDF		8
#define ETCERR_BATF		9
#define ETCERR_BIT10	10
#define ETCERR_BIT11	11

#define ETCERR_BIT12	12
#define ETCERR_BIT13	13
#define ETCERR_BIT14	14
#define ETCERR_BIT15	15


var Uint16 	FaultAck;

var Uint16 	OverLoadAck;	// 과부하면 1
var Uint16 	OverLoadFAck;	// 과부하(OverLoadAck==1)상태가 1분이상 지속시 1
var Uint32 	OverLoad_Counter;
var Uint32 	THRFAck_Counter;	// 히트싱크 95도이상 1분 카운터

var Uint32 	PSF_Counter;	// PSF 발생하면 곧바로 폴트발생안시키고,,, 일정시간 지연 ==> 제어전원저전압에서 컨텍터 동작시 폴트나는것 방지
                            // 시험결과 약 100msec인데...여유로 150msec, 시간은 폴트동작인지딜레이시간동안 전압떨어져서 컨텍터 동작안하면 됨.

//============================================================================
// Function Prototype Declaration
//============================================================================	
// Nths_main.c
void System_Define();
void VarialbeUpdate();
void VariableInit();
// GlobalFunc.c
void TimerInterruptEnable();
void PeripheralInit();
void Init_Gpio(void);
void Init_External_Interrupt(void);
Uint16	msec_dif(Uint16 msec1, Uint16 msec2);

// debug_2833x.c
void sci_debug_init();// Initialize SCI-A for data monitoring 


#endif // __NTHS_MAIN__  




