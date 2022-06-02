//==================================================================================//
// ¿ø°­¼± µ¿·ÂÂ÷(30kw)/°´Â÷(50kw) ÃàÀüÁö ÃæÀü±â ¸ðµâ(10kw) ¸ð´ÏÅÍ¸µÇÁ·Î±×·¥	  		
//==================================================================================//
// nvram ºÎºÐ																		
// DATE  : 2015. 02. 10  ---can protocol º¯°æ               by PACTECH R&D LSH		
//==================================================================================//

#include "Global_Variable_Function.h"

Uint16  RTC_years;
Uint16  RTC_months;
Uint16  RTC_dates;
Uint16  RTC_days;
Uint16  RTC_hours;
Uint16  RTC_minutes;
Uint16  RTC_seconds;

unsigned int BCDtoDEC(unsigned char data);
	
unsigned char RTC_years_WR = 2015;
unsigned char RTC_months_WR = 3;
unsigned char RTC_dates_WR = 4;
unsigned char RTC_days_WR = 4;
unsigned char RTC_hours_WR = 13;
unsigned char RTC_minutes_WR = 53;
unsigned char RTC_seconds_WR = 50;
long CanA_TX_CRC1 = 0;
long CanA_TX_CRC2 = 0;
long CanA_TX_CRC3 = 0;
long CanA_TX_CRC4 = 0;
long CanA_TX_CRC5 = 0;

long CanA_TX_L1_ver = 0;//¹öÀüÅ×½ºÆ®¿ë--->Áö¿ï°Í
unsigned long idxy = 0;
unsigned int TDPy_i = 0;

unsigned int FaultCounterTotal = 0;
// ´©ÀûÆúÆ®Ä«¿îÅÍ, Àü¿ø²¨µµ nvram¿¡ ÀúÀåµÊ, ºÎÆÃ½Ã nvram¿¡¼­ ÀÐ¾î¾ßÇÔ, td clear½Ã °°ÀÌ clear
// ¹üÀ§ : 0 ~ 250
unsigned int Total_SaveDt = 10;	// Æ®·¹ÀÌ½ºµ¥ÀÌÅ¸ ÆúÆ® ÀúÀå°£°Ý(msec*10 ´ÜÀ§, 10msec00>100,1msec-->10, 500usec-->5)
unsigned int TDP_i_current = 0;//Æ®·¹ÀÌ½ºµ¥ÀÌÅ¸ ÆäÀÌÁö ÀÎµ¦½º(FaultCounterTotal·Î °è»êµÊ)
unsigned int TDP_index_test = 0;//Æ®·¹ÀÌ½ºµ¥ÀÌÅ¸ ÆäÀÌÁö ÀÎµ¦½º(FaultCounterTotal·Î °è»êµÊ)
unsigned int FaultCounterTotal_test = 0;//Æ®·¹ÀÌ½ºµ¥ÀÌÅ¸ ÆäÀÌÁö ÀÎµ¦½º(FaultCounterTotal·Î °è»êµÊ)
unsigned int TDPy_i_testL = 0;//Æ®·¹ÀÌ½ºµ¥ÀÌÅ¸ ÆäÀÌÁö ÀÎµ¦½º(FaultCounterTotal·Î °è»êµÊ)
unsigned int TDPy_i_testH = 0;//Æ®·¹ÀÌ½ºµ¥ÀÌÅ¸ ÆäÀÌÁö ÀÎµ¦½º(FaultCounterTotal·Î °è»êµÊ)
unsigned int TDPy_i_test = 0;
unsigned int TDPx_i_offset_test = 0;
int TDPy_i_offset_test = 0;
unsigned int nvrd_data_test = 0;
unsigned long idxy_test = 0;

unsigned int nvr_f_counter = 0;
unsigned int nvr_f_saveflag = 0;

unsigned int start_idx_tmp = 0;

unsigned int CAN_TEST_MODE = 0;
unsigned int CanTx_flagON = 0;

// (0)COMM(Master)¿¡¼­ SDR¼ö½Å¾Æ´Ô ----> SD TX ¾ÈµÊ, SDÀüÃ¼ ´Ù Àü¼ÛÇÏ¸é (0)À¸·Î ¸¸µé¾îÁÜ
// (1)COMM(Master)¿¡¼­ SDR¼ö½ÅÇßÀ½(CANA ¼ö½Å INT¿¡¼­ SET(1) ----> SD TX °¡´ÉÇÔ(ÀÌÇÃ·¹±× º¸°í Àü¼ÛÇÔ), SDÀüÃ¼ ´Ù Àü¼ÛÇÏ¸é (0)À¸·Î ¸¸µé¾îÁÜ
unsigned int CanA_SDR_flag = 0;	// SDR
unsigned int CanA_TDR_flag = 0;
unsigned int CanA_TD_CLEAR_flag = 0;
unsigned int CanA_RTC_UPGrade_flag = 0;
unsigned int CanA_FW_UPGrade_flag = 0;

unsigned int CanA_PC_Cmd = 0;// Command : (1)SDR, (2)TDR, (3)CLEAR, (4)RTC, (5)F/W upgrade
unsigned int CanA_PC_Udata1 = 0;// User Data1
unsigned int CanA_PC_Udata2 = 0;// User Data2
unsigned int CanA_PC_Udata3 = 0;// User Data3
unsigned int CanA_PC_Udata4 = 0;// User Data4
unsigned int CanA_PC_Udata5 = 0;// User Data5
unsigned int CanA_PC_Udata6 = 0;// User Data6

unsigned int cana_TDPy_idx = 0;
unsigned int TDPy_idx = 0;
unsigned int TDPx_idx = 0;
unsigned int tdFaultCounterTotal = 0;

void RX_CANA_From_CommBoard();
void Make_SDTD();
void TX_CANA_To_CommBoard_TDCLEAR();
void TX_CANA_To_CommBoard_RTCUPGRADE();

unsigned int WGBC_TC0PC1_tmp = 0;

unsigned int tst_tmp0 = 0;
unsigned int tst_tmp1 = 0;
unsigned int tst_tmp2 = 0;
unsigned int tst_tmp3 = 0;
unsigned int tst_tmp4 = 0;

unsigned int TDP_Index_Search(unsigned int tmpfcnt);
void cana_TD_idx_Search();

void Nvram_TDpage_read_TEST()
{
	TDP_index_test = TDP_Index_Search(FaultCounterTotal_test)-1;//´©ÀûÆúÆ®Ä«¿îÅÍ·Î Æ®·¹ÀÌ½º ÆúÆ®ÆäÀÌÁö ÀÎµ¦½º »õ·Î °è»ê
	TDPy_i_testL = PWORD(ADD_NVR_CDP0 + TDP_index_test*2 + 20)&0xff;	// ÆúÆ®½ÃÁ¡(L)
	TDPy_i_testH = PWORD(ADD_NVR_CDP0 + TDP_index_test*2 + 21)&0xff;	// ÆúÆ®½ÃÁ¡(H) 	
	TDPy_i_test = (unsigned int)(((TDPy_i_testL)|(TDPy_i_testH<<8)) + TDPy_i_offset_test);

	idxy_test = ADD_NVR_TDP0 + ((unsigned long)TDP_index_test * TDP_SIZE) + TDPy_i_test*TDPx_N;
	nvrd_data_test = PWORD(idxy_test + TDPx_i_offset_test)&0xff;


}


//======================================================================//
// nvram ÃÊ±âÈ­
//======================================================================//
void Nvram_Init()
{
	unsigned char	ch0;
	unsigned char	ch1;
	unsigned char	ch2;
	unsigned char	ch3;
	unsigned char	ch4;
	

	ch0 = PWORD(ADD_NVR_CDP0 + 0) & 0x0ff;
	ch1 = PWORD(ADD_NVR_CDP0 + 1) & 0x0ff;
	ch2 = PWORD(ADD_NVR_CDP0 + 2) & 0x0ff;
	ch3 = PWORD(ADD_NVR_CDP0 + 3) & 0x0ff;
	ch4 = PWORD(ADD_NVR_CDP0 + 4) & 0x0ff;
	if ((ch0 != 'W') || (ch1 != 'G') || (ch2 != 'B') || (ch3 != 'C') || (ch4 != (ProgramVersion&0x0ff)))
	{
		CanA_TD_CLEAR_flag = 1;
		Nvram_TDpage_clear();	// nvram Trace data all clear 0

		CanA_RTC_UPGrade_flag = 2;	//½Ã°£µ¥ÀÌÅ¸ ÃÊ±âÈ­
		RTC_Write();

		PWORD(ADD_NVR_CDP0 + 0) = 'W';
		PWORD(ADD_NVR_CDP0 + 1) = 'G';
		PWORD(ADD_NVR_CDP0 + 2) = 'B';
		PWORD(ADD_NVR_CDP0 + 3) = 'C';
		PWORD(ADD_NVR_CDP0 + 4) = (ProgramVersion&0xff);		//ÇÁ·Î±×·¥¹öÁ¯ ÃÊ±âÈ­
		FaultCounterTotal = 0;
		PWORD(ADD_NVR_CDP0 + 10) = FaultCounterTotal&0xff;	// ´©ÀûÆúÆ®°¡¿îÅÍ ÃÊ±âÈ­(0)	
 	}
 	
 	FaultCounterTotal = PWORD(ADD_NVR_CDP0 + 10)&0xff;// ´©ÀûÆúÆ®°¡¿îÅÍ ÀÐ¾î¿È
	TDP_i_current = TDP_Index_Search(FaultCounterTotal);//´©ÀûÆúÆ®Ä«¿îÅÍ·Î ÇöÀç TD ÆúÆ®ÆäÀÌÁö ÀÎµ¦½º °è»ê		
	
	tst_tmp0 = PWORD(ADD_NVR_CDP0 + 0) & 0x0ff;
	tst_tmp1 = PWORD(ADD_NVR_CDP0 + 1) & 0x0ff;
	tst_tmp2 = PWORD(ADD_NVR_CDP0 + 2) & 0x0ff;
	tst_tmp3 = PWORD(ADD_NVR_CDP0 + 3) & 0x0ff;
	tst_tmp4 = PWORD(ADD_NVR_CDP0 + 4) & 0x0ff;
}
//======================================================================//
// nvram Æ®·¹ÀÌ½ºµ¥ÀÌÅ¸(TD) CLEAR(0)
// ÃÑ 12ÆäÀÌÁö 0À¸·Î CLEAR
// ´©ÀûÆúÆ®Ä«¿îÅÍ(FaultCounterTotal) ÃÊ±âÈ­(0)
//======================================================================//
void Nvram_TDpage_clear()
{
	unsigned int i = 0;
	
	if(CanA_TD_CLEAR_flag == 1){
		for(i = 0; i < TDP_SIZE; i++){
			PWORD(ADD_NVR_TDP0 + i) = 0x00;
			PWORD(ADD_NVR_TDP1 + i) = 0x00;
			PWORD(ADD_NVR_TDP2 + i) = 0x00;
			PWORD(ADD_NVR_TDP3 + i) = 0x00;
			PWORD(ADD_NVR_TDP4 + i) = 0x00;
			PWORD(ADD_NVR_TDP5 + i) = 0x00;
			PWORD(ADD_NVR_TDP6 + i) = 0x00;
			PWORD(ADD_NVR_TDP7 + i) = 0x00;
			PWORD(ADD_NVR_TDP8 + i) = 0x00;
			PWORD(ADD_NVR_TDP9 + i) = 0x00;
			PWORD(ADD_NVR_TDP10 + i) = 0x00;
			PWORD(ADD_NVR_TDP11 + i) = 0x00;
			
			PWORD(ADD_NVR_TDP12 + i) = 0x00;
			PWORD(ADD_NVR_TDP13 + i) = 0x00;
			PWORD(ADD_NVR_TDP14 + i) = 0x00;
		}

		FaultCounterTotal = 0;
		PWORD(ADD_NVR_CDP0 + 10) = FaultCounterTotal&0xff;	// ´©ÀûÆúÆ®°¡¿îÅÍ ÃÊ±âÈ­(0)	
		TDP_i_current = TDP_Index_Search(FaultCounterTotal);//´©ÀûÆúÆ®Ä«¿îÅÍ·Î ÇöÀç TD ÆúÆ®ÆäÀÌÁö ÀÎµ¦½º °è»ê		
		FaultCounter = 0;
		for(i=0;i<30;i++)
		{
			PWORD(ADD_NVR_CDP0 + 20 + i) = 0;	// PAGE0~PAGE14 ÆúÆ®½ÃÁ¡(L)/(H) ÃÊ±âÈ­	
		}
		
		CanA_TD_CLEAR_flag = 0;
		TX_CANA_To_CommBoard_TDCLEAR();
	}
}

