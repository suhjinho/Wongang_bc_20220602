//===========================================================================//
// 원강선 동력차(30kw)/객차(50kw) 축전지 충전기 모듈(10kw) 프로그램	           	
//===========================================================================//

#include "Global_Variable_Function.h"

//===========================================================================//
// 함수명 : Sequence()														  
// 기  능 : 프로그램 시퀸스 부분 처리												
// calling : main(), 10msec에서 동작                                                
//===========================================================================//
void Sequence()
{
	Check_DI();				//Digital 입력신호 처리		
	SequenceTimer();		//Sequence동작 타이머
	if(ProgramMode_B0M1C2 == ProgramMode_CPU2)
	{
		;;	
	}
	else
	{
		SEQ_StartChk();
		SEQ_StatusCheck();	
		SEQ_Step_Process();
		DOutPut();
	}
}


void Reset_Sequence_OLD()
{
	if(RESETAck == YES)
	{
		;;
	}
	//---차량 리셋명령(폴링에지) 동작 -->차량리셋은 박스 중고장시 리셋해야함
	//---중고장 없을때는 ...차량에서 RESET지령이 와도 리셋하면 안됨
	//---박스중고장 판단은?--->모듈 2개이상 중고장일때.. 
	if((RESETAck_OLD == 1)&&(RESETAck == 0))
	{
		HighFaultAck = 0;
		HighFaultMJF = 0;

		if(MajorFault == YES)
		{
			Seq_Status = SeqStatus_Init0;		// Init = SeqStatus_Init0(0)
			SEQ_Step = SEQ_DO_NOTHING_STATE;	// Init = SEQ_DO_NOTHING_STATE(0)
		}
	}

	RESETAck_OLD = RESETAck;	
	//---보드 PB SWITCH로 리셋은 중고장 상태에서 스위치 눌렸다 떼면(폴링에지) 동작
	if((MCUBD_PUSHBUTTONSW_IN_OLD == 1)&&(MCUBD_PUSHBUTTONSW_IN == 0))
	{//PB SWITCH 눌렸다 떼면 리셋동작
		if(MajorFault == YES)
		{
			Seq_Status = SeqStatus_Init0;	// Init = SeqStatus_Init0(0)
			SEQ_Step = SEQ_DO_NOTHING_STATE;	// Init = SEQ_DO_NOTHING_STATE(0)
		}
	}
	MCUBD_PUSHBUTTONSW_IN_OLD = MCUBD_PUSHBUTTONSW_IN;
}

//==================================================//
// 차량리셋 프로그램                                
//==================================================//

void Reset_Sequence()
{
	if((RESETAck_OLD == 1)&&(RESETAck == 0))
	{//차량 리셋명령(폴링에지)//20
		if(MajorFault == YES)
		{
			Seq_Status = SeqStatus_Init0;	// Init = SeqStatus_Init0(0)
			SEQ_Step = SEQ_DO_NOTHING_STATE;	// Init = SEQ_DO_NOTHING_STATE(0)


			FaultClear();
			BCFAULTOUT_OFF();//다른 모듈한테 내가 폴트 없다고 출력
			HighFaultAck = 0;
			HighFaultMJF = 0;
			MFAck_MY = NO;
			cbit(DioLedBuf3, 7);//중고장 표시해제
		}
	}
	
	RESETAck_OLD = RESETAck;	
	//---보드 PB SWITCH로 리셋은 중고장 상태에서 스위치 눌렸다 떼면(폴링에지) 동작
	if((MCUBD_PUSHBUTTONSW_IN_OLD == 1)&&(MCUBD_PUSHBUTTONSW_IN == 0)){//PB SWITCH 눌렸다 떼면 리셋동작
		if(MajorFault == YES)
		{	
			FaultClear();
			BCFAULTOUT_OFF();//다른 모듈한테 내가 폴트 없다고 출력
			HighFaultAck = 0;
			HighFaultMJF = 0;
			MFAck_MY = NO;
			cbit(DioLedBuf3, 7);//중고장 표시해제
			
			
			
			Seq_Status = SeqStatus_Init0;	// Init = SeqStatus_Init0(0)
			SEQ_Step = SEQ_DO_NOTHING_STATE;	// Init = SEQ_DO_NOTHING_STATE(0)
		}
	}
	MCUBD_PUSHBUTTONSW_IN_OLD = MCUBD_PUSHBUTTONSW_IN;
}

