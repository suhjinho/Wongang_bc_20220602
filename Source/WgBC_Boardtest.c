//==================================================================================//
// ������ ������(30kw)/����(50kw) ������ ������ ���(10kw) ���α׷�	           		//
//==================================================================================//

#include "Global_Variable_Function.h"

extern Uint16 msec_tmr;	
extern Uint16 msec_1000m,msec_500m,msec_200m, msec_10m;
extern Uint32 msec_1m;



//====================================================//
// ���� �׽�Ʈ�� ������...
//====================================================//
//---DSP LED, SWITCH �˻��

Uint16 TEST_DSP_MODESW_IN = 0;	// TEST/NORMAL MODE ���ý���ġ, ������ ��ġ
Uint16 TEST_DSP_HEXSW_IN = 0;	// HEX ROTARY SWITCH, ������ ��ġ <-- ID�� ����ϸ� �ɵ�..
Uint16 TEST_DSP_DIPSW_IN = 0;   // DIP SWITCH 2��Ʈ, 
Uint16 TEST_DSP_PUSHBUTTONSW_IN = 0; // PUSH BUTTON SWITCH
//---EPLD LED 4EA �˻��
unsigned char TEST_EPLD_ELED = 0xff;
//---EPLD LED 24EA �˻��
unsigned char TEST_EPLD_LED1 = 0x1;
unsigned char TEST_EPLD_LED2 = 0x1;
unsigned char TEST_EPLD_LED3 = 0x1;
//---EPLD DOUT1 8EA �˻��
unsigned char TEST_EPLD_DOUT1 = 0x1;
//---EPLD DIN1,2 16EA �˻��
unsigned char TEST_EPLD_DIN1 = 0x0;
unsigned char TEST_EPLD_DIN2 = 0x0;
//---EPLD HWF1,2 12EA �˻��
unsigned char TEST_EPLD_HWF1 = 0x0;
unsigned char TEST_EPLD_HWF2 = 0x0;
//---NVRAM �˻��
Uint32 TEST_NVRAM_WAddr = 0x200001;
Uint32 TEST_NVRAM_RAddr = 0x200001;

Uint32 TEST_NVRAM_WAddr1 = 0x201001;
Uint32 TEST_NVRAM_RAddr1 = 0x201001;

Uint16 TEST_NVRAM_WData = 0;
Uint16 TEST_NVRAM_WData1 = 0;
Uint16 TEST_NVRAM_RData = 0; 


Uint16 TEST_NVRAM_RTC_YEAR_r = 0;
Uint16 TEST_NVRAM_RTC_MONTH_r = 0;
Uint16 TEST_NVRAM_RTC_DATE_r = 0;
Uint16 TEST_NVRAM_RTC_DAY_r = 0;
Uint16 TEST_NVRAM_RTC_HOUR_r = 0;
Uint16 TEST_NVRAM_RTC_MIN_r = 0;
Uint16 TEST_NVRAM_RTC_SECOND_r = 0;
Uint16 TEST_NVRAM_RTC_CENTURY_r = 0;

Uint16 TEST_NVRAM_RTC_MIN_rh = 0;
Uint16 TEST_NVRAM_RTC_MIN_rl = 0;
Uint16 TEST_NVRAM_RTC_SECOND_rh = 0;
Uint16 TEST_NVRAM_RTC_SECOND_rl = 0;

Uint16 TEST_NVRAM_RTC_YEAR_w = 0;
Uint16 TEST_NVRAM_RTC_MONTH_w = 0;
Uint16 TEST_NVRAM_RTC_DATE_w = 0;
Uint16 TEST_NVRAM_RTC_DAY_w = 0;
Uint16 TEST_NVRAM_RTC_HOUR_w = 0;
Uint16 TEST_NVRAM_RTC_MIN_w = 0;
Uint16 TEST_NVRAM_RTC_SECOND_w = 0;
Uint16 TEST_NVRAM_RTC_CENTURY_w = 0;

//---DSP EEPROM �˻��
Uint16 TEST_DSP_EEPROM_WriteFlag = 0; 	// eeprom ����Ÿ ����, �ѹ� ���Ŀ��� 0���� �������(��� ���� eeprom����..)
Uint16 TEST_DSP_EEPROM_WAddr = 0; 		// eeprom�� �� ����
Uint16 TEST_DSP_EEPROM_WData = 0; 		// eeprom�� �� ����Ÿ
Uint16 TEST_DSP_EEPROM_RData = 0; 		// eeprom�� ���ذ� ���� ����Ÿ

