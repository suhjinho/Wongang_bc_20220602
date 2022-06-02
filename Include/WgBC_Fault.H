#ifndef __NTHS_FAULT_H
#define __NTHS_FAULT_H

//===========================================================
// 함수 선언
//===========================================================
void FaultLevelSetting();

void Check_SWF();
void Check_HWF();
void Check_GDF();
void Check_ETF();
void Check_TESTF();


void FaultProcess_int();
void FaultClear();

void SWF_LatchGEN();
void SWF_LatchCLR();
void MidiumFault_3min3N_Check_sub(Uint16 *FCounter, Uint16 *FTimer1, Uint16 *FTimer2, Uint16 *Fmjf);
void MidiumFault_3min3N_Check();
void MidiumFault_Count_INC();
void MidiumFault_3min3N_INIT();
#endif
