//==================================================================================//
// ¿ø°­¼± µ¿·ÂÂ÷(30kw)/°´Â÷(50kw) ÃàÀüÁö ÃæÀü±â ¸ğµâ(10kw) ÇÁ·Î±×·¥	           		//
//==================================================================================//
// ÆúÆ®Ã³¸®ºÎºĞ                                                                     //
// DATE  : 2015. 01. 07            						  by PACTECH R&D LSH		//
//==================================================================================//
//°æ°íÀå : ´ëºÎºĞ °æ°íÀåÀ¸·Î ¹«ÇÑ Àç±âµ¿ °¡´ÉÇÔ                                     //
//   ´Ü, 3ºĞÀÌ³» 3È¸ ÆúÆ®½Ã Áß°íÀå Ã³¸®´Â ¾Æ·¡                                      //
//      --> GDF1,2 - Â÷·® ¸®¼Â¿¡ÀÇÇØ Àç±âµ¿ ºÒ°¡                                    //
//      --> ÀÔ·Â°úÀü·ù, ¸ğµâ Ãâ·Â°úÀü·ù - Â÷·® ¸®¼Â¿¡ÀÇÇØ Àç±âµ¿ °¡´É               //
//Áß°íÀå : ÈŞÁîÆúÆ®, ÄÁÅØÅÍ ¼Ò¼Õ, ¹èÅÍ¸® °úÀü·ù(Â÷·® CB½ÅÈ£) -->Â÷·®¸®¼Â Àç±âµ¿ ºÒ°¡//
//==================================================================================//
#include "Global_Variable_Function.h"

unsigned int DioInPutBuf_tst = 0x01F0;

float BBatCur2LPF_max = 0;
float BBatCur1LPF_max = 0;

Uint16 MOutVolSWOVF_Counter = 0.0;
Uint16 MBatVolSWOVF_Counter = 0.0;
Uint16 BBatCur1SWOVF_Counter = 0.0;
Uint16 BBatCur2SWOVF_Counter = 0.0;
Uint16 MOutVolSWUVF_Counter = 0;
Uint16 MOutVolSWUVF_Counter1 = 0;
Uint16 MOutVolSWUVF_Counter2 = 0;

Uint16 MInputVolUVF_Counter = 0;
float MBatCurNC_Counter=0;

