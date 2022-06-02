//==================================================================================//
// ø¯∞≠º± µø∑¬¬˜(30kw)/∞¥¬˜(50kw) √‡¿¸¡ˆ √Ê¿¸±‚ ∏µ‚(10kw) «¡∑Œ±◊∑•	           		//
//==================================================================================//
// DIO ∫Œ∫–  															       			
//==================================================================================//

#include "Global_Variable_Function.h"
//==================================================================================//
// «‘ºˆ∏Ì : DOutputInit()								
// ±‚  ¥… : DIO √‚∑¬/led √ ±‚»≠																									
//==================================================================================//
Uint16 BCSTARTAck_Delay_Counter = 0;//startΩ≈»£ ¿ŒΩƒ¿ª 0.3√  µÙ∑π¿Ã¡‹, ¿‘∑¬∞˙¿¸æ– ∂ßπÆø°

void DOutputInit()
{
/*
var Uint16 	DioOutBuf1;
#define BC_FAULT_OUT			0	//∏µ‚ ∆˙∆ÆªÛ≈¬ √‚∑¬ ---> ∏±∑π¿Ã∫∏µÂ
#define BC_STATE_OUT			1	//∏µ‚ ªÛ≈¬ √‚∑¬ ---> ∏±∑π¿Ã∫∏µÂ
#define BC_SPARE1_OUT			2	//∏µ‚ Ω∫∆‰æÓ √‚∑¬ ---> ∏±∑π¿Ã∫∏µÂ
#define BC_STATE2_OUT			3	//∏µ‚ Ω∫∆‰æÓ √‚∑¬ ---> mcu∫∏µÂ ≥ª∫Œ ƒø≥ÿ≈Õ(j18)
#define BC_CHKON_OUT			4	//∏µ‚ CHKµø¿€√‚∑¬ ---> ¿‘∑¬« ≈Õ∫∏µÂ
#define BC_LKON_OUT				5	//∏µ‚ LKµø¿€√‚∑¬ ---> ¿‘∑¬« ≈Õ∫∏µÂ
#define BC_SPARE72V_OUT			6	//∏µ‚ Ω∫∆‰æÓ√‚∑¬ ---> ¿‘∑¬« ≈Õ∫∏µÂ
#define BC_FANON_OUT			7	//∆“µø¿€ √‚∑¬ ---> ∆“∆ƒøˆ º≠«√∂Û¿Ã
*/

	//DioOutBuf1 = 0x01;
	DioOutBuf1 = 0x02;
	//DioOutBuf1 = 0x00;

	DioLedBuf1 = 0x00;
	DioLedBuf2 = 0x00;
	DioLedBuf3 = 0x00;
	Epld_EledBuf = 0xff;// logic π›¥Î, 1¿Ã∏È 0¿∏∑Œ √‚∑¬(led4∞≥, TP4∞≥)
		
	CS_WR_DOUT1() = DioOutBuf1;// ALL DOUT OFF

	CS_WR_LED1() = (DioLedBuf1 & 0xFF);// ALL DLEDOUT OFF
	CS_WR_LED2() = (DioLedBuf2 & 0xFF);// ALL DLEDOUT OFF
	CS_WR_LED3() = (DioLedBuf3 & 0xFF);// ALL DLEDOUT OFF
	CS_WR_ELED() = (Epld_EledBuf & 0xFF);// ALL LED4∞≥ OFF, TP 0¿∏∑Œ √‚∑¬
}

