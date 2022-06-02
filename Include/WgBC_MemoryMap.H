#ifndef NTHS_MEMORYMAP_H
#define NTHS_MEMORYMAP_H

#ifdef __cplusplus
extern "C" {
#endif

/* micro-seconds delay function */
// TI SDK 1.10의 소스 DSP2833x_usDelay.asm에서 제공하는 DELAY_US 함수를 사용
#define delay_us(us)		DELAY_US(us)	 
/* milli-seconds delay function */
// TI SDK 1.10의 소스 DSP2833x_usDelay.asm에서 제공하는 DELAY_US 함수를 사용
#define delay_ms(ms)		DELAY_US(ms*1000)


//**********************************************************************//
// WGBC MEMORY MAP I/O ADDRESS											//
//**********************************************************************//
//ZCS0 ---> EPLD I/O
#define CS0_START_ADDR		0x004000
#define CS0_END_ADDR		0x004FFF
#define CS0_SIZE			0x001000
//ZCS6 ---> TBD
#define CS6_START_ADDR		0x100000
#define CS6_END_ADDR		0x1FFFFF
#define CS6_SIZE			0x100000
//ZCS7 ---> NVRAM
#define CS7_START_ADDR		0x200000
#define CS7_END_ADDR		0x2FFFFF
#define CS7_SIZE			0x100000

//========================================
// 메인 DSP 어드레스
//========================================
//**********************************************************************//
// NVRAM 어드레스(ZCS7)												    //
//**********************************************************************//\
//---NVRAM TDP(Trace Data Page) START ADDRESS

//---트레이스 데이타 구조
#define TDP_SIZE		0x8000
//#define TDPxy_LEN		0x7D00
//#define TDP_SIZE		32768
#define TDPxy_LEN		32000
#define cana_TD_idxMAX	4000
#define TDPxy_TXByte	8
#define TDPy_N			1000
#define TDPx_N			32
//--->실제로 1001개
//TDPxy_LEN = TDPx_N*TDPy_N


//---NVRAM TDP(Trace Data Page) START ADDRESS
#define CS_NVRAM_START				0x200000
#define CS_NVRAM_END				0x27FFFF

#define ADD_NVR_TDP0				0x200000
#define ADD_NVR_TDP1				0x208000
#define ADD_NVR_TDP2				0x210000
#define ADD_NVR_TDP3				0x218000
#define ADD_NVR_TDP4				0x220000
#define ADD_NVR_TDP5				0x228000
#define ADD_NVR_TDP6				0x230000
#define ADD_NVR_TDP7				0x238000
#define ADD_NVR_TDP8				0x240000
#define ADD_NVR_TDP9				0x248000
#define ADD_NVR_TDP10				0x250000
#define ADD_NVR_TDP11				0x258000
#define ADD_NVR_TDP12				0x260000
#define ADD_NVR_TDP13				0x268000
#define ADD_NVR_TDP14				0x270000

//ZONE		HEX							 DEC								비고	 							순환여부
//PAGE		START	 END		LENGTH	 START		 END		 LENGTH					
//Page0		200000	 207FFF	    8000	 2,097,152 	 2,129,919 	 32,768 	32K X 8	    1번째 고장데이타			X
//Page1		208000	 20FFFF 	8000	 2,129,920 	 2,162,687 	 32,768 	32K X 9	    2번째 고장데이타			X
//Page2		210000	 217FFF 	8000	 2,162,688 	 2,195,455 	 32,768 	32K X 10	3번째 고장데이타			X
//Page3		218000	 21FFFF 	8000	 2,195,456 	 2,228,223 	 32,768 	32K X 11	4번째 고장데이타			O
//Page4		220000	 227FFF 	8000	 2,228,224 	 2,260,991 	 32,768 	32K X 12	5번째 고장데이타			O
//Page5		228000	 22FFFF 	8000	 2,260,992 	 2,293,759 	 32,768 	32K X 13	6번째 고장데이타			O
//Page6		230000	 237FFF 	8000	 2,293,760 	 2,326,527 	 32,768 	32K X 14	7번째 고장데이타			O
//Page7		238000	 23FFFF 	8000	 2,326,528 	 2,359,295 	 32,768 	32K X 15	8번째 고장데이타			O
//Page8		240000	 247FFF 	8000	 2,359,296 	 2,392,063 	 32,768 	32K X 16	9번째 고장데이타			O
//Page9		248000	 24FFFF 	8000	 2,392,064 	 2,424,831 	 32,768 	32K X 17	10번째 고장데이타			O
//Page10	250000	 257FFF 	8000	 2,424,832 	 2,457,599 	 32,768 	32K X 18	11번째 고장데이타			O
//Page11	258000	 25FFFF 	8000	 2,457,600 	 2,490,367 	 32,768 	32K X 19	12번째 고장데이타			O
//Page12	260000	 267FFF 	8000	 2,490,368 	 2,523,135 	 32,768 	32K X 20	13번째 고장데이타			O
//Page13	268000	 26FFFF 	8000	 2,523,136 	 2,555,903 	 32,768 	32K X 21	14번째 고장데이타			O
//Page14	270000	 277FFF 	8000	 2,555,904 	 2,588,671 	 32,768 	32K X 22	15번째 고장데이타			O
//				 		 491,520 					




//---NVRAM CDP(Control Data Page) START ADDRESS
#define ADD_NVR_CDP0				0x278000
//#define ADD_NVR_CDP0				0x278001
#define ADD_NVR_CDP1				0x27B000
#define ADD_NVR_CDP2				0x27C000
#define ADD_NVR_CDP3				0x27D000
#define ADD_NVR_CDP4				0x27E000
#define ADD_NVR_CDP5				0x27F000

//---NVRAM RTC ADDRESS
#define ADD_NVR_RTC_START			0x27FFF8
#define ADD_NVR_RTC_YEAR			0x27FFFF
#define ADD_NVR_RTC_MONTH			0x27FFFE
#define ADD_NVR_RTC_DATE			0x27FFFD
#define ADD_NVR_RTC_DAY				0x27FFFC
#define ADD_NVR_RTC_HOUR			0x27FFFB
#define ADD_NVR_RTC_MIN				0x27FFFA
#define ADD_NVR_RTC_SEC				0x27FFF9
#define ADD_NVR_RTC_CENTURY			0x27FFF8

//**********************************************************************//
// EPLD I/O 어드레스(ZCS0)												//
//**********************************************************************//
// 출력
#define CS_WR_LED1()		PWORD(0x4100)
#define CS_WR_LED2()		PWORD(0x4101)
#define CS_WR_LED3()		PWORD(0x4102)
#define CS_WR_DOUT1()		PWORD(0x4103)
#define CS_WR_PWMEN()		PWORD(0x4104)

#define CS_WR_CTRL()		PWORD(0x4105)
//---epld eled와 이름 다름
#define CS_WR_FCLR()		PWORD(0x4106)
#define CS_WR_WRR7()		PWORD(0x4107)

// 입력
#define CS_RD_DIN1()		PWORD(0x4200)
#define CS_RD_DIN2()		PWORD(0x4201)
#define CS_RD_GDF()			PWORD(0x4202)
#define CS_RD_HWF1()		PWORD(0x4203)
#define CS_RD_HWF2()		PWORD(0x4204)
#define CS_RD_R5()			PWORD(0x4205)
#define CS_RD_R6()			PWORD(0x4206)
#define CS_RD_R7()			PWORD(0x4207)

// 미결정
#define CS_RD_INT()			PWORD(0x4300)
#define CS_RD_DIOS()		PWORD(0x4301)
#define CS_RD_TST()			PWORD(0x4302)
#define CS_WR_INT()			PWORD(0x4303)
#define CS_WR_DIOS()		PWORD(0x4304)

#define CS_WR_ELED()		PWORD(0x4305)

#define CS_RWD_R6()			PWORD(0x4306)
#define CS_RWD_R7()			PWORD(0x4307)

//================================================================
// PWM 관련 메크로
//================================================================
// PWM 인에이블
#define PWM_ON()	CS_WR_PWMEN() = 0x03; // PWM ON
#define PWM_OFF()	CS_WR_PWMEN() = 0x00; // PWM OFF

//**********************************************************************//
// DSP GPIO 매크로														//
//**********************************************************************//
// WG CPU 상태 LED
// MCU BOARD LED(LD4) -- WGBC_TC0PC1---ON(TCBC), OFF(PCBC)
#define MCU_LD4_ON()			(GpioDataRegs.GPACLEAR.bit.GPIO8 = 1)	
#define MCU_LD4_OFF()			(GpioDataRegs.GPASET.bit.GPIO8 = 1)
#define MCU_LD4_TOGGLE()		(GpioDataRegs.GPATOGGLE.bit.GPIO8 = 1)

#define MCU_LD3_ON()			(GpioDataRegs.GPACLEAR.bit.GPIO9 = 1)	
#define MCU_LD3_OFF()			(GpioDataRegs.GPASET.bit.GPIO9 = 1)
#define MCU_LD3_TOGGLE()		(GpioDataRegs.GPATOGGLE.bit.GPIO9 = 1)

#define MCU_LD2_ON()			(GpioDataRegs.GPACLEAR.bit.GPIO10 = 1)	
#define MCU_LD2_OFF()			(GpioDataRegs.GPASET.bit.GPIO10 = 1)
#define MCU_LD2_TOGGLE()		(GpioDataRegs.GPATOGGLE.bit.GPIO10 = 1)

#define MCU_LD1_ON()			(GpioDataRegs.GPACLEAR.bit.GPIO11 = 1)	
#define MCU_LD1_OFF()			(GpioDataRegs.GPASET.bit.GPIO11 = 1)
#define MCU_LD1_TOGGLE()		(GpioDataRegs.GPATOGGLE.bit.GPIO11 = 1)

//--CPU 보드 LED(D33)
#define MCU_D33_ON()			(GpioDataRegs.GPBCLEAR.bit.GPIO60 = 1)	
#define MCU_D33_OFF()			(GpioDataRegs.GPBSET.bit.GPIO60 = 1)
#define MCU_D33_TOGGLE()		(GpioDataRegs.GPBTOGGLE.bit.GPIO60 = 1)
//--CPU 보드 LED(D34)
#define MCU_D34_ON()			(GpioDataRegs.GPBCLEAR.bit.GPIO61 = 1)	
#define MCU_D34_OFF()			(GpioDataRegs.GPBSET.bit.GPIO61 = 1)
#define MCU_D34_TOGGLE()		(GpioDataRegs.GPBTOGGLE.bit.GPIO61 = 1)



	
	
#define FAULTLATCHCLEAR()				CS_WR_FCLR() = 0x00
//#define FAULTGENSW()			WR_DSP_SW_FAULT_CS() = 0x00


// DSP28335 DSK에서 외부 버스 영역 중 Zone0과  Zone7를 16비트 모드로 초기화하는 함수
// RealSYS DSP28335 DSK에서는 SCI-A와 CAN-A를 사용하므로 A17 ~ A19를 External Address 핀으로 사용하지 않음.
// 따라서 최대 쓸수 있는 영역 사이즈는 0x100000 -> 0x20000 임. 이점 유의하시길 바랍니다.
void Init_Ext_Bus(void);	// DSP ADDRESS/DATA BUS
 /* 데이터 변환 관련 함수(LCD 및 통신 프로토콜 처리에서 사용함 */
extern char asc2hex(char asc);  // ASCII 데이터를 HEX 데이터로 변환(debug_2833x.c 파일에 구현)
extern char hex2asc(char hex);  // HEX 데이터를 ASCII 데이터로 변환(debug_2833x.c 파일에 구현)

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif // end of DSK2833x_GLOBALFUNC_H definition