unsigned int FLGDF_1ST = 0x00;	
unsigned int FLHWF_1ST = 0x00;
unsigned int FLSWF_1ST = 0x00;
unsigned int FLSQF_1ST = 0x00;
unsigned int FLETF_1ST = 0x00;	
unsigned int FLBXF_1ST = 0x00;
unsigned int test_OVF_flag1 = 0;
unsigned int test_OVF_flag2 = 0;
extern intpin_Chk();
//==================================================================================//
// ÇÔ¼ö¸í : SW_FaultProcess()		     										//
// ±â  ´É : S/W FAULT 1°³¶óµµ ¹ß»ıÇÏ¸é dsp card epld¿¡ sw fault¸¦ ½áÁÜ				//
//----------------------------------------------------------------------------------//
// Àü·ù : HW·Î FAULT, SW(MIN/MAX)·Î LIMITÁ¦¾î										//
// Àü¾Ğ : SW·Î FAULT, SW(MIN/MAX)·Î LIMITÁ¦¾î										//
//----------------------------------------------------------------------------------//
// DATE  : 2010. 08. 13																//
//==================================================================================//
void Check_SWF()
{	
	float Temp = 0;

	Temp = fabs(MInputCurLPF);	// ÀÔ·Â°úÀü·ù
	if (Temp > MInputCurSWOCF){
//	if (Temp > MInputCurSWOCF+10){
		sbit(MidFaultAck, MidMICOCF);	// Áß°£·¹º§(3ºĞ3È¸) - ÀÔ·Â°úÀü·ù
		sbit(FSWF,SWF0_ICOCF);
	}
	else{
		cbit(FSWF,SWF0_ICOCF);
	}
	Temp = fabs(MBatCurLPF);	//¸ğµâ ÃæÀüÀü·ù °úÀü·ù
	if (Temp > MBatCurSWOCF){
		LowFaultAck = 1;	// Low Level ÆúÆ®·Î Ã³¸® --> Á¶°Ç ÇØÁ¦½Ã ¹«ÇÑ Àç±âµ¿
		sbit(FSWF,SWF1_MBATOCF);
	}
	else{
		cbit(FSWF,SWF1_MBATOCF);
	}

	Temp = fabs(MOutCurLPF);	//¸ğµâ Ãâ·ÂÀü·ù °úÀü·ù
	if (Temp > MOutCurSWOCF){
		sbit(MidFaultAck, MidMICOCF);	// Áß°£·¹º§(3ºĞ3È¸) - ÀÔ·Â°úÀü·ù
		sbit(FSWF,SWF2_MOCF);
	}
	else{
		cbit(FSWF,SWF2_MOCF);
	}

//BOX ÃæÀüÀü·ù1 °úÀü·ù
	Temp = fabs(BBatCur1LPF);	//BOX ÃæÀüÀü·ù1 °úÀü·ù
	if (Temp > BBatCur1SWOCF){
		BBatCur1SWOVF_Counter++;
	//	if(BBatCur1SWOVF_Counter > 600){//OVF·¹º§º¸´Ù Å«°ªÀÌ 300¹øÀÌ»ó(600*66usec = 36msec) Áö¼ÓµÇ¸é ÀÌ¸é ÆúÆ®Ã³¸®
		if(BBatCur1SWOVF_Counter > 7500){ // 20151117_Ã¢¿ø¿¡¼­ °´Â÷ BOXÃæÀü°úÀü·ù ¹ß»ıÇØ¼­ ¼öÁ¤
			LowFaultAck = 1;	
			sbit(FSWF,SWF4_BBAT1OCF);
			BBatCur1SWOVF_Counter = 0;
		}
//		if(Temp > BBatCur1SWOCF+80){//OVF+80Vº¸´Ù Å©¸é  Áï½Ã ÆúÆ®Ã³¸®
		if(Temp > BBatCur1SWOCF+100){// 20151117_Ã¢¿ø¿¡¼­ °´Â÷ BOXÃæÀü°úÀü·ù ¹ß»ıÇØ¼­ ¼öÁ¤
			LowFaultAck = 1;	
			sbit(FSWF,SWF4_BBAT1OCF);
			BBatCur1SWOVF_Counter = 0;
		}
	}
	else{
		BBatCur1SWOVF_Counter = 0;
	}
//BOX ÃæÀüÀü·ù2 °úÀü·ù
	Temp = fabs(BBatCur2LPF);	//BOX ÃæÀüÀü·ù2 °úÀü·ù
	if (Temp > BBatCur2SWOCF){
		BBatCur2SWOVF_Counter++;
//		if(BBatCur2SWOVF_Counter > 600){//OVF·¹º§º¸´Ù Å«°ªÀÌ 100¹øÀÌ»ó(600*66usec = 36msec) Áö¼ÓµÇ¸é ÀÌ¸é ÆúÆ®Ã³¸®
				if(BBatCur2SWOVF_Counter > 7500){ // 20151117_Ã¢¿ø¿¡¼­ °´Â÷ BOXÃæÀü°úÀü·ù ¹ß»ıÇØ¼­ ¼öÁ¤
			LowFaultAck = 1;	
			sbit(FSWF,SWF5_BBAT2OCF);
			BBatCur2SWOVF_Counter = 0;
		}
//		if(Temp > BBatCur2SWOCF+80){//OVF+80Vº¸´Ù Å©¸é  Áï½Ã ÆúÆ®Ã³¸®
		if(Temp > BBatCur2SWOCF+100){// 20151117_Ã¢¿ø¿¡¼­ °´Â÷ BOX ÃæÀü°úÀü·ù ¹ß»ıÇØ¼­ ¼öÁ¤
			LowFaultAck = 1;	
			sbit(FSWF,SWF5_BBAT2OCF);
			BBatCur2SWOVF_Counter = 0;
		}
	}
	else{
		BBatCur2SWOVF_Counter = 0;
	}

	
	Temp = fabs(BOutCurLPF);	//BOX Ãâ·ÂÀü·ù °úÀü·ù
	if (Temp > BOutCurSWOCF){
		LowFaultAck = 1;	// Low Level ÆúÆ®·Î Ã³¸® --> Á¶°Ç ÇØÁ¦½Ã ¹«ÇÑ Àç±âµ¿
		sbit(FSWF,SWF6_BOCF);
	}
	
	
//---Á¦¾îÀü¾Ğ °úÀü¾Ğ °Ë»ç---
	Temp = fabs(MCtrlVolLPF);	//Á¦¾îÀü¾Ğ °úÀü¾Ğ -----> ADC·Î ÀĞÀº°ªÀ¸·Î ÀúÀü¾Ğ/°úÀü¾Ğ µÑ´Ù ÇØ¾ßÇÔ
	if (Temp > MCtrlVolSWOVF){
		LowFaultAck = 1;	// Low Level ÆúÆ®·Î Ã³¸® --> Á¶°Ç ÇØÁ¦½Ã ¹«ÇÑ Àç±âµ¿
		sbit(FSWF,SWF7_CPOVF);
	}
	else{
		cbit(FSWF,SWF7_CPOVF);
	}
	
//---Á¦¾îÀü¾Ğ ÀúÀü¾Ğ °Ë»ç---Á¦¾îÀü¾Ğ ÀúÀü¾ĞÀº Á¶°Ç¿¡ °ü°è¾øÀÌ Ç×»ó °Ë»ç
	Temp = fabs(MCtrlVolLPF);	//Á¦¾îÀü¾Ğ ÀúÀü¾Ğ -----> ADC·Î ÀĞÀº°ªÀ¸·Î ÀúÀü¾Ğ/°úÀü¾Ğ µÑ´Ù ÇØ¾ßÇÔ
	if (Temp < MCtrlVolUVF){
		LowFaultAck = 1;	// Low Level ÆúÆ®·Î Ã³¸® --> Á¶°Ç ÇØÁ¦½Ã ¹«ÇÑ Àç±âµ¿
		sbit(FSWF,SWF14_CPUVF);
	}
	else{
		cbit(FSWF,SWF14_CPUVF);
	}

//---Ãâ·ÂÀü¾Ğ °úÀü¾Ğ °Ë»ç	
	Temp = fabs(MOutVolLPF);	//Ãâ·ÂÀü¾Ğ °úÀü¾Ğ(OBDÀü)
	if (Temp > MOutVolSWOVF){				
		MOutVolSWOVF_Counter++;
		if(MOutVolSWOVF_Counter > 100){//OVF·¹º§º¸´Ù Å«°ªÀÌ 100¹øÀÌ»ó(100*66usec = 6.6msec) Áö¼ÓµÇ¸é ÀÌ¸é ÆúÆ®Ã³¸®
			LowFaultAck = 1;	
			sbit(FSWF,SWF8_OVOVF);
			MOutVolSWOVF_Counter = 0;
		}
		if(Temp > MOutVolSWOVF+12){//OVF+10Vº¸´Ù Å©¸é  Áï½Ã ÆúÆ®Ã³¸®
			LowFaultAck = 1;	
			sbit(FSWF,SWF8_OVOVF);
			MOutVolSWOVF_Counter = 0;
		}
	}
	else{
		MOutVolSWOVF_Counter = 0;
	}
//---ÃÖÁ¾Ãâ·ÂÀü¾Ğ(¹èÅÍ¸®Ãø) °úÀü¾Ğ °Ë»ç	
	Temp = fabs(MBatVolLPF);	//Ãâ·ÂÀü¾Ğ °úÀü¾Ğ(OBDÀü)
	if (Temp > MBatVolSWOVF){
		MBatVolSWOVF_Counter++;
		if(MBatVolSWOVF_Counter > 100){//OVF·¹º§º¸´Ù Å«°ªÀÌ 100¹øÀÌ»ó(100*66usec = 6.6msec) Áö¼ÓµÇ¸é ÀÌ¸é ÆúÆ®Ã³¸®
			LowFaultAck = 1;	
			sbit(FSWF,SWF9_BVOVF);
			MBatVolSWOVF_Counter = 0;
		}
		if(Temp > MBatVolSWOVF+12){//OVF+10Vº¸´Ù Å©¸é  Áï½Ã ÆúÆ®Ã³¸®
			LowFaultAck = 1;	
			sbit(FSWF,SWF9_BVOVF);
			MBatVolSWOVF_Counter = 0;
		}
	}
	else{
		MBatVolSWOVF_Counter = 0;
	}	


//===20160203 ÀÔ·Â°úÀü¾Ğ Ã¼Å© ·ÎÁ÷º¯°æ(µô·¹ÀÌÁÜ)
//===1Æí¼º ¿îÇàÁß ÀÔ·Â°úÀü¾Ğ ÆúÆ®¹ß»ı, STARTÀü ÀÔ·ÂÀü¾Ğ 940V±îÁö »ó½ÂÇÏ°í 920V±ÙÃ³¿¡¼­ START½ÅÈ£°¡ µé¾î¿È
//===±âÁ¸ : Ç×»ó ÀÔ·Â°úÀü¾Ğ ÆúÆ®°ËÁö
//===º¯°æ : START½ÅÈ£°¡ µé¾î¿À¸é START½ÅÈ£(BCSTARTAck)¸¦ 0.3ÃÊÈÄ¿¡ ÀÎ½ÄÇÏ°í, BCSTARTAck°¡ ÀÎ½ÄµÇ¸é ÀÔ·Â°úÀü¾Ğ Ã¼Å©
//==----->Æò»ó½Ã¿¡´Â ÀÔ·Â°úÀü¾Ğ °ËÁö¾ÈÇÏ´Â °ÍÀÓ
//====================================================================
//---ÀÔ·ÂÀü¾Ğ °úÀü¾Ğ °Ë»ç(1Æí¼º Ãâ°í½Ã)
	//Temp = fabs(MInputVolLPF);	//¸ğµâÀÔ·ÂÀü¾Ğ --> ADCRD10
	//if (Temp > MInputVolSWOVF){
	//	LowFaultAck = 1;	// Low Level ÆúÆ®·Î Ã³¸® --> Á¶°Ç ÇØÁ¦½Ã ¹«ÇÑ Àç±âµ¿
	//	sbit(FSWF,SWF10_IVOVF);
	//}
//---ÀÔ·ÂÀü¾Ğ °úÀü¾Ğ °Ë»ç(20160203 ·ÂúÀü¾Ğ Ã¼Å© ·ÎÁ÷º¯°æ(µô·¹ÀÌÁÜ)	
	//if(BCSTARTAck == YES){
	//	Temp = fabs(MInputVolLPF);	//¸ğµâÀÔ·ÂÀü¾Ğ --> ADCRD10
	//	if (Temp > MInputVolSWOVF){
	//		LowFaultAck = 1;	// Low Level ÆúÆ®·Î Ã³¸® --> Á¶°Ç ÇØÁ¦½Ã ¹«ÇÑ Àç±âµ¿
	//		sbit(FSWF,SWF10_IVOVF);
	//	}
	//}
//===20160324 ÀÔ·Â°úÀü¾Ğ Ã¼Å© ·ÎÁ÷º¯°æ(¼ö¼­²¨ ¹®Á¦¹®Á¦µÇ¾î °úÀü¾Ğ ·ÎÁ÷ Ãß°¡º¸¿Ï)
//===¼ö¼­Æí¼º ¿îÇàÁß ÀÔ·Â°úÀü¾Ğ ÆúÆ®¹ß»ı-->·ÎÅÛ Ãß°¡º¸¿Ï¿äÃ»
//===±âÁ¸ : START½ÅÈ£°¡ µé¾î¿À¸é START½ÅÈ£(BCSTARTAck)¸¦ 0.3ÃÊÈÄ¿¡ ÀÎ½ÄÇÏ°í, BCSTARTAck ÀÎ½ÄµÇ¸é ÀÔ·Â°úÀü¾Ğ Ã¼Å©
//==-------->Æò»ó½Ã¿¡´Â ÀÔ·Â°úÀü¾Ğ °ËÁö¾ÈÇÏ´Â °ÍÀÓ
//===º¯°æ : ±âµ¿Á¶°Ç(590V~810V)¸¸Á·µÇ¸é ±âµ¿½ÃÀÛ --ÀÔ·Â°úÀü¾ĞÆúÆ®°ËÁö´Â ¾ÆÁ÷ ¾ÈÇÔ
//          START½ÅÈ£°¡ µé¾î¿À¸é START½ÅÈ£(BCSTARTAck)¸¦ 0.3ÃÊÈÄ¿¡ ÀÎ½ÄÇÏ°í, BCSTARTAck°¡ ÀÎ½ÄµÇ¸é ±âµ¿½ÃÀÛ
//          CHKµ¿ÀÛ½ÃÄÑ µ¿ÀÛÈ®ÀÎµÇ¸é ÀÔ·Â°úÀü¾Ğ Ã¼Å©
//====================================================================	
//---ÀÔ·ÂÀü¾Ğ °úÀü¾Ğ °Ë»ç(20160324 ÀÔ·Â°úÀü¾Ğ Ã¼Å© ·ÎÁ÷º¯°æ(Ãß°¡ º¸¿Ï)	

	if((SEQ_Step >= SEQ_LK_ON)&&(BCSTARTAck == YES)){//CHKAÈ®ÀÎµÇ¸é SEQ_StepÀÌ SEQ_LK_ONº¸´Ù Å©°Å³ª °°À¸¹Ç·Î ÀÔ·Â°úÀü¾Ğ Ã¼Å©ÇÔ
		Temp = fabs(MInputVolLPF);	//¸ğµâÀÔ·ÂÀü¾Ğ --> ADCRD10
		if (Temp > MInputVolSWOVF){
			LowFaultAck = 1;	// Low Level ÆúÆ®·Î Ã³¸® --> Á¶°Ç ÇØÁ¦½Ã ¹«ÇÑ Àç±âµ¿
			sbit(FSWF,SWF10_IVOVF);
		
		}
	}	
	
			
	
//---DCLINKÀü¾Ğ °úÀü¾Ğ °Ë»ç	
	Temp = fabs(MDcLinkVolLPF);	//¸ğµâ DCLINKÀü¾Ğ(RATED 670Vdc) --> ADCRD11
	if (Temp > MDclinkVolSWOVF){
		LowFaultAck = 1;	// Low Level ÆúÆ®·Î Ã³¸® --> Á¶°Ç ÇØÁ¦½Ã ¹«ÇÑ Àç±âµ¿
		sbit(FSWF,SWF11_FCOVF);
	}
	
//---ÀúÀü¾Ğ °Ë»ç	

	if((SEQ_Step == SEQ_Normal_State)&&(MODEAck == NORMALMODE1)){
		//Temp = fabs(MInputVolLPF);	//¸ğµâÀÔ·ÂÀü¾Ğ --> ADCRD10
		//if (Temp < MInputVolUVF){	//620.0;		//»ó¼¼¼³°è¼­ ½ÃÇè±âÁØ 620V
		//	LowFaultAck = 1;	// Low Level ÆúÆ®·Î Ã³¸® --> Á¶°Ç ÇØÁ¦½Ã ¹«ÇÑ Àç±âµ¿
		//	sbit(FSWF,SWF13_IVUVF);
		//}
//======ÀÔ·ÂÀúÀü¾Ğ Ã³¸® º¯°æ 20160129=========================================
//======1ÃÊµ¿¾È °è¼Ó ÀÔ·ÂÀúÀü¾ĞÀÌ À¯ÁöµÇ¸é ÆúÆ®Ã³¸®·Î º¯°æ
		Temp = fabs(MInputVolLPF);	//¸ğµâÀÔ·ÂÀü¾Ğ --> ADCRD10
		if (Temp < MInputVolUVF){	//620.0;		//»ó¼¼¼³°è¼­ ½ÃÇè±âÁØ 620V
			MInputVolUVF_Counter++;
			if(MInputVolUVF_Counter>15000){	// 66.6667usec * 15000 = 1sec
				MInputVolUVF_Counter = 0;
				LowFaultAck = 1;	// Low Level ÆúÆ®·Î Ã³¸® --> Á¶°Ç ÇØÁ¦½Ã ¹«ÇÑ Àç±âµ¿
				sbit(FSWF,SWF13_IVUVF);
			}
		}
		else{
			MInputVolUVF_Counter = 0;
		}
/*
		//---Ãâ·ÂÀúÀü¾ĞÀº PWM ON½Ã¸¸ °Ë»ç ---> ºÎÇÏ½Ã STOPÇßÀ»¶§ ÀúÀü¾Ğ °ËÁö ¹æÁö¸ñÀû
		if(PWM_FLAG == YES){
			MOutVolSWUVF_Counter++;
			//if(MOutVolSWUVF_Counter>20000){
			//	MOutVolSWUVF_Counter = 20001;
			if(MOutVolSWUVF_Counter>50000){//20160211->20000(1.3sec)-->50000(3.33ÃÊ)
				MOutVolSWUVF_Counter = 0;
				Temp = fabs(MBatVolLPF);	//Ãâ·ÂÀü¾Ğ ÀúÀü¾Ğ(OBDÈÄ)
				if (Temp < MBatVolUVF){	//50.0;		//»ó¼¼¼³°è¼­ ½ÃÇè±âÁØ 50V
					LowFaultAck = 1;	// Low Level ÆúÆ®·Î Ã³¸® --> Á¶°Ç ÇØÁ¦½Ã ¹«ÇÑ Àç±âµ¿
					//sbit(FSWF,SWF12_OVUVF);
					sbit(FSWF,SWF15_OVUVF2);//20150922 ÃÖÁ¾Ãâ·ÂÀúÀü¾Ğ¸¸ º°µµ·Î bit15¿¡ ÇÒ´ç-->¸ğ´ÏÅÍ¸µÇÁ·Î±×·¥¿¡¼­ ÃÖÁ¾Ãâ·ÂÀü¾ĞÀúÀü¾Ğ ÆúÆ®Ç¥½ÃÇÏ°Ô
				}
				Temp = fabs(MOutVolLPF);	//Ãâ·ÂÀü¾Ğ ÀúÀü¾Ğ(OBDÀü)
				if (Temp < MOutVolUVF){	//50.0;		//»ó¼¼¼³°è¼­ ½ÃÇè±âÁØ 50V
					LowFaultAck = 1;	// Low Level ÆúÆ®·Î Ã³¸® --> Á¶°Ç ÇØÁ¦½Ã ¹«ÇÑ Àç±âµ¿
					sbit(FSWF,SWF12_OVUVF);
				}
			}
		}
		else{
			MOutVolSWUVF_Counter = 0;
		}	
*/		

		//---Ãâ·ÂÀúÀü¾ĞÀº PWM ON½Ã¸¸ °Ë»ç ---> ºÎÇÏ½Ã STOPÇßÀ»¶§ ÀúÀü¾Ğ °ËÁö ¹æÁö¸ñÀû
		if(PWM_FLAG == YES){
			MOutVolSWUVF_Counter++;
			if(MOutVolSWUVF_Counter>50000){//20160211->20000(1.3sec)-->50000(3.33ÃÊ)
				MOutVolSWUVF_Counter = 50001;
				Temp = fabs(MBatVolLPF);	//Ãâ·ÂÀü¾Ğ ÀúÀü¾Ğ(OBDÈÄ)
				if (Temp < MBatVolUVF){	//50.0;		//»ó¼¼¼³°è¼­ ½ÃÇè±âÁØ 50V
					MOutVolSWUVF_Counter2++;
					if(MOutVolSWUVF_Counter2>5000){	// 66.6667usec * 7500 = 0.5sec
						MOutVolSWUVF_Counter2 = 0;
						LowFaultAck = 1;	// Low Level ÆúÆ®·Î Ã³¸® --> Á¶°Ç ÇØÁ¦½Ã ¹«ÇÑ Àç±âµ¿
						sbit(FSWF,SWF15_OVUVF2);//20150922 ÃÖÁ¾Ãâ·ÂÀúÀü¾Ğ¸¸ º°µµ·Î bit15¿¡ ÇÒ´ç-->¸ğ´ÏÅÍ¸µÇÁ·Î±×·¥¿¡¼­ ÃÖÁ¾Ãâ·ÂÀü¾ĞÀúÀü¾Ğ ÆúÆ®Ç¥½ÃÇÏ°Ô
					}
				}
				else{
					MOutVolSWUVF_Counter2 = 0;
				}
				Temp = fabs(MOutVolLPF);	//Ãâ·ÂÀü¾Ğ ÀúÀü¾Ğ(OBDÀü)
				if (Temp < MOutVolUVF){	//50.0;		//»ó¼¼¼³°è¼­ ½ÃÇè±âÁØ 50V
					MOutVolSWUVF_Counter1++;
					if(MOutVolSWUVF_Counter1>5000){	// 66.6667usec * 75000 = 1sec
						MOutVolSWUVF_Counter1 = 0;
						LowFaultAck = 1;	// Low Level ÆúÆ®·Î Ã³¸® --> Á¶°Ç ÇØÁ¦½Ã ¹«ÇÑ Àç±âµ¿
						sbit(FSWF,SWF12_OVUVF);//20150922 ÃÖÁ¾Ãâ·ÂÀúÀü¾Ğ¸¸ º°µµ·Î bit15¿¡ ÇÒ´ç-->¸ğ´ÏÅÍ¸µÇÁ·Î±×·¥¿¡¼­ ÃÖÁ¾Ãâ·ÂÀü¾ĞÀúÀü¾Ğ ÆúÆ®Ç¥½ÃÇÏ°Ô
					}
				}
				else{
					MOutVolSWUVF_Counter1 = 0;
				}
			}
		}
		else{
			MOutVolSWUVF_Counter = 0;
			MOutVolSWUVF_Counter1 = 0;
			MOutVolSWUVF_Counter2 = 0;
		}				
			
	}	


	//----SW Fault°¡ ÇÏ³ª¶óµµ ÀÖÀ¸¸é -- EPLD·Î SW Fault¸¦ Ãâ·ÂÇÏ¿© ÆúÆ®·¡Ä¡½ÃÅ´----//
	if(FSWF > 0){
	//	FAULTGENSW(); // ¶Ç´Â R_DSP_SW_FAULT_CS() = 0x00;
	}

	if(BBatCur2LPF_max < BBatCur2LPF) BBatCur2LPF_max = BBatCur2LPF;
	if(BBatCur1LPF_max < BBatCur1LPF) BBatCur1LPF_max = BBatCur1LPF;

}

