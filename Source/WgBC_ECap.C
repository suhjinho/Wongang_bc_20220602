#include "Global_Variable_Function.h"



void Config_bcPhaseShiftLower()
{
	// Gpio for the ECap initialization
	InitECapGpio();
	//===============================================
	// CAP1 PWM Leading leg 윗상 PWM
	// CAP2 PWM Leading leg 아랫상 PWM
	//-----------------------------------------------
	// CAP3 PWM Lagging leg 윗상 PWM
	// CAP4 PWM Lagging leg 아랫상 PWM
	//===============================================
	// CAP1 PWM
    ECap1Regs.ECCTL2.bit.CAP_APWM 	= EC_APWM_MODE;		// Enable APWM mode   
    //ECap1Regs.ECCTL2.bit.SWSYNC 	= EC_ENABLE;
	ECap1Regs.ECCTL2.bit.APWMPOL 	= EC_ACTV_HI;		// Active High  
	
   	ECap1Regs.ECCTL2.bit.SYNCI_EN 	= EC_ENABLE;		// No sync in for master
   	ECap1Regs.ECCTL2.bit.SYNCO_SEL 	= EC_SYNCIN;		// CTR=PRD event to be the sync-out signal      
   	ECap1Regs.CAP1 = APWM_TIMER_PRD;			// Set Period value
   	ECap1Regs.CTRPHS = 0;						// make eCAP1 reference phase = zero
   	ECap1Regs.CAP2 = (Uint32)(APWM_TIMER_PRD/2)-DEAD_TIME_SHIFT;// Set Compare value
   	
   	ECap1Regs.ECCLR.all = 0x0FF;				// Clear pending interrupts
   	ECap1Regs.ECEINT.bit.CTR_EQ_CMP = 1; 		// enable Compare Equal Int
   	
   	
   	// CAP2 PWM
   	ECap2Regs.ECCTL2.bit.CAP_APWM 	= EC_APWM_MODE;		// Enable APWM mode 
   	//ECap2Regs.ECCTL2.bit.SWSYNC 	= EC_ENABLE;                   
   	ECap2Regs.ECCTL2.bit.APWMPOL 	= EC_ACTV_LO;		// Active Low                         
   	ECap2Regs.ECCTL2.bit.SYNCI_EN 	= EC_ENABLE;		// No sync in for master                   
   	ECap2Regs.ECCTL2.bit.SYNCO_SEL 	= EC_SYNCIN;		// sync "flow-through 
   	// 데드타임 주기 위해서 cap1 pwm보다 위상을 5us 앞서게 했다.
   	ECap2Regs.CAP1 = APWM_TIMER_PRD;			// Set Period value
   	// rising 시 데드타임
   	ECap2Regs.CTRPHS = DEAD_TIME_SHIFT;			//  make eCAP1 reference phase = zero
   	// falling시 데드타임
   	// 데드타임 2배 이유는 데드타임 만큼 이동 하였고, 폴링에지에서 데드타임 주기 위해서
   	ECap2Regs.CAP2 = (Uint16)(APWM_TIMER_PRD/2)+DEAD_TIME_SHIFT;// Set Compare value
   	
   	ECap2Regs.ECCLR.all = 0x0FF;				// Clear pending interrupts
   	ECap1Regs.ECEINT.bit.CTR_EQ_CMP = 1; // enable Compare Equal Int
   	
   	// CAP3 PWM                                                                                        
   	ECap3Regs.ECCTL2.bit.CAP_APWM 	= EC_APWM_MODE;		// Enable APWM mode                        	
   	//ECap2Regs.ECCTL2.bit.SWSYNC 	= EC_ENABLE;                                                   
   	ECap3Regs.ECCTL2.bit.APWMPOL 	= EC_ACTV_HI;		// Active Low                              
   	ECap3Regs.ECCTL2.bit.SYNCI_EN 	= EC_ENABLE;		// No sync in for master                   
   	ECap3Regs.ECCTL2.bit.SYNCO_SEL 	= EC_SYNCIN;		// sync "flow-through                      
   	// 데드타임 주기 위해서 cap1 pwm보다 위상을 5us 앞서게 했다.                                   
   	ECap3Regs.CAP1 = APWM_TIMER_PRD;			// Set Period value                                
   	// rising 시 데드타임                                                                          
   	ECap3Regs.CTRPHS = APWM_TIMER_PRD_HALF-APWM_TIMER_PRD_HALF;			//  make eCAP1 reference phase = zero                          
   	// falling시 데드타임                                                                          
   	// 데드타임 2배 이유는 데드타임 만큼 이동 하였고, 폴링에지에서 데드타임 주기 위해서            
   	ECap3Regs.CAP2 = (Uint16)(APWM_TIMER_PRD/2)-DEAD_TIME_SHIFT;// Set Compare value            
   	                                                                                               
   	ECap3Regs.ECCLR.all = 0x0FF;				// Clear pending interrupts                        
   	ECap1Regs.ECEINT.bit.CTR_EQ_CMP = 1; // enable Compare Equal Int  
   	
   	// CAP4 PWM                                                                                    
   	ECap4Regs.ECCTL2.bit.CAP_APWM 	= EC_APWM_MODE;		// Enable APWM mode                        
   	//ECap2Regs.ECCTL2.bit.SWSYNC 	= EC_ENABLE;                                                   
   	ECap4Regs.ECCTL2.bit.APWMPOL 	= EC_ACTV_LO;		// Active Low                              
   	ECap4Regs.ECCTL2.bit.SYNCI_EN 	= EC_ENABLE;		// No sync in for master 
   	ECap4Regs.ECCTL2.bit.SYNCO_SEL 	= EC_SYNCIN;		// sync "flow-through                      
   	// 데드타임 주기 위해서 cap1 pwm보다 위상을 5us 앞서게 했다.                                   
   	ECap4Regs.CAP1 = APWM_TIMER_PRD;					// Set Period value                                
   	// rising 시 데드타임                                                                          
   	ECap4Regs.CTRPHS = DEAD_TIME_SHIFT;								//  make eCAP1 reference phase = zero                          
   	// falling시 데드타임                                                                          
   	// 데드타임 2배 이유는 데드타임 만큼 이동 하였고, 폴링에지에서 데드타임 주기 위해서            
   	ECap4Regs.CAP2 = (Uint16)(APWM_TIMER_PRD/2)+DEAD_TIME_SHIFT;// Set Compare value            
   	                                                                                               
   	ECap4Regs.ECCLR.all = 0x0FF;						// Clear pending interrupts                        
   	ECap1Regs.ECEINT.bit.CTR_EQ_CMP = 1; 				// enable Compare Equal Int  
   	  	
   	// Start counters
   	ECap1Regs.ECCTL2.bit.TSCTRSTOP = EC_RUN;
   	ECap2Regs.ECCTL2.bit.TSCTRSTOP = EC_RUN;
   	ECap3Regs.ECCTL2.bit.TSCTRSTOP = EC_RUN;
   	ECap4Regs.ECCTL2.bit.TSCTRSTOP = EC_RUN;
} 
void PwmUpDate_bcPhaseShiftLower(Uint16 PhaseShift, Uint16 DeadTime)
{  
	// 데드타임으로 출력 카운트 값 제한
	if(PhaseShift <= DeadTime)	PhaseShift = DeadTime;
	else if(PhaseShift >= (APWM_TIMER_PRD_HALF-DeadTime)) PhaseShift = (APWM_TIMER_PRD_HALF-DeadTime);
		
	ECap3Regs.CTRPHS = APWM_TIMER_PRD-PhaseShift;
	ECap4Regs.CTRPHS = APWM_TIMER_PRD-PhaseShift +DeadTime;    
}
void InitECapGpio()
{   
    
   InitECap1Gpio();
   InitECap2Gpio();
   InitECap3Gpio();
   InitECap4Gpio();
   InitECap5Gpio();
   InitECap6Gpio();

}   
    
