//================================================================================//
// 원강선 BC 제어프로그램    		//
//================================================================================//
// DCDC컨버터  																
// Rated Input Voltage : dc 670v										
// Rated output DcLink Voltage : 720vdc~83Vdc    									
// fsw = 15khz, 제어주기 = 15khz(66usec), 스위칭주파수와 제어주기 동기화               												
// DATE  : 2015. 01. 02            						  by PACTECH R&D LSH		
//================================================================================//
// 제어기 보드 -- 원강선 MCU                                                		
//================================================================================//
// pwm 세팅 															
// EPWM 1,2 사용(3,4는 원강선에서 사용안함
// fsw = 15khz, 제어주기 = 15khz(66usec), 제어주기 1번마다 pwm업데이트            
//================================================================================//

#include "Global_Variable_Function.h"
//================================================================================//
// 스위칭 방식 : PSFB(Phase Shift Full Bridge) switching 
// 스위칭 주파수 : 15khz
//================================================================================//
// 트랜스포머 사양                                설계:팩테크, 제작 : (코아테크)    
//================================================================================//
// 용량 : 11kva                                               //
// 턴수비 = N1/N2 = V1/V2 = 20:4:4                     //
//================================================================================//
// 2차측 리액터 사양                       설계:팩테크, 제작 : (코아테크)         
// 용량 : 140A(204A), 90uH                                            
//=======파워회로/PWM==============================================================
// FB(Full-Bridge) phase shift dc/dc컨버터 zvzcs--
// PWM1A(Q1A), PWM1B(Q1B), PWM2A(Q2A), PWM2B(Q2B)
//================================================================================//

//---병렬운전 관련 임시 변수들
//float Pall_dV = 0.001;
float Pall_dV = 0.0001;
//float UnbalCur_Err = 0.0;


float UnbalCur_ErrMinMax = 0.5;//토탈전류평균값과 모듈전류 오차

float Unbal_I_Kp = 0.20;
float Unbal_I_Ki = 0.2;

//float Vcontrol_Max = 120.;
float Vcontrol_Max = 125.;
//float BBatCurLimit = 10.0;
//==박스/모듈간 출력전류 언발란스
float Unbal_dVmax = 1.5;
float Unbal_dVmin = -1.5;
//float Unbal_dVmin = -11.5;//리튬폴리머 배터리 튜닝, -1.5V-->-7.5V-->-11.5V
float Unbal_incdV = 0.0001;
float UnbalVSet = 0.0;
float UnbalVSetCNT = 0.0;
//==충전전류 제한시 박스/모듈간 출력전류 언발란스 보정
//float Unbal_dVmax_sub = 6.5;
//float Unbal_dVmin_sub = -6.5;
float Unbal_dVmax_sub = 10.0;

float Unbal_dVmin_sub = -10.0;
float Unbal_incdV_sub = 0.0001;
float UnbalVSet_sub = 0.0;

float OutCurI_Max = 0.0;
float OutCurI_old = 0.0;

float OutVolPI_iref = 0.0;
float OutCurRef_Tst = 0.0;

float MOutVolLPF_max = 0.0;
float MBatVolLPF_max = 0.0;

//Uint16 DutyCount_down = 200;
Uint16 DutyCount_down = 400;
Uint16 DutyCount_up = 300;
float DutyCount_up_limitV = 85.5;
float DutyCount_dn_limitV = 78.5;

float BBatCurRef_test = 60.0;

long Counter_CV_to_CC = 0;
long Counter_CC_to_CV = 0;
long Counter_CV_to_CC_delay = 10000.0;
long Counter_CC_to_CV_delay = 10000.0;

float BBatCur_Kp1 = 0.01;