//==========================================================================================//
// nvram¿¡ TRACE µ¥ÀÌÅ¸ ÀúÀå(1msecÁÖ±â·Î 48¹ÙÀÌÆ® µ¥ÀÌÅ¸¸¦ 1000°³±îÁö ÀúÀå ---¸µ¹öÆÛ±¸Á¶
// 
//------TRACE µ¥ÀÌÅ¸ ÀúÀå¹æ¹ý----------------------------------------------------------------//
//1. ºÎÆÃ½Ã ´©ÀûÆúÆ®Ä«¿îÅÍ(FaultCounterTotal)¸¦ ÀÐ¾î¼­ ÇöÀçÆäÀÌÁö·Î ÂüÁ¶ÇÔ(ÇöÀçÆäÀÌÁö´Â ´©ÀûÆúÆ®Ä«¿îÆ®)
//1. ÇöÀç ÆäÀÌÁö¿¡ 1msec °£°ÝÀ¸·Î ¸µ¹öÆÛ·Î µ¥ÀÌÅ¸ ÀúÀå --> page.i(TDPy_i)++
//2. ÆúÆ®¹ß»ýÇÑ ½ÃÁ¡¿¡ ÀÎµ¦½º(TDPy_i)¸¦ ±â¾ïÇÏ°í, ÇöÀç ÆäÀÌÁö¿¡ (TD_Row_Idx  == TD_Row_N /2)±îÁö °è¼Ó ÀúÀå
//3. TDPy_i °¡ TDPy_N /2 ¸é Page0 µ¥ÀÌÅÍ¸¦ ÀüºÎ ÀÐ¾î¼­ ÇØ´ç °íÀåÆäÀÌÁö¿¡ ÀúÀå
//===========================================================================================//
void NvramSDTDWrite()
{
	unsigned int tempL, tempH;

	Make_SDTD();
	
	RTC_Read();	//½Ã°£µ¥ÀÌÅ¸ ÀÐ¾î¿È
	Nvram_TDpage_read_TEST();
	
	if((FaultAck == YES)&&(FaultAckOld == NO)){	// ÆúÆ® »ó½Â¿¡Áö¿¡¼­..
		nvr_f_saveflag = YES;	
		nvr_f_counter = 0;
		TDP_i_current = TDP_Index_Search(FaultCounterTotal);//´©ÀûÆúÆ®Ä«¿îÅÍ·Î ÇöÀç TD ÆúÆ®ÆäÀÌÁö ÀÎµ¦½º °è»ê
		PWORD(ADD_NVR_CDP0 + 20 + TDP_i_current*2) = (TDPy_i)&0xff;		// ÆúÆ®½ÃÁ¡(L) ÀúÀå
		PWORD(ADD_NVR_CDP0 + 21 + TDP_i_current*2) = (TDPy_i>>8)&0xff;	// ÆúÆ®½ÃÁ¡(H) ÀúÀå

		PWORD(ADD_NVR_TDP0 + (TDP_i_current * TDP_SIZE) + TDPy_N*TDPx_N + 0) = (RTC_years-(RTC_years/100)*100) & 0xff;	//³âµµ ÇÏÀ§ 10, 1ÀÚ¸®
		PWORD(ADD_NVR_TDP0 + (TDP_i_current * TDP_SIZE) + TDPy_N*TDPx_N + 1) = RTC_months;
		PWORD(ADD_NVR_TDP0 + (TDP_i_current * TDP_SIZE) + TDPy_N*TDPx_N + 2) = RTC_dates;
		PWORD(ADD_NVR_TDP0 + (TDP_i_current * TDP_SIZE) + TDPy_N*TDPx_N + 3) = RTC_hours;
		PWORD(ADD_NVR_TDP0 + (TDP_i_current * TDP_SIZE) + TDPy_N*TDPx_N + 4) = RTC_minutes;
		PWORD(ADD_NVR_TDP0 + (TDP_i_current * TDP_SIZE) + TDPy_N*TDPx_N + 5) = RTC_seconds;		
		PWORD(ADD_NVR_TDP0 + (TDP_i_current * TDP_SIZE) + TDPy_N*TDPx_N + 6) = (Total_SaveDt) & 0xff;	//ÆúÆ® ÀúÀå°£°Ý	-->CRC·Î º¯°æ¿¹Á¤	
		PWORD(ADD_NVR_TDP0 + (TDP_i_current * TDP_SIZE) + TDPy_N*TDPx_N + 7) = (FaultCounterTotal) & 0xff;	//ÀüÃ¼ ÆúÆ®Ä«¿îÆ®(´©Àû)

	}
	FaultAckOld = FaultAck;
	
	if(nvr_f_saveflag == YES)
	{
		if(nvr_f_counter == TDPy_N/2){//ÆúÆ®³­ÈÄ¿¡ 1000/2 = 500°³ µ¥ÀÌÅ¸ ÀúÀåÇÏ±â ÇßÀ¸¸é ÇöÀç ÆäÀÌÁö¸¦ ´ÙÀ½ ÆäÀÌÁö·Î ³Ñ±è
			nvr_f_saveflag = NO;
			nvr_f_counter = 0;
			FaultCounterTotal++;
			PWORD(ADD_NVR_CDP0 + 10) = FaultCounterTotal & 0xff;	// ´©ÀûÆúÆ®°¡¿îÅÍ +1
			TDP_i_current = TDP_Index_Search(FaultCounterTotal);//´©ÀûÆúÆ®Ä«¿îÅÍ·Î Æ®·¹ÀÌ½º ÆúÆ®ÆäÀÌÁö ÀÎµ¦½º »õ·Î °è»ê
			TDPy_i = 0;
		}
		nvr_f_counter++;	//ÆúÆ®³­ÈÄ¿¡ 500°³ µ¥ÀÌÅ¸ ´õ ÀúÀåÇÏ±â À§ÇØ
	}

	//----ÀúÀåÇÒ ¹øÁö °è»ê
	//ÀúÀåÇÒ ¹øÁö(idxy) = page0¹øÁö+(ÆäÀÌÁöÀÎµ¦½º * ÆäÀÌÁö»çÀÌÁî(0xC000)) + ÆäÀÌÁö³»(YÀÎµ¦½º*XÅ©±â(40));
	idxy = ADD_NVR_TDP0 + ((unsigned long)TDP_i_current * TDP_SIZE) + TDPy_i*TDPx_N;

	TDPy_i++;
	if(TDPy_i == TDPy_N)	TDPy_i = 0; //0~999±îÁö¸¸ ÀúÀå

	//----TRADE DATA PAGE¿¡ TRACE DATA ÀúÀå	---> ÃÑ 40°³ ¹ÙÀÌÆ®
	PWORD(idxy + 0) = BCBox_Status&0xff;	
	PWORD(idxy + 1) = BCM_Status1&0xff;
	PWORD(idxy + 2) = BCM_Status2&0xff;	
	PWORD(idxy + 3) = BCM_Status3&0xff;	
	
	PWORD(idxy + 4) = BCM_Status4&0xff;	
	PWORD(idxy + 5) = BCM_TDF1&0xff;	
	PWORD(idxy + 6) = BCM_TDF2&0xff;	
	PWORD(idxy + 7) = BCM_TDF3&0xff;	
	
	PWORD(idxy + 8) = BCM_TDF4&0xff;
		
	PWORD(idxy + 9) = (unsigned char)(MInputVolLPF/4);//a1
	PWORD(idxy + 10) = (unsigned char)(MDcLinkVolLPF/4);		
	PWORD(idxy + 11) = (unsigned char)(MOutVolLPF);		
	PWORD(idxy + 12) = (unsigned char)(MBatVolLPF);		
	
	PWORD(idxy + 13) = (unsigned char)(MInputCurLPF);//a5
	PWORD(idxy + 14) = (unsigned char)(MOutCurLPF);		
	PWORD(idxy + 15) = (unsigned char)(MBatCurLPF);		
	PWORD(idxy + 16) = (unsigned char)(BOutCurLPF/2);		


	PWORD(idxy + 17) = (unsigned char)(BLoadCurLPF/4);		
	PWORD(idxy + 18) = (unsigned char)(BBatCur1LPF);	
	PWORD(idxy + 19) = (unsigned char)(BBatCur2LPF);//a10			
	PWORD(idxy + 20) = (unsigned char)(MCtrlVolLPF);//a10	
	
	PWORD(idxy + 21) = (unsigned char)(ProgramVersion);//a10	
	PWORD(idxy + 22) = (unsigned char)(FaultCounter);	
	PWORD(idxy + 23) = (unsigned char)(FaultCounterTotal);			//a15
	PWORD(idxy + 24) = 0;	

	tempL = (FLHWF & 0xff);
	tempH = (FLHWF>>8 & 0xff);

	PWORD(idxy + 25) = tempL;//(unsigned char)();		
	PWORD(idxy + 26) = tempH;//(unsigned char)();		

	tempL = (FLSWF & 0xff);
	tempH = (FLSWF>>8 & 0xff);
	
	PWORD(idxy + 27) = tempL;//(unsigned char)();		
	PWORD(idxy + 28) = tempH;//(unsigned char)();		//a20
	PWORD(idxy + 29) = 0;//(unsigned char)();		
	PWORD(idxy + 30) = 0;//(unsigned char)();		
	PWORD(idxy + 31) = 0;//(unsigned char)();		//a23


}

