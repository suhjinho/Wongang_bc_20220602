//###############################################################################################################
// ���ϸ�   : I2C_eeprom.c
// ȸ���   : RealSYS
// �ۼ���¥ : 2008.02.21
// �ۼ���   : R.N.K
// ����     : RealSYS DSP28335 DSK�� �ִ� Serial EEPROM(AT24C16) ������ �ʿ��� I2C ���� �Լ� ����
//
// [ ����� ���� ȯ�� ]
// 1) TI CCS3.3�� Service Pack v8.0(CCS_v3.3_SR8_77.exe ����) - 2008.02.21
// 2) CCS3.3 Setup�� Target Driver Package(setupCCSPlatinum_v30323.exe ����) - 2007.12.19
// 3) C2000 Code Generation Tools v5.0 Release(C2000CodeGenerationTools5.0.0.exe ����) - 2008.02.21
//
// [ ������ Software Developer Kit(SDK) ]
// 1) TI�� SDK�� C/C++ Header Files and Peripheral Examples v1.10 ��� - 2008.02.15
//
// [ Revision History ]
//
//###################################################################################

/////////////////////////////////////////////////////////////////////////////////////
// [ AT24C16 ���� Ư�� ]
// 1) 2048(0x800) * 8 Bytes(16K)
// 2) Two-wire serial interface
// 3) 100KHz(1.8V) and 400KHz(2.7V, 5V) in Clock Frequency, SCL
//    Clock Pulse Width Low  : 4.7us(1.8V), 1.2us(2.7V, 5V) - Minimum value
//	  Clock Pulse Width High : 4.0us(1.8V), 0.6us(2.7V, 5V) - Minimum value
// 4) 16 Byte Page Write Mode
// 5) Self-timed Write Cycle(5ms max)
// 6) organized with 128 pages of 16 bytes each, requires an 11-bit data word address
//
// Serial Clock(SCL) 	: is used to positive edge clock data into each EEPROM device
//                     	  and negative edge clock data out of each device
// Serial Data(SDA)  	: is bidirectional for serial data transfer. This pin is open-drain driven.
// Address(A0, A1, A2)	: does not use the device address pins
//
// [ 2 Byte Address ��� ��� - A0, A1 ]
// 1) A0[D7 ~ D4] : ���� 4��Ʈ�� 1010
// 2) A0[D3 ~ D1] : �ֻ��� 3��Ʈ Address�� ���
// 3) A0[D0]      : 1�̸� Read ���, 0�̸� Write ���
// 4) A1[D7 ~ D0] : ���� 8��Ʈ Address�� ���   
///////////////////////////////////////////////////////////////////////////////

#include "Global_Variable_Function.h"


// Initialize I2C for serial eeprom(AT24C16) access
void Init_I2C_eeprom(void){
	// Setting GPIO for I2C
	EALLOW;
	/* Enable internal pull-up for the selected pins */
	GpioCtrlRegs.GPBPUD.bit.GPIO32 = 0;    // Enable pull-up for GPIO32 (SDAA)
	GpioCtrlRegs.GPBPUD.bit.GPIO33 = 0;	   // Enable pull-up for GPIO33 (SCLA)
	
	/* Set qualification for selected pins to asynch only */
	GpioCtrlRegs.GPBQSEL1.bit.GPIO32 = 3;  // Asynch input GPIO32 (SDAA)
	GpioCtrlRegs.GPBQSEL1.bit.GPIO33 = 3;  // Asynch input GPIO33 (SCLA)
	
	/* Configure I2C pins using GPIO regs*/
	// This specifies which of the possible GPIO pins will be I2C functional pins.
	GpioCtrlRegs.GPBMUX1.bit.GPIO32 = 1;   // Configure GPIO32 for SDAA operation
	GpioCtrlRegs.GPBMUX1.bit.GPIO33 = 1;   // Configure GPIO33 for SCLA operation
	EDIS;
	
	// Initialize I2C interface
	// Put I2C module in the reset state
	I2caRegs.I2CMDR.bit.IRS = 0;
	
	// Set Slave Address according to AT24C16 device
	I2caRegs.I2CSAR = 0x0050;

	// Set I2C module clock input
	I2caRegs.I2CPSC.all = 14; // need 7-12 Mhz on module clk (150/15 = 10MHz)//14
	
	// 400KHz clock speed in SCL for master mode(2.5us period)
	// Clock Pulse Width Low  : 1.5us
	// Clock Pulse Width High : 1.0us
	// F28335�� I2C User Guide���� Master Clock �ֱ⸦ ���ϴ� ������ �����ٶ�.
	I2caRegs.I2CCLKL = 20;  //10
	I2caRegs.I2CCLKH = 10;   //5
	
	// Disable I2C interrupt
	I2caRegs.I2CIER.all = 0x0;
	
	// Enable TX and RX FIFO in I2C module
	I2caRegs.I2CFFTX.all = 0x6000; // Enable FIFO mode and TXFIFO
	I2caRegs.I2CFFRX.all = 0x2040; // Enable RXFIFO, clear RXFFINT

	// Enable I2C module
	I2caRegs.I2CMDR.all = 0x20; 
}

