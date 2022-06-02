//###############################################################################################################
// 파일명   : eCAN.c
// 회사명   : RealSYS
// 작성날짜 : 2008.02.21
// 작성자   : R.N.K
// 설명     : F28335의 내부 CAN-A/B 주변장치 관련 구현 코드
//
// [ 사용한 개발 환경 ]
// 1) TI CCS3.3과 Service Pack v8.0(CCS_v3.3_SR8_77.exe 파일)
// 2) CCS3.3 Setup용 Target Driver Package(setupCCSPlatinum_v30323.exe 파일) 
// 3) C2000 Code Generation Tools v5.0 Release(C2000CodeGenerationTools5.0.0.exe 파일) 
// [ 참조한 Software Developer Kit(SDK) ]
// 1) TI의 SDK인 C/C++ Header Files and Peripheral Examples v1.10 사용 
//###############################################################################################################

#include "Define.h"
#include "Global_Variable_Function.h"

// 제공된 예제에서는 인터럽트 서비스 루틴을 "ramfuncs" 이라는 섹션에 할당하고
// "ramfuncs" 섹션은 빠른 응답 처리를 위해 내부 RAM에서 동작하도록 함

// eCAN-A/B Interrupt Service Function 선언
#pragma CODE_SECTION(ecan0_inta_isr, "ramfuncs");
#pragma CODE_SECTION(ecan1_inta_isr, "ramfuncs");
#pragma CODE_SECTION(ecan0_intb_isr, "ramfuncs");
#pragma CODE_SECTION(ecan1_intb_isr, "ramfuncs");

interrupt void ecan0_inta_isr(void);
interrupt void ecan1_inta_isr(void);
interrupt void ecan0_intb_isr(void);
interrupt void ecan1_intb_isr(void);

unsigned int canA_TX_Counter = 0;
unsigned int canA_RX_Counter = 0;
unsigned int canB_TX_Counter = 0;
unsigned int canB_RX_Counter = 0;
unsigned int canB_RX_Counter1 = 0;
unsigned int test_can1 = 0;
unsigned int test_can2 = 0;

// eCAN-A/B 처리를 위한 변수 ------------------------------------------------
BOOL cana_rx_flag=FALSE;	// 사용자가 CAN-A 수신받은 데이타 저장한후(0), 아직 저장아님(1)
LONG cana_rx_id=0;			// CAN-A 수신 ID
BYTE cana_rx_length=0;    	// CAN-A 수신 데이터 크기
LONG cana_rx_low_data=0;	// CAN-A 수신 Low Word Data
LONG cana_rx_high_data=0;	// CAN-A 수신 High Word Data

BOOL canb_rx_flag=FALSE;	// 사용자가 CAN-B 수신받은 데이타 저장한후(0), 아직 저장아님(1)
LONG canb_rx_id=0;			// CAN-B 수신 ID
BYTE canb_rx_length=0;    	// CAN-B 수신 데이터 크기
LONG canb_rx_low_data=0;	// CAN-B 수신 Low Word Data
LONG canb_rx_high_data=0;	// CAN-B 수신 High Word Data

/*
// ex08_can.c에 정의된 eCAN-A/B 처리를 위한 변수 -----------------------------
extern BOOL cana_rx_flag;		// CAN-A에 새로운 수신 데이터를 받았는지 여부
extern LONG cana_rx_id;			// CAN-A 수신 ID
extern BYTE cana_rx_length;   	// CAN-A 수신 데이터 크기
extern LONG cana_rx_low_data;	// CAN-A 수신 Low Word Data
extern LONG cana_rx_high_data;	// CAN-A 수신 High Word Data

extern BOOL canb_rx_flag;		// CAN-B에 새로운 수신 데이터를 받았는지 여부
extern LONG canb_rx_id;			// CAN-B 수신 ID
extern BYTE canb_rx_length;   	// CAN-B 수신 데이터 크기
extern LONG canb_rx_low_data;	// CAN-B 수신 Low Word Data
extern LONG canb_rx_high_data;	// CAN-B 수신 High Word Data
//----------------------------------------------------------------------------
*/
//===========================================================//
// 

//---------------------------------------------------------------------------
// InitECan:
//---------------------------------------------------------------------------
// This function initializes the eCAN module to a known state.
//
void InitECan(void)
{
   InitECana();
//#if DSP28_ECANB
   InitECanb();
//#endif // if DSP28_ECANB
}

