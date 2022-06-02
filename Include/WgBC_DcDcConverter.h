//==================================================================================//
// ������ Battery Charger(30kw, 50kw)  DC/DC Converter Controller           		//
//==================================================================================//
// BC Main header file															//
// DATE : 2015. 01. 7            						  by PACTECH R&D LSH		//
//==================================================================================//

#ifndef __NTHS_BACH_DCDCCONTROLLER__   
#define __NTHS_BACH_DCDCCONTROLLER__   

//========BC dc/dc controller ���ú�������=======//
void DcDcController();
void InitControlVar();
void SoftStart();
//=================================================//
//===��� ������� �������====
var float	V_Kp;
var float	V_Ki;
var float	OutVolRef;		// ��������(SoftStart+���Ŀ���+������������)--->��������� �Է�
var float	OutVolSSRef;	// ��������(SoftStart����)

var float	OutVolRefSet;	// �������� 
var float	OutVolPI;	    // pi����� ���
var float	OutVolI;
var float	OutVolErr;
var float	OutVolP;
//===��� ������� �������====
var float	I_Kp;
var float	I_Ki;
var float	OutPutCur_Ka;
var float	OutPutVol_Ka;
var float	V_ref;
var float	I_ref;
var float	OutCurLimit;
var float	V_Duty_Uint;


var float	OutCurRef;	// �������� 
var float	OutCurPI;	    // pi����� ���
var float	OutCurI;
var float	OutCurP;
var float	OutCurErr;


//===��� �������� ���Ѱ���====
var float	MBatCur_Kp;
var float	MBatCur_Ki;
var float	MBatCurRef;	// ��� ������������ 
var float	MBatCurPI;	    // pi����� ���
var float	MBatCurI;
var float	MBatCurErr;
var float	MBatCurLimit;
//===�ڽ� �������� ���Ѱ���====
var float	BBatCur_Kp;
var float	BBatCur_Ki;
var float	BBatCurRef;	// �ڽ� ������������ 
var float	BBatCurPI;	    // pi����� ���
var float	BBatCurP;	    // pi����� ���
var float	BBatCurI;
var float	BBatCurErr;
var float	BBatCurLimit;	
//===��Ÿ�������

var float	OutVol_KI_damp;
var float	DutyLimit;


//---���� �ڽ����� ��߶��� ����===
var float	UnbalPI;	    //
var float	UnbalI;
var float	UnbalCur_Err;

var float	DutyRatio;
var Uint16	DutyCount;

var Uint16	Vcontrol_Counter;	// acdc��������� ī����(10�� ���������ϸ� ��������� 1��)

void DcDcController_WGBC_PALL_VI();
void DcDcController_WGBC_PALL();
void WGBC_DCDC_Controller_20150522();
//----------------------------------//	

#endif //  




