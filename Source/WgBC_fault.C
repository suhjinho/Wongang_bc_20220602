//==================================================================================//
// 원강선 동력차(30kw)/객차(50kw) 축전지 충전기 모듈(10kw) 프로그램	           		//
//==================================================================================//
// 폴트처리부분                                                                     //
// DATE  : 2015. 01. 07            						  by PACTECH R&D LSH		//
//==================================================================================//
//경고장 : 대부분 경고장으로 무한 재기동 가능함                                     //
//   단, 3분이내 3회 폴트시 중고장 처리는 아래                                      //
//      --> GDF1,2 - 차량 리셋에의해 재기동 불가                                    //
//      --> 입력과전류, 모듈 출력과전류 - 차량 리셋에의해 재기동 가능               //
//중고장 : 휴즈폴트, 컨텍터 소손, 배터리 과전류(차량 CB신호) -->차량리셋 재기동 불가//
//==================================================================================//
#include "Global_Variable_Function.h"

unsigned int DioInPutBuf_tst = 0x01F0;

float BBatCur2LPF_max = 0;
float BBatCur1LPF_max = 0;

Uint16 MOutVolSWOVF_Counter = 0.0;
Uint16 MBatVolSWOVF_Counter = 0.0;
Uint16 BBatCur1SWOVF_Counter = 0.0;
Uint16 BBatCur2SWOVF_Counter = 0.0;
Uint16 MOutVolSWUVF_Counter = 0;
Uint16 MOutVolSWUVF_Counter1 = 0;
Uint16 MOutVolSWUVF_Counter2 = 0;

Uint16 MInputVolUVF_Counter = 0;
float MBatCurNC_Counter=0;