//==================================================================================//
// «‘ºˆ∏Ì : _Check_DI()		            								
// ±‚  ¥… : DIN√º≈©«œ∞Ì(√§≈Õ∏µ 3»∏) ∞¸∑√ ACKΩ≈»£ ª˝º∫								
// Calling : ¡¶æÓ¿Œ≈Õ∑¥∆Æø°º≠ 																				
//==================================================================================//
void Check_DI()
{
	unsigned int buf, buf1, buf2;
	
	Uint16 Temp0_GPIO = 0;
	Uint16 Temp1_GPIO = 0;
	Uint16 Temp2_GPIO = 0;
	Uint16 Temp3_GPIO = 0;		
	
	MODEAck = GpioDataRegs.GPADAT.bit.GPIO26;//SWSEL---> MODE SWITCH∑Œ ªÁøÎ«“±Ó?(TEST MODE / NORMAL MODE)

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
	MCUBD_MODESW_IN = GpioDataRegs.GPADAT.bit.GPIO26;//SWSEL---> MODE SWITCH∑Œ ªÁøÎ«“±Ó?(TEST MODE / NORMAL MODE)
	MCUBD_PUSHBUTTONSW_IN = !GpioDataRegs.GPADAT.bit.GPIO27;//SWPB - PUSH BUTTON SW	

	//==================================================
	// DIO ¿‘∑¬, √§≈Õ∏µ πÊ¡ˆ(3»∏)
	//==================================================
	buf1 = CS_RD_DIN1()& 0xFF;
	buf2 = CS_RD_DIN2()& 0xFF;
	buf  = ((buf2<<8)& 0xFF00)| buf1;

/*------*/
	if((buf == DioInPutBuf1_1st)&&(buf == DioInPutBuf1_2nd)&&(buf == DioInPutBuf1_3rd))
	{
		DioInPutBuf = buf;
	}

	DioInPutBuf1_3rd = DioInPutBuf1_2nd;
	DioInPutBuf1_2nd = DioInPutBuf1_1st;
	DioInPutBuf1_1st = buf;
/*------*/		

//--DioInPutBuf ---- CS_RD_DIN1(), CS_RD_DIN2()
	if(btst(DioInPutBuf,EXTRESET_Chk))  RESETAck = NO;
	else                                RESETAck = YES;

//===20160203 ¿‘∑¬∞˙¿¸æ– √º≈© ∑Œ¡˜∫Ø∞Ê(µÙ∑π¿Ã¡‹)
//===1∆Ìº∫ øÓ«‡¡ﬂ ¿‘∑¬∞˙¿¸æ– ∆˙∆Æπﬂª˝, START¿¸ ¿‘∑¬¿¸æ– 940V±Ó¡ˆ ªÛΩ¬«œ∞Ì 920V±Ÿ√≥ø°º≠ STARTΩ≈»£∞° µÈæÓø»
//===±‚¡∏ : «◊ªÛ ¿‘∑¬∞˙¿¸æ– ∆˙∆Æ∞À¡ˆ
//===∫Ø∞Ê : STARTΩ≈»£∞° µÈæÓø¿∏È STARTΩ≈»£(BCSTARTAck)∏¶ 0.3√ »ƒø° ¿ŒΩƒ«œ∞Ì, BCSTARTAck∞° ¿ŒΩƒµ«∏È ¿‘∑¬∞˙¿¸æ– √º≈©
//==----->∆ÚªÛΩ√ø°¥¬ ¿‘∑¬∞˙¿¸æ– ∞À¡ˆæ»«œ¥¬ ∞Õ¿”
//====================================================================
	//if(btst(DioInPutBuf,BCSTART_Chk))	BCSTARTAck = NO;
	//else                                BCSTARTAck = YES;

/*	
	if(btst(DioInPutBuf,BCSTART_Chk)){
		BCSTARTAck = NO;
		BCSTARTAck_Delay_Counter = 0;
	}
	else{//startΩ≈»£∞° ¿‘∑¬µ«∏È
		BCSTARTAck_Delay_Counter++;
		if(BCSTARTAck_Delay_Counter>5000){//66usec*5000 = 330msec, æ‡ 0.33√  µÙ∑π¿Ã
			BCSTARTAck = YES;
			BCSTARTAck_Delay_Counter = 5000;
		}
	}
*/
//===20160224 ¿‘∑¬∞˙¿¸æ– √º≈© ∑Œ¡˜∫Ø∞Ê(ºˆº≠≤® πÆ¡¶πÆ¡¶µ«æÓ ∞˙¿¸æ– ∑Œ¡˜ √ﬂ∞°∫∏øœ)
//===ºˆº≠∆Ìº∫ øÓ«‡¡ﬂ ¿‘∑¬∞˙¿¸æ– ∆˙∆Æπﬂª˝-->∑Œ≈€ √ﬂ∞°∫∏øœø‰√ª
//===±‚¡∏ : STARTΩ≈»£∞° µÈæÓø¿∏È STARTΩ≈»£(BCSTARTAck)∏¶ 0.3√ »ƒø° ¿ŒΩƒ«œ∞Ì, BCSTARTAck∞° ¿ŒΩƒµ«∏È ¿‘∑¬∞˙¿¸æ– √º≈©
//==-------->∆ÚªÛΩ√ø°¥¬ ¿‘∑¬∞˙¿¸æ– ∞À¡ˆæ»«œ¥¬ ∞Õ¿”
//===∫Ø∞Ê : ±‚µø¡∂∞«(590V~810V)∏∏¡∑µ«∏È ±‚µøΩ√¿€ --¿‘∑¬∞˙¿¸æ–∆˙∆Æ∞À¡ˆ¥¬ æ∆¡˜ æ»«‘
//          STARTΩ≈»£∞° µÈæÓø¿∏È STARTΩ≈»£(BCSTARTAck)∏¶ 0.3√ »ƒø° ¿ŒΩƒ«œ∞Ì, BCSTARTAck∞° ¿ŒΩƒµ«∏È ±‚µøΩ√¿€
//          CHKµø¿€Ω√ƒ— µø¿€»Æ¿Œµ«∏È ¿‘∑¬∞˙¿¸æ– √º≈©
//---->µ˚∂Ûº≠ æ∆∑° STARTΩ≈»£ 0.3√  ¡ˆø¨Ω√≈≤∞Õ¿ª ±◊¥Î∑Œ ¿Ø¡ˆ
//====================================================================
	if(btst(DioInPutBuf,BCSTART_Chk)){
		BCSTARTAck = NO;
		BCSTARTAck_Delay_Counter = 0;
	}
	else{//startΩ≈»£∞° ¿‘∑¬µ«∏È
		BCSTARTAck_Delay_Counter++;
		if(BCSTARTAck_Delay_Counter>5000){//66usec*5000 = 330msec, æ‡ 0.33√  µÙ∑π¿Ã
			BCSTARTAck = YES;
			BCSTARTAck_Delay_Counter = 5000;
		}
	}
	
	if(btst(DioInPutBuf,CHK_Chk))  		CHKAck = NO;
	else                                CHKAck = YES;
	if(btst(DioInPutBuf,LK_Chk))		LKAck = NO;
	else                                LKAck = YES;
	if(btst(DioInPutBuf,BC72V_Chk))		BC72VAck = NO;
	else                                BC72VAck = YES;
		
		
	if(btst(DioInPutBuf,TEMP85SW_Chk))	TEMP85SWAck = NO;	//b¡¢¡°
	else                                TEMP85SWAck = YES;
//-----2015≥‚ 3ø˘ 25¿œ --->∆“∫∏µÂ ∫Ø∞Ê¿¸
//	if(btst(DioInPutBuf,FANF1_Chk))		FANF1Ack = YES;
//	else                                FANF1Ack = NO;
//	if(btst(DioInPutBuf,FANF2_Chk)) 	FANF2Ack = YES;
//	else                                FANF2Ack = NO;
//-----2015≥‚ 3ø˘ 25¿œ --->∆“∫∏µÂ ∫Ø∞Ê»ƒ
	if(btst(DioInPutBuf,FANF1_Chk))		FANF1Ack = NO;
	else                                FANF1Ack = YES;
	if(btst(DioInPutBuf,FANF2_Chk)) 	FANF2Ack = NO;
	else                                FANF2Ack = YES;    
}