// refer to single byte write mode of AT24C16
void Write_EEPROM_Data(WORD addr, BYTE data){
WORD slave_addr, data_addr;
	
	slave_addr = 0x50 | ((addr >> 8) & 0x7);
	data_addr = addr & 0xff;
	 
	// Wait until the STP bit is cleared from any previous master communication.
	while(I2caRegs.I2CMDR.bit.STP == 1);
	
	// Wait until bus-free status
	while(I2caRegs.I2CSTR.bit.BB == 1);
	
	// Set Device(Slave) Address
	I2caRegs.I2CSAR = slave_addr;
	
	// Setup number of bytes to send
	I2caRegs.I2CCNT = 2;

	// Setup data to send
	I2caRegs.I2CDXR = data_addr; // Data Address
	I2caRegs.I2CDXR = data;		 // Write Data
	
	// Send start as master transmitter with STT(=1), STP(=1), XA(=0), RM(=0) 
	I2caRegs.I2CMDR.all = 0x6E20;
	
	// Wait until STOP condition is detected and clear STOP condition bit
	while(I2caRegs.I2CSTR.bit.SCD == 0);
	I2caRegs.I2CSTR.bit.SCD = 1;

	// Wait the Write-Cycle Time for EEPROM
	// refer to AT29C16 datasheet
	delay_ms(10);
}


// refer to single byte random read mode of AT24C16
void Read_EEPROM_Data(WORD addr, BYTE* pData){
WORD slave_addr, data_addr;
	
	slave_addr = 0x50 | ((addr >> 8) & 0x7);
	data_addr = addr & 0xff;
	 
	// Wait until the STP bit is cleared from any previous master communication.
	while(I2caRegs.I2CMDR.bit.STP == 1);
	
	
	// Wait until bus-free status
	while(I2caRegs.I2CSTR.bit.BB == 1);
	
	// Set Device(Slave) Address
	I2caRegs.I2CSAR = slave_addr;
	
	// Setup number of bytes to send
	I2caRegs.I2CCNT = 1;

	// Setup data to send
	I2caRegs.I2CDXR = data_addr; // Data Address
	
	// Send start as master transmitter with STT(=1), STP(=0), XA(=0), RM(=0) 
	I2caRegs.I2CMDR.all = 0x2620;
	
	// Wait until ARDY status bit is set
	while(I2caRegs.I2CSTR.bit.ARDY == 0);
	
	// Wait until the STP bit is cleared
	while(I2caRegs.I2CMDR.bit.STP == 1);
	
	// Set Device(Slave) Address
	I2caRegs.I2CSAR = slave_addr;
	
	// Setup number of bytes to read
	I2caRegs.I2CCNT = 1;
	
	// Send start as master receiver with STT(=1), STP(=1), XA(=0), RM(=0) 
	I2caRegs.I2CMDR.all = 0x2C20;
	
	// Wait until STOP condition is detected and clear STOP condition bit
	while(I2caRegs.I2CSTR.bit.SCD == 0);
	I2caRegs.I2CSTR.bit.SCD = 1;
	
	*pData = (BYTE)(I2caRegs.I2CDRR & 0xff);  //f
}


void WriteEEpromWord(BYTE addr,WORD d)
{
	BYTE r;

	r = (d >> 8);  //0���� ����
	Write_EEPROM_Data(addr++,r);

	r = d & 0x00ff; //1���� ����
	Write_EEPROM_Data(addr,r);
	

}

void ReadEEpromWord(BYTE addr, WORD *d)
{
	BYTE r;

	*d = 0;
	r = 0;
	
	Read_EEPROM_Data(addr++,&r);
	*d |= ((WORD)r<<8);

	r = 0;
	Read_EEPROM_Data(addr,&r);
	*d |= r;


}

