//==================================================================================//
// ������ ������(30kw)/����(50kw) ������ ������ ���(10kw) ���α׷�	           		//
//==================================================================================//
// main header file														//
// DATE : 2014. 11. 21            �����ۼ�				  by PACTECH R&D LSH		//
//==================================================================================//


//==================================================================================//
#ifndef __NTHS_MAIN__   
#define __NTHS_MAIN__   


//============================================
// ��������
//============================================


var float Theta_radU1;
var float Theta_radV1;
var float Theta_radW1;

var float Theta_inc;

var float sineU1;
var float sineV1;
var float sineW1;

var float sineU2;
var float sineV2;
var float sineW2;

var float siv_test_Vrefin1;
var float siv_test_Vrefin2;
var float siv_test_Vrefin1_Scale;
var float siv_test_Vrefin2_Scale;


var Uint16	PWM_FLAG;

//==================================================================================//
// DATE : 2014. 11. 21            						  by PACTECH R&D LSH		//
//==================================================================================//
#define ProgramMode_BOX0	0
#define ProgramMode_MODULE1	1
#define ProgramMode_CPU2	2
var Uint16 ProgramMode_B0M1C2;	//20150404�߰�

//==�ý��� ���ݰ���===

var Uint16 ProgramVersion;// 
	
//--������ ���� ����	
#define WGBC_TC0	0
#define WGBC_PC1	1
var  unsigned int WGBC_TC0PC1;

//--10KW ��� Number(ID)����
//--�ڽ�����Ŀ�� ���� ���ʺ��� 1..
//--������BC�� ���ʺ��� 1,2,3   ����BC�� ���ʺ��� 1,2,3,4,5
#define WGBC_M_ID1		1
#define WGBC_M_ID2		2
#define WGBC_M_ID3		3
#define WGBC_M_ID4		4
#define WGBC_M_ID5		5
//var  int WGBC_M_ID;
var  long WGBC_M_ID;

//--------------------------------------------------------------------
//����Է����� --> ADCRD10
var float 	MInputVol;
var float 	MInputVolLPF;		
var float 	MInputVolRATED;			//�ý�������(670[V])
var float	MInputVolMAX;			//�ý�������(720[V])
var float	MInputVolMIN;			//�ý�������(620[V])
var float	MInputVolSWOVF;
var float	MInputVolHWOVF;
var float	MInputVolUVF;
//��� DCLINK����(RATED 670Vdc) --> ADCRD11
var float 	MDcLinkVol;	
var float 	MDcLinkVolLPF;
var float	MDclinkVolRATED;		
var float	MDclinkVolMAX;
var float	MDclinkVolMIN;
var float	MDclinkVolSWOVF;	
var float	MDclinkVolHWOVF;	
var float	MDclinkVolUVF;
var float	TSTVAR11;
//����������1(OBD�Ĵ�) ���� 720Vdc(72~83) --> ADCRD9
var float 	MBatVol;
var float 	MBatVolLPF;			
var float	MBatVolRATED;
var float	MBatVolMAX;
var float	MBatVolMIN;
var float	MBatVolSWOVF;
var float	MBatVolHWOVF;
var float	MBatVolUVF;
//����������1(OBD����) ���� 720Vdc(72~83) --> ADCRD8
var float 	MOutVol;
var float 	MOutVolLPF;			
var float	MOutVolRATED;
var float	MOutVolMAX;
var float	MOutVolMIN;
var float	MOutVolSWOVF;
var float	MOutVolHWOVF;
var float	MOutVolUVF;
//��� �������� ���� 72Vdc(72~83) --> ADCRD7
var float 	MCtrlVol;
var float 	MCtrlVolLPF;			
var float	MCtrlVolRATED;
var float	MCtrlVolMAX;
var float	MCtrlVolMIN;
var float	MCtrlVolSWOVF;
var float	MCtrlVolHWOVF;
var float	MCtrlVolUVF;	
//��� �Է����� --> ADCRD0
var float 	MInputCur;
var float 	MInputCurLPF;
var float	MInputCurRATED;
var float	MInputCurMAX;
var float	MInputCurSWOCF;
var float	MInputCurHWOCF;	
//box ������� --> ADCRD6
var float 	BOutCur;
var float 	BOutCurLPF;	
var float	BOutCurRATED;
var float	BOutCurMAX;
var float	BOutCurHWOCF;
var float 	BOutCurSWOCF;
//��� �������� --> ADCRD1
var float 	MBatCur;				
var float 	MBatCurLPF;
var float 	MBatCurRATED;
var float 	MBatCurMAX;
var float 	MBatCurHWOCF;
var float 	MBatCurSWOCF;
//��� ������� --> ADCRD2
var float 	MOutCur;				
var float 	MOutCurLPF;
var float 	MOutCurRATED;
var float 	MOutCurMAX;
var float 	MOutCurHWOCF;
var float 	MOutCurSWOCF;
//box BAT����1 --> ADCRD4
var float 	BBatCur1;				
var float 	BBatCur1LPF;			
var float 	BBatCur1RATED;
var float 	BBatCur1MAX;
var float 	BBatCur1HWOCF;
var float 	BBatCur1SWOCF;
//box BAT����2 --> ADCRD5 (���� BC�� ������)
var float 	BBatCur2;				
var float 	BBatCur2LPF;			
var float 	BBatCur2RATED;
var float 	BBatCur2MAX;
var float 	BBatCur2HWOCF;
var float 	BBatCur2SWOCF;
//box BAT���� --> ������ BC =BAT1+BAT2, ���� BC = BAT1
var float 	BBatCur;				
var float 	BBatCurLPF;			
var float 	BBatCurRATED;
var float 	BBatCurMAX;
var float 	BBatCurSWOCF;