void InitECana(void)		// Initialize eCAN-A module
{
/* Create a shadow register structure for the CAN control registers. This is
 needed, since only 32-bit access is allowed to these registers. 16-bit access
 to these registers could potentially corrupt the register contents or return 
 false data. This is especially true while writing to/reading from a bit 
 (or group of bits) among bits 16 - 31 */

struct ECAN_REGS ECanaShadow;

	EALLOW;		// EALLOW enables access to protected bits

/* Configure eCAN RX and TX pins for CAN operation using eCAN regs*/  

    ECanaShadow.CANTIOC.all = ECanaRegs.CANTIOC.all;
    ECanaShadow.CANTIOC.bit.TXFUNC = 1;
    ECanaRegs.CANTIOC.all = ECanaShadow.CANTIOC.all;

    ECanaShadow.CANRIOC.all = ECanaRegs.CANRIOC.all;
    ECanaShadow.CANRIOC.bit.RXFUNC = 1;
    ECanaRegs.CANRIOC.all = ECanaShadow.CANRIOC.all;

/* Configure eCAN for HECC mode - (reqd to access mailboxes 16 thru 31) */
									// HECC mode also enables time-stamping feature

	ECanaShadow.CANMC.all = ECanaRegs.CANMC.all;
	ECanaShadow.CANMC.bit.SCB = 1;
	ECanaRegs.CANMC.all = ECanaShadow.CANMC.all;

/* Initialize all bits of 'Master Control Field' to zero */
// Some bits of MSGCTRL register come up in an unknown state. For proper operation,
// all bits (including reserved bits) of MSGCTRL must be initialized to zero

    ECanaMboxes.MBOX0.MSGCTRL.all = 0x00000000;
    ECanaMboxes.MBOX1.MSGCTRL.all = 0x00000000;
    ECanaMboxes.MBOX2.MSGCTRL.all = 0x00000000;
    ECanaMboxes.MBOX3.MSGCTRL.all = 0x00000000;
    ECanaMboxes.MBOX4.MSGCTRL.all = 0x00000000;
    ECanaMboxes.MBOX5.MSGCTRL.all = 0x00000000;
    ECanaMboxes.MBOX6.MSGCTRL.all = 0x00000000;
    ECanaMboxes.MBOX7.MSGCTRL.all = 0x00000000;
    ECanaMboxes.MBOX8.MSGCTRL.all = 0x00000000;
    ECanaMboxes.MBOX9.MSGCTRL.all = 0x00000000;
    ECanaMboxes.MBOX10.MSGCTRL.all = 0x00000000;
    ECanaMboxes.MBOX11.MSGCTRL.all = 0x00000000;
    ECanaMboxes.MBOX12.MSGCTRL.all = 0x00000000;
    ECanaMboxes.MBOX13.MSGCTRL.all = 0x00000000;
    ECanaMboxes.MBOX14.MSGCTRL.all = 0x00000000;
    ECanaMboxes.MBOX15.MSGCTRL.all = 0x00000000;
    ECanaMboxes.MBOX16.MSGCTRL.all = 0x00000000;
    ECanaMboxes.MBOX17.MSGCTRL.all = 0x00000000;
    ECanaMboxes.MBOX18.MSGCTRL.all = 0x00000000;
    ECanaMboxes.MBOX19.MSGCTRL.all = 0x00000000;
    ECanaMboxes.MBOX20.MSGCTRL.all = 0x00000000;
    ECanaMboxes.MBOX21.MSGCTRL.all = 0x00000000;
    ECanaMboxes.MBOX22.MSGCTRL.all = 0x00000000;
    ECanaMboxes.MBOX23.MSGCTRL.all = 0x00000000;
    ECanaMboxes.MBOX24.MSGCTRL.all = 0x00000000;
    ECanaMboxes.MBOX25.MSGCTRL.all = 0x00000000;
    ECanaMboxes.MBOX26.MSGCTRL.all = 0x00000000;
    ECanaMboxes.MBOX27.MSGCTRL.all = 0x00000000;
    ECanaMboxes.MBOX28.MSGCTRL.all = 0x00000000;
    ECanaMboxes.MBOX29.MSGCTRL.all = 0x00000000;
    ECanaMboxes.MBOX30.MSGCTRL.all = 0x00000000;
    ECanaMboxes.MBOX31.MSGCTRL.all = 0x00000000;

// TAn, RMPn, GIFn bits are all zero upon reset and are cleared again
//	as a matter of precaution.

	ECanaRegs.CANTA.all	= 0xFFFFFFFF;	/* Clear all TAn bits */

	ECanaRegs.CANRMP.all = 0xFFFFFFFF;	/* Clear all RMPn bits */

	ECanaRegs.CANGIF0.all = 0xFFFFFFFF;	/* Clear all interrupt flag bits */
	ECanaRegs.CANGIF1.all = 0xFFFFFFFF;


/* Configure bit timing parameters for eCANA*/
	ECanaShadow.CANMC.all = ECanaRegs.CANMC.all;
	ECanaShadow.CANMC.bit.CCR = 1 ;            // Set CCR = 1
    ECanaRegs.CANMC.all = ECanaShadow.CANMC.all;

    ECanaShadow.CANES.all = ECanaRegs.CANES.all;

    do
	{
	    ECanaShadow.CANES.all = ECanaRegs.CANES.all;
    } while(ECanaShadow.CANES.bit.CCE != 1 );  		// Wait for CCE bit to be set..

    ECanaShadow.CANBTC.all = 0;

    // TMX 최신 DataSheet에서는 eCAN의 동작 입력 클럭은 SYSCLKOUT을 2분주한 클럭을 사용한다고 함.
	// 그러나 eCAN User Manual에서는 직접 SYSCLKOUT을 사용한다고 함.
	// 테스트 결과 eCAN의 동작 입력 클럭은 SYSCLKOUT을 2분주한 클럭을 사용함.
	// 따라서 아래와 같이 TI의 SDK v1.10에서 제공하는 DSP2833x_ECan.c 파일을 수정함.
    #if (CPU_FRQ_150MHZ)                       // CPU_FRQ_150MHz is defined in DSP2833x_Examples.h
    	/* The following block for all 150 MHz SYSCLKOUT - default. Bit rate = 1 Mbps */
		/*
		ECanaShadow.CANBTC.bit.BRPREG = 9;
		ECanaShadow.CANBTC.bit.TSEG2REG = 2;
		ECanaShadow.CANBTC.bit.TSEG1REG = 10;
		*/

		// Bit Rate = (SYSCLKOUT/2) / (BRP * Bit Time)
		// BRP      = BRPreg + 1
		// Bit Time = (TSEG1reg + 1) + (TSEG2reg + 1) + 1
//---1m 설정
		//ECanaShadow.CANBTC.bit.BRPREG = 4;
		//ECanaShadow.CANBTC.bit.TSEG2REG = 2;
		//ECanaShadow.CANBTC.bit.TSEG1REG = 10;
//---0.5m 설정
		ECanaShadow.CANBTC.bit.BRPREG = 9;//4;
		ECanaShadow.CANBTC.bit.TSEG2REG = 2;
		ECanaShadow.CANBTC.bit.TSEG1REG = 10;
    #endif
	#if (CPU_FRQ_100MHZ)                       // CPU_FRQ_100MHz is defined in DSP2833x_Examples.h
		/* The following block is only for 100 MHz SYSCLKOUT. Bit rate = 1 Mbps */
		/*
	    ECanaShadow.CANBTC.bit.BRPREG = 9;
		ECanaShadow.CANBTC.bit.TSEG2REG = 1;
		ECanaShadow.CANBTC.bit.TSEG1REG = 6;
		*/

		// Bit Rate = (SYSCLKOUT/2) / (BRP * Bit Time)
		// BRP      = BRPreg + 1
		// Bit Time = (TSEG1reg + 1) + (TSEG2reg + 1) + 1
		ECanaShadow.CANBTC.bit.BRPREG = 4;
		ECanaShadow.CANBTC.bit.TSEG2REG = 1;
		ECanaShadow.CANBTC.bit.TSEG1REG = 6;
	#endif


    ECanaShadow.CANBTC.bit.SAM = 1;
    ECanaRegs.CANBTC.all = ECanaShadow.CANBTC.all;

    ECanaShadow.CANMC.all = ECanaRegs.CANMC.all;
	ECanaShadow.CANMC.bit.CCR = 0 ;            // Set CCR = 0
    ECanaRegs.CANMC.all = ECanaShadow.CANMC.all;

    ECanaShadow.CANES.all = ECanaRegs.CANES.all;

    do
    {
       ECanaShadow.CANES.all = ECanaRegs.CANES.all;
    } while(ECanaShadow.CANES.bit.CCE != 0 ); 		// Wait for CCE bit to be  cleared..

/* Disable all Mailboxes  */
 	ECanaRegs.CANME.all = 0;		// Required before writing the MSGIDs

    EDIS;
}