void RTC_Write()
{
	unsigned int tmp1000, tmp100, tmp10, tmp1, tmp;

	if(CanA_RTC_UPGrade_flag==1){	// Â÷·®½Ã°£°ú µ¿±âÈ­
		RTC_years_WR = (2000+CanA_PC_Udata1);	// User Data1
		RTC_months_WR = CanA_PC_Udata2;	// User Data2
		RTC_dates_WR = CanA_PC_Udata3; // User Data3
		//RTC_days_WR
	
		RTC_hours_WR = CanA_PC_Udata4;	// User Data4
		RTC_minutes_WR = CanA_PC_Udata5;	// User Data5
		RTC_seconds_WR = CanA_PC_Udata6;	// User Data6
			

		PWORD(ADD_NVR_RTC_CENTURY) = 0x80;
			tmp1000 = RTC_years_WR/1000;
			tmp = RTC_years_WR - tmp1000*1000;
			tmp100 = tmp/100;
			tmp = tmp - tmp100*100;
			tmp10 = tmp/10;
			tmp1 = tmp - tmp10*10;		
		PWORD(ADD_NVR_RTC_YEAR) = ((tmp10 << 4) + tmp1);
		
			tmp10 = RTC_months_WR/10;
			tmp1 = RTC_months_WR - tmp10*10;
		PWORD(ADD_NVR_RTC_MONTH) = ((tmp10 << 4) + tmp1);
		
			tmp10 = RTC_dates_WR/10;
			tmp1 = RTC_dates_WR - tmp10*10;
		PWORD(ADD_NVR_RTC_DATE) = ((tmp10 << 4) + tmp1);//RTC_dates_WR & 0xff;
	
		PWORD(ADD_NVR_RTC_DAY) = RTC_days_WR & 0x07;
			tmp10 = RTC_hours_WR/10;
			tmp1 = RTC_hours_WR - tmp10*10;
		PWORD(ADD_NVR_RTC_HOUR) = (((tmp10 << 4) + tmp1)&0x3f);//RTC_hours_WR & 0xff;
			tmp10 = RTC_minutes_WR/10;
			tmp1 = RTC_minutes_WR - tmp10*10;
		PWORD(ADD_NVR_RTC_MIN) = (((tmp10 << 4) + tmp1)&0x7f);//RTC_minutes_WR & 0xff;
			tmp10 = RTC_seconds_WR/10;
			tmp1 = RTC_seconds_WR - tmp10*10;
		PWORD(ADD_NVR_RTC_SEC) = (((tmp10 << 4) + tmp1)&0x7f);//RTC_seconds_WR & 0xff;
		
		PWORD(ADD_NVR_RTC_CENTURY) = (((tmp1000 << 4) + tmp100) & 0x3f);//wrcentury & 0xff;
		
		PWORD(ADD_NVR_RTC_CENTURY) = 0x00;
		
		CanA_RTC_UPGrade_flag = 0;
				TX_CANA_To_CommBoard_RTCUPGRADE();
	}
	else if(CanA_RTC_UPGrade_flag==2){	// MCU¿¡ ³ëÆ®ºÏ¿¬°áÇØ¼­ CAN ¶Ç´Â ¸®¾ó½Ã½º¸ð´ÏÅÍ¸µÀ¸·Î RTCÃÊ±âÈ­
	
		PWORD(ADD_NVR_RTC_CENTURY) = 0x80;
			tmp1000 = RTC_years_WR/1000;
			tmp = RTC_years_WR - tmp1000*1000;
			tmp100 = tmp/100;
			tmp = tmp - tmp100*100;
			tmp10 = tmp/10;
			tmp1 = tmp - tmp10*10;		
		PWORD(ADD_NVR_RTC_YEAR) = ((tmp10 << 4) + tmp1);
		
			tmp10 = RTC_months_WR/10;
			tmp1 = RTC_months_WR - tmp10*10;
		PWORD(ADD_NVR_RTC_MONTH) = ((tmp10 << 4) + tmp1);
		
			tmp10 = RTC_dates_WR/10;
			tmp1 = RTC_dates_WR - tmp10*10;
		PWORD(ADD_NVR_RTC_DATE) = ((tmp10 << 4) + tmp1);//RTC_dates_WR & 0xff;
	
		PWORD(ADD_NVR_RTC_DAY) = RTC_days_WR & 0x07;
			tmp10 = RTC_hours_WR/10;
			tmp1 = RTC_hours_WR - tmp10*10;
		PWORD(ADD_NVR_RTC_HOUR) = (((tmp10 << 4) + tmp1)&0x3f);//RTC_hours_WR & 0xff;
			tmp10 = RTC_minutes_WR/10;
			tmp1 = RTC_minutes_WR - tmp10*10;
		PWORD(ADD_NVR_RTC_MIN) = (((tmp10 << 4) + tmp1)&0x7f);//RTC_minutes_WR & 0xff;
			tmp10 = RTC_seconds_WR/10;
			tmp1 = RTC_seconds_WR - tmp10*10;
		PWORD(ADD_NVR_RTC_SEC) = (((tmp10 << 4) + tmp1)&0x7f);//RTC_seconds_WR & 0xff;
		
		PWORD(ADD_NVR_RTC_CENTURY) = (((tmp1000 << 4) + tmp100) & 0x3f);//wrcentury & 0xff;
		
		PWORD(ADD_NVR_RTC_CENTURY) = 0x00;
		
		CanA_RTC_UPGrade_flag = 0;
	}
	else{
		;;
	}	
	
}

void RTC_Read()
{
	unsigned char rdyear;
	unsigned char rdmonth;
	unsigned char rddate;
	unsigned char rdday;
	unsigned char rdhour;
	unsigned char rdmin;
	unsigned char rdsec;
	unsigned char rdcentury;
	
	PWORD(ADD_NVR_RTC_CENTURY) = 0x40; // Read start RTC
	
	rdyear = PWORD(ADD_NVR_RTC_YEAR) & 0xff;
	rdmonth = PWORD(ADD_NVR_RTC_MONTH) & 0x1f;
	rddate = PWORD(ADD_NVR_RTC_DATE) & 0x3f;
	rdday = PWORD(ADD_NVR_RTC_DAY) & 0x07;
	rdhour = PWORD(ADD_NVR_RTC_HOUR) & 0x3f;
	rdmin = PWORD(ADD_NVR_RTC_MIN) & 0x7f;
	rdsec = PWORD(ADD_NVR_RTC_SEC) & 0x7f;
	rdcentury = PWORD(ADD_NVR_RTC_CENTURY) & 0x3f;

	PWORD(ADD_NVR_RTC_CENTURY) = 0x00; // Read stop RTC

	RTC_years = BCDtoDEC(rdyear) + BCDtoDEC(rdcentury)*100;
	RTC_months = BCDtoDEC(rdmonth);
	RTC_dates = BCDtoDEC(rddate);
	RTC_days = rdday;
	RTC_hours = BCDtoDEC(rdhour);
	RTC_minutes = BCDtoDEC(rdmin);
	RTC_seconds = BCDtoDEC(rdsec);

}
	
