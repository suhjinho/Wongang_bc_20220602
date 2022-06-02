//==================================================================================//
// 차세대 고속철도(NTHS) 배터리 충전장치(BACH) DC/DC Converter Controller    		//
//==================================================================================//
// BACH Main header file															//
// 나중에 화일이름 NTHS_Bach.h로 변경												//
// DATE : 2010. 05. 11            						  by PACTECH R&D LSH		//
//==================================================================================//

#ifndef __NTHS_BACH_DCDCCONTROLLER__   
#define __NTHS_BACH_DCDCCONTROLLER__   

//==================================================================================//
// DATE : 2010. 08. 11            						  by PACTECH R&D LSH		//
//==================================================================================//

//========bach dc/dc controller 관련변수선언=======//
void DcDcController();
void InitDcDcControlVar();
void DcdcSoftStart();
//=================================================//
var float	DcdcOutCurKp;
var float	DcdcOutCurKi;
var float	DcdcOutCurLimit;

var float	DcdcOutCurRef;	// 전류지령 
var float	DcdcOutCurPI;	    // pi제어기 출력
var float	DcdcOutCurI;
var float	DcdcOutCurErr;
var float	DcdcOutVolKp;
var float	DcdcOutVolKi;
var float	DcdcDutyLimit;


var float	DcdcOutVolRef;	// 전류지령 
var float	DcdcOutVolRefSet;	// 전류지령 
var float	DcdcOutVolPI;	    // pi제어기 출력
var float	DcdcOutVolI;
var float	DcdcOutVolErr;
var float	DcdcDutyRatio;
var Uint16	DcdcDutyCount;

var Uint16	DcdcoopCounter;	// acdc전류제어기 카운터(10번 전류제어하면 전압제어기 1번)


//----------------------------------//	

#endif //  




