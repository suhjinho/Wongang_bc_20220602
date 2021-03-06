//==================================================================================//
// 원강선 동력차(30kw)/객차(50kw) 축전지 충전기 모듈(10kw) 프로그램	           		//
//==================================================================================//
// main header file														//
// DATE : 2014. 11. 21            최초작성				  by PACTECH R&D LSH		//
//==================================================================================//


//==================================================================================//
#ifndef __NTHS_MAIN__   
#define __NTHS_MAIN__   


//============================================
// 변수선언
//============================================


var float Theta_radU1;
var float Theta_radV1;
var float Theta_radW1;

var float Theta_inc;

var float sineU1;
var float sineV1;
var float sineW1;

var float sineU2;
var float sineV2;
var float sineW2;

var float siv_test_Vrefin1;
var float siv_test_Vrefin2;
var float siv_test_Vrefin1_Scale;
var float siv_test_Vrefin2_Scale;


var Uint16	PWM_FLAG;

//==================================================================================//
// DATE : 2014. 11. 21            						  by PACTECH R&D LSH		//
//==================================================================================//
#define ProgramMode_BOX0	0
#define ProgramMode_MODULE1	1
#define ProgramMode_CPU2	2
var Uint16 ProgramMode_B0M1C2;	//20150404추가

//==시스템 정격관련===

var Uint16 ProgramVersion;// 
	
//--충전기 종류 설정	
#define WGBC_TC0	0
#define WGBC_PC1	1
var  unsigned int WGBC_TC0PC1;

//--10KW 모듈 Number(ID)설정
//--박스전면커버 기준 왼쪽부터 1..
//--동력차BC는 왼쪽부터 1,2,3   객차BC는 왼쪽부터 1,2,3,4,5
#define WGBC_M_ID1		1
#define WGBC_M_ID2		2
#define WGBC_M_ID3		3
#define WGBC_M_ID4		4
#define WGBC_M_ID5		5
//var  int WGBC_M_ID;
var  long WGBC_M_ID;

//--------------------------------------------------------------------
//모듈입력전압 --> ADCRD10
var float 	MInputVol;
var float 	MInputVolLPF;		
var float 	MInputVolRATED;			//시스템정격(670[V])
var float	MInputVolMAX;			//시스템정격(720[V])
var float	MInputVolMIN;			//시스템정격(620[V])
var float	MInputVolSWOVF;
var float	MInputVolHWOVF;
var float	MInputVolUVF;
//모듈 DCLINK전압(RATED 670Vdc) --> ADCRD11
var float 	MDcLinkVol;	
var float 	MDcLinkVolLPF;
var float	MDclinkVolRATED;		
var float	MDclinkVolMAX;
var float	MDclinkVolMIN;
var float	MDclinkVolSWOVF;	
var float	MDclinkVolHWOVF;	
var float	MDclinkVolUVF;
var float	TSTVAR11;
//모듈출력전압1(OBD후단) 정격 720Vdc(72~83) --> ADCRD9
var float 	MBatVol;
var float 	MBatVolLPF;			
var float	MBatVolRATED;
var float	MBatVolMAX;
var float	MBatVolMIN;
var float	MBatVolSWOVF;
var float	MBatVolHWOVF;
var float	MBatVolUVF;
//모듈출력전압1(OBD전단) 정격 720Vdc(72~83) --> ADCRD8
var float 	MOutVol;
var float 	MOutVolLPF;			
var float	MOutVolRATED;
var float	MOutVolMAX;
var float	MOutVolMIN;
var float	MOutVolSWOVF;
var float	MOutVolHWOVF;
var float	MOutVolUVF;
//모듈 제어전압 정격 72Vdc(72~83) --> ADCRD7
var float 	MCtrlVol;
var float 	MCtrlVolLPF;			
var float	MCtrlVolRATED;
var float	MCtrlVolMAX;
var float	MCtrlVolMIN;
var float	MCtrlVolSWOVF;
var float	MCtrlVolHWOVF;
var float	MCtrlVolUVF;	
//모듈 입력전류 --> ADCRD0
var float 	MInputCur;
var float 	MInputCurLPF;
var float	MInputCurRATED;
var float	MInputCurMAX;
var float	MInputCurSWOCF;
var float	MInputCurHWOCF;	
//box 출력전류 --> ADCRD6
var float 	BOutCur;
var float 	BOutCurLPF;	
var float	BOutCurRATED;
var float	BOutCurMAX;
var float	BOutCurHWOCF;
var float 	BOutCurSWOCF;
//모듈 충전전류 --> ADCRD1
var float 	MBatCur;				
var float 	MBatCurLPF;
var float 	MBatCurRATED;
var float 	MBatCurMAX;
var float 	MBatCurHWOCF;
var float 	MBatCurSWOCF;
//모듈 출력전류 --> ADCRD2
var float 	MOutCur;				
var float 	MOutCurLPF;
var float 	MOutCurRATED;
var float 	MOutCurMAX;
var float 	MOutCurHWOCF;
var float 	MOutCurSWOCF;
//box BAT전류1 --> ADCRD4
var float 	BBatCur1;				
var float 	BBatCur1LPF;			
var float 	BBatCur1RATED;
var float 	BBatCur1MAX;
var float 	BBatCur1HWOCF;
var float 	BBatCur1SWOCF;
//box BAT전류2 --> ADCRD5 (객차 BC는 사용안함)
var float 	BBatCur2;				
var float 	BBatCur2LPF;			
var float 	BBatCur2RATED;
var float 	BBatCur2MAX;
var float 	BBatCur2HWOCF;
var float 	BBatCur2SWOCF;
//box BAT전류 --> 동력차 BC =BAT1+BAT2, 객차 BC = BAT1
var float 	BBatCur;				
var float 	BBatCurLPF;			
var float 	BBatCurRATED;
var float 	BBatCurMAX;
var float 	BBatCurSWOCF;