//box �������� --> box�������-box��������(BAT1+BAT2)
var float 	BLoadCur;				
var float 	BLoadCurLPF;			
var float 	BLoadCurRATED;
//module �������� --> module�������-module��������
var float 	MLoadCur;				
var float 	MLoadCurLPF;			
var float 	MLoadCurRATED;


//=====Acknowledge ���� ����

#define Mode_CV0	0
#define Mode_CC1	1
var Uint16 	Mode_CV0CC1;// CVCC ���

var Uint16 	MODEAck;//dsp select switch, module mode ����ġ�� ���
var Uint16	MInputVolAck;		// �Է����а���
var Uint16 	FaultAck;
var Uint16 	FaultAckOld;
var Uint16 	ModuleFault_NUM;	// 10kW ��� ���ͼ���
var Uint16 	Pall_OK_ModuleN;	// 10kW ��� ���ͼ���

var Uint16 	RESETAck;
var Uint16 	RESETAck_OLD;
var Uint16 	BATOC1Ack;
var Uint16 	BATOC2Ack;
var Uint16 	BCSTARTAck;

var Uint16 	M1FAck;
var Uint16 	M2FAck;
var Uint16 	M3FAck;
var Uint16 	M4FAck;
var Uint16 	M5FAck;
var Uint16 	MFAck_MY;
var Uint16 	ModuleFault2ea_flag;

var Uint16 	BCModeAck;
var Uint16 	CHKAck;
var Uint16 	LKAck;
var Uint16 	BC72VAck;
var Uint16 	TEMP85SWAck;
var Uint16 	FANF1Ack;
var Uint16 	FANF2Ack;
var Uint16 	FUSEAck;

var Uint16 	BCSTATUSAck;
var Uint16 	MOUTV_OKAck;///

var Uint16 	FanOnOffCmd_flag;
//---------------------------------------------
// ������ ���ú���
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

var Uint16	RunOKNG; // Init = RUN_NG(0)
#define RUN_OK	1
#define RUN_NG	0

//var Uint16	RunOKNG; // Init = RUN_NG(0)
//--MODEAck
#define TESTMODE0	0
#define NORMALMODE1	1

var Uint16	Seq_Status;	// Init = SeqStatus_INIT_STATE0(0)
#define SeqStatus_Init0			0
#define SeqStatus_Check1		1
#define SeqStatus_Restart2		2
#define SeqStatus_MajorFault3	3

var Uint16	SEQ_Step;// Init = SEQ_DO_NOTHING_STATE(0)
#define SEQ_DO_NOTHING_STATE	0
#define SEQ_START				1
#define SEQ_CHK_ON				2
#define SEQ_CHKa_CHK			3