//#if (DSP28_ECANB)
void InitECanb(void)		// Initialize eCAN-B module
{
/* Create a shadow register structure for the CAN control registers. This is
 needed, since only 32-bit access is allowed to these registers. 16-bit access
 to these registers could potentially corrupt the register contents or return 
 false data. This is especially true while writing to/reading from a bit 
 (or group of bits) among bits 16 - 31 */

struct ECAN_REGS ECanbShadow;

   EALLOW;		// EALLOW enables access to protected bits

/* Configure eCAN RX and TX pins for CAN operation using eCAN regs*/  

    ECanbShadow.CANTIOC.all = ECanbRegs.CANTIOC.all;
    ECanbShadow.CANTIOC.bit.TXFUNC = 1;
    ECanbRegs.CANTIOC.all = ECanbShadow.CANTIOC.all;

    ECanbShadow.CANRIOC.all = ECanbRegs.CANRIOC.all;
    ECanbShadow.CANRIOC.bit.RXFUNC = 1;
    ECanbRegs.CANRIOC.all = ECanbShadow.CANRIOC.all;

/* Configure eCAN for HECC mode - (reqd to access mailboxes 16 thru 31) */

	ECanbShadow.CANMC.all = ECanbRegs.CANMC.all;
	ECanbShadow.CANMC.bit.SCB = 1;
	ECanbRegs.CANMC.all = ECanbShadow.CANMC.all;

/* Initialize all bits of 'Master Control Field' to zero */
// Some bits of MSGCTRL register come up in an unknown state. For proper operation,
// all bits (including reserved bits) of MSGCTRL must be initialized to zero

    ECanbMboxes.MBOX0.MSGCTRL.all = 0x00000000;
    ECanbMboxes.MBOX1.MSGCTRL.all = 0x00000000;
    ECanbMboxes.MBOX2.MSGCTRL.all = 0x00000000;
    ECanbMboxes.MBOX3.MSGCTRL.all = 0x00000000;
    ECanbMboxes.MBOX4.MSGCTRL.all = 0x00000000;
    ECanbMboxes.MBOX5.MSGCTRL.all = 0x00000000;
    ECanbMboxes.MBOX6.MSGCTRL.all = 0x00000000;
    ECanbMboxes.MBOX7.MSGCTRL.all = 0x00000000;
    ECanbMboxes.MBOX8.MSGCTRL.all = 0x00000000;
    ECanbMboxes.MBOX9.MSGCTRL.all = 0x00000000;
    ECanbMboxes.MBOX10.MSGCTRL.all = 0x00000000;
    ECanbMboxes.MBOX11.MSGCTRL.all = 0x00000000;
    ECanbMboxes.MBOX12.MSGCTRL.all = 0x00000000;
    ECanbMboxes.MBOX13.MSGCTRL.all = 0x00000000;
    ECanbMboxes.MBOX14.MSGCTRL.all = 0x00000000;
    ECanbMboxes.MBOX15.MSGCTRL.all = 0x00000000;
    ECanbMboxes.MBOX16.MSGCTRL.all = 0x00000000;
    ECanbMboxes.MBOX17.MSGCTRL.all = 0x00000000;
    ECanbMboxes.MBOX18.MSGCTRL.all = 0x00000000;
    ECanbMboxes.MBOX19.MSGCTRL.all = 0x00000000;
    ECanbMboxes.MBOX20.MSGCTRL.all = 0x00000000;
    ECanbMboxes.MBOX21.MSGCTRL.all = 0x00000000;
    ECanbMboxes.MBOX22.MSGCTRL.all = 0x00000000;
    ECanbMboxes.MBOX23.MSGCTRL.all = 0x00000000;
    ECanbMboxes.MBOX24.MSGCTRL.all = 0x00000000;
    ECanbMboxes.MBOX25.MSGCTRL.all = 0x00000000;
    ECanbMboxes.MBOX26.MSGCTRL.all = 0x00000000;
    ECanbMboxes.MBOX27.MSGCTRL.all = 0x00000000;
    ECanbMboxes.MBOX28.MSGCTRL.all = 0x00000000;
    ECanbMboxes.MBOX29.MSGCTRL.all = 0x00000000;
    ECanbMboxes.MBOX30.MSGCTRL.all = 0x00000000;
    ECanbMboxes.MBOX31.MSGCTRL.all = 0x00000000;

// TAn, RMPn, GIFn bits are all zero upon reset and are cleared again
//	as a matter of precaution.

	ECanbRegs.CANTA.all	= 0xFFFFFFFF;	/* Clear all TAn bits */

	ECanbRegs.CANRMP.all = 0xFFFFFFFF;	/* Clear all RMPn bits */

	ECanbRegs.CANGIF0.all = 0xFFFFFFFF;	/* Clear all interrupt flag bits */
	ECanbRegs.CANGIF1.all = 0xFFFFFFFF;


/* Configure bit timing parameters for eCANB*/

	ECanbShadow.CANMC.all = ECanbRegs.CANMC.all;
	ECanbShadow.CANMC.bit.CCR = 1 ;            // Set CCR = 1
    ECanbRegs.CANMC.all = ECanbShadow.CANMC.all;

    ECanbShadow.CANES.all = ECanbRegs.CANES.all;

    do
	{
	    ECanbShadow.CANES.all = ECanbRegs.CANES.all;
	} while(ECanbShadow.CANES.bit.CCE != 1 ); 		// Wait for CCE bit to be  cleared..


    ECanbShadow.CANBTC.all = 0;

    // TMX 최신 DataSheet에서는 eCAN의 동작 입력 클럭은 SYSCLKOUT을 2분주한 클럭을 사용한다고 함.
	// 그러나 eCAN User Manual에서는 직접 SYSCLKOUT을 사용한다고 함.
	// 테스트 결과 eCAN의 동작 입력 클럭은 SYSCLKOUT을 2분주한 클럭을 사용함.
    #if (CPU_FRQ_150MHZ)                       // CPU_FRQ_150MHz is defined in DSP2833x_Examples.h
		/* The following block for all 150 MHz SYSCLKOUT - default. Bit rate = 1 Mbps */
		/*
		ECanbShadow.CANBTC.bit.BRPREG = 9;
		ECanbShadow.CANBTC.bit.TSEG2REG = 2;
		ECanbShadow.CANBTC.bit.TSEG1REG = 10;
		*/

		// Bit Rate = (SYSCLKOUT/2) / (BRP * Bit Time)
		// BRP      = BRPreg + 1
		// Bit Time = (TSEG1reg + 1) + (TSEG2reg + 1) + 1
//---1m 설정
		//ECanbShadow.CANBTC.bit.BRPREG = 4;
		//ECanbShadow.CANBTC.bit.TSEG2REG = 2;
		//ECanbShadow.CANBTC.bit.TSEG1REG = 10;
//---0.5m 설정

		ECanbShadow.CANBTC.bit.BRPREG = 9;
		ECanbShadow.CANBTC.bit.TSEG2REG = 2;
		ECanbShadow.CANBTC.bit.TSEG1REG = 10;
	#endif
	#if (CPU_FRQ_100MHZ)                       // CPU_FRQ_100MHz is defined in DSP2833x_Examples.h
		/* The following block is only for 100 MHz SYSCLKOUT. Bit rate = 1 Mbps */
	    /*
	    ECanbShadow.CANBTC.bit.BRPREG = 9;
		ECanbShadow.CANBTC.bit.TSEG2REG = 1;
		ECanbShadow.CANBTC.bit.TSEG1REG = 6;
		*/

		// Bit Rate = (SYSCLKOUT/2) / (BRP * Bit Time)
		// BRP      = BRPreg + 1
		// Bit Time = (TSEG1reg + 1) + (TSEG2reg + 1) + 1
		ECanbShadow.CANBTC.bit.BRPREG = 4;
		ECanbShadow.CANBTC.bit.TSEG2REG = 1;
		ECanbShadow.CANBTC.bit.TSEG1REG = 6;
	#endif

    ECanbShadow.CANBTC.bit.SAM = 1;
    ECanbRegs.CANBTC.all = ECanbShadow.CANBTC.all;

    ECanbShadow.CANMC.all = ECanbRegs.CANMC.all;
	ECanbShadow.CANMC.bit.CCR = 0 ;            // Set CCR = 0
    ECanbRegs.CANMC.all = ECanbShadow.CANMC.all;

    ECanbShadow.CANES.all = ECanbRegs.CANES.all;

    do
    {
        ECanbShadow.CANES.all = ECanbRegs.CANES.all;
    } while(ECanbShadow.CANES.bit.CCE != 0 ); 		// Wait for CCE bit to be  cleared..


/* Disable all Mailboxes  */
 	ECanbRegs.CANME.all = 0;		// Required before writing the MSGIDs

    EDIS;
}
//#endif // if DSP28_ECANB


