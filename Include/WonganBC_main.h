//==================================================================================//
// ������ ������(30kw)/����(50kw) ������ ������ ���(10kw) ���α׷�	           		//
//==================================================================================//
// main header file														//
// DATE : 2014. 11. 21            �����ۼ�				  by PACTECH R&D LSH		//
//==================================================================================//


//==================================================================================//
#ifndef __NTHS_MAIN__   
#define __NTHS_MAIN__   

//***************************************************************//

//===============================================================//

//============================================
// ��������
//============================================
var Uint16 	MainEpldData;
var Uint16 	Sen1EpldData;

//var Uint16	boostConPwmModulation;

var Uint16	dcdcPwmModulation;

//==================================================================================//
// DATE : 2014. 11. 21            						  by PACTECH R&D LSH		//
//==================================================================================//
//==�ý��� ���ݰ���===

var Uint16 ProgramVersion;// 

var float	InputVolRATED;			// ����Է����� ����
var float	InputCurRATED;			// ����Է����� ����
var float	DclinkVolRATED;			// ���dclink���� ����
var float	Bat1OutCurRATED;		// �����������1 ����
var float	Bat2OutCurRATED;		// �����������2 ����

var float	BoxOutCurRATED;			// box��ü �������
var float	BoxBat1OutCurRATED;		// box��ü ����(bat)����1
var float	BoxBat2OutCurRATED;		// box��ü ����(bat)����2

var float	OutPowerRATED;
var float	OutVolRATED;
var float	OutCurRATED;







//--------------------------------------------------------------------
var float 	InputVol;			//����Է�����
var float 	InputVolLPF;		
var float	InputVolSWOVF;
var float	InputVolHWOVF;
var float	InputVolMAX;
var float	InputVolMIN;
var float	InputVolUVF;

var float 	DcLinkVol;			//��� DCV,	DC-LINK ����, RATED 670Vdc
var float 	DcLinkVolLPF;
var float	DclinkVolHWOVF;	
var float	DclinkVolSWOVF;	
var float	DclinkVolMAX;
var float	DclinkVolMIN;
var float	DclinkVolLKON;
var float	DclinkVolUVF;
	//--- ������� ���� 720Vdc(72~83)
var float 	OutVol;				
var float 	OutVolLPF;			
var float	OutVolSWOVF;
var float	OutVolHWOVF;
var float	OutVolMAX;
var float	OutVolMIN;
var float	OutVolUVF;

	//--- ����Է����� (???)
var float 	InputCur;		//�Է�����
var float 	InputCurLPF;			//
var float	InputCurSWOCF;			//
var float	InputCurHWOCF;			//
var float	InputCurMAX;
var float	InputCurMIN;
	//--- ������� ���� 800Adc
var float 	OutCur;		//DCOC	DC�������
var float 	OutCurLPF;		//DCOC	DC�������
var float	OutCurHWOCF;
var float 	OutCurSWOCF;
var float	OutCurMAX;
var float	OutCurMIN;
	//--- �����������1 (???)
var float 	Bat1OutCur;				//
var float 	Bat1OutCurLPF;			//
var float 	Bat1OutCurHWOCF;
var float 	Bat1OutCurSWOCF;
	//--- �����������2 (???)
var float 	Bat2OutCur;				//
var float 	Bat2OutCurLPF;			//
var float 	Bat2OutCurHWOCF;
var float 	Bat2OutCurSWOCF;

//---------------------------------------------
// ������ ���ú���
// Nths_Seq.c
//---------------------------------------------
var Uint16	SeqTimer;
var Uint16	SeqTimerSet;
var Uint16	SeqTimer_flag;
var Uint16	FaultRestartTimer;
var Uint16	FaultRestartTimerSet;
var Uint16	FaultRestartTimer_flag;


var Uint16	DCDC_SoftStart_flag;
var Uint16	DCDC_SoftStart_counter;
var float	DCDC_SoftStart_dv;




#define FLAGSET		1
#define FLAGCLR		0

var Uint16	RunOKNG; // Init = APS_RUN_NG(0)
#define RUN_OK	1
#define RUN_NG	0