void MCUTEST_Make_SDTD_Analog()
{
//MCUBD_HEXSW_IN
	CaTX_a1 = (((unsigned int)MInputVolLPF) >> 2) & 0xff;	// ÀÔ·ÂÀü¾Ð/4
	CaTX_a2 = (((unsigned int)MDcLinkVolLPF) >> 2) & 0xff;	// ÀÔ·ÂÀü¾Ð/4
	//CaTX_a3 = (((unsigned int)MCUBD_HEXSW_IN) >> 0) & 0xff;		// ÀÔ·ÂÀü¾Ð(obdÈÄ´Ü)/1
	CaTX_a3 = 50;		// ÀÔ·ÂÀü¾Ð(obdÈÄ´Ü)/1
	CaTX_a4 = 50;		// ÀÔ·ÂÀü¾Ð(obdÈÄ´Ü)/1
	CaTX_a5 = 50;		// ÀÔ·ÂÀü¾Ð(obdÈÄ´Ü)/1
	CaTX_a6 = 50;		// ÀÔ·ÂÀü¾Ð(obdÈÄ´Ü)/1
	CaTX_a7 = 50;		// ÀÔ·ÂÀü¾Ð(obdÈÄ´Ü)/1
//	CaTX_a3 = (((unsigned int)MOutVolLPF) >> 0) & 0xff;		// ÀÔ·ÂÀü¾Ð(obdÈÄ´Ü)/1
	//CaTX_a4 = (((unsigned int)MBatVolLPF) >> 0) & 0xff;	// Ãâ·ÂÀü¾Ð1(obdÀü´Ü)/1
	
	//CaTX_a5 = (((unsigned int)MInputCurLPF) >> 0) & 0xff;	// ¸ðµâ ÀÔ·ÂÀü·ù/1
	//CaTX_a6 = (((unsigned int)MOutCurLPF) >> 0) & 0xff;		// ¸ðµâÃâ·ÂÀü·ù/1
	//CaTX_a7 = (((unsigned int)MBatCurLPF) >> 0) & 0xff;		// ¸ðµâ ÃæÀüÀü·ù/1
	CaTX_a8 = (((unsigned int)BOutCurLPF) >> 2) & 0xff;		// ¹Ú½º Ãâ·ÂÀü·ù/4

	CaTX_a9 = (((unsigned int)BLoadCurLPF) >> 2) & 0xff;// ¹Ú½º ºÎÇÏÀü·ù/4 = Ãâ·ÂÀü·ù-ÃæÀüÀü·ù(1,2)
	CaTX_a10 = (((unsigned int)BBatCur1LPF) >> 0) & 0xff;		// ¹Ú½º ¹èÅÍ¸® Àü·ù1/1
	CaTX_a11 = (((unsigned int)BBatCur2LPF) >> 0) & 0xff;	// ¹Ú½º ¹èÅÍ¸® Àü·ù2/1
	CaTX_a12 = (((unsigned int)MCtrlVolLPF) >> 0) & 0xff;	// Á¦¾îÀü¾Ð/1

	//CaTX_a13 = (((unsigned int)ModuleFault_NUM) >> 2) & 0xff;	// ÀÓ½Ã--°íÀå¸ðµâ¼ö·® ÀÎ½ÄÇÑ°Í
	CaTX_a13 = (((unsigned int)Pall_OK_ModuleN) >> 2) & 0xff;	// ÀÓ½Ã--Á¤»ó¸ðµâ¼ö·® ÀÎ½ÄÇÑ°Í
	CaTX_a14 = (((unsigned int)FaultCounter) >> 0) & 0xff;	// ÆúÆ®Ä«¿îÅÍ Ç¥½Ã
	CaTX_a15 = (((unsigned int)FaultCounterTotal) >> 0) & 0xff;	// ´©ÀûÆúÆ® Ç¥½Ã
	CaTX_a16 = (((unsigned int)((OutVolRef-70.0)*10.0)) >> 0) & 0xff;	//

	CaTX_a17 = (((unsigned int)BLoadCurLPF) >> 2) & 0xff;//
	CaTX_a18 = (((unsigned int)BBatCur1LPF) >> 0) & 0xff;	



}
void Make_SDTD_Analog()
{

	CaTX_a1 = (((unsigned int)MInputVolLPF) >> 2) & 0xff;	// ÀÔ·ÂÀü¾Ð/4
	CaTX_a2 = (((unsigned int)MDcLinkVolLPF) >> 2) & 0xff;	// ÀÔ·ÂÀü¾Ð/4
	CaTX_a3 = (((unsigned int)MOutVolLPF) >> 0) & 0xff;		// ÀÔ·ÂÀü¾Ð(obdÈÄ´Ü)/1
	CaTX_a4 = (((unsigned int)MBatVolLPF) >> 0) & 0xff;	// Ãâ·ÂÀü¾Ð1(obdÀü´Ü)/1
	
	CaTX_a5 = (((unsigned int)MInputCurLPF) >> 0) & 0xff;	// ¸ðµâ ÀÔ·ÂÀü·ù/1
	CaTX_a6 = (((unsigned int)MOutCurLPF) >> 0) & 0xff;		// ¸ðµâÃâ·ÂÀü·ù/1
	CaTX_a7 = (((unsigned int)MBatCurLPF) >> 0) & 0xff;		// ¸ðµâ ÃæÀüÀü·ù/1
	CaTX_a8 = (((unsigned int)BOutCurLPF) >> 2) & 0xff;		// ¹Ú½º Ãâ·ÂÀü·ù/4

	CaTX_a9 = (((unsigned int)BLoadCurLPF) >> 2) & 0xff;// ¹Ú½º ºÎÇÏÀü·ù/4 = Ãâ·ÂÀü·ù-ÃæÀüÀü·ù(1,2)
	CaTX_a10 = (((unsigned int)BBatCur1LPF) >> 0) & 0xff;		// ¹Ú½º ¹èÅÍ¸® Àü·ù1/1
	CaTX_a11 = (((unsigned int)BBatCur2LPF) >> 0) & 0xff;	// ¹Ú½º ¹èÅÍ¸® Àü·ù2/1
	CaTX_a12 = (((unsigned int)MCtrlVolLPF) >> 0) & 0xff;	// Á¦¾îÀü¾Ð/1

	CaTX_a13 = (((unsigned int)ProgramVersion)) & 0xff;	// ÀÓ½Ã--Á¤»ó¸ðµâ¼ö·® ÀÎ½ÄÇÑ°Í
	CaTX_a14 = (((unsigned int)FaultCounter) >> 0) & 0xff;	// ÆúÆ®Ä«¿îÅÍ Ç¥½Ã
	CaTX_a15 = (((unsigned int)FaultCounterTotal) >> 0) & 0xff;	// ´©ÀûÆúÆ® Ç¥½Ã
	CaTX_a16 = (((unsigned int)((OutVolRef-70.0)*10.0)) >> 0) & 0xff;	//

	CaTX_a17 = (((unsigned int)BLoadCurLPF) >> 2) & 0xff;//
	CaTX_a18 = (((unsigned int)BBatCur1LPF) >> 0) & 0xff;	



}

void Make_TD_Fault()
{
//=====FAULT DATA
//=====TD´Â ½Çµ¥ÀÌÅ¸¹Ý¿µ, SD´Â Àç±âµ¿ÇÏ±â Àü±îÁö ÆúÆ®Ç¥½Ã
//---DIG5----BCM_TDF1
	if(btst(FGDF,GDF1))			sbit(BCM_TDF1, 0);// GDF1 ÆúÆ®
	else						cbit(BCM_TDF1, 0);

	if(btst(FGDF,GDF2))			sbit(BCM_TDF1, 1);// GDF2 ÆúÆ®
	else						cbit(BCM_TDF1, 1);
	
	if((btst(FHWF,HWF10_IVOVF))||(btst(FSWF,SWF10_IVOVF)))		sbit(BCM_TDF1, 2);// ÀÔ·Â°úÀü¾Ð(HWF ¶Ç´Â SWF)
	else														cbit(BCM_TDF1, 2);

	//if(btst(FSWF,SWF15_FCUVF))  								sbit(BCM_TDF1, 3);//ÀÔ·ÂÀúÀü¾Ð===> FCÀúÀü¾Ð	
	//else														cbit(BCM_TDF1, 3);	
	
	if((btst(FHWF,HWF11_FCOVF))||(btst(FSWF,SWF11_FCOVF)))  		sbit(BCM_TDF1, 4);// DCLINK°úÀü¾Ð(HWF ¶Ç´Â SWF)
	else														cbit(BCM_TDF1, 4);	
	
	if((btst(FHWF,HWF8_OVOVF))||(btst(FSWF,SWF8_OVOVF)))  		sbit(BCM_TDF1, 5);// Ãâ·Â°úÀü¾Ð((OBDÈÄ))(HWF ¶Ç´Â SWF)
	else														cbit(BCM_TDF1, 5);	
	
	if((btst(FHWF,HWF8_OVOVF))||(btst(FSWF,SWF8_OVOVF)))  		sbit(BCM_TDF1, 6);// Ãâ·Â°úÀü¾Ð((OBDÈÄ))(HWF ¶Ç´Â SWF)
	else														cbit(BCM_TDF1, 6);	
	
	if((btst(FHWF,HWF0_ICOCF))||(btst(FSWF,SWF0_ICOCF)))  		sbit(BCM_TDF1, 7);// ÀÔ·Â°úÀü·ù (HWF ¶Ç´Â SWF)
	else  														cbit(BCM_TDF1, 7);
	//---DIG6----BCM_TDF2
	
	if((btst(FHWF,HWF2_MOCF))||(btst(FSWF,SWF2_MOCF)))  			sbit(BCM_TDF2, 0);// ÀÔ·Â Ãâ·Â°úÀü·ù (HWF ¶Ç´Â SWF)
	else														cbit(BCM_TDF2, 0);	
	
	if((btst(FHWF,HWF1_MBATOCF))||(btst(FSWF,SWF1_MBATOCF)))  	sbit(BCM_TDF2, 1);// ¸ðµâ ÃæÀü°úÀü·ù (HWF ¶Ç´Â SWF)
	else														cbit(BCM_TDF2, 1);	
	
	//if((btst(FHWF,HWF1_MBATOCF))||(btst(FSWF,SWF1_MBATOCF)))  	sbit(BCM_TDF2, 2);// ¸ðµâ °úºÎÇÏ½Ã
	//else														cbit(BCM_TDF2, 2);	
	cbit(BCM_TDF2, 2);		
	
	if((btst(FETF,ETCF_FANF1))||(btst(FETF,ETCF_FANF2)))  		sbit(BCM_TDF2, 3);// FAN1 ¶Ç´Â FAN2 ÆúÆ®
	else														cbit(BCM_TDF2, 3);	

	if(btst(FETF,ETCF_HSKTHRF))  								sbit(BCM_TDF2, 3);// È÷Æ®½ÌÅ© °ú¿ÂÆúÆ®½Ã ÆúÆ®½Ã
	else														cbit(BCM_TDF2, 3);	
	
	if((btst(FHWF,HWF7_CPOVF))||(btst(FSWF,SWF7_CPOVF))||(btst(FSWF,SWF14_CPUVF)))  		sbit(BCM_TDF2, 4);// Á¦¾îÀü¾Ð °úÀü¾Ð(HWF,SWF)/ÀúÀü¾Ð(SWF)
	else																				cbit(BCM_TDF2, 4);	
	
	//if((btst(FHWF,HWF7_CPOVF))||(btst(FSWF,SWF7_CPOVF))||(btst(FSWF,SWF14_CPUVF)))  	sbit(BCM_TDF2, 5);// ½ÃÄý½ºÆúÆ® ÆúÆ®--> 1°³ºñÆ®·Î Ç¥Çö?
	//else																				cbit(BCM_TDF2, 5);	
	cbit(BCM_TDF2, 5);		
	
	//if((btst(FHWF,HWF7_CPOVF))||(btst(FSWF,SWF7_CPOVF))||(btst(FSWF,SWF14_CPUVF)))  	sbit(BCM_TDF2, 6);// ÈÞÁî´Ü¼±½Ã 
	//else																				cbit(BCM_TDF2, 6);	
	//cbit(BCM_TDF2, 6);		
	if(btst(FETF,ETCF_FUSEF))  	sbit(BCM_TDF2, 6);// ÈÞÁî´Ü¼±½Ã 
	else							cbit(BCM_TDF2, 6);	

	
	if(MajorFault)  							sbit(BCM_TDF2, 7);// Áß°íÀå½Ã
	else										cbit(BCM_TDF2, 7);	
//---DIG6----BCM_TDF3
	BCM_TDF3 = 0x00;	
	BCM_TDF4 = 0x00;		
}


