//==================================================================================//
// ¿ø°­¼± µ¿·ÂÂ÷(30kw)/°´Â÷(50kw) ÃàÀüÁö ÃæÀü±â ¸ğµâ(10kw) ÇÁ·Î±×·¥	           		//
//==================================================================================//
// main header file														//
// DATE : 2014. 11. 21            ÃÖÃÊÀÛ¼º				  by PACTECH R&D LSH		//
//==================================================================================//


//==================================================================================//
#ifndef __NTHS_MAIN__   
#define __NTHS_MAIN__   


//============================================
// º¯¼ö¼±¾ğ
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
var Uint16 ProgramMode_B0M1C2;	//20150404Ãß°¡

//==½Ã½ºÅÛ Á¤°İ°ü·Ã===

var Uint16 ProgramVersion;// 
	
//--ÃæÀü±â Á¾·ù ¼³Á¤	
#define WGBC_TC0	0
#define WGBC_PC1	1
var  unsigned int WGBC_TC0PC1;

//--10KW ¸ğµâ Number(ID)¼³Á¤
//--¹Ú½ºÀü¸éÄ¿¹ö ±âÁØ ¿ŞÂÊºÎÅÍ 1..
//--µ¿·ÂÂ÷BC´Â ¿ŞÂÊºÎÅÍ 1,2,3   °´Â÷BC´Â ¿ŞÂÊºÎÅÍ 1,2,3,4,5
#define WGBC_M_ID1		1
#define WGBC_M_ID2		2
#define WGBC_M_ID3		3
#define WGBC_M_ID4		4
#define WGBC_M_ID5		5
//var  int WGBC_M_ID;
var  long WGBC_M_ID;

//--------------------------------------------------------------------
//¸ğµâÀÔ·ÂÀü¾Ğ --> ADCRD10
var float 	MInputVol;
var float 	MInputVolLPF;		
var float 	MInputVolRATED;			//½Ã½ºÅÛÁ¤°İ(670[V])
var float	MInputVolMAX;			//½Ã½ºÅÛÁ¤°İ(720[V])
var float	MInputVolMIN;			//½Ã½ºÅÛÁ¤°İ(620[V])
var float	MInputVolSWOVF;
var float	MInputVolHWOVF;
var float	MInputVolUVF;
//¸ğµâ DCLINKÀü¾Ğ(RATED 670Vdc) --> ADCRD11
var float 	MDcLinkVol;	
var float 	MDcLinkVolLPF;
var float	MDclinkVolRATED;		
var float	MDclinkVolMAX;
var float	MDclinkVolMIN;
var float	MDclinkVolSWOVF;	
var float	MDclinkVolHWOVF;	
var float	MDclinkVolUVF;
var float	TSTVAR11;
//¸ğµâÃâ·ÂÀü¾Ğ1(OBDÈÄ´Ü) Á¤°İ 720Vdc(72~83) --> ADCRD9
var float 	MBatVol;
var float 	MBatVolLPF;			
var float	MBatVolRATED;
var float	MBatVolMAX;
var float	MBatVolMIN;
var float	MBatVolSWOVF;
var float	MBatVolHWOVF;
var float	MBatVolUVF;
//¸ğµâÃâ·ÂÀü¾Ğ1(OBDÀü´Ü) Á¤°İ 720Vdc(72~83) --> ADCRD8
var float 	MOutVol;
var float 	MOutVolLPF;			
var float	MOutVolRATED;
var float	MOutVolMAX;
var float	MOutVolMIN;
var float	MOutVolSWOVF;
var float	MOutVolHWOVF;
var float	MOutVolUVF;
//¸ğµâ Á¦¾îÀü¾Ğ Á¤°İ 72Vdc(72~83) --> ADCRD7
var float 	MCtrlVol;
var float 	MCtrlVolLPF;			
var float	MCtrlVolRATED;
var float	MCtrlVolMAX;
var float	MCtrlVolMIN;
var float	MCtrlVolSWOVF;
var float	MCtrlVolHWOVF;
var float	MCtrlVolUVF;	
//¸ğµâ ÀÔ·ÂÀü·ù --> ADCRD0
var float 	MInputCur;
var float 	MInputCurLPF;
var float	MInputCurRATED;
var float	MInputCurMAX;
var float	MInputCurSWOCF;
var float	MInputCurHWOCF;	
//box Ãâ·ÂÀü·ù --> ADCRD6
var float 	BOutCur;
var float 	BOutCurLPF;	
var float	BOutCurRATED;
var float	BOutCurMAX;
var float	BOutCurHWOCF;
var float 	BOutCurSWOCF;
//¸ğµâ ÃæÀüÀü·ù --> ADCRD1
var float 	MBatCur;				
var float 	MBatCurLPF;
var float 	MBatCurRATED;
var float 	MBatCurMAX;
var float 	MBatCurHWOCF;
var float 	MBatCurSWOCF;
//¸ğµâ Ãâ·ÂÀü·ù --> ADCRD2
var float 	MOutCur;				
var float 	MOutCurLPF;
var float 	MOutCurRATED;
var float 	MOutCurMAX;
var float 	MOutCurHWOCF;
var float 	MOutCurSWOCF;
//box BATÀü·ù1 --> ADCRD4
var float 	BBatCur1;				
var float 	BBatCur1LPF;			
var float 	BBatCur1RATED;
var float 	BBatCur1MAX;
var float 	BBatCur1HWOCF;
var float 	BBatCur1SWOCF;
//box BATÀü·ù2 --> ADCRD5 (°´Â÷ BC´Â »ç¿ë¾ÈÇÔ)
var float 	BBatCur2;				
var float 	BBatCur2LPF;			
var float 	BBatCur2RATED;
var float 	BBatCur2MAX;
var float 	BBatCur2HWOCF;
var float 	BBatCur2SWOCF;
//box BATÀü·ù --> µ¿·ÂÂ÷ BC =BAT1+BAT2, °´Â÷ BC = BAT1
var float 	BBatCur;				
var float 	BBatCurLPF;			
var float 	BBatCurRATED;
var float 	BBatCurMAX;
var float 	BBatCurSWOCF;