//box 부하전류 --> box출력전류-box충전전류(BAT1+BAT2)
var float 	BLoadCur;				
var float 	BLoadCurLPF;			
var float 	BLoadCurRATED;
//module 부하전류 --> module출력전류-module충전전류
var float 	MLoadCur;				
var float 	MLoadCurLPF;			
var float 	MLoadCurRATED;


//=====Acknowledge 관련 변수

#define Mode_CV0	0
#define Mode_CC1	1
var Uint16 	Mode_CV0CC1;// CVCC 모드

var Uint16 	MODEAck;//dsp select switch, module mode 스위치로 사용
var Uint16	MInputVolAck;		// 입력전압검지
var Uint16 	FaultAck;
var Uint16 	FaultAckOld;
var Uint16 	ModuleFault_NUM;	// 10kW 모듈 폴터숫자
var Uint16 	Pall_OK_ModuleN;	// 10kW 모듈 폴터숫자

var Uint16 	RESETAck;
var Uint16 	RESETAck_OLD;
var Uint16 	BATOC1Ack;
var Uint16 	BATOC2Ack;
var Uint16 	BCSTARTAck;

var Uint16 	M1FAck;
var Uint16 	M2FAck;
var Uint16 	M3FAck;
var Uint16 	M4FAck;
var Uint16 	M5FAck;
var Uint16 	MFAck_MY;
var Uint16 	ModuleFault2ea_flag;

var Uint16 	BCModeAck;
var Uint16 	CHKAck;
var Uint16 	LKAck;
var Uint16 	BC72VAck;
var Uint16 	TEMP85SWAck;
var Uint16 	FANF1Ack;
var Uint16 	FANF2Ack;
var Uint16 	FUSEAck;

var Uint16 	BCSTATUSAck;
var Uint16 	MOUTV_OKAck;///

var Uint16 	FanOnOffCmd_flag;
//---------------------------------------------
// 시퀸스 관련변수
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

var Uint16	RunOKNG; // Init = RUN_NG(0)
#define RUN_OK	1
#define RUN_NG	0

//var Uint16	RunOKNG; // Init = RUN_NG(0)
//--MODEAck
#define TESTMODE0	0
#define NORMALMODE1	1

var Uint16	Seq_Status;	// Init = SeqStatus_INIT_STATE0(0)
#define SeqStatus_Init0			0
#define SeqStatus_Check1		1
#define SeqStatus_Restart2		2
#define SeqStatus_MajorFault3	3