//---DSP �ܺ� FLASH MEMORY �˻��
Uint16 TEST_EXT_FLASH_READ_buf[128];
Uint16 TEST_EXT_FLASH_READ_data = 0;
Uint16 TEST_EXT_FLASH_WRITE_Flag = 0;
Uint16 TEST_EXT_FLASH_READADD = 0;


//---DSP GDF �˻��
Uint16 TEST_DSP_GDF_buf = 0x00; 		// GDF ������
Uint16 TEST_DSP_GDF_flearFlag = 0; 		// GDF (HW,SW) ��ġ�Ȱ� Ŭ�����Ҷ�(1���ָ� Ŭ�����ϰ� ������ 0�̵ǰ� �ؾ���)
Uint16 TEST_DSP_GDF_MASKbuf = 0x00; 	// GDF MASK -- BIT0,1,2,3->GDF0,1,2,3 ����ũ(������(1), ����Ҷ��� (0))
                                        // EPLD��ġ�� �Ǿ�����, EPLD ��ġ�����Ϸ��� TEST_DSP_GDF_flearFlag(1)
Uint16 TEST_DSP_GDF_GENbuf = 0x00; 		// GDF ����Ʈ����� ���� GDF�� �߻���Ŵ(1), �߻��Ƚ�Ŵ(0)
//---DSP GDF �˻��
Uint16 TEST_CAN_AB = 0;	// (0): CANA TX --> CANB RX, (1) CANB TX --> CANA RX


//---DSP RS232b ��Ű˻�
Uint16 TEST_DSP_SCICTX_buf = 0;
Uint16 TEST_DSP_SCICRX_buf = 0;

extern unsigned int scic_rxd;	// mps_SciBC.c���� ����

unsigned int Sec_counter = 0;
unsigned int Sec_counter1 = 0;
unsigned char Sec_counter2 = 0;
extern unsigned int secondcounter;// main.c���� ����/ �ʱ�ȭ
void nvram_test();
void scic_test(void);
void can_test(void);

