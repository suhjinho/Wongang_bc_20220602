#ifndef __MONITORING_DPRAM__    
#define __MONITORING_DPRAM__    

var	Uint16  test_dpramdata1;
var	Uint16  test_dpramdata2;
var	Uint16  test_dpramdata3;

var float 	WHM1KW;
void WHM();
void DpramWrite();
void DpramRead();
void DpramWriteDataRemake();
void DpramTestToFromTCMS();
void DpramWriteAnalogData();
void MonCardIrqGen(unsigned int irqN);
#endif // __MONITORING_DPRAM__
