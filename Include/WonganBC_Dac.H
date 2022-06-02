#ifndef __NTHS_DAC_H
#define __NTHS_DAC_H

//============================================================================
// DAC
//============================================================================	
//==============================================================
var Uint16  tst_DACa;
var Uint16  tst_DACb;

var Uint16  tst_DAC1;
var Uint16  tst_DAC2;
var Uint16  tst_DAC3;
var Uint16  tst_DAC4;
var Uint16  tst_DAC5;
var Uint16  tst_DAC6;
var Uint16  tst_DAC7;
var Uint16  tst_DAC8;
var Uint16  tst_DAC9;
var Uint16  tst_DAC10;
var Uint16  tst_DAC11;
var Uint16  tst_DAC12;
var Uint16  tst_DAC13;
var Uint16  tst_DAC14;
var Uint16  tst_DAC15;
var Uint16  tst_DAC16;
var Uint16  tst_DAC17;
var signed int  tst_DAC1offset;
var signed int	tst_DAC2offset;
var signed int	tst_DAC3offset;
var signed int	tst_DAC4offset;
var signed int	tst_DAC5offset;
var signed int	tst_DAC6offset;
var signed int	tst_DAC7offset;
var signed int	tst_DAC8offset;
var signed int	tst_DAC9offset;
var signed int	tst_DAC10offset;
var signed int	tst_DAC11offset;
var signed int	tst_DAC12offset;
var signed int	tst_DAC13offset;
var signed int	tst_DAC14offset;
var signed int	tst_DAC15offset;
var signed int	tst_DAC16offset;



var Uint16	sine_value;



void Init_Spi_Dac();
void Tcard_DAC_Out(WORD ch, WORD dat);
void Tcard_Sequence_Out();		// sequence 동작시험용 출력
void InitTestVariable();



#endif