//box ºÎÇÏÀü·ù --> boxÃâ·ÂÀü·ù-boxÃæÀüÀü·ù(BAT1+BAT2)
var float 	BLoadCur;				
var float 	BLoadCurLPF;			
var float 	BLoadCurRATED;
//module ºÎÇÏÀü·ù --> moduleÃâ·ÂÀü·ù-moduleÃæÀüÀü·ù
var float 	MLoadCur;				
var float 	MLoadCurLPF;			
var float 	MLoadCurRATED;


//=====Acknowledge °ü·Ã º¯¼ö

#define Mode_CV0	0
#define Mode_CC1	1
var Uint16 	Mode_CV0CC1;// CVCC ¸ğµå

var Uint16 	MODEAck;//dsp select switch, module mode ½ºÀ§Ä¡·Î »ç¿ë
var Uint16	MInputVolAck;		// ÀÔ·ÂÀü¾Ğ°ËÁö
var Uint16 	FaultAck;
var Uint16 	FaultAckOld;
var Uint16 	ModuleFault_NUM;	// 10kW ¸ğµâ ÆúÅÍ¼ıÀÚ
var Uint16 	Pall_OK_ModuleN;	// 10kW ¸ğµâ ÆúÅÍ¼ıÀÚ

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
// ½ÃÄı½º °ü·Ãº¯¼ö
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
// ÆúÆ® °ü·Ã º¯¼ö
//---------------------------------------------
var Uint16 LowFaultAck;		// 1. °æ°íÀå 
var Uint16 MidFaultAck;		// 2. °æÁß°íÀå(3ºĞÀÌ³» µ¿ÀÏ°íÀå 3È¸½Ã Áß°íÀå Ã³¸®µÇ´Â °íÀå)
var Uint16 HighFaultAck;	// 3. Áß°íÀå 
//==MidFaultAck BIT Á¤ÀÇ====
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
//	#define MidFanf		9==>ÆÒÆúÆ® ÀÓ½Ã·Î Áß°£°íÀå, ¿ø·¡´Â Áß°íÀåÀÓ

