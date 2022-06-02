//==================================================================================//
// 원강선 Battery Charger(30kw, 50kw)  DC/DC Converter Controller           		//
//==================================================================================//
// BC Main header file															//
// DATE : 2015. 01. 7            						  by PACTECH R&D LSH		//
//==================================================================================//

#ifndef __NTHS_BACH_DCDCCONTROLLER__   
#define __NTHS_BACH_DCDCCONTROLLER__   

//========BC dc/dc controller 관련변수선언=======//
void DcDcController();
void InitControlVar();
void SoftStart();
//=================================================//
//===모듈 출력전압 제어관련====
var float	V_Kp;
var float	V_Ki;
var float	OutVolRef;		// 전압지령(SoftStart+병렬운전+충전전류제한)--->전압제어기 입력
var float	OutVolSSRef;	// 전압지령(SoftStart지령)

var float	OutVolRefSet;	// 전류지령 
var float	OutVolPI;	    // pi제어기 출력
var float	OutVolI;
var float	OutVolErr;
var float	OutVolP;
//===모듈 출력전류 제어관련====
var float	I_Kp;
var float	I_Ki;
var float	OutPutCur_Ka;
var float	OutPutVol_Ka;
var float	V_ref;
var float	I_ref;
var float	OutCurLimit;
var float	V_Duty_Uint;


var float	OutCurRef;	// 전류지령 
var float	OutCurPI;	    // pi제어기 출력
var float	OutCurI;
var float	OutCurP;
var float	OutCurErr;


//===모듈 충전전류 제한관련====
var float	MBatCur_Kp;
var float	MBatCur_Ki;
var float	MBatCurRef;	// 모듈 충전전류지령 
var float	MBatCurPI;	    // pi제어기 출력
var float	MBatCurI;
var float	MBatCurErr;
var float	MBatCurLimit;
//===박스 충전전류 제한관련====
var float	BBatCur_Kp;
var float	BBatCur_Ki;
var float	BBatCurRef;	// 박스 충전전류지령 
var float	BBatCurPI;	    // pi제어기 출력
var float	BBatCurP;	    // pi제어기 출력
var float	BBatCurI;
var float	BBatCurErr;
var float	BBatCurLimit;	
//===기타제어관련

var float	OutVol_KI_damp;
var float	DutyLimit;


//---모듈과 박스전류 언발란스 관련===
var float	UnbalPI;	    //
var float	UnbalI;
var float	UnbalCur_Err;

var float	DutyRatio;
var Uint16	DutyCount;

var Uint16	Vcontrol_Counter;	// acdc전류제어기 카운터(10번 전류제어하면 전압제어기 1번)

void DcDcController_WGBC_PALL_VI();
void DcDcController_WGBC_PALL();
void WGBC_DCDC_Controller_20150522();
//----------------------------------//	

#endif //  




