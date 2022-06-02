
#ifndef __EXTERNAL_INPUT_OUTPUT_H
#define __EXTERNAL_INPUT_OUTPUT_H







//------------------------------------------------------//
// EXTERNAL INPUT  FROM 차량 및 장비내부				//
//------------------------------------------------------//
var Uint16 	DioInPutBuf;
var Uint16 	DioInPutBuf1;
var Uint16 	DioInPutBuf1_1st;
var Uint16 	DioInPutBuf1_2nd;
var Uint16 	DioInPutBuf1_3rd;
//--DioInPutBuf ---- CS_RD_DIN1(), CS_RD_DIN2()
#define EXTRESET_Chk		0	//BOX 외부(차량) 리셋지령 입력 <---릴레이보드
#define BATOC1_Chk			1	//BOX 외부(차량) 배터리1 과전류(CB1)폴트 입력 <---릴레이보드
#define BATOC2_Chk			2	//BOX 외부(차량) 배터리2 과전류(CB2)폴트 입력 <---릴레이보드
#define BCSTART_Chk			3	//BOX 외부(차량) BC START지령 입력 <---입력필터보드

#define M1F_Chk				4	//BC MODULE1 고장 입력 <---릴레이보드
#define M2F_Chk				5	//BC MODULE2 고장 입력 <---릴레이보드
#define M3F_Chk				6	//BC MODULE3 고장 입력 <---릴레이보드
#define M4F_Chk				7	//BC MODULE4 고장 입력 <---릴레이보드

#define M5F_Chk				8	//BC MODULE5 고장 입력 <---릴레이보드
#define BCMODE_Chk			9	//MODE?? <---릴레이보드, spare2
#define CHK_Chk				10	//CHK접점동작 입력 <---입력필터보드
#define LK_Chk				11	//LK접점동작 입력 <---입력필터보드

#define BC72V_Chk			12	//72V출력정상 입력 <---입력필터보드???
#define TEMP85SW_Chk		13	//Temperature switch(85도) FAULT(b접점) <---방열판 온도스위치
#define FANF1_Chk			14	//팬폴트1 입력 <---팬파워 서플라이
#define FANF2_Chk			15	//팬폴트2 입력 <---팬파워 서플라이

//------------------------------------------------------//
// MCU보드 내부 입출력									//
//------------------------------------------------------//
var Uint16 MCUBD_MODESW_IN;	// TEST/NORMAL MODE 선택스위치, 전면판 위치
var Uint16 MCUBD_HEXSW_IN;	// HEX ROTARY SWITCH, 전면판 위치 <-- ID로 사용하면 될듯..
var Uint16 MCUBD_DIPSW_IN;   // DIP SWITCH 2포트, 
var Uint16 MCUBD_PUSHBUTTONSW_IN; // PUSH BUTTON SWITCH
var Uint16 MCUBD_PUSHBUTTONSW_IN_OLD; // PUSH BUTTON SWITCH

//------------------------------------------------------//
// EXTERNAL OUTPUT  TO 차량 및 장비내부					//
//------------------------------------------------------//
//--DioOutBuf1 ---- CS_WR_DOUT1()
var Uint16 	DioOutBuf1;
#define BC_FAULT_OUT			0	//모듈 폴트상태 출력 ---> 릴레이보드
#define BC_STATE_OUT			1	//모듈 상태 출력 ---> 릴레이보드
#define BC_SPARE1_OUT			2	//모듈 스페어 출력 ---> 릴레이보드
#define BC_STATE2_OUT			3	//모듈 스페어 출력 ---> mcu보드 내부 커넥터(j18)
#define BC_CHKON_OUT			4	//모듈 CHK동작출력 ---> 입력필터보드
#define BC_LKON_OUT				5	//모듈 LK동작출력 ---> 입력필터보드
#define BC_SPARE72V_OUT			6	//모듈 스페어출력 ---> 입력필터보드
#define BC_FANON_OUT			7	//팬동작 출력 ---> 팬파워 서플라이

