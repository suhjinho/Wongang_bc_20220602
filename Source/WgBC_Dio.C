//==================================================================================//
// 원강선 동력차(30kw)/객차(50kw) 축전지 충전기 모듈(10kw) 프로그램	           		//
//==================================================================================//
// DIO 부분  															       			
//==================================================================================//

#include "Global_Variable_Function.h"
//==================================================================================//
// 함수명 : DOutputInit()								
// 기  능 : DIO 출력/led 초기화																									
//==================================================================================//
Uint16 BCSTARTAck_Delay_Counter = 0;//start신호 인식을 0.3초 딜레이줌, 입력과전압 때문에

void DOutputInit()
{
/*
var Uint16 	DioOutBuf1;
#define BC_FAULT_OUT			0	//모듈 폴트상태 출력 ---> 릴레이보드
#define BC_STATE_OUT			1	//모듈 상태 출력 ---> 릴레이보드
#define BC_SPARE1_OUT			2	//모듈 스페어 출력 ---> 릴레이보드
#define BC_STATE2_OUT			3	//모듈 스페어 출력 ---> mcu보드 내부 커넥터(j18)
#define BC_CHKON_OUT			4	//모듈 CHK동작출력 ---> 입력필터보드
#define BC_LKON_OUT				5	//모듈 LK동작출력 ---> 입력필터보드
#define BC_SPARE72V_OUT			6	//모듈 스페어출력 ---> 입력필터보드
#define BC_FANON_OUT			7	//팬동작 출력 ---> 팬파워 서플라이
*/

	//DioOutBuf1 = 0x01;
	DioOutBuf1 = 0x02;
	//DioOutBuf1 = 0x00;

	DioLedBuf1 = 0x00;
	DioLedBuf2 = 0x00;
	DioLedBuf3 = 0x00;
	Epld_EledBuf = 0xff;// logic 반대, 1이면 0으로 출력(led4개, TP4개)
		
	CS_WR_DOUT1() = DioOutBuf1;// ALL DOUT OFF

	CS_WR_LED1() = (DioLedBuf1 & 0xFF);// ALL DLEDOUT OFF
	CS_WR_LED2() = (DioLedBuf2 & 0xFF);// ALL DLEDOUT OFF
	CS_WR_LED3() = (DioLedBuf3 & 0xFF);// ALL DLEDOUT OFF
	CS_WR_ELED() = (Epld_EledBuf & 0xFF);// ALL LED4개 OFF, TP 0으로 출력
}