//==================================================================================//
// ÇÔ¼ö¸í : ÆúÆ® Å×½ºÆ®¿ë ()														//
//----------------------------------------------------------------------------------//
// DATE  : 2015. 4. 28																//
//==================================================================================//
void Check_TESTF()
{
	if(btst(MCUBD_DIPSW_IN,1)){//Áß°£·¹º§(3ºĞ3È¸) °íÀå Å×½ºÆ®¿ë-- GDF1
		//FaultTest_flag = 1;
		FGDF = 1;
		sbit(MidFaultAck, MidGDF);		// Áß°£·¹º§(3ºĞ3È¸) - GDF1
	}
	if(btst(MCUBD_DIPSW_IN,0)){//Áß°íÀå Å×½ºÆ®¿ë-- iocF
		MidFaultMJF = 1;
		sbit(FSWF,SWF0_ICOCF);
	}
}
//==================================================================================//
// ÇÔ¼ö¸í : GDU_FaultProcess()														//
//----------------------------------------------------------------------------------//
// DATE  : 2015. 1. 7																//
//==================================================================================//
void Check_GDF()
{
	FGDF = CS_RD_GDF() & 0x03; 

	if(btst(FGDF,GDF1)){			//(dcdc-Leading Leg)
		sbit(MidFaultAck, MidGDF);		// Áß°£·¹º§(3ºĞ3È¸) - GDF
	}
	
	if(btst(FGDF,GDF2)){			//(dcdc-Lagging Leg)
		sbit(MidFaultAck, MidGDF);		// Áß°£·¹º§(3ºĞ3È¸) - GDF
	}	

}