unsigned int FLGDF_1ST = 0x00;	
unsigned int FLHWF_1ST = 0x00;
unsigned int FLSWF_1ST = 0x00;
unsigned int FLSQF_1ST = 0x00;
unsigned int FLETF_1ST = 0x00;	
unsigned int FLBXF_1ST = 0x00;
unsigned int test_OVF_flag1 = 0;
unsigned int test_OVF_flag2 = 0;
extern intpin_Chk();
//==================================================================================//
// 함수명 : SW_FaultProcess()		     										//
// 기  능 : S/W FAULT 1개라도 발생하면 dsp card epld에 sw fault를 써줌				//
//----------------------------------------------------------------------------------//
// 전류 : HW로 FAULT, SW(MIN/MAX)로 LIMIT제어										//
// 전압 : SW로 FAULT, SW(MIN/MAX)로 LIMIT제어										//
//----------------------------------------------------------------------------------//
// DATE  : 2010. 08. 13																//
//==================================================================================//
void Check_SWF()
{	
	float Temp = 0;

	Temp = fabs(MInputCurLPF);	// 입력과전류
	if (Temp > MInputCurSWOCF){
//	if (Temp > MInputCurSWOCF+10){
		sbit(MidFaultAck, MidMICOCF);	// 중간레벨(3분3회) - 입력과전류
		sbit(FSWF,SWF0_ICOCF);
	}
	else{
		cbit(FSWF,SWF0_ICOCF);
	}
	Temp = fabs(MBatCurLPF);	//모듈 충전전류 과전류
	if (Temp > MBatCurSWOCF){
		LowFaultAck = 1;	// Low Level 폴트로 처리 --> 조건 해제시 무한 재기동
		sbit(FSWF,SWF1_MBATOCF);
	}
	else{
		cbit(FSWF,SWF1_MBATOCF);
	}

	Temp = fabs(MOutCurLPF);	//모듈 출력전류 과전류
	if (Temp > MOutCurSWOCF){
		sbit(MidFaultAck, MidMICOCF);	// 중간레벨(3분3회) - 입력과전류
		sbit(FSWF,SWF2_MOCF);
	}
	else{
		cbit(FSWF,SWF2_MOCF);
	}

//BOX 충전전류1 과전류
	Temp = fabs(BBatCur1LPF);	//BOX 충전전류1 과전류
	if (Temp > BBatCur1SWOCF){
		BBatCur1SWOVF_Counter++;
	//	if(BBatCur1SWOVF_Counter > 600){//OVF레벨보다 큰값이 300번이상(600*66usec = 36msec) 지속되면 이면 폴트처리
		if(BBatCur1SWOVF_Counter > 7500){ // 20151117_창원에서 객차 BOX충전과전류 발생해서 수정
			LowFaultAck = 1;	
			sbit(FSWF,SWF4_BBAT1OCF);
			BBatCur1SWOVF_Counter = 0;
		}
//		if(Temp > BBatCur1SWOCF+80){//OVF+80V보다 크면  즉시 폴트처리
		if(Temp > BBatCur1SWOCF+100){// 20151117_창원에서 객차 BOX충전과전류 발생해서 수정
			LowFaultAck = 1;	
			sbit(FSWF,SWF4_BBAT1OCF);
			BBatCur1SWOVF_Counter = 0;
		}
	}
	else{
		BBatCur1SWOVF_Counter = 0;
	}
//BOX 충전전류2 과전류
	Temp = fabs(BBatCur2LPF);	//BOX 충전전류2 과전류
	if (Temp > BBatCur2SWOCF){
		BBatCur2SWOVF_Counter++;
//		if(BBatCur2SWOVF_Counter > 600){//OVF레벨보다 큰값이 100번이상(600*66usec = 36msec) 지속되면 이면 폴트처리
				if(BBatCur2SWOVF_Counter > 7500){ // 20151117_창원에서 객차 BOX충전과전류 발생해서 수정
			LowFaultAck = 1;	
			sbit(FSWF,SWF5_BBAT2OCF);
			BBatCur2SWOVF_Counter = 0;
		}
//		if(Temp > BBatCur2SWOCF+80){//OVF+80V보다 크면  즉시 폴트처리
		if(Temp > BBatCur2SWOCF+100){// 20151117_창원에서 객차 BOX 충전과전류 발생해서 수정
			LowFaultAck = 1;	
			sbit(FSWF,SWF5_BBAT2OCF);
			BBatCur2SWOVF_Counter = 0;
		}
	}
	else{
		BBatCur2SWOVF_Counter = 0;
	}

	
	Temp = fabs(BOutCurLPF);	//BOX 출력전류 과전류
	if (Temp > BOutCurSWOCF){
		LowFaultAck = 1;	// Low Level 폴트로 처리 --> 조건 해제시 무한 재기동
		sbit(FSWF,SWF6_BOCF);
	}
	
	
//---제어전압 과전압 검사---
	Temp = fabs(MCtrlVolLPF);	//제어전압 과전압 -----> ADC로 읽은값으로 저전압/과전압 둘다 해야함
	if (Temp > MCtrlVolSWOVF){
		LowFaultAck = 1;	// Low Level 폴트로 처리 --> 조건 해제시 무한 재기동
		sbit(FSWF,SWF7_CPOVF);
	}
	else{
		cbit(FSWF,SWF7_CPOVF);
	}
	
//---제어전압 저전압 검사---제어전압 저전압은 조건에 관계없이 항상 검사
	Temp = fabs(MCtrlVolLPF);	//제어전압 저전압 -----> ADC로 읽은값으로 저전압/과전압 둘다 해야함
	if (Temp < MCtrlVolUVF){
		LowFaultAck = 1;	// Low Level 폴트로 처리 --> 조건 해제시 무한 재기동
		sbit(FSWF,SWF14_CPUVF);
	}
	else{
		cbit(FSWF,SWF14_CPUVF);
	}

//---출력전압 과전압 검사	
	Temp = fabs(MOutVolLPF);	//출력전압 과전압(OBD전)
	if (Temp > MOutVolSWOVF){				
		MOutVolSWOVF_Counter++;
		if(MOutVolSWOVF_Counter > 100){//OVF레벨보다 큰값이 100번이상(100*66usec = 6.6msec) 지속되면 이면 폴트처리
			LowFaultAck = 1;	
			sbit(FSWF,SWF8_OVOVF);
			MOutVolSWOVF_Counter = 0;
		}
		if(Temp > MOutVolSWOVF+12){//OVF+10V보다 크면  즉시 폴트처리
			LowFaultAck = 1;	
			sbit(FSWF,SWF8_OVOVF);
			MOutVolSWOVF_Counter = 0;
		}
	}
	else{
		MOutVolSWOVF_Counter = 0;
	}
//---최종출력전압(배터리측) 과전압 검사	
	Temp = fabs(MBatVolLPF);	//출력전압 과전압(OBD전)
	if (Temp > MBatVolSWOVF){
		MBatVolSWOVF_Counter++;
		if(MBatVolSWOVF_Counter > 100){//OVF레벨보다 큰값이 100번이상(100*66usec = 6.6msec) 지속되면 이면 폴트처리
			LowFaultAck = 1;	
			sbit(FSWF,SWF9_BVOVF);
			MBatVolSWOVF_Counter = 0;
		}
		if(Temp > MBatVolSWOVF+12){//OVF+10V보다 크면  즉시 폴트처리
			LowFaultAck = 1;	
			sbit(FSWF,SWF9_BVOVF);
			MBatVolSWOVF_Counter = 0;
		}
	}
	else{
		MBatVolSWOVF_Counter = 0;
	}	


//===20160203 입력과전압 체크 로직변경(딜레이줌)
//===1편성 운행중 입력과전압 폴트발생, START전 입력전압 940V까지 상승하고 920V근처에서 START신호가 들어옴
//===기존 : 항상 입력과전압 폴트검지
//===변경 : START신호가 들어오면 START신호(BCSTARTAck)를 0.3초후에 인식하고, BCSTARTAck가 인식되면 입력과전압 체크
//==----->평상시에는 입력과전압 검지안하는 것임
//====================================================================
//---입력전압 과전압 검사(1편성 출고시)
	//Temp = fabs(MInputVolLPF);	//모듈입력전압 --> ADCRD10
	//if (Temp > MInputVolSWOVF){
	//	LowFaultAck = 1;	// Low Level 폴트로 처리 --> 조건 해제시 무한 재기동
	//	sbit(FSWF,SWF10_IVOVF);
	//}
//---입력전압 과전압 검사(20160203 력珦驩� 체크 로직변경(딜레이줌)	
	//if(BCSTARTAck == YES){
	//	Temp = fabs(MInputVolLPF);	//모듈입력전압 --> ADCRD10
	//	if (Temp > MInputVolSWOVF){
	//		LowFaultAck = 1;	// Low Level 폴트로 처리 --> 조건 해제시 무한 재기동
	//		sbit(FSWF,SWF10_IVOVF);
	//	}
	//}
//===20160324 입력과전압 체크 로직변경(수서꺼 문제문제되어 과전압 로직 추가보완)
//===수서편성 운행중 입력과전압 폴트발생-->로템 추가보완요청
//===기존 : START신호가 들어오면 START신호(BCSTARTAck)를 0.3초후에 인식하고, BCSTARTAck 인식되면 입력과전압 체크
//==-------->평상시에는 입력과전압 검지안하는 것임
//===변경 : 기동조건(590V~810V)만족되면 기동시작 --입력과전압폴트검지는 아직 안함
//          START신호가 들어오면 START신호(BCSTARTAck)를 0.3초후에 인식하고, BCSTARTAck가 인식되면 기동시작
//          CHK동작시켜 동작확인되면 입력과전압 체크
//====================================================================	
//---입력전압 과전압 검사(20160324 입력과전압 체크 로직변경(추가 보완)	

	if((SEQ_Step >= SEQ_LK_ON)&&(BCSTARTAck == YES)){//CHKA확인되면 SEQ_Step이 SEQ_LK_ON보다 크거나 같으므로 입력과전압 체크함
		Temp = fabs(MInputVolLPF);	//모듈입력전압 --> ADCRD10
		if (Temp > MInputVolSWOVF){
			LowFaultAck = 1;	// Low Level 폴트로 처리 --> 조건 해제시 무한 재기동
			sbit(FSWF,SWF10_IVOVF);
		
		}
	}	
	
			
	
//---DCLINK전압 과전압 검사	
	Temp = fabs(MDcLinkVolLPF);	//모듈 DCLINK전압(RATED 670Vdc) --> ADCRD11
	if (Temp > MDclinkVolSWOVF){
		LowFaultAck = 1;	// Low Level 폴트로 처리 --> 조건 해제시 무한 재기동
		sbit(FSWF,SWF11_FCOVF);
	}
	
//---저전압 검사	

	if((SEQ_Step == SEQ_Normal_State)&&(MODEAck == NORMALMODE1)){
		//Temp = fabs(MInputVolLPF);	//모듈입력전압 --> ADCRD10
		//if (Temp < MInputVolUVF){	//620.0;		//상세설계서 시험기준 620V
		//	LowFaultAck = 1;	// Low Level 폴트로 처리 --> 조건 해제시 무한 재기동
		//	sbit(FSWF,SWF13_IVUVF);
		//}
//======입력저전압 처리 변경 20160129=========================================
//======1초동안 계속 입력저전압이 유지되면 폴트처리로 변경
		Temp = fabs(MInputVolLPF);	//모듈입력전압 --> ADCRD10
		if (Temp < MInputVolUVF){	//620.0;		//상세설계서 시험기준 620V
			MInputVolUVF_Counter++;
			if(MInputVolUVF_Counter>15000){	// 66.6667usec * 15000 = 1sec
				MInputVolUVF_Counter = 0;
				LowFaultAck = 1;	// Low Level 폴트로 처리 --> 조건 해제시 무한 재기동
				sbit(FSWF,SWF13_IVUVF);
			}
		}
		else{
			MInputVolUVF_Counter = 0;
		}
/*
		//---출력저전압은 PWM ON시만 검사 ---> 부하시 STOP했을때 저전압 검지 방지목적
		if(PWM_FLAG == YES){
			MOutVolSWUVF_Counter++;
			//if(MOutVolSWUVF_Counter>20000){
			//	MOutVolSWUVF_Counter = 20001;
			if(MOutVolSWUVF_Counter>50000){//20160211->20000(1.3sec)-->50000(3.33초)
				MOutVolSWUVF_Counter = 0;
				Temp = fabs(MBatVolLPF);	//출력전압 저전압(OBD후)
				if (Temp < MBatVolUVF){	//50.0;		//상세설계서 시험기준 50V
					LowFaultAck = 1;	// Low Level 폴트로 처리 --> 조건 해제시 무한 재기동
					//sbit(FSWF,SWF12_OVUVF);
					sbit(FSWF,SWF15_OVUVF2);//20150922 최종출력저전압만 별도로 bit15에 할당-->모니터링프로그램에서 최종출력전압저전압 폴트표시하게
				}
				Temp = fabs(MOutVolLPF);	//출력전압 저전압(OBD전)
				if (Temp < MOutVolUVF){	//50.0;		//상세설계서 시험기준 50V
					LowFaultAck = 1;	// Low Level 폴트로 처리 --> 조건 해제시 무한 재기동
					sbit(FSWF,SWF12_OVUVF);
				}
			}
		}
		else{
			MOutVolSWUVF_Counter = 0;
		}	
*/		

		//---출력저전압은 PWM ON시만 검사 ---> 부하시 STOP했을때 저전압 검지 방지목적
		if(PWM_FLAG == YES){
			MOutVolSWUVF_Counter++;
			if(MOutVolSWUVF_Counter>50000){//20160211->20000(1.3sec)-->50000(3.33초)
				MOutVolSWUVF_Counter = 50001;
				Temp = fabs(MBatVolLPF);	//출력전압 저전압(OBD후)
				if (Temp < MBatVolUVF){	//50.0;		//상세설계서 시험기준 50V
					MOutVolSWUVF_Counter2++;
					if(MOutVolSWUVF_Counter2>5000){	// 66.6667usec * 7500 = 0.5sec
						MOutVolSWUVF_Counter2 = 0;
						LowFaultAck = 1;	// Low Level 폴트로 처리 --> 조건 해제시 무한 재기동
						sbit(FSWF,SWF15_OVUVF2);//20150922 최종출력저전압만 별도로 bit15에 할당-->모니터링프로그램에서 최종출력전압저전압 폴트표시하게
					}
				}
				else{
					MOutVolSWUVF_Counter2 = 0;
				}
				Temp = fabs(MOutVolLPF);	//출력전압 저전압(OBD전)
				if (Temp < MOutVolUVF){	//50.0;		//상세설계서 시험기준 50V
					MOutVolSWUVF_Counter1++;
					if(MOutVolSWUVF_Counter1>5000){	// 66.6667usec * 75000 = 1sec
						MOutVolSWUVF_Counter1 = 0;
						LowFaultAck = 1;	// Low Level 폴트로 처리 --> 조건 해제시 무한 재기동
						sbit(FSWF,SWF12_OVUVF);//20150922 최종출력저전압만 별도로 bit15에 할당-->모니터링프로그램에서 최종출력전압저전압 폴트표시하게
					}
				}
				else{
					MOutVolSWUVF_Counter1 = 0;
				}
			}
		}
		else{
			MOutVolSWUVF_Counter = 0;
			MOutVolSWUVF_Counter1 = 0;
			MOutVolSWUVF_Counter2 = 0;
		}				
			
	}	


	//----SW Fault가 하나라도 있으면 -- EPLD로 SW Fault를 출력하여 폴트래치시킴----//
	if(FSWF > 0){
	//	FAULTGENSW(); // 또는 R_DSP_SW_FAULT_CS() = 0x00;
	}

	if(BBatCur2LPF_max < BBatCur2LPF) BBatCur2LPF_max = BBatCur2LPF;
	if(BBatCur1LPF_max < BBatCur1LPF) BBatCur1LPF_max = BBatCur1LPF;

}