//==================================================================================//
// 함수명 : _Check_DI()		            								
// 기  능 : DIN체크하고(채터링 3회) 관련 ACK신호 생성								
// Calling : 제어인터럽트에서 																				
//==================================================================================//
void Check_DI()
{
	unsigned int buf, buf1, buf2;
	
	Uint16 Temp0_GPIO = 0;
	Uint16 Temp1_GPIO = 0;
	Uint16 Temp2_GPIO = 0;
	Uint16 Temp3_GPIO = 0;		
	
	MODEAck = GpioDataRegs.GPADAT.bit.GPIO26;//SWSEL---> MODE SWITCH로 사용할까?(TEST MODE / NORMAL MODE)

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

	//==================================================
	// DIO 입력, 채터링 방지(3회)
	//==================================================
	buf1 = CS_RD_DIN1()& 0xFF;
	buf2 = CS_RD_DIN2()& 0xFF;
	buf  = ((buf2<<8)& 0xFF00)| buf1;

/*------*/
	if((buf == DioInPutBuf1_1st)&&(buf == DioInPutBuf1_2nd)&&(buf == DioInPutBuf1_3rd))
	{
		DioInPutBuf = buf;
	}

	DioInPutBuf1_3rd = DioInPutBuf1_2nd;
	DioInPutBuf1_2nd = DioInPutBuf1_1st;
	DioInPutBuf1_1st = buf;
/*------*/		

//--DioInPutBuf ---- CS_RD_DIN1(), CS_RD_DIN2()
	if(btst(DioInPutBuf,EXTRESET_Chk))  RESETAck = NO;
	else                                RESETAck = YES;

//===20160203 입력과전압 체크 로직변경(딜레이줌)
//===1편성 운행중 입력과전압 폴트발생, START전 입력전압 940V까지 상승하고 920V근처에서 START신호가 들어옴
//===기존 : 항상 입력과전압 폴트검지
//===변경 : START신호가 들어오면 START신호(BCSTARTAck)를 0.3초후에 인식하고, BCSTARTAck가 인식되면 입력과전압 체크
//==----->평상시에는 입력과전압 검지안하는 것임
//====================================================================
	//if(btst(DioInPutBuf,BCSTART_Chk))	BCSTARTAck = NO;
	//else                                BCSTARTAck = YES;

/*	
	if(btst(DioInPutBuf,BCSTART_Chk)){
		BCSTARTAck = NO;
		BCSTARTAck_Delay_Counter = 0;
	}
	else{//start신호가 입력되면
		BCSTARTAck_Delay_Counter++;
		if(BCSTARTAck_Delay_Counter>5000){//66usec*5000 = 330msec, 약 0.33초 딜레이
			BCSTARTAck = YES;
			BCSTARTAck_Delay_Counter = 5000;
		}
	}
*/
//===20160224 입력과전압 체크 로직변경(수서꺼 문제문제되어 과전압 로직 추가보완)
//===수서편성 운행중 입력과전압 폴트발생-->로템 추가보완요청
//===기존 : START신호가 들어오면 START신호(BCSTARTAck)를 0.3초후에 인식하고, BCSTARTAck가 인식되면 입력과전압 체크
//==-------->평상시에는 입력과전압 검지안하는 것임
//===변경 : 기동조건(590V~810V)만족되면 기동시작 --입력과전압폴트검지는 아직 안함
//          START신호가 들어오면 START신호(BCSTARTAck)를 0.3초후에 인식하고, BCSTARTAck가 인식되면 기동시작
//          CHK동작시켜 동작확인되면 입력과전압 체크
//---->따라서 아래 START신호 0.3초 지연시킨것을 그대로 유지
//====================================================================
	if(btst(DioInPutBuf,BCSTART_Chk)){
		BCSTARTAck = NO;
		BCSTARTAck_Delay_Counter = 0;
	}
	else{//start신호가 입력되면
		BCSTARTAck_Delay_Counter++;
		if(BCSTARTAck_Delay_Counter>5000){//66usec*5000 = 330msec, 약 0.33초 딜레이
			BCSTARTAck = YES;
			BCSTARTAck_Delay_Counter = 5000;
		}
	}
	
	if(btst(DioInPutBuf,CHK_Chk))  		CHKAck = NO;
	else                                CHKAck = YES;
	if(btst(DioInPutBuf,LK_Chk))		LKAck = NO;
	else                                LKAck = YES;
	if(btst(DioInPutBuf,BC72V_Chk))		BC72VAck = NO;
	else                                BC72VAck = YES;
		
		
	if(btst(DioInPutBuf,TEMP85SW_Chk))	TEMP85SWAck = NO;	//b접점
	else                                TEMP85SWAck = YES;
//-----2015년 3월 25일 --->팬보드 변경전
//	if(btst(DioInPutBuf,FANF1_Chk))		FANF1Ack = YES;
//	else                                FANF1Ack = NO;
//	if(btst(DioInPutBuf,FANF2_Chk)) 	FANF2Ack = YES;
//	else                                FANF2Ack = NO;
//-----2015년 3월 25일 --->팬보드 변경후
	if(btst(DioInPutBuf,FANF1_Chk))		FANF1Ack = NO;
	else                                FANF1Ack = YES;
	if(btst(DioInPutBuf,FANF2_Chk)) 	FANF2Ack = NO;
	else                                FANF2Ack = YES;    
}