//==================================================================================//
// «‘ºˆ∏Ì : _DOutPut()													//
// ±‚  ¥… :                      				//
// Calling : Ω√ƒ˝Ω∫∑Á∆æø°º≠(æ‡ 10msec)							//
// DATE  : 2015. 01. 19		ºˆ¡§ : 2015≥‚4ø˘9¿œ									//
//==================================================================================//
void DOutPut()
{
//--¡§ªÛ√Ê¿¸ √‚∑¬Ω≈»£ «•Ω√  -->¬˜∑Æ¿∏∑Œ ¡§ªÛ√‚∑¬¿Ã∂Û∞Ì ∫∏≥ª∞Ì mcu∫∏µÂ≥ª led(LD5) ON
//--∏µ‚ ¡§ªÛ√‚∑¬ --> ∏±∑π¿Ã∫∏µÂ --> ¬˜∑Æ¡¶æÓ

	if(MajorFault == YES)
	{
	  //ELED_LD7_ON();//20150524 LD5¥¬ √‚∑¬¡§ªÛ(ON), LD6¿∫ CVCC∏µÂ(CV ON, CC OFF), LD7¿∫ ¡ﬂ∞Ì¿ÂΩ√ ON¿∏∑Œ ∫Ø∞Ê
		MFAck_MY = NO;
	}
	
	
	if(SEQ_Step == SEQ_Normal_State)
	{
		if((MODEAck == TESTMODE0)||((MODEAck == NORMALMODE1)&&(MBatVolLPF>60)&&(MOutVolLPF>60))){
			MOUTV_OKAck = 1;	//√‚∑¬¿¸æ– ¡§ªÛ
			ELED_LD5_ON();
			BCSTATUSAck = 1;	//√‚∑¬¿¸æ– 70v¿Ã«œ
		  cbit(DioOutBuf1,BC_STATE_OUT);	
		}
		else
		{
			MOUTV_OKAck	= 0;
			ELED_LD5_OFF();		
			BCSTATUSAck = 0;	//√‚∑¬¿¸æ– ¡§ªÛ
			sbit(DioOutBuf1,BC_STATE_OUT);//Active low	
		}
	}
	else
	{
		MOUTV_OKAck	= 0;	//√‚∑¬¿¸æ– 50v¿Ã«œ
		ELED_LD5_OFF();		
		BCSTATUSAck = 0;	//√‚∑¬¿¸æ– ¡§ªÛ
		sbit(DioOutBuf1,BC_STATE_OUT);//Active low
	}	
	
//--CVCC∏Â «•Ω√(Mode_CV0CC1)
	if(Mode_CV0CC1 == Mode_CV0)		{ELED_LD6_ON();}	//CV∏µÂ∏È ON
	else							{ELED_LD6_OFF();}	//CC∏µÂ∏È OFF
	
//-----DioLedBuf2(active high¿”)-------------------------------------------------------------//
	if(BCSTARTAck == YES)		sbit(DioLedBuf2, 0);//STARTΩ≈»£¿‘∑¬ »Æ¿Œµ 
	else						cbit(DioLedBuf2, 0);//STARTΩ≈»£¿‘∑¬ »Æ¿Œæ»µ 
	if(MODEAck)					sbit(DioLedBuf2, 1);	// MODE = TESTMODE0, NORMALMODE1
	else						cbit(DioLedBuf2, 1);		
	if(!FaultAck)				sbit(DioLedBuf2, 2);	//±‚µø¡ÿ∫ÒªÛ≈¬(READY)Ω≈»£¥¬ øÏº± ∆˙∆Æ∞° æ¯¿∏∏È 
	else						cbit(DioLedBuf2, 2);		
	if(CHKAck)					sbit(DioLedBuf2, 3);	// CHKAªÛ≈¬
	else						cbit(DioLedBuf2, 3);		
	if(LKAck)					sbit(DioLedBuf2, 4);	// LKAªÛ≈¬
	else						cbit(DioLedBuf2, 4);		
	if(PWM_FLAG)				sbit(DioLedBuf2, 5);	// PWM ON
	else						cbit(DioLedBuf2, 5);		
	if(BCSTATUSAck)				sbit(DioLedBuf2, 6);	// ∏µ‚ √‚∑¬¿¸æ– ¡§ªÛ √‚∑¬
	else						cbit(DioLedBuf2, 6);		
	if(FanOnOffCmd_flag == 1)
	{	// FAN1,2 µø¿€Ω√≈∞∏È
		if((!FANF2Ack)&&(!FANF1Ack))
		{// ∆“∆˙∆Æ æ¯¿∏∏È
			sbit(DioLedBuf2, 7);
		}
		else
		{
			cbit(DioLedBuf2, 7);	
		}
	}
	else
	{
		cbit(DioLedBuf2, 7);		
	}
	
//-----Epld_EledBuf(active low¿”)-------------------------------------------------------------//
	if(btst(FLGDF,GDF1))									cbit(Epld_EledBuf, 0);			//(dcdc-Leading Leg)
	else 													sbit(Epld_EledBuf, 0);	
	if(btst(FLGDF,GDF2))									cbit(Epld_EledBuf, 1);			//(dcdc-Lagging Leg)
	else 													sbit(Epld_EledBuf, 1);	
	if(btst(FLETF,ETCF_HSKTHRF))							cbit(Epld_EledBuf, 2);			// πÊø≠∆« ø¬µµºæº≠ 1,2 ∆˙∆Æ√≥∏Æ	
	else 													sbit(Epld_EledBuf, 2);	
	if(btst(FLETF,ETCF_FANF1)||btst(FLETF,ETCF_FANF2))		cbit(Epld_EledBuf, 3);			//∆“1,2
	else 													sbit(Epld_EledBuf, 3);	
	if(btst(FLBXF,BOXF_BATOC1))								cbit(Epld_EledBuf, 4);			//¬˜∑Æ BAT1 CB TRIP
	else 													sbit(Epld_EledBuf, 4);	
	if(btst(FLBXF,BOXF_BATOC2))								cbit(Epld_EledBuf, 5);			//¬˜∑Æ BAT2 CB TRIP
	else 													sbit(Epld_EledBuf, 5);	

	if(btst(FLSQF,SEQF_CHKa))								cbit(Epld_EledBuf, 6);			//CHK∆˙∆Æ
	else 													sbit(Epld_EledBuf, 6);	
	if(btst(FLSQF,SEQF_LKa))								cbit(Epld_EledBuf, 7);			//LK∆˙∆Æ
	else 													sbit(Epld_EledBuf, 7);	
		
//-----DioLedBuf1-------------------------------------------------------------//
	if(btst(FLSWF,SWF0_ICOCF)||btst(FLHWF,HWF0_ICOCF))		sbit(DioLedBuf1, DLED1_BIT0);			//¿‘∑¬ ∞˙¿¸∑˘(SWF∂«¥¬ HWF)
	else 													cbit(DioLedBuf1, DLED1_BIT0);	
	if(btst(FLSWF,SWF1_MBATOCF)||btst(FLHWF,HWF1_MBATOCF))	sbit(DioLedBuf1, DLED1_BIT1);			//∏µ‚ √Ê¿¸∞˙¿¸∑˘(SWF∂«¥¬ HWF)
	else 													cbit(DioLedBuf1, DLED1_BIT1);	
	if(btst(FLSWF,SWF2_MOCF)||btst(FLHWF,HWF2_MOCF))		sbit(DioLedBuf1, DLED1_BIT2);			//∏µ‚ √‚∑¬∞˙¿¸∑˘(SWF∂«¥¬ HWF)
	else 													cbit(DioLedBuf1, DLED1_BIT2);	
	cbit(DioLedBuf1, DLED1_BIT3);//---->≥™¡ﬂø° « ø‰Ω√ ø¿πˆ∑ŒµÂ
	//if(MajorFault)				sbit(DioLedBuf1, DLED1_BIT3);//MajorFault »Æ¿Œµ 
	//else						cbit(DioLedBuf1, DLED1_BIT3);//MajorFault »Æ¿Œæ»µ 

	if(btst(FLSWF,SWF4_BBAT1OCF)||btst(FLHWF,HWF4_BBAT1OCF))	sbit(DioLedBuf1, DLED1_BIT4);		//BOX √Ê¿¸¿¸∑˘1 ∞˙¿¸∑˘(SWF∂«¥¬ HWF)
	else 														cbit(DioLedBuf1, DLED1_BIT4);	
	if(btst(FLSWF,SWF5_BBAT2OCF)||btst(FLHWF,HWF5_BBAT2OCF))	sbit(DioLedBuf1, DLED1_BIT5);		//BOX √Ê¿¸¿¸∑˘2 ∞˙¿¸∑˘(SWF∂«¥¬ HWF)
	else 														cbit(DioLedBuf1, DLED1_BIT5);	
	if(btst(FLSWF,SWF6_BOCF)||btst(FLHWF,HWF6_BOCF))			sbit(DioLedBuf1, DLED1_BIT6);		//BOX √‚∑¬¿¸∑˘ ∞˙¿¸∑˘ ∞˙¿¸∑˘(SWF∂«¥¬ HWF)
	else 														cbit(DioLedBuf1, DLED1_BIT6);	
	if(btst(FLSWF,SWF14_CPUVF)||btst(FLSWF,SWF7_CPOVF)||btst(FLHWF,HWF7_CPOVF))			sbit(DioLedBuf1, DLED1_BIT7);		//¡¶æÓ¿¸æ– ¿˙/∞˙¿¸æ–(SWF∂«¥¬ HWF)
	else 																				cbit(DioLedBuf1, DLED1_BIT7);	
//-----DioLedBuf3-------------------------------------------------------------//
	if(btst(FLSWF,SWF8_OVOVF)||btst(FLHWF,HWF8_OVOVF))		sbit(DioLedBuf3, DLED1_BIT0);			//√‚∑¬ ∞˙¿¸æ–-≥ª∫Œ(SWF∂«¥¬ HWF)
	else 													cbit(DioLedBuf3, DLED1_BIT0);	
	if(btst(FLSWF,SWF9_BVOVF)||btst(FLHWF,HWF9_BVOVF))		sbit(DioLedBuf3, DLED1_BIT1);			//√‚∑¬ ∞˙¿¸æ–-ø‹∫Œ(SWF∂«¥¬ HWF)
	else 													cbit(DioLedBuf3, DLED1_BIT1);	
	if(btst(FLSWF,SWF10_IVOVF)||btst(FLHWF,HWF10_IVOVF))	sbit(DioLedBuf3, DLED1_BIT2);			//¿‘∑¬ ∞˙¿¸æ–(SWF∂«¥¬ HWF)
	else 													cbit(DioLedBuf3, DLED1_BIT2);	
	if(btst(FLSWF,SWF11_FCOVF)||btst(FLHWF,HWF11_FCOVF))	sbit(DioLedBuf3, DLED1_BIT3);			//DCLINK ∞˙¿¸æ–(SWF∂«¥¬ HWF)
	else 													cbit(DioLedBuf3, DLED1_BIT3);	

	
//	if(btst(FLSWF,SWF12_OVUVF))								sbit(DioLedBuf3, DLED1_BIT4);			//√‚∑¬¿¸æ– ¿˙¿¸æ–(OBD¿¸,»ƒ ∞¯øÎ) 50V
//	else 													cbit(DioLedBuf3, DLED1_BIT4);	
	//√‚∑¬¿˙¿¸æ–∆˙∆Æ(OBD ¿¸/»ƒ)¥¬ ∫∏µÂø°º≠ LED 1∞≥∑Œ «•Ω√
	if(btst(FLSWF,SWF12_OVUVF)||btst(FLSWF,SWF15_OVUVF2))								sbit(DioLedBuf3, DLED1_BIT4);			//√‚∑¬¿¸æ– ¿˙¿¸æ–(OBD¿¸,»ƒ ∞¯øÎ) 50V
	else 													cbit(DioLedBuf3, DLED1_BIT4);	
	if(btst(FLSWF,SWF13_IVUVF))								sbit(DioLedBuf3, DLED1_BIT5);			//¿‘∑¬¿¸æ– ¿˙¿¸æ– 620V-->610V
	else 													cbit(DioLedBuf3, DLED1_BIT5);	
	
	//if(btst(FLSWF,SWF15_FCUVF))								sbit(DioLedBuf3, DLED1_BIT6);			//DCLINK¿¸æ– ¿˙¿¸æ– 620V-->610V
	//else 													cbit(DioLedBuf3, DLED1_BIT6);	
	
	if(btst(FLETF,ETCF_FUSEF))								sbit(DioLedBuf3, DLED1_BIT6);			//»ﬁ¡Ó∆˙∆Æ
	else 													cbit(DioLedBuf3, DLED1_BIT6);	
		
	if(MajorFault)				sbit(DioLedBuf3, 7);//MajorFault »Æ¿Œµ 
	else						cbit(DioLedBuf3, 7);//MajorFault »Æ¿Œæ»µ 
	
	//=======================================================	
	CS_WR_DOUT1() = DioOutBuf1;//
	//=======================================================
	CS_WR_LED1() = (DioLedBuf1 & 0xFF);
	CS_WR_LED2() = (DioLedBuf2 & 0xFF);
	CS_WR_LED3() = (DioLedBuf3 & 0xFF);
	CS_WR_ELED() = (Epld_EledBuf & 0xFF);
	//=======================================================
}