var Uint16	Sys_Status;	// Init = SYS_INIT_STATE(0)
#define SYS_INIT_STATE			0
#define SYS_CHK_STATE			1
#define SYS_Restart_STATE		2
#define SYS_MajorFault_STATE	3

var Uint16	SEQ_Step;// Init = SEQ_DO_NOTHING_STATE(0)
#define SEQ_DO_NOTHING_STATE	0
#define SEQ_START				1
#define SEQ_CHK_ON				2
#define SEQ_CHKa_ON_Delay		3
#define SEQ_CHKa_CHK			3

#define SEQ_LK_ON				4
#define SEQ_LKa_ON_Delay		5
#define SEQ_LKa_CHK				5
#define SEQ_CHK_OFF				6

#define SEQ_DCDC_PWM_ON			7
#define SEQ_OUTV_CHK			8

#define SEQ_Normal_State		10
#define SEQ_ALLOFF				11
#define SEQ_RESET				12

var Uint16	InputVolAck;		// �Է����а���



//---------------------------------------------
// ��Ʈ ���� ����
//---------------------------------------------
var Uint16 LowFaultAck;		// 1. ����� 
var Uint16 MidFaultAck;		// 2. ���߰���(3���̳� ���ϰ��� 3ȸ�� �߰��� ó���Ǵ� ����)
var Uint16 MidFaultAckold;		// 2. ���߰���(3���̳� ���ϰ��� 3ȸ�� �߰��� ó���Ǵ� ����)
var Uint16 HighFaultAck;	// 3. �߰��� 
//==MidFaultAck BIT ����====
#define MidGDF		0
#define MidINOVF	1
#define MidINOCF	2
#define MidOVOVF	3

#define MidOCOCF	4
#define MidSEQF		5	
#define MidFCOVF	6
#define MidFCOCF	8

#define MidFanf		9
#define MidPSF		7
//	#define MidFanf		9==>����Ʈ �ӽ÷� �߰�����, ������ �߰�����

/*
sbit(MidFaultAck, MidGDF);		// �߰�����(3��3ȸ) - GDF
sbit(MidFaultAck, MidINOVF);	// �߰�����(3��3ȸ) - �Է°�����
sbit(MidFaultAck, MidINOCF);	// �߰�����(3��3ȸ) - �Է°�����
sbit(MidFaultAck, MidOVOVF);	// �߰�����(3��3ȸ) - ��� ������
sbit(MidFaultAck, MidOCOCF);	// �߰�����(3��3ȸ) - ��� ������ ��Ʈ
sbit(MidFaultAck, MidSEQF);	// �߰�����(3��3ȸ) - ��������Ʈ
sbit(MidFaultAck, MidFCOVF);	// �߰�����(3��3ȸ) - FCV(DCLINK VOLTAGE) ������ ��Ʈ
sbit(MidFaultAck, MidFCOCF);	// �߰�����(3��3ȸ) - FCI(DCLINK CURRENT) ������ ��Ʈ
sbit(MidFaultAck, MidPSF);		// �߰�����(3��3ȸ) - �������� ������ ��Ʈ	
*/		
var Uint16 MidFaultMJF;		// 2. ���߰���(3���̳� ���ϰ��� 3ȸ�� �߰��� ó���Ǵ� ����) 
var Uint16 HighFaultMJF;	// 3. �߰��� 
//====MidFaultAck���� ��Ʈ����
// GDF���� ����
var Uint16 MidFCounter0;	//GDF1,2,3,4,5 �߻�Ƚ�� 
var Uint16 MidFTimer01;		//GDFs ù��° ����ð�  
var Uint16 MidFTimer02;		//GDFs �ι�° ����ð�  

var Uint16 MidFCounter1;	//�Է°����� ��Ʈ �߻�Ƚ��
var Uint16 MidFTimer11;		//�Է°����� ��Ʈ ù��° ����ð� 
var Uint16 MidFTimer12;		//�Է°����� ��Ʈ �ι�° ����ð�  

var Uint16 MidFCounter2;	//�Է°����� ��Ʈ �߻�Ƚ��
var Uint16 MidFTimer21;		//�Է°����� ��Ʈ ù��° ����ð�  
var Uint16 MidFTimer22;		//�Է°����� ��Ʈ �ι�° ����ð�  