var Uint16	SEQ_Step;// Init = SEQ_DO_NOTHING_STATE(0)
#define SEQ_DO_NOTHING_STATE	0
#define SEQ_START				1
#define SEQ_CHK_ON				2
#define SEQ_CHKa_CHK			3

#define SEQ_LK_ON				4
#define SEQ_LKa_CHK				5
#define SEQ_CHK_OFF				6

#define SEQ_PWM_ON				7
#define SEQ_OUTV_CHK			8

#define SEQ_Normal_State		9
#define SEQ_ALLOFF				10
#define SEQ_RESET				11


//---------------------------------------------
// 폴트 관련 변수
//---------------------------------------------
var Uint16 LowFaultAck;		// 1. 경고장 
var Uint16 MidFaultAck;		// 2. 경중고장(3분이내 동일고장 3회시 중고장 처리되는 고장)
var Uint16 HighFaultAck;	// 3. 중고장 
//==MidFaultAck BIT 정의====
#define MidGDF		0
#define MidMICOCF	1
#define MidMOCOCF	2

//#define MidOVOVF	3
//#define MidINOVF	1
//#define MidSEQF		5	
//#define MidFCOVF	6
//#define MidFCOCF	8
//#define MidFanf		9
//#define MidPSF		7
//	#define MidFanf		9==>팬폴트 임시로 중간고장, 원래는 중고장임

/*
sbit(MidFaultAck, MidGDF);		// 중간레벨(3분3회) - GDF
sbit(MidFaultAck, MidINOVF);	// 중간레벨(3분3회) - 입력과전압
sbit(MidFaultAck, MidMICOCF);	// 중간레벨(3분3회) - 입력과전류
sbit(MidFaultAck, MidOVOVF);	// 중간레벨(3분3회) - 출력 과전압
sbit(MidFaultAck, MidMOCOCF);	// 중간레벨(3분3회) - 출력 과전류 폴트
sbit(MidFaultAck, MidSEQF);	// 중간레벨(3분3회) - 시퀸스폴트
sbit(MidFaultAck, MidFCOVF);	// 중간레벨(3분3회) - FCV(DCLINK VOLTAGE) 과전압 폴트
sbit(MidFaultAck, MidFCOCF);	// 중간레벨(3분3회) - FCI(DCLINK CURRENT) 과전류 폴트
sbit(MidFaultAck, MidPSF);		// 중간레벨(3분3회) - 제어전원 저전압 폴트	
*/		
var Uint16 MidFaultMJF;		// 2. 경중고장(3분이? 동일고장 3회시 중고장 처리되는 고장) 
var Uint16 HighFaultMJF;	// 3. 중고장 
//====MidFaultAck관련 폴트변수
// GDF관련 변수
var Uint16 MidFCounter0;	//GDF1,2 발생횟수 
var Uint16 MidFTimer01;		//GDFs 첫번째 경과시간  
var Uint16 MidFTimer02;		//GDFs 두번째 경과시간  

var Uint16 MidFCounter1;	//입력과전류 폴트 발생횟수
var Uint16 MidFTimer11;		//입력과전류 폴트 첫번째 경과시간 
var Uint16 MidFTimer12;		//입력과전류 폴트 두번째 경과시간  

var Uint16 MidFCounter2;	//모듈출력과전류 폴트 발생횟수
var Uint16 MidFTimer21;		//모듈출력과전류 폴트 첫번째 경과시간  
var Uint16 MidFTimer22;		//모듈출력과전류 폴트 두번째 경과시간  
/*
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
var Uint16 MidFTimer42;		//綏째珦晥? 폴트 두번째 경과시간  

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
*/
//=======================================================//



var Uint16	FaultCounter;
var Uint16	FaultCheckCounter;
var Uint16	FaultCounter_old;
var Uint16	MajorFault;
//---------------------------------------------
// GDU 폴트 관련 bit 정의
//---------------------------------------------
var Uint16	FGDF;
#define GDF1		0	// Leading Leg
#define GDF2		1	// dcdc-Lagging Leg
#define GDF3		2	// 사용안함
#define GDF4		3	// 사용안함

//---------------------------------------------
// HW 폴트 관련 bit 정의
//---------------------------------------------
var Uint16	FHWF;

