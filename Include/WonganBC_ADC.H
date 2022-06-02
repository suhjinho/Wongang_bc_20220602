#ifndef __ADC_H
#define __ADC_H


var float InvMaxQ12;//(1/2048)
var float MaxQ12;//-2047~2048




var int ADCRD0;
var int ADCRD1;
var int ADCRD2;
var int ADCRD3;
var int ADCRD4;
var int ADCRD5;
var int ADCRD6;
var int ADCRD7;
var int ADCRD8;
var int ADCRD9;
var int ADCRD10;
var int ADCRD11;
var int ADCRD12;
var int ADCRD13;
var int ADCRD14;
var int ADCRD15;

var unsigned int adc0_offset;
var unsigned int adc1_offset;
var unsigned int adc2_offset;
var unsigned int adc3_offset;
var unsigned int adc4_offset;
var unsigned int adc5_offset;
var unsigned int adc6_offset;
var unsigned int adc7_offset;
var unsigned int adc8_offset;
var unsigned int adc9_offset;
var unsigned int adc10_offset;
var unsigned int adc11_offset;
var unsigned int adc12_offset;
var unsigned int adc13_offset;
var unsigned int adc14_offset;
var unsigned int adc15_offset;

var Uint32 adc0_sum;
var Uint32 adc1_sum;
var Uint32 adc2_sum;
var Uint32 adc3_sum;
var Uint32 adc4_sum;
var Uint32 adc5_sum;
var Uint32 adc6_sum;
var Uint32 adc7_sum;
var Uint32 adc8_sum;
var Uint32 adc9_sum;
var Uint32 adc10_sum;
var Uint32 adc11_sum;
var Uint32 adc12_sum;
var Uint32 adc13_sum;
var Uint32 adc14_sum;
var Uint32 adc15_sum;


var unsigned int adc_offsetCount;
//====ADC BASE SCALE 관련변수=====
var float 	InputVolSensorBase;	
var float 	DcLinkVolBase;
var float 	OutVolSensorBase;

var float 	InputCurBase;
var float 	Bat1OutCurBase;
var float 	OutCurSensorBase;

		
var float   DcCtOffset1,
			DcCtOffset2,
			DcCtOffset3,
			DcCtOffset4,
			DcCtOffset5,
			DcCtOffset6,
			DcCtOffset7,
			DcCtOffset8;
			

var Uint32	CurrOffsetCount,
			CurrOffsetCalcCount;
var Uint16	CurrOffsetComplete,
			CurrOffsetCalcOrder;



var float 	DcCtOffsetSum1;
var float 	DcCtOffsetSum2;
var float 	DcCtOffsetSum3;
var float 	DcCtOffsetSum4;
var float 	DcCtOffsetSum5;
var float 	DcCtOffsetSum6;
var float 	DcCtOffsetSum7;
var float 	DcCtOffsetSum8;


//===========================================================
// 함수 선언
//===========================================================

//void AdcInitialization();
void CurrentOffsetCalc();
void SensorBoard_Scale_Init();	// 차세대용, APS, BACH공용
void SensorBoard_AnalogRead();	// 차세대용, APS, BACH공용
void SensorBoard_AnalogRmsLPF();
void InitSensorVar();
#endif