void Make_SD_Fault()
{
	BCM_SDF1 = ~Epld_EledBuf;
	BCM_SDF2 = DioLedBuf1;
	BCM_SDF3 = DioLedBuf3;
//	BCM_SDF4 = (DioLedBuf2<<4)&0xff;

	//BCM_SDF4 = (DioLedBuf2<<4)&0xff;//LSH 20150922 ÃÖÁ¾Ãâ·ÂÀü¾Ð ÀúÀü¾Ð(OVUV2)¸¦ ºñÆ®0À¸·Î Ç¥½Ã, ³ª¸ÓÁö »óÀ§ºñÆ®´Â ¾ÆÁ÷ »ç¿ë¾ÈÇÔ
	if(btst(FSWF,SWF12_OVUVF)){
		sbit(BCM_SDF4,0);//LSH 20151208 ÃÖÁ¾Ãâ·ÂÀü¾Ð ÀúÀü¾Ð(OVUV2)¸¦ ºñÆ®0À¸·Î Ç¥½Ã, ³ª¸ÓÁö »óÀ§ºñÆ®´Â ¾ÆÁ÷ »ç¿ë¾ÈÇÔ
	}
	else{
		cbit(BCM_SDF4,0);//LSH 20151208 ÃÖÁ¾Ãâ·ÂÀü¾Ð ÀúÀü¾Ð(OVUV2)¸¦ ºñÆ®0À¸·Î Ç¥½Ã, ³ª¸ÓÁö »óÀ§ºñÆ®´Â ¾ÆÁ÷ »ç¿ë¾ÈÇÔ
	}

	if(btst(FSWF,SWF15_OVUVF2))
	{
		sbit(BCM_SDF4,1);//LSH 20151208 ÃÖÁ¾Ãâ·ÂÀü¾Ð ÀúÀü¾Ð(OVUV2)¸¦ ºñÆ®1À¸·Î Ç¥½Ã, ³ª¸ÓÁö »óÀ§ºñÆ®´Â ¾ÆÁ÷ »ç¿ë¾ÈÇÔ
	}
	else
	{
		cbit(BCM_SDF4,1);//LSH 20151208 ÃÖÁ¾Ãâ·ÂÀü¾Ð ÀúÀü¾Ð(OVUV2)¸¦ ºñÆ®1À¸·Î Ç¥½Ã, ³ª¸ÓÁö »óÀ§ºñÆ®´Â ¾ÆÁ÷ »ç¿ë¾ÈÇÔ
	}
	



	//220124 µ¿Â÷¸ðµâ¿¡ ÀúÀü·ù ÆúÆ® Ç¥½Ã 
	if((btst(FETF,ETCF_NCARGING))&&((MCUBD_HEXSW_IN == 5)||(MCUBD_HEXSW_IN == 6)||(MCUBD_HEXSW_IN == 7)))
	{
	sbit(BCM_SDF4,2);
	}
	else
	{
		cbit(BCM_SDF4,2);
	}
	
	BCM_TDF1 = BCM_SDF1;
	BCM_TDF2 = BCM_SDF2;
	BCM_TDF3 = BCM_SDF3;
	BCM_TDF4 = BCM_SDF4;
}

void Make_SDTD_Status()
{

//---DIG1-----BOX »óÅÂÁ¤º¸1
	//if(MODEAck)		sbit(BCBox_Status, 0);	// box Á¤»ó
	//else				cbit(BCBox_Status, 0);		
	//if(MODEAck)		sbit(BCBox_Status, 1);	// box Áß°íÀå
	//else				cbit(BCBox_Status, 1);		

	//cbit(BCBox_Status, 0);// box Á¤»ó		//===20160323===bcstatus¼öÁ¤, BCOK(1), BCMJF(1)	
	//cbit(BCBox_Status, 1);// box Áß°íÀå		//===20160323===bcstatus¼öÁ¤, BCOK(1), BCMJF(1)	


//---DIG2-----MODULE »óÅÂÁ¤º¸1

		
	if(BCSTARTAck)		sbit(BCM_Status1, 0);	// Â÷·®¿¡¼­ BC START¸í·É ÀÎ½Ä
	else				cbit(BCM_Status1, 0);
	if(MODEAck)			sbit(BCM_Status1, 1);	// MODE = TESTMODE0, NORMALMODE1
	else				cbit(BCM_Status1, 1);		
	if(!FaultAck){								//±âµ¿ÁØºñ»óÅÂ(READY)½ÅÈ£´Â ¿ì¼± ÆúÆ®°¡ ¾øÀ¸¸é 
		sbit(BCM_Status1, 2);			
	}
	else{
		cbit(BCM_Status1, 2);			
	}
	if(CHKAck)			sbit(BCM_Status1, 3);	// CHKA»óÅÂ
	else				cbit(BCM_Status1, 3);		
	if(LKAck)			sbit(BCM_Status1, 4);	// LKA»óÅÂ
	else				cbit(BCM_Status1, 4);		
	if(PWM_FLAG)		sbit(BCM_Status1, 5);	// PWM ON
	else				cbit(BCM_Status1, 5);		
	if(MOUTV_OKAck)		sbit(BCM_Status1, 6);	// ¸ðµâ Ãâ·ÂÀü¾Ð Á¤»ó Ãâ·Â
	else				cbit(BCM_Status1, 6);		

	
	if(FanOnOffCmd_flag == 1)
	{	// FAN1,2 µ¿ÀÛ½ÃÅ°°í
		if((!FANF1Ack)&&(!FANF2Ack)){// ÆÒÆúÆ® ¾øÀ¸¸é
			sbit(BCM_Status1, 7);
		}
		else
		{
			cbit(BCM_Status1, 7);	
		}
	}
	else
	{
		cbit(BCM_Status1, 7);		
	}
		
	//sbit(BCM_Status1, 7);		
	//if(RESETAck)		sbit(BCM_Status1, 6);	// ¸ðµâ Ãâ·ÂÀü¾Ð Á¤»ó Ãâ·Â½Ã(1), ¾Æ´Ï¸é(0)
	//else				cbit(BCM_Status1, 6);		
//---DIG3-----MODULE »óÅÂÁ¤º¸2 ---->Åë½ÅÇÁ·ÎÅäÄÝ Ãß°¡ÇØ¾ßÇÔ(ÇöÀç DIG2±îÁö¸¸ µÇ¾îÀÖÀ½)
	if(Mode_CV0CC1 == Mode_CV0)		sbit(BCM_Status2, 0);	// CV MODE¸é 
	else							cbit(BCM_Status2, 0);	// CC MODE¸é 
	cbit(BCM_Status2, 1);	// TBD
	cbit(BCM_Status2, 2);	// TBD
	cbit(BCM_Status2, 3);	// TBD
	cbit(BCM_Status2, 4);	// TBD
	cbit(BCM_Status2, 5);	// TBD
	cbit(BCM_Status2, 6);	// TBD
	cbit(BCM_Status2, 7);	// TBD
	
//---DIG4-----MODULE »óÅÂÁ¤º¸3 ---->Åë½ÅÇÁ·ÎÅäÄÝ Ãß°¡ÇØ¾ßÇÔ(ÇöÀç DIG2±îÁö¸¸ µÇ¾îÀÖÀ½)
	BCM_Status3 = BCBox_Status;		//¹Ú½ºÁ¤º¸¸¦ 3¹øÂ°·Î Àü¼Û
	BCM_Status4 = 0x00;		


}
void Make_SDTD()
{
	unsigned int tmp1000,tmp100,tmp10,tmp1,tmp;
	
	Make_SD_Fault();		// Status Data(Fault)
	Make_SDTD_Analog();		// Analog data \
	//MCUTEST_Make_SDTD_Analog();
	Make_SDTD_Status();		// Status Data for SD,TD
	//---ÇÁ·Î±×·¥ ¹öÁ¯
	tmp1000 = ProgramVersion/1000;
	tmp = ProgramVersion - tmp1000*1000;
	tmp100 = tmp/100;
	tmp = tmp - tmp100*100;
	tmp10 = tmp/10;
	tmp1 = tmp - tmp10*10;
	tmp = (tmp1000<<12) | (tmp100<<8) | (tmp10<<4) | tmp1; //

	//---ÃæÀü±âÁ¾·ù -- PCÈ­¸é¿¡¼­ ¹Ý´ë·Î Ç¥½ÃµÇ¾î¼­--->³ªÁß¿¡ PCÈ­¸é ¼öÁ¤¿¹Á¤
	if(WGBC_TC0PC1 == WGBC_TC0){	//---PCÈ­¸é¿¡¼­ ¹Ý´ë·Î Ç¥½ÃµÇ¾î¼­--->³ªÁß¿¡ PCÈ­¸é ¼öÁ¤¿¹Á¤
		WGBC_TC0PC1_tmp = WGBC_PC1;
	}
	else{
		WGBC_TC0PC1_tmp = WGBC_TC0;
	}
	//--Àü¼Ûµ¥ÀÌÅ¸ Á¤·Ä
	CanA_TX_L1 = (((long)BCM_HartBeat & 0xffff) << 16) | tmp;                                 //Hartbeat(16)+programversion(16)
CanA_TX_L1_ver = tmp;
	CanA_TX_H1 = ((long)WGBC_M_ID<<24) | ((long)WGBC_TC0PC1_tmp<<16) | (BCBox_Status<<8) | BCM_Status1; //id(8)+bcÁ¾·ù(8)+box»óÅÂ(8)+mdstate1(8)

	CanA_TX_L2 = ((long)BCM_Status2<<24) | ((long)BCM_Status3<<16) | (BCM_Status4<<8) | BCM_SDF1;    // mdstate2(8)+mdstate3(8)+fault1(8)+fault2(8)
	CanA_TX_H2 = ((long)BCM_SDF2<<24) | ((long)BCM_SDF3<<16) | (BCM_SDF4<<8) | CaTX_a1;                // fault3(8)+fault4(8)+ad1(8)+ad2(8)

	CanA_TX_L3 = ((long)CaTX_a2<<24) | ((long)CaTX_a3<<16) | (CaTX_a4<<8) | CaTX_a5;  	  // ad3(8)+ad4(8)+ad5(8)+ad6(8)
	CanA_TX_H3 = ((long)CaTX_a6<<24) | ((long)CaTX_a7<<16) | (CaTX_a8<<8) | CaTX_a9;                // analog 4byte

	CanA_TX_L4 = ((long)CaTX_a10<<24) | ((long)CaTX_a11<<16) | (CaTX_a12<<8) | CaTX_a13;                // analog 4byte
	CanA_TX_H4 = ((long)CaTX_a14<<24) | ((long)CaTX_a15<<16) | (CaTX_a16<<8) | CaTX_a17;             // analog 4byte

	CanA_TX_L5 = ((long)CaTX_a18<<24) | ((long)CaTX_a19<<16) | (CaTX_a20<<8) | CaTX_a21;                // analog 4byte
	CanA_TX_H5 = ((long)CaTX_a23<<24) | ((long)CaTX_a24<<16) | (CaTX_a25<<8) | CaTX_a26;             // analog 4byte
	
}