#define SEQ_LK_ON				4
#define SEQ_LKa_CHK				5
#define SEQ_CHK_OFF				6

#define SEQ_PWM_ON				7
#define SEQ_OUTV_CHK			8

#define SEQ_Normal_State		9
#define SEQ_ALLOFF				10
#define SEQ_RESET				11


//---------------------------------------------
// ��Ʈ ���� ����
//---------------------------------------------
var Uint16 LowFaultAck;		// 1. ����� 
var Uint16 MidFaultAck;		// 2. ���߰���(3���̳� ���ϰ��� 3ȸ�� �߰��� ó���Ǵ� ����)
var Uint16 HighFaultAck;	// 3. �߰��� 
//==MidFaultAck BIT ����====
#define MidGDF		0
#define MidMICOCF	1
#define MidMOCOCF	2

//#define MidOVOVF	3
//#define MidINOVF	1
//#define MidSEQF		5	
//#define MidFCOVF	6
//#define MidFCOCF	8
//#define MidFanf		9
//#define MidPSF		7
//	#define MidFanf		9==>����Ʈ �ӽ÷� �߰�����, ������ �߰�����

/*
sbit(MidFaultAck, MidGDF);		// �߰�����(3��3ȸ) - GDF
sbit(MidFaultAck, MidINOVF);	// �߰�����(3��3ȸ) - �Է°�����
sbit(MidFaultAck, MidMICOCF);	// �߰�����(3��3ȸ) - �Է°�����
sbit(MidFaultAck, MidOVOVF);	// �߰�����(3��3ȸ) - ��� ������
sbit(MidFaultAck, MidMOCOCF);	// �߰�����(3��3ȸ) - ��� ������ ��Ʈ
sbit(MidFaultAck, MidSEQF);	// �߰�����(3��3ȸ) - ��������Ʈ
sbit(MidFaultAck, MidFCOVF);	// �߰�����(3��3ȸ) - FCV(DCLINK VOLTAGE) ������ ��Ʈ
sbit(MidFaultAck, MidFCOCF);	// �߰�����(3��3ȸ) - FCI(DCLINK CURRENT) ������ ��Ʈ
sbit(MidFaultAck, MidPSF);		// �߰�����(3��3ȸ) - �������� ������ ��Ʈ	
*/		
var Uint16 MidFaultMJF;		// 2. ���߰���(3���̻ ���ϰ��� 3ȸ�� �߰��� ó���Ǵ� ����) 
var Uint16 HighFaultMJF;	// 3. �߰��� 
//====MidFaultAck���� ��Ʈ����
// GDF���� ����
var Uint16 MidFCounter0;	//GDF1,2 �߻�Ƚ�� 
var Uint16 MidFTimer01;		//GDFs ù��° ����ð�  
var Uint16 MidFTimer02;		//GDFs �ι�° ����ð�  

var Uint16 MidFCounter1;	//�Է°����� ��Ʈ �߻�Ƚ��
var Uint16 MidFTimer11;		//�Է°����� ��Ʈ ù��° ����ð� 
var Uint16 MidFTimer12;		//�Է°����� ��Ʈ �ι�° ����ð�  

var Uint16 MidFCounter2;	//�����°����� ��Ʈ �߻�Ƚ��
var Uint16 MidFTimer21;		//�����°����� ��Ʈ ù��° ����ð�  
var Uint16 MidFTimer22;		//�����°����� ��Ʈ �ι�° ����ð�  
/*
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
var Uint16 MidFTimer42;		//�°����� ��Ʈ �ι�° ����ð�  

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
*/
//=======================================================//



var Uint16	FaultCounter;
var Uint16	FaultCheckCounter;
var Uint16	FaultCounter_old;
var Uint16	MajorFault;
//---------------------------------------------
// GDU ��Ʈ ���� bit ����
//---------------------------------------------
var Uint16	FGDF;
#define GDF1		0	// Leading Leg
#define GDF2		1	// dcdc-Lagging Leg
#define GDF3		2	// ������
#define GDF4		3	// ������

//---------------------------------------------
// HW ��Ʈ ���� bit ����
//---------------------------------------------
var Uint16	FHWF;

