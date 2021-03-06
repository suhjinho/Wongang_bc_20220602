//==================================================================================//
// 원강선 동력차(30kw)/객차(50kw) 축전지 충전기 모듈(10kw) 모니터링프로그램	  		
//==================================================================================//
// nvram 부분																		
// DATE  : 2015. 02. 10  ---can protocol 변경               by PACTECH R&D LSH		
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

long CanA_TX_L1_ver = 0;//버전테스트용--->지울것
unsigned long idxy = 0;
unsigned int TDPy_i = 0;

unsigned int FaultCounterTotal = 0;
// 누적폴트카운터, 전원꺼도 nvram에 저장됨, 부팅시 nvram에서 읽어야함, td clear시 같이 clear
// 범위 : 0 ~ 250
unsigned int Total_SaveDt = 10;	// 트레이스데이타 폴트 저장간격(msec*10 단위, 10msec00>100,1msec-->10, 500usec-->5)
unsigned int TDP_i_current = 0;//트레이스데이타 페이지 인덱스(FaultCounterTotal로 계산됨)
unsigned int TDP_index_test = 0;//트레이스데이타 페이지 인덱스(FaultCounterTotal로 계산됨)
unsigned int FaultCounterTotal_test = 0;//트레이스데이타 페이지 인덱스(FaultCounterTotal로 계산됨)
unsigned int TDPy_i_testL = 0;//트레이스데이타 페이지 인덱스(FaultCounterTotal로 계산됨)
unsigned int TDPy_i_testH = 0;//트레이스데이타 페이지 인덱스(FaultCounterTotal로 계산됨)
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

// (0)COMM(Master)에서 SDR수신아님 ----> SD TX 안됨, SD전체 다 전송하면 (0)으로 만들어줌
// (1)COMM(Master)에서 SDR수신했음(CANA 수신 INT에서 SET(1) ----> SD TX 가능함(이플레그 보고 전송함), SD전체 다 전송하면 (0)으로 만들어줌
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
	TDP_index_test = TDP_Index_Search(FaultCounterTotal_test)-1;//누적폴트카운터로 트레이스 폴트페이지 인덱스 새로 계산
	TDPy_i_testL = PWORD(ADD_NVR_CDP0 + TDP_index_test*2 + 20)&0xff;	// 폴트시점(L)
	TDPy_i_testH = PWORD(ADD_NVR_CDP0 + TDP_index_test*2 + 21)&0xff;	// 폴트시점(H) 	
	TDPy_i_test = (unsigned int)(((TDPy_i_testL)|(TDPy_i_testH<<8)) + TDPy_i_offset_test);

	idxy_test = ADD_NVR_TDP0 + ((unsigned long)TDP_index_test * TDP_SIZE) + TDPy_i_test*TDPx_N;
	nvrd_data_test = PWORD(idxy_test + TDPx_i_offset_test)&0xff;


}


//======================================================================//
// nvram 초기화
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

		CanA_RTC_UPGrade_flag = 2;	//시간데이타 초기화
		RTC_Write();

		PWORD(ADD_NVR_CDP0 + 0) = 'W';
		PWORD(ADD_NVR_CDP0 + 1) = 'G';
		PWORD(ADD_NVR_CDP0 + 2) = 'B';
		PWORD(ADD_NVR_CDP0 + 3) = 'C';
		PWORD(ADD_NVR_CDP0 + 4) = (ProgramVersion&0xff);		//프로그램버젼 초기화
		FaultCounterTotal = 0;
		PWORD(ADD_NVR_CDP0 + 10) = FaultCounterTotal&0xff;	// 누적폴트가운터 초기화(0)	
 	}
 	
 	FaultCounterTotal = PWORD(ADD_NVR_CDP0 + 10)&0xff;// 누적폴트가운터 읽어옴
	TDP_i_current = TDP_Index_Search(FaultCounterTotal);//누적폴트카운터로 현재 TD 폴트페이지 인덱스 계산		
	
	tst_tmp0 = PWORD(ADD_NVR_CDP0 + 0) & 0x0ff;
	tst_tmp1 = PWORD(ADD_NVR_CDP0 + 1) & 0x0ff;
	tst_tmp2 = PWORD(ADD_NVR_CDP0 + 2) & 0x0ff;
	tst_tmp3 = PWORD(ADD_NVR_CDP0 + 3) & 0x0ff;
	tst_tmp4 = PWORD(ADD_NVR_CDP0 + 4) & 0x0ff;
}
//======================================================================//
// nvram 트레이스데이타(TD) CLEAR(0)
// 총 12페이지 0으로 CLEAR
// 누적폴트카운터(FaultCounterTotal) 초기화(0)
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
		PWORD(ADD_NVR_CDP0 + 10) = FaultCounterTotal&0xff;	// 누적폴트가운터 초기화(0)	
		TDP_i_current = TDP_Index_Search(FaultCounterTotal);//누적폴트카운터로 현재 TD 폴트페이지 인덱스 계산		
		FaultCounter = 0;
		for(i=0;i<30;i++)
		{
			PWORD(ADD_NVR_CDP0 + 20 + i) = 0;	// PAGE0~PAGE14 폴트시점(L)/(H) 초기화	
		}
		
		CanA_TD_CLEAR_flag = 0;
		TX_CANA_To_CommBoard_TDCLEAR();
	}
}