/*	
void ELED_LD8_ON()
{
	cbit(Epld_EledBuf,EELED_LED1); 
	CS_WR_ELED() = Epld_EledBuf; 	
}		
void ELED_LD8_OFF()
{
	sbit(Epld_EledBuf,EELED_LED1);
	CS_WR_ELED() = Epld_EledBuf; 	
}
void ELED_LD8_TOGGLE()
{
	tbit(Epld_EledBuf,EELED_LED1);
	CS_WR_ELED() = Epld_EledBuf; 	
}			
*/
//---------------------------------------		
void ELED_LD7_ON()
{
	cbit(Epld_CtrlBuf,EELED_LED2); 
	CS_WR_ELED() = Epld_CtrlBuf; 	
}		
void ELED_LD7_OFF()
{
	sbit(Epld_CtrlBuf,EELED_LED2);
	CS_WR_ELED() = Epld_CtrlBuf; 	
}
void ELED_LD7_TOGGLE()
{
	tbit(Epld_CtrlBuf,EELED_LED2);
	CS_WR_ELED() = Epld_CtrlBuf; 	
}	
		
//---------------------------------------		
void ELED_LD6_ON()
{
	cbit(Epld_CtrlBuf,EELED_LED3); 
	CS_WR_CTRL() = Epld_CtrlBuf; 	
}		
void ELED_LD6_OFF()
{
	sbit(Epld_CtrlBuf,EELED_LED3);
	CS_WR_CTRL() = Epld_CtrlBuf; 	
}
void ELED_LD6_TOGGLE()
{
	tbit(Epld_CtrlBuf,EELED_LED3);
	CS_WR_CTRL() = Epld_CtrlBuf; 	
}			
//---------------------------------------		