//==================================================================================//
// 함수명 : 폴트 테스트용 ()														//
//----------------------------------------------------------------------------------//
// DATE  : 2015. 4. 28																//
//==================================================================================//
void Check_TESTF()
{
	if(btst(MCUBD_DIPSW_IN,1)){//중간레벨(3분3회) 고장 테스트용-- GDF1
		//FaultTest_flag = 1;
		FGDF = 1;
		sbit(MidFaultAck, MidGDF);		// 중간레벨(3분3회) - GDF1
	}
	if(btst(MCUBD_DIPSW_IN,0)){//중고장 테스트용-- iocF
		MidFaultMJF = 1;
		sbit(FSWF,SWF0_ICOCF);
	}
}
//==================================================================================//
// 함수명 : GDU_FaultProcess()														//
//----------------------------------------------------------------------------------//
// DATE  : 2015. 1. 7																//
//==================================================================================//
void Check_GDF()
{
	FGDF = CS_RD_GDF() & 0x03; 

	if(btst(FGDF,GDF1)){			//(dcdc-Leading Leg)
		sbit(MidFaultAck, MidGDF);		// 중간레벨(3분3회) - GDF
	}
	
	if(btst(FGDF,GDF2)){			//(dcdc-Lagging Leg)
		sbit(MidFaultAck, MidGDF);		// 중간레벨(3분3회) - GDF
	}	

}