//========================================================//
// ����� ���� �׽�Ʈ
//========================================================//		
void BoardTest(unsigned char boardname)
{
	Uint16 Temp0_GPIO = 0;
	Uint16 Temp1_GPIO = 0;
	Uint16 Temp2_GPIO = 0;
	Uint16 Temp3_GPIO = 0;	
	
//	GpioCtrlRegs.GPAMUX1.bit.GPIO12 = 0;	//SWHEX0
//	GpioCtrlRegs.GPAMUX1.bit.GPIO13 = 0;	//SWHEX1
//	GpioCtrlRegs.GPAMUX1.bit.GPIO14 = 0;	//SWHEX2
//	GpioCtrlRegs.GPAMUX1.bit.GPIO15 = 0;	//SWHEX3	
	// WGMCU-Dip Switch �Է�
//	GpioCtrlRegs.GPAMUX2.bit.GPIO22 = 0;	//SWDIP0
//	GpioCtrlRegs.GPAMUX2.bit.GPIO23 = 0;	//SWDIP1
//	GpioCtrlRegs.GPAMUX2.bit.GPIO24 = 0;	//SWDIP2
//	GpioCtrlRegs.GPAMUX2.bit.GPIO25 = 0;	//SWDIP3
	
//	GpioCtrlRegs.GPAMUX2.bit.GPIO26 = 0;	//SWSEL
//	GpioCtrlRegs.GPAMUX2.bit.GPIO27 = 0;	//SWPB	
	
	if( 5000 <= msec_dif(msec_tmr, msec_1000m)){		// 1000mse = 200usec*5000
		msec_1000m = msec_tmr;			
		Sec_counter++;
		Sec_counter2++;

		MCU_LD2_TOGGLE();
		MCU_LD1_TOGGLE();
			
		TEST_EPLD_ELED ^=0xff;
		CS_WR_ELED() = TEST_EPLD_ELED;
	
		scic_test();
		can_test();

	}
	
		
	if( 2500 <= msec_dif(msec_tmr, msec_500m)){			// 500mse = 200usec*2500
		msec_500m = msec_tmr;

		CS_WR_LED1() = TEST_EPLD_LED1;
		CS_WR_LED2() = TEST_EPLD_LED2;
		CS_WR_LED3() = TEST_EPLD_LED3;
		
		TEST_EPLD_LED1 = (TEST_EPLD_LED1 << 1);
		TEST_EPLD_LED2 = (TEST_EPLD_LED2 << 1);
		TEST_EPLD_LED3 = (TEST_EPLD_LED3 << 1);
		if(TEST_EPLD_LED1 == 0x100) TEST_EPLD_LED1 = 0x1;
		if(TEST_EPLD_LED2 == 0x100) TEST_EPLD_LED2 = 0x1;
		if(TEST_EPLD_LED3 == 0x100) TEST_EPLD_LED3 = 0x1;


	//	CS_WR_DOUT1() = TEST_EPLD_DOUT1;
		CS_WR_DOUT1() = DioOutBuf1;
		TEST_EPLD_DOUT1 = (TEST_EPLD_DOUT1 << 1);
		if(TEST_EPLD_DOUT1 == 0x100) TEST_EPLD_DOUT1 = 0x1;

	}

	//== 10msec tesk ====
	if( 50 <= msec_dif(msec_tmr, msec_10m)){			// 10ms = 200usec*50; 
		msec_10m = msec_tmr;
		
		Temp0_GPIO = GpioDataRegs.GPADAT.bit.GPIO12;//SWHEX0--> HEX SWITCH BIT0
		Temp1_GPIO = GpioDataRegs.GPADAT.bit.GPIO13;//SWHEX1--> HEX SWITCH BIT1
		Temp2_GPIO = GpioDataRegs.GPADAT.bit.GPIO14;//SWHEX2--> HEX SWITCH BIT2
		Temp3_GPIO = GpioDataRegs.GPADAT.bit.GPIO15;//SWHEX3--> HEX SWITCH BIT3
		TEST_DSP_HEXSW_IN = !Temp3_GPIO*8 + !Temp2_GPIO*4 +!Temp1_GPIO*2 +!Temp0_GPIO*1;
	
		Temp0_GPIO = GpioDataRegs.GPADAT.bit.GPIO22;//SWDIP0
		Temp1_GPIO = GpioDataRegs.GPADAT.bit.GPIO23;//SWDIP1
		Temp2_GPIO = GpioDataRegs.GPADAT.bit.GPIO24;//SWDIP2
		Temp3_GPIO = GpioDataRegs.GPADAT.bit.GPIO25;//SWDIP3
		TEST_DSP_DIPSW_IN = !Temp3_GPIO*8 + !Temp2_GPIO*4 +!Temp1_GPIO*2 +!Temp0_GPIO*1;
		TEST_DSP_MODESW_IN = GpioDataRegs.GPADAT.bit.GPIO26;//SWSEL---> MODE SWITCH�� ����ұ�?(TEST MODE / NORMAL MODE)
		TEST_DSP_PUSHBUTTONSW_IN = !GpioDataRegs.GPADAT.bit.GPIO27;//SWPB - PUSH BUTTON SW	

		TEST_EPLD_DIN1 = (~CS_RD_DIN1() & 0xff);// active low
		TEST_EPLD_DIN2 = (~CS_RD_DIN2() & 0xff);

		TEST_DSP_GDF_buf = CS_RD_GDF()&0x0f;	  // GATE DERIVER FAULT(GDF) ����
		if(TEST_DSP_GDF_flearFlag == 1){	      // latch�Ǿ��� ��Ʈ Ŭ����
			CS_WR_FCLR() = 0x00;
			TEST_DSP_GDF_flearFlag = 0;
		}
		TEST_EPLD_HWF1 = CS_RD_HWF1()&0xff; // hwf 8ea ����
		TEST_EPLD_HWF2 = CS_RD_HWF2()&0x0f; // hwf 4ea ����XCCCCCCCCCCCDCDCDCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC

		CS_WR_PWMEN() = 0x03; // PWM ENABLE

		//===================================================================================
		// DSP���� EEPROM(24C16) 16BIT(������ �а� ���°� Ȯ��)
		// ���� ����Ÿ�� ��� ������ TEST_DSP_EEPROM_WriteFlag�� 1�� ����� ������ 0�� �Ǹ鼭
		// ���� ��巹�� ����Ÿ�� ���� ����Ÿ�� ������ ������
		// !!!���� : EEPROM ���� ���α׷����� ��� ���ְ� ���α׷��ϸ� EEPROM������<-������ 100����������
		//====================================================================================
		if(TEST_DSP_EEPROM_WriteFlag == 1){
			if(TEST_DSP_EEPROM_WAddr > 1000){
				TEST_DSP_EEPROM_WAddr = 1000;	
			}
			WriteEEpromWord(TEST_DSP_EEPROM_WAddr,TEST_DSP_EEPROM_WData);			
			TEST_DSP_EEPROM_WriteFlag = 0;
			ReadEEpromWord(TEST_DSP_EEPROM_WAddr, &TEST_DSP_EEPROM_RData);
		}	
	}
}
//================================//
// 40�� ������ 8usec-10usec
// 40�� ���� 4usec
//===============================//
void nvram_test()
{
	PWORD(TEST_NVRAM_WAddr1) = TEST_NVRAM_WData1 & 0xff;

	MCU_LD4_OFF();
		TEST_NVRAM_RData = PWORD(TEST_NVRAM_RAddr+0);
		TEST_NVRAM_RData = PWORD(TEST_NVRAM_RAddr+1);
		TEST_NVRAM_RData = PWORD(TEST_NVRAM_RAddr+2);
		TEST_NVRAM_RData = PWORD(TEST_NVRAM_RAddr+3);
		TEST_NVRAM_RData = PWORD(TEST_NVRAM_RAddr+4);
		TEST_NVRAM_RData = PWORD(TEST_NVRAM_RAddr+5);
		TEST_NVRAM_RData = PWORD(TEST_NVRAM_RAddr+6);
		TEST_NVRAM_RData = PWORD(TEST_NVRAM_RAddr+7);
		TEST_NVRAM_RData = PWORD(TEST_NVRAM_RAddr+8);
		TEST_NVRAM_RData = PWORD(TEST_NVRAM_RAddr+9);
		TEST_NVRAM_RData = PWORD(TEST_NVRAM_RAddr+0);
		TEST_NVRAM_RData = PWORD(TEST_NVRAM_RAddr+1);
		TEST_NVRAM_RData = PWORD(TEST_NVRAM_RAddr+2);
		TEST_NVRAM_RData = PWORD(TEST_NVRAM_RAddr+3);
		TEST_NVRAM_RData = PWORD(TEST_NVRAM_RAddr+4);
		TEST_NVRAM_RData = PWORD(TEST_NVRAM_RAddr+5);
		TEST_NVRAM_RData = PWORD(TEST_NVRAM_RAddr+6);
		TEST_NVRAM_RData = PWORD(TEST_NVRAM_RAddr+7);
		TEST_NVRAM_RData = PWORD(TEST_NVRAM_RAddr+8);
		TEST_NVRAM_RData = PWORD(TEST_NVRAM_RAddr+9);
		TEST_NVRAM_RData = PWORD(TEST_NVRAM_RAddr+0);
		TEST_NVRAM_RData = PWORD(TEST_NVRAM_RAddr+1);
		TEST_NVRAM_RData = PWORD(TEST_NVRAM_RAddr+2);
		TEST_NVRAM_RData = PWORD(TEST_NVRAM_RAddr+3);
		TEST_NVRAM_RData = PWORD(TEST_NVRAM_RAddr+4);
		TEST_NVRAM_RData = PWORD(TEST_NVRAM_RAddr+5);
		TEST_NVRAM_RData = PWORD(TEST_NVRAM_RAddr+6);
		TEST_NVRAM_RData = PWORD(TEST_NVRAM_RAddr+7);
		TEST_NVRAM_RData = PWORD(TEST_NVRAM_RAddr+8);
		TEST_NVRAM_RData = PWORD(TEST_NVRAM_RAddr+9);
		TEST_NVRAM_RData = PWORD(TEST_NVRAM_RAddr+0);
		TEST_NVRAM_RData = PWORD(TEST_NVRAM_RAddr+1);
		TEST_NVRAM_RData = PWORD(TEST_NVRAM_RAddr+2);
		TEST_NVRAM_RData = PWORD(TEST_NVRAM_RAddr+3);
		TEST_NVRAM_RData = PWORD(TEST_NVRAM_RAddr+4);
		TEST_NVRAM_RData = PWORD(TEST_NVRAM_RAddr+5);
		TEST_NVRAM_RData = PWORD(TEST_NVRAM_RAddr+6);
		TEST_NVRAM_RData = PWORD(TEST_NVRAM_RAddr+7);
		TEST_NVRAM_RData = PWORD(TEST_NVRAM_RAddr+8);
		TEST_NVRAM_RData = PWORD(TEST_NVRAM_RAddr+9);
	MCU_LD4_ON();
	MCU_LD3_OFF();	
		PWORD(TEST_NVRAM_WAddr+0) = TEST_NVRAM_WData;
		PWORD(TEST_NVRAM_WAddr+1) = TEST_NVRAM_WData;
		PWORD(TEST_NVRAM_WAddr+2) = TEST_NVRAM_WData;
		PWORD(TEST_NVRAM_WAddr+3) = TEST_NVRAM_WData;
		PWORD(TEST_NVRAM_WAddr+4) = TEST_NVRAM_WData;
		PWORD(TEST_NVRAM_WAddr+5) = TEST_NVRAM_WData;
		PWORD(TEST_NVRAM_WAddr+6) = TEST_NVRAM_WData;
		PWORD(TEST_NVRAM_WAddr+7) = TEST_NVRAM_WData;
		PWORD(TEST_NVRAM_WAddr+8) = TEST_NVRAM_WData;
		PWORD(TEST_NVRAM_WAddr+9) = TEST_NVRAM_WData;
		PWORD(TEST_NVRAM_WAddr+0) = TEST_NVRAM_WData;
		PWORD(TEST_NVRAM_WAddr+1) = TEST_NVRAM_WData;
		PWORD(TEST_NVRAM_WAddr+2) = TEST_NVRAM_WData;
		PWORD(TEST_NVRAM_WAddr+3) = TEST_NVRAM_WData;
		PWORD(TEST_NVRAM_WAddr+4) = TEST_NVRAM_WData;
		PWORD(TEST_NVRAM_WAddr+5) = TEST_NVRAM_WData;
		PWORD(TEST_NVRAM_WAddr+6) = TEST_NVRAM_WData;
		PWORD(TEST_NVRAM_WAddr+7) = TEST_NVRAM_WData;
		PWORD(TEST_NVRAM_WAddr+8) = TEST_NVRAM_WData;
		PWORD(TEST_NVRAM_WAddr+9) = TEST_NVRAM_WData;
		PWORD(TEST_NVRAM_WAddr+0) = TEST_NVRAM_WData;
		PWORD(TEST_NVRAM_WAddr+1) = TEST_NVRAM_WData;
		PWORD(TEST_NVRAM_WAddr+2) = TEST_NVRAM_WData;
		PWORD(TEST_NVRAM_WAddr+3) = TEST_NVRAM_WData;
		PWORD(TEST_NVRAM_WAddr+4) = TEST_NVRAM_WData;
		PWORD(TEST_NVRAM_WAddr+5) = TEST_NVRAM_WData;
		PWORD(TEST_NVRAM_WAddr+6) = TEST_NVRAM_WData;
		PWORD(TEST_NVRAM_WAddr+7) = TEST_NVRAM_WData;
		PWORD(TEST_NVRAM_WAddr+8) = TEST_NVRAM_WData;
		PWORD(TEST_NVRAM_WAddr+9) = TEST_NVRAM_WData;
		PWORD(TEST_NVRAM_WAddr+0) = TEST_NVRAM_WData;
		PWORD(TEST_NVRAM_WAddr+1) = TEST_NVRAM_WData;
		PWORD(TEST_NVRAM_WAddr+2) = TEST_NVRAM_WData;
		PWORD(TEST_NVRAM_WAddr+3) = TEST_NVRAM_WData;
		PWORD(TEST_NVRAM_WAddr+4) = TEST_NVRAM_WData;
		PWORD(TEST_NVRAM_WAddr+5) = TEST_NVRAM_WData;
		PWORD(TEST_NVRAM_WAddr+6) = TEST_NVRAM_WData;
		PWORD(TEST_NVRAM_WAddr+7) = TEST_NVRAM_WData;
		PWORD(TEST_NVRAM_WAddr+8) = TEST_NVRAM_WData;
		PWORD(TEST_NVRAM_WAddr+9) = TEST_NVRAM_WData;
	MCU_LD3_ON();	
	/*
	MCU_LD4_OFF();
		TEST_NVRAM_RData = PWORD(TEST_NVRAM_RAddr+0) & 0xff;
		TEST_NVRAM_RData = PWORD(TEST_NVRAM_RAddr+1) & 0xff;
		TEST_NVRAM_RData = PWORD(TEST_NVRAM_RAddr+2) & 0xff;
		TEST_NVRAM_RData = PWORD(TEST_NVRAM_RAddr+3) & 0xff;
		TEST_NVRAM_RData = PWORD(TEST_NVRAM_RAddr+4) & 0xff;
		TEST_NVRAM_RData = PWORD(TEST_NVRAM_RAddr+5) & 0xff;
		TEST_NVRAM_RData = PWORD(TEST_NVRAM_RAddr+6) & 0xff;
		TEST_NVRAM_RData = PWORD(TEST_NVRAM_RAddr+7) & 0xff;
		TEST_NVRAM_RData = PWORD(TEST_NVRAM_RAddr+8) & 0xff;
		TEST_NVRAM_RData = PWORD(TEST_NVRAM_RAddr+9) & 0xff;
	MCU_LD4_ON();
	MCU_LD3_OFF();	
		PWORD(TEST_NVRAM_WAddr+0) = TEST_NVRAM_WData & 0xff;
		PWORD(TEST_NVRAM_WAddr+1) = TEST_NVRAM_WData & 0xff;
		PWORD(TEST_NVRAM_WAddr+2) = TEST_NVRAM_WData & 0xff;
		PWORD(TEST_NVRAM_WAddr+3) = TEST_NVRAM_WData & 0xff;
		PWORD(TEST_NVRAM_WAddr+4) = TEST_NVRAM_WData & 0xff;
		PWORD(TEST_NVRAM_WAddr+5) = TEST_NVRAM_WData & 0xff;
		PWORD(TEST_NVRAM_WAddr+6) = TEST_NVRAM_WData & 0xff;
		PWORD(TEST_NVRAM_WAddr+7) = TEST_NVRAM_WData & 0xff;
		PWORD(TEST_NVRAM_WAddr+8) = TEST_NVRAM_WData & 0xff;
		PWORD(TEST_NVRAM_WAddr+9) = TEST_NVRAM_WData & 0xff;
	MCU_LD3_ON();		
	*/
	/*
	for(i = 0; i < 100; i++){
		
		PWORD(TEST_NVRAM_WAddr+i) = TEST_NVRAM_WData & 0xff;
	}
	MCU_LD4_ON();
	MCU_LD3_OFF();
	for(i = 0; i < 100; i++){
		//PWORD(TEST_NVRAM_WAddr) = TEST_NVRAM_WData & 0xff;
		TEST_NVRAM_RData = PWORD(TEST_NVRAM_RAddr+i) & 0xff;
	}
	MCU_LD3_ON();
	*/

/*
	if(CanA_RTC_UPGrade_flag==1){
		CanA_RTC_UPGrade_flag = 0;
		
		TEST_NVRAM_RTC_YEAR_w = 15;
		TEST_NVRAM_RTC_MONTH_w = 1;
		TEST_NVRAM_RTC_DATE_w = 1;
		TEST_NVRAM_RTC_DAY_w = 6;
		TEST_NVRAM_RTC_HOUR_w = 14;
		TEST_NVRAM_RTC_MIN_w = 56;
		TEST_NVRAM_RTC_SECOND_w = 0;
//		TEST_NVRAM_RTC_CENTURY_w = 0x20;	
		
		PWORD(ADD_NVR_RTC_CENTURY) = 0x80;
		
		PWORD(ADD_NVR_RTC_YEAR) = TEST_NVRAM_RTC_YEAR_w & 0xff;
		PWORD(ADD_NVR_RTC_MONTH) = TEST_NVRAM_RTC_MONTH_w & 0xff;
		PWORD(ADD_NVR_RTC_DATE) = TEST_NVRAM_RTC_DATE_w & 0xff;
		PWORD(ADD_NVR_RTC_DAY) = TEST_NVRAM_RTC_DAY_w & 0xff;
		PWORD(ADD_NVR_RTC_HOUR) = TEST_NVRAM_RTC_HOUR_w & 0xff;
		PWORD(ADD_NVR_RTC_MIN) = TEST_NVRAM_RTC_MIN_w & 0xff;
		PWORD(ADD_NVR_RTC_SEC) = TEST_NVRAM_RTC_SECOND_w & 0xff;
		PWORD(ADD_NVR_RTC_CENTURY) = 0x00;
	
	}		
		PWORD(ADD_NVR_RTC_CENTURY) = 0x40;
	
	TEST_NVRAM_RTC_YEAR_r = PWORD(ADD_NVR_RTC_YEAR) & 0xff;
	TEST_NVRAM_RTC_MONTH_r = PWORD(ADD_NVR_RTC_MONTH) & 0xff;
	TEST_NVRAM_RTC_DATE_r = PWORD(ADD_NVR_RTC_DATE) & 0xff;
	TEST_NVRAM_RTC_DAY_r = PWORD(ADD_NVR_RTC_DAY) & 0xff;
	TEST_NVRAM_RTC_HOUR_r = PWORD(ADD_NVR_RTC_HOUR) & 0xff;
	TEST_NVRAM_RTC_MIN_r = PWORD(ADD_NVR_RTC_MIN) & 0xff;
	TEST_NVRAM_RTC_SECOND_r = PWORD(ADD_NVR_RTC_SEC) & 0xff;
	//TEST_NVRAM_RTC_CENTURY_r = PWORD(ADD_NVR_RTC_YEAR);
		PWORD(ADD_NVR_RTC_CENTURY) = 0x00;

	TEST_NVRAM_RTC_MIN_rh = TEST_NVRAM_RTC_MIN_r & 0xf0;
	TEST_NVRAM_RTC_MIN_rl = TEST_NVRAM_RTC_MIN_r & 0x0f;
	TEST_NVRAM_RTC_SECOND_rh = TEST_NVRAM_RTC_SECOND_r & 0xf0;
	TEST_NVRAM_RTC_SECOND_rl = TEST_NVRAM_RTC_SECOND_r & 0x0f;
*/	
}