void InitECap1Gpio(void)
{   
   EALLOW;
/* Enable internal pull-up for the selected pins */
// Pull-ups can be enabled or disabled by the user.
// This will enable the pullups for the specified pins.
// Comment out other unwanted lines.

// GpioCtrlRegs.GPAPUD.bit.GPIO5 = 0;      // Enable pull-up on GPIO5 (CAP1)
   GpioCtrlRegs.GPAPUD.bit.GPIO24 = 0;     // Enable pull-up on GPIO24 (CAP1)
// GpioCtrlRegs.GPBPUD.bit.GPIO34 = 0;     // Enable pull-up on GPIO34 (CAP1)


// Inputs are synchronized to SYSCLKOUT by default.
// Comment out other unwanted lines.

// GpioCtrlRegs.GPAQSEL1.bit.GPIO5 = 0;    // Synch to SYSCLKOUT GPIO5 (CAP1)
   GpioCtrlRegs.GPAQSEL2.bit.GPIO24 = 0;   // Synch to SYSCLKOUT GPIO24 (CAP1)
// GpioCtrlRegs.GPBQSEL1.bit.GPIO34 = 0;   // Synch to SYSCLKOUT GPIO34 (CAP1)

/* Configure eCAP-1 pins using GPIO regs*/
// This specifies which of the possible GPIO pins will be eCAP1 functional pins.
// Comment out other unwanted lines.

// GpioCtrlRegs.GPAMUX1.bit.GPIO5 = 3;     // Configure GPIO5 as CAP1
   GpioCtrlRegs.GPAMUX2.bit.GPIO24 = 1;    // Configure GPIO24 as CAP1
// GpioCtrlRegs.GPBMUX1.bit.GPIO34 = 1;    // Configure GPIO24 as CAP1

    EDIS;
}


