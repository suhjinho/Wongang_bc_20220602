/*
 * FW_Download.h
 *
 *  Created on: 2015. 3. 23.
 *      Author: Administrator
 */

#ifndef FW_DOWNLOAD_H_
#define FW_DOWNLOAD_H_

#define FW_BUFFER_SIZE	1024

void Firmware_Update(void);
unsigned int GetWordNvram(void);
unsigned long GetLongNvram(void);
void InRomUnlock(void);



#endif /* FW_DOWNLOAD_H_ */