/*
sbit(MidFaultAck, MidGDF);		// Áß°£·¹º§(3ºĞ3È¸) - GDF
sbit(MidFaultAck, MidINOVF);	// Áß°£·¹º§(3ºĞ3È¸) - ÀÔ·Â°úÀü¾Ğ
sbit(MidFaultAck, MidMICOCF);	// Áß°£·¹º§(3ºĞ3È¸) - ÀÔ·Â°úÀü·ù
sbit(MidFaultAck, MidOVOVF);	// Áß°£·¹º§(3ºĞ3È¸) - Ãâ·Â °úÀü¾Ğ
sbit(MidFaultAck, MidMOCOCF);	// Áß°£·¹º§(3ºĞ3È¸) - Ãâ·Â °úÀü·ù ÆúÆ®
sbit(MidFaultAck, MidSEQF);	// Áß°£·¹º§(3ºĞ3È¸) - ½ÃÄı½ºÆúÆ®
sbit(MidFaultAck, MidFCOVF);	// Áß°£·¹º§(3ºĞ3È¸) - FCV(DCLINK VOLTAGE) °úÀü¾Ğ ÆúÆ®
sbit(MidFaultAck, MidFCOCF);	// Áß°£·¹º§(3ºĞ3È¸) - FCI(DCLINK CURRENT) °úÀü·ù ÆúÆ®
sbit(MidFaultAck, MidPSF);		// Áß°£·¹º§(3ºĞ3È¸) - Á¦¾îÀü¿ø ÀúÀü¾Ğ ÆúÆ®	
*/		
var Uint16 MidFaultMJF;		// 2. °æÁß°íÀå(3ºĞÀÌ» µ¿ÀÏ°íÀå 3È¸½Ã Áß°íÀå Ã³¸®µÇ´Â °íÀå) 
var Uint16 HighFaultMJF;	// 3. Áß°íÀå 
//====MidFaultAck°ü·Ã ÆúÆ®º¯¼ö
// GDF°ü·Ã º¯¼ö
var Uint16 MidFCounter0;	//GDF1,2 ¹ß»ıÈ½¼ö 
var Uint16 MidFTimer01;		//GDFs Ã¹¹øÂ° °æ°ú½Ã°£  
var Uint16 MidFTimer02;		//GDFs µÎ¹øÂ° °æ°ú½Ã°£  

var Uint16 MidFCounter1;	//ÀÔ·Â°úÀü·ù ÆúÆ® ¹ß»ıÈ½¼ö
var Uint16 MidFTimer11;		//ÀÔ·Â°úÀü·ù ÆúÆ® Ã¹¹øÂ° °æ°ú½Ã°£ 
var Uint16 MidFTimer12;		//ÀÔ·Â°úÀü·ù ÆúÆ® µÎ¹øÂ° °æ°ú½Ã°£  