#define HWF0_ICOCF		0	//�Է°�����
#define HWF1_MBATOCF	1	//��� �������� ������
#define HWF2_MOCF		2	//��� ������� ������
#define HWF3_SPAREF		3	//�����

#define HWF4_BBAT1OCF	4	//BOX ��������1 ������
#define HWF5_BBAT2OCF	5	//BOX ��������2 ������
#define HWF6_BOCF		6	//BOX ������� ������
#define HWF7_CPOVF		7	//�������� ������

#define HWF8_OVOVF		8	//������� ������(OBD��)
#define HWF9_BVOVF		9	//������� ������(OBD��)
#define HWF10_IVOVF		10	//�Է����� ������
#define HWF11_FCOVF		11	//DCLINK���� ������


//---------------------------------------------
// S/W ��Ʈ ���� ���� �� bit ����
//---------------------------------------------
var Uint16 	FSWF;   
#define SWF0_ICOCF		0	//�Է°�����
#define SWF1_MBATOCF	1	//��� �������� ������
#define SWF2_MOCF		2	//��� ������� ������
#define SWF3_SPAREF		3	//�����

#define SWF4_BBAT1OCF	4	//BOX ��������1 ������
#define SWF5_BBAT2OCF	5	//BOX ��������2 ������
#define SWF6_BOCF		6	//BOX ������� ������
#define SWF7_CPOVF		7	//�������� ������

#define SWF8_OVOVF		8	//������� ������(OBD��)
#define SWF9_BVOVF		9	//������� ������(OBD��)
#define SWF10_IVOVF		10	//�Է����� ������
#define SWF11_FCOVF		11	//DCLINK���� ������

#define SWF12_OVUVF		12	//������� ������(OBD��,�� ����) 50V
#define SWF13_IVUVF		13	//�Է����� ������ 620V
#define SWF14_CPUVF		14	//�������� ������ 50V
#define SWF15_OVUVF2	15	//������� ������(OBD�ĸ� ) 50V. 20150922 ������� ������(������)�� ����͸����� ǥ���ϰ�

var Uint16	FSQF;// Init = 0
#define SEQF_CHKa		0
#define SEQF_LKa		1
#define SEQERR_DCUV		3

#define SEQERR_OCVUV	4
#define SEQERR_BIT5		5
#define SEQERR_BIT6		6
#define SEQERR_BIT7		7

var Uint16	FETF;
#define ETCF_MJF		0
#define ETCF_FUSEF		1
#define ETCF_PSF		2
#define ETCF_FANF1		3

#define ETCF_FANF2		4
#define ETCF_HSKTHRF	5
#define	ETCF_NCARGING	6
//#define ETCF_BATOC1		6
//#define ETCF_BATOC2		7


var Uint16	FBXF;
#define BOXF_BMJF		0
#define BOXF_BATOC1		1
#define BOXF_BATOC2		2

var Uint16	FLGDF;
var Uint16	FLHWF;
var Uint16	FLSWF;
var Uint16	FLSQF;
var Uint16	FLETF;
var Uint16	FLBXF;


var Uint16 	OverLoadAck;	// �����ϸ� 1
var Uint16 	OverLoadFAck;	// ������(OverLoadAck==1)���°� 1���̻� ���ӽ� 1
var Uint32 	OverLoad_Counter;
var Uint32 	THRFAck_Counter;	// ��Ʈ��ũ 95���̻� 1�� ī����
var Uint32 	FANF1Ack_Counter;	// ����Ʈ1 ī���� 10��
var Uint32 	FANF2Ack_Counter;	// ����Ʈ2 ī���� 10��

var Uint32 	PSF_Counter;	// PSF �߻��ϸ� ��ٷ� ��Ʈ�߻��Ƚ�Ű��,,, �����ð� ���� ==> �������������п��� ������ ���۽� ��Ʈ���°� ����
                            // ������ �� 100msec�ε�...������ 150msec, �ð��� ��Ʈ�������������̽ð����� ���ж������� ������ ���۾��ϸ� ��.
                            
//========CAN ���ú���

var unsigned int CANR_HB;
var unsigned int CANR_ID;
var unsigned int CANR_BCN; 
          