//---------------------------------------------------------------------------
// Example: InitECanGpio:
//---------------------------------------------------------------------------
// This function initializes GPIO pins to function as eCAN pins
//
// Each GPIO pin can be configured as a GPIO pin or up to 3 different
// peripheral functional pins. By default all pins come up as GPIO
// inputs after reset.
//
// Caution:
// Only one GPIO pin should be enabled for CANTXA/B operation.
// Only one GPIO pin shoudl be enabled for CANRXA/B operation.
// Comment out other unwanted lines.


void InitECanGpio(void)
{
   InitECanaGpio();
//#if (DSP28_ECANB)
   InitECanbGpio();
//#endif // if DSP28_ECANB
}

void InitECanaGpio(void)
{
   EALLOW;

/* Enable internal pull-up for the selected CAN pins */
// Pull-ups can be enabled or disabled by the user.
// This will enable the pullups for the specified pins.
// Comment out other unwanted lines.

	//GpioCtrlRegs.GPAPUD.bit.GPIO30 = 0;	    // Enable pull-up for GPIO30 (CANRXA)
	GpioCtrlRegs.GPAPUD.bit.GPIO18 = 0;	    // Enable pull-up for GPIO18 (CANRXA)

	//GpioCtrlRegs.GPAPUD.bit.GPIO31 = 0;	    // Enable pull-up for GPIO31 (CANTXA)
	GpioCtrlRegs.GPAPUD.bit.GPIO19 = 0;	    // Enable pull-up for GPIO19 (CANTXA)

/* Set qualification for selected CAN pins to asynch only */
// Inputs are synchronized to SYSCLKOUT by default.
// This will select asynch (no qualification) for the selected pins.

    //GpioCtrlRegs.GPAQSEL2.bit.GPIO30 = 3;   // Asynch qual for GPIO30 (CANRXA)
	GpioCtrlRegs.GPAQSEL2.bit.GPIO18 = 3;   // Asynch qual for GPIO18 (CANRXA)


/* Configure eCAN-A pins using GPIO regs*/
// This specifies which of the possible GPIO pins will be eCAN functional pins.

	//GpioCtrlRegs.GPAMUX2.bit.GPIO30 = 1;	// Configure GPIO30 for CANRXA operation
  GpioCtrlRegs.GPAMUX2.bit.GPIO18 = 3;	// Configure GPIO18 for CANRXA operation
	//GpioCtrlRegs.GPAMUX2.bit.GPIO31 = 1;	// Configure GPIO31 for CANTXA operation
  GpioCtrlRegs.GPAMUX2.bit.GPIO19 = 3;	// Configure GPIO19 for CANTXA operation

    EDIS;
}