//==========20160229---차량에서 배터리휴즈폴트 배선 수정안해서---------
void Check_BXF()
{
	unsigned int ModuleFault_NUM_temp = 0;	

	//====배터리 과전류(외부입력) 체크
	//===외부 배터리 휴즈폴트 로직처리를 딥스위치 bit3으로 판단하게
	//===결론은 딥스위치 로직에 따라 휴즈폴트 로직 변경하게 함
	if(!btst(MCUBD_DIPSW_IN,3)){//===dipsw.3이 L면 --->기존 객차와 같이
//============================
		if(WGBC_TC0PC1 == WGBC_TC0){	// 객차용 축전지 충전기
			if(!btst(DioInPutBuf,BATOC1_Chk)){ // 20151117_BATOC1 노멀일 때 High로 수정 - 객차
				BATOC1Ack = NO; // 
			}
			else{
				BATOC1Ack = YES; //
				HighFaultAck = 1;
				HighFaultMJF = 1;
				sbit(FBXF, BOXF_BATOC1);
			}
			BATOC2Ack = NO;
			cbit(FBXF, BOXF_BATOC2);		
		}
		else
		{	// 동력차용 축전지 충전기
			if(!btst(DioInPutBuf,BATOC1_Chk))
			{ // 20160218_차량 로직 설계 변경으로 객차랑 동일하게 수정_BATOC1 노멀일 때 High
				BATOC1Ack = NO;
			}
			else
			{
				BATOC1Ack = YES;
				HighFaultAck = 1;
				HighFaultMJF = 1;
				sbit(FBXF, BOXF_BATOC1);
			}
			if(!btst(DioInPutBuf,BATOC2_Chk))
			{ // 20160218_차량 로직 설계 변경으로 객차랑 동일하게 수정_BATOC2 노멀일 때 High
				BATOC2Ack = NO;
			}
			else
			{
				BATOC2Ack = YES;
				HighFaultAck = 1;
				HighFaultMJF = 1;
				sbit(FBXF, BOXF_BATOC2);
			}
		}
	}
	else
	{//===dipsw.3이 H면 --->기존 동차와 같이
		if(WGBC_TC0PC1 == WGBC_TC0)
		{	// 객차용 축전지 충전기
			if(btst(DioInPutBuf,BATOC1_Chk))
			{ // 20151117_BATOC1 노멀일 때 High로 수정 - 객차
				BATOC1Ack = NO; 
			}
			else
			{
				BATOC1Ack = YES; 
				HighFaultAck = 1;
				HighFaultMJF = 1;
				sbit(FBXF, BOXF_BATOC1);
			}
			BATOC2Ack = NO;
			cbit(FBXF, BOXF_BATOC2);		
		}
		else{	// 동력차용 축전지 충전기
			if(btst(DioInPutBuf,BATOC1_Chk)){ // 20160218_차량 로직 설계 변경으로 객차랑 동일하게 수정_BATOC1 노멀일 때 High
				BATOC1Ack = NO;
			}
			else{
				BATOC1Ack = YES;
				HighFaultAck = 1;
				HighFaultMJF = 1;
				sbit(FBXF, BOXF_BATOC1);
			}
			if(btst(DioInPutBuf,BATOC2_Chk)){ // 20160218_차량 로직 설계 변경으로 객차랑 동일하게 수정_BATOC2 노멀일 때 High
				BATOC2Ack = NO;
			}
			else{
				BATOC2Ack = YES;
				HighFaultAck = 1;
				HighFaultMJF = 1;
				sbit(FBXF, BOXF_BATOC2);
			}
		}		
	}	
//=================================


				
	ModuleFault_NUM_temp = 0;

	if(WGBC_TC0PC1 == WGBC_TC0){	// 객차용 축전지 충전기
/*
		if(!btst(DioInPutBuf,M1F_Chk)){//bit4
			M1FAck = NO;
			sbit(BCBox_Status, 2);	// M1OK 모듈1 정상
			 MFAck_MY = NO;//20151112---
		}
		else{
			M1FAck = YES;
			cbit(BCBox_Status, 2);	// M1NG 모듈1 고장
			ModuleFault_NUM_temp++;
			if(WGBC_M_ID == WGBC_M_ID1) MFAck_MY = YES;
		}
*/		
		if(!btst(DioInPutBuf,M2F_Chk)){
			M2FAck = NO;
			sbit(BCBox_Status, 3);	// M2OK 모듈2 정상
			 MFAck_MY = NO;//20151112---
		}
		else{
			M2FAck = YES;
			cbit(BCBox_Status, 3);	// M2NG 모듈2 고장
			ModuleFault_NUM_temp++;
			if(WGBC_M_ID == WGBC_M_ID2) MFAck_MY = YES;
		}
		if(!btst(DioInPutBuf,M3F_Chk)){
			M3FAck = NO;
			sbit(BCBox_Status, 4);	// M3OK 모듈3 정상
			 MFAck_MY = NO;//20151112---
		}
		else{
			M3FAck = YES;
			cbit(BCBox_Status, 4);	// M3NG 모듈3 고장
			ModuleFault_NUM_temp++;
			if(WGBC_M_ID == WGBC_M_ID3) MFAck_MY = YES;
		}
		if(!btst(DioInPutBuf,M5F_Chk)){
			M5FAck = NO;
			sbit(BCBox_Status, 6);	// M5OK 모듈5 정상
			 MFAck_MY = NO;//20151112---
		}
		else{
			M5FAck = YES;
			cbit(BCBox_Status, 6);	// M5NG 모듈5 고장
			ModuleFault_NUM_temp++;
			if(WGBC_M_ID == WGBC_M_ID5) MFAck_MY = YES;
		}
		if(!btst(DioInPutBuf,M4F_Chk)){
			M4FAck = NO;
			sbit(BCBox_Status, 5);	// M4OK 모듈4 정상
			 MFAck_MY = NO;//20151112---
		}
		else{
			M4FAck = YES;
			cbit(BCBox_Status, 5);	// M4NG 모듈4 고장
			ModuleFault_NUM_temp++;
			if(WGBC_M_ID == WGBC_M_ID4) MFAck_MY = YES;
		}
		/*
		if(!btst(DioInPutBuf,M5F_Chk)){
			M5FAck = NO;
			sbit(BCBox_Status, 6);	// M5OK 모듈5 정상
			 MFAck_MY = NO;//20151112---
		}
		else{
			M5FAck = YES;
			cbit(BCBox_Status, 6);	// M5NG 모듈5 고장
			ModuleFault_NUM_temp++;
			if(WGBC_M_ID == WGBC_M_ID5) MFAck_MY = YES;
		}
		*/
		if(!btst(DioInPutBuf,M1F_Chk)){//bit4
			M1FAck = NO;
			sbit(BCBox_Status, 2);	// M1OK 모듈1 정상
			 MFAck_MY = NO;//20151112---
		}
		else{
			M1FAck = YES;
			cbit(BCBox_Status, 2);	// M1NG 모듈1 고장
			ModuleFault_NUM_temp++;
			if(WGBC_M_ID == WGBC_M_ID1) MFAck_MY = YES;
		}		
		
		ModuleFault_NUM = ModuleFault_NUM_temp;	

		/*---------*/
	//	if(MFAck_MY == NO){	// 20160811_모니터링 프로그램 상에서 BCMF 처리 오류 수정
			if(ModuleFault_NUM >= 2){	// 내폴트 아니고 폴트가 2개이상이면 --> 내꺼포함 전체모듈 출력차단	
				ModuleFault2ea_flag = YES;				
				PWM_FLAG = NO;
				PWM_OFF();					
			}
			else{						// 내폴트 아니고 폴트가 2개미만이면	--> 내꺼 재기동 가능하게...
				ModuleFault2ea_flag = NO; 	
			}
	//	}
/*		else{	// 내모듈 폴트면 
			if(ModuleFault_NUM >= 3){	// 내폴트 포함 폴트가 3개 이상이면 --> 내꺼 재기동 금지
				ModuleFault2ea_flag = YES;
				PWM_FLAG = NO;
				PWM_OFF();					
			}
			else{						// 내폴트이고 폴트가 3개미만이면	--> 내꺼 재기동 해야함  
				ModuleFault2ea_flag = NO;  	
			}
		}	// 20160811_모니터링 프로그램 상에서 BCMF 처리 오류 수정 */
		/*-----------*/
		//===20160323===bcstatus수정, BCOK(1), BCMJF(1)	
		if(ModuleFault2ea_flag == YES){
			cbit(BCBox_Status, 0);	// BCOK가 아님(0)		
			sbit(BCBox_Status, 1);	// BCMF(1)		
		}
		else{
			sbit(BCBox_Status, 0);	// BCOK임 (1)		
			cbit(BCBox_Status, 1);	// BCMF가 아님(0)			
		}
		//===20160323===bcstatus수정, BCOK(1), BCMJF(1)	
	
	}
	else{		//동력차 축전지 충전기
		if(!btst(DioInPutBuf,M1F_Chk)){
			M1FAck = NO;
			sbit(BCBox_Status, 2);	// M1OK 모듈1 정상
			 MFAck_MY = NO;//20151112---
		}
		else{
			M1FAck = YES;
			cbit(BCBox_Status, 2);	// M1NG 모듈1 고장
			ModuleFault_NUM_temp++;
//			if(WGBC_M_ID == WGBC_M_ID1) MFAck_MY = YES; // 20160811_모니터링 프로그램 상에서 BCMF 처리 오류 수정
		}
		if(!btst(DioInPutBuf,M2F_Chk)){
			M2FAck = NO;
			sbit(BCBox_Status, 3);	// M2OK 모듈2 정상
			 MFAck_MY = NO;//20151112---
		}
		else{
			M2FAck = YES;
			cbit(BCBox_Status, 3);	// M2NG 모듈2 고장
			ModuleFault_NUM_temp++;
//			if(WGBC_M_ID == WGBC_M_ID2) MFAck_MY = YES; // 20160811_모니터링 프로그램 상에서 BCMF 처리 오류 수정
		}
		if(!btst(DioInPutBuf,M3F_Chk)){
			M3FAck = NO;
			sbit(BCBox_Status, 4);	// M3OK 모듈3 정상
			 MFAck_MY = NO;//20151112---
		}
		else{
			M3FAck = YES;
			cbit(BCBox_Status, 4);	// M3NG 모듈3 고장
			ModuleFault_NUM_temp++;
//			if(WGBC_M_ID == WGBC_M_ID3) MFAck_MY = YES; // 20160811_모니터링 프로그램 상에서 BCMF 처리 오류 수정
		}
				
		ModuleFault_NUM = ModuleFault_NUM_temp;
	
		if(MFAck_MY == NO){	// 내모듈 폴트가 아니면  -->다른모듈 폴트
			if(ModuleFault_NUM >= 2){	// 내폴트 아니고 폴트가 2개이상이면 --> 내꺼포함 전체모듈 출력차단	
				ModuleFault2ea_flag = YES;				
				PWM_FLAG = NO;//20150908추가
				PWM_OFF();	//20150908추가
			}
			else{						// 내폴트 아니고 폴트가 2개미만이면	--> 내꺼 재기동 가능하게...
				ModuleFault2ea_flag = NO; 	
			}
		}
	/*	else
	   {	// 내모듈 폴트면 
	       if(ModuleFault_NUM >= 3){	// 내폴트 포함 폴트가 3개 이상이면 --> 내꺼 재기동 금지
		   ModuleFault2ea_flag = YES;				
	       PWM_FLAG = NO;//20150908추가
		   PWM_OFF();	//20150908추가			
	   }
			else
			{						// 내폴트이고 폴트가 3개미만이면	--> 내꺼 재기동 해야함  
				ModuleFault2ea_flag = NO;  	
			}
		}*/ // 20160811_모니터링 프로그램 상에서 BCMF 처리 오류 수정
		//===20160323===bcstatus수정, BCOK(1), BCMJF(1)	
		if(ModuleFault2ea_flag == YES)
		{
			cbit(BCBox_Status, 0);	// BCOK가 아님(0)		
			sbit(BCBox_Status, 1);	// BCMF(1)		
		}
		else
		{
			sbit(BCBox_Status, 0);	// BCOK임 (1)		
			cbit(BCBox_Status, 1);	// BCMF가 아님(0)			
		}
		//===20160323===bcstatus수정, BCOK(1), BCMJF(1)	
	}
}
	

	
//=====================	
void Check_BXF_old()
{
	unsigned int ModuleFault_NUM_temp = 0;	


	//====배터리 과전류(외부입력) 체크
	if(WGBC_TC0PC1 == WGBC_TC0)
	{	// 객차용 축전지 충전기
		if(!btst(DioInPutBuf,BATOC1_Chk)){ // 20151117_BATOC1 노멀일 때 High로 수정 - 객차
			BATOC1Ack = NO;  
			
		}
		else{
			BATOC1Ack = YES; 
			HighFaultAck = 1;
			HighFaultMJF = 1;
			sbit(FBXF, BOXF_BATOC1);
			
		}
		BATOC2Ack = NO;
		cbit(FBXF, BOXF_BATOC2);		
	}
	else{	// 동력차용 축전지 충전기
		if(!btst(DioInPutBuf,BATOC1_Chk)){ // 20160218_차량 로직 설계 변경으로 객차랑 동일하게 수정_BATOC1 노멀일 때 High
			BATOC1Ack = NO;
		}
		else{
			BATOC1Ack = YES;
			HighFaultAck = 1;
			HighFaultMJF = 1;
			sbit(FBXF, BOXF_BATOC1);
		}
		if(!btst(DioInPutBuf,BATOC2_Chk)){ // 20160218_차량 로직 설계 변경으로 객차랑 동일하게 수정_BATOC2 노멀일 때 High
			BATOC2Ack = NO;
		}
		else{
			BATOC2Ack = YES;
			HighFaultAck = 1;
			HighFaultMJF = 1;
			sbit(FBXF, BOXF_BATOC2);
		}
	}	
				
	ModuleFault_NUM_temp = 0;

	if(WGBC_TC0PC1 == WGBC_TC0){	// 객차용 축전지 충전기
/*
		if(!btst(DioInPutBuf,M1F_Chk)){//bit4
			M1FAck = NO;
			sbit(BCBox_Status, 2);	// M1OK 모듈1 정상
			 MFAck_MY = NO;//20151112---
		}
		else{
			M1FAck = YES;
			cbit(BCBox_Status, 2);	// M1NG 모듈1 고장
			ModuleFault_NUM_temp++;
			if(WGBC_M_ID == WGBC_M_ID1) MFAck_MY = YES;
		}
*/		
		if(!btst(DioInPutBuf,M2F_Chk)){
			M2FAck = NO;
			sbit(BCBox_Status, 3);	// M2OK 모듈2 정상
			 MFAck_MY = NO;//20151112---
		}
		else{
			M2FAck = YES;
			cbit(BCBox_Status, 3);	// M2NG 모듈2 고장
			ModuleFault_NUM_temp++;
			if(WGBC_M_ID == WGBC_M_ID2) MFAck_MY = YES;
		}
		if(!btst(DioInPutBuf,M3F_Chk)){
			M3FAck = NO;
			sbit(BCBox_Status, 4);	// M3OK 모듈3 정상
			 MFAck_MY = NO;//20151112---
		}
		else{
			M3FAck = YES;
			cbit(BCBox_Status, 4);	// M3NG 모듈3 고장
			ModuleFault_NUM_temp++;
			if(WGBC_M_ID == WGBC_M_ID3) MFAck_MY = YES;
		}
		if(!btst(DioInPutBuf,M5F_Chk)){
			M5FAck = NO;
			sbit(BCBox_Status, 6);	// M5OK 모듈5 정상
			 MFAck_MY = NO;//20151112---
		}
		else{
			M5FAck = YES;
			cbit(BCBox_Status, 6);	// M5NG 모듈5 고장
			ModuleFault_NUM_temp++;
			if(WGBC_M_ID == WGBC_M_ID5) MFAck_MY = YES;
		}
		if(!btst(DioInPutBuf,M4F_Chk)){
			M4FAck = NO;
			sbit(BCBox_Status, 5);	// M4OK 모듈4 정상
			 MFAck_MY = NO;//20151112---
		}
		else{
			M4FAck = YES;
			cbit(BCBox_Status, 5);	// M4NG 모듈4 資�
			ModuleFault_NUM_temp++;
			if(WGBC_M_ID == WGBC_M_ID4) MFAck_MY = YES;
		}
		/*
		if(!btst(DioInPutBuf,M5F_Chk)){
			M5FAck = NO;
			sbit(BCBox_Status, 6);	// M5OK 모듈5 정상
			 MFAck_MY = NO;//20151112---
		}
		else{
			M5FAck = YES;
			cbit(BCBox_Status, 6);	// M5NG 모듈5 고장
			ModuleFault_NUM_temp++;
			if(WGBC_M_ID == WGBC_M_ID5) MFAck_MY = YES;
		}
		*/
		if(!btst(DioInPutBuf,M1F_Chk))
		{//bit4
			M1FAck = NO;
			sbit(BCBox_Status, 2);	// M1OK 모듈1 정상
			 MFAck_MY = NO;//20151112---
		}
		else
		{
			M1FAck = YES;
			cbit(BCBox_Status, 2);	// M1NG 모듈1 고장
			ModuleFault_NUM_temp++;
			if(WGBC_M_ID == WGBC_M_ID1) MFAck_MY = YES;
		}		
		

		ModuleFault_NUM = ModuleFault_NUM_temp;	

		/*---------*/
		if(MFAck_MY == NO)
		{	// 내모듈 폴트가 아니면  -->다른모듈 폴트
			if(ModuleFault_NUM >= 2){	// 내폴트 아니고 폴트가 2개이상이면 --> 내꺼포함 전체모듈 출력차단	
				ModuleFault2ea_flag = YES;				
				PWM_FLAG = NO;
				PWM_OFF();					
			}
			else
			{						// 내폴트 아니고 폴트가 2개미만이면	--> 내꺼 재기동 가능하게...
				ModuleFault2ea_flag = NO; 	
			}
		}
		else
		{	// 내모듈 폴트면 
			if(ModuleFault_NUM >= 3)
			{	// 내폴트 포함 폴트가 3개 이상이면 --> 내꺼 재기동 금지
				ModuleFault2ea_flag = YES;
				PWM_FLAG = NO;
				PWM_OFF();					
			}
			else
			{						// 내폴트이고 폴트가 3개미만이면	--> 내꺼 재기동 해야함  
				ModuleFault2ea_flag = NO;  	
			}
		}	
		/*-----------*/	
	}
	else
	{		//동력차 축전지 충전기
		if(!btst(DioInPutBuf,M1F_Chk))
		{
			M1FAck = NO;
			sbit(BCBox_Status, 2);	// M1OK 모듈1 정상
			 MFAck_MY = NO;//20151112---
		}
		else
		{
			M1FAck = YES;
			cbit(BCBox_Status, 2);	// M1NG 모듈1 고장
			ModuleFault_NUM_temp++;
			if(WGBC_M_ID == WGBC_M_ID1) MFAck_MY = YES;
		}
		if(!btst(DioInPutBuf,M2F_Chk))
		{
			M2FAck = NO;
			sbit(BCBox_Status, 3);	// M2OK 모듈2 정상
			 MFAck_MY = NO;//20151112---
		}
		else
		{
			M2FAck = YES;
			cbit(BCBox_Status, 3);	// M2NG 모듈2 고장
			ModuleFault_NUM_temp++;
			if(WGBC_M_ID == WGBC_M_ID2) MFAck_MY = YES;
		}
		if(!btst(DioInPutBuf,M3F_Chk))
		{
			M3FAck = NO;
			sbit(BCBox_Status, 4);	// M3OK 모듈3 정상
			 MFAck_MY = NO;//20151112---
		}
		else
		{
			M3FAck = YES;
			cbit(BCBox_Status, 4);	// M3NG 모듈3 고장
			ModuleFault_NUM_temp++;
			if(WGBC_M_ID == WGBC_M_ID3) MFAck_MY = YES;
		}
				
		ModuleFault_NUM = ModuleFault_NUM_temp;
	
		if(MFAck_MY == NO)
		{	// 내모듈 폴트가 아니면  -->다른모듈 폴트
			if(ModuleFault_NUM >= 2){	// 내폴트 아니고 폴트가 2개이상이면 --> 내꺼포함 전체모듈 출력차단	
				ModuleFault2ea_flag = YES;				
				PWM_FLAG = NO;//20150908추가
				PWM_OFF();	//20150908추가
			}
			else
			{						// 내폴트 아니고 폴트가 2개미만이면	--> 내꺼 재기동 가능하게...
				ModuleFault2ea_flag = NO; 	
			}
		}
		else
		{	// 내모듈 폴트면 
			if(ModuleFault_NUM >= 3)
			{	// 내폴트 포함 폴트가 3개 이상이면 --> 내꺼 재기동 금지
				ModuleFault2ea_flag = YES;				
				PWM_FLAG = NO;//20150908추가
				PWM_OFF();	//20150908추가			
			}
			else
			{						// 내폴트이고 폴트가 3개미만이면	--> 내꺼 재기동 해야함  
				ModuleFault2ea_flag = NO;  	
			}
		}
	}
}