//==========20160229---Â÷·®¿¡¼­ ¹èÅÍ¸®ÈŞÁîÆúÆ® ¹è¼± ¼öÁ¤¾ÈÇØ¼­---------
void Check_BXF()
{
	unsigned int ModuleFault_NUM_temp = 0;	

	//====¹èÅÍ¸® °úÀü·ù(¿ÜºÎÀÔ·Â) Ã¼Å©
	//===¿ÜºÎ ¹èÅÍ¸® ÈŞÁîÆúÆ® ·ÎÁ÷Ã³¸®¸¦ µö½ºÀ§Ä¡ bit3À¸·Î ÆÇ´ÜÇÏ°Ô
	//===°á·ĞÀº µö½ºÀ§Ä¡ ·ÎÁ÷¿¡ µû¶ó ÈŞÁîÆúÆ® ·ÎÁ÷ º¯°æÇÏ°Ô ÇÔ
	if(!btst(MCUBD_DIPSW_IN,3)){//===dipsw.3ÀÌ L¸é --->±âÁ¸ °´Â÷¿Í °°ÀÌ
//============================
		if(WGBC_TC0PC1 == WGBC_TC0){	// °´Â÷¿ë ÃàÀüÁö ÃæÀü±â
			if(!btst(DioInPutBuf,BATOC1_Chk)){ // 20151117_BATOC1 ³ë¸ÖÀÏ ¶§ High·Î ¼öÁ¤ - °´Â÷
				BATOC1Ack = NO; // 
			}
			else{
				BATOC1Ack = YES; //
				HighFaultAck = 1;
				HighFaultMJF = 1;
				sbit(FBXF, BOXF_BATOC1);
			}
			BATOC2Ack = NO;
			cbit(FBXF, BOXF_BATOC2);		
		}
		else
		{	// µ¿·ÂÂ÷¿ë ÃàÀüÁö ÃæÀü±â
			if(!btst(DioInPutBuf,BATOC1_Chk))
			{ // 20160218_Â÷·® ·ÎÁ÷ ¼³°è º¯°æÀ¸·Î °´Â÷¶û µ¿ÀÏÇÏ°Ô ¼öÁ¤_BATOC1 ³ë¸ÖÀÏ ¶§ High
				BATOC1Ack = NO;
			}
			else
			{
				BATOC1Ack = YES;
				HighFaultAck = 1;
				HighFaultMJF = 1;
				sbit(FBXF, BOXF_BATOC1);
			}
			if(!btst(DioInPutBuf,BATOC2_Chk))
			{ // 20160218_Â÷·® ·ÎÁ÷ ¼³°è º¯°æÀ¸·Î °´Â÷¶û µ¿ÀÏÇÏ°Ô ¼öÁ¤_BATOC2 ³ë¸ÖÀÏ ¶§ High
				BATOC2Ack = NO;
			}
			else
			{
				BATOC2Ack = YES;
				HighFaultAck = 1;
				HighFaultMJF = 1;
				sbit(FBXF, BOXF_BATOC2);
			}
		}
	}
	else
	{//===dipsw.3ÀÌ H¸é --->±âÁ¸ µ¿Â÷¿Í °°ÀÌ
		if(WGBC_TC0PC1 == WGBC_TC0)
		{	// °´Â÷¿ë ÃàÀüÁö ÃæÀü±â
			if(btst(DioInPutBuf,BATOC1_Chk))
			{ // 20151117_BATOC1 ³ë¸ÖÀÏ ¶§ High·Î ¼öÁ¤ - °´Â÷
				BATOC1Ack = NO; 
			}
			else
			{
				BATOC1Ack = YES; 
				HighFaultAck = 1;
				HighFaultMJF = 1;
				sbit(FBXF, BOXF_BATOC1);
			}
			BATOC2Ack = NO;
			cbit(FBXF, BOXF_BATOC2);		
		}
		else{	// µ¿·ÂÂ÷¿ë ÃàÀüÁö ÃæÀü±â
			if(btst(DioInPutBuf,BATOC1_Chk)){ // 20160218_Â÷·® ·ÎÁ÷ ¼³°è º¯°æÀ¸·Î °´Â÷¶û µ¿ÀÏÇÏ°Ô ¼öÁ¤_BATOC1 ³ë¸ÖÀÏ ¶§ High
				BATOC1Ack = NO;
			}
			else{
				BATOC1Ack = YES;
				HighFaultAck = 1;
				HighFaultMJF = 1;
				sbit(FBXF, BOXF_BATOC1);
			}
			if(btst(DioInPutBuf,BATOC2_Chk)){ // 20160218_Â÷·® ·ÎÁ÷ ¼³°è º¯°æÀ¸·Î °´Â÷¶û µ¿ÀÏÇÏ°Ô ¼öÁ¤_BATOC2 ³ë¸ÖÀÏ ¶§ High
				BATOC2Ack = NO;
			}
			else{
				BATOC2Ack = YES;
				HighFaultAck = 1;
				HighFaultMJF = 1;
				sbit(FBXF, BOXF_BATOC2);
			}
		}		
	}	
//=================================


				
	ModuleFault_NUM_temp = 0;

	if(WGBC_TC0PC1 == WGBC_TC0){	// °´Â÷¿ë ÃàÀüÁö ÃæÀü±â
/*
		if(!btst(DioInPutBuf,M1F_Chk)){//bit4
			M1FAck = NO;
			sbit(BCBox_Status, 2);	// M1OK ¸ğµâ1 Á¤»ó
			 MFAck_MY = NO;//20151112---
		}
		else{
			M1FAck = YES;
			cbit(BCBox_Status, 2);	// M1NG ¸ğµâ1 °íÀå
			ModuleFault_NUM_temp++;
			if(WGBC_M_ID == WGBC_M_ID1) MFAck_MY = YES;
		}
*/		
		if(!btst(DioInPutBuf,M2F_Chk)){
			M2FAck = NO;
			sbit(BCBox_Status, 3);	// M2OK ¸ğµâ2 Á¤»ó
			 MFAck_MY = NO;//20151112---
		}
		else{
			M2FAck = YES;
			cbit(BCBox_Status, 3);	// M2NG ¸ğµâ2 °íÀå
			ModuleFault_NUM_temp++;
			if(WGBC_M_ID == WGBC_M_ID2) MFAck_MY = YES;
		}
		if(!btst(DioInPutBuf,M3F_Chk)){
			M3FAck = NO;
			sbit(BCBox_Status, 4);	// M3OK ¸ğµâ3 Á¤»ó
			 MFAck_MY = NO;//20151112---
		}
		else{
			M3FAck = YES;
			cbit(BCBox_Status, 4);	// M3NG ¸ğµâ3 °íÀå
			ModuleFault_NUM_temp++;
			if(WGBC_M_ID == WGBC_M_ID3) MFAck_MY = YES;
		}
		if(!btst(DioInPutBuf,M5F_Chk)){
			M5FAck = NO;
			sbit(BCBox_Status, 6);	// M5OK ¸ğµâ5 Á¤»ó
			 MFAck_MY = NO;//20151112---
		}
		else{
			M5FAck = YES;
			cbit(BCBox_Status, 6);	// M5NG ¸ğµâ5 °íÀå
			ModuleFault_NUM_temp++;
			if(WGBC_M_ID == WGBC_M_ID5) MFAck_MY = YES;
		}
		if(!btst(DioInPutBuf,M4F_Chk)){
			M4FAck = NO;
			sbit(BCBox_Status, 5);	// M4OK ¸ğµâ4 Á¤»ó
			 MFAck_MY = NO;//20151112---
		}
		else{
			M4FAck = YES;
			cbit(BCBox_Status, 5);	// M4NG ¸ğµâ4 °íÀå
			ModuleFault_NUM_temp++;
			if(WGBC_M_ID == WGBC_M_ID4) MFAck_MY = YES;
		}
		/*
		if(!btst(DioInPutBuf,M5F_Chk)){
			M5FAck = NO;
			sbit(BCBox_Status, 6);	// M5OK ¸ğµâ5 Á¤»ó
			 MFAck_MY = NO;//20151112---
		}
		else{
			M5FAck = YES;
			cbit(BCBox_Status, 6);	// M5NG ¸ğµâ5 °íÀå
			ModuleFault_NUM_temp++;
			if(WGBC_M_ID == WGBC_M_ID5) MFAck_MY = YES;
		}
		*/
		if(!btst(DioInPutBuf,M1F_Chk)){//bit4
			M1FAck = NO;
			sbit(BCBox_Status, 2);	// M1OK ¸ğµâ1 Á¤»ó
			 MFAck_MY = NO;//20151112---
		}
		else{
			M1FAck = YES;
			cbit(BCBox_Status, 2);	// M1NG ¸ğµâ1 °íÀå
			ModuleFault_NUM_temp++;
			if(WGBC_M_ID == WGBC_M_ID1) MFAck_MY = YES;
		}		
		
		ModuleFault_NUM = ModuleFault_NUM_temp;	

		/*---------*/
	//	if(MFAck_MY == NO){	// 20160811_¸ğ´ÏÅÍ¸µ ÇÁ·Î±×·¥ »ó¿¡¼­ BCMF Ã³¸® ¿À·ù ¼öÁ¤
			if(ModuleFault_NUM >= 2){	// ³»ÆúÆ® ¾Æ´Ï°í ÆúÆ®°¡ 2°³ÀÌ»óÀÌ¸é --> ³»²¨Æ÷ÇÔ ÀüÃ¼¸ğµâ Ãâ·ÂÂ÷´Ü	
				ModuleFault2ea_flag = YES;				
				PWM_FLAG = NO;
				PWM_OFF();					
			}
			else{						// ³»ÆúÆ® ¾Æ´Ï°í ÆúÆ®°¡ 2°³¹Ì¸¸ÀÌ¸é	--> ³»²¨ Àç±âµ¿ °¡´ÉÇÏ°Ô...
				ModuleFault2ea_flag = NO; 	
			}
	//	}
/*		else{	// ³»¸ğµâ ÆúÆ®¸é 
			if(ModuleFault_NUM >= 3){	// ³»ÆúÆ® Æ÷ÇÔ ÆúÆ®°¡ 3°³ ÀÌ»óÀÌ¸é --> ³»²¨ Àç±âµ¿ ±İÁö
				ModuleFault2ea_flag = YES;
				PWM_FLAG = NO;
				PWM_OFF();					
			}
			else{						// ³»ÆúÆ®ÀÌ°í ÆúÆ®°¡ 3°³¹Ì¸¸ÀÌ¸é	--> ³»²¨ Àç±âµ¿ ÇØ¾ßÇÔ  
				ModuleFault2ea_flag = NO;  	
			}
		}	// 20160811_¸ğ´ÏÅÍ¸µ ÇÁ·Î±×·¥ »ó¿¡¼­ BCMF Ã³¸® ¿À·ù ¼öÁ¤ */
		/*-----------*/
		//===20160323===bcstatus¼öÁ¤, BCOK(1), BCMJF(1)	
		if(ModuleFault2ea_flag == YES){
			cbit(BCBox_Status, 0);	// BCOK°¡ ¾Æ´Ô(0)		
			sbit(BCBox_Status, 1);	// BCMF(1)		
		}
		else{
			sbit(BCBox_Status, 0);	// BCOKÀÓ (1)		
			cbit(BCBox_Status, 1);	// BCMF°¡ ¾Æ´Ô(0)			
		}
		//===20160323===bcstatus¼öÁ¤, BCOK(1), BCMJF(1)	
	
	}
	else{		//µ¿·ÂÂ÷ ÃàÀüÁö ÃæÀü±â
		if(!btst(DioInPutBuf,M1F_Chk)){
			M1FAck = NO;
			sbit(BCBox_Status, 2);	// M1OK ¸ğµâ1 Á¤»ó
			 MFAck_MY = NO;//20151112---
		}
		else{
			M1FAck = YES;
			cbit(BCBox_Status, 2);	// M1NG ¸ğµâ1 °íÀå
			ModuleFault_NUM_temp++;
//			if(WGBC_M_ID == WGBC_M_ID1) MFAck_MY = YES; // 20160811_¸ğ´ÏÅÍ¸µ ÇÁ·Î±×·¥ »ó¿¡¼­ BCMF Ã³¸® ¿À·ù ¼öÁ¤
		}
		if(!btst(DioInPutBuf,M2F_Chk)){
			M2FAck = NO;
			sbit(BCBox_Status, 3);	// M2OK ¸ğµâ2 Á¤»ó
			 MFAck_MY = NO;//20151112---
		}
		else{
			M2FAck = YES;
			cbit(BCBox_Status, 3);	// M2NG ¸ğµâ2 °íÀå
			ModuleFault_NUM_temp++;
//			if(WGBC_M_ID == WGBC_M_ID2) MFAck_MY = YES; // 20160811_¸ğ´ÏÅÍ¸µ ÇÁ·Î±×·¥ »ó¿¡¼­ BCMF Ã³¸® ¿À·ù ¼öÁ¤
		}
		if(!btst(DioInPutBuf,M3F_Chk)){
			M3FAck = NO;
			sbit(BCBox_Status, 4);	// M3OK ¸ğµâ3 Á¤»ó
			 MFAck_MY = NO;//20151112---
		}
		else{
			M3FAck = YES;
			cbit(BCBox_Status, 4);	// M3NG ¸ğµâ3 °íÀå
			ModuleFault_NUM_temp++;
//			if(WGBC_M_ID == WGBC_M_ID3) MFAck_MY = YES; // 20160811_¸ğ´ÏÅÍ¸µ ÇÁ·Î±×·¥ »ó¿¡¼­ BCMF Ã³¸® ¿À·ù ¼öÁ¤
		}
				
		ModuleFault_NUM = ModuleFault_NUM_temp;
	
		if(MFAck_MY == NO){	// ³»¸ğµâ ÆúÆ®°¡ ¾Æ´Ï¸é  -->´Ù¸¥¸ğµâ ÆúÆ®
			if(ModuleFault_NUM >= 2){	// ³»ÆúÆ® ¾Æ´Ï°í ÆúÆ®°¡ 2°³ÀÌ»óÀÌ¸é --> ³»²¨Æ÷ÇÔ ÀüÃ¼¸ğµâ Ãâ·ÂÂ÷´Ü	
				ModuleFault2ea_flag = YES;				
				PWM_FLAG = NO;//20150908Ãß°¡
				PWM_OFF();	//20150908Ãß°¡
			}
			else{						// ³»ÆúÆ® ¾Æ´Ï°í ÆúÆ®°¡ 2°³¹Ì¸¸ÀÌ¸é	--> ³»²¨ Àç±âµ¿ °¡´ÉÇÏ°Ô...
				ModuleFault2ea_flag = NO; 	
			}
		}
	/*	else
	   {	// ³»¸ğµâ ÆúÆ®¸é 
	       if(ModuleFault_NUM >= 3){	// ³»ÆúÆ® Æ÷ÇÔ ÆúÆ®°¡ 3°³ ÀÌ»óÀÌ¸é --> ³»²¨ Àç±âµ¿ ±İÁö
		   ModuleFault2ea_flag = YES;				
	       PWM_FLAG = NO;//20150908Ãß°¡
		   PWM_OFF();	//20150908Ãß°¡			
	   }
			else
			{						// ³»ÆúÆ®ÀÌ°í ÆúÆ®°¡ 3°³¹Ì¸¸ÀÌ¸é	--> ³»²¨ Àç±âµ¿ ÇØ¾ßÇÔ  
				ModuleFault2ea_flag = NO;  	
			}
		}*/ // 20160811_¸ğ´ÏÅÍ¸µ ÇÁ·Î±×·¥ »ó¿¡¼­ BCMF Ã³¸® ¿À·ù ¼öÁ¤
		//===20160323===bcstatus¼öÁ¤, BCOK(1), BCMJF(1)	
		if(ModuleFault2ea_flag == YES)
		{
			cbit(BCBox_Status, 0);	// BCOK°¡ ¾Æ´Ô(0)		
			sbit(BCBox_Status, 1);	// BCMF(1)		
		}
		else
		{
			sbit(BCBox_Status, 0);	// BCOKÀÓ (1)		
			cbit(BCBox_Status, 1);	// BCMF°¡ ¾Æ´Ô(0)			
		}
		//===20160323===bcstatus¼öÁ¤, BCOK(1), BCMJF(1)	
	}
}
	

	
//=====================	
void Check_BXF_old()
{
	unsigned int ModuleFault_NUM_temp = 0;	


	//====¹èÅÍ¸® °úÀü·ù(¿ÜºÎÀÔ·Â) Ã¼Å©
	if(WGBC_TC0PC1 == WGBC_TC0)
	{	// °´Â÷¿ë ÃàÀüÁö ÃæÀü±â
		if(!btst(DioInPutBuf,BATOC1_Chk)){ // 20151117_BATOC1 ³ë¸ÖÀÏ ¶§ High·Î ¼öÁ¤ - °´Â÷
			BATOC1Ack = NO;  
			
		}
		else{
			BATOC1Ack = YES; 
			HighFaultAck = 1;
			HighFaultMJF = 1;
			sbit(FBXF, BOXF_BATOC1);
			
		}
		BATOC2Ack = NO;
		cbit(FBXF, BOXF_BATOC2);		
	}
	else{	// µ¿·ÂÂ÷¿ë ÃàÀüÁö ÃæÀü±â
		if(!btst(DioInPutBuf,BATOC1_Chk)){ // 20160218_Â÷·® ·ÎÁ÷ ¼³°è º¯°æÀ¸·Î °´Â÷¶û µ¿ÀÏÇÏ°Ô ¼öÁ¤_BATOC1 ³ë¸ÖÀÏ ¶§ High
			BATOC1Ack = NO;
		}
		else{
			BATOC1Ack = YES;
			HighFaultAck = 1;
			HighFaultMJF = 1;
			sbit(FBXF, BOXF_BATOC1);
		}
		if(!btst(DioInPutBuf,BATOC2_Chk)){ // 20160218_Â÷·® ·ÎÁ÷ ¼³°è º¯°æÀ¸·Î °´Â÷¶û µ¿ÀÏÇÏ°Ô ¼öÁ¤_BATOC2 ³ë¸ÖÀÏ ¶§ High
			BATOC2Ack = NO;
		}
		else{
			BATOC2Ack = YES;
			HighFaultAck = 1;
			HighFaultMJF = 1;
			sbit(FBXF, BOXF_BATOC2);
		}
	}	
				
	ModuleFault_NUM_temp = 0;

	if(WGBC_TC0PC1 == WGBC_TC0){	// °´Â÷¿ë ÃàÀüÁö ÃæÀü±â
/*
		if(!btst(DioInPutBuf,M1F_Chk)){//bit4
			M1FAck = NO;
			sbit(BCBox_Status, 2);	// M1OK ¸ğµâ1 Á¤»ó
			 MFAck_MY = NO;//20151112---
		}
		else{
			M1FAck = YES;
			cbit(BCBox_Status, 2);	// M1NG ¸ğµâ1 °íÀå
			ModuleFault_NUM_temp++;
			if(WGBC_M_ID == WGBC_M_ID1) MFAck_MY = YES;
		}
*/		
		if(!btst(DioInPutBuf,M2F_Chk)){
			M2FAck = NO;
			sbit(BCBox_Status, 3);	// M2OK ¸ğµâ2 Á¤»ó
			 MFAck_MY = NO;//20151112---
		}
		else{
			M2FAck = YES;
			cbit(BCBox_Status, 3);	// M2NG ¸ğµâ2 °íÀå
			ModuleFault_NUM_temp++;
			if(WGBC_M_ID == WGBC_M_ID2) MFAck_MY = YES;
		}
		if(!btst(DioInPutBuf,M3F_Chk)){
			M3FAck = NO;
			sbit(BCBox_Status, 4);	// M3OK ¸ğµâ3 Á¤»ó
			 MFAck_MY = NO;//20151112---
		}
		else{
			M3FAck = YES;
			cbit(BCBox_Status, 4);	// M3NG ¸ğµâ3 °íÀå
			ModuleFault_NUM_temp++;
			if(WGBC_M_ID == WGBC_M_ID3) MFAck_MY = YES;
		}
		if(!btst(DioInPutBuf,M5F_Chk)){
			M5FAck = NO;
			sbit(BCBox_Status, 6);	// M5OK ¸ğµâ5 Á¤»ó
			 MFAck_MY = NO;//20151112---
		}
		else{
			M5FAck = YES;
			cbit(BCBox_Status, 6);	// M5NG ¸ğµâ5 °íÀå
			ModuleFault_NUM_temp++;
			if(WGBC_M_ID == WGBC_M_ID5) MFAck_MY = YES;
		}
		if(!btst(DioInPutBuf,M4F_Chk)){
			M4FAck = NO;
			sbit(BCBox_Status, 5);	// M4OK ¸ğµâ4 Á¤»ó
			 MFAck_MY = NO;//20151112---
		}
		else{
			M4FAck = YES;
			cbit(BCBox_Status, 5);	// M4NG ¸ğµâ4 íÀå
			ModuleFault_NUM_temp++;
			if(WGBC_M_ID == WGBC_M_ID4) MFAck_MY = YES;
		}
		/*
		if(!btst(DioInPutBuf,M5F_Chk)){
			M5FAck = NO;
			sbit(BCBox_Status, 6);	// M5OK ¸ğµâ5 Á¤»ó
			 MFAck_MY = NO;//20151112---
		}
		else{
			M5FAck = YES;
			cbit(BCBox_Status, 6);	// M5NG ¸ğµâ5 °íÀå
			ModuleFault_NUM_temp++;
			if(WGBC_M_ID == WGBC_M_ID5) MFAck_MY = YES;
		}
		*/
		if(!btst(DioInPutBuf,M1F_Chk))
		{//bit4
			M1FAck = NO;
			sbit(BCBox_Status, 2);	// M1OK ¸ğµâ1 Á¤»ó
			 MFAck_MY = NO;//20151112---
		}
		else
		{
			M1FAck = YES;
			cbit(BCBox_Status, 2);	// M1NG ¸ğµâ1 °íÀå
			ModuleFault_NUM_temp++;
			if(WGBC_M_ID == WGBC_M_ID1) MFAck_MY = YES;
		}		
		

		ModuleFault_NUM = ModuleFault_NUM_temp;	

		/*---------*/
		if(MFAck_MY == NO)
		{	// ³»¸ğµâ ÆúÆ®°¡ ¾Æ´Ï¸é  -->´Ù¸¥¸ğµâ ÆúÆ®
			if(ModuleFault_NUM >= 2){	// ³»ÆúÆ® ¾Æ´Ï°í ÆúÆ®°¡ 2°³ÀÌ»óÀÌ¸é --> ³»²¨Æ÷ÇÔ ÀüÃ¼¸ğµâ Ãâ·ÂÂ÷´Ü	
				ModuleFault2ea_flag = YES;				
				PWM_FLAG = NO;
				PWM_OFF();					
			}
			else
			{						// ³»ÆúÆ® ¾Æ´Ï°í ÆúÆ®°¡ 2°³¹Ì¸¸ÀÌ¸é	--> ³»²¨ Àç±âµ¿ °¡´ÉÇÏ°Ô...
				ModuleFault2ea_flag = NO; 	
			}
		}
		else
		{	// ³»¸ğµâ ÆúÆ®¸é 
			if(ModuleFault_NUM >= 3)
			{	// ³»ÆúÆ® Æ÷ÇÔ ÆúÆ®°¡ 3°³ ÀÌ»óÀÌ¸é --> ³»²¨ Àç±âµ¿ ±İÁö
				ModuleFault2ea_flag = YES;
				PWM_FLAG = NO;
				PWM_OFF();					
			}
			else
			{						// ³»ÆúÆ®ÀÌ°í ÆúÆ®°¡ 3°³¹Ì¸¸ÀÌ¸é	--> ³»²¨ Àç±âµ¿ ÇØ¾ßÇÔ  
				ModuleFault2ea_flag = NO;  	
			}
		}	
		/*-----------*/	
	}
	else
	{		//µ¿·ÂÂ÷ ÃàÀüÁö ÃæÀü±â
		if(!btst(DioInPutBuf,M1F_Chk))
		{
			M1FAck = NO;
			sbit(BCBox_Status, 2);	// M1OK ¸ğµâ1 Á¤»ó
			 MFAck_MY = NO;//20151112---
		}
		else
		{
			M1FAck = YES;
			cbit(BCBox_Status, 2);	// M1NG ¸ğµâ1 °íÀå
			ModuleFault_NUM_temp++;
			if(WGBC_M_ID == WGBC_M_ID1) MFAck_MY = YES;
		}
		if(!btst(DioInPutBuf,M2F_Chk))
		{
			M2FAck = NO;
			sbit(BCBox_Status, 3);	// M2OK ¸ğµâ2 Á¤»ó
			 MFAck_MY = NO;//20151112---
		}
		else
		{
			M2FAck = YES;
			cbit(BCBox_Status, 3);	// M2NG ¸ğµâ2 °íÀå
			ModuleFault_NUM_temp++;
			if(WGBC_M_ID == WGBC_M_ID2) MFAck_MY = YES;
		}
		if(!btst(DioInPutBuf,M3F_Chk))
		{
			M3FAck = NO;
			sbit(BCBox_Status, 4);	// M3OK ¸ğµâ3 Á¤»ó
			 MFAck_MY = NO;//20151112---
		}
		else
		{
			M3FAck = YES;
			cbit(BCBox_Status, 4);	// M3NG ¸ğµâ3 °íÀå
			ModuleFault_NUM_temp++;
			if(WGBC_M_ID == WGBC_M_ID3) MFAck_MY = YES;
		}
				
		ModuleFault_NUM = ModuleFault_NUM_temp;
	
		if(MFAck_MY == NO)
		{	// ³»¸ğµâ ÆúÆ®°¡ ¾Æ´Ï¸é  -->´Ù¸¥¸ğµâ ÆúÆ®
			if(ModuleFault_NUM >= 2){	// ³»ÆúÆ® ¾Æ´Ï°í ÆúÆ®°¡ 2°³ÀÌ»óÀÌ¸é --> ³»²¨Æ÷ÇÔ ÀüÃ¼¸ğµâ Ãâ·ÂÂ÷´Ü	
				ModuleFault2ea_flag = YES;				
				PWM_FLAG = NO;//20150908Ãß°¡
				PWM_OFF();	//20150908Ãß°¡
			}
			else
			{						// ³»ÆúÆ® ¾Æ´Ï°í ÆúÆ®°¡ 2°³¹Ì¸¸ÀÌ¸é	--> ³»²¨ Àç±âµ¿ °¡´ÉÇÏ°Ô...
				ModuleFault2ea_flag = NO; 	
			}
		}
		else
		{	// ³»¸ğµâ ÆúÆ®¸é 
			if(ModuleFault_NUM >= 3)
			{	// ³»ÆúÆ® Æ÷ÇÔ ÆúÆ®°¡ 3°³ ÀÌ»óÀÌ¸é --> ³»²¨ Àç±âµ¿ ±İÁö
				ModuleFault2ea_flag = YES;				
				PWM_FLAG = NO;//20150908Ãß°¡
				PWM_OFF();	//20150908Ãß°¡			
			}
			else
			{						// ³»ÆúÆ®ÀÌ°í ÆúÆ®°¡ 3°³¹Ì¸¸ÀÌ¸é	--> ³»²¨ Àç±âµ¿ ÇØ¾ßÇÔ  
				ModuleFault2ea_flag = NO;  	
			}
		}
	}
}