//--Epld_CtrlBuf ---- CS_WR_CTRL()
var Uint16 	Epld_CtrlBuf;
#define ECTRL_SWFGEN		0	//강제로 폴트 발생(래치)
#define ECTRL_TMP			1	//모듈 상태 출력 ---> 릴레이보드
#define ECTRL_DIOS1			2	// ---> 
#define ECTRL_DIOS3			3	// ---> 
#define ECTRL_DIOS4			4	// ---> 
#define ECTRL_DIOS5			5	// ---> 
#define ECTRL_DIOS6			6	// ---> 
#define ECTRL_DIOS7			7	// ---> 

//--Epld_EledBuf ---- CS_WR_ELED()
var Uint16 	Epld_EledBuf;
#define EELED_LED1			0	//ELED1(LD8)
#define EELED_LED2			1	//ELED2(LD7)
#define EELED_LED3			2	//ELED3(LD6)
#define EELED_LED4			3	//ELED4(LD5)

#define EELED_TP1			4	//TSTPIN(TP1)
#define EELED_TP2			5	//TSTPIN(TP2)
#define EELED_TP3			6	//TSTPIN(TP3)
#define EELED_TP4			7	//TSTPIN(TP4)

//------------------------------------------------------//
// LED											//
//------------------------------------------------------//
//#define CS_WR_LED1()		PWORD(0x4100)
//#define CS_WR_LED2()		PWORD(0x4101)
//#define CS_WR_LED3()		PWORD(0x4102)
var Uint16 	DioLedBuf1;	// 
#define DLED1_BIT0		0	//GDF1
#define DLED1_BIT1		1	//GDF2
#define DLED1_BIT2		2	//입력과전류
#define DLED1_BIT3		3	////모듈 충전전류 과전류
#define DLED1_BIT4		4	////모듈 출력전류 과전류
#define DLED1_BIT5		5	////BOX 충전전류1 과전류
#define DLED1_BIT6		6	////BOX 충전전류2 과전류
#define DLED1_BIT7		7	////BOX 출력전류 과전류
var Uint16 	DioLedBuf2;	// 
#define DLED2_BIT0		0	//START신호입력 확인(LED --> L13)
#define DLED2_BIT1		1	//
#define DLED2_BIT2		2	//
#define DLED2_BIT3		3	//
#define DLED2_BIT4		4	//
#define DLED2_BIT5		5	//
#define DLED2_BIT6		6	//
#define DLED2_BIT7		7	//
var Uint16 	DioLedBuf3;	// 
#define DLED3_BIT0		0	//
#define DLED3_BIT1		1	//출력전압 과전압(OBD전,후)
#define DLED3_BIT2		2	//입력과전압
#define DLED3_BIT3		3	//DCLINK과전압
#define DLED3_BIT4		4	//
#define DLED3_BIT5		5	//
#define DLED3_BIT6		6	//
#define DLED3_BIT7		7	//

//
void DOutputInit();//
void Check_DI();
void DOutPut();
//---------------------------------------		
void ELED_LD8_ON();
void ELED_LD8_OFF();
void ELED_LD8_TOGGLE();
void ELED_LD7_ON();
void ELED_LD7_OFF();
void ELED_LD7_TOGGLE();
void ELED_LD6_ON();
void ELED_LD6_OFF();
void ELED_LD6_TOGGLE();
void ELED_LD5_ON();
void ELED_LD5_OFF();
void ELED_LD5_TOGGLE();
void ELED_TP1_HI();
void ELED_TP1_LOW();
void ELED_TP1_TOGGLE();
void ELED_TP2_HI();
void ELED_TP2_LOW();
void ELED_TP2_TOGGLE();
void ELED_TP3_HI();
void ELED_TP3_LOW();
void ELED_TP3_TOGGLE();
void ELED_TP4_HI();
void ELED_TP4_LOW();
void ELED_TP4_TOGGLE();

#endif

