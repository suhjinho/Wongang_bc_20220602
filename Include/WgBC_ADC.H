#ifndef __ADC_H
#define __ADC_H


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

var float Scale_MInputVol;		//모듈입력전압 --> ADCRD10
var float Scale_MDcLinkVol;		//모듈 DCLINK전압(RATED 670Vdc) --> ADCRD11
var float Scale_MOutVol;		//모듈출력전압1(OBD후단) 정격 720Vdc(72~83) --> ADCRD9
//var float Scale_MOutVol1;		//모듈출력전압1(OBD전단) 정격 720Vdc(72~83) --> ADCRD8
var float Scale_MCtrlVol;		//모듈 제어전압 정격 72Vdc(72~83) --> ADCRD7
var float Scale_MInputCur;		//모듈 입력전류 --> ADCRD0
var float Scale_BOutCur;		//box 출력전류 --> ADCRD6
var float Scale_MBatCur;		//모듈 충전전류 --> ADCRD1
var float Scale_MOutCur;		//모듈 출력전류 --> ADCRD2
var float Scale_BBatCur1;		//box BAT전류1 --> ADCRD4
var float Scale_BBatCur2;		//box BAT전류2 --> ADCRD5 (객차 BC는 사용안함)

var int adc0_offset;
var int adc1_offset;
var int adc2_offset;
var int adc3_offset;
var int adc4_offset;
var int adc5_offset;
var int adc6_offset;
var int adc7_offset;
var int adc8_offset;
var int adc9_offset;
var int adc10_offset;
var int adc11_offset;

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

var unsigned int adc_offsetCounter;


		
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
void Adc_CurrentOffset_Caculation();
void SensorBoard_Scale_Init();	// 차세대용, APS, BACH공용
void InitSensorVar();
void InitADC();
void InitADC_Var();
void Sensor_Readout();
#endif