//========================================================
// CANA COMM BOARD SDR¿¡ ÀÀ´ä(SD)
//========================================================
void TX_CANA_To_CommBoard_SD()
{
	MCU_D34_OFF();	//D34;		
	if(cantxidx == 0){
		SendDataToECanA(WGBC_M_ID, 0x08, CanA_TX_L1, CanA_TX_H1);
		cantxidx = 1;
	}
	else if(cantxidx == 1){
		SendDataToECanA(WGBC_M_ID, 0x08, CanA_TX_L2, CanA_TX_H2);
		cantxidx = 2;
	}
	else if(cantxidx == 2){
		SendDataToECanA(WGBC_M_ID, 0x08, CanA_TX_L3, CanA_TX_H3);
		cantxidx = 3;
	}
	else if(cantxidx == 3){
		SendDataToECanA(WGBC_M_ID, 0x08, CanA_TX_L4, CanA_TX_H4);
		cantxidx = 4;
	}
	else if(cantxidx == 4){
		//SendDataToECanA(WGBC_M_ID, 0x08, CanA_TX_L5, CanA_TX_H5);
		SendDataToECanA(WGBC_M_ID, 0x06, CanA_TX_L5, CanA_TX_H5);
		cantxidx = 0;
		CanA_SDR_flag = 0;	// ÀüÃ¼ ¼Û½Å ÇÁ·¹ÀÓ Àü¼Û ¿Ï·á
	}
	else {
		;;	
	}

}


//========================================================
// CANA COMM BOARD TRACE DATA CLEAR¿¡ ÀÀ´ä()
//========================================================
void TX_CANA_To_CommBoard_TDCLEAR()
{
	CanA_TX_L0 = 0x30000000;
	CanA_TX_H0 = 0x00000000;
	SendDataToECanA(WGBC_M_ID, 0x08, CanA_TX_L0, CanA_TX_H0);
}
//========================================================
// CANA COMM BOARD RTC UPDATE¿¡ ÀÀ´ä()
//========================================================
void TX_CANA_To_CommBoard_RTCUPGRADE()
{
	CanA_TX_L0 = 0x40000000;
	CanA_TX_H0 = 0x00000000;
	SendDataToECanA(WGBC_M_ID, 0x08, CanA_TX_L0, CanA_TX_H0);
}


void CanA_PC_Command_chk()
{
	CanA_PC_Cmd = (WGcana_rx_low_data >> 24)&0xff;	// Command : (1)SDR, (2)TDR, (3)CLEAR, (4)RTC, (5)F/W upgrade
	CanA_PC_Udata1 = (WGcana_rx_low_data >> 16)&0xff;	// User Data1
	CanA_PC_Udata2 = (WGcana_rx_low_data >> 8)&0xff;	// User Data2
	CanA_PC_Udata3 = (WGcana_rx_low_data >> 0)&0xff; 	// User Data3
	
	CanA_PC_Udata4 = (WGcana_rx_high_data >> 24)&0xff;	// User Data4
	CanA_PC_Udata5 = (WGcana_rx_high_data >> 16)&0xff;	// User Data5
	CanA_PC_Udata6 = (WGcana_rx_high_data >> 8)&0xff;	// User Data6
	
	if(CanA_PC_Cmd == 1)
	{
		CanA_SDR_flag = 1;	// COMMº¸µå·ÎºÎÅÍ CANA SDR ¹Þ¾ÒÀ½
	}
	else if(CanA_PC_Cmd == 2)
	{
		CanA_TDR_flag = 1;	// COMMº¸µå·ÎºÎÅÍ CANA TDR ¹Þ¾ÒÀ½
		cana_TDPy_idx = 0;
		TDPx_idx = 0;
	}
	else if(CanA_PC_Cmd == 3)
	{
		CanA_TD_CLEAR_flag = 1;	// COMMº¸µå·ÎºÎÅÍ CANA CLEAR ¹Þ¾ÒÀ½
		//CanA_PC_Udata1;
	}
	else if(CanA_PC_Cmd == 4)
	{
		CanA_RTC_UPGrade_flag = 1;	// COMMº¸µå·ÎºÎÅÍ CANA RTC DATA ¹Þ¾ÒÀ½
	}
	else if(CanA_PC_Cmd == 10)
	{	//---ÇÁ·Î±×·¥ ¾÷±×·¹ÀÌµå ±â´ÉÀº ³ªÁß¿¡ ±¸Çö..
		CanA_FW_UPGrade_flag = 1;	// COMMº¸µå·ÎºÎÅÍ CANA ÇÁ·Î±×·¥ ¾÷±×·¹ÀÌµå ¹Þ¾ÒÀ½
	}
}


void RX_CANA_From_CommBoard()
{
	MCU_D34_ON();	//D34;
//MCU_D34_TOGGLE();	//D34;	
	CANR_HB = WGcana_rx_low_data >> 16;
	CANR_ID = (WGcana_rx_low_data >> 8)&0xff;
	CANR_BCN = (WGcana_rx_low_data)&0xff;
	

//	if(WGcana_rx_id == 1){	// ³»²¨¸¸ ¹ÞÀ½
	if(WGcana_rx_id == WGBC_M_ID)
	{	// ³»²¨¸¸ ¹ÞÀ½
		CanA_RX_L1 = WGcana_rx_low_data;
		CanA_RX_H1 = WGcana_rx_high_data;

		CanA_PC_Command_chk();		
	}
}