void ELED_LD5_ON()
{
	cbit(Epld_CtrlBuf,EELED_LED4); 
	CS_WR_CTRL() = Epld_EledBuf; 	
}		
void ELED_LD5_OFF()
{
	sbit(Epld_CtrlBuf,EELED_LED4);
	CS_WR_CTRL() = Epld_EledBuf; 	
}
void ELED_LD5_TOGGLE()
{
	tbit(Epld_CtrlBuf,EELED_LED4);
	CS_WR_CTRL() = Epld_EledBuf; 	
}			

//---------------------------------------		
void ELED_TP1_HI()
{
	sbit(Epld_CtrlBuf,EELED_TP1); //TSTPIN(TP1)
	CS_WR_CTRL() = Epld_CtrlBuf; 	
}		
void ELED_TP1_LOW()
{
	cbit(Epld_CtrlBuf,EELED_TP1);
	CS_WR_CTRL() = Epld_CtrlBuf; 	
}
void ELED_TP1_TOGGLE()
{
	tbit(Epld_CtrlBuf,EELED_TP1);
	CS_WR_CTRL() = Epld_CtrlBuf; 	
}			
//---------------------------------------		
void ELED_TP2_HI()
{
	sbit(Epld_CtrlBuf,EELED_TP2); //TSTPIN(TP2)
	CS_WR_CTRL() = Epld_CtrlBuf; 	
}		
void ELED_TP2_LOW()
{
	cbit(Epld_CtrlBuf,EELED_TP2);
	CS_WR_CTRL() = Epld_CtrlBuf; 	
}
void ELED_TP2_TOGGLE()
{
	tbit(Epld_CtrlBuf,EELED_TP2);
	CS_WR_CTRL() = Epld_CtrlBuf; 	
}
//---------------------------------------		
void ELED_TP3_HI()
{
	sbit(Epld_CtrlBuf,EELED_TP3); //TSTPIN(TP3)
	CS_WR_CTRL() = Epld_CtrlBuf; 	
}		
void ELED_TP3_LOW()
{
	cbit(Epld_CtrlBuf,EELED_TP3);
	CS_WR_CTRL() = Epld_CtrlBuf; 	
}
void ELED_TP3_TOGGLE()
{
	tbit(Epld_CtrlBuf,EELED_TP3);
	CS_WR_CTRL() = Epld_CtrlBuf; 	
}			
//---------------------------------------		
void ELED_TP4_HI()
{
	sbit(Epld_CtrlBuf,EELED_TP4); //TSTPIN(TP3)
	CS_WR_CTRL() = Epld_CtrlBuf; 	
}		
void ELED_TP4_LOW()
{
	cbit(Epld_CtrlBuf,EELED_TP4);
	CS_WR_CTRL() = Epld_CtrlBuf; 	
}
void ELED_TP4_TOGGLE()
{
	tbit(Epld_CtrlBuf,EELED_TP4);
	CS_WR_CTRL() = Epld_CtrlBuf; 	
}			