//===============================================================//
// 함수명 : ETC_FaultProcess()														
// 기  능 : 온도폴트, 휴즈폴트등 처리									
//---------------------------------------------------------------//
void Check_ETF()
{
//////////220122 suhjinho 출력저전류 추가    220325  삭제(전자파 시험 대비)   220510 재추가(성능 재시험 대비)
	if( (SEQ_Step == SEQ_Normal_State) && (MODEAck == NORMALMODE1) && (BOutCurLPF < 10) && (MBatVolLPF > 80)
	&&((MCUBD_HEXSW_IN == 5)||(MCUBD_HEXSW_IN == 6)||(MCUBD_HEXSW_IN == 7)) )
	{//20170713 출력전류 저전류 폴트
		MBatCurNC_Counter++;
		if(MBatCurNC_Counter > 4500000)			//1sec 15000
		{//5min : 4500000
			sbit(FETF,ETCF_NCARGING);
			HighFaultAck = 1;
			HighFaultMJF = 1;
		}
	}
	else
	{
		MBatCurNC_Counter = 0;
	}


	if(btst(FETF,ETCF_FUSEF)){//==휴즈폴트는 시퀸스에서 판단, 중고장처리, 강제리셋 불가능
		HighFaultAck = 1;
		HighFaultMJF = 1;
	}
	
	if(btst(FSQF,SEQF_LKa)){	//==LK폴트는 중고장처리, 강제리셋 불가능
		HighFaultAck = 1;
		HighFaultMJF = 1;
	}
	if(btst(FSQF,SEQF_CHKa)){	//==CHK폴트는 중고장처리, 강제리셋 불가능
		HighFaultAck = 1;
		HighFaultMJF = 1;
	}

	//==과온폴트
	if(!TEMP85SWAck){// 방열판 온도스위치 2개 직렬로 연결하여 입력(B접점)
		THRFAck_Counter++;			
		if(THRFAck_Counter > 900000){	// 0.5sec*120 = 1min
			THRFAck_Counter = 0;
			LowFaultAck = 1;	// Low Level 폴트로 처리 --> 조건 해제시 무한 재기동
			sbit(FETF, ETCF_HSKTHRF);	// 방열판 온도센서 1,2 폴트처리
		}
	}
	else{
		THRFAck_Counter = 0;
	}

	
	if((FANF1Ack)&&(FanOnOffCmd_flag == 1)){
		FANF1Ack_Counter++;// 팬폴트1 10초
		if(FANF1Ack_Counter > CtrTimer_10s){
			FANF1Ack_Counter = 0;
			LowFaultAck = 1;	// Low Level 폴트로 처리 --> 조건 해제시 무한 재기동
			sbit(FETF, ETCF_FANF1);	// 방열판 온도센서 1,2 폴트처리
		}
	}
	else{
		FANF1Ack_Counter = 0;
	}
	if((FANF2Ack)&&(FanOnOffCmd_flag == 1)){
		FANF2Ack_Counter++;// 팬폴트1 10초
		if(FANF2Ack_Counter > CtrTimer_10s){
			FANF2Ack_Counter = 0;
			LowFaultAck = 1;	// Low Level 폴트로 처리 --> 조건 해제시 무한 재기동
			sbit(FETF, ETCF_FANF2);	// 방열판 온도센서 1,2 폴트처리
		}
	}
	else{
		FANF2Ack_Counter = 0;
	}
}



