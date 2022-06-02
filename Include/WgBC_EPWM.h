#ifndef __ePWM_H
#define __ePWM_H
#define Ts				(66.7e-6)
//#define Ts				(200e-6)
//#define Ts				(166.666e-6)
//#define Ts				(133.333e-6)
//#define Ts				(142.857e-6)


// ePWM1/2/3에서 사용하는 Timer Period
// 스위칭 주파수2.5kHz, TBCLK = SYSCLKOUT/(HSPCLKDIV*CLKDIV)=150E+6/(1*2)=75E+6;
// 카운트 갯수는 75E+6/5k = 15000개
// 카운터를 업/다운 으로 사용하기 때문에 TBPRD에 카운트 갯수의 절반을 로드한다.

//====15khz === 20141125 원강선 주파수 세팅 15khz
#define EPWM_DCDC_TBPRD  10000
#define EPWM_DCDC_TBPRD_HALF  5000
#define DEADTIME_DCDC	  300	// 데드타임 2usec(300)
//====10khz === 20141125 원강선 주파수 세팅 15khz
//#define EPWM_DCDC_TBPRD  15000
//#define EPWM_DCDC_TBPRD_HALF  7500
//#define DEADTIME_DCDC	  700	// 데드타임 2usec(300)


//====5khz === 20140830 주파수 변경 5khz
#define EPWM_BOOST_TBPRD_HALF  7500


void EPwmInterruptEnable();
void EPwmInterruptDisable();


void InitEPwm1();
void InitEPwm2();
void InitEPwm3();


void InitWGEpwm();


#endif


