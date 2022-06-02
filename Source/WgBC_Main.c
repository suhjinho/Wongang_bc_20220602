//==================================================================================//
// ¿ø°­¼± µ¿·ÂÂ÷(30kw)/°´Â÷(50kw) ÃàÀüÁö ÃæÀü±â ¸ðµâ(10kw) ÇÁ·Î±×·¥	           		//
//==================================================================================//
// INPUT : 670Vdc(620~720Vdc)														//
// Ãâ·Â¿ë·® : °´Â÷(50kW, 10kw*5module), 72Vdc ~ 83Vdc								//
// È¸·Î¹æ½Ä : ¸ðµâ(Àý¿¬Çü zvzcs dcdc converter										//
// Á¦¾î±â ; wongan Battery MCU(TMS320F28335)                                        //
// ÇÁ·Î±×·¥¸í : WGBC Control                                        //
//======================================================================================//
// ProgramVersion                                                    by PACTECH R&D LSH //
//------------------------------------------------------------------------------------------------------//
// REV      | DATE           | ³»¿ë                                                                     //
//------------------------------------------------------------------------------------------------------//
// ver0.01  | 2015³â04¿ù28ÀÏ | 4/24~25 °´Â÷/µ¿·ÂÂ÷ ÀÚÃ¼ ¿Âµµ»ó½Â½ÃÇèÇÑ ÇÁ·Î±×·¥							//
//------------------------------------------------------------------------------------------------------//
// ver0.01  | 2015³â04¿ù28ÀÏ | ¹Ú½ºÆúÆ® Ã³¸®ºÎ ¼öÁ¤(°æ°íÀåÀÎ°æ¿ì ¸ðµâÆúÆ®Ã³¸® ¾ÈÇÏ°í Àç±âµ¿)			//
//------------------------------------------------------------------------------------------------------//
// ver2.00  | 2015³â0?¿ù??ÀÏ | Á¶ÇÕ½ÃÇèÀü ´ÏÆ¬Æú¸®¸Ó ¹èÅÍ¸® Æ¯¼ºÀÌ ±×µ¿¾È ½ÃÇèÇÏ´ø ¹èÅÍ¸®¿Í Æ¯¼ºÆ²·Á ´Ù½Ã Æ©´×ÇÔ			//
//------------------------------------------------------------------------------------------------------//
// ver1.00  | 2015³â11¿ù12ÀÏ | ½ÃÇè½Ã SW2·Î ÆúÆ® Ã³¸®ÇÏ´ÂºÎºÐ, Â÷·®¿¡¼­ ¸®¼ÂÁö·É ¼öÁ¤     	   		    //
//                           | 1È£±â ¿Ï¼ºÂ÷½ÃÇè(11¿ù16~18ÀÏ)Àü È¸»ç¿¡¼­ ÇÁ·Î±×·¥ ¼öÁ¤ÇÔ                 //
//======================================================================================================//

#define	GLOBAL__VARIABLE_FUNCTION_H
#include "Global_Variable_Function.h"

#define ProgramMode_BOX0	0
#define ProgramMode_MODULE1	1
#define ProgramMode_CPU2	2
unsigned int ProgramMode_B0M1C2 = 0;	//20150404Ãß°¡


//0---Á¤»ó ¸ðµå
//1---mcu Å×½ºÆ® ¸ðµå
//2--¸ðµâ Å×½ºÆ® ¸ðµå

extern Uint16 msec_tmr;	


// These are defined by the linker (see FLASH.cmd)
// ÀÌ º¯¼ö´Â ¸ÞÀÎ ÇÔ¼öÀÇ ÃÊ±âÈ­ ·çÆ¾¿¡¼­ "ramfuncs" ÀÌ¶ó°í Á¤ÀÇµÈ ÄÚµå ¼½¼ÇÀ»
// ³»ºÎ Flash¿¡¼­ RAM¿µ¿ªÀ¸·Î º¹»çÇÏ±â À§ÇÑ MemCopy ÇÔ¼ö¿¡¼­ »ç¿ëµÊ.
extern Uint16 RamfuncsLoadStart;
extern Uint16 RamfuncsLoadEnd;
extern Uint16 RamfuncsRunStart;

Uint16 msec_1000m = 0,msec_500m = 0,msec_200m = 0,msec_100m = 0, msec_10m = 0;
Uint16 msec_1m = 0;
Uint16 msec_2m = 0;
Uint16 msec_400u = 0;

unsigned int fault_swclear = 0;
unsigned int secondcounter = 0;

//void BoardTest(unsigned char boardname);
Uint16 FanOnCmd = 0;
Uint16 DAC_Mode = 0;

Uint16 GPIO_INT0 = 0;
Uint16 GPIO_INT1 = 0;
Uint16 GPIO_INT2 = 0;
Uint16 GPIO_INT3 = 0;
Uint16 GPIO_INT4 = 0;
Uint16 GPIO_INT5 = 0;
Uint16 GPIO_INT6 = 0;
Uint16 GPIO_L_INT0 = 0;
Uint16 GPIO_L_INT1 = 0;
Uint16 GPIO_L_INT2 = 0;
Uint16 GPIO_L_INT3 = 0;
Uint16 GPIO_L_INT4 = 0;
Uint16 GPIO_L_INT5 = 0;
Uint16 GPIO_L_INT6 = 0;
void intpin_Chk();