//==================================================================================//
// 함수명 : HW_FaultProcess()														//
//----------------------------------------------------------------------------------//
// DATE  : 2015. 01. 07																//
//==================================================================================//
void Check_HWF()
{
	unsigned int buf1 = 0;
	unsigned int buf2 = 0;

	//---HW FAULT
	buf1 = CS_RD_HWF1() & 0xff;// 
	buf2 = CS_RD_HWF2() & 0x1F;// 
	//buf2 = CS_RD_HWF2() & 0x0F;// 
	FHWF = ((buf2<<8)& 0xff00)| buf1;


	if(btst(FHWF,HWF0_ICOCF)){	//입력과전류--> 3분3회중고장, 강제리셋 가능
		sbit(MidFaultAck, MidMICOCF);	
	}
	if(btst(FHWF,HWF1_MBATOCF)){//모듈 충전전류 과전류--> 조건 해제시 무한 재기동
		LowFaultAck = 1;
	}
	if(btst(FHWF,HWF2_MOCF)){	//모듈 출력전류 과전류--> 3분3회중고장, 강제리셋 가능
		sbit(MidFaultAck, MidMOCOCF);	
	}
	if(btst(FHWF,HWF4_BBAT1OCF)){//BOX 충전전류1 과전류--> 조건 해제시 무한 재기동
		LowFaultAck = 1;
	}
	if(btst(FHWF,HWF5_BBAT2OCF)){//BOX 충전전류2 과전류--> 조건 해제시 무한 재기동
		LowFaultAck = 1;
	}
	if(btst(FHWF,HWF6_BOCF)){//BOX 출력전류 과전류--> 조건 해제시 무한 재기동
		LowFaultAck = 1;
	}
	if(btst(FHWF,HWF7_CPOVF)){//제어전압 과전압--> 조건 해제시 무한 재기동 -----> ADC로 읽은값으로 저전압/과전압 둘다 해야함
		LowFaultAck = 1;
	}
	if(btst(FHWF,HWF8_OVOVF)){//출력전압 과전압(OBD전)--> 조건 해제시 무한 재기동
		LowFaultAck = 1;	
	}
	if(btst(FHWF,HWF9_BVOVF)){//출력전압 과전압(OBD후)--> 조건 해제시 무한 재기동
		LowFaultAck = 1;
	}
//===20160203 입력과전압 HWF임시로 사용안함
	//if(btst(FHWF,HWF10_IVOVF)){//입력과전압--> 조건 해제시 무한 재기동
	//	LowFaultAck = 1;
	//}
	if(btst(FHWF,HWF10_IVOVF)){//입력과전압--> 조건 해제시 무한 재기동
		FAULTLATCHCLEAR();
		cbit(FHWF,HWF10_IVOVF);	
	}
	if(btst(FHWF,HWF11_FCOVF)){//DCLINK과전압--> 조건 해제시 무한 재기동
		LowFaultAck = 1;
	}
}