void InitECap2Gpio(void)
{
   EALLOW;
/* Enable internal pull-up for the selected pins */
// Pull-ups can be enabled or disabled by the user.
// This will enable the pullups for the specified pins.
// Comment out other unwanted lines.

//   GpioCtrlRegs.GPAPUD.bit.GPIO7 = 0;     // Enable pull-up on GPIO7 (CAP2)
 GpioCtrlRegs.GPAPUD.bit.GPIO25 = 0;    // Enable pull-up on GPIO25 (CAP2)
// GpioCtrlRegs.GPBPUD.bit.GPIO37 = 0;    // Enable pull-up on GPIO37 (CAP2)

// Inputs are synchronized to SYSCLKOUT by default.
// Comment out other unwanted lines.

//   GpioCtrlRegs.GPAQSEL1.bit.GPIO7 = 0;    // Synch to SYSCLKOUT GPIO7 (CAP2)
 GpioCtrlRegs.GPAQSEL2.bit.GPIO25 = 0;   // Synch to SYSCLKOUT GPIO25 (CAP2)
// GpioCtrlRegs.GPBQSEL1.bit.GPIO37 = 0;   // Synch to SYSCLKOUT GPIO37 (CAP2)

/* Configure eCAP-2 pins using GPIO regs*/
// This specifies which of the possible GPIO pins will be eCAP2 functional pins.
// Comment out other unwanted lines.

//   GpioCtrlRegs.GPAMUX1.bit.GPIO7 = 3;    // Configure GPIO7 as CAP2
 GpioCtrlRegs.GPAMUX2.bit.GPIO25 = 1;   // Configure GPIO25 as CAP2
// GpioCtrlRegs.GPBMUX1.bit.GPIO37 = 3;   // Configure GPIO37 as CAP2

    EDIS;
}

void InitECap3Gpio(void)
{
   EALLOW;

/* Enable internal pull-up for the selected pins */
// Pull-ups can be enabled or disabled by the user.
// This will enable the pullups for the specified pins.
// Comment out other unwanted lines.

//   GpioCtrlRegs.GPAPUD.bit.GPIO9 = 0;      // Enable pull-up on GPIO9 (CAP3)
 	GpioCtrlRegs.GPAPUD.bit.GPIO26 = 0;     // Enable pull-up on GPIO26 (CAP3)

// Inputs are synchronized to SYSCLKOUT by default.
// Comment out other unwanted lines.

//   GpioCtrlRegs.GPAQSEL1.bit.GPIO9 = 0;    // Synch to SYSCLKOUT GPIO9 (CAP3)
 	GpioCtrlRegs.GPAQSEL2.bit.GPIO26 = 0;   // Synch to SYSCLKOUT GPIO26 (CAP3)

/* Configure eCAP-3 pins using GPIO regs*/
// This specifies which of the possible GPIO pins will be eCAP3 functional pins.
// Comment out other unwanted lines.

//   GpioCtrlRegs.GPAMUX1.bit.GPIO9 = 3;     // Configure GPIO9 as CAP3
	GpioCtrlRegs.GPAMUX2.bit.GPIO26 = 1;    // Configure GPIO26 as CAP3

    EDIS;
}