//============================================================================//
// 함수명 : _SEQ_StartChk()                                                         
// 기  능 : 기동조건 점검(입력전압, 차량RUN지령, FAULT등)	                        
// calling : _Sequence()                                                            
                                                           
//=========================================================================//
// 기동조건                                                                         
// 1. 입력전압 정상                                                                 
// 2. 폴트없음                                                                      
// 3. 차량 기동지령 입력                                                            
// 4. 타모듈 고장상태(2개 고장시 기동정지)                                          
// 5.                                                                               
//=======================================================================//
//=== 입력과전압 체크 로직변경하면서 기동조건 보완
//===수서편성 운행중 입력과전압 폴트발생-->로템 추가보완요청
//===기존 : START신호가 들어오면 START신호(BCSTARTAck)를 0.3초후에 인식하고, BCSTARTAck가 인식되면 입력과전압 체크
//          기동조건에 입력전압 500V이상이면 기동가능에서
//===변경 : 기동조건(590V~810V)만족되면 기동시작 -->하한전압 500V에서 590V로 올리고,
//                                               -->상한전압 810V로
//====================================================================	
void SEQ_StartChk()
{
	//----- 기동조건 검사 --------------//	
	if(MODEAck == TESTMODE0){	// TESTMODE(0), NORMALMODE(1)
		if(MInputVolLPF > (MInputVolRATED * 0.1)) MInputVolAck = YES;
		else                                        MInputVolAck = NO;
		
		if((BCSTARTAck == YES)&&(MInputVolAck == NO)&&(FaultAck == NO)&&(ModuleFault2ea_flag == NO)){
			RunOKNG = RUN_OK;	// 테스트모드에서 입력전압이 없으면 APSRunOKNG = APS_RUN_OK;
		}
		else{
			RunOKNG = RUN_NG;	// 테스트모드에서 입력전압이 있으면 APSRunOKNG = APS_RUN_NG;
		}
	}
	else
	{	// NORMAL MODE
		if((MInputVolLPF > 590))						MInputVolAck = YES;//InputVolMIN = 620.00;			//[Vdc] 원강선 System 사양	
		else                                      		MInputVolAck = NO;

		if((BCSTARTAck == YES)&&(MInputVolAck == YES)&&(FaultAck == NO)&&(ModuleFault2ea_flag == NO))
		{
			RunOKNG = RUN_OK;	// 노말모드에서 상위RUN지령이 있고,입력전압이 있고, PLL이 인식되고, 폴트가 없으면 APSRunOKNG = APS_RUN_OK;
		}
		else
		{
			RunOKNG = RUN_NG;	// 노말모드에서 상위RUN지령이 없거나, 입력전압이 없거나, PLL이 인식안되거나, 폴트가 있으면 APSRunOKNG = APS_RUN_NG;	
		}
	}	

}
//===================================================================//
// 함수명 : _SEQ_StatusCheck()													    
// 기  능 : SEQUENCE 상태 점검                                                      
// calling : _Sequence()                                                                                                                        
//===================================================================//
//#define SeqStatus_Init0			0
//#define SeqStatus_Check1			1
//#define SeqStatus_Restart2		2
//#define SeqStatus_MajorFault3		3