//==================================================================================//
// 함수명 : FaultProcess() 인터늠�루틴에서 바로 차단 ==>테스트용				        //
// 기  능 : H/W, S/W , GDU, FAULT LEVEL SETTING									    //
//----------------------------------------------------------------------------------//
// CALLING : Nths_Seq.c                        										//
// DATE  : 2010. 12. 02	, 수정(20101207) : etc폴트 추가							//
//==================================================================================//
void FaultProcess_int()
{	
	intpin_Chk();//테스트용-->지울것
	
	Check_GDF();		//GDU FAULT 처리
	Check_HWF();		//HW FAULT 처리
	Check_SWF();		//센서 SW FAULT 처리
	Check_ETF();		//방열판온도,휴즈 폴트처리
	Check_BXF();		//다른모듈 및 차량 BOX 폴트
	Check_TESTF();		//임의로 폴트발생
	
	if((FGDF != 0)||(FHWF != 0) ||(FSWF != 0) ||(FSQF != 0) ||(FETF != 0) ||(FBXF != 0)){
		FaultAck = YES;
		//SWF_LatchGEN(); //소프트웨어로 하드웨어 폴트 발생하여 PWM차단
	
		if(FGDF != 0){	FLGDF = FGDF;FLGDF_1ST = FGDF;}
		if(FHWF != 0){	FLHWF = FHWF;FLHWF_1ST = FHWF;}
		if(FSWF != 0){	FLSWF = FSWF;FLSWF_1ST = FSWF;}
		if(FSQF != 0){	FLSQF = FSQF;FLSQF_1ST = FSQF;}
		if(FETF != 0){	FLETF = FETF;FLETF_1ST = FETF;}
		if(FBXF != 0){	FLBXF = FBXF;FLBXF_1ST = FBXF;}

	}

//====추가--중고장인 경우 인터럽트에서 바로 차단====
	if(FaultAck == YES){
		PWM_FLAG = NO;
		PWM_OFF();
		CHK_OFF();
		LK_OFF();
	}
	if(MajorFault == YES){	// 중고장인경우 
		PWM_FLAG = NO;
		PWM_OFF();
		BCFAULTOUT_ON();	// 모듈 폴트라고 릴레이보드로 출력(다른모듈로 정보공유, 차량)
		CHK_OFF();
		LK_OFF();
	}
	
	Reset_Sequence();

}

