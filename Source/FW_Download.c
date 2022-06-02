/*
 * FW_Download.c
 *
 *  Created on: 2015. 3. 23.
 *      Author: Administrator
 */
#include "DSP28x_Project.h"							/* Device Headerfile and Examples Include File */
#include "Flash2833x_API_Library.h"
#include "Device.h"
#include "FW_Download.h"


unsigned int Fw_writebuf[FW_BUFFER_SIZE];
volatile unsigned char	*pFw_data;
extern 	Uint32	FW_Size;

#pragma CODE_SECTION(Firmware_Update,"ramfuncs");
#pragma CODE_SECTION(GetWordNvram,"ramfuncs");
#pragma CODE_SECTION(GetLongNvram,"ramfuncs");

void Firmware_Update(void)
{
	FLASH_ST stFlashStatus;
	unsigned int i, SectorMask, WriteCnt, *pFlash_write_addr;

	// 동작되는 모든 프로세스들을 종료 시킨다.
	//DINT;

	pFw_data = (volatile unsigned char *)(NVRAM_FW_BINARY_ADDR);

	Flash_CPUScaleFactor = SCALE_FACTOR;
	Flash_CallbackPtr = 0;

	//GpioDataRegs.GPBSET.bit.GPIO61 = 1;				//LED On
	GpioDataRegs.GPBCLEAR.bit.GPIO61 = 1;				//LED Off

	InRomUnlock();
/*
	Status = CsmUnlock();
	if(Status != STATUS_SUCCESS)
	{
		// Error 처리
		GpioDataRegs.GPBSET.bit.GPIO61 = 1;				//LED On
	   return;
	}
*/
	DINT;

	SectorMask = SECTORA | SECTORB | SECTORC | SECTORD;

	if(Flash_Erase(SectorMask, &stFlashStatus) != STATUS_SUCCESS)
	{
		// Error 처리
		while(1)
		{
			//GpioDataRegs.GPBSET.bit.GPIO61 = 1;				//LED On
		}
	}

	if(GetWordNvram() != 0x08AA)			// Read Key Value 0x08AA
	{
		while(1)
		{
		}
	}

	GpioDataRegs.GPBSET.bit.GPIO61 = 1;				//LED On

	// Read Reserved Data 8 words
	for(i=0; i<8; i++)
		GetWordNvram();

	GetLongNvram();							// Read Entry Point Address	4 Bytes

	while(FW_Size > 0)
	{
		WriteCnt = GetWordNvram();			// Count of word data for write
		pFlash_write_addr = (unsigned int *)(GetLongNvram());

		while(WriteCnt > FW_BUFFER_SIZE)
		{
			for(i=0; i<FW_BUFFER_SIZE; i++)
				Fw_writebuf[i] = GetWordNvram();

			if((Flash_Program(pFlash_write_addr, Fw_writebuf, FW_BUFFER_SIZE, &stFlashStatus) != STATUS_SUCCESS) ||
				(Flash_Verify(pFlash_write_addr, Fw_writebuf, FW_BUFFER_SIZE, &stFlashStatus) != STATUS_SUCCESS))
			{
				// Error 처리
				while(1)
				{
				}
			}
			GpioDataRegs.GPBTOGGLE.bit.GPIO61 = 1;
			pFlash_write_addr += FW_BUFFER_SIZE;
			WriteCnt -= FW_BUFFER_SIZE;
		}

		for(i=0; i<WriteCnt; i++)
			Fw_writebuf[i] = GetWordNvram();

		if((Flash_Program(pFlash_write_addr, Fw_writebuf, WriteCnt, &stFlashStatus) != STATUS_SUCCESS) ||
			(Flash_Verify(pFlash_write_addr, Fw_writebuf, WriteCnt, &stFlashStatus) != STATUS_SUCCESS))
		{
			// Error 처리
			while(1)
			{
			}
		}
		GpioDataRegs.GPBTOGGLE.bit.GPIO61 = 1;
	}

	GpioDataRegs.GPBSET.bit.GPIO61 = 1;				//LED On
    // Enable watchdog
	EALLOW;
    SysCtrlRegs.WDCR= 0x0028;
    EDIS;

    while(1)
    {
    }
}

//-----------------------------------------------------
unsigned int GetWordNvram(void)
{
	unsigned int uiRetWord;

	uiRetWord  = (*pFw_data++ & 0xFF) << 8;
	uiRetWord |= (*pFw_data++ & 0xFF);

	if(FW_Size >= 2)
		FW_Size -= 2;
	else
		FW_Size = 0;

	return(uiRetWord);
}

//-----------------------------------------------------
unsigned long GetLongNvram(void)
{
	unsigned long ulRetLong;

	ulRetLong  =  (unsigned long)(GetWordNvram()) << 16;
	ulRetLong |= GetWordNvram();

	return ulRetLong;
}

void InRomUnlock(void)
{
 	// Read the password locations - this will unlock the
  	// CSM only if the passwords are erased.  Otherwise it
  	// will not have an effect.
	CsmPwl.PSWD0;
  	CsmPwl.PSWD1;
  	CsmPwl.PSWD2;
  	CsmPwl.PSWD3;
  	CsmPwl.PSWD4;
  	CsmPwl.PSWD5;
  	CsmPwl.PSWD6;
  	CsmPwl.PSWD7;
}