//===============================================================//
// ÇÔ¼ö¸í : ETC_FaultProcess()														
// ±â  ´É : ¿ÂµµÆúÆ®, ÈŞÁîÆúÆ®µî Ã³¸®									
//---------------------------------------------------------------//
void Check_ETF()
{
//////////220122 suhjinho Ãâ·ÂÀúÀü·ù Ãß°¡    220325  »èÁ¦(ÀüÀÚÆÄ ½ÃÇè ´ëºñ)   220510 ÀçÃß°¡(¼º´É Àç½ÃÇè ´ëºñ)
	if( (SEQ_Step == SEQ_Normal_State) && (MODEAck == NORMALMODE1) && (BOutCurLPF < 10) && (MBatVolLPF > 80)
	&&((MCUBD_HEXSW_IN == 5)||(MCUBD_HEXSW_IN == 6)||(MCUBD_HEXSW_IN == 7)) )
	{//20170713 Ãâ·ÂÀü·ù ÀúÀü·ù ÆúÆ®
		MBatCurNC_Counter++;
		if(MBatCurNC_Counter > 4500000)			//1sec 15000
		{//5min : 4500000
			sbit(FETF,ETCF_NCARGING);
			HighFaultAck = 1;
			HighFaultMJF = 1;
		}
	}
	else
	{
		MBatCurNC_Counter = 0;
	}


	if(btst(FETF,ETCF_FUSEF)){//==ÈŞÁîÆúÆ®´Â ½ÃÄı½º¿¡¼­ ÆÇ´Ü, Áß°íÀåÃ³¸®, °­Á¦¸®¼Â ºÒ°¡´É
		HighFaultAck = 1;
		HighFaultMJF = 1;
	}
	
	if(btst(FSQF,SEQF_LKa)){	//==LKÆúÆ®´Â Áß°íÀåÃ³¸®, °­Á¦¸®¼Â ºÒ°¡´É
		HighFaultAck = 1;
		HighFaultMJF = 1;
	}
	if(btst(FSQF,SEQF_CHKa)){	//==CHKÆúÆ®´Â Áß°íÀåÃ³¸®, °­Á¦¸®¼Â ºÒ°¡´É
		HighFaultAck = 1;
		HighFaultMJF = 1;
	}

	//==°ú¿ÂÆúÆ®
	if(!TEMP85SWAck){// ¹æ¿­ÆÇ ¿Âµµ½ºÀ§Ä¡ 2°³ Á÷·Ä·Î ¿¬°áÇÏ¿© ÀÔ·Â(BÁ¢Á¡)
		THRFAck_Counter++;			
		if(THRFAck_Counter > 900000){	// 0.5sec*120 = 1min
			THRFAck_Counter = 0;
			LowFaultAck = 1;	// Low Level ÆúÆ®·Î Ã³¸® --> Á¶°Ç ÇØÁ¦½Ã ¹«ÇÑ Àç±âµ¿
			sbit(FETF, ETCF_HSKTHRF);	// ¹æ¿­ÆÇ ¿Âµµ¼¾¼­ 1,2 ÆúÆ®Ã³¸®
		}
	}
	else{
		THRFAck_Counter = 0;
	}

	
	if((FANF1Ack)&&(FanOnOffCmd_flag == 1)){
		FANF1Ack_Counter++;// ÆÒÆúÆ®1 10ÃÊ
		if(FANF1Ack_Counter > CtrTimer_10s){
			FANF1Ack_Counter = 0;
			LowFaultAck = 1;	// Low Level ÆúÆ®·Î Ã³¸® --> Á¶°Ç ÇØÁ¦½Ã ¹«ÇÑ Àç±âµ¿
			sbit(FETF, ETCF_FANF1);	// ¹æ¿­ÆÇ ¿Âµµ¼¾¼­ 1,2 ÆúÆ®Ã³¸®
		}
	}
	else{
		FANF1Ack_Counter = 0;
	}
	if((FANF2Ack)&&(FanOnOffCmd_flag == 1)){
		FANF2Ack_Counter++;// ÆÒÆúÆ®1 10ÃÊ
		if(FANF2Ack_Counter > CtrTimer_10s){
			FANF2Ack_Counter = 0;
			LowFaultAck = 1;	// Low Level ÆúÆ®·Î Ã³¸® --> Á¶°Ç ÇØÁ¦½Ã ¹«ÇÑ Àç±âµ¿
			sbit(FETF, ETCF_FANF2);	// ¹æ¿­ÆÇ ¿Âµµ¼¾¼­ 1,2 ÆúÆ®Ã³¸®
		}
	}
	else{
		FANF2Ack_Counter = 0;
	}
}