void SWF_LatchGEN()
{
	sbit(Epld_CtrlBuf,0); //active high
	CS_WR_CTRL() = Epld_CtrlBuf; 
}

void SWF_LatchCLR()
{
	cbit(Epld_CtrlBuf,0); 
	CS_WR_CTRL() = Epld_CtrlBuf; 
}
 
//==================================================================================//
// 함수명 : _FaultClear()													//
// 기  능 : H/W, S/W, GDU FAULT Clear											//
//----------------------------------------------------------------------------------//
// DATE  : 2015. 01. 10																//
//==================================================================================//
void FaultClear()
{	
	SWF_LatchCLR();
	SWF_LatchGEN();
	SWF_LatchCLR();	
	
	FAULTLATCHCLEAR();
	
//	BCOKOUT_OFF();

	FaultAck = NO;
	//MFAck_MY = NO;		
	MidFaultAck = 0;
	LowFaultAck = 0;
	MajorFault = 0;
	MidFaultMJF = 0;
	HighFaultMJF = 0;
	
		ModuleFault2ea_flag = NO;//20150521
	FGDF = 0x00;	
	FHWF = 0x00;
	FSWF = 0x00;
	FSQF = 0x00;
	FETF = 0x00;	
	FBXF = 0x00;	
	
	FLGDF = FGDF;
	FLHWF = FHWF;
	FLSWF = FSWF;
	FLSQF = FSQF;
	FLETF = FETF;
	FLBXF = FBXF;  

}

//==============================================================================//
// FCounter : fault 횟수
// FTtimer1 : 첫번째 폴트후 경과시간
// FTtimer2 : 두번째 폴트후 경과시간
// Fmjf : 중고장발생(1), 중고장 아님(0)
//==============================================================================//
void MidiumFault_3min3N_Check_sub(Uint16 *FCounter, Uint16 *FTimer1, Uint16 *FTimer2, Uint16 *Fmjf)
{
	unsigned int fcnt, ftmr1, ftmr2, fmjff;
	//unsigned int MIN3Temp = 6000;	
	unsigned int MIN3Temp = 18000;	
	
	fcnt = *FCounter;
	ftmr1 = *FTimer1;
	ftmr2 = *FTimer2;
	fmjff = *Fmjf;
	
	if(fcnt == 1){//폴트가 1회면
		ftmr1++;
		if(ftmr1 >= MIN3Temp){	// 폴트가 1번걸렸는데,,,첫번째 폴트발생후 3분이 지났으면 중고장 처리 안하고, 폴트누적 클리어
			ftmr1 = 0;	//폴트타이머 클리어
			fcnt = 0;	// 폴트카운트 하나 감소	
		}
	}
	if(fcnt == 2){//폴트가 2회면
		ftmr1++;
		ftmr2++;		
		if(ftmr1 >= MIN3Temp){	// 폴트가 2번걸렸는데,,,첫번째 폴트발생후 3분이 지났으면 중고장 아니고, 폴트누적 1감소
			ftmr1 = ftmr2;	//
			fcnt = 1;	// 폴트카운트 하나 감소		
			ftmr2 = 0;	//폴트타이머2 클리어
		}
	}		
	if(fcnt == 3){//폴트가 3회면 중고장 검사
		ftmr1++;
		ftmr2++;		
		if(ftmr1 >= MIN3Temp){	// 폴트가 2번걸렸는데,,,첫번째 폴트발생후 3분이 지났으면 중고장 아니고, 폴트㈏� 1감소
			ftmr1 = ftmr2;	//
			fcnt = 2;	// 폴트카운트 하나 감소		
			ftmr2 = 0;	//폴트타이머2 클리어
		}
		else{		//폴트 3회� 첫번째 폴트발생후  3분미만이면 중고장 처리
			ftmr1 = 0;	//폴트타이머 클리어	
			ftmr2 = 0;	//폴트타이머 클리어		
			fmjff = 1;	// 중고장 발생					
		}
	}	
	
	*FCounter = fcnt;
	*FTimer1 = ftmr1;
	*FTimer2 = ftmr2;
	*Fmjf = fmjff;
			
}

void MidiumFault_3min3N_Check()
{
	MidiumFault_3min3N_Check_sub(&MidFCounter0, &MidFTimer01, &MidFTimer02, &MidFaultMJF);
	MidiumFault_3min3N_Check_sub(&MidFCounter1, &MidFTimer11, &MidFTimer12, &MidFaultMJF);
	MidiumFault_3min3N_Check_sub(&MidFCounter2, &MidFTimer21, &MidFTimer22, &MidFaultMJF);

}
void MidiumFault_Count_INC()
{
	if(btst(MidFaultAck,MidGDF)){		// BIT 0, GDFs 폴트 3분3회시 중고장 처리  -->리셋시 재기동 불가		
		MidFCounter0++;
	}
	if(btst(MidFaultAck,MidMICOCF)){	// BIT 2, 입력과전류 폴트 3분3회 -->리셋시 재기동 가능 
		MidFCounter1++;
	}
	if(btst(MidFaultAck,MidMOCOCF)){	// BIT 4, 모듈 출력과전류 폴트 3분3회 -->리셋시 재기동 가능
		MidFCounter2++;
	}
}

void MidiumFault_3min3N_INIT()
{
	//==3분3회 동일고장 폴트처리 테스트변수===
	MidFaultAck = 0;
	LowFaultAck = 0;
	HighFaultAck = 0;//

	HighFaultMJF = 0;
	MidFaultMJF = 0;

	MidFCounter0 = 0;	//GDF1,2 발생횟수 
	MidFTimer01 = 0;		//GDFs 첫번째 경과시간  
	MidFTimer02 = 0;		//GDFs 두번째 경과시간  

	MidFCounter1 = 0;	//입력과전류 폴트 발생횟수
	MidFTimer11 = 0;		//입력과전류 폴트 첫번째 경과시간 
	MidFTimer12 = 0;		//입력과전류 폴트 두번째 경과시간  

	MidFCounter2 = 0;	//모듈출력과전류 폴트 발생횟수
	MidFTimer21 = 0;		//모듈출력과전류 폴트 첫번째 경과시간  
	MidFTimer22 = 0;		//입력과모듈출력과전류전류 폴트 두번째 경과시간  

	
}