var Uint16 MidFCounter2;	//¸ğµâÃâ·Â°úÀü·ù ÆúÆ® ¹ß»ıÈ½¼ö
var Uint16 MidFTimer21;		//¸ğµâÃâ·Â°úÀü·ù ÆúÆ® Ã¹¹øÂ° °æ°ú½Ã°£  
var Uint16 MidFTimer22;		//¸ğµâÃâ·Â°úÀü·ù ÆúÆ® µÎ¹øÂ° °æ°ú½Ã°£  
/*
var Uint16 MidFCounter1;	//ÀÔ·Â°úÀü¾Ğ ÆúÆ® ¹ß»ıÈ½¼ö
var Uint16 MidFTimer11;		//ÀÔ·Â°úÀü¾Ğ ÆúÆ® Ã¹¹øÂ° °æ°ú½Ã°£ 
var Uint16 MidFTimer12;		//ÀÔ·Â°úÀü¾Ğ ÆúÆ® µÎ¹øÂ° °æ°ú½Ã°£  

var Uint16 MidFCounter2;	//ÀÔ·Â°úÀü·ù ÆúÆ® ¹ß»ıÈ½¼ö
var Uint16 MidFTimer21;		//ÀÔ·Â°úÀü·ù ÆúÆ® Ã¹¹øÂ° °æ°ú½Ã°£  
var Uint16 MidFTimer22;		//ÀÔ·Â°úÀü·ù ÆúÆ® µÎ¹øÂ° °æ°ú½Ã°£  

var Uint16 MidFCounter3;	//Ãâ·Â°úÀü¾Ğ ÆúÆ® ¹ß»ıÈ½¼ö
var Uint16 MidFTimer31;		//Ãâ·Â°úÀü¾Ğ ÆúÆ® Ã¹¹øÂ° °æ°ú½Ã°£ 
var Uint16 MidFTimer32;		//Ãâ·Â°úÀü¾Ğ ÆúÆ® µÎ¹øÂ° °æ°ú½Ã°£  

var Uint16 MidFCounter4;	//Ãâ·Â°úÀü·ù ÆúÆ® ¹ß»ıÈ½¼ö
var Uint16 MidFTimer41;		//Ãâ·Â°úÀü·ù ÆúÆ® Ã¹¹øÂ° °æ°ú½Ã°£ 
var Uint16 MidFTimer42;		//â·Â°úÀü·ù ÆúÆ® µÎ¹øÂ° °æ°ú½Ã°£  

var Uint16 MidFCounter5;	//°úºÎÇÏ ÆúÆ® ¹ß»ıÈ½¼ö
var Uint16 MidFTimer51;		//°úºÎÇÏ ÆúÆ® Ã¹¹øÂ° °æ°ú½Ã°£ 
var Uint16 MidFTimer52;		//°úºÎÇÏ ÆúÆ® µÎ¹øÂ° °æ°ú½Ã°£    

var Uint16 MidFCounter6;	//FCV(DCLINK VOLTAGE) °úÀü¾Ğ ÆúÆ® ¹ß»ıÈ½¼ö
var Uint16 MidFTimer61;		//FCV(DCLINK VOLTAGE) °úÀü¾Ğ ÆúÆ® Ã¹¹øÂ° °æ°ú½Ã°£ 
var Uint16 MidFTimer62;		//FCV(DCLINK VOLTAGE) °úÀü¾Ğ ÆúÆ® µÎ¹øÂ° °æ°ú½Ã°£  

var Uint16 MidFCounter7;	//FCI(DCLINK CURRENT) °úÀü·ù ÆúÆ® ¹ß»ıÈ½¼ö
var Uint16 MidFTimer71;		//FCI(DCLINK CURRENT) °úÀü·ù ÆúÆ® Ã¹¹øÂ° °æ°ú½Ã°£ 
var Uint16 MidFTimer72;		//FCI(DCLINK CURRENT) °úÀü·ù ÆúÆ® µÎ¹øÂ° °æ°ú½Ã°£ 

var Uint16 MidFCounter8;	//Á¦¾îÀü¿ø ÀúÀü¾Ğ ÆúÆ® ¹ß»ıÈ½¼ö
var Uint16 MidFTimer81;		//Á¦¾îÀü¿ø ÀúÀü¾Ğ ÆúÆ® Ã¹¹øÂ° °æ°ú½Ã°£ 
var Uint16 MidFTimer82;		//Á¦¾îÀü¿ø ÀúÀü¾Ğ ÆúÆ® µÎ¹øÂ° °æ°ú½Ã°£  

var Uint16 MidFCounter9;	//Á¦¾îÀü¿ø ÀúÀü¾Ğ ÆúÆ® ¹ß»ıÈ½¼ö
var Uint16 MidFTimer91;		//Á¦¾îÀü¿ø ÀúÀü¾Ğ ÆúÆ® Ã¹¹øÂ° °æ°ú½Ã°£ 
var Uint16 MidFTimer92;		//Á¦¾îÀü¿ø ÀúÀü¾Ğ ÆúÆ® µÎ¹øÂ° °æ°ú½Ã°£  
*/
//=======================================================//