void SEQ_StatusCheck()
{	
	switch(Seq_Status)
	{
		case SeqStatus_Init0:	//0		//Seq 초기조건			
 			SEQ_Step = SEQ_DO_NOTHING_STATE;
			Seq_Status = SeqStatus_Check1;
		break;
		case SeqStatus_Check1:		
			if(RunOKNG == RUN_OK)
			{	// 초기 기동시 정상기동조건이면
				if(SEQ_Step == SEQ_DO_NOTHING_STATE)
				{ 
					SEQ_Step = SEQ_START;			//정상기동조건에서 아무것도 하지 않고 있으면 처음 시작 Sequence Start
				}
				// 여기가 정상운전상태임<------
				
				//  if(RESETAck == YES) {    //만약에 외부에서 Reset신호가 입력될때 처리함.
				//	SEQ_Step = SEQ_RESET;
				//	리셋추가코드필요함
				//}				
			}
			else
			{// 정상기동조건이 아닐때...
				PWM_OFF();
				SEQ_Step = SEQ_ALLOFF;	// 정상조건 아니면..

				if(FaultAck)
				{
					if(MidFaultAck)
					{// 중간레벨 고장시(3분3회고장적용고장) 5초후 재기동
						MidiumFault_Count_INC();
						FaultCounter++;		//폴트 카운터 증가시키고
						FaultRestartTimer_flag = FLAGCLR;
						FaultRestartTimerSet = 500;// 5sec후 재기동 5sec = 10msec*500;	
						Seq_Status = SeqStatus_Restart2;
					}
					else
					{		// 경고장일때 토탈 폴트카운터만 하고...자동 재기동
						FaultCounter++;		//폴트 카운터 증가시키고 -->모니터링 폴트 카운터 -->재기동 횟수??
						FaultRestartTimer_flag = FLAGCLR;
						FaultRestartTimerSet = 500;// 5sec후 재기동 5sec = 10msec*500;	
						Seq_Status = SeqStatus_Restart2;
					}
				}
				if((MidFaultMJF > 0)||(HighFaultMJF==1))
				{	// 3분3회중고장 또는 중고장시 메이저폴트발생
					MajorFault = YES;
		 			SEQ_Step = SEQ_ALLOFF;			//시퀸스 초기화
					Seq_Status = SeqStatus_MajorFault3;	//Sys_Status를 majorfault로 ..
				}
			}
		break;	
		case SeqStatus_Restart2:		//2----재기동
			if (FaultRestartTimer_flag == FLAGSET)
			{						// 재기동 시간 5sec check
				if(FaultAck){
					FaultClear();	// 재기동전 폴트 클리어시킴...
					FAULTLATCHCLEAR();//재기동하기전에 epld에서 래치되었던 폴트 래치 풀어줌
					//FaultAck = 0;
				}
				else
				{	//폴트가 없으면 재기동하고, 계속 폴트상태면 재기동 안하고 대기
					Seq_Status = SeqStatus_Check1;
					SEQ_Step = SEQ_DO_NOTHING_STATE; 	
				}
			}
			if((MidFaultMJF > 0)||(HighFaultMJF==1))
			{	// 3분3회중고장 또는 중고장시 메이저폴트발생
				MajorFault = YES;
	 			SEQ_Step = SEQ_ALLOFF;			//시퀸스 초기화
				Seq_Status = SeqStatus_MajorFault3;	//Sys_Status를 majorfault로 ..
			}
		break;
										
		case SeqStatus_MajorFault3:		//3		//중고장일경우 폴트 클리어 안함
			PWM_FLAG = NO;
			PWM_OFF();
		
			CHK_OFF();
			LK_OFF();
			MajorFault = YES;
			MidFaultAck = 0;
			FaultAck = NO;	//임시 <====여기서 FaultAck를 0으로 하면 안됨.<---	FaultAckOld
					//중고장시험시 5초 딜레이 줘야함---	FaultAckOld
		break;	
		default:
			Seq_Status = SeqStatus_Init0;
		break;
	}
	MidiumFault_3min3N_Check();	//3분이내 3회 동일고장시 중고장 검사
}