//#if (DSP28_ECANB)
void InitECanbGpio(void)
{
   EALLOW;

/* Enable internal pull-up for the selected CAN pins */
// Pull-ups can be enabled or disabled by the user.
// This will enable the pullups for the specified pins.
// Comment out other unwanted lines.

//	GpioCtrlRegs.GPAPUD.bit.GPIO8 = 0;	  // Enable pull-up for GPIO8  (CANTXB)
//  GpioCtrlRegs.GPAPUD.bit.GPIO12 = 0;   // Enable pull-up for GPIO12 (CANTXB)
	//GpioCtrlRegs.GPAPUD.bit.GPIO16 = 0;   // Enable pull-up for GPIO16 (CANTXB)
  GpioCtrlRegs.GPAPUD.bit.GPIO20 = 0;   // Enable pull-up for GPIO20 (CANTXB)

//	GpioCtrlRegs.GPAPUD.bit.GPIO10 = 0;	  // Enable pull-up for GPIO10 (CANRXB)
//  GpioCtrlRegs.GPAPUD.bit.GPIO13 = 0;   // Enable pull-up for GPIO13 (CANRXB)
	//GpioCtrlRegs.GPAPUD.bit.GPIO17 = 0;   // Enable pull-up for GPIO17 (CANRXB)
  GpioCtrlRegs.GPAPUD.bit.GPIO21 = 0;   // Enable pull-up for GPIO21 (CANRXB)

/* Set qualification for selected CAN pins to asynch only */
// Inputs are synchronized to SYSCLKOUT by default.
// This will select asynch (no qualification) for the selected pins.
// Comment out other unwanted lines.

//	GpioCtrlRegs.GPAQSEL1.bit.GPIO10 = 3; // Asynch qual for GPIO10 (CANRXB)
//  GpioCtrlRegs.GPAQSEL1.bit.GPIO13 = 3; // Asynch qual for GPIO13 (CANRXB)
	//GpioCtrlRegs.GPAQSEL2.bit.GPIO17 = 3; // Asynch qual for GPIO17 (CANRXB)
//	//GpioCtrlRegs.GPAQSEL2.bit.GPIO16 = 3; // Asynch qual for GPIO17 (CANRXB)
  GpioCtrlRegs.GPAQSEL2.bit.GPIO21 = 3; // Asynch qual for GPIO21 (CANRXB)

/* Configure eCAN-B pins using GPIO regs*/
// This specifies which of the possible GPIO pins will be eCAN functional pins.

//	GpioCtrlRegs.GPAMUX1.bit.GPIO8 = 2;   // Configure GPIO8 for CANTXB operation
//  GpioCtrlRegs.GPAMUX1.bit.GPIO12 = 2;  // Configure GPIO12 for CANTXB operation
	//GpioCtrlRegs.GPAMUX2.bit.GPIO16 = 2;  // Configure GPIO16 for CANTXB operation
  GpioCtrlRegs.GPAMUX2.bit.GPIO20 = 3;  // Configure GPIO20 for CANTXB operation

//	GpioCtrlRegs.GPAMUX1.bit.GPIO10 = 2;  // Configure GPIO10 for CANRXB operation
//  GpioCtrlRegs.GPAMUX1.bit.GPIO13 = 2;  // Configure GPIO13 for CANRXB operation
	//GpioCtrlRegs.GPAMUX2.bit.GPIO17 = 2;  // Configure GPIO17 for CANRXB operation
  GpioCtrlRegs.GPAMUX2.bit.GPIO21 = 3;  // Configure GPIO21 for CANRXB operation

    EDIS;
}
//#endif // if DSP28_ECANB