Uint16 FaultClear_flag = 0;
//==================================================================================//
// Main()		    																//
//==================================================================================//
void main(void)
{

	//ProgramVersion = 101;// REV01.01 ---> 1,// REV01.23 ---> 123, // REV12.34 ---> 1234
	ProgramVersion = 2;// rev0.2 ---> 2015³â4¿ù28ÀÏ
	ProgramVersion = 200;// rev0.2 ---> 2015³â4¿ù28ÀÏ
	ProgramVersion = 100;// rev1.00 ---> 2015³â11¿ù12ÀÏ-->¿Ï¼ºÂ÷½ÃÇèÀü 1È£±â ¹öÀü-->¸ð´ÏÅÍÇÁ·Î±×·¥ ¾Æ³ª·Î±× SPARE1¿¡ Ç¥Çö
	
	// ÁÖº¯ÀåÄ¡ ÃÊ±âÈ­
	PeripheralInit();			// DSP ÃÊ±âÈ­
	DioOutBuf1 = 0x02;
	CS_WR_DOUT1() = DioOutBuf1;// ALL DOUT OFF

	System_Define();			// Àåºñ¼Â¾÷,½Ã½ºÅÛ Á¤°Ý
	Init_I2C_eeprom();			// eeprom ÃÊ±âÇÏ
	
	PWM_OFF();			// PWM Ãâ·Â µð½º¿¡ÀÌºí
	TimerInterruptEnable(); 	// Å¸ÀÌ¸Ó ÀÎÅÍ·´Æ® ÀÎ¿¡ÀÌºí
	
	VariableInit();				// ¸ðµç º¯¼ö 0À¸·Î ÃÊ±âÈ­
	FaultClear();				// ¼¾¼­º¸µå ÇÏµå¿þ¾î ÆúÆ®, DSP IDUÆúÆ® Å¬¸®¾î

	DOutputInit();				// IOº¸µå Ãâ·Â, LED SENSORº¸µå LED ¸ðµÎ ²ô±â	

	delay_ms(500);				// ½½·¹ÀÌºê º¸µå ÇÏµå¿þ¾î ¾ÈÁ¤È­ ½Ã°£
	FaultClear();				// ¼¾¼­º¸µå ÇÏµå¿þ¾î ÆúÆ®, DSP IDUÆúÆ® Å¬¸®¾î
	
	Check_DI();
	msec_1000m = msec_500m=msec_200m = msec_10m = msec_1m = msec_tmr;	// ÀÏÁ¤ ÁÖ±â task¸¦ ¸¸µé±â À§ÇØ¼­ º¯¼ö ÃÊ±âÈ­		

	EPwmInterruptEnable();		// PWM ÀÎÅÍ·´Æ®
	Nvram_Init();
	
	FaultClear();				// ¼¾¼­º¸µå ÇÏµå¿þ¾î ÆúÆ®, DSP IDUÆúÆ® Å¬¸®¾î
	ReloadCpuTimer1();  
	MidiumFault_3min3N_INIT();// 3ºÐ3È¸ Áß°íÀå Ã³¸®º¯¼ö ÃÊ±âÈ­
	
	//===20150404Ãß°¡
//#define ProgramMode_BOX0	0
//#define ProgramMode_MODULE1	1
//#define ProgramMode_CPU2	2
//var Uint16 ProgramMode_B0M1C2;	//20150404Ãß°¡
	if(ProgramMode_B0M1C2 == ProgramMode_CPU2)
	{
		while(1)
		{
			BoardTest(1);
		}
	}
	siv_test_init();
	//=======main loop===========//	
	while(TRUE)
	{
		//== 1.0 sec ====
		if( 5000 <= msec_dif(msec_tmr, msec_1000m))
		{		// 1000mse = 200usec*5000
			msec_1000m = msec_tmr;		
			secondcounter++;	
		}
		//== 0.5sec ====
		if( 2500 <= msec_dif(msec_tmr, msec_500m))
		{			// 500mse = 200usec*2500
			msec_500m = msec_tmr;
			Nvram_TDpage_clear();
		}
		//== 0.1sec ====
		if( 500 <= msec_dif(msec_tmr, msec_100m))
		{			// 100ms = 200usec*500; 
			msec_100m = msec_tmr;
//			MCU_D33_TOGGLE();	//D33				//20220325 ÀüÀÚÇÇ½ÃÇè ´ëºñ ºñÈ°¼ºÈ­

		}
		//== 10msec ====
		if( 50 <= msec_dif(msec_tmr, msec_10m))
		{			// 10ms = 200usec*50; 
			msec_10m = msec_tmr;
			Sequence();
			if(fault_swclear == 1)
			{
				fault_swclear = 0;
				FaultClear();
			}
		}
		//== 400usec ====	
		if( 2 <= msec_dif(msec_tmr, msec_400u))
		{				// 1ms = 200usec*2; 
			msec_400u = msec_tmr;
		}	
		//== 1msec ====	
		if( 5 <= msec_dif(msec_tmr, msec_1m))
		{				// 1ms = 200usec*5; 
			msec_1m = msec_tmr;
			RTC_Read();
			RTC_Write();
			NvramSDTDWrite();
			DAC_Out_test(DAC_Mode);
		}
		//== 2msec ====	
		if( 10 <= msec_dif(msec_tmr, msec_2m))
		{				// 2ms = 200usec*10; 
			msec_2m = msec_tmr;
			Can_Routine();
		}

		if(FaultClear_flag == 1)
		{
			FaultClear();
			FaultClear_flag = 0;
		}
		
 		
	}   
}