//==================================================================================//
// 함수명 : _SEQ_Step_Process()													    //
// 기  능 : SEQUENCE STEP                                                           //
// calling : _Sequence()                                                             
//==================================================================================//
void SEQ_Step_Process()
{
/*---*/
	switch(SEQ_Step)
	{
		case SEQ_DO_NOTHING_STATE:		//0
			;;
		break;							
		case SEQ_START:					//1
			FaultClear();	
			PWM_OFF();
			
			SeqTimer_flag = FLAGCLR;//Clear;
			SeqTimerSet = 160;			//

			SEQ_Step = SEQ_CHK_ON;
		break;
		case SEQ_CHK_ON:				//2
			if(SeqTimer_flag)
			{
				CHK_ON();
				//FAN_ON();
				
				SeqTimer_flag = FLAGCLR;
				SeqTimerSet = 30;
				SEQ_Step = SEQ_CHKa_CHK;
			}
		break;
		case SEQ_CHKa_CHK:			//3
			if(SeqTimer_flag)
			{ 		// Normal mode에서는 chk 접점, dclink충전전압 확인,// Test mode에서는 chk 접점만 확인											
				if(((CHKAck == YES) && (MDcLinkVolLPF > 400.0))||((CHKAck == YES) && (MODEAck == TESTMODE0))){		//CHK동작 & DCLINK VOLTAGE 확인
					SEQ_Step = SEQ_LK_ON;
					SeqTimer_flag = FLAGCLR;
					SeqTimerSet = 60;
				}
				else
				{
					//SEQ_Step = SEQ_ALLOFF;
					//sbit(FSQF,SEQF_CHKa);	// CHK or DCLINK Voltage low level ERROR
					if(MODEAck == TESTMODE0)
					{
						if(CHKAck == NO){
							sbit(FSQF,SEQF_CHKa);	// CHK 불량 --->CHKa신호선 불량이거나, CHK불량 또는 CHK ON 와이어링 불량				
						}
						else{//CHK는 동작했는데....DCLINK전압이 없으면 휴즈단선으로 판단
							cbit(FETF,SEQF_CHKa);	//휴즈단선		
						}			
					}
					
					if(MODEAck == NORMALMODE1)
					{
						if(CHKAck == NO)
						{
							sbit(FSQF,SEQF_CHKa);	// CHK 불량 --->CHKa신호선 불량이거나, CHK불량 또는 CHK ON 와이어링 불량				
						}
						else
						{//CHK는 동작했는데....DCLINK전압이 없으면 휴즈단선으로 판단
							sbit(FETF,ETCF_FUSEF);	//휴즈단선		
						}			
					}
					SEQ_Step = SEQ_ALLOFF;
				}				
			}
		break;
		case SEQ_LK_ON:					//4
			if(SeqTimer_flag)
			{	
				LK_ON();
				SeqTimer_flag = FLAGCLR;
				SeqTimerSet = 60;			//LK동작시간 1sec시간 세팅
				SEQ_Step = SEQ_LKa_CHK;
			}
		break;

		case SEQ_LKa_CHK:			//5
			if(SeqTimer_flag)
			{ 	// Normal mode에서는 chk 접점, dclink자연충전전압 확인													
				if(((LKAck == YES) && (MDcLinkVolLPF > 450.0)) || ((LKAck == YES) && (MODEAck == TESTMODE0))){		//LK동작 & DCLINK VOLTAGE 확인
					SEQ_Step = SEQ_CHK_OFF;
					SeqTimer_flag = FLAGCLR;
					SeqTimerSet = 50;
				}
				else
				{
					SEQ_Step = SEQ_ALLOFF;
					sbit(FSQF,SEQF_LKa);	// LK or DCLINK Voltage low level ERROR
				}				
			}
		break;
		case SEQ_CHK_OFF:				//6
			if(SeqTimer_flag)
			{
				CHK_OFF();
				SeqTimer_flag = FLAGCLR;
				SeqTimerSet = 30;
				SEQ_Step = SEQ_PWM_ON;
			}
		break;			
		case SEQ_PWM_ON:			//7
			if(SeqTimer_flag)
			{						
				if((MDcLinkVolLPF > 550.0)||(MODEAck == TESTMODE0))
				{	//0.9 원래꺼	
					DCDC_SoftStart_flag = YES;	//DCDC CON Soft_start==>SET	
					PWM_ON();	
					PWM_FLAG = YES;
					BCFAULTOUT_OFF();//다른 모듈한테 내가 폴트 없다고 출력
					SeqTimer_flag = FLAGCLR;
					SeqTimerSet = 200;						//1.5sec 다음 Seq시간 세팅

					SEQ_Step = SEQ_OUTV_CHK;		
				}
			}
		break;	
		case SEQ_OUTV_CHK:		//8
			if(SeqTimer_flag)
			{						//정격의 90%이상													
				if((MOutVolLPF > 58.0) || (MODEAck == TESTMODE0))
				{
					SeqTimer_flag = FLAGCLR;
					SeqTimerSet = 100;						//다음 Seq시간 세팅
					SEQ_Step = SEQ_Normal_State;		// bach는 SIVK없으므로 ...
					if(MODEAck == NORMALMODE1)
					{
						FAN_ON();
					}
					/*
					if((MODEAck == NORMALMODE1)&&(ProgramMode_B0M1C2 = ProgramMode_MODULE1)){
						FAN_OFF();
					}
					else{MODEAck == NORMALMODE1)&&(ProgramMode_B0M1C2 = ProgramMode_MODULE1)){
						FAN_OFF();
					}
					*/

				}
			}
		break;

		case SEQ_Normal_State:			//9
			// 저전압 폴트 체크 --> SW FAULT 루틴에서 검사
			// 입력저전압, 출력저전압, DCLINK 저전압 ==> LOW LEVEL FAULT임
			;;
//			if((MBatVolLPF>40)&&(MOutVolLPF))(
//				BCSTATUSAck = 1;	//출력전압 정상
//			}
			
				//BCSTARTAck = 0;//테스트용
		break;
		case SEQ_ALLOFF:				//10
			InitControlVar();//InitDcDcControlVar
			PWM_FLAG = 0;
			PWM_OFF();			
			//BCFAULTOUT_OFF();//??????????????????????????????
			CHK_OFF();
			LK_OFF();
			FAN_OFF();	//강제로 팬 꺼줌
			//BC_SPARE_OFF();//테스트용--나중 지울것
			SeqTimer_flag = FLAGCLR;
			SeqTimerSet = 50;	//다음 Seq시간 세팅	
			SEQ_Step =  SEQ_DO_NOTHING_STATE;
		break;
		case SEQ_RESET:					//11
			SEQ_Step =  SEQ_ALLOFF;
		break;		
		default:
			SEQ_Step = SEQ_ALLOFF;
		break;
	}

}