//==========================================================================//
// ±â  ´É : Trace data can Àü¼Û(pc<-->Commº¸µå<-->mcu)								
// ÀÔ·Â : º¸³¾ ÆúÆ®Ä«¿îÅÍ(tdFaultCounterTotal)
// ³»ºÎ°è»ê : tdpage(Àü¼ÛÇÒ ÆúÆ®Ä«¿îÅÍ·ÎºÎÅÍ °è»êÇÑ nvram¿¡ ÀúÀåµÈ ÆúÆ®ÆäÀÌÁö
//            tdpage_fidx(ÆúÆ®ÆäÀÌÁöÀÇ ÆúÆ®¹ß»ý ½ÃÁ¡)
//            TDPy_idx(ÀúÀåµÈ td¸¦ ÆúÆ®¹ß»ý½ÃÁ¡À» Áß°£À¸·Î È¯»êÇÏ¿© Á¤·ÄÇØ¼­ Àü¼Û)
// Ãâ·Â : 
// ÃÑ Àü¼Û µ¥ÀÌÅ¸ : 1000 * 4 * 8 = 32000¹ÙÀÌÆ®
// Åë½ÅÀü¼Û È½¼ö = ÃÑ Àü¼Ûµ¥ÀÌÅ¸ / 8¹ÙÀÌÆ® = 32000¹ÙÀÌÆ®/8¹ÙÀÌÆ® = 4000¹ø
// Ãß°¡·Î ½Ã°£µ¥ÀÌÅ¸ 1¹ø, CRC 1¹øÇØ¼­ ÃÑ 4002¹ø
//==========================================================================//
void TX_CANA_To_CommBoard_TD_OLD()
{
	unsigned int tdpage = 0;

	unsigned int tdpage_fidxL = 1;
	unsigned int tdpage_fidxH = 1;
	unsigned int tdpage_fidx = 1;
	
	tdFaultCounterTotal = CanA_PC_Udata1;	// TD ¿äÃ»ÆäÀÌÁö
	
	tdpage = TDP_Index_Search(tdFaultCounterTotal)-1;//´©ÀûÆúÆ®Ä«¿îÅÍ·Î Æ®·¹ÀÌ½º ÆúÆ®ÆäÀÌÁö ÀÎµ¦½º »õ·Î °è»ê
	tdpage_fidxL = PWORD(ADD_NVR_CDP0 + 20 + tdpage*2)&0xff;		// ÆúÆ®¹ß»ý½ÃÁ¡(L)
	tdpage_fidxH = (PWORD(ADD_NVR_CDP0 + 21 + tdpage*2)<<8)&0xff;	// ÆúÆ®¹ß»ý½ÃÁ¡(H)
	tdpage_fidx = (tdpage_fidxH<<8)|tdpage_fidxL;					// ÆúÆ®¹ß»ý½ÃÁ¡(HL)

	if(cana_TDPy_idx < TDPy_N){//ÃÑ 1000*4*8 =  320000¹ÙÀÌÆ®
		if((cana_TDPy_idx == 0)&&(TDPx_idx == 0))
		{
			CanA_TX_CRC = 0;
		}
		//===ÆúÆ®½ÃÁ¡À» Áß°£À¸·Î Á¤·ÄÇÏ¿© Àü¼Û
		if(cana_TDPy_idx < (TDPy_N/2 - tdpage_fidx - 1))
		{
			TDPy_idx = (TDPy_N/2 + tdpage_fidx + 1) + cana_TDPy_idx;	
		}
		else
		{
			TDPy_idx = (((TDPy_N/2 + tdpage_fidx + 1) + cana_TDPy_idx)-TDPy_N)%TDPy_N;			
		}

		CaTX_a1 = PWORD(ADD_NVR_TDP0 + (long)tdpage*TDP_SIZE + TDPy_idx*TDPx_N + TDPx_idx*8 + 0)&0xff;//TDPx_N=32
		CaTX_a2 = PWORD(ADD_NVR_TDP0 + (long)tdpage*TDP_SIZE + TDPy_idx*TDPx_N + TDPx_idx*8 + 1)&0xff;
		CaTX_a3 = PWORD(ADD_NVR_TDP0 + (long)tdpage*TDP_SIZE + TDPy_idx*TDPx_N + TDPx_idx*8 + 2)&0xff;
		CaTX_a4 = PWORD(ADD_NVR_TDP0 + (long)tdpage*TDP_SIZE + TDPy_idx*TDPx_N + TDPx_idx*8 + 3)&0xff;
		
		CaTX_a5 = PWORD(ADD_NVR_TDP0 + (long)tdpage*TDP_SIZE + TDPy_idx*TDPx_N + TDPx_idx*8 + 4)&0xff;
		CaTX_a6 = PWORD(ADD_NVR_TDP0 + (long)tdpage*TDP_SIZE + TDPy_idx*TDPx_N + TDPx_idx*8 + 5)&0xff;
		CaTX_a7 = PWORD(ADD_NVR_TDP0 + (long)tdpage*TDP_SIZE + TDPy_idx*TDPx_N + TDPx_idx*8 + 6)&0xff;
		CaTX_a8 = PWORD(ADD_NVR_TDP0 + (long)tdpage*TDP_SIZE + TDPy_idx*TDPx_N + TDPx_idx*8 + 7)&0xff;
		CanA_TX_L0 = ((long)CaTX_a1<<24) | ((long)CaTX_a2<<16) | (CaTX_a3<<8) | CaTX_a4;
		CanA_TX_H0 = ((long)CaTX_a5<<24) | ((long)CaTX_a6<<16) | (CaTX_a7<<8) | CaTX_a8;
		CanA_TX_CRC ^= CanA_TX_L0;
		CanA_TX_CRC ^= CanA_TX_H0;
		SendDataToECanA(WGBC_M_ID, 0x08, CanA_TX_L0, CanA_TX_H0);		
			
		if((cana_TDPy_idx == 0)&&(TDPx_idx == 0))
		{
			CanA_TX_CRC1 = CanA_TX_H0;
			CanA_TX_CRC4 = TDPy_idx;
	
		}
		if((cana_TDPy_idx == 499)&&(TDPx_idx == 0))
		{
			CanA_TX_CRC2 = CanA_TX_H0;	
		}		
		if((cana_TDPy_idx == 500)&&(TDPx_idx == 0))
		{
			CanA_TX_CRC3 = CanA_TX_H0;

		}		
		TDPx_idx++;
		if(TDPx_idx > 3)
		{
			TDPx_idx = 0;
			cana_TDPy_idx++;
		}
	}
	else if(cana_TDPy_idx == TDPy_N)
		{	// ÀüÃ¼ 32kbyte ´Ù º¸³ÂÀ¸¸é ÆúÆ®¹ß»ý ½Ã°£ º¸³¿
		CaTX_a1 = PWORD(ADD_NVR_TDP0 + ((long)tdpage * TDP_SIZE) + TDPy_N*TDPx_N + 0)&0xff;//³âµµ ÇÏÀ§ 10, 1ÀÚ¸®
		CaTX_a2 = PWORD(ADD_NVR_TDP0 + ((long)tdpage * TDP_SIZE) + TDPy_N*TDPx_N + 1)&0xff;//RTC_months;
		CaTX_a3 = PWORD(ADD_NVR_TDP0 + ((long)tdpage * TDP_SIZE) + TDPy_N*TDPx_N + 2)&0xff;//RTC_dates
		CaTX_a4 = PWORD(ADD_NVR_TDP0 + ((long)tdpage * TDP_SIZE) + TDPy_N*TDPx_N + 3)&0xff;//RTC_hours
	
		CaTX_a5 = PWORD(ADD_NVR_TDP0 + ((long)tdpage * TDP_SIZE) + TDPy_N*TDPx_N + 4)&0xff;//RTC_minutes
		CaTX_a6 = PWORD(ADD_NVR_TDP0 + ((long)tdpage * TDP_SIZE) + TDPy_N*TDPx_N + 5)&0xff;//RTC_seconds
		CaTX_a7 = PWORD(ADD_NVR_TDP0 + ((long)tdpage * TDP_SIZE) + TDPy_N*TDPx_N + 6)&0xff;//0
		CaTX_a8 = PWORD(ADD_NVR_TDP0 + ((long)tdpage * TDP_SIZE) + TDPy_N*TDPx_N + 7)&0xff;//0
		CanA_TX_L0 = ((long)CaTX_a1<<24) | ((long)CaTX_a2<<16) | (CaTX_a3<<8) | CaTX_a4;
		CanA_TX_H0 = ((long)CaTX_a5<<24) | ((long)CaTX_a6<<16) | (CaTX_a7<<8) | CaTX_a8;
		CanA_TX_CRC ^= CanA_TX_L0;
		CanA_TX_CRC ^= CanA_TX_H0;

		SendDataToECanA(WGBC_M_ID, 0x08, CanA_TX_L0, CanA_TX_H0);	
		cana_TDPy_idx++;	
	}
	else if(cana_TDPy_idx == TDPy_N+1)
	{	// ¸¶Áö¸·À¸·Î CRCº¸³¿
		SendDataToECanA(WGBC_M_ID, 0x08, CanA_TX_CRC, CanA_TX_CRC);		
		cana_TDPy_idx = 0;	
		CanA_TDR_flag = 0;	// ÀüÃ¼ ¼Û½Å ÇÁ·¹ÀÓ Àü¼Û ¿Ï·á
	}
	else
	{
		cana_TDPy_idx = 0;	
		CanA_TDR_flag = 0;	// ÀüÃ¼ ¼Û½Å ÇÁ·¹ÀÓ Àü¼Û ¿Ï·á	
	}
	if(CanTx_flagON)
	{
		MCU_LD2_ON();	
	
	}
	else
	{
		MCU_LD2_OFF();
	}
}