//================================================================//
//==½Ã½ºÅÛ ¼Â¾÷ ¹× Á¤°Ý°ü·Ã Á¤ÀÇ
//================================================================//
void System_Define()
{
	
	Setting_Chk();	// ÃæÀü±â Á¾·ù(°´Â÷,µ¿·ÂÂ÷) ¹× ¸ðµâ ID ¼³Á¤
		
	//¸ðµâÀÔ·ÂÀü¾Ð --> ADCRD10
	MInputVolRATED = 670.0;		//½Ã½ºÅÛÁ¤°Ý(670[V])
	MInputVolMAX = 720.0;		//½Ã½ºÅÛÁ¤°Ý(720[V])
	MInputVolMIN = 620.0;		//½Ã½ºÅÛÁ¤°Ý(620[V])
	MInputVolSWOVF = 900.0;		//½ÃÇèÀýÂ÷¼­ ±âÁØ
	MInputVolHWOVF = 1005.0;	//
//	MInputVolUVF = 620.0;
	MInputVolUVF = 600.0;

	//¸ðµâ DCLINKÀü¾Ð(RATED 670Vdc) --> ADCRD11
	MDclinkVolRATED = 670.0;
	MDclinkVolMAX = 720.0;	
	MDclinkVolMIN = 620.0;
	MDclinkVolSWOVF = 900.0;
	MDclinkVolHWOVF = 1005.0;	//
	//MDclinkVolUVF = 620.0;
//	MDclinkVolUVF = 610.0; // 620VÀÎµ¥  ½ÃÇèÁß ÆúÆ®°É·Á 610V·Î ¼öÁ¤
	MDclinkVolUVF = 600.0; // 620VÀÎµ¥  ½ÃÇèÁß ÆúÆ®°É·Á 610V·Î ¼öÁ¤
//¸ðµâÃâ·ÂÀü¾Ð(OBDÈÄ´Ü) Á¤°Ý 720Vdc(72~83) --> ADCRD9
	//MBatVolRATED = 73.0;
	//MBatVolRATED = 83.0;
	MBatVolRATED = 84.0;
	//MBatVolRATED = 80.0;//2015³â8¿ù11ÀÏ ÀÓ½Ã·Î 84v¿¡¼­ 80v·Î, ´õ¼ÇÅ×Å© ¹èÅÍ¸®½ÃÇèÁß
	MBatVolMAX = 83.0;
	MBatVolMIN = 72.0;
	//MBatVolSWOVF = 90.0;	//»ó¼¼¼³°è¼­ ½ÃÇè±âÁØ 90V
	//MBatVolSWOVF = 105.0;	//20150307ÀÓ½Ã·ÎÆ©´×Áß
	MBatVolSWOVF = 90.0;	//20150307ÀÓ½Ã·ÎÆ©´×Áß
	MBatVolHWOVF = 103.25;	//
//	MBatVolUVF = 40.0;		//»ó¼¼¼³°è¼­ ½ÃÇè±âÁØ 50V
	MBatVolUVF = 50.0;		//»ó¼¼¼³°è¼­ ½ÃÇè±âÁØ 50V

//¸ðµâÃâ·ÂÀü¾Ð1(OBDÀü´Ü) Á¤°Ý 720Vdc(72~83) --> ADCRD8
	//MOutVolRATED = 74.0;
	MOutVolRATED = 84.0;
	MOutVolMAX = 83.0;
	MOutVolMIN = 72.0;
	//MOutVolSWOVF = 90.0;	//»ó¼¼¼³°è¼­ ½ÃÇè±âÁØ 90V
	//MOutVolSWOVF = 105.0;	//20150408ÀÓ½Ã·ÎÆ©´×Áß
	MOutVolSWOVF = 90.0;	//20150307ÀÓ½Ã·ÎÆ©´×Áß
	MOutVolHWOVF = 103.25;	//
//	MOutVolUVF = 40.0;		//½ÃÇèÀýÂ÷¼­ ±âÁØ
	MOutVolUVF = 50.0;		//½ÃÇèÀýÂ÷¼­ ±âÁØ

//¸ðµâ Á¦¾îÀü¾Ð Á¤°Ý 72Vdc(72~83) --> ADCRD7
	MCtrlVolRATED = 72.0;
	MCtrlVolMAX = 83.0;
	MCtrlVolMIN = 72.0;
//	MCtrlVolSWOVF = 95.0;	//½ÃÇèÀýÂ÷¼­ ±âÁØ//92v
	//MCtrlVolSWOVF = 90.0;	//½ÃÇèÀýÂ÷¼­ ±âÁØ//92v
	MCtrlVolSWOVF = 92.0;	//½ÃÇèÀýÂ÷¼­ ±âÁØ//92v--20150522
	MCtrlVolHWOVF = 93.8;	//
	MCtrlVolUVF = 48.0;		//½ÃÇèÀýÂ÷¼­ ±âÁØ	
//¸ðµâ ÀÔ·ÂÀü·ù --> ADCRD0
	MInputCurRATED = 20.0;		//È®ÀÎÇÊ¿ä????
	MInputCurMAX = 20.0;		//È®ÀÎÇÊ¿ä????
	MInputCurSWOCF = 30.00;		// 20150326,25A-->30A(½ÃÇèÀýÂ÷¼­),--->20150407 º¸µå¼öÁ¤ÇÏ¸é¼­ Á¤»óÀ¸·Î º¹±Í(30A)
	MInputCurHWOCF = 30.00;		// 
//box Ãâ·ÂÀü·ù --> ADCRD6
	if(WGBC_TC0PC1 == WGBC_TC0) 
	{	// °´Â÷¿ë ÃàÀüÁö ÃæÀü±â
		BOutCurRATED = 695.0;		//°´Â÷ »ó¼¼¼³°è¼­ »ç¾ç, ÃæÀüÀüÀü·ù´Â 60A, °úÃæÀüÀü·ù 66A(½ÃÇè±âÁØ»ç¾ç)
		BOutCurMAX = 835.0;			//°´Â÷ °úºÎÇÏ 120%
		BOutCurSWOCF = 900.0;		//°´Â÷ »ó¼¼¼³°è¼­/½ÃÇè±âÁØ »ç¾ç
		BOutCurHWOCF = 928.0;		//
		//¸ðµâ ÃæÀüÀü·ù --> ADCRD1
		MBatCurRATED = 12.0;	//°´Â÷ »ó¼¼¼³°è¼­, 60[A]/5¸ðµâ = 12[A]/M
		MBatCurMAX = 14.4;		//Á¤°Ý*120%·Î, 60[A]/5¸ðµâ*120% = 12[A]/M*1.2
		MBatCurSWOCF = 95.0;	//»ó¼¼¼³°è¼­ ½ÃÇè±âÁØ, ¹Ú½ºÃæÀü °úÀü·ùº¸´Ù ÀÛ¾Æ¼­ ÀÌ»óÇÔ
		MBatCurHWOCF = 120.0;	
	}
	else
	{
		BOutCurRATED = 417.0;		//µ¿·ÂÂ÷ »ó¼¼¼³°è¼­ »ç¾ç, ÃæÀüÀüÀü·ù´Â 68A, ¹èÅÍ¸®1°³´ç °úÃæÀü 75A(½ÃÇè±âÁØ»çç)
		BOutCurMAX = 500.0;			//µ¿·ÂÂ÷ °úºÎÇÏ 120%
		BOutCurSWOCF = 550.0;		//µ¿·ÂÂ÷ »ó¼¼¼³°è¼­/½ÃÇè±âÁØ »ç¾ç
		BOutCurHWOCF = 593.0;		//
		//¸ðµâ ÃæÀüÀü·ù --> ADCRD1
		MBatCurRATED = 22.6;	//µ¿·ÂÂ÷ »ó¼¼¼³°è¼­, 68[A]/3¸ðµâ = 22.6[A]/M
		MBatCurMAX = 27.2;		//Á¤°Ý*120%·Î, 68[A]/3¸ðµâ*120% = 22.6[A]/M*1.2
		MBatCurSWOCF = 95.0;	//»ó¼¼¼³°è¼­ ½ÃÇè±âÁØ, ¹Ú½ºÃæÀü °úÀü·ùº¸´Ù ÀÛ¾Æ¼­ ÀÌ»óÇÔ
		MBatCurHWOCF = 120.0;	
	}
	
//¸ðµâ Ãâ·ÂÀü·ù --> ADCRD2
	MOutCurRATED = 138.9;	// 10kW/72V = 138.9[A]
	MOutCurMAX = 166.7;		// 10kW/72V = 138.9[A] * 1.2
	MOutCurSWOCF = 180.0;	// »ó¼¼¼³°è¼­ ½ÃÇè±âÁØ 180A
	MOutCurHWOCF = 190.0;	// 

//box BATÀü·ù1 --> ADCRD4
//box BATÀü·ù2 --> ADCRD5 (°´Â÷ BC´Â »ç¿ë¾ÈÇÔ)
	if(WGBC_TC0PC1 == WGBC_TC0) 
	{	// °´Â÷¿ë ÃàÀüÁö ÃæÀü±â
		//box BATÀü·ù1 --> ADCRD4
		BBatCur1RATED = 60.0;	// °´Â÷ »ó¼¼¼³°è¼­ »ç¾ç 60A
		BBatCur1MAX = 72.0;		// °´Â÷ °úºÎÇÏ120%	
		BBatCur1SWOCF = 75.0;	// °´Â÷,µ¿·ÂÂ÷ »ó¼¼¼³°è¼­ ½ÃÇè±âÁØ 75A
//		BBatCur1SWOCF = 95.0;	// 20151116_Ã¢¿ø °´Â÷ Å×½ºÆ® (BOXÃæÀü°úÀü·ù ¹ß»ý)
		//BBatCur1SWOCF = 100.0;	// 20150326--ºÎÇÏ±Þº¯½ÃÇèÁß ÆúÆ®³ª¼­ ¿ì¼± Á¦ÇÑ ¿Ã¸²
		BBatCur1HWOCF = 102.3;	// 
		//box BATÀü·ù2 --> ADCRD5 (°´Â÷ BC´Â »ç¿ë¾ÈÇÔ)
		BBatCur2RATED = 0.0;	//
		BBatCur2MAX = 0.0;		//
		BBatCur2SWOCF = 75.0;	// °´Â÷´Â »ç¿ë¾ÈÇÔ 75A
		BBatCur2HWOCF = 102.3;	// °´Â÷´Â »ç¿ë¾ÈÇÔ 
		//box BATÀü·ù --> °´Â÷ BC = BAT1 + BAT2(0)
		BBatCurRATED = BBatCur1RATED + BBatCur2RATED;	// 
		BBatCurMAX = BBatCur1MAX + BBatCur2MAX;			//
		BBatCurSWOCF = BBatCur1SWOCF + BBatCur2SWOCF;	//
	}
	else
	{						// µ¿·ÂÂ÷¿ë ÃàÀüÁö ÃæÀü±â
		//box BATÀü·ù1 --> ADCRD4
		BBatCur1RATED = 34.0;	// µ¿·ÂÂ÷ »ó¼¼¼³°è¼­ »ç¾ç 68A
		BBatCur1MAX = 81.6;		// µ¿·ÂÂ÷ °úºÎÇÏ120%
		BBatCur1SWOCF = 75.0;	// °´Â÷,µ¿·ÂÂ÷ »ó¼¼¼³°è¼­ ½ÃÇè±âÁØ 75A
		BBatCur1HWOCF = 102.3;	// 
		//box BATÀü·ù2 --> ADCRD5 (°´Â÷ BC´Â »ç¿ë¾ÈÇÔ) --- ¹èÅÍ¸® 2°³¶óµµ ÃæÀüÀü·ù/ÆúÆ®º£·¼µîÀº ÃÖ´ëÄ¡·Î
		BBatCur2RATED = 34.0;	// µ¿·ÂÂ÷ »ó¼¼¼³°è¼­ »ç¾ç 68A
		BBatCur2MAX = 81.6;		// µ¿·ÂÂ÷ °úºÎÇÏ120%
		BBatCur2SWOCF = 75.0;	// µ¿·ÂÂ÷ »ó¼¼¼³°è¼­ ½ÃÇè±âÁØ 75A
		BBatCur2HWOCF = 102.3;	// 
		//box BATÀü·ù --> µ¿·ÂÂ÷BC = BAT1 + BAT2
		//BBatCurRATED = 45.0;//BBatCur1RATED + BBatCur2RATED;	// 
		BBatCurRATED = BBatCur1RATED + BBatCur2RATED;	// 
		BBatCurMAX = BBatCur1MAX + BBatCur2MAX;			//
		BBatCurSWOCF = BBatCur1SWOCF + BBatCur2SWOCF;	//
	}
}
//==================================================================================//
// ÇÔ¼ö¸í : Setting_Chk()		            										//
// DATE(¼öÁ¤)  : 2015. 01. 16														//
//==================================================================================//
void Setting_Chk()
{
	Uint16 Temp0_GPIO = 0;
	Uint16 Temp1_GPIO = 0;
	Uint16 Temp2_GPIO = 0;
	Uint16 Temp3_GPIO = 0;		
	

	Temp0_GPIO = GpioDataRegs.GPADAT.bit.GPIO12;//SWHEX0--> HEX SWITCH BIT0
	Temp1_GPIO = GpioDataRegs.GPADAT.bit.GPIO13;//SWHEX1--> HEX SWITCH BIT1
	Temp2_GPIO = GpioDataRegs.GPADAT.bit.GPIO14;//SWHEX2--> HEX SWITCH BIT2
	Temp3_GPIO = GpioDataRegs.GPADAT.bit.GPIO15;//SWHEX3--> HEX SWITCH BIT3
	MCUBD_HEXSW_IN = !Temp3_GPIO*8 + !Temp2_GPIO*4 +!Temp1_GPIO*2 +!Temp0_GPIO*1;
	
	Temp0_GPIO = GpioDataRegs.GPADAT.bit.GPIO22;//SWDIP0
	Temp1_GPIO = GpioDataRegs.GPADAT.bit.GPIO23;//SWDIP1
	Temp2_GPIO = GpioDataRegs.GPADAT.bit.GPIO24;//SWDIP2
	Temp3_GPIO = GpioDataRegs.GPADAT.bit.GPIO25;//SWDIP3

	MCUBD_DIPSW_IN = (!Temp3_GPIO*8 + !Temp2_GPIO*4 +!Temp1_GPIO*2 +!Temp0_GPIO*1)&0xff;
	MCUBD_MODESW_IN = GpioDataRegs.GPADAT.bit.GPIO26;//SWSEL---> MODE SWITCH·Î »ç¿ëÇÒ±î?(TEST MODE / NORMAL MODE)
	MCUBD_PUSHBUTTONSW_IN = !GpioDataRegs.GPADAT.bit.GPIO27;//SWPB - PUSH BUTTON SW	

//---¸ðµâ ¸ðµå (Å×½ºÆ®¸ðµå/NORMAL ¸ðµå)
	MODEAck = GpioDataRegs.GPADAT.bit.GPIO26;//SWSEL---> MODE SWITCH·Î »ç(TEST MODE(0) / NORMAL MODE(1))

	if(MCUBD_HEXSW_IN == 0)
	{//°´Â÷ BC ¸ðµâ1
		WGBC_TC0PC1 = WGBC_TC0;
		WGBC_M_ID = WGBC_M_ID1;
		MCU_LD4_ON();	// TCBC(ON), PCBC(OFF)
	}
	else if(MCUBD_HEXSW_IN == 1)
	{//°´Â÷ BC ¸ðµâ2
		WGBC_TC0PC1 = WGBC_TC0;
		WGBC_M_ID = WGBC_M_ID2;
		MCU_LD4_ON();	// TCBC(ON), PCBC(OFF)
	}
	else if(MCUBD_HEXSW_IN == 2)
	{//°´Â÷ BC ¸ðµâ3
		WGBC_TC0PC1 = WGBC_TC0;
		WGBC_M_ID = WGBC_M_ID3;
		MCU_LD4_ON();	// TCBC(ON), PCBC(OFF)
	}
	else if(MCUBD_HEXSW_IN == 3)
	{//°´Â÷ BC ¸ðµâ4
		WGBC_TC0PC1 = WGBC_TC0;
		WGBC_M_ID = WGBC_M_ID4;
		MCU_LD4_ON();	// TCBC(ON), PCBC(OFF)
	}
	else if(MCUBD_HEXSW_IN == 4)
	{//°´Â÷ BC ¸ðµâ5
		WGBC_TC0PC1 = WGBC_TC0;
		WGBC_M_ID = WGBC_M_ID5;
		MCU_LD4_ON();	// TCBC(ON), PCBC(OFF)
	}
	
	else if(MCUBD_HEXSW_IN == 5)
	{//µ¿·ÂÂ÷ BC ¸ðµâ1
		WGBC_TC0PC1 = WGBC_PC1;
		WGBC_M_ID = WGBC_M_ID1;
		MCU_LD4_OFF();	// TCBC(ON), PCBC(OFF)
	}
	else if(MCUBD_HEXSW_IN == 6)
	{//µ¿·ÂÂ÷ BC ¸ðµâ2
		WGBC_TC0PC1 = WGBC_PC1;
		WGBC_M_ID = WGBC_M_ID2;
		MCU_LD4_OFF();	// TCBC(ON), PCBC(OFF)
	}
	else if(MCUBD_HEXSW_IN == 7)
	{//µ¿·ÂÂ÷ BC ¸ðµâ3
		WGBC_TC0PC1 = WGBC_PC1;
		WGBC_M_ID = WGBC_M_ID3;
		MCU_LD4_OFF();	// TCBC(ON), PCBC(OFF)
	}

	else if(MCUBD_HEXSW_IN == 10)
	{//¸ðµâ Å×½ºÆ®¿ë
		WGBC_TC0PC1 = WGBC_PC1;
		WGBC_M_ID = WGBC_M_ID1;
		MCU_LD4_OFF();	// TCBC(ON), PCBC(OFF)
		ProgramMode_B0M1C2 = ProgramMode_MODULE1;//20150404Ãß°¡
	}
	else if(MCUBD_HEXSW_IN == 11)
	{//MCU BOARD °Ë»ç¿ë
		WGBC_TC0PC1 = WGBC_PC1;
		WGBC_M_ID = WGBC_M_ID1;
		MCU_LD4_OFF();	// TCBC(ON), PCBC(OFF)
		ProgramMode_B0M1C2 = ProgramMode_CPU2;//20150404Ãß°¡
	}
	else
	{
		WGBC_TC0PC1 = WGBC_TC0;
		WGBC_M_ID = WGBC_M_ID1;
		MCU_LD4_ON();	// TCBC(ON), PCBC(OFF)
		ProgramMode_B0M1C2 = ProgramMode_BOX0;//20150404Ãß°¡
	}
}