//==================================================================================//
// 함수명 : _DOutPut()													//
// 기  능 :                      				//
// Calling : 시퀸스루틴에서(약 10msec)							//
// DATE  : 2015. 01. 19		수정 : 2015년4월9일									//
//==================================================================================//
void DOutPut()
{
//--정상충전 출력신호 표시  -->차량으로 정상출력이라고 보내고 mcu보드내 led(LD5) ON
//--모듈 정상출력 --> 릴레이보드 --> 차량제어

	if(MajorFault == YES)
	{
	  //ELED_LD7_ON();//20150524 LD5는 출력정상(ON), LD6은 CVCC모드(CV ON, CC OFF), LD7은 중고장시 ON으로 변경
		MFAck_MY = NO;
	}
	
	
	if(SEQ_Step == SEQ_Normal_State)
	{
		if((MODEAck == TESTMODE0)||((MODEAck == NORMALMODE1)&&(MBatVolLPF>60)&&(MOutVolLPF>60))){
			MOUTV_OKAck = 1;	//출력전압 정상
			ELED_LD5_ON();
			BCSTATUSAck = 1;	//출력전압 70v이하
		  cbit(DioOutBuf1,BC_STATE_OUT);	
		}
		else
		{
			MOUTV_OKAck	= 0;
			ELED_LD5_OFF();		
			BCSTATUSAck = 0;	//출력전압 정상
			sbit(DioOutBuf1,BC_STATE_OUT);//Active low	
		}
	}
	else
	{
		MOUTV_OKAck	= 0;	//출력전압 50v이하
		ELED_LD5_OFF();		
		BCSTATUSAck = 0;	//출력전압 정상
		sbit(DioOutBuf1,BC_STATE_OUT);//Active low
	}	
	
//--CVCC모� 표시(Mode_CV0CC1)
	if(Mode_CV0CC1 == Mode_CV0)		{ELED_LD6_ON();}	//CV모드면 ON
	else							{ELED_LD6_OFF();}	//CC모드면 OFF
	
//-----DioLedBuf2(active high임)-------------------------------------------------------------//
	if(BCSTARTAck == YES)		sbit(DioLedBuf2, 0);//START신호입력 확인됨
	else						cbit(DioLedBuf2, 0);//START신호입력 확인안됨
	if(MODEAck)					sbit(DioLedBuf2, 1);	// MODE = TESTMODE0, NORMALMODE1
	else						cbit(DioLedBuf2, 1);		
	if(!FaultAck)				sbit(DioLedBuf2, 2);	//기동준비상태(READY)신호는 우선 폴트가 없으면 
	else						cbit(DioLedBuf2, 2);		
	if(CHKAck)					sbit(DioLedBuf2, 3);	// CHKA상태
	else						cbit(DioLedBuf2, 3);		
	if(LKAck)					sbit(DioLedBuf2, 4);	// LKA상태
	else						cbit(DioLedBuf2, 4);		
	if(PWM_FLAG)				sbit(DioLedBuf2, 5);	// PWM ON
	else						cbit(DioLedBuf2, 5);		
	if(BCSTATUSAck)				sbit(DioLedBuf2, 6);	// 모듈 출력전압 정상 출력
	else						cbit(DioLedBuf2, 6);		
	if(FanOnOffCmd_flag == 1)
	{	// FAN1,2 동작시키면
		if((!FANF2Ack)&&(!FANF1Ack))
		{// 팬폴트 없으면
			sbit(DioLedBuf2, 7);
		}
		else
		{
			cbit(DioLedBuf2, 7);	
		}
	}
	else
	{
		cbit(DioLedBuf2, 7);		
	}
	
//-----Epld_EledBuf(active low임)-------------------------------------------------------------//
	if(btst(FLGDF,GDF1))									cbit(Epld_EledBuf, 0);			//(dcdc-Leading Leg)
	else 													sbit(Epld_EledBuf, 0);	
	if(btst(FLGDF,GDF2))									cbit(Epld_EledBuf, 1);			//(dcdc-Lagging Leg)
	else 													sbit(Epld_EledBuf, 1);	
	if(btst(FLETF,ETCF_HSKTHRF))							cbit(Epld_EledBuf, 2);			// 방열판 온도센서 1,2 폴트처리	
	else 													sbit(Epld_EledBuf, 2);	
	if(btst(FLETF,ETCF_FANF1)||btst(FLETF,ETCF_FANF2))		cbit(Epld_EledBuf, 3);			//팬1,2
	else 													sbit(Epld_EledBuf, 3);	
	if(btst(FLBXF,BOXF_BATOC1))								cbit(Epld_EledBuf, 4);			//차량 BAT1 CB TRIP
	else 													sbit(Epld_EledBuf, 4);	
	if(btst(FLBXF,BOXF_BATOC2))								cbit(Epld_EledBuf, 5);			//차량 BAT2 CB TRIP
	else 													sbit(Epld_EledBuf, 5);	

	if(btst(FLSQF,SEQF_CHKa))								cbit(Epld_EledBuf, 6);			//CHK폴트
	else 													sbit(Epld_EledBuf, 6);	
	if(btst(FLSQF,SEQF_LKa))								cbit(Epld_EledBuf, 7);			//LK폴트
	else 													sbit(Epld_EledBuf, 7);	
		
//-----DioLedBuf1-------------------------------------------------------------//
	if(btst(FLSWF,SWF0_ICOCF)||btst(FLHWF,HWF0_ICOCF))		sbit(DioLedBuf1, DLED1_BIT0);			//입력 과전류(SWF또는 HWF)
	else 													cbit(DioLedBuf1, DLED1_BIT0);	
	if(btst(FLSWF,SWF1_MBATOCF)||btst(FLHWF,HWF1_MBATOCF))	sbit(DioLedBuf1, DLED1_BIT1);			//모듈 충전과전류(SWF또는 HWF)
	else 													cbit(DioLedBuf1, DLED1_BIT1);	
	if(btst(FLSWF,SWF2_MOCF)||btst(FLHWF,HWF2_MOCF))		sbit(DioLedBuf1, DLED1_BIT2);			//모듈 출력과전류(SWF또는 HWF)
	else 													cbit(DioLedBuf1, DLED1_BIT2);	
	cbit(DioLedBuf1, DLED1_BIT3);//---->나중에 필요시 오버로드
	//if(MajorFault)				sbit(DioLedBuf1, DLED1_BIT3);//MajorFault 확인됨
	//else						cbit(DioLedBuf1, DLED1_BIT3);//MajorFault 확인안됨

	if(btst(FLSWF,SWF4_BBAT1OCF)||btst(FLHWF,HWF4_BBAT1OCF))	sbit(DioLedBuf1, DLED1_BIT4);		//BOX 충전전류1 과전류(SWF또는 HWF)
	else 														cbit(DioLedBuf1, DLED1_BIT4);	
	if(btst(FLSWF,SWF5_BBAT2OCF)||btst(FLHWF,HWF5_BBAT2OCF))	sbit(DioLedBuf1, DLED1_BIT5);		//BOX 충전전류2 과전류(SWF또는 HWF)
	else 														cbit(DioLedBuf1, DLED1_BIT5);	
	if(btst(FLSWF,SWF6_BOCF)||btst(FLHWF,HWF6_BOCF))			sbit(DioLedBuf1, DLED1_BIT6);		//BOX 출력전류 과전류 과전류(SWF또는 HWF)
	else 														cbit(DioLedBuf1, DLED1_BIT6);	
	if(btst(FLSWF,SWF14_CPUVF)||btst(FLSWF,SWF7_CPOVF)||btst(FLHWF,HWF7_CPOVF))			sbit(DioLedBuf1, DLED1_BIT7);		//제어전압 저/과전압(SWF또는 HWF)
	else 																				cbit(DioLedBuf1, DLED1_BIT7);	
//-----DioLedBuf3-------------------------------------------------------------//
	if(btst(FLSWF,SWF8_OVOVF)||btst(FLHWF,HWF8_OVOVF))		sbit(DioLedBuf3, DLED1_BIT0);			//출력 과전압-내부(SWF또는 HWF)
	else 													cbit(DioLedBuf3, DLED1_BIT0);	
	if(btst(FLSWF,SWF9_BVOVF)||btst(FLHWF,HWF9_BVOVF))		sbit(DioLedBuf3, DLED1_BIT1);			//출력 과전압-외부(SWF또는 HWF)
	else 													cbit(DioLedBuf3, DLED1_BIT1);	
	if(btst(FLSWF,SWF10_IVOVF)||btst(FLHWF,HWF10_IVOVF))	sbit(DioLedBuf3, DLED1_BIT2);			//입력 과전압(SWF또는 HWF)
	else 													cbit(DioLedBuf3, DLED1_BIT2);	
	if(btst(FLSWF,SWF11_FCOVF)||btst(FLHWF,HWF11_FCOVF))	sbit(DioLedBuf3, DLED1_BIT3);			//DCLINK 과전압(SWF또는 HWF)
	else 													cbit(DioLedBuf3, DLED1_BIT3);	

	
//	if(btst(FLSWF,SWF12_OVUVF))								sbit(DioLedBuf3, DLED1_BIT4);			//출력전압 저전압(OBD전,후 공용) 50V
//	else 													cbit(DioLedBuf3, DLED1_BIT4);	
	//출력저전압폴트(OBD 전/후)는 보드에서 LED 1개로 표시
	if(btst(FLSWF,SWF12_OVUVF)||btst(FLSWF,SWF15_OVUVF2))								sbit(DioLedBuf3, DLED1_BIT4);			//출력전압 저전압(OBD전,후 공용) 50V
	else 													cbit(DioLedBuf3, DLED1_BIT4);	
	if(btst(FLSWF,SWF13_IVUVF))								sbit(DioLedBuf3, DLED1_BIT5);			//입력전압 저전압 620V-->610V
	else 													cbit(DioLedBuf3, DLED1_BIT5);	
	
	//if(btst(FLSWF,SWF15_FCUVF))								sbit(DioLedBuf3, DLED1_BIT6);			//DCLINK전압 저전압 620V-->610V
	//else 													cbit(DioLedBuf3, DLED1_BIT6);	
	
	if(btst(FLETF,ETCF_FUSEF))								sbit(DioLedBuf3, DLED1_BIT6);			//휴즈폴트
	else 													cbit(DioLedBuf3, DLED1_BIT6);	
		
	if(MajorFault)				sbit(DioLedBuf3, 7);//MajorFault 확인됨
	else						cbit(DioLedBuf3, 7);//MajorFault 확인안됨
	
	//=======================================================	
	CS_WR_DOUT1() = DioOutBuf1;//
	//=======================================================
	CS_WR_LED1() = (DioLedBuf1 & 0xFF);
	CS_WR_LED2() = (DioLedBuf2 & 0xFF);
	CS_WR_LED3() = (DioLedBuf3 & 0xFF);
	CS_WR_ELED() = (Epld_EledBuf & 0xFF);
	//=======================================================
}