//=====================================================================//
// 함수명 : SequenceTimer()														   															
//=====================================================================//
void SequenceTimer()
{
	if(SeqTimer_flag == FLAGCLR)
	{
		SeqTimer++;
		if(SeqTimer >= SeqTimerSet)	
		{
			SeqTimer_flag = FLAGSET;
			SeqTimer = 0;
		}
	}
	if(FaultRestartTimer_flag == FLAGCLR)
	{
		FaultRestartTimer++;
		if(FaultRestartTimer >= FaultRestartTimerSet)
		{
			FaultRestartTimer_flag = FLAGSET;
			FaultRestartTimer = 0;
		}
	}
}

void CHK_ON()
{
	sbit(DioOutBuf1,BC_CHKON_OUT);
//	sbit(DioLedBuf01_16, DLED_CHKX);
}
void CHK_OFF()
{
	cbit(DioOutBuf1,BC_CHKON_OUT);
//	cbit(DioLedBuf01_16, DLED_CHKX);
}
void LK_ON()
{
	sbit(DioOutBuf1,BC_LKON_OUT);
//	sbit(DioLedBuf01_16, DLED_LKX);
}
void LK_OFF()
{
	cbit(DioOutBuf1,BC_LKON_OUT);
//	cbit(DioLedBuf01_16, DLED_LKX);
}
void BC_SPARE_ON()
{
	sbit(DioOutBuf1,BC_SPARE72V_OUT);
//	sbit(DioLedBuf01_16, DLED_LKX);
}
void BC_SPARE_OFF()
{
	cbit(DioOutBuf1,BC_SPARE72V_OUT);
//	cbit(DioLedBuf01_16, DLED_LKX);
}

//===================================================//
//==FAN ON/OFF
//== ON : 모듈 정상출력이면 
//== OFF : 모듈 폴트면 정지
//===================================================//

void FAN_ON()
{
	FanOnOffCmd_flag = 1;
	sbit(DioOutBuf1,BC_FANON_OUT); 
//	cbit(DioLedBuf17_26, DLED_FANF);
}
void FAN_OFF()
{
	FanOnOffCmd_flag = 0;
	cbit(DioOutBuf1,BC_FANON_OUT);
//	cbit(DioLedBuf17_26, DLED_FANF);
}

/*---*/
void BCFAULTOUT_ON()
{
	sbit(DioOutBuf1,BC_FAULT_OUT);		// 모듈 폴트면 폴트라고 출력
	CS_WR_DOUT1() = DioOutBuf1;// ALL DOUT OFF

//	sbit(DioLedBuf01_16, DLED_TSTOK);/
}
void BCFAULTOUT_OFF()
{
	cbit(DioOutBuf1,BC_FAULT_OUT);		
	CS_WR_DOUT1() = DioOutBuf1;// ALL DOUT OFF
//	cbit(DioLedBuf01_16, DLED_TSTOK); 
}