//==================================================================================//
// ÇÔ¼ö¸í : intÇÉ Ã¼Å©()		            										//
// DATE(¼öÁ¤)  : 2015. 04. 22														//
//==================================================================================//
void intpin_Chk()
{
	
	// WGMCU-INT ÀÔ·Â(DIR-->0)
	GpioCtrlRegs.GPBMUX2.bit.GPIO52 = 0;	//-INT0
	GpioCtrlRegs.GPBMUX2.bit.GPIO51 = 0;	//-INT1
	GpioCtrlRegs.GPBMUX2.bit.GPIO50 = 0;	//-INT2---pcb¿¡¼­ gnd·Î ¿¬°áµÊ ¶Ç´Â openµÇ¾î ÀÖÀ½
	GpioCtrlRegs.GPBMUX2.bit.GPIO49 = 0;	//-INT3
	GpioCtrlRegs.GPBMUX2.bit.GPIO48 = 0;	//-INT4
	GpioCtrlRegs.GPBMUX2.bit.GPIO58 = 0;	//-INT5
	GpioCtrlRegs.GPBMUX2.bit.GPIO57 = 0;	//-INT6
		

	GPIO_INT0 = GpioDataRegs.GPBDAT.bit.GPIO52;//-INT0
	GPIO_INT1 = GpioDataRegs.GPBDAT.bit.GPIO51;//SWHEX1--> HEX SWITCH BIT1
	GPIO_INT2 = GpioDataRegs.GPBDAT.bit.GPIO50;//SWHEX2--> HEX SWITCH BIT2
	GPIO_INT3 = GpioDataRegs.GPBDAT.bit.GPIO49;//SWHEX3--> HEX SWITCH BIT3
	
	GPIO_INT4 = GpioDataRegs.GPBDAT.bit.GPIO48;//SWDIP0
	GPIO_INT5 = GpioDataRegs.GPBDAT.bit.GPIO58;//SWDIP1
	GPIO_INT6 = GpioDataRegs.GPBDAT.bit.GPIO57;//SWDIP2

	if(!GPIO_INT0)		GPIO_L_INT0 = 1;//hwf0
	if(!GPIO_INT1)		GPIO_L_INT1 = 1;//hwf1
	//if(!GPIO_INT2)		GPIO_L_INT2 = GPIO_INT2;//hwf2
	if(!GPIO_INT3)		GPIO_L_INT3 = 1;//hwf2
	if(!GPIO_INT4)		GPIO_L_INT4 = GPIO_INT4;//--hwf1,2,3
	if(GPIO_INT5)		GPIO_L_INT5 = GPIO_INT5;//gdf,hwfs --->f¸é h, latch=0
	if(GPIO_INT6)		GPIO_L_INT6 = GPIO_INT6;//pwm


}