var Uint16	FaultCounter;
var Uint16	FaultCheckCounter;
var Uint16	FaultCounter_old;
var Uint16	MajorFault;
//---------------------------------------------
// GDU ÆúÆ® °ü·Ã bit Á¤ÀÇ
//---------------------------------------------
var Uint16	FGDF;
#define GDF1		0	// Leading Leg
#define GDF2		1	// dcdc-Lagging Leg
#define GDF3		2	// »ç¿ë¾ÈÇÔ
#define GDF4		3	// »ç¿ë¾ÈÇÔ

//---------------------------------------------
// HW ÆúÆ® °ü·Ã bit Á¤ÀÇ
//---------------------------------------------
var Uint16	FHWF;

#define HWF0_ICOCF		0	//ÀÔ·Â°úÀü·ù
#define HWF1_MBATOCF	1	//¸ğµâ ÃæÀüÀü·ù °úÀü·ù
#define HWF2_MOCF		2	//¸ğµâ Ãâ·ÂÀü·ù °úÀü·ù
#define HWF3_SPAREF		3	//½ºÆä¾î

#define HWF4_BBAT1OCF	4	//BOX ÃæÀüÀü·ù1 °úÀü·ù
#define HWF5_BBAT2OCF	5	//BOX ÃæÀüÀü·ù2 °úÀü·ù
#define HWF6_BOCF		6	//BOX Ãâ·ÂÀü·ù °úÀü·ù
#define HWF7_CPOVF		7	//Á¦¾îÀü¾Ğ °úÀü¾Ğ

#define HWF8_OVOVF		8	//Ãâ·ÂÀü¾Ğ °úÀü¾Ğ(OBDÀü)
#define HWF9_BVOVF		9	//Ãâ·ÂÀü¾Ğ °úÀü¾Ğ(OBDÈÄ)
#define HWF10_IVOVF		10	//ÀÔ·ÂÀü¾Ğ °úÀü¾Ğ
#define HWF11_FCOVF		11	//DCLINKÀü¾Ğ °úÀü¾Ğ


//---------------------------------------------
// S/W ÆúÆ® °ü·Ã º¯¼ö ¹× bit Á¤ÀÇ
//---------------------------------------------
var Uint16 	FSWF;   
#define SWF0_ICOCF		0	//ÀÔ·Â°úÀü·ù
#define SWF1_MBATOCF	1	//¸ğµâ ÃæÀüÀü·ù °úÀü·ù
#define SWF2_MOCF		2	//¸ğµâ Ãâ·ÂÀü·ù °úÀü·ù
#define SWF3_SPAREF		3	//½ºÆä¾î

#define SWF4_BBAT1OCF	4	//BOX ÃæÀüÀü·ù1 °úÀü·ù
#define SWF5_BBAT2OCF	5	//BOX ÃæÀüÀü·ù2 °úÀü·ù
#define SWF6_BOCF		6	//BOX Ãâ·ÂÀü·ù °úÀü·ù
#define SWF7_CPOVF		7	//Á¦¾îÀü¾Ğ °úÀü¾Ğ

#define SWF8_OVOVF		8	//Ãâ·ÂÀü¾Ğ °úÀü¾Ğ(OBDÀü)
#define SWF9_BVOVF		9	//Ãâ·ÂÀü¾Ğ °úÀü¾Ğ(OBDÈÄ)
#define SWF10_IVOVF		10	//ÀÔ·ÂÀü¾Ğ °úÀü¾Ğ
#define SWF11_FCOVF		11	//DCLINKÀü¾Ğ °úÀü¾Ğ