//==================================================================================//
// ÇÔ¼ö¸í : HW_FaultProcess()														//
//----------------------------------------------------------------------------------//
// DATE  : 2015. 01. 07																//
//==================================================================================//
void Check_HWF()
{
	unsigned int buf1 = 0;
	unsigned int buf2 = 0;

	//---HW FAULT
	buf1 = CS_RD_HWF1() & 0xff;// 
	buf2 = CS_RD_HWF2() & 0x1F;// 
	//buf2 = CS_RD_HWF2() & 0x0F;// 
	FHWF = ((buf2<<8)& 0xff00)| buf1;


	if(btst(FHWF,HWF0_ICOCF)){	//ÀÔ·Â°úÀü·ù--> 3ºĞ3È¸Áß°íÀå, °­Á¦¸®¼Â °¡´É
		sbit(MidFaultAck, MidMICOCF);	
	}
	if(btst(FHWF,HWF1_MBATOCF)){//¸ğµâ ÃæÀüÀü·ù °úÀü·ù--> Á¶°Ç ÇØÁ¦½Ã ¹«ÇÑ Àç±âµ¿
		LowFaultAck = 1;
	}
	if(btst(FHWF,HWF2_MOCF)){	//¸ğµâ Ãâ·ÂÀü·ù °úÀü·ù--> 3ºĞ3È¸Áß°íÀå, °­Á¦¸®¼Â °¡´É
		sbit(MidFaultAck, MidMOCOCF);	
	}
	if(btst(FHWF,HWF4_BBAT1OCF)){//BOX ÃæÀüÀü·ù1 °úÀü·ù--> Á¶°Ç ÇØÁ¦½Ã ¹«ÇÑ Àç±âµ¿
		LowFaultAck = 1;
	}
	if(btst(FHWF,HWF5_BBAT2OCF)){//BOX ÃæÀüÀü·ù2 °úÀü·ù--> Á¶°Ç ÇØÁ¦½Ã ¹«ÇÑ Àç±âµ¿
		LowFaultAck = 1;
	}
	if(btst(FHWF,HWF6_BOCF)){//BOX Ãâ·ÂÀü·ù °úÀü·ù--> Á¶°Ç ÇØÁ¦½Ã ¹«ÇÑ Àç±âµ¿
		LowFaultAck = 1;
	}
	if(btst(FHWF,HWF7_CPOVF)){//Á¦¾îÀü¾Ğ °úÀü¾Ğ--> Á¶°Ç ÇØÁ¦½Ã ¹«ÇÑ Àç±âµ¿ -----> ADC·Î ÀĞÀº°ªÀ¸·Î ÀúÀü¾Ğ/°úÀü¾Ğ µÑ´Ù ÇØ¾ßÇÔ
		LowFaultAck = 1;
	}
	if(btst(FHWF,HWF8_OVOVF)){//Ãâ·ÂÀü¾Ğ °úÀü¾Ğ(OBDÀü)--> Á¶°Ç ÇØÁ¦½Ã ¹«ÇÑ Àç±âµ¿
		LowFaultAck = 1;	
	}
	if(btst(FHWF,HWF9_BVOVF)){//Ãâ·ÂÀü¾Ğ °úÀü¾Ğ(OBDÈÄ)--> Á¶°Ç ÇØÁ¦½Ã ¹«ÇÑ Àç±âµ¿
		LowFaultAck = 1;
	}
//===20160203 ÀÔ·Â°úÀü¾Ğ HWFÀÓ½Ã·Î »ç¿ë¾ÈÇÔ
	//if(btst(FHWF,HWF10_IVOVF)){//ÀÔ·Â°úÀü¾Ğ--> Á¶°Ç ÇØÁ¦½Ã ¹«ÇÑ Àç±âµ¿
	//	LowFaultAck = 1;
	//}
	if(btst(FHWF,HWF10_IVOVF)){//ÀÔ·Â°úÀü¾Ğ--> Á¶°Ç ÇØÁ¦½Ã ¹«ÇÑ Àç±âµ¿
		FAULTLATCHCLEAR();
		cbit(FHWF,HWF10_IVOVF);	
	}
	if(btst(FHWF,HWF11_FCOVF)){//DCLINK°úÀü¾Ğ--> Á¶°Ç ÇØÁ¦½Ã ¹«ÇÑ Àç±âµ¿
		LowFaultAck = 1;
	}
}