//==========================================================//
// º¯¼ö ÃÊ±âÈ­
//==========================================================//
void VariableInit()
{
	//ProgramMode_B0M1C2 = 0;	//20150404Ãß°¡
	
	msec_500m = msec_200m = msec_10m = msec_1m = msec_tmr = 0;	// ÀÏÁ¤ ÁÖ±â task¸¦ ¸¸µé±â À§ÇØ¼­ º¯¼ö ÃÊ±âÈ­

 	DioOutBuf1 = 0x02;
	MCUBD_PUSHBUTTONSW_IN = 0;
	//=====sequence °ü·Ãº¯¼ö ÃÊ±âÈ­============//
	Seq_Status = 0;	// Init = SeqStatus_INIT_STATE0(0)
	SEQ_Step = 0;	// Init = SEQ_DO_NOTHING_STATE(0)
	SeqTimer = 0;
	SeqTimerSet = 0;
	SeqTimer_flag = 0;

	//=====FAULT °ü·Ã º¯¼ö============//
	HighFaultMJF = 0;

	FGDF = 0;
	FHWF = 0;
	FSWF = 0;
 	FSQF = 0;	
    FETF = 0;				//
    FBXF = 0;				//
 
	FLGDF = FGDF;
	FLHWF = FHWF;
	FLSWF = FSWF;
	FLSQF = FSQF;
	FLETF = FETF;
	FLBXF = FBXF;  	
	
	
	FaultRestartTimer = 0;
	FaultRestartTimerSet = 500;		// faultÈÄ Àç±âµ¿ ½Ã°£ 10msec*500 = 5sec
	FaultRestartTimer_flag = FLAGSET;
	
	FaultCheckCounter = 0;
	
	
	MajorFault = 0;

	FaultCounter = 0;
	FaultCounter_old = 0;

	OverLoadAck = 0;	// °úºÎÇÏ¸í 1, °úºÎÇÏ 1ºÐÀÌ»ó Áö¼Ó½Ã ÆúÆ®Ã³¸®
	OverLoad_Counter = 0;
	THRFAck_Counter = 0; // È÷Æ®½ÌÅ© 90µµÀÌ»ó 1ºÐ
	FANF1Ack_Counter = 0;// ÆÒÆúÆ®1 10ÃÊ
	FANF2Ack_Counter = 0;// ÆÒÆúÆ®2 10ÃÊ
		
	PSF_Counter = 0;	// PSF µô·¹ÀÌ Ä«¿îÅÍ ÃÊ±âÈ­(10msec*20 = 200msec)
	//=====Acknowledge °ü·Ã º¯¼öÃÊ±âÈ­============//

	Mode_CV0CC1 = Mode_CV0;// CVCC ¸ðµå(#define Mode_CV0	0	#define Mode_CC1	1)
	MODEAck = NORMALMODE1; // TESTMODE0, NORMALMODE1
	MInputVolAck = 0;
 	FaultAck = 0;
 	FaultAckOld = 0;
	ModuleFault_NUM = 0;		
	Pall_OK_ModuleN = 0;
	RESETAck = 0;
	BATOC1Ack = 0;
	BATOC2Ack = 0;
	BCSTARTAck = 0;
	MOUTV_OKAck = 0;
	M1FAck = 0;
	M2FAck = 0;
	M3FAck = 0;
	M4FAck = 0;
	M5FAck = 0;
	MFAck_MY = 0;
	ModuleFault2ea_flag = 0; 	
	BCModeAck = 0;
	CHKAck = 0;
	LKAck = 0;
	BC72VAck = 0;
	TEMP85SWAck = 0;
	FANF1Ack = 0;
	FANF2Ack = 0;
	FUSEAck = 0;
	FanOnOffCmd_flag = 0;//fan off 	
	PWM_FLAG = 0;
	

	//---Àü¾ÐÁ¦¾î±â¸¸ ÇÒ¶§ °ÔÀÎ
	V_Kp = 0.005;// 0.05; 20150128_gain ¼öÁ¤
	V_Ki = 1.00;//0.03;

	I_Kp = 0.001;
	I_Ki = 0.025;

	I_Kp = 0.01;//-----
	I_Ki = 0.25;



	//I_Kp = 0.03;
	//I_Ki = 0.40;
	//---Àü·ùÁ¦¾î±âÃß°¡ÇÏ¸é¼­ º¯°æ-->20150202_23_58
	//--->1KW±âº»ºÎÇÏ¿¡¼­ 5KW±Þº¯-->10KWºÎÇÏ±Þº¯Àº ÆúÆ®³²
	V_Kp = 3.000;// 0.05; 
	V_Ki = 150.00;//0.03;
	I_Kp = 0.001;
	I_Ki = 1.000;
	//---20150314Æ©´× µ¿Â÷3¸ðµâ
	V_Kp = 3.0;//1.200;// 0.05; 
	V_Ki = 150.00;//0.03;
	I_Kp = 0.004;
	I_Ki = 0.100;	
	//---20150408Æ©´× µ¿Â÷3¸ðµâ
	V_Kp = 3.0;//1.200;// 0.05; 
	V_Ki = 150.00;//0.03;
	I_Kp = 0.0004;
	I_Ki = 4.000;	
//----À±´ë¸®

	//---Àü¾Ð+Àü·ù Á¦¾î±â °ÔÀÎ
	V_Kp = 60.0;// 0.05; 20150128_gain ¼öÁ¤
	V_Ki = 350.00;//0.03;

	I_Kp = .1;//0303
	I_Ki = 20.0;//0303	
	V_ref = 0.0;
	V_Duty_Uint = 0.0;
	OutPutCur_Ka = 1/I_Kp;
	OutPutVol_Ka = 1/V_Kp;
//---20150409Æ©´× µ¿Â÷3¸ðµâ
	V_Kp = 60.0;
	V_Ki = 350.00;
	I_Kp = 0.3;
	I_Ki = 30.;		
//---20150818Æ©´× °´Â÷Æ©´×--¸®Æ¬Æú¸®¸Ó¹èÅÍ¸®
	V_Kp = 60.0;//60-->30-->60.
	V_Ki = 350.00;
	I_Kp = 0.1;//0.3-->0.1
	I_Ki = 30.;		
	V_Duty_Uint = 0.0;
	OutPutCur_Ka = 1/I_Kp;
	OutPutVol_Ka = 1/V_Kp;	
//-----------
//	OutPutCur_Ka = 0.0;
	OutVol_KI_damp = 1.1;

//	DutyLimit = 0.92;//0.85;//¿ì¼± 80%µàÆ¼
	DutyLimit = 0.8;//0.85;//¿ì¼± 80%µàÆ¼
	DutyLimit = 0.9;//0.85;//¿ì¼± 80%µàÆ¼
	
	
//==¸ðµâ ÃæÀüÀü·ù Á¦ÇÑ°ü·Ã=================================//
	MBatCur_Kp = 0.001;
	MBatCur_Ki = 100.0;
	MBatCurRef = MBatCurRATED;	// ¸ðµâ ÃæÀüÀü·ùÁö·É 
//==¹Ú½º ÃæÀüÀü·ù Á¦ÇÑ°ü·Ã=================================//
	BBatCur_Kp = 1.0;//0.5;
	BBatCur_Ki = 0.2;

	//BBatCur_Kp = 0.05;//1.0;
	//BBatCur_Ki = 0.2;
	//BBatCur_Kp = 0.1;//0.05 -->0.1-->0.5-->0.1-->0.05;//20150811 ´õ¼ÇÅ×Å© ¹èÅÍ¸®·Î ½ÃÇè
	//BBatCur_Ki = 2.0;//0.2-->2.0-->5.0->2.0-->1.0;//20150811 ´õ¼ÇÅ×Å© ¹èÅÍ¸®·Î ½ÃÇè
	BBatCur_Kp = 0.005;//0.05 -->0.1-->0.5-->0.1-->0.05;//20150811 ´õ¼ÇÅ×Å© ¹èÅÍ¸®·Î ½ÃÇè
	BBatCur_Ki = 3.0;//0.2-->2.0-->5.0->2.0-->1.0;//20150811 ´õ¼ÇÅ×Å© ¹èÅÍ¸®·Î ½ÃÇè

	BBatCurRef = BBatCurRATED;//BBatCurRATED;
	
//=====Softstart Àü¾Ð ref ÁõºÐ°ª===========================		
	//DCDC_SoftStart_dv = 0.002;//0.005-->¾à 1.1ÃÊ
	DCDC_SoftStart_dv = 0.001;//0.002 ---> 0.001, 2015³â8¿ù11ÀÏ ´õ¼ÇÅ×Å© ¹èÅÍ¸®·Î ½ÃÇè
//	DCDC_SoftStart_dv = 0.001;//0.005-->¾à 1.1ÃÊ

	//========CAN °ü·Ãº¯¼ö

	cantxidx = 0;
	canrxidx = 0;


	InitADC_Var();	
	adc0_sum = 0;
	adc1_sum = 0;
	adc2_sum = 0;
	adc3_sum = 0;
	adc4_sum = 0;
	adc5_sum = 0;
	adc6_sum = 0;


	InitControlVar();
	
	BCM_HartBeat = 0;
	BCBox_Status = 0;
	
	BCM_Status1 = 0;
	BCM_Status2 = 0;
	BCM_Status3 = 0;
	BCM_Status4 = 0;

	BCM_TDF1 = 0;
	BCM_TDF2 = 0;
	BCM_TDF3 = 0;
	BCM_TDF4 = 0;

	BCM_SDF1 = 0;
	BCM_SDF2 = 0;
	BCM_SDF3 = 0;
	BCM_SDF4 = 0;  

}    