var Uint16 MidFCounter3;	//��°����� ��Ʈ �߻�Ƚ��
var Uint16 MidFTimer31;		//��°����� ��Ʈ ù��° ����ð� 
var Uint16 MidFTimer32;		//��°����� ��Ʈ �ι�° ����ð�  

var Uint16 MidFCounter4;	//��°����� ��Ʈ �߻�Ƚ��
var Uint16 MidFTimer41;		//��°����� ��Ʈ ù��° ����ð� 
var Uint16 MidFTimer42;		//��°����� ��Ʈ �ι�° ����ð�  

var Uint16 MidFCounter5;	//������ ��Ʈ �߻�Ƚ��
var Uint16 MidFTimer51;		//������ ��Ʈ ù��° ����ð� 
var Uint16 MidFTimer52;		//������ ��Ʈ �ι�° ����ð�    

var Uint16 MidFCounter6;	//FCV(DCLINK VOLTAGE) ������ ��Ʈ �߻�Ƚ��
var Uint16 MidFTimer61;		//FCV(DCLINK VOLTAGE) ������ ��Ʈ ù��° ����ð� 
var Uint16 MidFTimer62;		//FCV(DCLINK VOLTAGE) ������ ��Ʈ �ι�° ����ð�  

var Uint16 MidFCounter7;	//FCI(DCLINK CURRENT) ������ ��Ʈ �߻�Ƚ��
var Uint16 MidFTimer71;		//FCI(DCLINK CURRENT) ������ ��Ʈ ù��° ����ð� 
var Uint16 MidFTimer72;		//FCI(DCLINK CURRENT) ������ ��Ʈ �ι�° ����ð� 

var Uint16 MidFCounter8;	//�������� ������ ��Ʈ �߻�Ƚ��
var Uint16 MidFTimer81;		//�������� ������ ��Ʈ ù��° ����ð� 
var Uint16 MidFTimer82;		//�������� ������ ��Ʈ �ι�° ����ð�  

var Uint16 MidFCounter9;	//�������� ������ ��Ʈ �߻�Ƚ��
var Uint16 MidFTimer91;		//�������� ������ ��Ʈ ù��° ����ð� 
var Uint16 MidFTimer92;		//�������� ������ ��Ʈ �ι�° ����ð�  
//=======================================================//


var Uint16	FaultCounter;
var Uint16	FaultCheckCounter;
var Uint16	FaultCounter_old;
var Uint16	MajorFault;
//---------------------------------------------
// GDU ��Ʈ ���� bit ����
//---------------------------------------------
//--APS/BACH --
var Uint16	FaultGDFBuf;
#define GDF1		0	// APS(siv-U), BACH(dcdc-Leading Leg)
#define GDF2		1	// APS(siv-V), BACH(dcdc-Lagging Leg)
#define GDF3		2	// APS(siv-W), BACH(none)
#define GDF4		3	// APS,BACH (acdc - Leading Leg)
#define GDF5		4	// APS,BACH (acdc - Lagging Leg)
#define GDF6		5	// SPARE
#define GDF7		6	// SPARE
#define GDF8		7	// SPARE

//---------------------------------------------
// HW ��Ʈ ���� bit ����
//---------------------------------------------
var Uint16	FaultHWBuf;


//--20140613---ȸ�ε�--EPLD����
//#define BACH_HWF_DCLINKOVF	3	//DCCT4F = DCLINK OVERVOLTAGE H/W ��Ʈ�߰���
//#define BACH_HWF_ACICOCF	4	//ACCT1F = ACIC, AC LINE �Է�����, ��ȸ��(ACCT)
//#define BACH_HWF_DCLINKOCF	5	//ACCT2F = DCIC,DCLINK ������,  ��ȸ��(DCCT1)
//#define BACH_HWF_OCOCF		6	//ACCT3F = ��� ������,  ��ȸ��(DCCT2)
//#define BACH_HWF_BATOCF		7	//ACCT4F = BAT����������,  ��ȸ��(DCCT3)