/******************************************************/
/* Bit configuration parameters for 150 MHz SYSCLKOUT */
/******************************************************/
/*

The table below shows how BRP(reg) field must be changed to achieve different bit
rates with a BT of 15, for a 80% SP:
---------------------------------------------------------------
BT = 15, TSEG1(reg) = 10, TSEG2(reg) = 2, Sampling Point = 80%
---------------------------------------------------------------
1   Mbps : BRP(reg)+1 = 10 		: CAN clock = 15 MHz
500 kbps : BRP(reg)+1 = 20 		: CAN clock = 7.5 MHz
250 kbps : BRP(reg)+1 = 40 		: CAN clock = 3.75 MHz
125 kbps : BRP(reg)+1 = 80 		: CAN clock = 1.875 MHz
100 kbps : BRP(reg)+1 = 100 	: CAN clock = 1.5 MHz
50  kbps : BRP(reg)+1 = 200 	: CAN clock = 0.75 MHz

The table below shows how to achieve different sampling points with a BT of 15:
----------------------------------------------------------------------
Achieving desired SP by changing TSEG1(reg) & TSEG2(reg) with BT = 15
----------------------------------------------------------------------

TSEG1(reg) = 10, TSEG2(reg) = 2, SP = 80%
TSEG1(reg) = 9, TSEG2(reg) = 3, SP = 73%
TSEG1(reg) = 8, TSEG2(reg) = 4, SP = 66%
TSEG1(reg) = 7, TSEG2(reg) = 5, SP = 60%

The table below shows how BRP(reg) field must be changed to achieve different bit
rates with a BT of 10, for a 80% SP:
--------------------------------------------------------------
BT = 10, TSEG1(reg) = 6, TSEG2(reg) = 1, Sampling Point = 80%
--------------------------------------------------------------
1   Mbps : BRP(reg)+1 = 15 	: CAN clock = 10 MHz
500 kbps : BRP(reg)+1 = 30 	: CAN clock = 5 MHz
250 kbps : BRP(reg)+1 = 60 	: CAN clock = 2.5 MHz
125 kbps : BRP(reg)+1 = 120	: CAN clock = 1.25 MHz
100 kbps : BRP(reg)+1 = 150 : CAN clock = 1 MHz
50  kbps : BRP(reg)+1 = 300 : CAN clock = 0.5 MHz

*/

/***************************************************/
/* Bit configuration parameters for 100 MHz SYSCLKOUT*/
/***************************************************/
/*

The table below shows how BRP(reg) field must be changed to achieve different bit
rates with a BT of 10, for a 80% SP:
---------------------------------------------------
BT = 10, TSEG1 = 6, TSEG2 = 1, Sampling Point = 80%
---------------------------------------------------
1   Mbps : BRP(reg)+1 = 10 	: CAN clock = 10 MHz
500 kbps : BRP(reg)+1 = 20 	: CAN clock = 5 MHz
250 kbps : BRP(reg)+1 = 40 	: CAN clock = 2.5 MHz
125 kbps : BRP(reg)+1 = 80 	: CAN clock = 1.25 MHz
100 kbps : BRP(reg)+1 = 100 : CAN clock = 1 MHz
50  kbps : BRP(reg)+1 = 200 : CAN clock = 0.5 MHz

The table below shows how BRP(reg) field must be changed to achieve different bit
rates with a BT of 20:
---------------------------------------------------
BT = 20
---------------------------------------------------
1   Mbps : BRP(reg)+1 = 5
500 kbps : BRP(reg)+1 = 10
250 kbps : BRP(reg)+1 = 20
125 kbps : BRP(reg)+1 = 40
100 kbps : BRP(reg)+1 = 50
50  kbps : BRP(reg)+1 = 100

The table below shows how to achieve different sampling points with a BT of 20:
------------------------------------------------------------
Achieving desired SP by changing TSEG1 & TSEG2 with BT = 20
------------------------------------------------------------

TSEG1(reg) = 15, TSEG2(reg) = 2, SP = 85%
TSEG1(reg) = 14, TSEG2(reg) = 3, SP = 80%
TSEG1(reg) = 13, TSEG2(reg) = 4, SP = 75%
TSEG1(reg) = 12, TSEG2(reg) = 5, SP = 70%
TSEG1(reg) = 11, TSEG2(reg) = 6, SP = 65%
TSEG1(reg) = 10, TSEG2(reg) = 7, SP = 60%

Note: BRP(reg), TSEG1(reg) & TSEG2(reg) indicate the actual value that is written
 into the bit fields of the CAN control registers. These values are increased by 1
 by the CAN module when these registers are accessed.

*/

//===========================================================================
// End of file.
//===========================================================================