//==================================================================================//
// ÇÔ¼ö¸í : Setting_Chk()		            										//
// DATE(¼öÁ¤)  : 2015. 01. 16														//
//==================================================================================//
void Setting_Chk_old()
{
	Uint16 Temp0_GPIO = 0;
	Uint16 Temp1_GPIO = 0;
	Uint16 Temp2_GPIO = 0;
	Uint16 Temp3_GPIO = 0;		
	

	Temp0_GPIO = GpioDataRegs.GPADAT.bit.GPIO12;//SWHEX0--> HEX SWITCH BIT0
	Temp1_GPIO = GpioDataRegs.GPADAT.bit.GPIO13;//SWHEX1--> HEX SWITCH BIT1
	Temp2_GPIO = GpioDataRegs.GPADAT.bit.GPIO14;//SWHEX2--> HEX SWITCH BIT2
	Temp3_GPIO = GpioDataRegs.GPADAT.bit.GPIO15;//SWHEX3--> HEX SWITCH BIT3
	MCUBD_HEXSW_IN = !Temp3_GPIO*8 + !Temp2_GPIO*4 +!Temp1_GPIO*2 +!Temp0_GPIO*1;
	
	Temp0_GPIO = GpioDataRegs.GPADAT.bit.GPIO22;//SWDIP0
	Temp1_GPIO = GpioDataRegs.GPADAT.bit.GPIO23;//SWDIP1
	Temp2_GPIO = GpioDataRegs.GPADAT.bit.GPIO24;//SWDIP2
	Temp3_GPIO = GpioDataRegs.GPADAT.bit.GPIO25;//SWDIP3

	MCUBD_DIPSW_IN = (!Temp3_GPIO*8 + !Temp2_GPIO*4 +!Temp1_GPIO*2 +!Temp0_GPIO*1)&0xff;
	MCUBD_MODESW_IN = GpioDataRegs.GPADAT.bit.GPIO26;//SWSEL---> MODE SWITCH·Î »ç¿ëÇÒ±î?(TEST MODE / NORMAL MODE)
	MCUBD_PUSHBUTTONSW_IN = !GpioDataRegs.GPADAT.bit.GPIO27;//SWPB - PUSH BUTTON SW	

//---¸ðµâ ¸ðµå (Å×½ºÆ®¸ðµå/NORMAL ¸ðµå)
	MODEAck = GpioDataRegs.GPADAT.bit.GPIO26;//SWSEL---> MODE SWITCH·Î »ç(TEST MODE(0) / NORMAL MODE(1))

	if((MCUBD_HEXSW_IN == 0)|(MCUBD_HEXSW_IN == 5)){//°´Â÷ BC ¸ðµâ1
		WGBC_TC0PC1 = WGBC_TC0;
		WGBC_M_ID = WGBC_M_ID1;
		MCU_LD4_ON();	// TCBC(ON), PCBC(OFF)
	}
	else if((MCUBD_HEXSW_IN == 1)|(MCUBD_HEXSW_IN == 6)){//°´Â÷ BC ¸ðµâ2
		WGBC_TC0PC1 = WGBC_TC0;
		WGBC_M_ID = WGBC_M_ID2;
		MCU_LD4_ON();	// TCBC(ON), PCBC(OFF)
	}
	else if((MCUBD_HEXSW_IN == 2)|(MCUBD_HEXSW_IN == 7)){//°´Â÷ BC ¸ðµâ3
		WGBC_TC0PC1 = WGBC_TC0;
		WGBC_M_ID = WGBC_M_ID3;
		MCU_LD4_ON();	// TCBC(ON), PCBC(OFF)
	}
	else if((MCUBD_HEXSW_IN == 3)|(MCUBD_HEXSW_IN == 8)){//°´Â÷ BC ¸ðµâ4
		WGBC_TC0PC1 = WGBC_TC0;
		WGBC_M_ID = WGBC_M_ID4;
		MCU_LD4_ON();	// TCBC(ON), PCBC(OFF)
	}
	else if((MCUBD_HEXSW_IN == 4)|(MCUBD_HEXSW_IN == 9)){//°´Â÷ BC ¸ðµâ5
		WGBC_TC0PC1 = WGBC_TC0;
		WGBC_M_ID = WGBC_M_ID5;
		MCU_LD4_ON();	// TCBC(ON), PCBC(OFF)
	}
	
	else if((MCUBD_HEXSW_IN == 10)|(MCUBD_HEXSW_IN == 13)){//µ¿·ÂÂ÷ BC ¸ðµâ1
		WGBC_TC0PC1 = WGBC_PC1;
		WGBC_M_ID = WGBC_M_ID1;
		MCU_LD4_OFF();	// TCBC(ON), PCBC(OFF)
	}
	else if((MCUBD_HEXSW_IN == 11)|(MCUBD_HEXSW_IN == 14)){//µ¿·ÂÂ÷ BC ¸ðµâ2
		WGBC_TC0PC1 = WGBC_PC1;
		WGBC_M_ID = WGBC_M_ID2;
		MCU_LD4_OFF();	// TCBC(ON), PCBC(OFF)
	}
	else if((MCUBD_HEXSW_IN == 12)|(MCUBD_HEXSW_IN == 15)){//µ¿·ÂÂ÷ BC ¸ðµâ3
		WGBC_TC0PC1 = WGBC_PC1;
		WGBC_M_ID = WGBC_M_ID3;
		MCU_LD4_OFF();	// TCBC(ON), PCBC(OFF)
	}
	else{
		WGBC_TC0PC1 = WGBC_TC0;
		WGBC_M_ID = WGBC_M_ID1;
		MCU_LD4_ON();	// TCBC(ON), PCBC(OFF)
	}
}
