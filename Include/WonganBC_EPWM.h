#ifndef __ePWM_H
#define __ePWM_H
#define Ts				(66.7e-6)
//#define Ts				(200e-6)
//#define Ts				(166.666e-6)
//#define Ts				(133.333e-6)
//#define Ts				(142.857e-6)
//***********************************
// Phase Shifted Time Base Period
// Switching Freq 5kHz
// Count Up 
// 스위칭 주파수 5kHz, TBCLK = SYSCLKOUT/(HSPCLKDIV*CLKDIV)=150E+6/(1*2)=75E+6;
// 카운트 갯수는 75E+6/5k = 15000개
// 카운트 갯수는 75E+6/10k = 7500개
//***********************************
//#define PSFB_TIMER_TBPRD 		15000
//#define PSFB_TIMER_TBPRD_HALF 	(PSFB_TIMER_TBPRD/2)

//***********************************
// Phase Shifted Time Base Period
// Switching Freq 7.5kHz
// Count Up 
// 스위칭 주파수 5kHz, TBCLK = SYSCLKOUT/(HSPCLKDIV*CLKDIV)=150E+6/(1*2)=75E+6;
// 카운트 갯수는 75E+6/3k = 25000
// 카운트 갯수는 75E+6/5k = 15000
// 카운트 갯수는 75E+6/6k = 12500
// 카운트 갯수는 75E+6/7 = 10714
// 카운트 갯수는 75E+6/7.5 = 10000
// 카운트 갯수는 75E+6/10k = 7500
// 카운트 갯수는 75E+6/12k = 6250
//***********************************
#define PSFB_TIMER_TBPRD 		15000
//#define PSFB_TIMER_TBPRD 		37500
//#define PSFB_TIMER_TBPRD 		14999
#define PSFB_TIMER_TBPRD_HALF 	(PSFB_TIMER_TBPRD/2)


// TBCLK의 타임은 0.01333us, 0.01333us*375 = 5us;     
#define PSFB_DEADTIME_GAP	  375	// programmable delay to rising/falling edges
//#define PSFB_DEADTIME_GAP	  100	// programmable delay to rising/falling edges
// TBCLK의 타임은 0.01333us, 0.01333us*600 = 8us;     
//#define PSFB_DEADTIME_GAP	  600	// programmable delay to rising/falling edges

//==============================================


// ePWM1/2/3에서 사용하는 Timer Period
// 스위칭 주파수2.5kHz, TBCLK = SYSCLKOUT/(HSPCLKDIV*CLKDIV)=150E+6/(1*2)=75E+6;
// 카운트 갯수는 75E+6/5k = 15000개
// 카운터를 업/다운 으로 사용하기 때문에 TBPRD에 카운트 갯수의 절반을 로드한다.
//====75000 => 1khz
//#define EPWM_TIMER_TBPRD  35000  
#define EPWM_TIMER_TBPRD  37500  
//====BACH/APS AC/DC Switching frequence75000 => 1khz
//#define EPWM_ACDC_TBPRD  37500
//#define DEADTIME_ACDC	  600	// programmable delay to rising/falling edges

#define EPWM_ACDC_TBPRD  15000
#define EPWM_ACDC_TBPRD_HALF  7500
//#define EPWM_ACDC_TBPRD_HALF  7499
//#define EPWM_ACDC_TBPRD  37500

#define DEADTIME_ACDC	  600	// programmable delay to rising/falling edges
//====DC/DC Switching frequence 15000 => 10khz
//#define EPWM_DCDC_TBPRD  15000
//#define EPWM_DCDC_TBPRD_HALF  7500

/*//====5khz === 기존 데이터
#define EPWM_DCDC_TBPRD  30000  
#define EPWM_DCDC_TBPRD_HALF  15000
#define DEADTIME_DCDC	  700	// programmable delay to rising/falling edges)(8usec)
//====5khz
#define EPWM_BOOST_TBPRD  15000  
#define EPWM_BOOST_TBPRD_HALF  7500
#define DEADTIME_BOOST	  700	// programmable delay to rising/falling edges)(8usec)
*/

//====5khz === 20140830 주파수 변경 5khz
//#define EPWM_DCDC_TBPRD  30000
//#define EPWM_DCDC_TBPRD_HALF  15000
//#define DEADTIME_DCDC	  700	// programmable delay to rising/falling edges)(8usec)

//====15khz === 20141125 원강선 주파수 세팅 15khz
#define EPWM_DCDC_TBPRD  10000
#define EPWM_DCDC_TBPRD_HALF  5000
#define DEADTIME_DCDC	  300	// 데드타임 2usec(300)
//====10khz === 20141125 원강선 주파수 세팅 15khz
//#define EPWM_DCDC_TBPRD  15000
//#define EPWM_DCDC_TBPRD_HALF  7500
//#define DEADTIME_DCDC	  700	// 데드타임 2usec(300)


//====5khz === 20140830 주파수 변경 5khz
#define EPWM_BOOST_TBPRD  15000
#define EPWM_BOOST_TBPRD_HALF  7500
#define DEADTIME_BOOST	  700	// programmable delay to rising/falling edges)(8usec)



//====SIV Switching frequence EPWM_INV_TBPRD = 37500 => 1khz
//====SIV Switching frequence PWM_INV_TBPRD = 15000 => 2.5khz
//====SIV Switching frequence PWM_INV_TBPRD = 7500 => 5khz
//====SIV Switching frequence PWM_INV_TBPRD = 5000 => 7.5khz
//#define EPWM_INV_TBPRD	7500
		//#define EPWM_INV_TBPRD	37500

		//#define EPWM_INV_TBPRD	25000
#define EPWM_INV_TBPRD	18750
//---20120203 창원에서 왜율안좋아서 스위칭주파수 1KHZ(37500)->1.5KHZ(25000)으로 변경
#define DEADTIME_INV	  600	// programmable delay to rising/falling edges

// 3kHz 
//#define EPWM_TIMER_TBPRD  12500  
#define EPWM_TIMER_TBPRD_MG  600  
// 3us의 데드타임을 얻기 위해서는 
// TBCLK * 데드타임 : 75e+6 * 3e-6 = 225개
// TBCLK의 타임은 0.01333us, 0.01333us*225 = 3us;     
//#define DEADTIME_GAP	  225	// programmable delay to rising/falling edges

// 5us의 데드타임을 얻기 위해서는 
// TBCLK * 데드타임 : 75e+6 * 5e-6 = 375개
// TBCLK의 타임은 0.01333us, 0.01333us*375 = 5us;     
//#define DEADTIME_GAP	  375	// programmable delay to rising/falling edges
// TBCLK의 타임은 0.01333us, 0.01333us*600 = 8us;     
#define DEADTIME_GAP	  600	// programmable delay to rising/falling edges
// TBCLK의 타임은 0.01333us, 0.01333us*450 = 6us;     
//#define DEADTIME_GAP	  450	// programmable delay to rising/falling edges

void EPwmInterruptEnable();
void EPwmInterruptDisable();


void InitEPwm1();
void InitEPwm2();
void InitEPwm3();


void InitWGEpwm();

//void PwmUpDate_mdPhaseShiftUpper(Uint16 PhaseShift, Uint16 DeadTime);

#endif