//==================================================================================//
// ÇÔ¼ö¸í : FaultProcess() ÀÎÅÍ´Æ®·çÆ¾¿¡¼­ ¹Ù·Î Â÷´Ü ==>Å×½ºÆ®¿ë				        //
// ±â  ´É : H/W, S/W , GDU, FAULT LEVEL SETTING									    //
//----------------------------------------------------------------------------------//
// CALLING : Nths_Seq.c                        										//
// DATE  : 2010. 12. 02	, ¼öÁ¤(20101207) : etcÆúÆ® Ãß°¡							//
//==================================================================================//
void FaultProcess_int()
{	
	intpin_Chk();//Å×½ºÆ®¿ë-->Áö¿ï°Í
	
	Check_GDF();		//GDU FAULT Ã³¸®
	Check_HWF();		//HW FAULT Ã³¸®
	Check_SWF();		//¼¾¼­ SW FAULT Ã³¸®
	Check_ETF();		//¹æ¿­ÆÇ¿Âµµ,ÈŞÁî ÆúÆ®Ã³¸®
	Check_BXF();		//´Ù¸¥¸ğµâ ¹× Â÷·® BOX ÆúÆ®
	Check_TESTF();		//ÀÓÀÇ·Î ÆúÆ®¹ß»ı
	
	if((FGDF != 0)||(FHWF != 0) ||(FSWF != 0) ||(FSQF != 0) ||(FETF != 0) ||(FBXF != 0)){
		FaultAck = YES;
		//SWF_LatchGEN(); //¼ÒÇÁÆ®¿ş¾î·Î ÇÏµå¿ş¾î ÆúÆ® ¹ß»ıÇÏ¿© PWMÂ÷´Ü
	
		if(FGDF != 0){	FLGDF = FGDF;FLGDF_1ST = FGDF;}
		if(FHWF != 0){	FLHWF = FHWF;FLHWF_1ST = FHWF;}
		if(FSWF != 0){	FLSWF = FSWF;FLSWF_1ST = FSWF;}
		if(FSQF != 0){	FLSQF = FSQF;FLSQF_1ST = FSQF;}
		if(FETF != 0){	FLETF = FETF;FLETF_1ST = FETF;}
		if(FBXF != 0){	FLBXF = FBXF;FLBXF_1ST = FBXF;}

	}

//====Ãß°¡--Áß°íÀåÀÎ °æ¿ì ÀÎÅÍ·´Æ®¿¡¼­ ¹Ù·Î Â÷´Ü====
	if(FaultAck == YES){
		PWM_FLAG = NO;
		PWM_OFF();
		CHK_OFF();
		LK_OFF();
	}
	if(MajorFault == YES){	// Áß°íÀåÀÎ°æ¿ì 
		PWM_FLAG = NO;
		PWM_OFF();
		BCFAULTOUT_ON();	// ¸ğµâ ÆúÆ®¶ó°í ¸±·¹ÀÌº¸µå·Î Ãâ·Â(´Ù¸¥¸ğµâ·Î Á¤º¸°øÀ¯, Â÷·®)
		CHK_OFF();
		LK_OFF();
	}
	
	Reset_Sequence();

}