var unsigned int cantxidx;
var unsigned int canrxidx;
var long CanA_TX_CRC;
var long CanA_RX_CRC;
var unsigned int CanB_TX_CRC;
var unsigned int CanB_RX_CRC;

var long CanA_TX_L0;
var long CanA_TX_L1;
var long CanA_TX_L2;
var long CanA_TX_L3;
var long CanA_TX_L4;
var long CanA_TX_L5;

var long CanA_TX_H0;
var long CanA_TX_H1;
var long CanA_TX_H2;
var long CanA_TX_H3;
var long CanA_TX_H4;
var long CanA_TX_H5;

var long CanB_TX_L1;
var long CanB_TX_L2;
var long CanB_TX_L3;
var long CanB_TX_L4;
var long CanB_TX_H1;
var long CanB_TX_H2;
var long CanB_TX_H3;
var long CanB_TX_H4;

var long CanA_RX_L1;
var long CanA_RX_L2;
var long CanA_RX_L3;
var long CanA_RX_L4;
var long CanA_RX_H1;
var long CanA_RX_H2;
var long CanA_RX_H3;
var long CanA_RX_H4;

var long CanB_RX_L1;
var long CanB_RX_L2;
var long CanB_RX_L3;
var long CanB_RX_L4;
var long CanB_RX_H1;
var long CanB_RX_H2;
var long CanB_RX_H3;
var long CanB_RX_H4;

var unsigned int  CaTX_a1;
var unsigned int  CaTX_a2;
var unsigned int  CaTX_a3;
var unsigned int  CaTX_a4;
var unsigned int  CaTX_a5;
var unsigned int  CaTX_a6;
var unsigned int  CaTX_a7;
var unsigned int  CaTX_a8;
var unsigned int  CaTX_a9;
var unsigned int  CaTX_a10;
var unsigned int  CaTX_a11;
var unsigned int  CaTX_a12;
var unsigned int  CaTX_a13;
var unsigned int  CaTX_a14;
var unsigned int  CaTX_a15;
var unsigned int  CaTX_a16;
var unsigned int  CaTX_a17;
var unsigned int  CaTX_a18;
var unsigned int  CaTX_a19;
var unsigned int  CaTX_a20;
var unsigned int  CaTX_a21;
var unsigned int  CaTX_a23;
var unsigned int  CaTX_a24;
var unsigned int  CaTX_a25;
var unsigned int  CaTX_a26;


var unsigned int BCM_HartBeat;
var unsigned int BCBox_Status;
var unsigned int BCM_Status1;
var unsigned int BCM_Status2;
var unsigned int BCM_Status3;
var unsigned int BCM_Status4;

var unsigned int BCM_TDF1;
var unsigned int BCM_TDF2;
var unsigned int BCM_TDF3;
var unsigned int BCM_TDF4;  
             
var unsigned int BCM_SDF1;
var unsigned int BCM_SDF2;
var unsigned int BCM_SDF3;
var unsigned int BCM_SDF4;               
                
                
// eCAN-A/B ó���� ���� ���� ------------------------------------------------
var BOOL WGcana_rx_flag;	// ����ڰ� CAN-A ���Ź��� ����Ÿ ��������(0), ���� ����ƴ�(1)
var LONG WGcana_rx_id;			// CAN-A ���� ID
var BYTE WGcana_rx_length;    	// CAN-A ���� ������ ũ��
var LONG WGcana_rx_low_data;	// CAN-A ���� Low Word Data
var LONG WGcana_rx_high_data;	// CAN-A ���� High Word Data

var BOOL WGcanb_rx_flag;	// ����ڰ� CAN-A ���Ź��� ����Ÿ ��������(0), ��� ����ƴ�(1)
var LONG WGcanb_rx_id;			// CAN-A ���� ID
var BYTE WGcanb_rx_length;    	// CAN-A ���� ������ ũ��
var LONG WGcanb_rx_low_data;	// CAN-A ���� Low Word Data
var LONG WGcanb_rx_high_data;	// CAN-A ���� High Word Data



//============================================================================
// Function Prototype Declaration
//============================================================================	
// WgBc_Main.c
void System_Define();
void Setting_Chk();
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

void BoardTest(unsigned char boardname);
void siv_test_init();

#endif // __NTHS_MAIN__  