//=====================================================================//
// ��  �� : CAN1,2 �׽�Ʈ                    										
// CANA���� ������ CANB���� �޴°�Ȯ��
//=====================================================================//
void can_test(void)
{
	
	if(TEST_CAN_AB == 0)
	{
		SendDataToECanA(0x3L, 0x08, (long)secondcounter, (long)secondcounter);
	}
	else if(TEST_CAN_AB == 1)
	{
		SendDataToECanB(0x2L, 0x08, (long)secondcounter, (long)secondcounter);
	}
	else
	{
		SendDataToECanA(0x3L, 0x08, (long)secondcounter, (long)secondcounter);
	}
	
	
}

void scic_test(void)
{
	transmit_RS232(Sec_counter2);	
}


void siv_test_init()
{
	Theta_radU1 = 0;
	Theta_radV1 = -PI_2/3;
	Theta_radW1 = -PI_2/3*2;

	Theta_inc = 0;

	sineU1 = 0;
	sineV1 = 0;
	sineW1 = 0;
	
	sineU2 = 0;
	sineV2 = 0;
	sineW2 = 0;

	siv_test_Vrefin1 = 0;
	siv_test_Vrefin2 = 0;	
	siv_test_Vrefin1_Scale = 1;
	siv_test_Vrefin2_Scale = 1;

	
}