#define HWF0_ICOCF		0	//입력과전류
#define HWF1_MBATOCF	1	//모듈 충전전류 과전류
#define HWF2_MOCF		2	//모듈 출력전류 과전류
#define HWF3_SPAREF		3	//스페어

#define HWF4_BBAT1OCF	4	//BOX 충전전류1 과전류
#define HWF5_BBAT2OCF	5	//BOX 충전전류2 과전류
#define HWF6_BOCF		6	//BOX 출력전류 과전류
#define HWF7_CPOVF		7	//제어전압 과전압

#define HWF8_OVOVF		8	//출력전압 과전압(OBD전)
#define HWF9_BVOVF		9	//출력전압 과전압(OBD후)
#define HWF10_IVOVF		10	//입력전압 과전압
#define HWF11_FCOVF		11	//DCLINK전압 과전압


//---------------------------------------------
// S/W 폴트 관련 변수 및 bit 정의
//---------------------------------------------
var Uint16 	FSWF;   
#define SWF0_ICOCF		0	//입력과전류
#define SWF1_MBATOCF	1	//모듈 충전전류 과전류
#define SWF2_MOCF		2	//모듈 출력전류 과전류
#define SWF3_SPAREF		3	//스페어

#define SWF4_BBAT1OCF	4	//BOX 충전전류1 과전류
#define SWF5_BBAT2OCF	5	//BOX 충전전류2 과전류
#define SWF6_BOCF		6	//BOX 출력전류 과전류
#define SWF7_CPOVF		7	//제어전압 과전압

#define SWF8_OVOVF		8	//출력전압 과전압(OBD전)
#define SWF9_BVOVF		9	//출력전압 과전압(OBD후)
#define SWF10_IVOVF		10	//입력전압 과전압
#define SWF11_FCOVF		11	//DCLINK전압 과전압

#define SWF12_OVUVF		12	//출력전압 저전압(OBD전,후 공용) 50V
#define SWF13_IVUVF		13	//입력전압 저전압 620V
#define SWF14_CPUVF		14	//제어전압 저전압 50V
#define SWF15_OVUVF2	15	//출력전압 저전압(OBD후만 ) 50V. 20150922 출력전압 저전압(최종단)을 모니터링으로 표시하게

var Uint16	FSQF;// Init = 0
#define SEQF_CHKa		0
#define SEQF_LKa		1
#define SEQERR_DCUV		3

#define SEQERR_OCVUV	4
#define SEQERR_BIT5		5
#define SEQERR_BIT6		6
#define SEQERR_BIT7		7

var Uint16	FETF;
#define ETCF_MJF		0
#define ETCF_FUSEF		1
#define ETCF_PSF		2
#define ETCF_FANF1		3

#define ETCF_FANF2		4
#define ETCF_HSKTHRF	5
#define	ETCF_NCARGING	6
//#define ETCF_BATOC1		6
//#define ETCF_BATOC2		7


var Uint16	FBXF;
#define BOXF_BMJF		0
#define BOXF_BATOC1		1
#define BOXF_BATOC2		2

var Uint16	FLGDF;
var Uint16	FLHWF;
var Uint16	FLSWF;
var Uint16	FLSQF;
var Uint16	FLETF;
var Uint16	FLBXF;


var Uint16 	OverLoadAck;	// 과부하면 1
var Uint16 	OverLoadFAck;	// 과부하(OverLoadAck==1)상태가 1분이상 지속시 1
var Uint32 	OverLoad_Counter;
var Uint32 	THRFAck_Counter;	// 히트싱크 95도이상 1분 카운터
var Uint32 	FANF1Ack_Counter;	// 팬폴트1 카운터 10초
var Uint32 	FANF2Ack_Counter;	// 팬폴트2 카운터 10초

var Uint32 	PSF_Counter;	// PSF 발생하면 곧바로 폴트발생안시키고,,, 일정시간 지연 ==> 제어전원저전압에서 컨텍터 동작시 폴트나는것 방지
                            // 시험결과 약 100msec인데...여유로 150msec, 시간은 폴트동작인지딜레이시간동안 전압떨어져서 컨텍터 동작안하면 됨.
                            
//========CAN 관련변수

var unsigned int CANR_HB;
var unsigned int CANR_ID;
var unsigned int CANR_BCN; 
          