//===============================================================//
void TX_CANA_To_CommBoard_TD()
{
	unsigned int tdpage = 0;

	unsigned int tdpage_fidxL = 1;
	unsigned int tdpage_fidxH = 1;
	unsigned int tdpage_fidx = 1;
	
	tdFaultCounterTotal = CanA_PC_Udata1;	// TD ¿äÃ»ÆäÀÌÁö
	
	tdpage = TDP_Index_Search(tdFaultCounterTotal)-1;//´©ÀûÆúÆ®Ä«¿îÅÍ·Î Æ®·¹ÀÌ½º ÆúÆ®ÆäÀÌÁö ÀÎµ¦½º »õ·Î °è»ê

	tdpage_fidxL = PWORD(ADD_NVR_CDP0 + 20 + tdpage*2)&0xff;		// ÆúÆ®¹ß»ý½ÃÁ¡(L)
	tdpage_fidxH = (PWORD(ADD_NVR_CDP0 + 21 + tdpage*2))&0xff;	// ÆúÆ®¹ß»ý½ÃÁ¡(H)
	tdpage_fidx = (tdpage_fidxH<<8)|tdpage_fidxL;					// ÆúÆ®¹ß»ý½ÃÁ¡(HL)

	if(cana_TDPy_idx < TDPy_N){//ÃÑ 1000*4*8 =  320000¹ÙÀÌÆ®
		if((cana_TDPy_idx == 0)&&(TDPx_idx == 0))
		{
			CanA_TX_CRC = 0;
		}
		//===ÆúÆ®½ÃÁ¡À» Áß°£À¸·Î Á¤·ÄÇÏ¿© Àü¼Û
		if(cana_TDPy_idx < (TDPy_N/2 - tdpage_fidx - 1))
		{
			TDPy_idx = (TDPy_N/2 + tdpage_fidx + 1) + cana_TDPy_idx;	
		}
		else{
			TDPy_idx = (((TDPy_N/2 + tdpage_fidx + 1) + cana_TDPy_idx)-TDPy_N)%TDPy_N;			
		}

		//===start idx ªO±â=====
		if((tdpage_fidx > 497)&&(tdpage_fidx <= 499))
		{
			start_idx_tmp = 1000 + (tdpage_fidx - (TDPy_N/2 -1)-1);
		}
		if(tdpage_fidx >= 500){
			start_idx_tmp = tdpage_fidx - (TDPy_N/2 -1)-1;	
		}
		else
		{
			start_idx_tmp = tdpage_fidx + (TDPy_N/2);		
		}				
		//===td idx ªO±â=====
		if(cana_TDPy_idx > 1000-1-start_idx_tmp){
			TDPy_idx = (start_idx_tmp+cana_TDPy_idx)-1000;
		}
		else
		{	
			TDPy_idx = (start_idx_tmp+cana_TDPy_idx);
		}


		CaTX_a1 = PWORD(ADD_NVR_TDP0 + (long)tdpage*TDP_SIZE + TDPy_idx*TDPx_N + TDPx_idx*8 + 0)&0xff;//TDPx_N=32
		CaTX_a2 = PWORD(ADD_NVR_TDP0 + (long)tdpage*TDP_SIZE + TDPy_idx*TDPx_N + TDPx_idx*8 + 1)&0xff;
		CaTX_a3 = PWORD(ADD_NVR_TDP0 + (long)tdpage*TDP_SIZE + TDPy_idx*TDPx_N + TDPx_idx*8 + 2)&0xff;
		CaTX_a4 = PWORD(ADD_NVR_TDP0 + (long)tdpage*TDP_SIZE + TDPy_idx*TDPx_N + TDPx_idx*8 + 3)&0xff;
		
		CaTX_a5 = PWORD(ADD_NVR_TDP0 + (long)tdpage*TDP_SIZE + TDPy_idx*TDPx_N + TDPx_idx*8 + 4)&0xff;
		CaTX_a6 = PWORD(ADD_NVR_TDP0 + (long)tdpage*TDP_SIZE + TDPy_idx*TDPx_N + TDPx_idx*8 + 5)&0xff;
		CaTX_a7 = PWORD(ADD_NVR_TDP0 + (long)tdpage*TDP_SIZE + TDPy_idx*TDPx_N + TDPx_idx*8 + 6)&0xff;
		CaTX_a8 = PWORD(ADD_NVR_TDP0 + (long)tdpage*TDP_SIZE + TDPy_idx*TDPx_N + TDPx_idx*8 + 7)&0xff;
		CanA_TX_L0 = ((long)CaTX_a1<<24) | ((long)CaTX_a2<<16) | (CaTX_a3<<8) | CaTX_a4;
		CanA_TX_H0 = ((long)CaTX_a5<<24) | ((long)CaTX_a6<<16) | (CaTX_a7<<8) | CaTX_a8;
		CanA_TX_CRC ^= CanA_TX_L0;
		CanA_TX_CRC ^= CanA_TX_H0;
		SendDataToECanA(WGBC_M_ID, 0x08, CanA_TX_L0, CanA_TX_H0);		
			
		if((cana_TDPy_idx == 0)&&(TDPx_idx == 0))
		{
			CanA_TX_CRC1 = CanA_TX_H0;
			CanA_TX_CRC4 = TDPy_idx;
		}

		if((cana_TDPy_idx == 499)&&(TDPx_idx == 0))
		{

			CanA_TX_CRC2 = CanA_TX_H0;	
		}
				
		if((cana_TDPy_idx == 500)&&(TDPx_idx == 0))
		{

			CanA_TX_CRC3 = CanA_TX_H0;

		}		
		TDPx_idx++;

		if(TDPx_idx > 3)
		{
			TDPx_idx = 0;
			cana_TDPy_idx++;
		}
	}
	else if(cana_TDPy_idx == TDPy_N)
	{	// ÀüÃ¼ 32kbyte ´Ù º¸³ÂÀ¸¸é ÆúÆ®¹ß»ý ½Ã°£ º¸³¿

		CaTX_a1 = PWORD(ADD_NVR_TDP0 + ((long)tdpage * TDP_SIZE) + TDPy_N*TDPx_N + 0)&0xff;//³âµµ ÇÏÀ§ 10, 1ÀÚ¸®
		CaTX_a2 = PWORD(ADD_NVR_TDP0 + ((long)tdpage * TDP_SIZE) + TDPy_N*TDPx_N + 1)&0xff;//RTC_months;
		CaTX_a3 = PWORD(ADD_NVR_TDP0 + ((long)tdpage * TDP_SIZE) + TDPy_N*TDPx_N + 2)&0xff;//RTC_dates
		CaTX_a4 = PWORD(ADD_NVR_TDP0 + ((long)tdpage * TDP_SIZE) + TDPy_N*TDPx_N + 3)&0xff;//RTC_hours
	
		CaTX_a5 = PWORD(ADD_NVR_TDP0 + ((long)tdpage * TDP_SIZE) + TDPy_N*TDPx_N + 4)&0xff;//RTC_minutes
		CaTX_a6 = PWORD(ADD_NVR_TDP0 + ((long)tdpage * TDP_SIZE) + TDPy_N*TDPx_N + 5)&0xff;//RTC_seconds
		CaTX_a7 = PWORD(ADD_NVR_TDP0 + ((long)tdpage * TDP_SIZE) + TDPy_N*TDPx_N + 6)&0xff;//0
		CaTX_a8 = PWORD(ADD_NVR_TDP0 + ((long)tdpage * TDP_SIZE) + TDPy_N*TDPx_N + 7)&0xff;//0
		CanA_TX_L0 = ((long)CaTX_a1<<24) | ((long)CaTX_a2<<16) | (CaTX_a3<<8) | CaTX_a4;
		CanA_TX_H0 = ((long)CaTX_a5<<24) | ((long)CaTX_a6<<16) | (CaTX_a7<<8) | CaTX_a8;
		CanA_TX_CRC ^= CanA_TX_L0;
		CanA_TX_CRC ^= CanA_TX_H0;

		SendDataToECanA(WGBC_M_ID, 0x08, CanA_TX_L0, CanA_TX_H0);	
		cana_TDPy_idx++;	
	}

	else if(cana_TDPy_idx == TDPy_N+1)
	{	// ¸¶Áö¸·À¸·Î CRCº¸³¿
		SendDataToECanA(WGBC_M_ID, 0x08, CanA_TX_CRC, CanA_TX_CRC);		
		cana_TDPy_idx = 0;	
		CanA_TDR_flag = 0;	// ÀüÃ¼ ¼Û½Å ÇÁ·¹ÀÓ Àü¼Û ¿Ï·á
	}

	else
	{
		cana_TDPy_idx = 0;	
		CanA_TDR_flag = 0;	// ÀüÃ¼ ¼Û½Å ÇÁ·¹ÀÓ Àü¼Û ¿Ï·á	
	}
}
//======================================================================//
// ±â  ´É : CAN1,2 Å×½ºÆ®                    										
// CANA¿¡¼­ º¸³»°í CANB¿¡¼­ ¹Þ´Â°ÅÈ®ÀÎ
//========================================================================//
void Can_Routine(void)
{	
	if(CAN_TEST_MODE == 0)
	{	// Á¤»ó¸ðµå(MCU CANA ---> COMM BOARD(MASTER))
		if(CanA_SDR_flag)
		{
			TX_CANA_To_CommBoard_SD();			// Á¤»óÀû SD Åë½Å¹ÝÀÀ(MCU(CANA) ---> COMM)
		}
		else if(CanA_TDR_flag)
		{
			TX_CANA_To_CommBoard_TD();			// Trace data Àü¼Û(MCU(CANA) ---> COMM)
		}
		else if(CanA_TD_CLEAR_flag)
		{
			Nvram_TDpage_clear();				// Trace data clear(from PC-->Commº¸µå -->MCU)
			//TX_CANA_To_CommBoard_TDCLEAR();		// TRACE DATA Áö¿ì°í ´Ù Áö¿ü´Ù°í Commº¸µå¿¡ ÀÀ´ä
		}
		else if(CanA_RTC_UPGrade_flag)
		{
			RTC_Write();						// RTC DATA °»½Å(from Â÷·® --> commº¸µå --> MCU)
			//TX_CANA_To_CommBoard_RTCUPGRADE();
		}
		else if(CanA_FW_UPGrade_flag)
		{
			CanA_FW_UPGrade_flag = 0;				// MCU Firmware Upgrade(from PC-->Commº¸µå -->MCU)
		}	
	}
}

//==============================================================//
//==ÆúÆ®Ä«¿îÍ·Î ÆúÆ®ÀúÀåÆäÀÌÁöÀÎµ¦½º ªOÀ½
//  in : tmpfcnt(ÀúÀåÇÒ ÆäÀÌÁö ÀÎµ¦½º ¶Ç´Â ÀúÀåµÈ ÆúÆ®ÆäÀÌÁö ÀÎµ¦½º
//  out : tmpfpindex(ªOÀº ÆúÆ®ÆäÀÌÁö ÀÎµ¦½º)
//  ÀúÀåÆäÀÌÁö --- 0~2(3°³)ÆäÀÌÁö´Â ¼øÈ¯¾ÈµÊ(0:Ã³À½¹ß»ýÆúÆ®,1:2¹øÂ°,2:3¹øÂ°) --> clearÇÏ±âÀü±îÁö °íÁ¤À¸·Î ÀúÀå
//  ÀúÀåÆäÀÌÁö --- 3~14(9°³)ÆäÀÌÁö´Â ¼øÈ¯(¸µ¹öÆÛ·Î ÀúÀå)
//==============================================================//
unsigned int TDP_Index_Search(unsigned int tmpfcnt)
{
	unsigned int fpage_Total_N = 15;//ÃÑ ÆúÆ®ÆäÀÌÁö ¼ö·® 9ÆäÀÌÁö
	unsigned int fpage_RBuf_StartIdx = 3;//¸µ¹öÆÛ ½ÃÀÛ ÀÎµ¦½º
	unsigned int fpage_RBuf_N = 12;//¸µ¹öÆÛ ¼ö·®(3,4,5,6,7,8,9,10,11,12,13,14)
	unsigned int tmpfpindex;//

	fpage_RBuf_N = (fpage_Total_N) - fpage_RBuf_StartIdx; // 15-3 = 12;
	
	if(tmpfcnt < fpage_Total_N) tmpfpindex = tmpfcnt;//14±îÁö, 15¸é 3ÆäÀÌÁö¿¡ ÀúÀå
	else{
		tmpfpindex = tmpfcnt- fpage_RBuf_N*((tmpfcnt-fpage_RBuf_StartIdx)/fpage_RBuf_N); // ¸µ¹öÆÛ ¹æ½ÄÀÇ Æ®·¹ÀÌ½ºµ¥ÀÌÅ¸ ÀúÀåÀÎµ¦½º °è»ê
	}
	return tmpfpindex;
}

unsigned int BCDtoDEC(unsigned char data)
{
	unsigned int a,b;
	
	a = ((data >> 4) & 0x0f);
	b = data - a*0x10;
	return (a*10+b);
}