void siv_test()
{
	if(MODEAck == TESTMODE0)
	{
		Theta_inc = 0.025;//60HZ
	}
	else if(MODEAck == NORMALMODE1)
	{
		Theta_inc = 0.021;//50HZ
	}
	else
	{
		Theta_inc = 0.025;;
	}
	

	Theta_radU1 = Theta_radU1 + Theta_inc;	
	Theta_radV1 = Theta_radV1 + Theta_inc;	
	Theta_radW1 = Theta_radW1 + Theta_inc;	

	if(Theta_radU1 >= PI)	Theta_radU1 -= PI_2;
	if(Theta_radU1 < -PI)	Theta_radU1 += PI_2;


	if(Theta_radV1 >= PI)	Theta_radV1 -= PI_2;
	if(Theta_radV1 < -PI)	Theta_radV1 += PI_2;


	if(Theta_radW1 >= PI)	Theta_radW1 -= PI_2;
	if(Theta_radW1 < -PI)	Theta_radW1 += PI_2;
		
	sineU1 = sin(Theta_radU1)*siv_test_Vrefin1;
	sineV1 = sin(Theta_radV1)*siv_test_Vrefin1;
	sineW1 = sin(Theta_radW1)*siv_test_Vrefin1;
	
	sineU2 = sin(Theta_radU1)*siv_test_Vrefin2;
	sineV2 = sin(Theta_radV1)*siv_test_Vrefin2;
	sineW2 = sin(Theta_radW1)*siv_test_Vrefin2;
}