#define SWF12_OVUVF		12	//Ãâ·ÂÀü¾Ğ ÀúÀü¾Ğ(OBDÀü,ÈÄ °ø¿ë) 50V
#define SWF13_IVUVF		13	//ÀÔ·ÂÀü¾Ğ ÀúÀü¾Ğ 620V
#define SWF14_CPUVF		14	//Á¦¾îÀü¾Ğ ÀúÀü¾Ğ 50V
#define SWF15_OVUVF2	15	//Ãâ·ÂÀü¾Ğ ÀúÀü¾Ğ(OBDÈÄ¸¸ ) 50V. 20150922 Ãâ·ÂÀü¾Ğ ÀúÀü¾Ğ(ÃÖÁ¾´Ü)À» ¸ğ´ÏÅÍ¸µÀ¸·Î Ç¥½ÃÇÏ°Ô

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


var Uint16 	OverLoadAck;	// °úºÎÇÏ¸é 1
var Uint16 	OverLoadFAck;	// °úºÎÇÏ(OverLoadAck==1)»óÅÂ°¡ 1ºĞÀÌ»ó Áö¼Ó½Ã 1
var Uint32 	OverLoad_Counter;
var Uint32 	THRFAck_Counter;	// È÷Æ®½ÌÅ© 95µµÀÌ»ó 1ºĞ Ä«¿îÅÍ
var Uint32 	FANF1Ack_Counter;	// ÆÒÆúÆ®1 Ä«¿îÅÍ 10ÃÊ
var Uint32 	FANF2Ack_Counter;	// ÆÒÆúÆ®2 Ä«¿îÅÍ 10ÃÊ

var Uint32 	PSF_Counter;	// PSF ¹ß»ıÇÏ¸é °ğ¹Ù·Î ÆúÆ®¹ß»ı¾È½ÃÅ°°í,,, ÀÏÁ¤½Ã°£ Áö¿¬ ==> Á¦¾îÀü¿øÀúÀü¾Ğ¿¡¼­ ÄÁÅØÅÍ µ¿ÀÛ½Ã ÆúÆ®³ª´Â°Í ¹æÁö
                            // ½ÃÇè°á°ú ¾à 100msecÀÎµ¥...¿©À¯·Î 150msec, ½Ã°£Àº ÆúÆ®µ¿ÀÛÀÎÁöµô·¹ÀÌ½Ã°£µ¿¾È Àü¾Ğ¶³¾îÁ®¼­ ÄÁÅØÅÍ µ¿ÀÛ¾ÈÇÏ¸é µÊ.
                            
//========CAN °ü·Ãº¯¼ö

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
                
                
// eCAN-A/B Ã³¸®¸¦ À§ÇÑ º¯¼ö ------------------------------------------------
var BOOL WGcana_rx_flag;	// »ç¿ëÀÚ°¡ CAN-A ¼ö½Å¹ŞÀº µ¥ÀÌÅ¸ ÀúÀåÇÑÈÄ(0), ¾ÆÁ÷ ÀúÀå¾Æ´Ô(1)
var LONG WGcana_rx_id;			// CAN-A ¼ö½Å ID
var BYTE WGcana_rx_length;    	// CAN-A ¼ö½Å µ¥ÀÌÅÍ Å©±â
var LONG WGcana_rx_low_data;	// CAN-A ¼ö½Å Low Word Data
var LONG WGcana_rx_high_data;	// CAN-A ¼ö½Å High Word Data

var BOOL WGcanb_rx_flag;	// »ç¿ëÀÚ°¡ CAN-A ¼ö½Å¹ŞÀº µ¥ÀÌÅ¸ ÀúÀåÇÑÈÄ(0), ¾Æ÷ ÀúÀå¾Æ´Ô(1)
var LONG WGcanb_rx_id;			// CAN-A ¼ö½Å ID
var BYTE WGcanb_rx_length;    	// CAN-A ¼ö½Å µ¥ÀÌÅÍ Å©±â
var LONG WGcanb_rx_low_data;	// CAN-A ¼ö½Å Low Word Data
var LONG WGcanb_rx_high_data;	// CAN-A ¼ö½Å High Word Data



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




