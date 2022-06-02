#ifndef GLOBAL__VARIABLE_FUNCTION
#define	GLOBAL__VARIABLE_FUNCTION


// 변수 선언을 한번만 하기 위해서 메크로 사용
#ifdef GLOBAL__VARIABLE_FUNCTION_H
#define	var
#else
#define	var	extern
#endif


#include "C28x_FPU_FastRTS.h"
#include "Define.h"
#include "DSP2833x_Device.h"     // Headerfile Include File

#include "WgBC_Dio.h"
#include "WgBC_EPWM.h"
#include "WgBC_ADC.h"   
#include "WgBC_Dac.h"   
#include "WgBC_DcDcConverter.h"
#include "WgBC_main.h"
#include "WgBC_Fault.h"
#include "WgBC_Seq.h"
#include "WgBC_monitoring.h"
#include "WgBC_Isr.h"
#include "WgBC_MemoryMap.h"
#include "WgBC_Controller.h"
#include "WgBC_EEprom.h"


#include "WgBC_eCAN.h"
#include "WgBC_SCI_BC.h"


#endif