/*	
void ELED_LD8_ON()
{
	cbit(Epld_EledBuf,EELED_LED1); 
	CS_WR_ELED() = Epld_EledBuf; 	
}		
void ELED_LD8_OFF()
{
	sbit(Epld_EledBuf,EELED_LED1);
	CS_WR_ELED() = Epld_EledBuf; 	
}
void ELED_LD8_TOGGLE()
{
	tbit(Epld_EledBuf,EELED_LED1);
	CS_WR_ELED() = Epld_EledBuf; 	
}			
*/
//---------------------------------------		
void ELED_LD7_ON()
{
	cbit(Epld_CtrlBuf,EELED_LED2); 
	CS_WR_ELED() = Epld_CtrlBuf; 	
}		
void ELED_LD7_OFF()
{
	sbit(Epld_CtrlBuf,EELED_LED2);
	CS_WR_ELED() = Epld_CtrlBuf; 	
}
void ELED_LD7_TOGGLE()
{
	tbit(Epld_CtrlBuf,EELED_LED2);
	CS_WR_ELED() = Epld_CtrlBuf; 	
}	
		
//---------------------------------------		
void ELED_LD6_ON()
{
	cbit(Epld_CtrlBuf,EELED_LED3); 
	CS_WR_CTRL() = Epld_CtrlBuf; 	
}		
void ELED_LD6_OFF()
{
	sbit(Epld_CtrlBuf,EELED_LED3);
	CS_WR_CTRL() = Epld_CtrlBuf; 	
}
void ELED_LD6_TOGGLE()
{
	tbit(Epld_CtrlBuf,EELED_LED3);
	CS_WR_CTRL() = Epld_CtrlBuf; 	
}			
//---------------------------------------		

