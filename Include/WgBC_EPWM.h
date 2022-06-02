#ifndef __ePWM_H
#define __ePWM_H
#define Ts				(66.7e-6)
//#define Ts				(200e-6)
//#define Ts				(166.666e-6)
//#define Ts				(133.333e-6)
//#define Ts				(142.857e-6)


// ePWM1/2/3���� ����ϴ� Timer Period
// ����Ī ���ļ�2.5kHz, TBCLK = SYSCLKOUT/(HSPCLKDIV*CLKDIV)=150E+6/(1*2)=75E+6;
// ī��Ʈ ������ 75E+6/5k = 15000��
// ī���͸� ��/�ٿ� ���� ����ϱ� ������ TBPRD�� ī��Ʈ ������ ������ �ε��Ѵ�.

//====15khz === 20141125 ������ ���ļ� ���� 15khz
#define EPWM_DCDC_TBPRD  10000
#define EPWM_DCDC_TBPRD_HALF  5000
#define DEADTIME_DCDC	  300	// ����Ÿ�� 2usec(300)
//====10khz === 20141125 ������ ���ļ� ���� 15khz
//#define EPWM_DCDC_TBPRD  15000
//#define EPWM_DCDC_TBPRD_HALF  7500
//#define DEADTIME_DCDC	  700	// ����Ÿ�� 2usec(300)


//====5khz === 20140830 ���ļ� ���� 5khz
#define EPWM_BOOST_TBPRD_HALF  7500


void EPwmInterruptEnable();
void EPwmInterruptDisable();


void InitEPwm1();
void InitEPwm2();
void InitEPwm3();


void InitWGEpwm();


#endif