//================================================================================//
// 1-1. 언발란스 병렬제어(박스전체전류평균값과 모듈전류차이를 이용) --->Vref가변
// 1-2. 충전전류제한(Boxbattery1,2 -->) --->Vref가변
// 1-3. 충전제한시 언발란스 보상
// 2. 전압제어(Vref-->pi전압제어기-->Iref)
// 3. 전류제어(Iref-->pi전류제어기-->OutCurPI-->pwmduty)
//===============================================================================//
void WGBC_DCDC_Controller_20150522()
{
	//====정상동작하는 모듈수량 계산
	if(WGBC_TC0PC1 == WGBC_TC0){	// 객차용 축전지 충전기
		Pall_OK_ModuleN = 5-ModuleFault_NUM;//5-비정상모듈수량;
		//OutCurRef = MOutCurRATED;
	}
	else{	// 동력차용 축전지 충전기
		Pall_OK_ModuleN = 3-ModuleFault_NUM;//5-비정상모듈수량;
	}
	//Pall_OK_ModuleN = 1;
	//==================================================================================
	//==1-1.병렬운전 언발란스 제어
	// (BOutCurLPF)/(N) > MOutCurLPF ---> MOutCurLPF감소 ---> Vref 감소
	// (BOutCurLPF)/(N) < MOutCurLPF ---> MOutCurLPF증가 ---> Vref 증가
	//==================================================================================
	UnbalCur_Err = (BOutCurLPF/(float)Pall_OK_ModuleN) - MOutCurLPF;//박스출력전류평균값-모듈출력전류
	if(DCDC_SoftStart_flag == NO){
		if(UnbalCur_Err > UnbalCur_ErrMinMax){	   // 전체출력전류 평균값이 모듈출력전류보다 크면 모듈출력감소
			UnbalVSet = UnbalVSet + Unbal_incdV;//Unbal_incdV = 0.0001[V]
			UnbalVSetCNT++;
			if(UnbalVSet > Unbal_dVmax) UnbalVSet = Unbal_dVmax;//Unbal_dVmax = 1.5
		}
		else if(UnbalCur_Err < -UnbalCur_ErrMinMax){   // 전체출력전류 평균값이 모듈출력전류보다 작으면 모듈출력증가
			UnbalVSet = UnbalVSet - Unbal_incdV;
			UnbalVSetCNT--;
			if(UnbalVSet < Unbal_dVmin) UnbalVSet = Unbal_dVmin;	//Unbal_dVmin = -1.5
		}
		else{
			UnbalVSet = UnbalVSet;
		}
	}			
	//==================================================================================
	//==1-2.충전전류 제한 제어
	//= 박스전체 충전전류로 제한. --->각 모듈충전전류로 제한할지는 고민
	//= 입력 : 충전전류제한값(객차 60A, 동력차 68A), 박스전체충전전류검출값(BAT1+BAT2)
	//= 출력 : 출력전압지령 삭감전압[V] --- BBatCurPI
	//==================================================================================
	//BBatCurRef = BBatCurRef_test;
	BBatCurErr = BBatCurRef - BBatCurLPF;//박스충전전류 제한 ---> OutVolRef 변경
	
//	if((fabs)(BBatCurErr) < 1.0)	BBatCur_Kp = BBatCur_Kp1;
//	if((fabs)(BBatCurErr) < 3.0)	BBatCur_Kp = 0.01;
//	if((fabs)(BBatCurErr) < 5.0)	BBatCur_Kp = 0.08;

//	if((fabs)(BBatCurErr) < 10.0)	BBatCur_Kp = 0.15;
//	if((fabs)(BBatCurErr) < 5.0)	BBatCur_Kp = 0.08;
//	if((fabs)(BBatCurErr) < 3.0)	BBatCur_Kp = 0.01;
//	if((fabs)(BBatCurErr) < 1.0)	BBatCur_Kp = BBatCur_Kp1;

//	if((fabs)(BBatCurErr) < 10.0)	BBatCur_Kp = 0.20;
//	if((fabs)(BBatCurErr) < 5.0)	BBatCur_Kp = 0.10;
//	if((fabs)(BBatCurErr) < 3.0)	BBatCur_Kp = 0.02;
//	if((fabs)(BBatCurErr) < 1.0)	BBatCur_Kp = BBatCur_Kp1;
	//if((fabs)(BBatCurErr) < 1.0)	BBatCur_Kp = BBatCur_Kp1;
		//BBatCur_Kp = BBatCur_Kp1;
		
		
//	if(BBatCurErr < -1.0)	BBatCur_Kp = 0.01;
//	if(BBatCurErr < -3.0)	BBatCur_Kp = 0.04;
//	if(BBatCurErr < -5.0)	BBatCur_Kp = 0.08;
//	if(BBatCurErr < -10.0)	BBatCur_Kp = 0.15;
		
	//if(BBatCurErr < -1.0)	BBatCur_Kp = 0.04;
	//if(BBatCurErr < -3.0)	BBatCur_Kp = 0.06;
	//if(BBatCurErr < -5.0)	BBatCur_Kp = 0.08;
	//if(BBatCurErr < -10.0)	BBatCur_Kp = 0.1;
	

	BBatCurP = BBatCur_Kp * BBatCurErr;
 	PI_Controller(BBatCurLimit,Ts,BBatCur_Kp,BBatCur_Ki,BBatCurErr,&BBatCurI,&BBatCurPI); 	//PI_Controller(BBatCurLimit,Ts,BBatCur_Kp,BBatCur_Ki,BBatCurErr,&BBatCurI,&BBatCurPI);
	if(BBatCurPI >= 0.0){//충전전류 제한상태 아님
		BBatCurPI = 0;
		BBatCurI = 0;
		//===========================================//
		UnbalVSet_sub = 0.0;
		//===========================================//
	}
	else if(BBatCurPI < -BBatCurLimit){//====>리미트는 전류값이 아니라 충전전류제한 제어기 출력인 전압제한값(BBatCurPI)에 대한 리미트임
		BBatCurPI = -BBatCurLimit;	//BBatCurPI = -BBatCurLimit;
	}
	else{	//충전전류 제한상태
		BBatCurPI = BBatCurPI;
	}

	//==================================================================================
	//==1-3.충전전류 제한시 병렬운전 언발란스 보상제어
	//= 위 1-1, 1-2로 하면 충전전류 제한안할때 언발란스제어는 잘되나
	//=                    임의로 1모듈 고장 발생시켜 재기동하면 언발란스 제어 안됨<--충전전류제한이 출력 붙잡고 있음
	//= 개선하기 위해 충전리미트시 언발란스 보상기능 추가
	//==================================================================================
/*
	if(BBatCurPI < 0.0){		//충전전류 제한상태면
		if(UnbalVSet > 1.0){	//언발란스가 상한기준값(1v)보다 크면 자기 출력전류가 작다는 의미니까.. 올리기 위해서 
			UnbalVSet_sub = UnbalVSet_sub + Unbal_incdV_sub;//Unbal_incdV_sub = 0.0001[V]
			if(UnbalVSet_sub > Unbal_dVmax_sub) UnbalVSet_sub = Unbal_dVmax_sub;//Unbal_dVmax_sub = 1.5
		}
		else if(UnbalVSet < -1.0){   // 전체출력전류 평균값이 모듈출력전류보다 작으면 모듈출력증가
			UnbalVSet_sub = UnbalVSet_sub - Unbal_incdV_sub;
			if(UnbalVSet_sub < Unbal_dVmin_sub) UnbalVSet_sub = Unbal_dVmin_sub;	//Unbal_dVmin_sub = -1.5
		}
		else{
			UnbalVSet_sub = UnbalVSet_sub;
		}
	} 
	else{
		UnbalVSet_sub = 0.0;
	}
*/

	//==================================================================================
	//==1-3.충전전류 제한시 병렬운전 언발란스 보상제어
	//= 위 1-1, 1-2로 하면 충전전류 제한안할때 언발란스제어는 잘되나
	//=                    임의로 1모듈 고장 발생시켜 재기동하면 언발란스 제어 안됨<--충전전류제한이 출력 붙잡고 있음
	//= 개선하기 위해 충전리미트시 언발란스 보상기능 추가
	//==================================================================================
	if(BBatCurPI < 0.0){		//충전전류 제한상태면
		if(UnbalVSet > 1.0){	//언발란스가 상한기준값(1v)보다 크면 자기 출력전류가 작다는 의미니까.. 올리기 위해서 
			BBatCurPI = BBatCurPI + Unbal_incdV_sub;//Unbal_incdV_sub = 0.0001[V]
			BBatCurI = BBatCurI + Unbal_incdV_sub;//Unbal_incdV_sub = 0.0001[V]
			//if(UnbalVSet_sub > Unbal_dVmax_sub) UnbalVSet_sub = Unbal_dVmax_sub;//Unbal_dVmax_sub = 1.5
		}
		else if(UnbalVSet < -1.0){   // 전체출력전류 평균값이 모듈출력전류보다 작으면 모듈출력증가
			BBatCurPI = BBatCurPI - Unbal_incdV_sub;
			BBatCurI = BBatCurI - Unbal_incdV_sub;
			//if(UnbalVSet_sub < Unbal_dVmin_sub) UnbalVSet_sub = Unbal_dVmin_sub;	//Unbal_dVmin_sub = -1.5
		}
		else{
			//UnbalVSet_sub = UnbalVSet_sub;
			BBatCurPI = BBatCurPI;
		}
	} 
	else{
		UnbalVSet_sub = 0.0;
		BBatCurPI = BBatCurPI;
	}
		UnbalVSet_sub = 0.0;



	OutVolRef = OutVolSSRef + UnbalVSet + BBatCurPI + UnbalVSet_sub;
//	OutVolRef = OutVolSSRef + UnbalVSet + BBatCurPI;// + UnbalVSet_sub;

	//OutVolRef = (OutVolSSRef + UnbalVSet);// * (1-(BBatCurPI/BBatCurLimit));// + UnbalVSet_sub;

	//출력전압지령 = 출력전압세팅(SoftStart지령)+언발란스조정값+충전전류제한값+충전리미트 언발란스보상값

	//==================================================================================
	//==2.전압제어기
	//= 입력 : 출력전압지령 = 출력전압세팅(SoftStart지령)+언발란스조정값+충전전류제한값+충전리미트 언발란스보상값
	//= 출력 : OutVolPI ==> 전류제어기 입력
	//==================================================================================
	if(OutVolRef > 84.5){	// 최대 출력전압 ref가 84v 이상 안되게...
		OutVolRef = 84.5;
	}
/*
	Vcontrol_Counter++;
	if(Vcontrol_Counter >= 10){
		Vcontrol_Counter = 0;
		OutVolErr = OutVolRef - MOutVolLPF;//
		//V_Kp = 3.0 + fabs(OutVolErr)*0.5;
		//V_Ki = 150.0 + fabs(OutVolErr)*0.5;
//--pi제어기--start		
 		//PI_Controller(Vcontrol_Max,Ts*10,V_Kp,V_Ki,OutVolErr,&OutVolI,&OutVolPI);
		//if(OutVolPI > Vcontrol_Max)				OutVolPI = Vcontrol_Max;//120A
		//if(OutVolPI < -Vcontrol_Max)			OutVolPI = -Vcontrol_Max;
		//OutCurRef = OutVolPI;	//전압제어기 출력을 전류제어기 ref로
//--pi제어기---end
//--antiwidup pi제어기--start
		OutVolP = (OutVolRef-MOutVolLPF)*V_Kp;
		OutVolI += V_Ki*((OutVolRef-MOutVolLPF-((OutVolPI-I_ref)*OutPutVol_Ka))*Ts*10);
		//OutVolI += V_Ki*((OutVolRef-MOutVolLPF)*Ts*10);
		OutVolPI = OutVolP+OutVolI;
				OutVolPI_iref = OutVolPI-I_ref;//확인용
		//if(OutVolPI > 125) I_ref = 125;//	else if(OutVolPI < 0) I_ref = 0;
		//else if(OutVolPI < -125) I_ref = -125;
		//else {I_ref = OutVolPI;}
	
		if(OutVolPI > Vcontrol_Max)			OutCurRef = Vcontrol_Max;//120A
		else if(OutVolPI < -Vcontrol_Max)	OutCurRef = -Vcontrol_Max;
		else								OutCurRef = OutVolPI;	//전압제어기 출력을 전류제어기 ref로
//--antiwidup제어기 pi--end			
	}
*/	
	//==================================================================================
	//==3. 전류제어기
	//= 입력 : OutCurRef(=OutVolPI=전압제어기 출력)
	//= 출력 : OutCurPI ==> DutyRatio == pwm duty
	//==================================================================================
			//OutCurRef = 120;
			//OutCurRef = OutCurRef_Tst;
	OutCurRef = I_ref;
	OutCurErr = OutCurRef - MOutCurLPF;// 전류제어기 추가
	//PI_Controller(OutCurLimit,Ts,I_Kp,I_Ki,OutCurErr,&OutCurI,&OutCurPI);
/*
	I_Kp = 0.3;
	if(OutCurErr < -10.0) { I_Kp = 1.0;}
	if(OutCurErr < -20.0) { I_Kp = 3.0;}
	if(OutCurErr < -30.0) { I_Kp = 5.0;} //--->90
	if(OutCurErr < -40.0) { I_Kp = 7.0;} //--->90
	if(OutCurErr < -50.0) { I_Kp = 9.0;} //--->90
*/

 	OutCurP = (I_ref - MOutCurLPF)*I_Kp;
	
	OutCurI += I_Ki*((I_ref - MOutCurLPF-((OutCurPI-V_ref)*OutPutCur_Ka))*Ts);
	
	
			//OutCurI += I_Ki*(OutCurErr-((OutCurPI-V_ref)*OutPutCur_Ka)*Ts);
	//OutCurI += I_Ki*(I_ref-MOutCurLPF)*Ts;
	OutCurPI = OutCurP + OutCurI;
	if(OutCurPI > 720.0){
		V_ref = 720.0;
	}
	else if(OutCurPI < 0.0){
		V_ref = 0.0;
	}
	else{
		V_ref = OutCurPI;
	}
	V_Duty_Uint = V_ref/720.0;
/*---*/
	Vcontrol_Counter++;
	if(Vcontrol_Counter >= 10){
		Vcontrol_Counter = 0;
		OutVolErr = OutVolRef - MOutVolLPF;
		//if((fabs)(OutVolErr) < 4.0)	V_Kp = 110.0;
		//if((fabs)(OutVolErr) < 3.0)	V_Kp = 90.0;
		//if((fabs)(OutVolErr) < 2.0)	V_Kp = 70.0;
		//if((fabs)(OutVolErr) < 1.0)	V_Kp = 60.0;
		
		
		OutVolP = (OutVolRef-MOutVolLPF)*V_Kp;
		OutVolI += V_Ki*((OutVolRef-MOutVolLPF)*Ts*10);
		OutVolPI = OutVolP+OutVolI;

	//	if(OutVolPI > 120) I_ref = 120;//	else if(OutVolPI < 0) I_ref = 0;
	//	else if(OutVolPI < -120) I_ref = -120;
	//	else {I_ref = OutVolPI;}
	
		//if(OutVolI > 120) OutVolI = 120;//	else if(OutVolPI < 0) I_ref = 0;
		//else if(OutVolI < -120) OutVolI = -120;
		//else {OutVolI = OutVolI;}
	
//		if(OutVolI > 1000) OutVolI = 1000;//	//2015811, 적분리미트추가
//		else if(OutVolI < -1000) OutVolI = -1000;
		
		if(OutVolPI > 125) I_ref = 125;//120--->125
		else if(OutVolPI < -125) I_ref = -125;
		else {I_ref = OutVolPI;}
	}
		if(MOutVolLPF>MOutVolLPF_max)	MOutVolLPF_max = MOutVolLPF;
		if(MBatVolLPF>MBatVolLPF_max)	MBatVolLPF_max = MBatVolLPF;
/*--*/
	DutyRatio = V_Duty_Uint;
	//==4. PWM Duty(ratio&count) 및 PWM 값 계산========================================
	//= 
	//==================================================================================
//	DutyRatio = OutCurPI;
//* (1-(BBatCurPI/BBatCurLimit));


	if(DutyRatio > DutyLimit)     DutyRatio = DutyLimit;
	if(DutyRatio < 0.)            DutyRatio = 0.;

//	DutyRatio = DutyRatio * (1-(fabs(BBatCurPI)/BBatCurLimit));
	//DutyRatio = DutyRatio * (1-((30-BBatCurLPF)/30.0));
	//if(BBatCurLPF>30.0)	DutyRatio = DutyRatio-(BBatCurErr*0.1);
	DutyCount = (Uint16)(DutyRatio*EPWM_DCDC_TBPRD_HALF);//PWM 값 계산

	//==5. PWM DutyCount 보정========================================
	//= 부하급변시 출력전압 오버슈트/언더슈트 방지위해 강제로 pwm DutyCount 조정
	//= 시험해서 부하별로 듀티카운트값 관찰해서 정해야함
	//==================================================================================
	if(MOutVolLPF > DutyCount_up_limitV)	DutyCount = DutyCount - DutyCount_down;//오버슈트 방지
	//OutVolErr = OutVolRef - MOutVolLPF;
	//if(OutVolErr > 5)	DutyCount = DutyCount + DutyCount_down;
	
	//if(DCDC_SoftStart_flag == NO){//---언더슈트는 여러가지 조건 넣어야
	//	if(MOutVolLPF < DutyCount_dn_limitV)	DutyCount = DutyCount + DutyCount_up;
	//}
//Uint16 DutyCount_down = 200;
//Uint16 DutyCount_up = 300;
//float DutyCount_up_limitV = 85.5;
//float DutyCount_dn_limitV = 78.5;	
	
	if(DutyCount > (EPWM_DCDC_TBPRD_HALF - DEADTIME_DCDC)){
		DutyCount = (EPWM_DCDC_TBPRD_HALF - DEADTIME_DCDC);
	}
	else if(DutyCount <  100){	// 데드타임보다 크지 않게..
		DutyCount = 100;
	}
	else{
		DutyCount = DutyCount;	
	}

	EPwm2Regs.TBPHS.half.TBPHS = DutyCount;				//phase shift value
	EPwm2Regs.TBPHS.half.TBPHSHR = 0;
	EPwm2Regs.ETCLR.bit.INT = 1;	

	//------cvcc모드 현시하려고...제어에는 사용안함--------------//
	if(Mode_CV0CC1 == Mode_CC1){//cc모드에서 CV로 변경될때 딜레이
		if(BBatCurPI >= -0.2){//충전전류 제한상태 아님
			Counter_CC_to_CV++;
//			if(Counter_CC_to_CV > 2000000){//cc모드에서 cv모드로의 딜레이(132초 = 66usec*2000000)
			if(Counter_CC_to_CV > 10000){//cc모드에서 cv모드로의 딜레이(132초 = 66usec*500000)
				Counter_CC_to_CV = 0;
				Mode_CV0CC1 = Mode_CV0;//CC-->CV MODE
			}
		}
		else{
			Counter_CC_to_CV = 0;
			Mode_CV0CC1 = Mode_CC1;//CC MODE
		}
	}
	if(Mode_CV0CC1 == Mode_CV0){//cV모드에서
		if(BBatCurPI < -0.4){//충전전류 제한상태가 되면
			Counter_CV_to_CC++;
			if(Counter_CV_to_CC > 10000){//cc모드에서 cv모드로의 딜레이
				Counter_CV_to_CC = 0;
				Mode_CV0CC1 = Mode_CC1;//CV-->CC MODE
			}
		}
		else{
			Counter_CV_to_CC = 0;
			Mode_CV0CC1 = Mode_CV0;//CV MODE
		}
	}
}