//==========================================================================================//
// nvram에 TRACE 데이타 저장(1msec주기로 48바이트 데이타를 1000개까지 저장 ---링버퍼구조
// 
//------TRACE 데이타 저장방법----------------------------------------------------------------//
//1. 부팅시 누적폴트카운터(FaultCounterTotal)를 읽어서 현재페이지로 참조함(현재페이지는 누적폴트카운트)
//1. 현재 페이지에 1msec 간격으로 링버퍼로 데이타 저장 --> page.i(TDPy_i)++
//2. 폴트발생한 시점에 인덱스(TDPy_i)를 기억하고, 현재 페이지에 (TD_Row_Idx  == TD_Row_N /2)까지 계속 저장
//3. TDPy_i 가 TDPy_N /2 면 Page0 데이터를 전부 읽어서 해당 고장페이지에 저장
//===========================================================================================//
void NvramSDTDWrite()
{
	unsigned int tempL, tempH;

	Make_SDTD();
	
	RTC_Read();	//시간데이타 읽어옴
	Nvram_TDpage_read_TEST();
	
	if((FaultAck == YES)&&(FaultAckOld == NO)){	// 폴트 상승에지에서..
		nvr_f_saveflag = YES;	
		nvr_f_counter = 0;
		TDP_i_current = TDP_Index_Search(FaultCounterTotal);//누적폴트카운터로 현재 TD 폴트페이지 인덱스 계산
		PWORD(ADD_NVR_CDP0 + 20 + TDP_i_current*2) = (TDPy_i)&0xff;		// 폴트시점(L) 저장
		PWORD(ADD_NVR_CDP0 + 21 + TDP_i_current*2) = (TDPy_i>>8)&0xff;	// 폴트시점(H) 저장

		PWORD(ADD_NVR_TDP0 + (TDP_i_current * TDP_SIZE) + TDPy_N*TDPx_N + 0) = (RTC_years-(RTC_years/100)*100) & 0xff;	//년도 하위 10, 1자리
		PWORD(ADD_NVR_TDP0 + (TDP_i_current * TDP_SIZE) + TDPy_N*TDPx_N + 1) = RTC_months;
		PWORD(ADD_NVR_TDP0 + (TDP_i_current * TDP_SIZE) + TDPy_N*TDPx_N + 2) = RTC_dates;
		PWORD(ADD_NVR_TDP0 + (TDP_i_current * TDP_SIZE) + TDPy_N*TDPx_N + 3) = RTC_hours;
		PWORD(ADD_NVR_TDP0 + (TDP_i_current * TDP_SIZE) + TDPy_N*TDPx_N + 4) = RTC_minutes;
		PWORD(ADD_NVR_TDP0 + (TDP_i_current * TDP_SIZE) + TDPy_N*TDPx_N + 5) = RTC_seconds;		
		PWORD(ADD_NVR_TDP0 + (TDP_i_current * TDP_SIZE) + TDPy_N*TDPx_N + 6) = (Total_SaveDt) & 0xff;	//폴트 저장간격	-->CRC로 변경예정	
		PWORD(ADD_NVR_TDP0 + (TDP_i_current * TDP_SIZE) + TDPy_N*TDPx_N + 7) = (FaultCounterTotal) & 0xff;	//전체 폴트카운트(누적)

	}
	FaultAckOld = FaultAck;
	
	if(nvr_f_saveflag == YES)
	{
		if(nvr_f_counter == TDPy_N/2){//폴트난후에 1000/2 = 500개 데이타 저장하기 했으면 현재 페이지를 다음 페이지로 넘김
			nvr_f_saveflag = NO;
			nvr_f_counter = 0;
			FaultCounterTotal++;
			PWORD(ADD_NVR_CDP0 + 10) = FaultCounterTotal & 0xff;	// 누적폴트가운터 +1
			TDP_i_current = TDP_Index_Search(FaultCounterTotal);//누적폴트카운터로 트레이스 폴트페이지 인덱스 새로 계산
			TDPy_i = 0;
		}
		nvr_f_counter++;	//폴트난후에 500개 데이타 더 저장하기 위해
	}

	//----저장할 번지 계산
	//저장할 번지(idxy) = page0번지+(페이지인덱스 * 페이지사이즈(0xC000)) + 페이지내(Y인덱스*X크기(40));
	idxy = ADD_NVR_TDP0 + ((unsigned long)TDP_i_current * TDP_SIZE) + TDPy_i*TDPx_N;

	TDPy_i++;
	if(TDPy_i == TDPy_N)	TDPy_i = 0; //0~999까지만 저장

	//----TRADE DATA PAGE에 TRACE DATA 저장	---> 총 40개 바이트
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

	if(CanA_RTC_UPGrade_flag==1){	// 차량시간과 동기화
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
	else if(CanA_RTC_UPGrade_flag==2){	// MCU에 노트북연결해서 CAN 또는 리얼시스모니터링으로 RTC초기화
	
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
	CaTX_a1 = (((unsigned int)MInputVolLPF) >> 2) & 0xff;	// 입력전압/4
	CaTX_a2 = (((unsigned int)MDcLinkVolLPF) >> 2) & 0xff;	// 입력전압/4
	//CaTX_a3 = (((unsigned int)MCUBD_HEXSW_IN) >> 0) & 0xff;		// 입력전압(obd후단)/1
	CaTX_a3 = 50;		// 입력전압(obd후단)/1
	CaTX_a4 = 50;		// 입력전압(obd후단)/1
	CaTX_a5 = 50;		// 입력전압(obd후단)/1
	CaTX_a6 = 50;		// 입력전압(obd후단)/1
	CaTX_a7 = 50;		// 입력전압(obd후단)/1
//	CaTX_a3 = (((unsigned int)MOutVolLPF) >> 0) & 0xff;		// 입력전압(obd후단)/1
	//CaTX_a4 = (((unsigned int)MBatVolLPF) >> 0) & 0xff;	// 출력전압1(obd전단)/1
	
	//CaTX_a5 = (((unsigned int)MInputCurLPF) >> 0) & 0xff;	// 모듈 입력전류/1
	//CaTX_a6 = (((unsigned int)MOutCurLPF) >> 0) & 0xff;		// 모듈출력전류/1
	//CaTX_a7 = (((unsigned int)MBatCurLPF) >> 0) & 0xff;		// 모듈 충전전류/1
	CaTX_a8 = (((unsigned int)BOutCurLPF) >> 2) & 0xff;		// 박스 출력전류/4

	CaTX_a9 = (((unsigned int)BLoadCurLPF) >> 2) & 0xff;// 박스 부하전류/4 = 출력전류-충전전류(1,2)
	CaTX_a10 = (((unsigned int)BBatCur1LPF) >> 0) & 0xff;		// 박스 배터리 전류1/1
	CaTX_a11 = (((unsigned int)BBatCur2LPF) >> 0) & 0xff;	// 박스 배터리 전류2/1
	CaTX_a12 = (((unsigned int)MCtrlVolLPF) >> 0) & 0xff;	// 제어전압/1

	//CaTX_a13 = (((unsigned int)ModuleFault_NUM) >> 2) & 0xff;	// 임시--고장모듈수량 인식한것
	CaTX_a13 = (((unsigned int)Pall_OK_ModuleN) >> 2) & 0xff;	// 임시--정상모듈수량 인식한것
	CaTX_a14 = (((unsigned int)FaultCounter) >> 0) & 0xff;	// 폴트카운터 표시
	CaTX_a15 = (((unsigned int)FaultCounterTotal) >> 0) & 0xff;	// 누적폴트 표시
	CaTX_a16 = (((unsigned int)((OutVolRef-70.0)*10.0)) >> 0) & 0xff;	//

	CaTX_a17 = (((unsigned int)BLoadCurLPF) >> 2) & 0xff;//
	CaTX_a18 = (((unsigned int)BBatCur1LPF) >> 0) & 0xff;	



}
void Make_SDTD_Analog()
{

	CaTX_a1 = (((unsigned int)MInputVolLPF) >> 2) & 0xff;	// 입력전압/4
	CaTX_a2 = (((unsigned int)MDcLinkVolLPF) >> 2) & 0xff;	// 입력전압/4
	CaTX_a3 = (((unsigned int)MOutVolLPF) >> 0) & 0xff;		// 입력전압(obd후단)/1
	CaTX_a4 = (((unsigned int)MBatVolLPF) >> 0) & 0xff;	// 출력전압1(obd전단)/1
	
	CaTX_a5 = (((unsigned int)MInputCurLPF) >> 0) & 0xff;	// 모듈 입력전류/1
	CaTX_a6 = (((unsigned int)MOutCurLPF) >> 0) & 0xff;		// 모듈출력전류/1
	CaTX_a7 = (((unsigned int)MBatCurLPF) >> 0) & 0xff;		// 모듈 충전전류/1
	CaTX_a8 = (((unsigned int)BOutCurLPF) >> 2) & 0xff;		// 박스 출력전류/4

	CaTX_a9 = (((unsigned int)BLoadCurLPF) >> 2) & 0xff;// 박스 부하전류/4 = 출력전류-충전전류(1,2)
	CaTX_a10 = (((unsigned int)BBatCur1LPF) >> 0) & 0xff;		// 박스 배터리 전류1/1
	CaTX_a11 = (((unsigned int)BBatCur2LPF) >> 0) & 0xff;	// 박스 배터리 전류2/1
	CaTX_a12 = (((unsigned int)MCtrlVolLPF) >> 0) & 0xff;	// 제어전압/1

	CaTX_a13 = (((unsigned int)ProgramVersion)) & 0xff;	// 임시--정상모듈수량 인식한것
	CaTX_a14 = (((unsigned int)FaultCounter) >> 0) & 0xff;	// 폴트카운터 표시
	CaTX_a15 = (((unsigned int)FaultCounterTotal) >> 0) & 0xff;	// 누적폴트 표시
	CaTX_a16 = (((unsigned int)((OutVolRef-70.0)*10.0)) >> 0) & 0xff;	//

	CaTX_a17 = (((unsigned int)BLoadCurLPF) >> 2) & 0xff;//
	CaTX_a18 = (((unsigned int)BBatCur1LPF) >> 0) & 0xff;	



}

void Make_TD_Fault()
{
//=====FAULT DATA
//=====TD는 실데이타반영, SD는 재기동하기 전까지 폴트표시
//---DIG5----BCM_TDF1
	if(btst(FGDF,GDF1))			sbit(BCM_TDF1, 0);// GDF1 폴트
	else						cbit(BCM_TDF1, 0);

	if(btst(FGDF,GDF2))			sbit(BCM_TDF1, 1);// GDF2 폴트
	else						cbit(BCM_TDF1, 1);
	
	if((btst(FHWF,HWF10_IVOVF))||(btst(FSWF,SWF10_IVOVF)))		sbit(BCM_TDF1, 2);// 입력과전압(HWF 또는 SWF)
	else														cbit(BCM_TDF1, 2);

	//if(btst(FSWF,SWF15_FCUVF))  								sbit(BCM_TDF1, 3);//입력저전압===> FC저전압	
	//else														cbit(BCM_TDF1, 3);	
	
	if((btst(FHWF,HWF11_FCOVF))||(btst(FSWF,SWF11_FCOVF)))  		sbit(BCM_TDF1, 4);// DCLINK과전압(HWF 또는 SWF)
	else														cbit(BCM_TDF1, 4);	
	
	if((btst(FHWF,HWF8_OVOVF))||(btst(FSWF,SWF8_OVOVF)))  		sbit(BCM_TDF1, 5);// 출력과전압((OBD후))(HWF 또는 SWF)
	else														cbit(BCM_TDF1, 5);	
	
	if((btst(FHWF,HWF8_OVOVF))||(btst(FSWF,SWF8_OVOVF)))  		sbit(BCM_TDF1, 6);// 출력과전압((OBD후))(HWF 또는 SWF)
	else														cbit(BCM_TDF1, 6);	
	
	if((btst(FHWF,HWF0_ICOCF))||(btst(FSWF,SWF0_ICOCF)))  		sbit(BCM_TDF1, 7);// 입력과전류 (HWF 또는 SWF)
	else  														cbit(BCM_TDF1, 7);
	//---DIG6----BCM_TDF2
	
	if((btst(FHWF,HWF2_MOCF))||(btst(FSWF,SWF2_MOCF)))  			sbit(BCM_TDF2, 0);// 입력 출력과전류 (HWF 또는 SWF)
	else														cbit(BCM_TDF2, 0);	
	
	if((btst(FHWF,HWF1_MBATOCF))||(btst(FSWF,SWF1_MBATOCF)))  	sbit(BCM_TDF2, 1);// 모듈 충전과전류 (HWF 또는 SWF)
	else														cbit(BCM_TDF2, 1);	
	
	//if((btst(FHWF,HWF1_MBATOCF))||(btst(FSWF,SWF1_MBATOCF)))  	sbit(BCM_TDF2, 2);// 모듈 과부하시
	//else														cbit(BCM_TDF2, 2);	
	cbit(BCM_TDF2, 2);		
	
	if((btst(FETF,ETCF_FANF1))||(btst(FETF,ETCF_FANF2)))  		sbit(BCM_TDF2, 3);// FAN1 또는 FAN2 폴트
	else														cbit(BCM_TDF2, 3);	

	if(btst(FETF,ETCF_HSKTHRF))  								sbit(BCM_TDF2, 3);// 히트싱크 과온폴트시 폴트시
	else														cbit(BCM_TDF2, 3);	
	
	if((btst(FHWF,HWF7_CPOVF))||(btst(FSWF,SWF7_CPOVF))||(btst(FSWF,SWF14_CPUVF)))  		sbit(BCM_TDF2, 4);// 제어전압 과전압(HWF,SWF)/저전압(SWF)
	else																				cbit(BCM_TDF2, 4);	
	
	//if((btst(FHWF,HWF7_CPOVF))||(btst(FSWF,SWF7_CPOVF))||(btst(FSWF,SWF14_CPUVF)))  	sbit(BCM_TDF2, 5);// 시퀸스폴트 폴트--> 1개비트로 표현?
	//else																				cbit(BCM_TDF2, 5);	
	cbit(BCM_TDF2, 5);		
	
	//if((btst(FHWF,HWF7_CPOVF))||(btst(FSWF,SWF7_CPOVF))||(btst(FSWF,SWF14_CPUVF)))  	sbit(BCM_TDF2, 6);// 휴즈단선시 
	//else																				cbit(BCM_TDF2, 6);	
	//cbit(BCM_TDF2, 6);		
	if(btst(FETF,ETCF_FUSEF))  	sbit(BCM_TDF2, 6);// 휴즈단선시 
	else							cbit(BCM_TDF2, 6);	

	
	if(MajorFault)  							sbit(BCM_TDF2, 7);// 중고장시
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

	//BCM_SDF4 = (DioLedBuf2<<4)&0xff;//LSH 20150922 최종출력전압 저전압(OVUV2)를 비트0으로 표시, 나머지 상위비트는 아직 사용안함
	if(btst(FSWF,SWF12_OVUVF)){
		sbit(BCM_SDF4,0);//LSH 20151208 최종출력전압 저전압(OVUV2)를 비트0으로 표시, 나머지 상위비트는 아직 사용안함
	}
	else{
		cbit(BCM_SDF4,0);//LSH 20151208 최종출력전압 저전압(OVUV2)를 비트0으로 표시, 나머지 상위비트는 아직 사용안함
	}

	if(btst(FSWF,SWF15_OVUVF2))
	{
		sbit(BCM_SDF4,1);//LSH 20151208 최종출력전압 저전압(OVUV2)를 비트1으로 표시, 나머지 상위비트는 아직 사용안함
	}
	else
	{
		cbit(BCM_SDF4,1);//LSH 20151208 최종출력전압 저전압(OVUV2)를 비트1으로 표시, 나머지 상위비트는 아직 사용안함
	}
	



	//220124 동차모듈에 저전류 폴트 표시 
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

//---DIG1-----BOX 상태정보1
	//if(MODEAck)		sbit(BCBox_Status, 0);	// box 정상
	//else				cbit(BCBox_Status, 0);		
	//if(MODEAck)		sbit(BCBox_Status, 1);	// box 중고장
	//else				cbit(BCBox_Status, 1);		

	//cbit(BCBox_Status, 0);// box 정상		//===20160323===bcstatus수정, BCOK(1), BCMJF(1)	
	//cbit(BCBox_Status, 1);// box 중고장		//===20160323===bcstatus수정, BCOK(1), BCMJF(1)	


//---DIG2-----MODULE 상태정보1

		
	if(BCSTARTAck)		sbit(BCM_Status1, 0);	// 차량에서 BC START명령 인식
	else				cbit(BCM_Status1, 0);
	if(MODEAck)			sbit(BCM_Status1, 1);	// MODE = TESTMODE0, NORMALMODE1
	else				cbit(BCM_Status1, 1);		
	if(!FaultAck){								//기동준비상태(READY)신호는 우선 폴트가 없으면 
		sbit(BCM_Status1, 2);			
	}
	else{
		cbit(BCM_Status1, 2);			
	}
	if(CHKAck)			sbit(BCM_Status1, 3);	// CHKA상태
	else				cbit(BCM_Status1, 3);		
	if(LKAck)			sbit(BCM_Status1, 4);	// LKA상태
	else				cbit(BCM_Status1, 4);		
	if(PWM_FLAG)		sbit(BCM_Status1, 5);	// PWM ON
	else				cbit(BCM_Status1, 5);		
	if(MOUTV_OKAck)		sbit(BCM_Status1, 6);	// 모듈 출력전압 정상 출력
	else				cbit(BCM_Status1, 6);		

	
	if(FanOnOffCmd_flag == 1)
	{	// FAN1,2 동작시키고
		if((!FANF1Ack)&&(!FANF2Ack)){// 팬폴트 없으면
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
	//if(RESETAck)		sbit(BCM_Status1, 6);	// 모듈 출력전압 정상 출력시(1), 아니면(0)
	//else				cbit(BCM_Status1, 6);		
//---DIG3-----MODULE 상태정보2 ---->통신프로토콜 추가해야함(현재 DIG2까지만 되어있음)
	if(Mode_CV0CC1 == Mode_CV0)		sbit(BCM_Status2, 0);	// CV MODE면 
	else							cbit(BCM_Status2, 0);	// CC MODE면 
	cbit(BCM_Status2, 1);	// TBD
	cbit(BCM_Status2, 2);	// TBD
	cbit(BCM_Status2, 3);	// TBD
	cbit(BCM_Status2, 4);	// TBD
	cbit(BCM_Status2, 5);	// TBD
	cbit(BCM_Status2, 6);	// TBD
	cbit(BCM_Status2, 7);	// TBD
	
//---DIG4-----MODULE 상태정보3 ---->통신프로토콜 추가해야함(현재 DIG2까지만 되어있음)
	BCM_Status3 = BCBox_Status;		//박스정보를 3번째로 전송
	BCM_Status4 = 0x00;		


}
void Make_SDTD()
{
	unsigned int tmp1000,tmp100,tmp10,tmp1,tmp;
	
	Make_SD_Fault();		// Status Data(Fault)
	Make_SDTD_Analog();		// Analog data \
	//MCUTEST_Make_SDTD_Analog();
	Make_SDTD_Status();		// Status Data for SD,TD
	//---프로그램 버젼
	tmp1000 = ProgramVersion/1000;
	tmp = ProgramVersion - tmp1000*1000;
	tmp100 = tmp/100;
	tmp = tmp - tmp100*100;
	tmp10 = tmp/10;
	tmp1 = tmp - tmp10*10;
	tmp = (tmp1000<<12) | (tmp100<<8) | (tmp10<<4) | tmp1; //

	//---충전기종류 -- PC화면에서 반대로 표시되어서--->나중에 PC화면 수정예정
	if(WGBC_TC0PC1 == WGBC_TC0){	//---PC화면에서 반대로 표시되어서--->나중에 PC화면 수정예정
		WGBC_TC0PC1_tmp = WGBC_PC1;
	}
	else{
		WGBC_TC0PC1_tmp = WGBC_TC0;
	}
	//--전송데이타 정렬
	CanA_TX_L1 = (((long)BCM_HartBeat & 0xffff) << 16) | tmp;                                 //Hartbeat(16)+programversion(16)
CanA_TX_L1_ver = tmp;
	CanA_TX_H1 = ((long)WGBC_M_ID<<24) | ((long)WGBC_TC0PC1_tmp<<16) | (BCBox_Status<<8) | BCM_Status1; //id(8)+bc종류(8)+box상태(8)+mdstate1(8)

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
// CANA COMM BOARD SDR에 응답(SD)
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
		CanA_SDR_flag = 0;	// 전체 송신 프레임 전송 완료
	}
	else {
		;;	
	}

}


//========================================================
// CANA COMM BOARD TRACE DATA CLEAR에 응답()
//========================================================
void TX_CANA_To_CommBoard_TDCLEAR()
{
	CanA_TX_L0 = 0x30000000;
	CanA_TX_H0 = 0x00000000;
	SendDataToECanA(WGBC_M_ID, 0x08, CanA_TX_L0, CanA_TX_H0);
}
//========================================================
// CANA COMM BOARD RTC UPDATE에 응답()
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
		CanA_SDR_flag = 1;	// COMM보드로부터 CANA SDR 받았음
	}
	else if(CanA_PC_Cmd == 2)
	{
		CanA_TDR_flag = 1;	// COMM보드로부터 CANA TDR 받았음
		cana_TDPy_idx = 0;
		TDPx_idx = 0;
	}
	else if(CanA_PC_Cmd == 3)
	{
		CanA_TD_CLEAR_flag = 1;	// COMM보드로부터 CANA CLEAR 받았음
		//CanA_PC_Udata1;
	}
	else if(CanA_PC_Cmd == 4)
	{
		CanA_RTC_UPGrade_flag = 1;	// COMM보드로부터 CANA RTC DATA 받았음
	}
	else if(CanA_PC_Cmd == 10)
	{	//---프로그램 업그레이드 기능은 나중에 구현..
		CanA_FW_UPGrade_flag = 1;	// COMM보드로부터 CANA 프로그램 업그레이드 받았음
	}
}


void RX_CANA_From_CommBoard()
{
	MCU_D34_ON();	//D34;
//MCU_D34_TOGGLE();	//D34;	
	CANR_HB = WGcana_rx_low_data >> 16;
	CANR_ID = (WGcana_rx_low_data >> 8)&0xff;
	CANR_BCN = (WGcana_rx_low_data)&0xff;
	

//	if(WGcana_rx_id == 1){	// 내꺼만 받음
	if(WGcana_rx_id == WGBC_M_ID)
	{	// 내꺼만 받음
		CanA_RX_L1 = WGcana_rx_low_data;
		CanA_RX_H1 = WGcana_rx_high_data;

		CanA_PC_Command_chk();		
	}
}


//==========================================================================//
// 기  능 : Trace data can 전송(pc<-->Comm보드<-->mcu)								
// 입력 : 보낼 폴트카운터(tdFaultCounterTotal)
// 내부계산 : tdpage(전송할 폴트카운터로부터 계산한 nvram에 저장된 폴트페이지
//            tdpage_fidx(폴트페이지의 폴트발생 시점)
//            TDPy_idx(저장된 td를 폴트발생시점을 중간으로 환산하여 정렬해서 전송)
// 출력 : 
// 총 전송 데이타 : 1000 * 4 * 8 = 32000바이트
// 통신전송 횟수 = 총 전송데이타 / 8바이트 = 32000바이트/8바이트 = 4000번
// 추가로 시간데이타 1번, CRC 1번해서 총 4002번
//==========================================================================//
void TX_CANA_To_CommBoard_TD_OLD()
{
	unsigned int tdpage = 0;

	unsigned int tdpage_fidxL = 1;
	unsigned int tdpage_fidxH = 1;
	unsigned int tdpage_fidx = 1;
	
	tdFaultCounterTotal = CanA_PC_Udata1;	// TD 요청페이지
	
	tdpage = TDP_Index_Search(tdFaultCounterTotal)-1;//누적폴트카운터로 트레이스 폴트페이지 인덱스 새로 계산
	tdpage_fidxL = PWORD(ADD_NVR_CDP0 + 20 + tdpage*2)&0xff;		// 폴트발생시점(L)
	tdpage_fidxH = (PWORD(ADD_NVR_CDP0 + 21 + tdpage*2)<<8)&0xff;	// 폴트발생시점(H)
	tdpage_fidx = (tdpage_fidxH<<8)|tdpage_fidxL;					// 폴트발생시점(HL)

	if(cana_TDPy_idx < TDPy_N){//총 1000*4*8 =  320000바이트
		if((cana_TDPy_idx == 0)&&(TDPx_idx == 0))
		{
			CanA_TX_CRC = 0;
		}
		//===폴트시점을 중간으로 정렬하여 전송
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
		{	// 전체 32kbyte 다 보냈으면 폴트발생 시간 보냄
		CaTX_a1 = PWORD(ADD_NVR_TDP0 + ((long)tdpage * TDP_SIZE) + TDPy_N*TDPx_N + 0)&0xff;//년도 하위 10, 1자리
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
	{	// 마지막으로 CRC보냄
		SendDataToECanA(WGBC_M_ID, 0x08, CanA_TX_CRC, CanA_TX_CRC);		
		cana_TDPy_idx = 0;	
		CanA_TDR_flag = 0;	// 전체 송신 프레임 전송 완료
	}
	else
	{
		cana_TDPy_idx = 0;	
		CanA_TDR_flag = 0;	// 전체 송신 프레임 전송 완료	
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
	
	tdFaultCounterTotal = CanA_PC_Udata1;	// TD 요청페이지
	
	tdpage = TDP_Index_Search(tdFaultCounterTotal)-1;//누적폴트카운터로 트레이스 폴트페이지 인덱스 새로 계산

	tdpage_fidxL = PWORD(ADD_NVR_CDP0 + 20 + tdpage*2)&0xff;		// 폴트발생시점(L)
	tdpage_fidxH = (PWORD(ADD_NVR_CDP0 + 21 + tdpage*2))&0xff;	// 폴트발생시점(H)
	tdpage_fidx = (tdpage_fidxH<<8)|tdpage_fidxL;					// 폴트발생시점(HL)

	if(cana_TDPy_idx < TDPy_N){//총 1000*4*8 =  320000바이트
		if((cana_TDPy_idx == 0)&&(TDPx_idx == 0))
		{
			CanA_TX_CRC = 0;
		}
		//===폴트시점을 중간으로 정렬하여 전송
		if(cana_TDPy_idx < (TDPy_N/2 - tdpage_fidx - 1))
		{
			TDPy_idx = (TDPy_N/2 + tdpage_fidx + 1) + cana_TDPy_idx;	
		}
		else{
			TDPy_idx = (((TDPy_N/2 + tdpage_fidx + 1) + cana_TDPy_idx)-TDPy_N)%TDPy_N;			
		}

		//===start idx 찿기=====
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
		//===td idx 찿기=====
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
	{	// 전체 32kbyte 다 보냈으면 폴트발생 시간 보냄

		CaTX_a1 = PWORD(ADD_NVR_TDP0 + ((long)tdpage * TDP_SIZE) + TDPy_N*TDPx_N + 0)&0xff;//년도 하위 10, 1자리
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
	{	// 마지막으로 CRC보냄
		SendDataToECanA(WGBC_M_ID, 0x08, CanA_TX_CRC, CanA_TX_CRC);		
		cana_TDPy_idx = 0;	
		CanA_TDR_flag = 0;	// 전체 송신 프레임 전송 완료
	}

	else
	{
		cana_TDPy_idx = 0;	
		CanA_TDR_flag = 0;	// 전체 송신 프레임 전송 완료	
	}
}
//======================================================================//
// 기  능 : CAN1,2 테스트                    										
// CANA에서 보내고 CANB에서 받는거확인
//========================================================================//
void Can_Routine(void)
{	
	if(CAN_TEST_MODE == 0)
	{	// 정상모드(MCU CANA ---> COMM BOARD(MASTER))
		if(CanA_SDR_flag)
		{
			TX_CANA_To_CommBoard_SD();			// 정상적 SD 통신반응(MCU(CANA) ---> COMM)
		}
		else if(CanA_TDR_flag)
		{
			TX_CANA_To_CommBoard_TD();			// Trace data 전송(MCU(CANA) ---> COMM)
		}
		else if(CanA_TD_CLEAR_flag)
		{
			Nvram_TDpage_clear();				// Trace data clear(from PC-->Comm보드 -->MCU)
			//TX_CANA_To_CommBoard_TDCLEAR();		// TRACE DATA 지우고 다 지웠다고 Comm보드에 응답
		}
		else if(CanA_RTC_UPGrade_flag)
		{
			RTC_Write();						// RTC DATA 갱신(from 차량 --> comm보드 --> MCU)
			//TX_CANA_To_CommBoard_RTCUPGRADE();
		}
		else if(CanA_FW_UPGrade_flag)
		{
			CanA_FW_UPGrade_flag = 0;				// MCU Firmware Upgrade(from PC-->Comm보드 -->MCU)
		}	
	}
}

//==============================================================//
//==폴트카운庫� 폴트저장페이지인덱스 찿음
//  in : tmpfcnt(저장할 페이지 인덱스 또는 저장된 폴트페이지 인덱스
//  out : tmpfpindex(찿은 폴트페이지 인덱스)
//  저장페이지 --- 0~2(3개)페이지는 순환안됨(0:처음발생폴트,1:2번째,2:3번째) --> clear하기전까지 고정으로 저장
//  저장페이지 --- 3~14(9개)페이지는 순환(링버퍼로 저장)
//==============================================================//
unsigned int TDP_Index_Search(unsigned int tmpfcnt)
{
	unsigned int fpage_Total_N = 15;//총 폴트페이지 수량 9페이지
	unsigned int fpage_RBuf_StartIdx = 3;//링버퍼 시작 인덱스
	unsigned int fpage_RBuf_N = 12;//링버퍼 수량(3,4,5,6,7,8,9,10,11,12,13,14)
	unsigned int tmpfpindex;//

	fpage_RBuf_N = (fpage_Total_N) - fpage_RBuf_StartIdx; // 15-3 = 12;
	
	if(tmpfcnt < fpage_Total_N) tmpfpindex = tmpfcnt;//14까지, 15면 3페이지에 저장
	else{
		tmpfpindex = tmpfcnt- fpage_RBuf_N*((tmpfcnt-fpage_RBuf_StartIdx)/fpage_RBuf_N); // 링버퍼 방식의 트레이스데이타 저장인덱스 계산
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