//==========================================================//
// CAN-A MBOX1을 사용하여 메시지를 전송함
void SendDataToECanA(LONG id, BYTE length, LONG low_data, LONG high_data)
{
	test_can1++;
	if(ECanaRegs.CANTRS.bit.TRS1 == 1) return; // 현재 CAN-A MBOX1가 전송 중(TRS1==1)이면 리턴함.
	test_can2++;
	ECanaRegs.CANME.bit.ME1 = 0;
	ECanaMboxes.MBOX1.MSGID.all = 0x80000000 | (id & 0x1fffffff);
//	ECanaMboxes.MBOX1.MSGID.all = 0x40000000 | (id & 0x1fffffff);
//	ECanaMboxes.MBOX1.MSGID.all = 0x00000000 | (id & 0x1fffffff);
	ECanaRegs.CANME.bit.ME1 = 1;

	ECanaMboxes.MBOX1.MSGCTRL.bit.DLC = length & 0xf;
	ECanaMboxes.MBOX1.MDL.all = low_data;
	ECanaMboxes.MBOX1.MDH.all = high_data;
	
	//ECanaRegs.CANTRS.bit.TRS1 = 0;//임시로..
	ECanaRegs.CANTRS.bit.TRS1 = 1;// 현재 CAN-A MBOX1 전송시킴(TRS1=1).



}

// CAN-B MBOX1을 사용하여 메시지를 전송함
void SendDataToECanB(LONG id, BYTE length, LONG low_data, LONG high_data)
{
	if(ECanbRegs.CANTRS.bit.TRS1 == 1) return;// 현재 CAN-B MBOX1가 전송 중(TRS1==1)이면 리턴함.
	
	ECanbRegs.CANME.bit.ME1 = 0;
	ECanbMboxes.MBOX1.MSGID.all = 0x80000000 | (id & 0x1fffffff);
	//ECanbMboxes.MBOX1.MSGID.all = 0x40000000 | (id & 0x1fffffff);
	//ECanbMboxes.MBOX1.MSGID.all = 0x00000000 | (id & 0x1fffffff);
	ECanbRegs.CANME.bit.ME1 = 1;
	
	ECanbMboxes.MBOX1.MSGCTRL.bit.DLC = length & 0xf;
	ECanbMboxes.MBOX1.MDL.all = low_data;
	ECanbMboxes.MBOX1.MDH.all = high_data;

	
	ECanbRegs.CANTRS.bit.TRS1 = 1;// 현재 CAN-B MBOX1 전송시킴(TRS1=1)

}

void init_can(void){
	// Initialize CAN-A/B peripheral with 1M Bps
	// refer to DSP2833x_ECan.c file
	// [ 알림 ]
	// TMX 최신 DataSheet에서는 eCAN의 동작 입력 클럭은 SYSCLKOUT을 2분주한 클럭을 사용한다고 함.
	// 그러나 eCAN User Manual에서는 직접 SYSCLKOUT� 사용한다고 함.
	// 테스트 결과 eCAN의 동작 입력 클럭은 SYSCLKOUT을 2분주한 클럭을 사용함.
	// 따라서 아래와 같이 TI의 SDK v1.10에서 제공하는 DSP2833x_ECan.c 파일을 수정함.
	InitECanGpio();
	InitECan();
	
	// Initialize CAN-A Transmiter/Receiver MailBox
	// CAN-A MBOX0 -> Receiver MailBox
	// CAN-A MBOX1 -> Transmiter MailBox
	ECanaLAMRegs.LAM0.all = 0x1fffffff; 	  // MBOX1 acceptance mask(IDE match, IDs is don't care)

	//LSH	
	ECanaMboxes.MBOX0.MSGID.all = 0xc0000000; // IDE=1, AME=1
//	ECanaMboxes.MBOX0.MSGID.all = 0x40000000; // IDE=1, AME=1
	//ECanaMboxes.MBOX0.MSGID.bit.IDE = 0; // IDE=0
	
	ECanaRegs.CANME.bit.ME0 = 1;
	
	ECanaRegs.CANMD.bit.MD0 = 1; // Receiver
	ECanaRegs.CANMD.bit.MD1 = 0; // Transmiter
	
	// Initialize CAN-B Transmiter/Receiver MailBox
	// CAN-B MBOX0 -> Receiver MailBox
	// CAN-B MBOX1 -> Transmiter MailBox
	ECanbLAMRegs.LAM0.all = 0x1fffffff; 	  // MBOX1 acceptance mask(IDE match, IDs is don't care)
	//LSH
	ECanbMboxes.MBOX0.MSGID.all = 0xc0000000; // IDE=1, AME=1
	//ECanbMboxes.MBOX0.MSGID.all = 0x40000000; // IDE=1, AME=1
	//ECanbMboxes.MBOX0.MSGID.bit.IDE = 0; // IDE=0
	
	ECanbRegs.CANME.bit.ME0 = 1;
	
	ECanbRegs.CANMD.bit.MD0 = 1; // Receiver
	ECanbRegs.CANMD.bit.MD1 = 0; // Transmiter
	
	// Initialize CAN-A/B interrupt vector
	EALLOW;
	PieVectTable.ECAN0INTA = &ecan0_inta_isr;
	PieVectTable.ECAN1INTA = &ecan1_inta_isr;
	PieVectTable.ECAN0INTB = &ecan0_intb_isr;
	PieVectTable.ECAN1INTB = &ecan1_intb_isr;
	EDIS;

	EALLOW;
	// Initialize CAN-A/B Receiver and Transmiter MailBox interrupt level
	// CAN-A/B Receiver interrupt level   -> 0
	// CAN-A/B Transmiter interrupt level -> 1
	ECanaRegs.CANMIL.bit.MIL0 = 0;
	ECanaRegs.CANMIL.bit.MIL1 = 1;
	ECanbRegs.CANMIL.bit.MIL0 = 0;
	ECanbRegs.CANMIL.bit.MIL1 = 1;

	// Initialize CAN-A/B Receiver and Transmiter MailBox interrupt mask
	ECanaRegs.CANMIM.bit.MIM0 = 1;
	ECanaRegs.CANMIM.bit.MIM1 = 1;
	ECanbRegs.CANMIM.bit.MIM0 = 1;
	ECanbRegs.CANMIM.bit.MIM1 = 1;
	
	// Initialize CAN-A/B Global interrupt mask
	ECanaRegs.CANGIM.bit.I0EN = 1;
	ECanaRegs.CANGIM.bit.I1EN = 1;
	ECanbRegs.CANGIM.bit.I0EN = 1;
	ECanbRegs.CANGIM.bit.I1EN = 1;
	EDIS;

	// Enable CPU INT9 for CAN-A/B interrupt
	IER |= M_INT9;
	
    // Enable CAN-A/B INT0/1 in the PIE: Group 9 interrupt 5/6/7/8
	PieCtrlRegs.PIEIER9.bit.INTx5 = 1;
	PieCtrlRegs.PIEIER9.bit.INTx6 = 1;
	PieCtrlRegs.PIEIER9.bit.INTx7 = 1;
	PieCtrlRegs.PIEIER9.bit.INTx8 = 1;
}