void InitECap4Gpio(void)
{
   EALLOW;

/* Enable internal pull-up for the selected pins */
// Pull-ups can be enabled or disabled by the user.
// This will enable the pullups for the specified pins.
// Comment out other unwanted lines.

//   GpioCtrlRegs.GPAPUD.bit.GPIO11 = 0;   // Enable pull-up on GPIO11 (CAP4)
 	GpioCtrlRegs.GPAPUD.bit.GPIO27 = 0;   // Enable pull-up on GPIO27 (CAP4)

// Inputs are synchronized to SYSCLKOUT by default.
// Comment out other unwanted lines.

//   GpioCtrlRegs.GPAQSEL1.bit.GPIO11 = 0; // Synch to SYSCLKOUT GPIO11 (CAP4)
	GpioCtrlRegs.GPAQSEL2.bit.GPIO27 = 0; // Synch to SYSCLKOUT GPIO27 (CAP4)

/* Configure eCAP-4 pins using GPIO regs*/
// This specifies which of the possible GPIO pins will be eCAP4 functional pins.
// Comment out other unwanted lines.

//   GpioCtrlRegs.GPAMUX1.bit.GPIO11 = 3;  // Configure GPIO11 as CAP4
	GpioCtrlRegs.GPAMUX2.bit.GPIO27 = 1;  // Configure GPIO27 as CAP4

    EDIS;
}

void InitECap5Gpio(void)
{
   EALLOW;

/* Enable internal pull-up for the selected pins */
// Pull-ups can be enabled or disabled by the user.
// This will enable the pullups for the specified pins.
// Comment out other unwanted lines.

//   GpioCtrlRegs.GPAPUD.bit.GPIO3 = 0;     // Enable pull-up on GPIO3 (CAP5)
 	GpioCtrlRegs.GPBPUD.bit.GPIO48 = 0;    // Enable pull-up on GPIO48 (CAP5)

// Inputs are synchronized to SYSCLKOUT by default.
// Comment out other unwanted lines.

//   GpioCtrlRegs.GPAQSEL1.bit.GPIO3 = 0;  // Synch to SYSCLKOUT GPIO3 (CAP5)
 	GpioCtrlRegs.GPBQSEL2.bit.GPIO48 = 0; // Synch to SYSCLKOUT GPIO48 (CAP5)

/* Configure eCAP-5 pins using GPIO regs*/
// This specifies which of the possible GPIO pins will be eCAP5 functional pins.
// Comment out other unwanted lines.

//   GpioCtrlRegs.GPAMUX1.bit.GPIO3 = 2;   // Configure GPIO3 as CAP5
	GpioCtrlRegs.GPBMUX2.bit.GPIO48 = 1;  // Configure GPIO48 as CAP5

    EDIS;
}

void InitECap6Gpio(void)
{
   EALLOW;

/* Enable internal pull-up for the selected pins */
// Pull-ups can be enabled or disabled by the user.
// This will enable the pullups for the specified pins.
// Comment out other unwanted lines.

//   GpioCtrlRegs.GPAPUD.bit.GPIO1 = 0;     // Enable pull-up on GPIO1 (CAP6)
 	GpioCtrlRegs.GPBPUD.bit.GPIO49 = 0;    // Enable pull-up on GPIO49 (CAP6)

// Inputs are synchronized to SYSCLKOUT by default.
// Comment out other unwanted lines.

//   GpioCtrlRegs.GPAQSEL1.bit.GPIO1 = 0;  // Synch to SYSCLKOUT GPIO1 (CAP6)
 	GpioCtrlRegs.GPBQSEL2.bit.GPIO49 = 0; // Synch to SYSCLKOUT GPIO49 (CAP6)

/* Configure eCAP-5 pins using GPIO regs*/
// This specifies which of the possible GPIO pins will be eCAP6 functional pins.
// Comment out other unwanted lines.

//   GpioCtrlRegs.GPAMUX1.bit.GPIO1 = 2;   // Configure GPIO1 as CAP6
	GpioCtrlRegs.GPBMUX2.bit.GPIO49 = 1;  // Configure GPIO49 as CAP6

    EDIS;
}