var unsigned int cantxidx;
var unsigned int canrxidx;
var long CanA_TX_CRC;
var long CanA_RX_CRC;
var unsigned int CanB_TX_CRC;
var unsigned int CanB_RX_CRC;

var long CanA_TX_L0;
var long CanA_TX_L1;
var long CanA_TX_L2;
var long CanA_TX_L3;
var long CanA_TX_L4;
var long CanA_TX_L5;

var long CanA_TX_H0;
var long CanA_TX_H1;
var long CanA_TX_H2;
var long CanA_TX_H3;
var long CanA_TX_H4;
var long CanA_TX_H5;

var long CanB_TX_L1;
var long CanB_TX_L2;
var long CanB_TX_L3;
var long CanB_TX_L4;
var long CanB_TX_H1;
var long CanB_TX_H2;
var long CanB_TX_H3;
var long CanB_TX_H4;

var long CanA_RX_L1;
var long CanA_RX_L2;
var long CanA_RX_L3;
var long CanA_RX_L4;
var long CanA_RX_H1;
var long CanA_RX_H2;
var long CanA_RX_H3;
var long CanA_RX_H4;

var long CanB_RX_L1;
var long CanB_RX_L2;
var long CanB_RX_L3;
var long CanB_RX_L4;
var long CanB_RX_H1;
var long CanB_RX_H2;
var long CanB_RX_H3;
var long CanB_RX_H4;

var unsigned int  CaTX_a1;
var unsigned int  CaTX_a2;
var unsigned int  CaTX_a3;
var unsigned int  CaTX_a4;
var unsigned int  CaTX_a5;
var unsigned int  CaTX_a6;
var unsigned int  CaTX_a7;
var unsigned int  CaTX_a8;
var unsigned int  CaTX_a9;
var unsigned int  CaTX_a10;
var unsigned int  CaTX_a11;
var unsigned int  CaTX_a12;
var unsigned int  CaTX_a13;
var unsigned int  CaTX_a14;
var unsigned int  CaTX_a15;
var unsigned int  CaTX_a16;
var unsigned int  CaTX_a17;
var unsigned int  CaTX_a18;
var unsigned int  CaTX_a19;
var unsigned int  CaTX_a20;
var unsigned int  CaTX_a21;
var unsigned int  CaTX_a23;
var unsigned int  CaTX_a24;
var unsigned int  CaTX_a25;
var unsigned int  CaTX_a26;


var unsigned int BCM_HartBeat;
var unsigned int BCBox_Status;
var unsigned int BCM_Status1;
var unsigned int BCM_Status2;
var unsigned int BCM_Status3;
var unsigned int BCM_Status4;

var unsigned int BCM_TDF1;
var unsigned int BCM_TDF2;
var unsigned int BCM_TDF3;
var unsigned int BCM_TDF4;  
             
var unsigned int BCM_SDF1;
var unsigned int BCM_SDF2;
var unsigned int BCM_SDF3;
var unsigned int BCM_SDF4;               
                
                
// eCAN-A/B 처리를 위한 변수 ------------------------------------------------
var BOOL WGcana_rx_flag;	// 사용자가 CAN-A 수신받은 데이타 저장한후(0), 아직 저장아님(1)
var LONG WGcana_rx_id;			// CAN-A 수신 ID
var BYTE WGcana_rx_length;    	// CAN-A 수신 데이터 크기
var LONG WGcana_rx_low_data;	// CAN-A 수신 Low Word Data
var LONG WGcana_rx_high_data;	// CAN-A 수신 High Word Data

var BOOL WGcanb_rx_flag;	// 사용자가 CAN-A 수신받은 데이타 저장한후(0), 아? 저장아님(1)
var LONG WGcanb_rx_id;			// CAN-A 수신 ID
var BYTE WGcanb_rx_length;    	// CAN-A 수신 데이터 크기
var LONG WGcanb_rx_low_data;	// CAN-A 수신 Low Word Data
var LONG WGcanb_rx_high_data;	// CAN-A 수신 High Word Data



//============================================================================
// Function Prototype Declaration
//============================================================================	
// WgBc_Main.c
void System_Define();
void Setting_Chk();
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

void BoardTest(unsigned char boardname);
void siv_test_init();

#endif // __NTHS_MAIN__  