#define HWF_BIT0				0	//DCCT1F = ������
#define HWF_IVOVF			1	//DCCT2F = �Է°�����, ��ȸ��(PT1)
#define HWF_FCOVF		2	//DCCT3F = DCLINK������, ��ȸ��(PT2)
#define HWF_OVOVF		3	//DCCT4F = ��°�����, ��ȸ��(PT3)
#define HWF_ICOCF		4	//ACCT1F = �Է°�����,  ��ȸ��(CT1)
#define HWF_IGBTOCF		5	//ACCT2F = IGBT��°�����,  ��ȸ��(CT2)
#define HWF_OCOCF		6	//ACCT3F = ��°�����,  ��ȸ��(CT3)
#define HWF_BIT7		7	//ACCT4F = ������

//---------------------------------------------
// S/W ��Ʈ ���� ���� �� bit ����
//---------------------------------------------
var Uint16 	FaultSWBuf;    //--low byte(SEN1_LED_CS1(), (U22 )), high byte(SEN1_LED_CS2(), (U23 ))

//--BACH --
#define 	SWF_IC_OCF			0	//�Է� ������ ��Ʈ
#define 	SWF_IGBT_OCF		1	//IGBT��� ������ ��Ʈ
#define 	SWF_OC_OCF			2	//������� ��Ʈ/��°�����
#define 	SWF_BIT3			3	//
#define 	SWF_BIT4			4	//SPARE
#define 	SWF_BIT5			5	//SPARE
#define 	SWF_BIT6			6	//SPARE
#define 	SWF_OVLDF			7	//������ ��Ʈ, ����120% 1���̻� ���ӽ�

#define 	SWF_IV_OVF			8	//�Է°�����
#define 	SWF_IV_LVF			9	//�Է�������
#define 	SWF_FCV_OVF			10	//DCLINK������
#define 	SWF_FCV_LVF			11	//DCLINK������
#define 	SWF_OV_OVF			12	//��°�����
#define 	SWF_OV_LVF			13	//���������
#define 	SWF_BIT14			14	//SPARE
#define 	SWF_BIT15			15	//SPARE

var Uint16	FaultSeqBuf;// Init = 0
#define SEQERR_CHKa		0
#define SEQERR_LKa		1
#define SEQERR_SIVKa	2
#define SEQERR_DCUV		3

#define SEQERR_OCVUV	4
#define SEQERR_BIT5		5
#define SEQERR_BIT6		6
#define SEQERR_BIT7		7

var Uint16	FaultEtcBuf;// Init = 0
#define ETCERR_MJF		0
#define ETCERR_MTF		1
#define ETCERR_OPF		2
#define ETCERR_ERF		3

#define ETCERR_FUSEF	4
#define ETCERR_PSF		5
#define ETCERR_FANF		6
#define ETCERR_HSKTHRF	7

#define ETCERR_BATTHRF	8
#define ETCERR_PCDF		8
#define ETCERR_BATF		9
#define ETCERR_BIT10	10
#define ETCERR_BIT11	11

#define ETCERR_BIT12	12
#define ETCERR_BIT13	13
#define ETCERR_BIT14	14
#define ETCERR_BIT15	15


var Uint16 	FaultAck;

var Uint16 	OverLoadAck;	// �����ϸ� 1
var Uint16 	OverLoadFAck;	// ������(OverLoadAck==1)���°� 1���̻� ���ӽ� 1
var Uint32 	OverLoad_Counter;
var Uint32 	THRFAck_Counter;	// ��Ʈ��ũ 95���̻� 1�� ī����

var Uint32 	PSF_Counter;	// PSF �߻��ϸ� ��ٷ� ��Ʈ�߻��Ƚ�Ű��,,, �����ð� ���� ==> �������������п��� ������ ���۽� ��Ʈ���°� ����
                            // ������ �� 100msec�ε�...������ 150msec, �ð��� ��Ʈ�������������̽ð����� ���ж������� ������ ���۾��ϸ� ��.

//============================================================================
// Function Prototype Declaration
//============================================================================	
// Nths_main.c
void System_Define();
void VarialbeUpdate();
void VariableInit();
// GlobalFunc.c
void TimerInterruptEnable();
void PeripheralInit();
void Init_Gpio(void);
void Init_External_Interrupt(void);
Uint16	msec_dif(Uint16 msec1, Uint16 msec2);

// debug_2833x.c
void sci_debug_init();// Initialize SCI-A for data monitoring 


#endif // __NTHS_MAIN__  




