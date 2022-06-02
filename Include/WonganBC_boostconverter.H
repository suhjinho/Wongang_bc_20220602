#ifndef __ACDCPLL_H
#define __ACDCPLL_H

#define LPF1_Ts	200e-6//수정

var float	boostIKp;
var float	boostIKi;

var float	boostVKp;
var float	boostVKi;
var float	boostDutyLimit;
var float	boostOutCurLimit;

//var Uint16	boostloopCounter;	// acdc전류제어기 카운터(10번 전류제어하면 전압제어기 1번)


var float	boostOutVolRef;	// 전압지령 (dclink전압)
var float	boostOutVolRefSet;
var float	boostOutVolLPF;	// 검출전압 LPF(dclink전압)
var float	boostOutVolPI;	    // pi제어기 출력
var float	boostOutVolI;		// pi제어기 적분치
var float	boostOutVol_Err;     // pi제어기 에러

var float	boostVacRef;

var float	boostDutyRatio;
var Uint16	boostDutyCount;

void InitAcDcControlVar();
void BoostSoftStart();
#endif

