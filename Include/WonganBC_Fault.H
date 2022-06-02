#ifndef __NTHS_FAULT_H
#define __NTHS_FAULT_H

//===========================================================
// 함수 선언
//===========================================================
void FaultLevelSetting();
void SW_FaultProcess();
void HW_FaultProcess();
void GDU_FaultProcess();
void FaultProcess();

void SW_FaultProcess_int();
void HW_FaultProcess_int();
void GDU_FaultProcess_int();
void ETC_FaultProcess_int();
void FaultProcess_int();
void FaultClear();
void MidiumFault_3min3N_Check_sub(Uint16 *FCounter, Uint16 *FTimer1, Uint16 *FTimer2, Uint16 *Fmjf);
void MidiumFault_3min3N_Check();
void MidiumFault_Count_INC();
void MidiumFault_3min3N_INIT();
#endif