void ELED_LD5_ON()
{
	cbit(Epld_CtrlBuf,EELED_LED4); 
	CS_WR_CTRL() = Epld_EledBuf; 	
}		
void ELED_LD5_OFF()
{
	sbit(Epld_CtrlBuf,EELED_LED4);
	CS_WR_CTRL() = Epld_EledBuf; 	
}
void ELED_LD5_TOGGLE()
{
	tbit(Epld_CtrlBuf,EELED_LED4);
	CS_WR_CTRL() = Epld_EledBuf; 	
}			

//---------------------------------------		
void ELED_TP1_HI()
{
	sbit(Epld_CtrlBuf,EELED_TP1); //TSTPIN(TP1)
	CS_WR_CTRL() = Epld_CtrlBuf; 	
}		
void ELED_TP1_LOW()
{
	cbit(Epld_CtrlBuf,EELED_TP1);
	CS_WR_CTRL() = Epld_CtrlBuf; 	
}
void ELED_TP1_TOGGLE()
{
	tbit(Epld_CtrlBuf,EELED_TP1);
	CS_WR_CTRL() = Epld_CtrlBuf; 	
}			
//---------------------------------------		
void ELED_TP2_HI()
{
	sbit(Epld_CtrlBuf,EELED_TP2); //TSTPIN(TP2)
	CS_WR_CTRL() = Epld_CtrlBuf; 	
}		
void ELED_TP2_LOW()
{
	cbit(Epld_CtrlBuf,EELED_TP2);
	CS_WR_CTRL() = Epld_CtrlBuf; 	
}
void ELED_TP2_TOGGLE()
{
	tbit(Epld_CtrlBuf,EELED_TP2);
	CS_WR_CTRL() = Epld_CtrlBuf; 	
}
//---------------------------------------		
void ELED_TP3_HI()
{
	sbit(Epld_CtrlBuf,EELED_TP3); //TSTPIN(TP3)
	CS_WR_CTRL() = Epld_CtrlBuf; 	
}		
void ELED_TP3_LOW()
{
	cbit(Epld_CtrlBuf,EELED_TP3);
	CS_WR_CTRL() = Epld_CtrlBuf; 	
}
void ELED_TP3_TOGGLE()
{
	tbit(Epld_CtrlBuf,EELED_TP3);
	CS_WR_CTRL() = Epld_CtrlBuf; 	
}			
//---------------------------------------		
void ELED_TP4_HI()
{
	sbit(Epld_CtrlBuf,EELED_TP4); //TSTPIN(TP3)
	CS_WR_CTRL() = Epld_CtrlBuf; 	
}		
void ELED_TP4_LOW()
{
	cbit(Epld_CtrlBuf,EELED_TP4);
	CS_WR_CTRL() = Epld_CtrlBuf; 	
}
void ELED_TP4_TOGGLE()
{
	tbit(Epld_CtrlBuf,EELED_TP4);
	CS_WR_CTRL() = Epld_CtrlBuf; 	
}			