// CAN-A MBOX0 수신 메시지 처리를 위한 인터럽트
interrupt void ecan0_inta_isr(void){
	canA_RX_Counter++;
	cana_rx_flag = 0;//우선 테스트용으로 
	if(ECanaRegs.CANRMP.bit.RMP0 == 1){
		if(!cana_rx_flag){
			cana_rx_id = ECanaMboxes.MBOX0.MSGID.all & 0x1fffffff;
			cana_rx_length = ECanaMboxes.MBOX0.MSGCTRL.bit.DLC;
			cana_rx_low_data = ECanaMboxes.MBOX0.MDL.all;
			cana_rx_high_data = ECanaMboxes.MBOX0.MDH.all;
			cana_rx_flag = TRUE;
			
// eCAN-A/B 처리를 위한 변수 ------------------------------------------------
			WGcana_rx_flag = cana_rx_flag;	// 사용자가 CAN-A 수신받은 데이타 저장한후(0), 아직 저장아님(1)
			WGcana_rx_id = cana_rx_id;			// CAN-A 수신 ID
			WGcana_rx_length = cana_rx_length;    	// CAN-A 수신 데이터 크기
			WGcana_rx_low_data = cana_rx_low_data;	// CAN-A 수신 Low Word Data
			WGcana_rx_high_data = cana_rx_high_data;	// CAN-A 수신 High Word Data	
		//	WGcana_rx_flag = cana_rx_flag;		
			RX_CANA_From_CommBoard();	// 통신보드 또는 MCU CANB로부터 수신
		}
		
		// Clear receive pending flag
		ECanaRegs.CANRMP.bit.RMP0 = 1;	
	}	

	// Acknowledge this interrupt to receive more interrupts from group 9
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP9;
}

// CAN-A MBOX1 송신 메시지 처리를 위한 인터럽트
interrupt void ecan1_inta_isr(void){
//ELED_TP2_TOGGLE();	
	canA_TX_Counter++;
	
	if(ECanaRegs.CANTA.bit.TA1 == 1){
		
		// Clear transmit-acknowledge pending flag
		ECanaRegs.CANTA.bit.TA1 = 1;
	}

	// Acknowledge this interrupt to receive more interrupts from group 9
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP9;
}

// CAN-B MBOX0 수신 메시지 처리를 위한 인터럽트
interrupt void ecan0_intb_isr(void){
	
	canB_RX_Counter++;
	canb_rx_flag = 0;//우선 테스트용으로 	
	
	if(ECanbRegs.CANRMP.bit.RMP0 == 1){
		if(!canb_rx_flag){
	canB_RX_Counter1++;			
			canb_rx_id = ECanbMboxes.MBOX0.MSGID.all & 0x1fffffff;
			canb_rx_length = ECanbMboxes.MBOX0.MSGCTRL.bit.DLC;
			canb_rx_low_data = ECanbMboxes.MBOX0.MDL.all;
			canb_rx_high_data = ECanbMboxes.MBOX0.MDH.all;
			canb_rx_flag = TRUE;	// can 
// eCAN-A/B 처리를 위한 변수 ------------------------------------------------
			WGcanb_rx_flag = canb_rx_flag;	// 사용자가 CAN-A 수신받은 데이타 저장한후(0), 아직 저장아님(1)
			WGcanb_rx_id = canb_rx_id;			// CAN-A 수신 ID
			WGcanb_rx_length = canb_rx_length;    	// CAN-A 수신 데이터 크기
			WGcanb_rx_low_data = canb_rx_low_data;	// CAN-A 수신 Low Word Data
			WGcanb_rx_high_data = canb_rx_high_data;	// CAN-A 수신 High Word Data			
			//RX_CANB_From_McuCanA();// MCU CANA로부터 수신
		}
		
		// Clear receive pending flag
		ECanbRegs.CANRMP.bit.RMP0 = 1;	
	}

	// Acknowledge this interrupt to receive more interrupts from group 9
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP9;
}

// CAN-B MBOX1 송신 메시지 처리를 위한 인터럽트
interrupt void ecan1_intb_isr(void){
	
	canB_TX_Counter++;
	
	if(ECanbRegs.CANTA.bit.TA1 == 1){
		
		// Clear transmit-acknowledge pending flag
		ECanbRegs.CANTA.bit.TA1 = 1;
	}

	// Acknowledge this interrupt to receive more interrupts from group 9
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP9;
}