//==================================================================================//
// 함수명 : _SoftStart()															//
// calling : 제어루틴																//
// DATE : 2015. 03. 13---변수변경(OutVolRef--> OutVolSSRef)							//
//==================================================================================//
void SoftStart()
{
	if(DCDC_SoftStart_flag == YES){		//제어기,Ramp, pwm_ON
		if(DCDC_SoftStart_counter++ > 50000){
			DCDC_SoftStart_counter = 50000;
		}
	
		if(DCDC_SoftStart_counter == 1){
			OutVolSSRef = MOutVolLPF;	
		}
		if(OutVolSSRef < OutVolRefSet){		
			OutVolSSRef += DCDC_SoftStart_dv; 
			if(OutVolSSRef >= OutVolRefSet){	
				OutVolSSRef = OutVolRefSet;
				DCDC_SoftStart_flag = NO;	
				DCDC_SoftStart_counter = 0;	
			}
		}
		else{
			OutVolSSRef = OutVolRefSet;
			DCDC_SoftStart_flag = NO;
			DCDC_SoftStart_counter = 0;
		}
	}
	else{
		OutVolSSRef = OutVolSSRef;
	}
	
}
//==================================================================================//
// 함수명 : InitDcDcControlVar()	    											//
// calling : ??????                                                                 //
// 제어기 관련 변수 초기화   (init or   FaultAck  )                                 //
// DATE : 2010. 08. 10            						  by PACTECH R&D LSH		//
//==================================================================================//
void InitControlVar()
{

	Mode_CV0CC1 = Mode_CV0;// CVCC 모드(#define Mode_CV0	0	#define Mode_CC1	1)
	Vcontrol_Counter = 0;
	OutCurRef = 0.0;//MOutCurRATED;//139.;
	MOutCurLPF = 0.0;	    // 검출전류 LPF
	OutCurPI = 0.0;	    // pi제어기 출력
	OutCurI = 0.0;
	OutCurP = 0.0;
	OutCurErr = 0.0;
	OutCurLimit = 0.9;//2015년3월16일 초기화 안되있어서 추가<---기존에 어떻게 동작했는지 의문??
//------------------------------------------//	
	
	OutVolRef = 0.0;	
	OutVolSSRef = 0.0;
	OutVolRefSet = MBatVolRATED;	

	OutVolPI = 0.0;
	OutVolI = 0.0;
	OutVolErr = 0.0;	
	OutVolP = 0.0;	
	
	
	UnbalPI = 0.0;
	UnbalI = 0.0;
	UnbalCur_Err = 0.;
	
	UnbalVSet = 0.0;
	UnbalVSet_sub = 0.0;
//==모듈 충전전류 제한관련=================================//
	//MBatCurRef = MBatCurRATED;	// 모듈 충전전류지령 
	MBatCurPI = 0.;
	MBatCurI = 0.;
	MBatCurErr = 0.;
	MBatCurLimit = 0.4;		
//==박스 충전전류 제한관련=================================//
	//BBatCurRef = BBatCurRATED;	// 모듈 충전전류지령 
	BBatCurPI = 0.;
	BBatCurP = 0.0;
	BBatCurI = 0.;
	BBatCurErr = 0.;
//	BBatCurLimit = 0.4;	
	//BBatCurLimit = 10.0;	
	//BBatCurLimit = 20.0;//20 => 83.5 - 20 = 63.5	
	//BBatCurLimit = 15.0;//20150522, 	
	//BBatCurLimit = 17.0;//20150811~12, 	더션테크 배터리, 15-->10-->11-->15-->17
	//BBatCurLimit = 25.0;//17-->22;20151116-->창원완성차시험시 객차 배터리전압 66v상태에서 충전과전류 걸림(모듈?박스?)
	BBatCurLimit = 30.0;//25-->30;20151117-->창원완성차시험시 객차 배터리전압 66v상태에서 충전과전류 걸림(모듈?박스?)
//================================//	
	DutyRatio = 0.;
	DutyCount = 0;
	
	DCDC_SoftStart_flag = 0;
	DCDC_SoftStart_counter = 0;	
//	DCDC_SoftStart_dv = 0.05;
	
	//--PWM값 초기화
//	EPwm1Regs.CMPA.half.CMPA = bcDutyCount;
//	EPwm1Regs.CMPB = EPWM_DCDC_TBPRD_HALF - bcDutyCount;
//	EPwm1Regs.ETCLR.bit.INT = 1;
	
//	EPwm2Regs.CMPA.half.CMPA = bcDutyCount;
//	EPwm2Regs.CMPB = EPWM_DCDC_TBPRD_HALF - bcDutyCount;
//	EPwm2Regs.ETCLR.bit.INT = 1;		
	
	//---조심!!! pwm 초기화 안하면 초기 기동시 최대듀티 나갈수 있음
	EPwm2Regs.TBPHS.half.TBPHS = 0;//DutyCount;				//phase shift value
	EPwm2Regs.TBPHS.half.TBPHSHR = 0;
	
}
//=================================================================//