void SWF_LatchGEN()
{
	sbit(Epld_CtrlBuf,0); //active high
	CS_WR_CTRL() = Epld_CtrlBuf; 
}

void SWF_LatchCLR()
{
	cbit(Epld_CtrlBuf,0); 
	CS_WR_CTRL() = Epld_CtrlBuf; 
}
 
//==================================================================================//
// ÇÔ¼ö¸í : _FaultClear()													//
// ±â  ´É : H/W, S/W, GDU FAULT Clear											//
//----------------------------------------------------------------------------------//
// DATE  : 2015. 01. 10																//
//==================================================================================//
void FaultClear()
{	
	SWF_LatchCLR();
	SWF_LatchGEN();
	SWF_LatchCLR();	
	
	FAULTLATCHCLEAR();
	
//	BCOKOUT_OFF();

	FaultAck = NO;
	//MFAck_MY = NO;		
	MidFaultAck = 0;
	LowFaultAck = 0;
	MajorFault = 0;
	MidFaultMJF = 0;
	HighFaultMJF = 0;
	
		ModuleFault2ea_flag = NO;//20150521
	FGDF = 0x00;	
	FHWF = 0x00;
	FSWF = 0x00;
	FSQF = 0x00;
	FETF = 0x00;	
	FBXF = 0x00;	
	
	FLGDF = FGDF;
	FLHWF = FHWF;
	FLSWF = FSWF;
	FLSQF = FSQF;
	FLETF = FETF;
	FLBXF = FBXF;  

}

//==============================================================================//
// FCounter : fault È½¼ö
// FTtimer1 : Ã¹¹øÂ° ÆúÆ®ÈÄ °æ°ú½Ã°£
// FTtimer2 : µÎ¹øÂ° ÆúÆ®ÈÄ °æ°ú½Ã°£
// Fmjf : Áß°íÀå¹ß»ı(1), Áß°íÀå ¾Æ´Ô(0)
//==============================================================================//
void MidiumFault_3min3N_Check_sub(Uint16 *FCounter, Uint16 *FTimer1, Uint16 *FTimer2, Uint16 *Fmjf)
{
	unsigned int fcnt, ftmr1, ftmr2, fmjff;
	//unsigned int MIN3Temp = 6000;	
	unsigned int MIN3Temp = 18000;	
	
	fcnt = *FCounter;
	ftmr1 = *FTimer1;
	ftmr2 = *FTimer2;
	fmjff = *Fmjf;
	
	if(fcnt == 1){//ÆúÆ®°¡ 1È¸¸é
		ftmr1++;
		if(ftmr1 >= MIN3Temp){	// ÆúÆ®°¡ 1¹ø°É·È´Âµ¥,,,Ã¹¹øÂ° ÆúÆ®¹ß»ıÈÄ 3ºĞÀÌ Áö³µÀ¸¸é Áß°íÀå Ã³¸® ¾ÈÇÏ°í, ÆúÆ®´©Àû Å¬¸®¾î
			ftmr1 = 0;	//ÆúÆ®Å¸ÀÌ¸Ó Å¬¸®¾î
			fcnt = 0;	// ÆúÆ®Ä«¿îÆ® ÇÏ³ª °¨¼Ò	
		}
	}
	if(fcnt == 2){//ÆúÆ®°¡ 2È¸¸é
		ftmr1++;
		ftmr2++;		
		if(ftmr1 >= MIN3Temp){	// ÆúÆ®°¡ 2¹ø°É·È´Âµ¥,,,Ã¹¹øÂ° ÆúÆ®¹ß»ıÈÄ 3ºĞÀÌ Áö³µÀ¸¸é Áß°íÀå ¾Æ´Ï°í, ÆúÆ®´©Àû 1°¨¼Ò
			ftmr1 = ftmr2;	//
			fcnt = 1;	// ÆúÆ®Ä«¿îÆ® ÇÏ³ª °¨¼Ò		
			ftmr2 = 0;	//ÆúÆ®Å¸ÀÌ¸Ó2 Å¬¸®¾î
		}
	}		
	if(fcnt == 3){//ÆúÆ®°¡ 3È¸¸é Áß°íÀå °Ë»ç
		ftmr1++;
		ftmr2++;		
		if(ftmr1 >= MIN3Temp){	// ÆúÆ®°¡ 2¹ø°É·È´Âµ¥,,,Ã¹¹øÂ° ÆúÆ®¹ß»ıÈÄ 3ºĞÀÌ Áö³µÀ¸¸é Áß°íÀå ¾Æ´Ï°í, ÆúÆ®©Àû 1°¨¼Ò
			ftmr1 = ftmr2;	//
			fcnt = 2;	// ÆúÆ®Ä«¿îÆ® ÇÏ³ª °¨¼Ò		
			ftmr2 = 0;	//ÆúÆ®Å¸ÀÌ¸Ó2 Å¬¸®¾î
		}
		else{		//ÆúÆ® 3È¸í Ã¹¹øÂ° ÆúÆ®¹ß»ıÈÄ  3ºĞ¹Ì¸¸ÀÌ¸é Áß°íÀå Ã³¸®
			ftmr1 = 0;	//ÆúÆ®Å¸ÀÌ¸Ó Å¬¸®¾î	
			ftmr2 = 0;	//ÆúÆ®Å¸ÀÌ¸Ó Å¬¸®¾î		
			fmjff = 1;	// Áß°íÀå ¹ß»ı					
		}
	}	
	
	*FCounter = fcnt;
	*FTimer1 = ftmr1;
	*FTimer2 = ftmr2;
	*Fmjf = fmjff;
			
}

void MidiumFault_3min3N_Check()
{
	MidiumFault_3min3N_Check_sub(&MidFCounter0, &MidFTimer01, &MidFTimer02, &MidFaultMJF);
	MidiumFault_3min3N_Check_sub(&MidFCounter1, &MidFTimer11, &MidFTimer12, &MidFaultMJF);
	MidiumFault_3min3N_Check_sub(&MidFCounter2, &MidFTimer21, &MidFTimer22, &MidFaultMJF);

}
void MidiumFault_Count_INC()
{
	if(btst(MidFaultAck,MidGDF)){		// BIT 0, GDFs ÆúÆ® 3ºĞ3È¸½Ã Áß°íÀå Ã³¸®  -->¸®¼Â½Ã Àç±âµ¿ ºÒ°¡		
		MidFCounter0++;
	}
	if(btst(MidFaultAck,MidMICOCF)){	// BIT 2, ÀÔ·Â°úÀü·ù ÆúÆ® 3ºĞ3È¸ -->¸®¼Â½Ã Àç±âµ¿ °¡´É 
		MidFCounter1++;
	}
	if(btst(MidFaultAck,MidMOCOCF)){	// BIT 4, ¸ğµâ Ãâ·Â°úÀü·ù ÆúÆ® 3ºĞ3È¸ -->¸®¼Â½Ã Àç±âµ¿ °¡´É
		MidFCounter2++;
	}
}

void MidiumFault_3min3N_INIT()
{
	//==3ºĞ3È¸ µ¿ÀÏ°íÀå ÆúÆ®Ã³¸® Å×½ºÆ®º¯¼ö===
	MidFaultAck = 0;
	LowFaultAck = 0;
	HighFaultAck = 0;//

	HighFaultMJF = 0;
	MidFaultMJF = 0;

	MidFCounter0 = 0;	//GDF1,2 ¹ß»ıÈ½¼ö 
	MidFTimer01 = 0;		//GDFs Ã¹¹øÂ° °æ°ú½Ã°£  
	MidFTimer02 = 0;		//GDFs µÎ¹øÂ° °æ°ú½Ã°£  

	MidFCounter1 = 0;	//ÀÔ·Â°úÀü·ù ÆúÆ® ¹ß»ıÈ½¼ö
	MidFTimer11 = 0;		//ÀÔ·Â°úÀü·ù ÆúÆ® Ã¹¹øÂ° °æ°ú½Ã°£ 
	MidFTimer12 = 0;		//ÀÔ·Â°úÀü·ù ÆúÆ® µÎ¹øÂ° °æ°ú½Ã°£  

	MidFCounter2 = 0;	//¸ğµâÃâ·Â°úÀü·ù ÆúÆ® ¹ß»ıÈ½¼ö
	MidFTimer21 = 0;		//¸ğµâÃâ·Â°úÀü·ù ÆúÆ® Ã¹¹øÂ° °æ°ú½Ã°£  
	MidFTimer22 = 0;		//ÀÔ·Â°ú¸ğµâÃâ·Â°úÀü·ùÀü·ù ÆúÆ® µÎ¹øÂ° °æ°ú½Ã°£  

	
}
