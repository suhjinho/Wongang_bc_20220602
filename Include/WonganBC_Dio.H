
#ifndef __EXTERNAL_INPUT_OUTPUT_H
#define __EXTERNAL_INPUT_OUTPUT_H


//====NTHS APS/BACH DIO, LED BIT DEFINE=====//

//------------------------------------------------------//
// EXTERNAL INPUT  FROM 차량 및 장비내부				//
//------------------------------------------------------//
	//--APS/BACH --DIN_CS1(),DINEN1(U2 )
	
var Uint16 	DioInPutBuf;
var Uint16 	DioInPutBuf1;
var Uint16 	DioInPutBuf1_1st;
var Uint16 	DioInPutBuf1_2nd;
var Uint16 	DioInPutBuf1_3rd;


var Uint16 	FUSEFAck;
var Uint16 	CHKAck;
var Uint16 	LKAck;
var Uint16 	MODEAck;
#define TESTMODE	0
#define NORMALMODE	1
var Uint16 	THRFAck;

//--500KM_DCDC50KW -- 20140611확인 -- DIN_CS1(),DINEN1(U2 )
#define CHK_Chk			0	//CHK ON
#define LK_Chk			1	//LK ON
#define DIN2_Chk		2	//사용안함
#define FUSE_Chk		3	//FUSE FAULT(b접점)

#define MODE_Chk		4	//NOMAL(H)/TEST(L) MODE
#define THRF1_Chk		5	//방열판 온도1
#define DIN6_Chk		6	//사용안함
#define DIN7_Chk		7	//사용안함

	//----500KM_DCDC50KW -- 20140611확인 -- DIN_CS2(),DINEN2(U4 )
#define MCBRUN_Chk		8	//DCDC RUN//MCB ON 지령, From 차량
#define RESET_Chk		9	//RESET//SIV 고장시 차량에서 RESET 가능, 차량에서 결정하면 됨
#define DIN10_Chk			10	//사용안함
#define DIN11_Chk		11	//사용안함

#define THRF2_Chk		12	//방열판 온도2, Normal Close
#define DIN13_Chk			14	//사용안함
#define DIN14_Chk			14	//사용안함
#define DIN15_Chk		15	//사용안함


	//--APS/BACH --DIN_CS2(),DINEN2(U4 )
var Uint16 	DioInPutBuf2;
var Uint16 	DioInPutBuf2_1st;
var Uint16 	DioInPutBuf2_2nd;
var Uint16 	DioInPutBuf2Ack;
//----------DIN_CS2()
var Uint16 	MCBRUNAck;
var Uint16 	RESETAck;

//------------------------------------------------------//
// EXTERNAL OUTPUT  TO 차량 및 장비내부					//
//------------------------------------------------------//
//----500KM_DCDC50KW -- 20140611확인 -- --DioOutBuf1,DOUT_CS1(),DOUTEN1(U33 )
var Uint16 	DioOutBuf1;
var Uint16 	DioOutBuf2;	// 사용안함
#define CHK_MC			0	//CHK ON,차징콘텍터 동작시킴
#define LK_MC			1	//LK ON,라인콘텍터 동작시킴
#define MJF				3	//MJF,장비이상임을 차량에 알려줌
#define BOXOK			4	//DCDC OK// BOX_OK(H),정상동작임을 차량에 알려줌
#define FANK_MC			5	//DCDC FANK


//var Uint16 	DioOutBuf2;	// 사용안함

//------------------------------------------------------//
// DIO CARD LED											//
//------------------------------------------------------//
	//-- --DIO_LED_CS1(),EPLD(U24), (U26 ) - LED 1-8
	//-- --DIO_LED_CS2(),EPLD(U24), (U25 ) - LED 9-12
//--500KM_DCDC50KW -- 20140611확인 -- 전면판 LED수정
var Uint16 	DioLedBuf01_16;	// 주로 상태표시 // DioLedBuf1+DioLedBuf2
#define DLED_TSTOK		0	//장비동작전 점검 정상동작중
#define DLED_MODE		1	//NOMAL(ON)/TEST(OFF) MODE
#define DLED_RUN		2	//RUN 지령오면 ON
#define DLED_ACIVOK		3	//AC가선전압(380V임,입력전압)
#define DLED_CHKA		4	//
#define DLED_CHKX		5	//추가
#define DLED_LKA		6	//
#define DLED_LKX		7	//
#define DLED_OUTV		8	//출력전압 정상
#define DLED_FCOK		9	//DC LINK전압
#define DLED_DCDCPWM	10	//DCDC PWM ON
#define DLED_BOOSTPWM	11	//BOOST PWM ON

#define DLED_GDF2		12	//
#define DLED_GDF1		13	//
#define DLED_GDF4		14	//
#define DLED_GDF3		15	//

//===================================
var unsigned int	DioLedBuf17_26;	// 주로 ERROR 표시 // DioLedBuf3+DioLedBuf4
#define DLED_THRF		0	//
#define DLED_GDF5		1	//
#define DLED_FUSEF		2	//
#define DLED_MJF		3	//
#define DLED_LD2		4	//
#define DLED_FANF		5	//
#define DLED_LD4		6	//
#define DLED_LD3		7	//
#define DLED_LD6		8	//
#define DLED_LD5		9	//

/*
#define DLED_SPR1		2	////RESETAck
#define DLED_SPR3		4	//
#define DLED_SPR2		5	//
#define DLED_SPR5		6	//
#define DLED_SPR4		7	//
#define DLED_SPR7		8	//
#define DLED_SPR6		9	//

*/


//------------------------------------------------------//
// SEN CARD LED											//
//------------------------------------------------------//
	//--APS/BACH --SEN1_LED_CS1(), (U22 )
	//--APS/BACH --SEN1_LED_CS2(), (U23 )
var Uint16 	SenLedBuf;
#define SLED_CT2_OCF		0	//IGBT 출력전류 과전류폴트
#define SLED_IC_OCF			1	//입력 과전류 폴트
#define SLED_SPR			2	//IGBT출력과전류--명판에는 SPR로 표시됨
#define SLED_OCOCF			3	//출력 과전류폴트
#define SLED_OVLD			4	//오버로드
#define SLED_PSF			5	//제어전원 폴트(L)
#define SLED_IV_LV		6	//입력저전압
#define SLED_IV_OV		7	//입력과전압
#define SLED_FCV_LV			8	//DCLINK저전압
#define SLED_FCV_OV			9	//DCLINK과전압
#define SLED_OV_LV			10	//출력저전압
#define SLED_OV_OV			11	//출력과전압
#define SLED_LD1			13	//SPARE
#define SLED_LD2			12	//SPARE
#define SLED_LD3			15	//SPARE
#define SLED_LD4			14	//SPARE

	//--APS/BACH --SEN1_VOL_CHK_CS(), (EPLD,U13 )
var Uint16 	SenInPutBuf;
var Uint16 	SenInPutBuf_1st;
var Uint16 	SenInPutBuf_2nd;
var Uint16 	PSFAck;
#define PS24V_Chk				0	//제어전원 /24V_OK, ACTIVE(L)
#define PS5V_Chk				1	//제어전원 /5V_OK, ACTIVE(L)
#define PS15V_Chk				2	//제어전원 /15V_OK, ACTIVE(L)
#define PS100V_Chk				3	//제어전원 /100V_OK, ACTIVE(L)


//APS_DIO_Process
void DioSenCard_OutputInit();//nths꺼..aps/bach공용
void DIO_InputChk();
void DIO_OutPutProcess();

#endif

